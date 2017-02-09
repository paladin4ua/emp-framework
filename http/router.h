//
// Created by Paladin on 22.01.2017.
//

#ifndef EMP_FRAMEWORK_ROUTER_H
#define EMP_FRAMEWORK_ROUTER_H

#include "request.h"
#include "response.h"
#include <functional>
#include <map>
#include <vector>

namespace emp {

    typedef std::function<void ()> next_t;
    typedef std::function<void (request & req, response_ptr res , next_t next)> handler_t;

    namespace implementation {

        template<class... Args>
        class handlers_aggregator;

        template<class A0, class... Args>
        class handlers_aggregator<A0, Args...> : handlers_aggregator<Args...> {

            typedef handlers_aggregator<Args...> base;

            handler_t handler;

            /* template <class T>
             void handle(request & req, response_ptr res , T next) const {
                 handler(req, res, [=]() {
                     base::handle(req, res, next);
                 });
             }*/

        public:

            handlers_aggregator(A0 && a0, Args&&...args) : base(std::forward<Args>(args)...), handler(a0) {
            }

            template<class T>
            void operator()(request & req, response_ptr res , T next) const {
                handler(req, res, [=]() {
                    base::operator()(req, res, next);
                });
            }

        };


        template<class A0>
        class handlers_aggregator<A0> {

            std::function<void (request & req, response_ptr res)> handler;

        public:

            handlers_aggregator(A0 && a0) : handler(a0) {
            }

            template<class T>
            void operator()(request & req, response_ptr res , T next) const {
                handler(req, res);
            }

        };

        template<class A0>
        class handlers_aggregator<A0*, void(A0::*)(request & req, response_ptr res) > {

            typedef void(A0::* member_fn)(request & req, response_ptr res);
            A0* this_;
            member_fn member_fn_;

        public:

            handlers_aggregator(A0* a0, member_fn membr_fn) : this_(a0), member_fn_(membr_fn) {
            }

            template<class T>
            void operator()(request & req, response_ptr res , T next) const {
                (this_->*member_fn_)(req, res);
            }

        };
    }

    class router {
    public:

        router()  {}

        void operator() (request & req, response_ptr res , next_t next);


        template <class... Args>
        void get(std::string path, Args&&...args) {
            method(requset_method::get, path, std::forward<Args>(args)...);
        }

        template <class... Args>
        void post(std::string path, Args&&...args) {
            method(requset_method::post, path, std::forward<Args>(args)...);
        }



        void use(std::string prefix, handler_t middleware) {
            middleware_routes_.push_back( middleware_route_t{prefix, middleware});
        }

        void use(handler_t middleware) {
            use("", middleware);
        }

    private:

        template <class... Args>
        void method(std::string method, std::string path, Args&&...args) {

            auto handler = implementation::handlers_aggregator<Args...>( std::forward<Args>(args)...);
            routes_.push_back(route_t{ std::move(method), std::move(path), std::move(handler)});

        }

    private:

        bool match_prefix(std::string prefix, std::string url) const ;

        struct route_t
        {
            std::string method;
            std::string path;
            handler_t handler;
        };

        struct middleware_route_t
        {
            std::string prefix;
            handler_t middleware;
        };

        std::vector<route_t> routes_;
        std::vector<middleware_route_t> middleware_routes_;

    };
}


#endif //EMP_FRAMEWORK_ROUTER_H
