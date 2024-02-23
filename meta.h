/****************************************************************
  meta.h

  Template metaprogramming helpers and traits
`
  Patrick J. Fasano
  Argonne National Laboratory

  02/21/24 (pjf): Created.

****************************************************************/

#ifndef MCUTILS_META_H_
#define MCUTILS_META_H_

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace mcutils
{

////////////////////////////////////////////////////////////////
// concat_sequences -- concatenate two std::integer_sequences
////////////////////////////////////////////////////////////////
template<typename, typename>
struct concat_sequences;

template<typename T, T... Seq1, T... Seq2>
struct concat_sequences<std::integer_sequence<T, Seq1...>, std::integer_sequence<T, Seq2...>>
{
  using type = std::integer_sequence<T, Seq1..., Seq2...>;
};

template<typename S1, typename S2>
using concat_sequences_v = typename concat_sequences<S1, S2>::type;


////////////////////////////////////////////////////////////////
// const_sequence -- std::integer_sequence consisting of
// an integer repeated N times
////////////////////////////////////////////////////////////////
namespace impl {
    template<typename T, T V, typename S>
    struct map_int_seq;

    template<typename T, T V, std::size_t... Is>
    struct map_int_seq<T, V, std::index_sequence<Is...>> {
        using type = std::integer_sequence<T, (Is, V)...>;
    };
}

template<std::size_t N, typename T, T V>
using const_seq = typename impl::map_int_seq<T, V, std::make_index_sequence<N>>::type;


////////////////////////////////////////////////////////////////
// is_derived_constructible -- detect if type is constructible
// by a derived type
//
// This is true even if the constructor in the base class
// is `protected`.
////////////////////////////////////////////////////////////////
namespace impl
{
template<typename T>
struct constructor_detector : public T
{
  template<typename... Args>
  constructor_detector(Args&&... args)
    : T{std::forward<Args>(args)...}
  {}
};
}

template<typename T, typename... Args>
struct is_derived_constructible
{
  static constexpr bool value = std::is_constructible<impl::constructor_detector<T>, Args...>::value;
};
template<typename T, typename... Args>
constexpr bool is_derived_constructible_v = is_derived_constructible<T, Args...>::value;

}  // namespace mcutils

#endif // MCUTILS_META_H_
