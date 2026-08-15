/// Base reusable type primitives

#pragma once

#include <stdint.h>
#include <funcpp.h>

template<uint64_t I>
using Uint64 = std::integral_constant<uint64_t, I>;

struct IncrUint64 {
    template <typename C>
    using apply = Uint64<C::value + 1>;
};

template <typename X, typename Y>
struct _AddUint64 {
    using type = Uint64<X::value + Y::value>;
};
using AddUint64 = curry_untemplate<_AddUint64>;

