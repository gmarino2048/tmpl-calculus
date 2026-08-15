
#include <funcpp.h>
#include <spec.h>
#include <types.h>

template <typename C, typename P>
struct FiboState {
    using current = C;
    using previous = P;
};

using FiboBase = FiboState<Uint64<1>, Uint64<0>>;

struct FiboStep {
    template <typename S>
    using apply = FiboState<typename AddUint64::apply<typename S::current>::apply<typename S::previous>, typename S::current>;
};

int main() {
    using base = Uint64<0>;

    // Make sure ApplyN works on incremental
    static_assert(ApplyN<3>::apply<IncrUint64>::apply<base>::value == 3);
    static_assert(ApplyN<1>::apply<IncrUint64>::apply<base>::value == IncrUint64::apply<base>::value);
    static_assert(ApplyN<5>::apply<AddUint64::apply<Uint64<2>>>::apply<base>::value == 10);

    // Go through the first 10 fibonnacci numbers
    static_assert(ApplyN<0>::apply<FiboStep>::apply<FiboBase>::current::value == 1);
    static_assert(ApplyN<1>::apply<FiboStep>::apply<FiboBase>::current::value == 1);
    static_assert(ApplyN<2>::apply<FiboStep>::apply<FiboBase>::current::value == 2);
    static_assert(ApplyN<3>::apply<FiboStep>::apply<FiboBase>::current::value == 3);
    static_assert(ApplyN<4>::apply<FiboStep>::apply<FiboBase>::current::value == 5);
    static_assert(ApplyN<5>::apply<FiboStep>::apply<FiboBase>::current::value == 8);
    static_assert(ApplyN<6>::apply<FiboStep>::apply<FiboBase>::current::value == 13);
    static_assert(ApplyN<7>::apply<FiboStep>::apply<FiboBase>::current::value == 21);
    static_assert(ApplyN<8>::apply<FiboStep>::apply<FiboBase>::current::value == 34);
    static_assert(ApplyN<9>::apply<FiboStep>::apply<FiboBase>::current::value == 55);

    return 0;
}
