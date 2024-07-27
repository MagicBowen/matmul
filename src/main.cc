#include <iostream>
#include <tuple>
#include <utility>
#include <stdexcept>
#include <type_traits>

// Helper template to check if a type is invocable with specific arguments
template <typename T, typename F, typename Tuple, std::size_t... I>
auto is_invocable_impl(T* obj, F func, Tuple&& t, std::index_sequence<I...>) 
    -> decltype((obj->*func)(std::get<I>(std::forward<Tuple>(t))...), std::true_type{}) {
    return std::true_type{};
}

template <typename T, typename F, typename Tuple, std::size_t... I>
std::false_type is_invocable_impl(...) {
    return std::false_type{};
}

template <typename T, typename F, typename Tuple>
constexpr auto is_invocable(T* obj, F func, Tuple&& t) {
    constexpr auto N = std::tuple_size_v<std::remove_reference_t<Tuple>>;
    return is_invocable_impl<T, F, Tuple>(obj, func, std::forward<Tuple>(t), std::make_index_sequence<N>{});
}

template <typename T, typename F, typename Tuple, std::size_t... I>
auto call_if_match_impl(T* obj, F func, Tuple&& t, std::index_sequence<I...>) 
    -> decltype((obj->*func)(std::get<I>(std::forward<Tuple>(t))...)) {
    return (obj->*func)(std::get<I>(std::forward<Tuple>(t))...);
}

template <typename T, typename F, typename Tuple>
auto call_if_match(T* obj, F func, Tuple&& t) {
    constexpr auto N = std::tuple_size_v<std::remove_reference_t<Tuple>>;
    return call_if_match_impl(obj, func, std::forward<Tuple>(t), std::make_index_sequence<N>{});
}

// Helper template to iterate over a tuple of member function pointers and attempt to call the matching one
template <typename T, typename Tuple, typename ArgsTuple, std::size_t... I>
void match_and_call(T* obj, Tuple&& funcs, ArgsTuple&& args, std::index_sequence<I...>) {
    bool called = false;
    auto try_call = [&](auto func) {
        if (!called) {
            if constexpr (decltype(is_invocable(obj, func, std::forward<ArgsTuple>(args)))::value) {
                call_if_match(obj, func, std::forward<ArgsTuple>(args));
                called = true;
            }
        }
    };

    (try_call(std::get<I>(funcs)), ...);
    if (!called) {
        throw std::runtime_error("No matching function found");
    }
}

// Main template to be used in calling the appropriate member function
template <typename T, typename... Funcs, typename... Args>
void call_matching_function(T* obj, const std::tuple<Funcs...>& funcs, Args&&... args) {
    match_and_call(obj, funcs, std::forward_as_tuple(args...), std::index_sequence_for<Funcs...>{});
}

// Example class
class MyClass {
public:
    void method1(int a) {
        std::cout << "Method1 called with " << a << std::endl;
    }

    void method2(int a, double b) {
        std::cout << "Method2 called with " << a << " and " << b << std::endl;
    }

    void method3(const std::string& str) {
        std::cout << "Method3 called with " << str << std::endl;
    }
};

// Example usage
int main() {
    MyClass obj;
    auto methods = std::make_tuple(&MyClass::method1, &MyClass::method2, &MyClass::method3);

    call_matching_function(&obj, methods, 42);               // Calls method1
    call_matching_function(&obj, methods, 42, 3.14);         // Calls method2
    call_matching_function(&obj, methods, std::string("Hello")); // Calls method3

    return 0;
}