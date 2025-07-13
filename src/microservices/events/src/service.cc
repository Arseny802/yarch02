#include "events/service.h"
#include "events/producer.h"
#include <crow.h>

#include <memory>
#include <utility>

namespace kb::events {

namespace {
const std::string JSON_SUCCESS = "{\"status\": \"success\"}";
const std::string JSON_SUCCESS_HEALTH =
    "{\"status\": true, \"service\": \"kb_events\"}";
} // namespace

Service::Service(int port, std::string kafka_brokers)
    : port_(port),
      producer_(std::make_unique<Producer>(std::move(kafka_brokers))) {}

Service::~Service() = default;

bool Service::debug = false;

void Service::start() {
  crow::SimpleApp app;
  app.loglevel(debug ? crow::LogLevel::Debug : crow::LogLevel::Info);

  CROW_ROUTE(app, "/api/events/health").methods(crow::HTTPMethod::Get)([]() {
    return crow::response(200, "application/json", JSON_SUCCESS_HEALTH);
  });

  CROW_ROUTE(app, "/api/events/user")
      .methods(crow::HTTPMethod::Post)([this](const crow::request &request) {
        producer_->write_user(request.body);
        return crow::response(201, "application/json", JSON_SUCCESS);
      });

  CROW_ROUTE(app, "/api/events/movie")
      .methods(crow::HTTPMethod::Post)([this](const crow::request &request) {
        CROW_LOG_INFO << static_cast<int>(request.method) << "; "
                      << request.body;
        producer_->write_movie(request.body);
        return crow::response(201, "application/json", JSON_SUCCESS);
      });

  CROW_ROUTE(app, "/api/events/payment")
      .methods(crow::HTTPMethod::Post)([this](const crow::request &request) {
        producer_->write_payment(request.body);
        return crow::response(201, "application/json", JSON_SUCCESS);
      });

  app.port(port_).multithreaded().run();
}

} // namespace kb::events
