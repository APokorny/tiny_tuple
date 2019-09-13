#ifndef TINY_TUPLE_MAP_HPP_INCLUDED
#define TINY_TUPLE_MAP_HPP_INCLUDED

#include <type_traits>
#include <utility>
#include "kvasir/mpl/algorithm/find_if.hpp"
#include "kvasir/mpl/sequence/front.hpp"
#include "kvasir/mpl/sequence/size.hpp"
#include "kvasir/mpl/types/bool.hpp"

namespace tiny_tuple
{
namespace detail
{
template <typename K, typename V, bool = std::is_empty<V>::value && !std::is_final<V>::value>
struct item;
template <typename K, typename V>
struct item<K, V, true> : V
{
    using key = K;
    using value = V;
    constexpr item() {}
    constexpr item(item<K, V, true>&& t) : V(static_cast<V&&>(t)) {}
    constexpr item(item<K, V, true> const& t) : V(static_cast<V const&>(t)) {}
    explicit constexpr item(V const& t) : V(t) {}
    explicit constexpr item(V&& t) : V(std::move(t)) {}
};
template <typename K, typename V>
struct item<K, V, false>
{
    using key = K;
    using value = V;
    constexpr item() : data_() {}

    constexpr item(item<K, V, false>&& t) : data_(std::move(t.data_)) {}
    constexpr item(item<K, V, false> const& t) : data_(t.data_) {}
    explicit constexpr item(V const& t) : data_(t) {}
    explicit constexpr item(V&& t) : data_(std::move(t)) {}
    V data_;
};

template <typename K>
struct is_key
{
    template <typename T>
    struct f_impl;
    template <typename V>
    struct f_impl<item<K, V>> : kvasir::mpl::bool_<true>
    {
    };
    template <typename OtherK, typename V>
    struct f_impl<item<OtherK, V>> : kvasir::mpl::bool_<false>
    {
    };
    template <typename T>
    using f = f_impl<T>;
};

template <typename K, typename V>
constexpr V const& get(item<K, V, true> const& x)
{
    return x;
}

template <typename K, typename V>
constexpr V& get(item<K, V, true>& x)
{
    return x;
}

template <typename K, typename V>
constexpr V&& get(item<K, V, true>&& x)
{
    return static_cast<V&&>(x);
}

template <typename K, typename V>
constexpr V const& get(item<K, V, false> const& x)
{
    return x.data_;
}

template <typename K, typename V>
constexpr V& get(item<K, V, false>& x)
{
    return x.data_;
}

template <typename K, typename V>
constexpr V&& get(item<K, V, false>&& x)
{
    return static_cast<V&&>(x.data_);
}
template <typename T>
struct value_type_impl;
template <typename K, typename V>
struct value_type_impl<item<K, V>>
{
    using type = V;
};

template <typename T>
using value_type = typename value_type_impl<T>::type;
}  // namespace detail

template <typename... Params>
struct map : Params...
{
    constexpr map() = default;

    using this_t = map<Params...>;
    template <typename... Others, typename = typename std::enable_if<std::is_same<this_t, map<std::decay_t<Others>...>>::value>::type>
    constexpr map(Others&&... other) : Params(static_cast<Others&&>(other))...
    {
    }
    template <typename... Others, typename = typename std::enable_if<std::is_same<this_t, map<std::decay_t<Others>...>>::value>::type>
    constexpr map(map<Others...> const& other) : Params(static_cast<Others&&>(other))...
    {
    }
};

template <typename Name, typename Parameters>
struct value_type;

template <typename Name, typename... Params>
struct value_type<Name, map<Params...>>
{
    using type = kvasir::mpl::call<kvasir::mpl::find_if<detail::is_key<Name>, kvasir::mpl::front<>>, Params...>;
};

template <typename Key, typename... Params,
          typename ActualType = kvasir::mpl::call<kvasir::mpl::find_if<detail::is_key<Key>, kvasir::mpl::front<>>, Params...>>
constexpr auto const& get(map<Params...> const& parameter_map)
{
    return get(static_cast<ActualType const&>(parameter_map));
}

template <typename Key, typename... Params,
          typename ActualType = kvasir::mpl::call<kvasir::mpl::find_if<detail::is_key<Key>, kvasir::mpl::front<>>, Params...>>
constexpr auto& get(map<Params...>& parameter_map)
{
    return get(static_cast<ActualType&>(parameter_map));
}

template <typename Name, typename Parameters>
struct has_key;

template <typename Name, typename... Params>
struct has_key<Name, map<Params...>>
    : kvasir::mpl::bool_<kvasir::mpl::call<kvasir::mpl::find_if<detail::is_key<Name>, kvasir::mpl::size<>>, Params...>::value != 0>
{
};
}  // namespace tiny_tuple

#endif
