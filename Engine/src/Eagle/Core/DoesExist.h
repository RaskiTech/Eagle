#pragma once
#include <type_traits>

template<typename T, typename F>
constexpr auto is_valid(F&& f) -> decltype(f(std::declval<T>()), true) { return true; }

template<typename>
constexpr bool is_valid(...) { return false; }

#define IS_VALID(T, EXPR) is_valid<T>( [](auto&& obj)->decltype(obj.EXPR){} )
#define COMPILE_IF_VALID(T, EXPR, CODE) if constexpr (IS_VALID(T, EXPR)) { CODE; }


/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/


// Different for event function since it takes a reference. If more functions like this are needed maybe could generalize this
#define COMPILE_IF_EVENTFUNC(T, CODE) if constexpr (HasEventFunc<T, bool(Event&)>::value) { CODE; }

template<typename, typename T>
struct HasEventFunc {
    static_assert(
        std::integral_constant<T, false>::value,
        "Second template parameter needs to be of function type.");
};

// specialization that does the checking

template<typename C, typename Ret, typename... Args>
struct HasEventFunc<C, Ret(Args...)> {
private:
    template<typename T>
    static constexpr auto check(T*)
        -> typename
        std::is_same<
        decltype(std::declval<T>().OnEvent(std::declval<Args>()...)),
        Ret       // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        >::type;  // attempt to call it and see if the return type is correct

    template<typename>
    static constexpr std::false_type check(...);

    typedef decltype(check<C>(0)) type;

public:
    static constexpr bool value = type::value;
};