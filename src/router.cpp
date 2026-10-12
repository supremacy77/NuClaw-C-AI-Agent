#pragma once 
#include "router.h"
#include <sstream>
#include <regex>


void router::add_route(const std::string& method, const std::string& path, handler h) {
    Router route;
    route.method = method;
    route.path = path;
    route.h = h;

    std::istringstream stream(path);
    std::string part;
    while (std::getline(stream, part, '/')) {
        if (!part.empty()) {
            route.path_parts.push_back(part);
        }
    }

    routes_.push_back(route);
}

std::optional<RouteMatch> router::match(const std::string& method, const std::string& path) const {
    for(const auto& route : routes_) {
        if (route.method != "*" && route.method != method) {
            continue;
        }

        std::regex route_regex(route.path);
        std::smatch match_result;

        if (std::regex_match(path, match_result, route_regex)) {
            RouteMatch result;
            result.h = route.h;

            for (size_t i = 1; i < match_result.size(); ++i) {
                std::string param_name = "param" + std::to_string(i);
                result.path_params[param_name] = match_result[i].str();
            }

            return result;
        }
    }
    return std::nullopt;
}

