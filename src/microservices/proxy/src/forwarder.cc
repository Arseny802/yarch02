#include "forwarder.h"
#include <algorithm>
#include <cpr/response.h>
#include <cpr/session.h>
#include <unordered_set>

namespace kb::proxy {
namespace {

cpr::Response apply_method(crow::HTTPMethod method, cpr::Session &session) {
  switch (method) {
  case crow::HTTPMethod::Get:
    return session.Get();
  case crow::HTTPMethod::Post:
    return session.Post();
  case crow::HTTPMethod::Put:
    return session.Put();
  case crow::HTTPMethod::Patch:
    return session.Patch();
  case crow::HTTPMethod::Delete:
    return session.Delete();
  case crow::HTTPMethod::Head:
    return session.Head();
  case crow::HTTPMethod::Options:
    return session.Options();
  default:
    CROW_LOG_CRITICAL << "Unknown HTTP method";
  }

  return {}; // Not Implemented
}
} // namespace

Forwarder::Forwarder() = default;
Forwarder::~Forwarder() = default;

crow::response Forwarder::forward(const crow::request &original_request,
                                  const std::string &url) {
  cpr::Session session;
  session.SetUrl(url);
  session.SetBody(std::move(original_request.body));

  cpr::Header headers;
  std::for_each(
      original_request.headers.begin(), original_request.headers.end(),
      [&headers](auto &&pair) {
        CROW_LOG_DEBUG << ">> Header '" << pair.first << "' with value '"
                       << pair.second << "' added.";
        headers.emplace(std::move(pair.first), std::move(pair.second));
      });
  session.SetHeader(std::move(headers));

  cpr::Response original_response =
      apply_method(original_request.method, session);
  if (original_response.status_code <= 0) {
    return crow::response(501);
  }

  crow::response redirected_response(original_response.status_code,
                                     std::move(original_response.text));

  std::for_each(
      original_response.header.begin(), original_response.header.end(),
      [&redirected_response](auto &&pair) {
        static const std::unordered_set<std::string> excluded_headers = {
            "Content-Encoding", "Content-Length", "Transfer-Encoding",
            "Connection"};

        if (excluded_headers.find(pair.first) != excluded_headers.end()) {
          CROW_LOG_DEBUG << "<< Header '" << pair.first << "' with value '"
                         << pair.second << "' EXCLUDED.";
          return;
        }

        CROW_LOG_DEBUG << "<< Header '" << pair.first << "' with value '"
                       << pair.second << "' added.";
        redirected_response.headers.emplace(std::move(pair.first),
                                            std::move(pair.second));
      });

  return redirected_response;
}

} // namespace kb::proxy
