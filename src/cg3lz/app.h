#pragma once

#include <crow.h>

#include "../common/config.h"
#include "default_logger.h"
#include "config_persistence.h"

#include "main_page.h"
#include "log_view.h"

#include <memory>
#include <string>
#include <atomic>
#include <cstdint>

class spdlogger;
class zeromq_log_source;
class websocket_ticker;

namespace zmq { class context_t; }

class simple_log_server {
    zmq::context_t& ctx;
    config cfg;
    config_persistence cfg_persistence;
    DefaultLogger default_log;
    std::unique_ptr<spdlogger> sink;
    std::unique_ptr<zeromq_log_source> source;
    std::unique_ptr<websocket_ticker> ticker;
    crow::SimpleApp app;

    std::unique_ptr<main_page> index;
    std::unique_ptr<log_view> logs;

    std::atomic<std::uint64_t> count;

public:
    simple_log_server(zmq::context_t& ctx);
    ~simple_log_server();
public:
    void log(std::string const& m);
    void run();
    void shutdown();
private:
    void forward_log(std::string const& m);
    void tick();
    void load_config();
    void save_config();
    void configure_sink();
    void configure_source();
    void configure_app_routing();
    void configure_app_logging();
    void configure_source_logging();
    void configure_views();
    void check_configuration();
    void configure_websocket_ticker();
private:
    void add_front_page();
    void add_logging_rest_endpoint();
    void add_crow_logging_toggle();
    void add_kill_switch();
    void add_naive_log_file_download();
    void add_websocket_ticker();
};
