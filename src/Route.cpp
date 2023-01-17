//
// Created by michele on 18.01.23.
//

#include "Route.h"
#include "utils.h"

#include <utility>

namespace Espresso {

Route::Route(const std::string &path, HTTPMethod method, RouteCallback callback)
    : path(path), method(method), callback(std::move(callback)) {
  std::vector<std::string> parts = split(path, '/');
  for (const auto &part : parts) {
    if (part.length() > 1 && part[0] == ':') {
      params.push_back(part.substr(1));
    }
  }
}

Route::~Route() = default;

}