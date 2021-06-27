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

#pragma once

#define YOGI_API  // To remove __declspec(dllimport) from API functions since we are linking them statically

#include <include/yogi_core.h>
#include <src/api/errors.h>
#include <src/data/buffer.h>
#include <src/objects/branch/branch_info.h>

#include <gtest/gtest.h>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/uuid/uuid.hpp>
#include <nlohmann/json.hpp>

#include <chrono>
#include <initializer_list>
#include <map>
#include <string>
#include <string_view>

using namespace std::chrono_literals;
using namespace std::string_literals;

// ========== Helper macros ==========
#define EXPECT_OK(res) EXPECT_ERR(res, YOGI_OK) << YOGI_GetLastErrorDetails()
#define ASSERT_OK(res) ASSERT_ERR(res, YOGI_OK) << YOGI_GetLastErrorDetails()
#define EXPECT_ERR(res, err) EXPECT_EQ(res, err) << YOGI_GetLastErrorDetails()
#define ASSERT_ERR(res, err) EXPECT_EQ(res, err) << YOGI_GetLastErrorDetails()
#define EXPECT_CONTAINS(str, substr) EXPECT_NE(std::string_view{str}.find(substr), std::string::npos) << str;

#define _EXPECT_THROW_ERROR(statement, ec, catch_statement)                                     \
  try {                                                                                         \
    statement;                                                                                  \
    FAIL() << "No exception thrown";                                                            \
  }                                                                                             \
  catch_statement catch (const std::exception& e) {                                             \
    FAIL() << "Wrong exception type thrown: " << typeid(e).name() << " with msg: " << e.what(); \
  }

#define EXPECT_THROW_ERROR(statement, ec) \
  _EXPECT_THROW_ERROR(                    \
      statement, ec, catch (const Error& err) { EXPECT_EQ(err.value(), ec); })

#define EXPECT_THROW_DESCRIPTIVE_ERROR(statement, ec)      \
  _EXPECT_THROW_ERROR(                                     \
      statement, ec, catch (const DescriptiveError& err) { \
        EXPECT_EQ(err.value(), ec);                        \
        EXPECT_FALSE(err.details().empty());               \
      })

// ========== Networking-related constants for testing ==========
static const nlohmann::json kBranchProps = nlohmann::json::parse(R"raw(
  {
    "advertising_interfaces": ["localhost"],
    "advertising_port": 44442,
    "advertising_address": "ff02::8000:2439",
    "advertising_interval": 0.1,
    "timeout": 3.0
  }
)raw");

static const std::chrono::nanoseconds kTimingMargin = 50ms;
static const std::string kAdvAddress                = kBranchProps["advertising_address"];
static const unsigned short kAdvPort                = kBranchProps["advertising_port"];

static const auto kUdpProtocol =
    boost::asio::ip::make_address(kAdvAddress).is_v4() ? boost::asio::ip::udp::v4() : boost::asio::ip::udp::v6();

static const auto kTcpProtocol =
    boost::asio::ip::make_address(kAdvAddress).is_v4() ? boost::asio::ip::tcp::v4() : boost::asio::ip::tcp::v6();

static const auto kLoopbackAddress = kTcpProtocol == boost::asio::ip::tcp::v4()
                                         ? boost::asio::ip::make_address("127.0.0.1")
                                         : boost::asio::ip::make_address("::1");

// ========== Test fixture base class ==========
class TestFixture : public testing::Test {
 public:
  TestFixture();
  virtual ~TestFixture();
};

// ========== Helpers for temporary directories ==========
class TemporaryWorkdirGuard final {
 public:
  TemporaryWorkdirGuard();
  TemporaryWorkdirGuard(TemporaryWorkdirGuard&& other);
  ~TemporaryWorkdirGuard();

  TemporaryWorkdirGuard(const TemporaryWorkdirGuard&) = delete;
  TemporaryWorkdirGuard& operator=(const TemporaryWorkdirGuard&) = delete;

 private:
  boost::filesystem::path old_working_dir_;
  boost::filesystem::path temp_path_;
};

// ========== Helpers for testing networking ==========
class MulticastSocket final {
 public:
  MulticastSocket(const boost::asio::ip::udp::endpoint& multicast_ep);

  void send(const Buffer& msg);
  std::pair<boost::asio::ip::address, Buffer> receive(
      const std::chrono::milliseconds& timeout = std::chrono::seconds(1));

 private:
  boost::asio::io_context ioc_;
  boost::asio::ip::udp::endpoint mc_ep_;
  boost::asio::ip::udp::socket socket_;
};

// ========== Helpers for testing branches ==========
class BranchEventRecorder final {
 public:
  BranchEventRecorder(void* context, void* branch);

  nlohmann::json run_context_until(int event, const boost::uuids::uuid& uuid, int ev_res);
  nlohmann::json run_context_until(int event, void* branch, int ev_res);

 private:
  void start_await_event();
  static void callback(int res, int ev, int evres, const void* uuid, const char* json, int jsonsize, void* userarg);

  struct CallbackData {
    boost::uuids::uuid uuid;
    nlohmann::json json;
    int event;
    int evres;
  };

  void* context_;
  void* branch_;
  std::vector<CallbackData> events_;
};

class FakeBranch final {
 public:
  FakeBranch();

  void connect(void* branch, std::function<void(Buffer*)> info_changer = {});
  void accept(std::function<void(Buffer*)> info_changer = {});
  void disconnect();
  void advertise(std::function<void(Buffer*)> msg_changer = {});

  bool is_connected_to(void* branch) const;

 private:
  void authenticate(std::function<void(Buffer*)> info_changer);
  void exchange_ack();

  LocalBranchInfoPtr info_;
  boost::asio::io_context ioc_;
  boost::asio::ip::tcp::acceptor acceptor_;
  boost::asio::ip::tcp::socket tcp_socket_;
  boost::asio::ip::udp::endpoint adv_ep_;
  MulticastSocket mc_socket_;
};

// ========== Helpers for command-line parameter emulation ==========
struct CommandLine final {
  CommandLine(std::initializer_list<std::string> args);
  ~CommandLine();

  CommandLine(const CommandLine&) = delete;
  CommandLine& operator=(const CommandLine&) = delete;

  int argc;
  char** argv;
};

// ========== Helper functions for creating objects ==========
void* create_context();
void* create_configuration(const nlohmann::json& json);
void* create_branch(void* context, const char* name = nullptr, const char* net_name = nullptr,
                    const char* password = nullptr, const char* path = nullptr, const char* adv_addr = nullptr,
                    std::size_t transceive_byte_limit = std::numeric_limits<std::size_t>::max());

// ========== Helper function for interacting with objects ==========
void poll_context(void* context);
void poll_context_one(void* context);
void run_context_in_background(void* context);
void run_context_until_branches_are_connected(void* context, std::initializer_list<void*> branches);
boost::uuids::uuid get_branch_uuid(void* branch);
nlohmann::json get_branch_info(void* branch);
std::map<boost::uuids::uuid, nlohmann::json> get_connected_branches(void* branch);

// ========== Various helper functions ==========
std::string make_version_string(int major, int minor, int patch, const std::string& suffix = {});
std::string read_file(const std::string& filename);
int find_unused_port();
void check_json_elements_are_equal(const nlohmann::json& a, const nlohmann::json& b, const std::string& key);
