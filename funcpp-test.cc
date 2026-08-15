#include <funcpp.h>

template <int I>
using Constant = std::integral_constant<int, I>;

struct Incr
{
    template <typename C>
    using apply = Constant<C::value + 1>;
};

int main()
{
    using c0 = Constant<0>;

    // single arg function
    static_assert(Incr::apply<c0>::value == 1);
    static_assert(partial<Incr, c0>::value == 1);
    static_assert(curry<Incr>::apply<c0>::value == 1);

    // two arg function
    static_assert(ONE::apply<Incr>::apply<c0>::value == 1);
    static_assert(partial<ONE, Incr>::apply<c0>::value == 1);
    static_assert(partial<partial<ONE, Incr>, c0>::value == 1);
    static_assert(curry<ONE>::apply<Incr>::apply<c0>::value == 1);
    static_assert(curry<ONE::apply<Incr>>::apply<c0>::value == 1);

    // three arg function
    static_assert(MUL::apply<ONE>::apply<ONE>::apply<Incr>::apply<c0>::value == 1);
    static_assert(partial<MUL::apply<ONE>, ONE>::apply<Incr>::apply<c0>::value == 1);
    static_assert(partial<partial<MUL, ONE>, ONE>::apply<Incr>::apply<c0>::value == 1);
    static_assert(partial<partial<partial<MUL, ONE>, ONE>, Incr>::apply<c0>::value == 1);
    static_assert(curry<MUL>::apply<ONE>::apply<ONE>::apply<Incr>::apply<c0>::value == 1);
    static_assert(curry<MUL::apply<ONE>>::apply<ONE>::apply<Incr>::apply<c0>::value == 1);
    static_assert(curry<MUL::apply<ONE>::apply<ONE>>::apply<Incr>::apply<c0>::value == 1);
    static_assert(curry<MUL::apply<ONE>::apply<ONE>::apply<Incr>>::apply<c0>::value == 1);

    // church numerals
    static_assert(ZERO::apply<Incr>::apply<c0>::value == 0);
    static_assert(ONE::apply<Incr>::apply<c0>::value == 1);
    static_assert(TWO::apply<Incr>::apply<c0>::value == 2);
    static_assert(THREE::apply<Incr>::apply<c0>::value == 3);
    static_assert(SUCC::apply<THREE>::apply<Incr>::apply<c0>::value == 4);

    // peano arithmetic
    using THREE_0 = ADD::apply<ZERO>::apply<THREE>;
    using THREE_1 = ADD::apply<ONE>::apply<TWO>;
    using THREE_2 = ADD::apply<TWO>::apply<ONE>;
    using THREE_3 = ADD::apply<THREE>::apply<ZERO>;
    using THREE_4 = MUL::apply<THREE>::apply<ONE>;
    using THREE_5 = MUL::apply<ONE>::apply<THREE>;
    static_assert(THREE_0::apply<Incr>::apply<c0>::value == 3);
    static_assert(THREE_1::apply<Incr>::apply<c0>::value == 3);
    static_assert(THREE_2::apply<Incr>::apply<c0>::value == 3);
    static_assert(THREE_3::apply<Incr>::apply<c0>::value == 3);
    static_assert(THREE_4::apply<Incr>::apply<c0>::value == 3);
    static_assert(THREE_5::apply<Incr>::apply<c0>::value == 3);
    using FOUR = SUCC::apply<THREE>;
    using FOUR_1 = MUL::apply<TWO>::apply<TWO>;
    static_assert(FOUR::apply<Incr>::apply<c0>::value == 4);
    static_assert(FOUR_1::apply<Incr>::apply<c0>::value == 4);

    // composition and call
    using c5 = Constant<5>;
    static_assert(call<Incr, c5>::value == 6);
    static_assert(call<THREE, Incr, c5>::value == 8);
    static_assert(call<SUCC, THREE, Incr, c5>::value == 9);
    static_assert(call<SUCC, SUCC::apply<THREE>, Incr, c5>::value == 10);
    static_assert(call<Bluebird, SUCC, SUCC, THREE, Incr, c5>::value == 10);
    static_assert(Bluebird::apply<SUCC>::apply<SUCC>::apply<THREE>::apply<Incr>::apply<c0>::value == 5);

    return 0;
}
