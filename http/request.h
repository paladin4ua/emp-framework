//
// Created by Paladin on 22.01.2017.
//

#ifndef EMP_FRAMEWORK_REQUEST_H
#define EMP_FRAMEWORK_REQUEST_H

#include <memory>
#include <vector>
#include <map>

namespace emp {

    class base_context {
    public:
        virtual ~base_context() = 0;
    };

    class contexts_container {
    public:

        template<class Ctx>
        Ctx & get() {
            for (auto & ctx : container_) {
                if (auto finding_ctx = dynamic_cast<Ctx*>(ctx.get())){
                    return *finding_ctx;
                }
            }
            throw std::logic_error("given context is not setup properly. get function name: ");
        }

        template<class Ctx>
        Ctx & setup() {

            for (auto & ctx : container_) {
                if (auto finding_ctx = dynamic_cast<Ctx*>(ctx.get())){
                    return *finding_ctx;
                }
            }

            auto ctx = new Ctx;
            container_.emplace_back(ctx);
            return *ctx;
        }


    private:
        std::vector<std::unique_ptr<base_context>> container_;
    };

    namespace requset_method {
        const std::string get = "get";
        const std::string post = "post";
    }

    class request {
    public:
        std::string url;
        std::string method;
        std::string status_code;

        contexts_container contexts;

        std::map<std::string, std::string> route_params;
    };

    template <class T>
    T get_route_param(const request& req, const std::string & name) {
        auto i = req.route_params.find(name);
        if (i == req.route_params.end()) {
            throw std::logic_error("missed route parameter '" + name + "'");
        }
    }



    //typedef std::shared_ptr<request> response_ptr;
}


#endif //EMP_FRAMEWORK_REQUEST_H
