//
// Created by paladin on 1/26/2017.
//

#ifndef EMP_FRAMEWORK_JSON_FAST_WRITER_H
#define EMP_FRAMEWORK_JSON_FAST_WRITER_H

#include <http/response.h>
#include <string>

class json_fast_writer {

    class fields_writer {

        json_fast_writer &parent_writer_;
        bool is_first_field;
    public:
        fields_writer(json_fast_writer &parent_writer) : parent_writer_(parent_writer), is_first_field(true) {}


        template<class T>
        void bind(T &obj, const char *fld_name) {

            if (!is_first_field)
                parent_writer_.buffer_ += ',';
            else
                is_first_field = false;

            parent_writer_.buffer_ += fld_name;
            parent_writer_.buffer_ += ':';
            parent_writer_(obj);
        }
    };

    template<class T>

    static void write(T &obj, json_fast_writer &parent) {
        parent.buffer_ += '{';
        fields_writer writer(parent);
        obj.bind(writer);
        parent.buffer_ += '}';
    }


    template<class T>
    static void write(std::map<std::string, T> &obj, json_fast_writer &parent) {
        parent.buffer_ += '{';
        for (auto i = obj.begin(), end = obj.end(); i != end; ++i) {
            if (i != obj.begin()) {
                parent.buffer_ += ',';
            }

            parent.buffer_ += i->first;
            parent.buffer_ += ':';
            parent(i->second);
        }
        parent.buffer_ += '}';
    }


    template<class T>
    static void write(std::vector<T> &array, json_fast_writer &parent) {
        parent.buffer_ += '[';
        for (auto i = array.begin(), end = array.end(); i != end; ++i) {
            if (i != array.begin()) {
                parent.buffer_ += ',';
            }
            parent(*i);
        }
        parent.buffer_ += ']';
    }


public:


    template<class T>
    void operator()(T &obj) {
        write(obj, *this);
    }


    void operator()( std::string &val) {
        buffer_ += '"';
        buffer_ += val;
        buffer_ += '"';
    }

    void operator()(int32_t val) {
        buffer_ += std::to_string(val);
    }

    void operator()(int64_t val) {
        buffer_ += std::to_string(val);
    }

    void operator()(float val) {
        buffer_ += std::to_string(val);
    }

    void operator()(double val) {
        buffer_ += std::to_string(val);
    }

    void operator()(decltype(nullptr) ) {
        buffer_ += "null";
    }


    std::string release_buffer() {
        return std::move(buffer_);
    }

private:
    std::string buffer_;
};


#endif //EMP_FRAMEWORK_JSON_FAST_WRITER_H
