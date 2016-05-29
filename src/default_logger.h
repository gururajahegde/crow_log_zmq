#pragma once

#include <crow.h>
#include <string>
#include <spdlog/spdlog.h>

namespace spd = spdlog;

class DefaultLogger : public crow::ILogHandler {
  bool& logging;
  std::shared_ptr<spd::logger> console;

 public:
  DefaultLogger(bool& l) : logging(l),console(spd::stdout_logger_mt("cg3lz_crow", true)) {}

  inline void log(std::string message, crow::LogLevel level) override {
    if (!logging)
      return;
    switch (level) {
    case crow::LogLevel::CRITICAL:
      console->critical(message);
      break;
    case crow::LogLevel::DEBUG:
      console->debug(message);
      break;
    case crow::LogLevel::ERROR:
      console->error(message);
      break;
    case crow::LogLevel::INFO:
      console->info(message);
      break;
    case crow::LogLevel::WARNING:
      console->warn(message);
      break;
    default:
      console->info(message);
      break;

    }
  }
};
