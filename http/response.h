//
// Created by Paladin on 22.01.2017.
//

#ifndef EMP_FRAMEWORK_RESPONSE_H
#define EMP_FRAMEWORK_RESPONSE_H

#include <memory>
#include <functional>
#include <boost/signals2.hpp>

namespace emp {

    class response {
    public:

        template<class T>
        void on_finish(T&& handler) {
            finish_signal.connect(std::forward<T>(handler));
        }


        virtual void add_header(std::string header, std::string value) = 0;

        virtual void write(std::string buffer);

    protected:
        boost::signals2::signal<void ()> finish_signal;
    };

    typedef std::shared_ptr<response> response_ptr;
}


#endif //EMP_FRAMEWORK_RESPONSE_H
