#include "events/producer.h"
#include <kafka/KafkaProducer.h>

#include <iostream>
#include <memory>
#include <string>
#include <utility>

namespace kb::events {

namespace {
void delivery_callback(const kafka::clients::producer::RecordMetadata &metadata,
                       const kafka::Error &error) {
  if (!error) {
    std::cout << "Message delivered: " << metadata.toString() << std::endl;
  } else {
    std::cerr << "Message failed to be delivered: " << error.message()
              << std::endl;
  }
}
} // namespace

Producer::Producer(std::string kafka_brokers)
    : kafka_brokers_(std::move(kafka_brokers)) {
  const kafka::Properties props({{"bootstrap.servers", kafka_brokers_}});
  producer_ = std::make_unique<kafka::clients::producer::KafkaProducer>(props);
}
Producer::~Producer() = default;

void Producer::write_user(std::string message) {
  static const kafka::Topic topic = "user-events";

  const kafka::clients::producer::ProducerRecord record(
      topic, kafka::NullKey, kafka::Value(message.c_str(), message.size()));

  producer_->send(record, delivery_callback);
}

void Producer::write_movie(std::string message) {
  static const kafka::Topic topic = "movie-events";

  const kafka::clients::producer::ProducerRecord record(
      topic, kafka::NullKey, kafka::Value(message.c_str(), message.size()));
  producer_->send(record, delivery_callback);
}

void Producer::write_payment(std::string message) {
  static const kafka::Topic topic = "payment-events";

  const kafka::clients::producer::ProducerRecord record(
      topic, kafka::NullKey, kafka::Value(message.c_str(), message.size()));
  producer_->send(record, delivery_callback);
}

} // namespace kb::events
