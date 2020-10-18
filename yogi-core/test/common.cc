/*
 * This file is part of the Yogi Framework
 * https://github.com/yohummus/yogi-framework.
 *
 * Copyright (c) 2020 Johannes Bergmann.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <test/common.h>

#include <src/api/constants.h>
#include <src/data/crypto.h>
#include <src/network/messages.h>
#include <src/objects/logger.h>

#include <boost/asio.hpp>
#include <boost/filesystem.hpp>

#include <fstream>
#include <sstream>

namespace fs    = boost::filesystem;
namespace asio  = boost::asio;
namespace ip    = asio::ip;
namespace uuids = boost::uuids;
using tcp       = ip::tcp;
using udp       = ip::udp;

namespace {

void setup_logging(int verbosity) {
  auto time_fmt = constants::kDefaultTimeFormat;
  auto log_fmt  = constants::kDefaultLogFormat;

  // For calls through the public API
  int res = YOGI_ConfigureConsoleLogging(YOGI_VB_TRACE, YOGI_ST_STDERR, YOGI_TRUE, time_fmt, log_fmt);
  EXPECT_OK(res);

  res = YOGI_LoggerSetComponentsVerbosity("Yogi\\..*", verbosity, nullptr);
  EXPECT_OK(res);

  // For direct calls, circumventing the public API (needed because of separate shared library and test memory spaces)
  Logger::configure_console_logging(verbosity, YOGI_ST_STDERR, YOGI_TRUE, time_fmt, log_fmt);
  Logger::set_components_verbosity("Yogi\\..*", verbosity);
}

}  // anonymous namespace

TestFixture::TestFixture() {
  // Verbose logging if --gtest_filter is set
  auto filter = testing::FLAGS_gtest_filter;
  if (!filter.empty() && filter != "*" && filter != "*.*") {
    setup_logging(YOGI_VB_TRACE);
  }
}

TestFixture::~TestFixture() {
  EXPECT_OK(YOGI_DestroyAll());

  YOGI_ConfigureConsoleLogging(YOGI_VB_NONE, 0, 0, nullptr, nullptr);
  YOGI_ConfigureHookLogging(YOGI_VB_NONE, nullptr, nullptr);
  YOGI_ConfigureFileLogging(YOGI_VB_NONE, nullptr, nullptr, 0, nullptr, nullptr);
}

TemporaryWorkdirGuard::TemporaryWorkdirGuard() {
  temp_path_ = fs::temp_directory_path() / fs::unique_path();
  fs::create_directory(temp_path_);
  old_working_dir_ = fs::current_path();
  fs::current_path(temp_path_);
}

TemporaryWorkdirGuard::TemporaryWorkdirGuard(TemporaryWorkdirGuard&& other) {
  old_working_dir_ = other.old_working_dir_;
  temp_path_       = other.temp_path_;
  other.temp_path_.clear();
}

TemporaryWorkdirGuard::~TemporaryWorkdirGuard() {
  if (!temp_path_.empty()) {
    fs::current_path(old_working_dir_);
    fs::remove_all(temp_path_);
  }
}

MulticastSocket::MulticastSocket(const udp::endpoint& multicast_ep)
    : mc_ep_(multicast_ep), socket_(ioc_, mc_ep_.protocol()) {
  socket_.set_option(udp::socket::reuse_address(true));
  socket_.bind(udp::endpoint(mc_ep_.protocol(), mc_ep_.port()));

  auto ifs  = get_filtered_network_interfaces({"localhost"}, mc_ep_.protocol());
  auto addr = ifs[0].addresses[0];

  if (addr.is_v6()) {
    socket_.set_option(ip::multicast::join_group(mc_ep_.address().to_v6(), addr.to_v6().scope_id()));
    socket_.set_option(ip::multicast::outbound_interface(static_cast<unsigned int>(addr.to_v6().scope_id())));
  } else {
    socket_.set_option(ip::multicast::join_group(mc_ep_.address().to_v4(), addr.to_v4()));
  }
}

void MulticastSocket::send(const Buffer& msg) {
  socket_.send_to(asio::buffer(msg), mc_ep_);
}

std::pair<ip::address, Buffer> MulticastSocket::receive(const std::chrono::milliseconds& timeout) {
  Buffer msg(1000);
  udp::endpoint sender_ep;
  socket_.async_receive_from(asio::buffer(msg), sender_ep, [&](auto ec, auto size) {
    EXPECT_FALSE(ec) << ec.message();
    msg.resize(size);
  });

  ioc_.reset();
  if (!ioc_.run_one_for(timeout)) {
    throw std::runtime_error("No multicast message received within the specified time.");
  }

  return {sender_ep.address(), msg};
}

BranchEventRecorder::BranchEventRecorder(void* context, void* branch)
    : context_(context), branch_(branch), json_str_(10000) {
  start_await_event();
}

nlohmann::json BranchEventRecorder::run_context_until(int event, const uuids::uuid& uuid, int ev_res) {
  while (true) {
    for (auto& entry : events_) {
      if (entry.uuid == uuid && entry.event == event && entry.ev_res == ev_res) {
        return entry.json;
      }
    }

    auto n = events_.size();
    while (n == events_.size()) {
      int res = YOGI_ContextRunOne(context_, nullptr, 3000000000ll);
      EXPECT_OK(res);
    }
  }
}

nlohmann::json BranchEventRecorder::run_context_until(int event, void* branch, int ev_res) {
  return run_context_until(event, get_branch_uuid(branch), ev_res);
}

void BranchEventRecorder::start_await_event() {
  int res = YOGI_BranchAwaitEventAsync(branch_, YOGI_BEV_ALL, &uuid_, json_str_.data(),
                                       static_cast<int>(json_str_.size()), &BranchEventRecorder::callback, this);
  EXPECT_OK(res);
}

void BranchEventRecorder::callback(int res, int event, int ev_res, void* userarg) {
  if (res == YOGI_ERR_CANCELED) return;

  auto self = static_cast<BranchEventRecorder*>(userarg);
  self->events_.push_back({self->uuid_, nlohmann::json::parse(self->json_str_.data()), event, ev_res});

  self->start_await_event();
}

FakeBranch::FakeBranch()
    : acceptor_(ioc_), tcp_socket_(ioc_), adv_ep_(ip::make_address(kAdvAddress), kAdvPort), mc_socket_(adv_ep_) {
  acceptor_.open(kTcpProtocol);
  acceptor_.set_option(tcp::acceptor::reuse_address(true));
  acceptor_.bind(tcp::endpoint(kTcpProtocol, 0));
  acceptor_.listen();

  auto adv_ifs = get_filtered_network_interfaces({"localhost"}, adv_ep_.protocol());

  nlohmann::json cfg = {
      {"name", "Fake Branch"},
      {"advertising_address", kAdvAddress},
      {"advertising_port", kAdvPort},
  };

  info_ = std::make_shared<LocalBranchInfo>(cfg, adv_ifs, acceptor_.local_endpoint().port());
}

void FakeBranch::connect(void* branch, std::function<void(Buffer*)> msg_changer) {
  auto addr = mc_socket_.receive().first;
  tcp_socket_.connect(tcp::endpoint(addr, get_branch_info(branch)["tcp_server_port"].get<unsigned short>()));
  authenticate(msg_changer);
}

void FakeBranch::accept(std::function<void(Buffer*)> msg_changer) {
  tcp_socket_ = acceptor_.accept();
  authenticate(msg_changer);
}

void FakeBranch::disconnect() {
  tcp_socket_.shutdown(tcp_socket_.shutdown_both);
  tcp_socket_.close();
}

void FakeBranch::advertise(std::function<void(Buffer*)> msg_changer) {
  auto msg = *info_->make_advertising_message();
  if (msg_changer) msg_changer(&msg);
  mc_socket_.send(msg);
}

bool FakeBranch::is_connected_to(void* branch) const {
  struct Data {
    uuids::uuid my_uuid;
    uuids::uuid uuid;
    bool connected = false;
  } data;

  data.my_uuid = info_->get_uuid();

  int res = YOGI_BranchGetConnectedBranches(
      branch, &data.uuid, nullptr, 0,
      [](int, void* userarg) {
        auto data = static_cast<Data*>(userarg);
        if (data->uuid == data->my_uuid) {
          data->connected = true;
        }
      },
      &data);
  EXPECT_OK(res);

  return data.connected;
}

void FakeBranch::authenticate(std::function<void(Buffer*)> msg_changer) {
  // Send branch info
  auto info_msg = *info_->make_info_message();
  if (msg_changer) msg_changer(&info_msg);
  asio::write(tcp_socket_, asio::buffer(info_msg));

  // Receive branch info
  auto buffer = Buffer(BranchInfo::kInfoMessageHeaderSize);
  asio::read(tcp_socket_, asio::buffer(buffer));
  std::size_t body_size;
  RemoteBranchInfo::deserialize_info_message_body_size(&body_size, buffer);
  buffer.resize(body_size);
  asio::read(tcp_socket_, asio::buffer(buffer));

  // ACK
  exchange_ack();

  // Send challenge
  auto my_challenge = generate_random_bytes(8);
  asio::write(tcp_socket_, asio::buffer(my_challenge));

  // Receive challenge
  auto remote_challenge = Buffer(8);
  asio::read(tcp_socket_, asio::buffer(remote_challenge));

  // Send solution
  auto password_hash = make_sha256(Buffer{});
  buffer             = remote_challenge;
  buffer.insert(buffer.end(), password_hash.begin(), password_hash.end());
  auto remote_solution = make_sha256(buffer);
  asio::write(tcp_socket_, asio::buffer(remote_solution));

  // Receive Solution
  buffer.resize(remote_solution.size());
  asio::read(tcp_socket_, asio::buffer(buffer));

  // ACK
  exchange_ack();
}

void FakeBranch::exchange_ack() {
  auto buffer = Buffer{MessageType::kAcknowledge};
  asio::write(tcp_socket_, asio::buffer(buffer));
  asio::read(tcp_socket_, asio::buffer(buffer));
  EXPECT_EQ(buffer[0], MessageType::kAcknowledge);
}

CommandLine::CommandLine(std::initializer_list<std::string> args) {
  argc = static_cast<int>(args.size() + 1);
  argv = new char*[static_cast<std::size_t>(argc)];

  std::string exe = "executable-name";
  argv[0]         = new char[exe.size() + 1];
  std::copy(exe.begin(), exe.end(), argv[0]);
  argv[0][exe.size()] = '\0';

  auto it = args.begin();
  for (int i = 1; i < argc; ++i) {
    auto& arg = *it;
    argv[i]   = new char[arg.size() + 1];
    std::copy(arg.begin(), arg.end(), argv[i]);
    argv[i][arg.size()] = '\0';
    ++it;
  }
}

CommandLine::~CommandLine() {
  for (int i = 0; i < argc; ++i) {
    delete[] argv[i];
  }

  delete[] argv;
}

void* create_context() {
  void* context = nullptr;
  int res       = YOGI_ContextCreate(&context);
  EXPECT_OK(res);
  EXPECT_NE(context, nullptr);
  return context;
}

void* create_configuration(const nlohmann::json& json) {
  void* config;
  int res = YOGI_ConfigurationCreate(&config, YOGI_CFG_NONE);
  EXPECT_OK(res);

  res = YOGI_ConfigurationUpdateFromJson(config, json.dump().c_str());
  EXPECT_OK(res);

  return config;
}

void* create_branch(void* context, const char* name, const char* net_name, const char* password, const char* path,
                    const char* adv_addr, std::size_t transceive_byte_limit) {
  auto props           = kBranchProps;
  props["description"] = "Description";
  if (name) props["name"] = name;
  if (net_name) props["network_name"] = net_name;
  if (password) props["network_password"] = password;
  if (path) props["path"] = path;
  if (adv_addr) props["advertising_address"] = adv_addr;

  if (transceive_byte_limit != std::numeric_limits<std::size_t>::max()) {
    props["_transceive_byte_limit"] = transceive_byte_limit;
  }

  void* branch = nullptr;
  int res      = YOGI_BranchCreate(&branch, context, create_configuration(props), nullptr);
  EXPECT_OK(res);

  return branch;
}

void poll_context(void* context) {
  int res = YOGI_ContextPoll(context, nullptr);
  EXPECT_OK(res);
}

void poll_context_one(void* context) {
  int res = YOGI_ContextPollOne(context, nullptr);
  EXPECT_OK(res);
}

void run_context_in_background(void* context) {
  int res = YOGI_ContextRunInBackground(context);
  EXPECT_OK(res);
  res = YOGI_ContextWaitForRunning(context, 1000000000ll);
  EXPECT_OK(res);
}

void run_context_until_branches_are_connected(void* context, std::initializer_list<void*> branches) {
  std::map<void*, std::set<uuids::uuid>> uuids;
  for (auto branch : branches) {
    uuids[branch] = {};
  }
  for (auto& entry : uuids) {
    for (auto branch : branches) {
      if (branch != entry.first) {
        entry.second.insert(get_branch_uuid(branch));
      }
    }
  }

  auto start = std::chrono::steady_clock::now();

  while (!uuids.empty()) {
    auto res = YOGI_ContextPoll(context, nullptr);
    EXPECT_OK(res);

    auto entry = uuids.begin();
    auto infos = get_connected_branches(entry->first);
    for (auto& info : infos) {
      auto uuid = info.first;
      entry->second.erase(uuid);
    }

    if (entry->second.empty()) {
      uuids.erase(entry);
    }

    if (std::chrono::steady_clock::now() - start > 3s) {
      throw std::runtime_error("Branches did not connect");
    }
  }
}

uuids::uuid get_branch_uuid(void* branch) {
  uuids::uuid uuid = {0};
  int res          = YOGI_BranchGetInfo(branch, &uuid, nullptr, 0);
  EXPECT_OK(res);
  EXPECT_NE(uuid, uuids::uuid{});
  return uuid;
}

nlohmann::json get_branch_info(void* branch) {
  char str[10000] = {0};
  int res         = YOGI_BranchGetInfo(branch, nullptr, str, sizeof(str));
  EXPECT_OK(res);
  return nlohmann::json::parse(str);
}

std::map<uuids::uuid, nlohmann::json> get_connected_branches(void* branch) {
  struct Data {
    uuids::uuid uuid;
    char json_str[1000] = {0};
    std::map<uuids::uuid, nlohmann::json> branches;
  } data;

  int res = YOGI_BranchGetConnectedBranches(
      branch, &data.uuid, data.json_str, sizeof(data.json_str),
      [](int, void* userarg) {
        auto data                  = static_cast<Data*>(userarg);
        data->branches[data->uuid] = nlohmann::json::parse(data->json_str);
      },
      &data);
  EXPECT_OK(res);

  return data.branches;
}

std::string make_version_string(int major, int minor, int patch, const std::string& suffix) {
  std::ostringstream ss;
  ss << major << '.' << minor << '.' << patch << suffix;
  return ss.str();
}

std::string read_file(const std::string& filename) {
  std::ifstream f(filename);
  EXPECT_TRUE(f.is_open()) << filename;
  std::string content((std::istreambuf_iterator<char>(f)), (std::istreambuf_iterator<char>()));
  return content;
}

int find_unused_port() {
  asio::io_context ioc;
  tcp::socket socket(ioc);
  socket.open(tcp::v6());

  unsigned short port = 20000;
  boost::system::error_code ec;
  do {
    ++port;
    socket.bind(tcp::endpoint(tcp::v6(), port), ec);
  } while (ec == boost::asio::error::address_in_use);

  socket.close();
  return static_cast<int>(port);
}

void check_json_elements_are_equal(const nlohmann::json& a, const nlohmann::json& b, const std::string& key) {
  ASSERT_TRUE(a.count(key)) << "Key \"" << key << "\" does not exist in a";
  ASSERT_TRUE(b.count(key)) << "Key \"" << key << "\" does not exist in b";
  EXPECT_EQ(a[key].dump(), b[key].dump());
}
