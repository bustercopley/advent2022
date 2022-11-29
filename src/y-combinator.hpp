#pragma once
// C++17 Y combinator by Barry[1]
// [1] https://stackoverflow.com/a/40873657/1396177

// Usage:
//
// y_combinator fib{[](auto fib, int n) -> unsigned {
//   return n < 2 ? 1 : fib(n - 2) + fib(n - 1);
// }};
//
// std::cout << fib(16) << std::endl; // => 1597

#include <functional>
#include <utility>

template <typename F> struct y_combinator {
  F f;
  template <typename... Args> decltype(auto) operator()(Args &&...args) const {
    return f(std::cref(*this), std::forward<Args>(args)...);
  }
};

template <typename F> y_combinator(F &&f) -> y_combinator<F>;
