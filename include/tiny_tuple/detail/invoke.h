#ifndef TINY_TUPLE_DETAIL_INVOKE_HPP_INCLUDED
#define TINY_TUPLE_DETAIL_INVOKE_HPP_INCLUDED

#include <type_traits>

#ifdef __has_include
#if __has_include(<version>)
#include <version>
#endif
#endif

namespace tiny_tuple
{
template <typename... Params>
struct tuple;

namespace detail
{
#ifdef __cpp_lib_is_invocable
template <typename... Ts>
using void_t = void;
template <typename Result, typename = void>
struct is_invocable_impl : std::false_type
{
};

template <typename Result>
struct is_invocable_impl<Result, void_t<typename Result::type>> : std::true_type
{
};

template <typename Fn, typename... ArgTypes>
struct is_invocable : is_invocable_impl<std::result_of<Fn(ArgTypes...)>>::type
{
};
#else
template <typename Fn, typename... ArgTypes>
using is_invocable = std::is_invocable<Fn, ArgTypes...>;
#endif

template <typename Fn, typename V, typename I>
constexpr typename std::enable_if<is_invocable<Fn, V, I>::value, int>::type call_with_index(Fn&& f, V&& v, I&& i)
{
    f(std::forward<V>(v), std::forward<I>(i));
    return 0;
}

template <typename Fn, typename V, typename I>
constexpr typename std::enable_if<is_invocable<Fn, V>::value, int>::type call_with_index(Fn&& f, V&& v, I&& i)
{
    f(std::forward<V>(v));
    (void)(i);
    return 0;
}

template <typename... Ts, int... Is, typename F>
constexpr void foreach_impl(tiny_tuple::tuple<Ts...>& tup, std::integer_sequence<int, Is...>, F&& fun)
{
    auto l = {call_with_index(std::forward<F>(fun), tiny_tuple::get<Is>(tup), kvasir::mpl::uint_<Is>{})...};
    (void)l;
}

template <typename... Ts, int... Is, typename F>
constexpr auto foreach_impl(tuple<Ts...> const& tup, std::integer_sequence<int, Is...>, F&& fun)
{
    auto l = {call_with_index(std::forward<F>(fun), tiny_tuple::get<Is>(tup), kvasir::mpl::uint_<Is>{})...};
    (void)l;
}

}  // namespace detail
}  // namespace tiny_tuple
#endif
