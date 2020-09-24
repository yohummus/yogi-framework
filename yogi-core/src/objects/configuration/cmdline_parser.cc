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

#include <src/api/errors.h>
#include <src/objects/configuration/cmdline_parser.h>
#include <src/objects/logger.h>
#include <src/system/glob.h>

#include <boost/algorithm/string.hpp>
#include <boost/optional/optional_io.hpp>
#include <fstream>
#include <sstream>

namespace po = boost::program_options;

CmdlineParser::CmdlineParser(int argc, const char* const* argv, int clo_flags)
    : argc_{argc},
      argv_{argv},
      clo_flags_{clo_flags},
      visible_options_{"Allowed options"},
      hidden_options_{"Hidden options"},
      files_json_({}),
      direct_json_({}) {
}

void CmdlineParser::parse() {
  YOGI_ASSERT(vm_.empty());  // only run once!

  add_help_options();
  add_logging_options();
  add_branch_options();
  add_file_options();
  add_override_options();
  add_variable_options();

  populate_variables_map();
  handle_help_options();
  extract_options();
  apply_overrides();

  load_config_files();
}

void CmdlineParser::add_help_options() {
  // clang-format off
  visible_options_.add_options()(
    "help,h",
    "Show this help message"
  );

  if (clo_flags_ & YOGI_CLO_LOGGING) {
    visible_options_.add_options()(
      "help-logging",
      "Shows information about the logging options"
    );
  }
  // clang-format on
}

void CmdlineParser::add_logging_options() {
  // clang-format off
  if (clo_flags_ & YOGI_CLO_LOGGING) {
    visible_options_.add_options()(
      "log-file", po::value<std::string>()->notifier([&](auto& val) {
        this->log_file_notifier(val);
      }),
      "Path to the logfile with support for time placeholders; set to NONE to disable"
    )(
      "log-console", po::value<std::string>()->notifier([&](auto& val) {
        this->log_console_notifier(val);
      })->implicit_value("STDERR"),
      "Log to either STDOUT, STDERR or NONE (implicit value is STDERR)"
    )(
      "log-color", po::value<bool>()->notifier([&](auto& val) {
        direct_json_["logging"]["color"] = val;
      })->implicit_value(true),
      "Use color when logging to the console"
    )(
      "log-fmt", po::value<std::string>()->notifier([&](auto& val) {
        direct_json_["logging"]["entry-format"] = val;
      }),
      "Format of a log entry (use entry placeholders)"
    )(
      "log-time-fmt", po::value<std::string>()->notifier([&](auto& val) {
        direct_json_["logging"]["time-format"] = val;
      }),
      "Format of a log entry's timestamp (use time placeholders)"
    )(
      "log-verbosity", po::value<std::vector<std::string>>()->notifier([&](auto& val) {
        this->log_verbosity_notifier(val);
      }),
      "Logger verbosities (e.g. --log-verbosity='Yogi.*=DEBUG')"
    );
  }
  // clang-format on
}

void CmdlineParser::add_branch_options() {
  // clang-format off
  if (clo_flags_ & YOGI_CLO_BRANCH_NAME) {
    visible_options_.add_options()(
      "name", po::value<std::string>()->notifier([&](auto& val) {
        direct_json_["branch"]["name"] = val;
      }),
      "Branch name"
    );
  }

  if (clo_flags_ & YOGI_CLO_BRANCH_DESCRIPTION) {
    visible_options_.add_options()(
      "description", po::value<std::string>()->notifier([&](auto& val) {
        direct_json_["branch"]["description"] = val;
      }),
      "Branch description"
    );
  }

  if (clo_flags_ & YOGI_CLO_BRANCH_NETWORK) {
    visible_options_.add_options()(
      "network", po::value<std::string>()->notifier([&](auto& val) {
        direct_json_["branch"]["network"] = val;
      }),
      "Network name"
    );
  }

  if (clo_flags_ & YOGI_CLO_BRANCH_PASSWORD) {
    visible_options_.add_options()(
      "password", po::value<std::string>()->notifier([&](auto& val) {
        direct_json_["branch"]["password"] = val;
      }),
      "Network password"
    );
  }

  if (clo_flags_ & YOGI_CLO_BRANCH_PATH) {
    visible_options_.add_options()(
      "path", po::value<std::string>()->notifier([&](auto& val) {
        direct_json_["branch"]["path"] = val;
      }),
      "Branch path"
    );
  }

  if (clo_flags_ & YOGI_CLO_BRANCH_ADV_IFS) {
    visible_options_.add_options()(
      "adv-ifs", po::value<std::vector<std::string>>()->notifier([&](auto& val) {
        direct_json_["branch"]["advertising_interfaces"] = val;
      }),
      "Branch advertising interfaces (e.g. --adv-ifs 'eth0' --adv-ifs 'localhost' --adv-ifs '11:22:33:44:55:66')"
    );
  }

  if (clo_flags_ & YOGI_CLO_BRANCH_ADV_ADDR) {
    visible_options_.add_options()(
      "adv-addr", po::value<std::string>()->notifier([&](auto& val) {
        direct_json_["branch"]["advertising_address"] = val;
      }),
      "Branch advertising address (e.g. --adv-addr 'ff02::8000:2439')"
    );
  }

  if (clo_flags_ & YOGI_CLO_BRANCH_ADV_PORT) {
    visible_options_.add_options()(
      "adv-port", po::value<unsigned>()->notifier([&](auto& val) {
        direct_json_["branch"]["advertising_port"] = val;
      }),
      "Branch advertising port (e.g. --adv-port 13531"
    );
  }

  if (clo_flags_ & YOGI_CLO_BRANCH_ADV_INT) {
    visible_options_.add_options()(
      "adv-int", po::value<float>()->notifier([&](auto& val) {
        direct_json_["branch"]["advertising_interval"] = val;
      }),
      "Branch advertising interval in seconds (e.g. --adv-int 3.0)"
    );
  }

  if (clo_flags_ & YOGI_CLO_BRANCH_TIMEOUT) {
    visible_options_.add_options()(
      "timeout", po::value<float>()->notifier([&](auto& val) {
        direct_json_["branch"]["timeout"] = val;
      }),
      "Ghost mode"
    );
  }

  if (clo_flags_ & YOGI_CLO_BRANCH_GHOST_MODE) {
    visible_options_.add_options()(
      "ghost", po::value<bool>()->notifier([&](auto& val) {
        direct_json_["branch"]["ghost_mode"] = val;
      })->implicit_value(true),
      "Branch timeout in seconds (e.g. --timeout 3.0)"
    );
  }
  // clang-format on
}

void CmdlineParser::add_file_options() {
  // clang-format off
  if (clo_flags_ & YOGI_CLO_FILES || clo_flags_ & YOGI_CLO_FILES_REQUIRED) {
    auto name = "_cfg_files";
    hidden_options_.add_options()(
      name, po::value<std::vector<std::string>>()->notifier([&](auto& val) {
        this->file_notifier(val);
      }),
      "Configuration files (JSON format); multiple files will be merged"
      " according to JSON Merge Patch (RFC 7386) from left to right"
    );

    positional_options_.add(name, -1);
  }
  // clang-format on
}

void CmdlineParser::add_override_options() {
  // clang-format off
  if (clo_flags_ & YOGI_CLO_OVERRIDES) {
    visible_options_.add_options()(
      "override,o", po::value<std::vector<std::string>>()->notifier([&](auto& val) {
        this->override_notifier(val);
      }),
      "Configuration overrides in either JSON Merge Patch (RFC 7386) format"
      " (--override '{\"person\":{\"age\":42}}') or simplified using JSON"
      " Pointer (RFC 6901) format (--override '/person/age=42')"
    );
  }
  // clang-format on
}

void CmdlineParser::add_variable_options() {
  // clang-format off
  if (clo_flags_ & YOGI_CLO_VARIABLES) {
    visible_options_.add_options()(
      "var,v", po::value<std::vector<std::string>>()->notifier([&](auto& val) {
        this->variable_notifier(val);
      }),
      "Configuration variables (e.g. --var 'DIR=\"/usr/local\"')"
    );
  }
  // clang-format on
}

void CmdlineParser::populate_variables_map() {
  po::options_description options;
  options.add(visible_options_);
  options.add(hidden_options_);

  try {
    po::store(po::command_line_parser(argc_, argv_).options(options).positional(positional_options_).run(), vm_);
  } catch (const std::exception& e) {
    throw DescriptiveError{YOGI_ERR_PARSING_CMDLINE_FAILED} << e.what();
  }
}

void CmdlineParser::handle_help_options() {
  if (vm_.count("help")) {
    std::string binary_name = argv_[0];
    auto pos                = binary_name.find_last_of("/\\");
    if (pos != std::string::npos) {
      binary_name = binary_name.substr(pos + 1);
    }

    std::stringstream ss;
    ss << "Usage: " << binary_name << " [options]";
    if (clo_flags_ & YOGI_CLO_FILES || clo_flags_ & YOGI_CLO_FILES_REQUIRED) {
      ss << (clo_flags_ & YOGI_CLO_FILES_REQUIRED ? " config.json" : " [config.json]");
      ss << " [config2.json ...]";
    }
    ss << std::endl;

    ss << std::endl;
    ss << visible_options_ << std::endl;

    throw DescriptiveError{YOGI_ERR_HELP_REQUESTED} << ss.str();
  }

  if (vm_.count("help-logging")) {
    // clang-format off
    std::stringstream ss;
    ss << "Some of the logging switches support the placeholders listed below." << std::endl;
    ss << std::endl;
    ss << "Time placeholders:" << std::endl;
    ss << "  %Y - Four digit year" << std::endl;
    ss << "  %m - Month name as a decimal 01 to 12" << std::endl;
    ss << "  %d - Day of the month as decimal 01 to 31" << std::endl;
    ss << "  %F - Equivalent to %Y-%m-%d (the ISO 8601 date format)" << std::endl;
    ss << "  %H - The hour as a decimal number using a 24-hour clock (range 00 to 23)" << std::endl;
    ss << "  %M - The minute as a decimal 00 to 59" << std::endl;
    ss << "  %S - Seconds as a decimal 00 to 59" << std::endl;
    ss << "  %T - Equivalent to %H:%M:%S (the ISO 8601 time format)" << std::endl;
    ss << "  %3 - Milliseconds as decimal number 000 to 999" << std::endl;
    ss << "  %6 - Microseconds as decimal number 000 to 999" << std::endl;
    ss << "  %9 - Nanoseconds as decimal number 000 to 999" << std::endl;
    ss << std::endl;
    ss << "Entry placeholders:" << std::endl;
    ss << "  $t - Timestamp, formatted according to the configured time format" << std::endl;
    ss << "  $P - Process ID (PID)" << std::endl;
    ss << "  $T - Thread ID" << std::endl;
    ss << "  $s - Severity as a 3 letter abbreviation (FAT, ERR, WRN, IFO, DBG or TRC)" << std::endl;
    ss << "  $m - Log message" << std::endl;
    ss << "  $f - Source filename" << std::endl;
    ss << "  $l - Source line number" << std::endl;
    ss << "  $c - Component tag" << std::endl;
    ss << "  $< - Set console color corresponding to severity" << std::endl;
    ss << "  $> - Reset the colours (also done after each log entry)" << std::endl;
    ss << "  $$ - A $ sign" << std::endl;
    // clang-format on

    throw DescriptiveError(YOGI_ERR_HELP_REQUESTED) << ss.str();
  }
}

void CmdlineParser::extract_options() {
  try {
    po::notify(vm_);
  } catch (const po::error& e) {
    throw DescriptiveError{YOGI_ERR_PARSING_CMDLINE_FAILED} << e.what();
  }
}

void CmdlineParser::apply_overrides() {
  try {
    for (auto& ovr : overrides_) {
      if (ovr.json_pointer_syntax) {
        direct_json_[ovr.path] = ovr.value;
      } else {
        direct_json_.merge_patch(ovr.value);
      }
    }
  } catch (const nlohmann::json::exception& e) {
    throw DescriptiveError{YOGI_ERR_PARSING_CMDLINE_FAILED} << e.what();
  }
}

void CmdlineParser::load_config_files() {
  for (auto& file : config_files_) {
    std::ifstream f(file);
    if (!f.is_open()) {
      throw DescriptiveError{YOGI_ERR_PARSING_FILE_FAILED} << "Could not open " << file;
    }

    nlohmann::json json;
    try {
      f >> json;
    } catch (const std::exception& e) {
      throw DescriptiveError{YOGI_ERR_PARSING_FILE_FAILED} << "Could not parse " << file << ": " << e.what();
    }

    files_json_.merge_patch(json);
  }
}

void CmdlineParser::log_file_notifier(const std::string& val) {
  if (boost::to_upper_copy(val) == "NONE") {
    direct_json_["logging"]["file"] = nullptr;
  } else {
    direct_json_["logging"]["file"] = val;
  }
}

void CmdlineParser::log_console_notifier(const std::string& val) {
  auto s = boost::to_upper_copy(val);
  if (s != "STDERR" && s != "STDOUT" && s != "NONE") {
    throw DescriptiveError{YOGI_ERR_PARSING_CMDLINE_FAILED}
        << "Invalid value \"" << val << "\"for --log-console. Allowed values are STDOUT, STDERR and NONE.";
  }

  if (s == "NONE") {
    direct_json_["logging"]["console"] = nullptr;
  } else {
    direct_json_["logging"]["console"] = s;
  }
}

void CmdlineParser::log_verbosity_notifier(const std::vector<std::string>& val) {
  for (auto& str : val) {
    auto sep_pos = str.find('=');
    if (sep_pos == std::string::npos) {
      throw DescriptiveError{YOGI_ERR_PARSING_CMDLINE_FAILED} << "Invalid log verbosity string format \"" << str
                                                              << "\"";
    }

    auto comp = str.substr(0, sep_pos);
    auto verb = str.substr(sep_pos + 1);

    try {
      Logger::string_to_verbosity(verb);
    } catch (const std::exception& e) {
      throw DescriptiveError{YOGI_ERR_PARSING_CMDLINE_FAILED} << e.what();
    }

    direct_json_["logging"]["verbosity"][comp] = verb;
  }
}

void CmdlineParser::file_notifier(const std::vector<std::string>& val) {
  try {
    config_files_ = glob_files(val);
  } catch (const std::exception& e) {
    throw DescriptiveError{YOGI_ERR_PARSING_CMDLINE_FAILED} << e.what();
  }

  if (clo_flags_ & YOGI_CLO_FILES_REQUIRED && config_files_.empty()) {
    throw DescriptiveError{YOGI_ERR_PARSING_CMDLINE_FAILED} << "No configuration files specified.";
  }
}

void CmdlineParser::override_notifier(const std::vector<std::string>& val) {
  for (auto& str : val) {
    Override ovr;

    if (str.front() == '{') {
      ovr.json_pointer_syntax = false;

      try {
        ovr.value = nlohmann::json::parse(str);
      } catch (const std::exception& e) {
        throw DescriptiveError{YOGI_ERR_PARSING_CMDLINE_FAILED} << "Parsing \"" << str << "\" failed: " << e.what();
      }
    } else {
      ovr.json_pointer_syntax = true;

      auto sep_pos = str.find('=');
      if (sep_pos == std::string::npos) {
        throw DescriptiveError{YOGI_ERR_PARSING_CMDLINE_FAILED} << "Invalid override format \"" << str << "\"";
      }

      ovr.path       = nlohmann::json::json_pointer(str.substr(0, sep_pos));
      auto value_str = str.substr(sep_pos + 1);
      try {
        ovr.value = nlohmann::json::parse(value_str);
      } catch (const nlohmann::json::exception&) {
        ovr.value = value_str;
      }
    }

    overrides_.push_back(ovr);
  }
}

void CmdlineParser::variable_notifier(const std::vector<std::string>& val) {
  for (auto& str : val) {
    auto sep_pos = str.find('=');
    if (sep_pos == std::string::npos) {
      throw DescriptiveError{YOGI_ERR_PARSING_CMDLINE_FAILED} << "Invalid veriable format \"" << str << "\"";
    }

    auto name  = str.substr(0, sep_pos);
    auto value = str.substr(sep_pos + 1);

    auto& target = direct_json_["variables"][name];
    try {
      target = nlohmann::json::parse(value);
    } catch (const nlohmann::json::exception&) {
      target = value;
    }
  }
}
