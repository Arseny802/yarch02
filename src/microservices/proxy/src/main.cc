#include <argparse/argparse.hpp>

#include <cstdlib>
#include <iostream>
#include <string>
#include <utility>

#include "proxy/router.h"

std::string get_env_var(const std::string &key) {
  char *val = std::getenv(key.c_str());
  return val == NULL ? std::string("") : std::string(val);
}

int main(int argc, char *argv[]) {
  argparse::ArgumentParser program("KinoBezdna API Gateway");
  kb::proxy::MigrationSetup setup;
  int port = 8000;

  program.add_argument("-p", "--port")
      .help("HTTP port to listen on")
      .scan<'i', int>()
      .default_value(port)
      .store_into(port);

  program.add_argument("-d", "--debug")
      .help("Is gradual migration enabled")
      .implicit_value(true)
      .default_value(false)
      .store_into(kb::proxy::Router::debug);

  program.add_argument("--monolith-url")
      .help("Monolith URL")
      .default_value(std::string("http://monolith:8080"))
      .store_into(setup.url_monolith);

  program.add_argument("--movies-service-url")
      .help("Movies URL")
      .default_value(std::string("http://movies-service:8081"))
      .store_into(setup.url_service_movies);

  program.add_argument("--events-service-url")
      .help("Events service URL")
      .default_value(std::string("http://events-service:8082"))
      .store_into(setup.url_service_events);

  program.add_argument("--gradual-migration")
      .help("Is gradual migration enabled")
      .default_value(false)
      .store_into(setup.migration_gradual);

  program.add_argument("--movies-migration-percent")
      .help("Percent of movies service migration")
      .scan<'i', int>()
      .default_value(0)
      .store_into(setup.migration_percent);

  try {
    program.parse_args(argc, argv);
  } catch (const std::exception &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    return EXIT_FAILURE;
  }

  kb::proxy::Router(port, std::move(setup)).start();
  return EXIT_SUCCESS;
}