//
// Created by Paladin on 22.01.2017.
//

#include "router.h"
#include "3d-party/route.h"

namespace emp {

    void router::operator() (request & req, response_ptr res, next_t next)
    {

        route::Route route_matcher;
        auto match = route_matcher.set(req.url);

        for (auto i = routes_.rbegin(); i != routes_.rend(); ++i) {
            if (req.method == i->method && match.test(i->path)) {
                next = [=, &req]() {
                    i->handler(req, res, next);
                };
            }
        }


        for (auto i = middleware_routes_.rbegin(); i != middleware_routes_.rend(); ++i) {
            if (match_prefix(i->prefix, req.url)) {
                next = [=, &req]() {
                    i->middleware(req, res, next);
                };
            }
        }

        next();
    }

    bool router::match_prefix(std::string prefix, std::string url) const {
        return std::mismatch(prefix.begin(), prefix.end(), url.begin(), url.end()).first == prefix.end();
    }
}