#pragma once
#include <crow/http_request.h>
#include <crow/http_response.h>
#include <string>

namespace kb::proxy {

class Forwarder {
public:
  Forwarder();
  ~Forwarder();

  crow::response forward(const crow::request &original_request,
                         const std::string &url);
};

} // namespace kb::proxy
