#pragma once

#include <iostream>
#include <type_traits>

namespace detail
{
    // apply cannot yet be called, so need to return a new type with another apply
    // a simpler (but invalid) version would look like <F, Args..., Last, void>
    // but we always need to put the variadic argument last
    template <typename F, typename Last, typename = void, typename... Args>
    struct partial_helper
    {
        struct type
        {
            template <typename Next>
            using apply =
                typename detail::partial_helper<F, Next, void, Args..., Last>::type;
        };
    };

    // apply gives the final arg, so call the func (need all prev args and final)
    template <typename F, typename Last, typename... Args>
    struct partial_helper<F, Last, std::void_t<typename F::apply<Args..., Last>>,
                          Args...>
    {
        using type = typename F::apply<Args..., Last>;
    };

};

// two options:
// - apply gives the last argument necessary, so call the func
// - apply doesn't give the last argument necessary, so store the arg, and recurse
template <typename F, typename X>
using partial = detail::partial_helper<F, X, void>::type;

namespace detail
{
    template <typename F, typename X, typename = void>
    struct curry_helper
    {
        struct type
        {
            template <typename Y>
            using apply = detail::curry_helper<partial<F, X>, Y>::type;
        };
    };

    template <typename F, typename X>
    struct curry_helper<F, X, std::void_t<typename F::apply<X>>>
    {
        using type = typename F::apply<X>;
    };
};

template <typename F>
struct curry
{
    template <typename X>
    using apply = detail::curry_helper<F, X>::type;
};

template <template <typename...> typename F>
struct untemplate
{
    template <typename... Args>
    using apply = F<Args...>::type;
};

template <template <typename...> typename F>
struct curry_untemplate
{
    template <typename X>
    using apply = typename curry<untemplate<F>>::apply<X>;
};

namespace detail
{
    template <typename F, typename X, typename... Args>
    struct call_helper
    {
        using type = detail::call_helper<typename F::apply<X>, Args...>::type;
    };

    template <typename F, typename X>
    struct call_helper<F, X>
    {
        using type = typename F::apply<X>;
    };
};

template <typename F, typename... Args>
using call = typename detail::call_helper<F, Args...>::type;

// TRUE = λxy.x
template <typename X, typename Y>
struct _TRUE
{
    using type = X;
};
using TRUE = curry_untemplate<_TRUE>;

// FALSE = λxy.y
template <typename X, typename Y>
struct _FALSE
{
    using type = Y;
};
using FALSE = curry_untemplate<_FALSE>;

// NOT = λxyz.xzy
template <typename X, typename Y, typename Z>
struct _NOT
{
    using type = typename X::apply<Z>::apply<Y>;
};
using NOT = curry_untemplate<_NOT>;

// AND = λxy.xyx
template <typename X, typename Y>
struct _AND
{
    using type = typename X::apply<Y>::apply<X>;
};
using AND = curry_untemplate<_AND>;

// OR = λxy.xxy
template <typename X, typename Y>
struct _OR
{
    using type = typename X::apply<X>::apply<Y>;
};
using OR = curry_untemplate<_OR>;

// ZERO = λfx.x
template <typename F, typename X>
struct _ZERO
{
    using type = X;
};
using ZERO = curry_untemplate<_ZERO>;

// ONE = λfx.fx
template <typename F, typename X>
struct _ONE
{
    using type = typename F::apply<X>;
};
using ONE = curry_untemplate<_ONE>;

// TWO = λfx.f(fx)
template <typename F, typename X>
struct _TWO
{
    using type = typename F::apply<typename F::apply<X>>;
};
using TWO = curry_untemplate<_TWO>;

// THREE = λfx.f(f(f(x)))
template <typename F, typename X>
struct _THREE
{
    using type = typename F::apply<typename F::apply<typename F::apply<X>>>;
};
using THREE = curry_untemplate<_THREE>;

// SUCC = λnfx.f(n(f)(x))
template <typename N, typename F, typename X>
struct _SUCC
{
    using type = typename F::apply<typename N::apply<F>::apply<X>>;
};
using SUCC = curry_untemplate<_SUCC>;

// ADD = λxyf.y(SUCC)(x)
template <typename X, typename Y>
struct _ADD
{
    using type = call<Y, SUCC, X>; // typename Y::apply<SUCC>::apply<X>;
};
using ADD = curry_untemplate<_ADD>;

// MUL = λxyf.y(x(f))
template <typename X, typename Y, typename F>
struct _MUL
{
    // using type = typename Y::apply<typename X::apply<F>>;
    using type = call<Y, call<X, F>>;
};
using MUL = curry_untemplate<_MUL>;

// Bluebird = λfga.f(g(a)) [composition]
template <typename F, typename G, typename A>
struct _Bluebird
{
    using type = typename F::apply<typename G::apply<A>>;
};
using Bluebird = curry_untemplate<_Bluebird>;
