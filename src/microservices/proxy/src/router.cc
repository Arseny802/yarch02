#include "proxy/router.h"
#include "forwarder.h"
#include <cpr/cpr.h>
#include <cpr/cprtypes.h>
#include <cpr/redirect.h>
#include <cpr/session.h>
#include <crow.h>

#include <crow/common.h>
#include <crow/logging.h>
#include <memory>
#include <utility>

namespace kb::proxy {
Router::Router(int port, MigrationSetup setup)
    : port_(port), setup_(std::move(setup)),
      routing_table_(
          {{MicroservicesPathes::EVENTS, setup_.url_service_events},
           {MicroservicesPathes::MOVIES, setup_.url_service_movies}}),
      proxy_(std::make_unique<Forwarder>()) {}

Router::~Router() = default;

bool Router::debug = false;
const std::string Router::MicroservicesPathes::MOVIES = "api/movies";
const std::string Router::MicroservicesPathes::EVENTS = "api/events";

void Router::start() {
  crow::SimpleApp app;
  app.loglevel(debug ? crow::LogLevel::Debug : crow::LogLevel::Info);
  CROW_LOG_INFO << "Migratio gradual: " << setup_.migration_gradual
                << "; percent = " << setup_.migration_percent;

  CROW_ROUTE(app, "/health").methods(crow::HTTPMethod::Get)([]() {
    return crow::response(200,
                          "{\"status\": \"ok\", \"service\": \"kb_proxy\"}");
  });

  CROW_ROUTE(app, "/<path>")
      .methods(crow::HTTPMethod::Get, crow::HTTPMethod::Post,
               crow::HTTPMethod::Put, crow::HTTPMethod::Patch,
               crow::HTTPMethod::Delete, crow::HTTPMethod::Head,
               crow::HTTPMethod::Options)(
          [this](const crow::request &request, const std::string &path) {
            const std::string url = route(path) + "/" + path;
            CROW_LOG_INFO << "Routing request with path '/" << path
                          << "' to URL '" << url << "'.";

            crow::response response = proxy_->forward(request, url);
            CROW_LOG_INFO << "Response status code " << response.code
                          << ", body size " << response.body.size()
                          << ", headers count " << response.headers.size();
            return response;
          });

  app.port(port_).multithreaded().run();
}

std::string Router::route(const std::string &path) const {
  if (setup_.migration_gradual && (rand() % 100 < setup_.migration_percent)) {
    auto iter = routing_table_.find(path);
    if (iter != routing_table_.end()) {
      return iter->second;
    }
  }

  return setup_.url_monolith;
}

} // namespace kb::proxy
