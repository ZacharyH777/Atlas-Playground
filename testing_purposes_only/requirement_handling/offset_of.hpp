/*
 * This was not built by me. Its goal is to find a compile time safe option, to
 * find offsets in a templated function. Given a base location and a pointer.
 * example use can be given in component_builder.hpp in the member function.
 *
 * Replacing the less safe and non-compile time solution:
 * reinterpret_cast<size_t>(&(reinterpret_cast<StructType*>(0)->*member_ptr));
 *
 * The tested solution can be found here:
 * https://godbolt.org/z/3qfTzn9oq
 *
 * and discussed here:
 * https://gist.github.com/graphitemaster/494f21190bb2c63c5516
 * if you search for: schaumb commented on May 13, 2022
 */

#pragma once

#include <tuple>
#include <type_traits>

namespace atlas {
#pragma pack(push, 1)
template <typename Member, std::size_t O> struct Pad {
  char pad[O];
  Member m;
};
#pragma pack(pop)

template <typename Member> struct Pad<Member, 0> {
  Member m;
};

template <typename Base, typename Member, std::size_t O> struct MakeUnion {
  union U {
    char c;
    Base base;
    Pad<Member, O> pad;
    constexpr U() noexcept : c{} {};
  };
  constexpr static U u{};
};

template <class Member, class Base>
std::tuple<Member, Base> get_types(Member Base::*);

template <
    class IC, class TheBase = void, std::ptrdiff_t off = 0,
    class T = decltype(get_types(std::declval<typename IC::value_type>())),
    class Member = typename std::tuple_element<0, T>::type,
    class Orig = typename std::tuple_element<1, T>::type,
    class Base = typename std::conditional<std::is_same<void, TheBase>::value,
                                           Orig, TheBase>::type,
    bool = (&((static_cast<const Orig *>(&MakeUnion<Base, Member, off>::u.base))
                  ->*IC::value) > &MakeUnion<Base, Member, off>::u.pad.m)>
struct offset_of_impl {
  constexpr static std::ptrdiff_t value =
      offset_of_impl<IC, TheBase,
                     off + (sizeof(Member) < alignof(Orig)
                                ? sizeof(Member)
                                : alignof(Orig))>::value;
};

template <class IC, class TheBase, std::ptrdiff_t off, class T, class Member,
          class Orig, class Base>
struct offset_of_impl<IC, TheBase, off, T, Member, Orig, Base, false> {
  constexpr static std::ptrdiff_t value = off;
};

#define offset_of(mptr)                                                        \
  offset_of_impl<std::integral_constant<decltype(mptr), mptr>>::value
#define offset_of_base(mptr, base_type)                                        \
  offset_of_impl<std::integral_constant<decltype(mptr), mptr>, base_type>::value
}; // namespace atlas