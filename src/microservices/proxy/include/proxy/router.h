#pragma once
#include <memory>
#include <string>
#include <unordered_map>

#include "migration_setutp.h"

namespace kb::proxy {

class Router {
public:
  Router(int port, MigrationSetup setup);
  ~Router();

  void start();

  std::string route(const std::string &path) const;

  static bool debug;

  struct MicroservicesPathes {
    static const std::string MOVIES;
    static const std::string EVENTS;
  };

private:
  const int port_;
  const MigrationSetup setup_;
  const std::unordered_map<std::string, std::string> routing_table_;

  const std::unique_ptr<class Forwarder> proxy_;
};

} // namespace kb::proxy
