/// Lambda calculus is fun and all, but writing THREE = Succ(Succ(Succ(Zero)))
/// tends to get old after a while. By using template specialization, we can
/// metaprogram our metaprogramming.

#pragma once

#include <stdint.h>
#include <funcpp.h>

template <uint64_t N, typename F, typename X>
struct _ApplyN {
    using type = typename F::apply<typename _ApplyN<N - 1, F, X>::type>;
};

template <typename F, typename X>
struct _ApplyN<0, F, X> {
    using type = X;
};

/// The pure-functional code doesn't know what to do with an integer constant,
/// so we have to roll our own equivalent of curry_untemplate
template <uint64_t N>
struct ApplyN {
    /// Must be class template, cannot be alias
    template <typename F, typename X>
    struct intermediate: _ApplyN<N, F, X> {};

    template <typename X>
    using apply = typename curry<untemplate<intermediate>>::apply<X>;
};
