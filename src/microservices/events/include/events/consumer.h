#pragma once
#include <atomic>
#include <string>

namespace kb::events {
class Consumer {
public:
  Consumer(std::string kafka_brokers);
  ~Consumer();

  void start();
  void stop();

private:
  std::atomic_bool running_;
  std::string kafka_brokers_;
};
} // namespace kb::events
