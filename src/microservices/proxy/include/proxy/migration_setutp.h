#pragma once
#include <string>

namespace kb::proxy {
struct MigrationSetup {

  int migration_percent = 0;
  bool migration_gradual = false;

  std::string url_monolith;
  std::string url_service_events;
  std::string url_service_movies;
};
} // namespace kb::proxy
