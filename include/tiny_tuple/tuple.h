#ifndef TINY_TUPLE_TUPLE_HPP_INCLUDED
#define TINY_TUPLE_TUPLE_HPP_INCLUDED

#include "kvasir/mpl/algorithm/find_if.hpp"
#include "kvasir/mpl/sequence/front.hpp"
#include "kvasir/mpl/sequence/size.hpp"
#include "kvasir/mpl/sequence/at.hpp"
#include "tiny_tuple/map.h"
#include "tiny_tuple/detail/tuple.h"
#include "tiny_tuple/detail/invoke.h"

namespace tiny_tuple
{
template <typename... Params>
struct tuple : detail::tuple_impl<std::make_integer_sequence<int, sizeof...(Params)>, Params...>
{
    constexpr tuple() = default;
    using Base        = detail::tuple_impl<std::make_integer_sequence<int, sizeof...(Params)>, Params...>;
    template <typename Other,
              typename = typename std::enable_if<std::is_same<typename std::decay<Other>::type, tuple<Params...>>::value, void>::type>
    constexpr tuple(Other&& other) : Base(detail::from_other{}, static_cast<Other&&>(other))
    {
    }

    template <typename... Yn>
    constexpr tuple(Yn&&... yn) : Base(static_cast<Yn&&>(yn)...)
    {
    }

    template <int... Is, typename Other>
    void assign(std::integer_sequence<int, Is...>, Other const& other)
    {
        auto assign_fun       = [this, &other](auto i) { get<decltype(i)::value>(*this) = get<decltype(i)::value>(other); };
        int  assigner_dummy[] = {(assign_fun(kvasir::mpl::int_<Is>{}), 0)...};
        (void)assigner_dummy;
    }
    tuple& operator=(tuple const& other)
    {
        assign(std::make_integer_sequence<int, sizeof...(Params)>(), other);
        return *this;
    }
    template <template <typename...> class C, typename... Yn>
    tuple& operator=(C<Yn...> const& other)
    {
        assign(std::make_integer_sequence<int, sizeof...(Params)>(), other);
        return *this;
    }
};

template <int I, typename... Ts, typename VT = kvasir::mpl::call<kvasir::mpl::at<kvasir::mpl::uint_<I>>, Ts...>>
constexpr auto const& get(tuple<Ts...> const& t)
{
    return get(static_cast<detail::item<kvasir::mpl::int_<I>, VT> const&>(t));
}

template <int I, typename... Ts, typename VT = kvasir::mpl::call<kvasir::mpl::at<kvasir::mpl::uint_<I>>, Ts...>>
auto& get(tuple<Ts...>& t)
{
    return get(static_cast<detail::item<kvasir::mpl::int_<I>, VT>&>(t));
}

template <int I, typename... Ts, typename VT = kvasir::mpl::call<kvasir::mpl::at<kvasir::mpl::uint_<I>>, Ts...>>
auto&& get(tuple<Ts...>&& t)
{
    return get(std::move(static_cast<detail::item<kvasir::mpl::int_<I>, VT>>(t)));
}

template <typename... Ts, typename... Us>
constexpr auto append(tuple<Ts...>&& elements, Us&&... params)
{
    return detail::append_impl(std::move(elements), std::make_integer_sequence<int, sizeof...(Ts)>(), std::forward<Us>(params)...);
}

template <typename... Ts, typename... Us>
constexpr auto append(tuple<Ts...> const& elements, Us&&... params)
{
    return detail::append_impl(elements, std::make_integer_sequence<int, sizeof...(Ts)>(), std::forward<Us>(params)...);
}

template <typename... Ts, typename F>
constexpr void foreach (tuple<Ts...>& tup, F && fun)
{
    return detail::foreach_impl(tup, std::make_integer_sequence<int, sizeof...(Ts)>(), std::forward<F>(fun));
}

template <typename... Ts, typename F>
constexpr void foreach (tuple<Ts...> const& tup, F && fun)
{
    return detail::foreach_impl(tup, std::make_integer_sequence<int, sizeof...(Ts)>(), std::forward<F>(fun));
}
}  // namespace tiny_tuple

#endif
