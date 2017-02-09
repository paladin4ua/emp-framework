//
// Created by paladin on 1/26/2017.
//

#ifndef EMP_FRAMEWORK_CONTEXT_H
#define EMP_FRAMEWORK_CONTEXT_H

#include <http/request.h>
#include <http/response.h>
#include <http/router.h>
#include <memory>
#include <validation/validation_result.h>
#include <json/json_fast_writer.h>

namespace emp {

    template <class Model>
    struct output_model {
        const Model &model;
        const std::map<std::string, std::string> &errors;
        bool success;

        template <class T>
        void bind(T & t) {
            t.bind(model, "model");
            t.bind(errors, "errors");
            t.bind(success, "success");
        }
    };

    class render_context : public base_context {
        validation_result validation_result_;

    public:
        validation_result & vr() {
            return validation_result_;
        }

        template <class Model>
        void render_json(Model && model, response_ptr res) {

            res->add_header("Content-Type", "application/json");

            json_fast_writer writer;
            output_model<Model> output = {model, validation_result_.get_errors(), !!validation_result_};
            writer(model);
            res->write(writer.release_buffer());
        }
    };

    class render_middleware {

    public:
        static render_context & ctx(request & req) {
            return req.contexts.get<render_context>();
        }

        void operator()(request & req, response_ptr res, next_t next) const {

            auto & ctx = req.contexts.setup<render_context>();




        }
    };
}


#endif //EMP_FRAMEWORK_CONTEXT_H
