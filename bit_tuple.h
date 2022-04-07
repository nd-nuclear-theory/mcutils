/****************************************************************
  bit_tuple.h

  A simple container for exposing bit fields with tuple-like
  semantics.

  TODO: Add more complete documentation comments.

  Patrick J. Fasano
  University of Notre Dame

  - 08/10/21 (pjf): Created.
  - 04/06/22 (pjf): Fixed constructors and added documentation.

****************************************************************/

#ifndef MCUTILS_BIT_TUPLE_H_
#define MCUTILS_BIT_TUPLE_H_

#include <functional>
#include <type_traits>
#include <utility>

namespace mcutils
{
// template declarations
template<typename tStorageType, std::size_t... args> struct bit_tuple;
template<typename tStorageType, std::size_t offset, std::size_t size>
struct bit_field;
namespace impl
{
template<std::size_t, typename> struct Get;
}

template<typename tStorageType, std::size_t offset, std::size_t size>
struct bit_field
{
  static_assert(
      (offset + size) <= sizeof(tStorageType) * 8,
      "bit field exceeds storage boundaries"
    );
  using storage_type = tStorageType;
  static constexpr tStorageType max_value = (tStorageType(1) << size) - 1;
  static constexpr tStorageType mask_value = (max_value << offset);

  constexpr bit_field()
      : storage{}
  {}

  // basic constructor which delegates to assignment operators
  template<typename T>
  explicit constexpr bit_field(const T& v)
  {
    *this = v;
  }

  // assignment between two bit_fields of the same storage type, offset, and size
  inline constexpr bit_field& operator=(bit_field& v)
  {
    storage = (storage & ~mask_value) | (v.storage & mask_value);
    return *this;
  }

  // assignment between two bit_fields of the same storage type but different offset and size
  template<std::size_t other_offset, std::size_t other_size>
  inline constexpr bit_field& operator=(
      bit_field<tStorageType, other_offset, other_size>& v
    )
  {
    storage = (storage & ~mask_value)
              | ((v.storage << (offset - other_offset)) & mask_value);
    return *this;
  }

  // assignment equal to a normal integer type
  template<typename T, typename std::enable_if_t<std::is_integral_v<T>>* = nullptr>
  inline constexpr bit_field& operator=(T v)
  {
    storage = (storage & ~mask_value) | ((v << offset) & mask_value);
    return *this;
  }

  // conversion to integer types
  template<typename T, typename std::enable_if_t<std::is_integral_v<T>>* = nullptr>
  explicit inline constexpr operator T() const
  {
    return static_cast<T>((storage >> offset) & max_value);
  }

  // conversion to other bit_field types with the same underlying storage
  template<std::size_t other_offset, std::size_t other_size>
  explicit inline constexpr operator bit_field<tStorageType, other_offset, other_size>()
  {
    bit_field<tStorageType, other_offset, other_size> other;
    other.storage = (storage << (other_offset - offset)) & other.mask_value;
    return other;
  }

  // get bit representation of field
  constexpr inline tStorageType bitrep() const
  {
    return (storage & mask_value);
  }

  // unified comparison operator
#if (__cplusplus >= 202002L)
  friend inline constexpr auto operator<=>(const bit_field& lhs, const bit_field& rhs)
  {
    return (lhs.storage & mask_value) <=> (rhs.storage & mask_value);
  }
#endif

  // equality operator
  friend inline constexpr bool operator==(const bit_field& lhs, const bit_field& rhs)
  {
    return (lhs.storage & mask_value) == (rhs.storage & mask_value);
  }

 private:
  // declare template friend to allow conversions between types with different
  // template arguments
  template<typename, std::size_t, std::size_t> friend class bit_field;

  // underlying storage
  tStorageType storage;
};

// base case of recursive template; just has a trivial union containing the
// underlying storage type
template<typename tStorageType> struct bit_tuple<tStorageType>
{
  constexpr bit_tuple() = default;

 private:
  union
  {
    tStorageType storage;
  };
};


// recursive template for bit_tuple
//
// each level of recursion peels off a single tuple element and makes a
// bit field, with the underlying storage and other fields as alternatives
// for the union
template<typename tStorageType, std::size_t size, std::size_t... args>
struct bit_tuple<tStorageType, size, args...>
{
  // default constructor -- initialize storage to zero
  constexpr bit_tuple()
      : storage{}
  {}

  // copy constructor -- just copy storage
  constexpr bit_tuple(const bit_tuple& t)
      : storage{t.storage}
  {}

  // copy assignment operator -- just copy storage
  constexpr bit_tuple& operator=(const bit_tuple& t)
  {
    storage = t.storage;
    return *this;
  }

  // recursive template constructor -- peel off one element, pass remaining
  // arguments down to lower-level constructor
  template<
      typename T,
      typename... cArgs,
      typename std::enable_if_t<!std::is_convertible_v<T, bit_tuple>>* = nullptr
    >
  constexpr bit_tuple(T&& v, cArgs&&... c)
      : others{std::forward<cArgs>(c)...}
  {
    field = std::forward<T>(v);
  }

  // unified comparison operator
#if (__cplusplus >= 202002L)
  friend inline constexpr auto operator<=>(const bit_tuple& lhs, const bit_tuple& rhs)
  {
    return (lhs.storage <=> rhs.storage);
  }
#endif

  // equality operator
  friend inline constexpr bool operator==(const bit_tuple& lhs, const bit_tuple& rhs)
  {
    return (lhs.storage == rhs.storage);
  }

  // get bit representation of tuple
  inline constexpr tStorageType bitrep() const { return storage; }

 private:
  // mark impl::Get as friend so it can access private union members
  template<std::size_t, typename> friend class impl::Get;

  union
  {
    // underlying storage type
    tStorageType storage;
    // this field -- offset is sum of all other fields' sizes
    bit_field<tStorageType, (args + ... + 0), size> field;
    // other fields -- recursive template
    bit_tuple<tStorageType, args...> others;
  };
};


// implementation namespace for mcutils::get
namespace impl
{

// template case where we've recursed to the desired element; return actual
// bit_field from this level of recursive template
template<typename tStorageType, std::size_t bits, std::size_t... args>
struct Get<0, bit_tuple<tStorageType, bits, args...>>
{
  template<
      typename T,
      std::enable_if_t<std::is_same_v<std::decay_t<T>, bit_tuple<tStorageType, bits, args...>>>* = nullptr
    >
  static constexpr auto&& get(T&& t)
  {
    return std::forward<T>(t).field;
  }
};

// if we haven't gotten to the desired index, peel off a layer of templates
// and recurse down
template<std::size_t index, typename tStorageType, std::size_t bits, std::size_t... args>
struct Get<index, bit_tuple<tStorageType, bits, args...>>
{
  template<
      typename T,
      std::enable_if_t<std::is_same_v<std::decay_t<T>, bit_tuple<tStorageType, bits, args...>>>* = nullptr
    >
  static constexpr auto&& get(T&& t)
  {
    return std::forward<decltype(Get<index - 1, bit_tuple<tStorageType, args...>>::get(
        std::forward<T>(t).others
      ))>(
        Get<index - 1, bit_tuple<tStorageType, args...>>::get(std::forward<T>(t).others)
      );
  }
};
}  // namespace impl


// user-facing interface for getting a tuple element
//   entirely analogous to std::get<>()
template<std::size_t index, template<typename, std::size_t...> class bit_tuple, typename tStorageType, std::size_t... Args>
constexpr auto& get(bit_tuple<tStorageType, Args...>& t)
{
  return impl::Get<index, bit_tuple<tStorageType, Args...>>::get(t);
}

template<std::size_t index, template<typename, std::size_t...> class bit_tuple, typename tStorageType, std::size_t... Args>
constexpr auto const& get(bit_tuple<tStorageType, Args...> const& t)
{
  return impl::Get<index, bit_tuple<tStorageType, Args...>>::get(t);
}

template<std::size_t index, template<typename, std::size_t...> class bit_tuple, typename tStorageType, std::size_t... Args>
constexpr auto&& get(bit_tuple<tStorageType, Args...>&& t)
{
  return std::move(
      impl::Get<index, bit_tuple<tStorageType, Args...>>::get(std::move(t))
    );
}

template<std::size_t index, template<typename, std::size_t...> class bit_tuple, typename tStorageType, std::size_t... Args>
constexpr auto const&& get(bit_tuple<tStorageType, Args...> const&& t)
{
  return std::move(
      impl::Get<index, bit_tuple<tStorageType, Args...>>::get(std::move(t))
    );
}

}  // namespace mcutils


namespace std
{
// define overload of std::hash to allow use of bit_tuple with unordered_map and
// unordered_set
template<typename tStorageType, std::size_t... args>
struct hash<mcutils::bit_tuple<tStorageType, args...>>
{
  inline std::size_t operator()(const mcutils::bit_tuple<tStorageType, args...>& h) const noexcept
  {
    return std::hash<tStorageType>()(static_cast<tStorageType>(h));
  }
};
}  // namespace std

namespace mcutils
{
  template<typename tStorageType, std::size_t... args>
  std::size_t hash_value(const bit_tuple<tStorageType, args...>& t) noexcept
  {
    return std::hash<bit_tuple<tStorageType, args...>>()(t);
  }
}

#endif  // MCUTILS_BIT_TUPLE_H_
