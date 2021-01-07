#pragma once
#include <type_traits>

template<typename T, typename F>
constexpr auto is_valid(F&& f) -> decltype(f(std::declval<T>()), true) { return true; }

template<typename>
constexpr bool is_valid(...) { return false; }

#define IS_VALID(T, EXPR) is_valid<T>( [](auto&& obj)->decltype(obj.EXPR){} )
#define COMPILE_IF_VALID(T, EXPR, CODE) if constexpr (IS_VALID(T, EXPR)) { CODE; }

// Example use

// IS_VALID(class, method/value)
/*
  
  
  class Test {
  	void Func() {}
  	int member;
  };
  
  void Example {
    if ( IS_VALID(Test, Func()) )        --> True
    if ( IS_VALID(Test, Func2()) )       --> False
    if ( IS_VALID(Test, member ) )       --> True
    if ( IS_VALID(Test, member2) )       --> False
  }
  
*/