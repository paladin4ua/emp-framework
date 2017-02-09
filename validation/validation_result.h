//
// Created by paladin on 1/26/2017.
//

#ifndef EMP_FRAMEWORK_VALIDATION_RESULT_H
#define EMP_FRAMEWORK_VALIDATION_RESULT_H

#include <map>
#include <string>



class validation_result {
    std::map<std::string, std::string> errors_;
    bool is_ok_;

public:
    validation_result() : is_ok_(true) {}

    explicit operator bool() {
        return is_ok_;
    }

    auto &get_errors() const {
        return errors_;
    }

    void add_error(std::string code, std::string value) {
        errors_[code] = value;
        is_ok_ = false;
    }
};


#endif //EMP_FRAMEWORK_VALIDATION_RESULT_H
