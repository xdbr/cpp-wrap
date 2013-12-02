#include <iostream>
#include <functional>
#include <boost/optional.hpp>

template<class T>
struct wrap {
    explicit wrap(std::function<T()> fun)
    : fun_{ fun } {}

    inline T operator()() {
        if (before_) (*before_)();
        fun_();
        if (after_) (*after_)();
    }

    inline T before(std::function<T()> before) {
        before_ = before;
    }
    inline T after(std::function<T()> after) {
        after_ = after;
    }
    inline T around(std::function<T()> before, std::function<T()> after) {
        before_ = before;
        after_  = after;
    }

    std::function<T()> fun_;
    boost::optional<std::function<T()>> before_ = nullptr;
    boost::optional<std::function<T()>> after_  = nullptr;
};

struct Test {
    wrap<void> foo {[]{
        std::cout << "foo" << std::endl;
    }};
};

int main() {
    Test t;
    t.foo.before([]{
        std::cout << "before foo" << std::endl;
    });
    t.foo.around([]{ std::cout << "before foo" << std::endl; }
                ,[]{ std::cout << "after foo"  << std::endl; });
    t.foo();
    return 0;
}