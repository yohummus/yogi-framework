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
#include <src/objects/logger.h>

#include <boost/filesystem.hpp>

#include <fstream>
#include <sstream>

namespace fs   = boost::filesystem;
namespace asio = boost::asio;
namespace ip   = asio::ip;
using tcp      = ip::tcp;
using udp      = ip::udp;

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

std::string make_version_string(int major, int minor, int patch, const std::string& suffix) {
  std::ostringstream ss;
  ss << major << '.' << minor << '.' << patch << suffix;
  return ss.str();
}

void* create_context() {
  void* context = nullptr;
  int res       = YOGI_ContextCreate(&context);
  EXPECT_OK(res);
  EXPECT_NE(context, nullptr);
  return context;
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
