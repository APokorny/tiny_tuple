#ifndef TINY_TUPLE_DETAIL_TUPLE_HPP_INCLUDED
#define TINY_TUPLE_DETAIL_TUPLE_HPP_INCLUDED

#include <type_traits>

namespace tiny_tuple
{
template <typename... Params>
struct tuple;

template <int I, typename... Ts, typename VT = kvasir::mpl::call<kvasir::mpl::at<kvasir::mpl::uint_<I>>, Ts...>>
constexpr auto const& get(tuple<Ts...> const& t);

template <int I, typename... Ts, typename VT = kvasir::mpl::call<kvasir::mpl::at<kvasir::mpl::uint_<I>>, Ts...>>
auto& get(tuple<Ts...>& t);
template <int I, typename... Ts, typename VT = kvasir::mpl::call<kvasir::mpl::at<kvasir::mpl::uint_<I>>, Ts...>>
auto&& get(tuple<Ts...>&& t);

namespace detail
{
struct from_other
{
};
template <typename Index, typename... Params>
struct tuple_impl;
template <int... Is, typename... Params>
struct tuple_impl<std::integer_sequence<int, Is...>, Params...> : item<kvasir::mpl::int_<Is>, Params>...
{
    constexpr tuple_impl() = default;
    template <typename Other>
    explicit constexpr tuple_impl(detail::from_other, Other&& other)
        : item<kvasir::mpl::int_<Is>, Params>(detail::get<kvasir::mpl::int_<Is>>(static_cast<Other&&>(other)))...
    {
    }

    constexpr tuple_impl(tuple_impl const&) = default;
    constexpr tuple_impl(tuple_impl &&) = default;
    constexpr tuple_impl(tuple_impl &) = default;
    template <typename... Yn>
    explicit constexpr tuple_impl(Yn&&... yn) : item<kvasir::mpl::int_<Is>, Params>(static_cast<Yn&&>(yn))...
    {
    }
};

template <typename... Ts, int... Is, typename... Us>
constexpr auto append_impl(tuple<Ts...>&& elements, std::integer_sequence<int, Is...>, Us&&... params)
{
    return tiny_tuple::tuple<Ts..., std::decay_t<Us>...>(std::move(tiny_tuple::get<Is>(elements))..., std::forward<Us>(params)...);
}

template <typename... Ts, int... Is, typename... Us>
constexpr auto append_impl(tuple<Ts...> const& elements, std::integer_sequence<int, Is...>, Us&&... params)
{
    return tiny_tuple::tuple<Ts..., std::decay_t<Us>...>(tiny_tuple::get<Is>(elements)..., std::forward<Us>(params)...);
}

template <typename... Ts, int... Is, typename... Us>
constexpr auto prepend_impl(tuple<Ts...>&& elements, std::integer_sequence<int, Is...>, Us&&... params)
{
    return tiny_tuple::tuple<std::decay_t<Us>..., Ts...>(std::forward<Us>(params)..., std::move(tiny_tuple::get<Is>(elements))...);
}

template <typename... Ts, int... Is, typename... Us>
constexpr auto prepend_impl(tuple<Ts...> const& elements, std::integer_sequence<int, Is...>, Us&&... params)
{
    return tiny_tuple::tuple<std::decay_t<Us>..., Ts...>(std::forward<Us>(params)... , tiny_tuple::get<Is>(elements)... );
}

template <typename... Ts, int... Is, typename... Us, int... Js>
constexpr auto concat_impl(tuple<Ts...>&& elements, std::integer_sequence<int, Is...>, tuple<Us...>&& elements2, std::integer_sequence<int, Js...>)
{
    return tiny_tuple::tuple<Ts..., Us...>(std::move(tiny_tuple::get<Is>(elements))..., std::move(tiny_tuple::get<Js>(elements2))...);
}

template <typename... Ts, int... Is, typename... Us, int... Js>
constexpr auto concat_impl(tuple<Ts...> const& elements, std::integer_sequence<int, Is...>, tuple<Us...>const& elements2, std::integer_sequence<int, Js...>)
{
    return tiny_tuple::tuple<Ts..., Us...>(tiny_tuple::get<Is>(elements)..., tiny_tuple::get<Js>(elements2)...);
}

}  // namespace detail
}  // namespace tiny_tuple

#endif
