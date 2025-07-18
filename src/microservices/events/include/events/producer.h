#pragma once
#include <atomic>
#include <memory>
#include <string>

namespace kafka::clients::producer {
class KafkaProducer;
} // namespace kafka::clients::producer

namespace kb::events {
class Producer {
public:
  Producer(std::string kafka_brokers);
  ~Producer();

  void write_user(std::string message);
  void write_movie(std::string message);
  void write_payment(std::string message);

private:
  std::string kafka_brokers_;

  std::unique_ptr<kafka::clients::producer::KafkaProducer> producer_;
};
} // namespace kb::events
