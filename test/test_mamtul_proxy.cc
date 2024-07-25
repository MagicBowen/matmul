#include "catch2/catch.hpp"
#include <utility>
#include <iostream>
#include <functional>
#include <type_traits>

// 示例结构体
struct Object {
    bool func(int value) {
        // 实际实现
        return true;
    }
};

struct ModuleDFX {
    template<typename... Args>
    static void PreCall(const char* funcName, Args&&... args) {
        printf("-------------[%s]-----------\n", funcName);
        printf("Params : ");
        (std::cout << ... << args) << std::endl;
    }

    static void PostCall() {
        printf("-----------------------------\n");
    }
};

// Helper to extract return type of member function pointer
template <typename T>
struct member_function_traits;

// Partial specialization for non-const member functions
template <typename R, typename C, typename... Args>
struct member_function_traits<R (C::*)(Args...)> {
    using return_type = R;
};

// Partial specialization for const member functions
template <typename R, typename C, typename... Args>
struct member_function_traits<R (C::*)(Args...) const> {
    using return_type = R;
};

// Template function to deduce return type
template <typename MemberFunctionPtr, typename... Args>
struct get_return_type {
    using type = typename member_function_traits<MemberFunctionPtr>::return_type;
};

// Register模板类
template <typename T>
struct Register : T {
};

template <> class Register<Object> {
public: 
    Register(Object& obj) : obj_{obj} {
    }

    struct FuncProxy {
        FuncProxy(Object& obj)
            : obj_{obj}{}

        template <typename... Args>
        typename get_return_type<decltype(&Object::func), Args...>::type func(Args&&... args) {
            ModuleDFX::PreCall("func", std::forward<Args>(args)...);
            auto result = (obj_.Object::func)(std::forward<Args>(args)...);
            ModuleDFX::PostCall();
            return result;
        }

    private:
        Object& obj_;
    };

    auto operator->() {
        static FuncProxy proxy{obj_};
        return &proxy; 
    }

private:
    Object& obj_;
};

SCENARIO("Module proxy Test 4") {
    Object obj;
    Register<decltype(obj)> reg{obj};

    // 调用成员函数
    REQUIRE(reg->func(1));
}