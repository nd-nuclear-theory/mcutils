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

template<typename, typename>
struct concat_sequences;

template<typename T, T... Seq1, T... Seq2>
struct concat_sequences<std::integer_sequence<T, Seq1...>, std::integer_sequence<T, Seq2...>>
{
  using type = std::integer_sequence<T, Seq1..., Seq2...>;
};

template<typename S1, typename S2>
using concat_sequences_v = typename concat_sequences<S1, S2>::type;

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

}  // namespace mcutils

#endif // MCUTILS_META_H_
