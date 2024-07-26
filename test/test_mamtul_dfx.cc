#include "catch2/catch.hpp"
#include "dfx/matmul_dfx_proxy.h"
#include <type_traits>

using namespace matmul;

struct Object {
    bool func(int value) {
        return true;
    }

    bool func(double value) {
        return false;
    }

    bool func(double value) const {
        return true;
    }

    bool func(const char*) const {
        return false;
    }

    void func(const char*, int v) const {
    }

    void func() {
        return;
    }

    void other_func() {        
    }

    const char* hello() const {
        return "hello";
    }
};

struct OtherObject {
    int func() const {
        return 5;
    }
};

//////////////////////////////////////////////////////////////
MATMUL_DFX_PROXY_REGISTER_DEFAULT();
MATMUL_DFX_PROXY_REGISTER(Object, func, other_func);

//////////////////////////////////////////////////////////////
SCENARIO("object dfx proxy Test") {
    Object obj;
    MatmulDfxProxy<Object> proxy{obj};

    proxy->func();
    proxy->other_func();
    REQUIRE(proxy->func(1));
    REQUIRE(!proxy->func(2.0));
    REQUIRE(!proxy->func("hello"));
}

SCENARIO("object dfx const proxy Test") {
    const Object obj;
    MatmulDfxProxy<const Object> proxy{obj};

    proxy->func("hello", 3);
    REQUIRE(proxy->func(2.1));
    REQUIRE(!proxy->func("hello"));
}

//////////////////////////////////////////////////////////////
SCENARIO("object dfx proxy not match test") {

    OtherObject obj;
    MatmulDfxProxy<OtherObject> proxy{obj};

    REQUIRE(proxy->func() == 5);
}
