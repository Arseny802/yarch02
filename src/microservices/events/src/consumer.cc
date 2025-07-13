#include "events/consumer.h"
#include <kafka/KafkaConsumer.h>

#include <chrono>
#include <iostream>
#include <string>
#include <utility>


namespace kb::events {

Consumer::Consumer(std::string kafka_brokers)
    : kafka_brokers_(std::move(kafka_brokers)) {}
Consumer::~Consumer() = default;

void Consumer::start() {
  const kafka::Properties props({{"bootstrap.servers", {kafka_brokers_}}});

  kafka::clients::consumer::KafkaConsumer consumer(props);
  consumer.subscribe({kafka::Topic("payment-events"),
                      kafka::Topic("movie-events"),
                      kafka::Topic("user-events")});

  while (running_) {
    auto records = consumer.poll(std::chrono::milliseconds(100));

    for (const auto &record : records) {
      if (!record.error()) {
        std::cout << "Got a new message..." << std::endl;
        std::cout << "    Topic    : " << record.topic() << std::endl;
        std::cout << "    Partition: " << record.partition() << std::endl;
        std::cout << "    Offset   : " << record.offset() << std::endl;
        std::cout << "    Timestamp: " << record.timestamp().toString()
                  << std::endl;
        std::cout << "    Headers  : " << toString(record.headers())
                  << std::endl;
        std::cout << "    Key   [" << record.key().toString() << "]"
                  << std::endl;
        std::cout << "    Value [" << record.value().toString() << "]"
                  << std::endl;
      } else {
        std::cerr << record.toString() << std::endl;
      }
    }
  }

  consumer.close();
}

void Consumer::stop() { running_ = false; }

} // namespace kb::events
