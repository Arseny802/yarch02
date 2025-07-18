#include <argparse/argparse.hpp>

#include <cstdlib>
#include <string>
#include <utility>


#include "events/consumer.h"
#include "events/service.h"

int main(int argc, char *argv[]) {
  argparse::ArgumentParser program("KinoBezdna API Gateway");
  // kb::proxy::MigrationSetup setup;
  int port = 8082;
  std::string kafka_brokers;

  program.add_argument("-p", "--port")
      .help("HTTP port to listen on")
      .scan<'i', int>()
      .default_value(port)
      .store_into(port);

  program.add_argument("-d", "--debug")
      .help("Is gradual migration enabled")
      .implicit_value(true)
      .default_value(false)
      .store_into(kb::events::Service::debug);

  program.add_argument("--kafka-brokers")
      .help("Monolith URL")
      .default_value(std::string("kafka:9092"))
      .store_into(kafka_brokers);

  try {
    program.parse_args(argc, argv);
  } catch (const std::exception &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    return EXIT_FAILURE;
  }

  kb::events::Service service(port, kafka_brokers);
  kb::events::Consumer consumer(std::move(kafka_brokers));

  consumer.start();
  service.start();

  return EXIT_SUCCESS;
}