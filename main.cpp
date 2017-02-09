#include <iostream>
#include <fstream>

#include "http/router.h"
#include <snapshot_db/writer.h>
#include <middleware/render.h>

#include "test_model.h"
#include <chrono>



struct sub_obj1 {
    std::string fld2;

    template <class T>
    void bind(T & t) {
        t.bind(fld2, "fld2");
    }
};

struct test_model {
    int fld1;
    std::vector<sub_obj1> objs1;
    //std::map<std::string,sub_obj1> objs2;

    template <class T>
    void bind(T & t) {
        t.bind(fld1, "fld1");
        t.bind(objs1, "objs1");
       // t.bind(objs2, "objs2");
    }
};


struct Test_Controller {

    struct get_test_params {
        int id;
        template <class T>
        void bind(T & t) {
            t.bind(id, "id");
        }
    };

    void get_test_object(emp::request & req, emp::response_ptr res);
};

void Test_Controller::get_test_object(emp::request & req, emp::response_ptr res) {

    auto& render_ctx = emp::render_middleware::ctx(req);
    //auto& user = auth_middleware::user(req);
    //auto& input_model = emp::body_parser_middleware::get_model<test_model>(req);
    //auto input_model = parse_json<test_model>( body(req), render_ctx.vr());
    //auto input_model = parse_body_json<test_model>( req, render_ctx.vr());
    //auto input_model = parse_input_model<test_model>( req, render_ctx.vr());
    //auto input_model = get_input_model<test_model>( req, render_ctx.vr());
    auto id =  emp::get_route_param<int>(req, "id");
    test_model model;

    render_ctx.render_json(model, res);
}

using namespace std::chrono;

int main() {
    std::cout << "Hello, World!" << std::endl;

    json_fast_writer writer;

    //test_model model = {10, { { "test"}}, { { "code", {"test"} } }};
    test_model model = {10, { { "test"}} };

    writer(model);

    std::cout << writer.release_buffer() << std::endl;


    auto large_m = build_test_model(1000000);



    metadata_writer meta_writer;
    snapshot_writer snapshot;

    meta_writer(large_m);

    snapshot.reserve(1500 * 1000 * 1000);

    microseconds min_duration = microseconds::max();

    for (int i = 0; i < 10; ++i) {

        snapshot.reset();
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        snapshot(large_m);
        high_resolution_clock::time_point t2 = high_resolution_clock::now();


        auto duration = duration_cast<microseconds>( t2 - t1 );

        min_duration = std::min(min_duration, duration);

    }


    std::cout << meta_writer.release_buffer() << std::endl;
    std::cout << snapshot.writed_bytes() << " bytes " << std::endl;
    std::cout << min_duration.count() << " microseconds " << std::endl;

    std::ofstream fout("test.txt", std::ios::binary | std::ios_base::out| std::ios_base::trunc);

    fout.write(snapshot.buffer(), snapshot.writed_bytes());


    emp::router r;

    Test_Controller c;

    r.get("/", &c, &Test_Controller::get_test_object );

    return 0;
}