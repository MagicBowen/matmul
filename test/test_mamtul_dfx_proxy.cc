#include "catch2/catch.hpp"
#include <iostream>
#include "dfx/matmul_dfx_proxy.h"
#include "dfx/matmul_dfx_handler.h"

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
};

MATMUL_DFX_PROXY_REGISTER(Object, func);
// MATMUL_DFX_PROXY_REGISTER(Object, other_func);

SCENARIO("object dfx proxy Test") {
    Object obj;
    MatmulDfxProxy<decltype(obj)> proxy{obj};

    proxy->func();
    // proxy->other_func();
    REQUIRE(proxy->func(1));
    REQUIRE(!proxy->func(2.0));
    REQUIRE(!proxy->func("hello"));
}

SCENARIO("object dfx const proxy Test") {
    const Object obj;
    auto& ob = obj;
    MatmulDfxProxy<std::remove_reference_t<decltype(ob)>> proxy{ob};

    REQUIRE(proxy->func(2.1));
    REQUIRE(!proxy->func("hello"));
    proxy->func("hello", 3);
}