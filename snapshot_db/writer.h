//
// Created by paladin on 2/6/2017.
//

#ifndef EMP_FRAMEWORK_WRITER_H
#define EMP_FRAMEWORK_WRITER_H

#include <vector>
#include <string>
#include <boost/optional.hpp>
#include <memory>

class metadata_writer {

    class fields_writer {

        metadata_writer &parent_writer_;
        bool is_first_field;
    public:
        fields_writer(metadata_writer &parent_writer) : parent_writer_(parent_writer), is_first_field(true) {}


        template<class T>
        void bind(T &obj, const char *fld_name) {

            if (!is_first_field)
                parent_writer_.buffer_ += ',';
            else
                is_first_field = false;

            parent_writer_.buffer_ += '"';
            parent_writer_.buffer_ += fld_name;
            parent_writer_.buffer_ += '"';
            parent_writer_.buffer_ += ':';
            parent_writer_(obj);
        }
    };

    template<class T>
    static void write(T &obj, metadata_writer &parent) {
        parent.buffer_ += '{';
        fields_writer writer(parent);
        obj.bind(writer);
        parent.buffer_ += '}';
    }

    template<class T>
    static void write(std::vector<T> &, metadata_writer &parent) {
        parent.buffer_ += '[';
        T obj;
        parent(obj);
        parent.buffer_ += ']';
    }

    template<class T>
    static void write(boost::optional<T> &, metadata_writer &parent) {
        parent.buffer_ += '[';
        T obj;
        parent(obj);
        parent.buffer_ += ']';
    }


    static void write( std::string &, metadata_writer &parent) {
        parent.buffer_ += '"';
        parent.buffer_ += "std::string";
        parent.buffer_ += '"';
    }

    static void write(int8_t , metadata_writer &parent) {
        parent.buffer_ += '"';
        parent.buffer_ += "int8_t";
        parent.buffer_ += '"';
    }

    static void write(int32_t , metadata_writer &parent) {
        parent.buffer_ += '"';
        parent.buffer_ += "int32_t";
        parent.buffer_ += '"';
    }


    static void write(int64_t , metadata_writer &parent) {
        parent.buffer_ += '"';
        parent.buffer_ += "int64_t";
        parent.buffer_ += '"';
    }


    static void write(float , metadata_writer &parent) {
        parent.buffer_ += '"';
        parent.buffer_ += "float";
        parent.buffer_ += '"';
    }


    static void write(double , metadata_writer &parent) {
        parent.buffer_ += '"';
        parent.buffer_ += "double";
        parent.buffer_ += '"';
    }



public:

    template<class T>
    void operator()(T &obj) {
        write(obj, *this);
    }

    std::string release_buffer() {
        return std::move(buffer_);
    }

private:
    std::string buffer_;
};

class snapshot_writer {

    class fields_writer {

        snapshot_writer &parent_writer_;
        bool is_first_field;
    public:
        fields_writer(snapshot_writer &parent_writer) : parent_writer_(parent_writer), is_first_field(true) {}


        template<class T>
        void bind(T &obj, const char *fld_name) {

            parent_writer_(obj);
        }
    };

    template<class T>
    static void write(T &obj, snapshot_writer &parent) {
        parent.write_quote('{');
        fields_writer writer(parent);
        obj.bind(writer);
        parent.write_quote('}');
    }

    template<class T>
    static void write(std::vector<T> & array, snapshot_writer &parent) {
        parent.write_quote('[');
        write((int64_t)array.size(), parent);

        for (auto i = array.begin(), end = array.end(); i != end; ++i) {
            parent(*i);
        }
        parent.write_quote(']');
    }

    template<class T>
    static void write(boost::optional<T> & val, snapshot_writer &parent) {
        parent.write_quote('[');
        write( val ? 1 : 0, parent);

        if (val)
            parent(*val);

        parent.write_quote (']');
    }


    static void write( std::string & val, snapshot_writer &parent) {
        parent.write_buffer('"', val.data(), val.length());
    }


    static void write(int32_t val, snapshot_writer &parent) {
        parent.write_buffer('"', val);
    }

    static void write(int8_t val, snapshot_writer &parent) {
        parent.write_buffer('"', val);
    }

    static void write(int64_t val, snapshot_writer &parent) {
        parent.write_buffer('"', val);
    }


    static void write(float val, snapshot_writer &parent) {
        parent.write_buffer('"', val);
    }


    static void write(double val, snapshot_writer &parent) {
        parent.write_buffer('"', val);
    }

    inline void write_buffer(char quote, const char * str, int len) {
        auto new_position = current_position_ + 2 + len;
        if (new_position > buffer_size_) {
            reserve(int(new_position * 1.5));
        }
        buffer_[current_position_] = quote;
        std::copy(str, str + len, buffer_.get() + current_position_ +  1);
        buffer_[current_position_ + 1 + len] = quote;
        current_position_ = new_position;
    }

    template <class T>
    inline void write_buffer(char quote, T value) {
        const int len = sizeof(T) * 2;
        const uint8_t * pval = reinterpret_cast<uint8_t* >(&value);
        auto new_position = current_position_ + 2 + len;
        if (new_position > buffer_size_) {
            reserve(int(new_position * 1.5));
        }
        buffer_[current_position_] = quote;
        for (int i = 0; i < sizeof(T); ++i) {
            buffer_[current_position_ + i * 2 + 1] = (pval[i] & 0x0F) + 65;
            buffer_[current_position_ + i * 2 + 2] = (pval[i] >> 4 & 0x0F)  + 65;

        }
        buffer_[current_position_ + 1 + len] = quote;
        current_position_ = new_position;
    }

    inline void write_quote(char quote) {
        auto new_position = current_position_ + 1;
        if (new_position > buffer_size_) {
            reserve(int(new_position * 1.5));
        }
        buffer_[current_position_] = quote;
        current_position_ = new_position;
    }

public:

    template<class T>
    void operator()(T &obj) {
        write(obj, *this);
    }

    /*std::string release_buffer() {
        return std::move(buffer_);
    }*/

    void reserve(unsigned int new_size) {

        if (new_size > buffer_size_) {
            std::unique_ptr<char[]> new_buffer(new char[new_size]);
            std::copy(buffer_.get(), buffer_.get() + current_position_, new_buffer.get());
            buffer_.swap(new_buffer);
            buffer_size_ = new_size;
        }
    }
    void reset() {
        current_position_ = 0;
    }

    unsigned int writed_bytes() {
        return current_position_;
    }

    const char * buffer() {
        return buffer_.get();
    }

    snapshot_writer() : current_position_(0), buffer_size_(0), test_val8("tt"), test_val16("ttxx"), test_val32("ttxxwwzz"), test_val64("ttxxwwzzttxxwwzz") {}

private:

    std::string test_val8;
    std::string test_val16;
    std::string test_val32;
    std::string test_val64;

    std::unique_ptr<char[]> buffer_;
    unsigned int current_position_;
    unsigned int buffer_size_;
};




#endif //EMP_FRAMEWORK_WRITER_H
