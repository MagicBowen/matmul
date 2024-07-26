#include <iostream>
#include <type_traits>
#include <functional>
#include <unordered_map>

// Primary template for FuncProxy, which does nothing
template <typename, auto>
struct FuncProxy {};

// Specialization for registered functions
template <typename T, typename R, typename... Args, R (T::*Func)(Args...)>
struct FuncProxy<T, Func> {
    FuncProxy(T& module) : module_(module) {}

    R operator()(Args... args) {
        return (module_.*Func)(args...);
    }

private:
    T& module_;
};

// Primary template for MatmulDfxProxy
template <typename T>
class MatmulDfxProxy {
public:
    MatmulDfxProxy(T& module) : module_(module) {}

    template <auto Func, typename... Args>
    auto invoke(Args&&... args) {
        if constexpr (std::is_member_function_pointer_v<decltype(Func)>) {
            FuncProxy<T, Func> funcProxy(module_);
            return funcProxy(std::forward<Args>(args)...);
        } else {
            return (module_.*Func)(std::forward<Args>(args)...);
        }
    }

    T* operator->() {
        return &module_;
    }

private:
    T& module_;
};

// Macro to register functions
#define MATMUL_DFX_PROXY_REGISTER(MODULE, FUNC) \
template <> \
class MatmulDfxProxy<MODULE> { \
public: \
    MatmulDfxProxy(MODULE& module) : module_(module) {} \
    template <auto Func, typename... Args> \
    auto invoke(Args&&... args) { \
        FuncProxy<MODULE, &MODULE::FUNC> funcProxy(module_); \
        return funcProxy(std::forward<Args>(args)...); \
    } \
    MODULE* operator->() { \
        return &module_; \
    } \
private: \
    MODULE& module_; \
};

// Define a class
struct Object {
    bool func(int value) {
        std::cout << "func called with value: " << value << std::endl;
        return true;
    }

    void other_func() {
        std::cout << "other_func called" << std::endl;
    }
};

// Register Object::func to proxy
MATMUL_DFX_PROXY_REGISTER(Object, func)

int main() {
    Object obj;
    MatmulDfxProxy<Object> proxy{obj};

    // Use proxy func via invoke
    proxy.invoke<&Object::func>(42);
    // Use other func directly via original object
    proxy->other_func();

    return 0;
}