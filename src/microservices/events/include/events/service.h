#pragma once
#include <memory>
#include <string>

namespace kb::events {
class Service {
public:
  Service(int port, std::string kafka_brokers);
  ~Service();

  void start();

  static bool debug;

private:
  const int port_;
  const std::unique_ptr<class Producer> producer_;
};
} // namespace kb::events
