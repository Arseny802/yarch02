#include "proxy/router.h"

#include <gtest/gtest.h>
#include <unordered_set>

namespace {
std::string url_monolith = "url_monolith";
std::string url_service_events = "url_service_events";
std::string url_service_movies = "url_service_movies";

kb::proxy::MigrationSetup get_default_setup() {
  kb::proxy::MigrationSetup setup;
  setup.migration_gradual = false;
  setup.migration_percent = 0;
  setup.url_monolith = url_monolith;
  setup.url_service_events = url_service_events;
  setup.url_service_movies = url_service_movies;
  return setup;
}
} // namespace

TEST(Router, RouterTestNoMonolithFalsePercent0) {
  kb::proxy::MigrationSetup setup = get_default_setup();
  setup.migration_gradual = false;
  setup.migration_percent = 0;

  kb::proxy::Router router(0, setup);

  EXPECT_EQ(router.route(kb::proxy::Router::MicroservicesPathes::MOVIES),
            url_monolith);
  EXPECT_EQ(router.route(kb::proxy::Router::MicroservicesPathes::EVENTS),
            url_monolith);
}

TEST(Router, RouterTestNoMonolithFalsePercent50) {
  kb::proxy::MigrationSetup setup = get_default_setup();
  setup.migration_gradual = false;
  setup.migration_percent = 50;

  kb::proxy::Router router(0, setup);

  EXPECT_EQ(router.route(kb::proxy::Router::MicroservicesPathes::MOVIES),
            url_monolith);
  EXPECT_EQ(router.route(kb::proxy::Router::MicroservicesPathes::EVENTS),
            url_monolith);
}

TEST(Router, RouterTestNoMonolithFalsePercent100) {
  kb::proxy::MigrationSetup setup = get_default_setup();
  setup.migration_gradual = false;
  setup.migration_percent = 100;

  kb::proxy::Router router(0, setup);

  EXPECT_EQ(router.route(kb::proxy::Router::MicroservicesPathes::MOVIES),
            url_monolith);
  EXPECT_EQ(router.route(kb::proxy::Router::MicroservicesPathes::EVENTS),
            url_monolith);
}

TEST(Router, RouterTestNoMonolithTruePercent0) {
  kb::proxy::MigrationSetup setup = get_default_setup();
  setup.migration_gradual = true;
  setup.migration_percent = 0;

  kb::proxy::Router router(0, setup);

  EXPECT_EQ(router.route(kb::proxy::Router::MicroservicesPathes::MOVIES),
            url_monolith);
  EXPECT_EQ(router.route(kb::proxy::Router::MicroservicesPathes::EVENTS),
            url_monolith);
}

TEST(Router, RouterTestNoMonolithTruePercent50) {
  kb::proxy::MigrationSetup setup = get_default_setup();
  setup.migration_gradual = true;
  setup.migration_percent = 50;

  kb::proxy::Router router(0, setup);

  constexpr int count = 10;
  std::unordered_set<std::string> urls;

  urls = {setup.url_monolith, setup.url_service_movies};
  for (int i = 0; i < count && !urls.empty(); ++i) {
    urls.erase(router.route(kb::proxy::Router::MicroservicesPathes::MOVIES));
  }
  EXPECT_TRUE(urls.empty()) << "Each url should be visited at least once";

  urls = {setup.url_monolith, setup.url_service_events};
  for (int i = 0; i < count && !urls.empty(); ++i) {
    urls.erase(router.route(kb::proxy::Router::MicroservicesPathes::EVENTS));
  }
  EXPECT_TRUE(urls.empty()) << "Each url should be visited at least once";
}

TEST(Router, RouterTestNoMonolithTruePercent100) {
  kb::proxy::MigrationSetup setup = get_default_setup();
  setup.migration_gradual = true;
  setup.migration_percent = 100;

  kb::proxy::Router router(0, setup);

  EXPECT_EQ(router.route(kb::proxy::Router::MicroservicesPathes::MOVIES),
            url_service_movies);
  EXPECT_EQ(router.route(kb::proxy::Router::MicroservicesPathes::EVENTS),
            url_service_events);
}

TEST(Router, RouterTestNoMonolithTruePercent50Resetup) {
  kb::proxy::MigrationSetup setup = get_default_setup();
  setup.migration_gradual = true;
  setup.migration_percent = 50;
  setup.url_monolith = "haha 00";
  setup.url_service_events = "haha 01";
  setup.url_service_movies = "haha 02";

  kb::proxy::Router router(0, setup);

  constexpr int count = 10;
  std::unordered_set<std::string> urls;

  urls = {setup.url_monolith, setup.url_service_movies};
  for (int i = 0; i < count && !urls.empty(); ++i) {
    urls.erase(router.route(kb::proxy::Router::MicroservicesPathes::MOVIES));
  }
  EXPECT_TRUE(urls.empty()) << "Each url should be visited at least once";

  urls = {setup.url_monolith, setup.url_service_events};
  for (int i = 0; i < count && !urls.empty(); ++i) {
    urls.erase(router.route(kb::proxy::Router::MicroservicesPathes::EVENTS));
  }
  EXPECT_TRUE(urls.empty()) << "Each url should be visited at least once";
}
