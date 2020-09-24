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

class LoggingTest : public Test {
 protected:
  yogi::LoggerPtr create_logger() {
    MOCK_LoggerCreate([](void** logger, const char*) {
      *logger = kPointer;
      return YOGI_OK;
    });

    MOCK_Destroy([](void*) { return YOGI_OK; });

    return Logger::create("");
  }
};

TEST_F(LoggingTest, SetupConsoleLogging) {
  MOCK_ConfigureConsoleLogging([](int verbosity, int stream, int color, const char* timefmt, const char* fmt) {
    EXPECT_EQ(verbosity, YOGI_VB_INFO);
    EXPECT_EQ(stream, YOGI_ST_STDERR);
    EXPECT_EQ(color, YOGI_FALSE);
    EXPECT_EQ(timefmt, nullptr);
    EXPECT_EQ(fmt, nullptr);
    return YOGI_OK;
  });
  yogi::configure_console_logging(yogi::Verbosity::kInfo, yogi::Stream::kStderr, false);

  MOCK_ConfigureConsoleLogging([](int verbosity, int stream, int color, const char* timefmt, const char* fmt) {
    EXPECT_EQ(verbosity, YOGI_VB_TRACE);
    EXPECT_EQ(stream, YOGI_ST_STDOUT);
    EXPECT_EQ(color, YOGI_TRUE);
    EXPECT_STREQ(timefmt, "foo");
    EXPECT_STREQ(fmt, "bar");
    return YOGI_OK;
  });
  yogi::configure_console_logging(yogi::Verbosity::kTrace, yogi::Stream::kStdout, true, "foo", "bar");
}

TEST_F(LoggingTest, SetupConsoleLoggingError) {
  MOCK_ConfigureConsoleLogging([](int, int, int, const char*, const char*) { return YOGI_ERR_UNKNOWN; });
  EXPECT_THROW(yogi::configure_console_logging(yogi::Verbosity::kError), yogi::FailureException);
}

TEST_F(LoggingTest, DisableConsoleLogging) {
  MOCK_ConfigureConsoleLogging([](int verbosity, int, int, const char*, const char*) {
    EXPECT_EQ(verbosity, YOGI_VB_NONE);
    return YOGI_OK;
  });
  yogi::disable_console_logging();
}

TEST_F(LoggingTest, DisableConsoleLoggingError) {
  MOCK_ConfigureConsoleLogging([](int, int, int, const char*, const char*) { return YOGI_ERR_UNKNOWN; });
  EXPECT_THROW(yogi::disable_console_logging(), yogi::FailureException);
}

TEST_F(LoggingTest, SetupHookLogging) {
  bool called = false;
  auto fn     = [&](yogi::Verbosity severity, yogi::Timestamp timestamp, int tid, std::string file, int line,
                std::string comp, std::string msg) {
    EXPECT_EQ(severity, yogi::Verbosity::kTrace);
    EXPECT_EQ(timestamp.duration_since_epoch().nanoseconds_count(), 12345ll);
    EXPECT_EQ(tid, 555);
    EXPECT_EQ(file, "foo");
    EXPECT_EQ(line, 111);
    EXPECT_EQ(comp, "bar");
    EXPECT_EQ(msg, "hello");
    called = true;
  };

  MOCK_ConfigureHookLogging([](int verbosity,
                               void (*fn)(int severity, long long timestamp, int tid, const char* file, int line,
                                          const char* comp, const char* msg, void* userarg),
                               void* userarg) {
    EXPECT_EQ(verbosity, YOGI_VB_INFO);
    EXPECT_NE(fn, nullptr);
    fn(YOGI_VB_TRACE, 12345, 555, "foo", 111, "bar", "hello", userarg);
    return YOGI_OK;
  });
  yogi::configure_hook_logging(yogi::Verbosity::kInfo, fn);
  EXPECT_TRUE(called);
}

TEST_F(LoggingTest, SetupHookLoggingWithNoFn) {
  MOCK_ConfigureHookLogging([](int,
                               void (*fn)(int severity, long long timestamp, int tid, const char* file, int line,
                                          const char* comp, const char* msg, void* userarg),
                               void*) {
    EXPECT_EQ(fn, nullptr);
    return YOGI_OK;
  });

  EXPECT_NO_THROW(yogi::configure_hook_logging(yogi::Verbosity::kInfo, {}));
}

TEST_F(LoggingTest, SetupHookLoggingError) {
  MOCK_ConfigureHookLogging([](int, void (*)(int, long long, int, const char*, int, const char*, const char*, void*),
                               void*) { return YOGI_ERR_UNKNOWN; });
  EXPECT_THROW(yogi::configure_hook_logging(yogi::Verbosity::kInfo, {}), yogi::FailureException);
}

TEST_F(LoggingTest, DisableHookLogging) {
  MOCK_ConfigureHookLogging(
      [](int verbosity, void (*)(int, long long, int, const char*, int, const char*, const char*, void*), void*) {
        EXPECT_EQ(verbosity, YOGI_VB_NONE);
        return YOGI_OK;
      });
  yogi::disable_hook_logging();
}

TEST_F(LoggingTest, DisableHookLoggingError) {
  MOCK_ConfigureHookLogging([](int, void (*)(int, long long, int, const char*, int, const char*, const char*, void*),
                               void*) { return YOGI_ERR_UNKNOWN; });
  EXPECT_THROW(yogi::disable_hook_logging(), yogi::FailureException);
}

TEST_F(LoggingTest, SetupFileLogging) {
  MOCK_ConfigureFileLogging([](int verbosity, const char* filename, const char** genfn, int* genfnsize,
                               const char* timefmt, const char* fmt) {
    EXPECT_EQ(verbosity, YOGI_VB_INFO);
    EXPECT_STREQ(filename, "foo");
    EXPECT_NE(genfn, nullptr);
    EXPECT_NE(genfnsize, nullptr);
    *genfn     = "x";
    *genfnsize = static_cast<int>(strlen(*genfn) + 1);
    EXPECT_EQ(timefmt, nullptr);
    EXPECT_EQ(fmt, nullptr);
    return YOGI_OK;
  });
  EXPECT_EQ(yogi::configure_file_logging(yogi::Verbosity::kInfo, "foo"), "x");

  MOCK_ConfigureFileLogging([](int verbosity, const char* filename, const char** genfn, int* genfnsize,
                               const char* timefmt, const char* fmt) {
    EXPECT_EQ(verbosity, YOGI_VB_TRACE);
    EXPECT_STREQ(filename, "moo");
    EXPECT_NE(genfn, nullptr);
    EXPECT_NE(genfnsize, nullptr);
    *genfn     = "y";
    *genfnsize = static_cast<int>(strlen(*genfn) + 1);
    EXPECT_STREQ(timefmt, "foo");
    EXPECT_STREQ(fmt, "bar");
    return YOGI_OK;
  });
  EXPECT_EQ(yogi::configure_file_logging(yogi::Verbosity::kTrace, "moo", "foo", "bar"), "y");
}

TEST_F(LoggingTest, SetupFileLoggingError) {
  MOCK_ConfigureFileLogging(
      [](int, const char*, const char**, int*, const char*, const char*) { return YOGI_ERR_UNKNOWN; });
  EXPECT_THROW(yogi::configure_file_logging(yogi::Verbosity::kTrace, "foo"), yogi::FailureException);
}

TEST_F(LoggingTest, DisableFileLogging) {
  MOCK_ConfigureFileLogging([](int verbosity, const char*, const char**, int*, const char*, const char*) {
    EXPECT_EQ(verbosity, YOGI_VB_NONE);
    return YOGI_OK;
  });
  yogi::disable_file_logging();
}

TEST_F(LoggingTest, DisableFileLoggingError) {
  MOCK_ConfigureFileLogging(
      [](int, const char*, const char**, int*, const char*, const char*) { return YOGI_ERR_UNKNOWN; });
  EXPECT_THROW(yogi::disable_file_logging(), yogi::FailureException);
}

TEST_F(LoggingTest, SetComponentsVerbosity) {
  MOCK_LoggerSetComponentsVerbosity([](const char* components, int verbosity, int* count) {
    EXPECT_STREQ(components, "foo");
    EXPECT_EQ(verbosity, YOGI_VB_INFO);
    EXPECT_NE(count, nullptr);
    *count = 5;
    return YOGI_OK;
  });

  EXPECT_EQ(yogi::Logger::set_components_verbosity("foo", yogi::Verbosity::kInfo), 5);
}

TEST_F(LoggingTest, SetComponentsVerbosityError) {
  MOCK_LoggerSetComponentsVerbosity([](const char* components, int verbosity, int*) {
    EXPECT_STREQ(components, "bar");
    EXPECT_EQ(verbosity, YOGI_VB_ERROR);
    return YOGI_ERR_UNKNOWN;
  });

  EXPECT_THROW(yogi::Logger::set_components_verbosity("bar", yogi::Verbosity::kError), yogi::FailureException);
}

TEST_F(LoggingTest, CreateLogger) {
  MOCK_LoggerCreate([](void** logger, const char* component) {
    EXPECT_NE(logger, nullptr);
    EXPECT_STREQ(component, "foo");
    *logger = kPointer;
    return YOGI_OK;
  });
  auto logger = Logger::create("foo");

  static bool called;
  called = false;
  MOCK_Destroy([](void* obj) {
    EXPECT_EQ(obj, kPointer);
    called = true;
    return YOGI_OK;
  });
  logger.reset();
  EXPECT_TRUE(called);
}

TEST_F(LoggingTest, CreateLoggerError) {
  MOCK_LoggerCreate([](void**, const char*) { return YOGI_ERR_UNKNOWN; });
  EXPECT_THROW(Logger::create("foo"), yogi::FailureException);
}

TEST_F(LoggingTest, LoggerVerbosity) {
  auto logger = create_logger();

  MOCK_LoggerGetVerbosity([](void* logger, int* verbosity) {
    EXPECT_EQ(logger, kPointer);
    EXPECT_NE(verbosity, nullptr);
    *verbosity = YOGI_VB_TRACE;
    return YOGI_OK;
  });
  EXPECT_EQ(logger->verbosity(), yogi::Verbosity::kTrace);
}

TEST_F(LoggingTest, LoggerVerbosityError) {
  auto logger = create_logger();

  MOCK_LoggerGetVerbosity([](void*, int*) { return YOGI_ERR_UNKNOWN; });
  EXPECT_THROW(logger->verbosity(), yogi::FailureException);
}

TEST_F(LoggingTest, Log) {
  auto logger = create_logger();

  MOCK_LoggerLog([](void* logger, int severity, const char* file, int line, const char* msg) {
    EXPECT_EQ(logger, kPointer);
    EXPECT_EQ(severity, YOGI_VB_TRACE);
    EXPECT_STREQ(file, "bar");
    EXPECT_EQ(line, 123);
    EXPECT_STREQ(msg, "foo");
    return YOGI_OK;
  });
  logger->log(yogi::Verbosity::kTrace, "foo", "bar", 123);

  MOCK_LoggerLog([](void* logger, int severity, const char* file, int line, const char* msg) {
    EXPECT_EQ(logger, kPointer);
    EXPECT_EQ(severity, YOGI_VB_TRACE);
    EXPECT_EQ(file, nullptr);
    EXPECT_EQ(line, 0);
    EXPECT_STREQ(msg, "foo");
    return YOGI_OK;
  });
  logger->log(yogi::Verbosity::kTrace, "foo");
}

TEST_F(LoggingTest, LogError) {
  auto logger = create_logger();

  MOCK_LoggerLog([](void*, int, const char*, int, const char*) { return YOGI_ERR_UNKNOWN; });
  EXPECT_THROW(logger->log(yogi::Verbosity::kTrace, "foo", "bar", 123), yogi::FailureException);
  EXPECT_THROW(logger->log(yogi::Verbosity::kTrace, "foo"), yogi::FailureException);
}

TEST_F(LoggingTest, AppLogger) {
  MOCK_LoggerLog([](void* logger, int, const char*, int, const char*) {
    EXPECT_EQ(logger, nullptr);
    return YOGI_OK;
  });
  yogi::app_logger->log(yogi::Verbosity::kTrace, "foo");
}

TEST_F(LoggingTest, Macros) {
  MOCK_LoggerGetVerbosity([](void*, int* verbosity) {
    *verbosity = YOGI_VB_TRACE;
    return YOGI_OK;
  });

  static bool called;

  // App logger FATAL
  called = false;
  MOCK_LoggerLog([](void* logger, int severity, const char* file, int line, const char* msg) {
    EXPECT_EQ(logger, nullptr);
    EXPECT_EQ(severity, YOGI_VB_FATAL);
    EXPECT_NE(file, nullptr);
    EXPECT_GT(line, 0);
    EXPECT_STREQ(msg, "foo_bar");
    called = true;
    return YOGI_OK;
  });
  YOGI_LOG_FATAL("foo" << '_' << "bar");
  EXPECT_TRUE(called);

  // App logger ERROR
  MOCK_LoggerLog([](void*, int severity, const char*, int, const char*) {
    EXPECT_EQ(severity, YOGI_VB_ERROR);
    return YOGI_OK;
  });
  YOGI_LOG_ERROR("foo" << '_' << "bar");

  // App logger WARNING
  MOCK_LoggerLog([](void*, int severity, const char*, int, const char*) {
    EXPECT_EQ(severity, YOGI_VB_WARNING);
    return YOGI_OK;
  });
  YOGI_LOG_WARNING("foo" << '_' << "bar");

  // App logger INFO
  MOCK_LoggerLog([](void*, int severity, const char*, int, const char*) {
    EXPECT_EQ(severity, YOGI_VB_INFO);
    return YOGI_OK;
  });
  YOGI_LOG_INFO("foo" << '_' << "bar");

  // App logger DEBUG
  MOCK_LoggerLog([](void*, int severity, const char*, int, const char*) {
    EXPECT_EQ(severity, YOGI_VB_DEBUG);
    return YOGI_OK;
  });
  YOGI_LOG_DEBUG("foo" << '_' << "bar");

  // App logger TRACE
  MOCK_LoggerLog([](void*, int severity, const char*, int, const char*) {
    EXPECT_EQ(severity, YOGI_VB_TRACE);
    return YOGI_OK;
  });
  YOGI_LOG_TRACE("foo" << '_' << "bar");

  // Custom logger FATAL
  auto logger = create_logger();

  called = false;
  MOCK_LoggerLog([](void* logger, int severity, const char* file, int line, const char* msg) {
    EXPECT_EQ(logger, kPointer);
    EXPECT_EQ(severity, YOGI_VB_FATAL);
    EXPECT_NE(file, nullptr);
    EXPECT_GT(line, 0);
    EXPECT_STREQ(msg, "foo_bar");
    called = true;
    return YOGI_OK;
  });
  YOGI_LOG_FATAL(logger, "foo" << '_' << "bar");
  EXPECT_TRUE(called);

  // Custom logger ERROR
  MOCK_LoggerLog([](void*, int severity, const char*, int, const char*) {
    EXPECT_EQ(severity, YOGI_VB_ERROR);
    return YOGI_OK;
  });
  YOGI_LOG_ERROR("foo" << '_' << "bar");

  // Custom logger WARNING
  MOCK_LoggerLog([](void*, int severity, const char*, int, const char*) {
    EXPECT_EQ(severity, YOGI_VB_WARNING);
    return YOGI_OK;
  });
  YOGI_LOG_WARNING("foo" << '_' << "bar");

  // Custom logger INFO
  MOCK_LoggerLog([](void*, int severity, const char*, int, const char*) {
    EXPECT_EQ(severity, YOGI_VB_INFO);
    return YOGI_OK;
  });
  YOGI_LOG_INFO("foo" << '_' << "bar");

  // Custom logger DEBUG
  MOCK_LoggerLog([](void*, int severity, const char*, int, const char*) {
    EXPECT_EQ(severity, YOGI_VB_DEBUG);
    return YOGI_OK;
  });
  YOGI_LOG_DEBUG("foo" << '_' << "bar");

  // Custom logger TRACE
  MOCK_LoggerLog([](void*, int severity, const char*, int, const char*) {
    EXPECT_EQ(severity, YOGI_VB_TRACE);
    return YOGI_OK;
  });
  YOGI_LOG_TRACE("foo" << '_' << "bar");
}
