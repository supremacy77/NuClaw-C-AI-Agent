#pragma once
#include <string>
#include <vector>
#include <optional>
#include <functional>
#include <map>
#include "request.h"


using handler = std::function<std::string(const Request&)>;

struct RouteMatch{
    
    handler h;
    std::map<std::string, std::string> path_params; 
};


class Router {
public:
    void add_route(const std::string& method, const std::string& path, handler h);


    std::optional<RouteMatch> match(const std::string& method, const std::string& path) const;
private:
    struct Route {
        std::string method;
        std::string path;
        handler h;
        std::vector<std::string> path_parts;
    };
    std::vector<Route> routes_;
    
};

