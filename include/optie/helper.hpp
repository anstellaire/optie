#ifndef ENN_OPTIE_HELPER_HPP
#define ENN_OPTIE_HELPER_HPP

#include <cstdlib>     // std::exit
#include <iostream>    // std::cerr
#include <exception>   // std::exception
#include <type_traits> // std::is_convertible

namespace optie {
namespace helper {

    // -------------------------------------------------------------------------
    //   NOCOPY HELPER
    // -------------------------------------------------------------------------

    struct nocopy {
        nocopy() = default;
        nocopy(nocopy const&) = delete;
        nocopy& operator=(nocopy const&) = delete;
    };

    // -------------------------------------------------------------------------
    //   ERROR HANDLE HELPERS
    // -------------------------------------------------------------------------

    struct exit_handler {
        char const* message;

        [[noreturn]] void operator()() const {
            if (message != NULL)
                std::cerr << message << std::endl;
            std::exit(EXIT_FAILURE);
        }
    };

    // -------------------------------------------------------------------------
    //   SFINAE AND FORWARDING HELPERS
    // -------------------------------------------------------------------------

    template<typename T>
    using forward_reference = T&&;

    template<typename F, typename T>
    using enable_if_convertible =
        typename std::enable_if<std::is_convertible<F, T>::value, int>::type;

    // -------------------------------------------------------------------------
    //   IFELSE HELPER
    // -------------------------------------------------------------------------

    template<typename T, typename U, bool C>
    struct if_else {};

    template<typename T, typename U>
    struct if_else<T, U, false> {
        using type = U;
    };

    template<typename T, typename U>
    struct if_else<T, U, true> {
        using type = T;
    };

    template<typename T, typename U, bool C>
    using if_else_t = typename if_else<T, U, C>::type;

    template<typename T, typename U>
    using lesser_t = if_else_t<T, U, (sizeof(T) < sizeof(U))>;

    template<typename T, typename U>
    using lesser_equal_t = if_else_t<T, U, (sizeof(T) <= sizeof(U))>;

    template<typename T, typename U>
    using greater_t = if_else_t<T, U, (sizeof(T) > sizeof(U))>;

    template<typename T, typename U>
    using greater_equal_t = if_else_t<T, U, (sizeof(T) >= sizeof(U))>;

    // -------------------------------------------------------------------------
    //   MOST HELPER
    // -------------------------------------------------------------------------

    template<typename... T>
    struct most;

    template<typename T>
    struct most<T> {
        using big = T;
    };

    template<typename T, typename U, typename... O>
    struct most<T, U, O...> {
        using big = greater_equal_t<T, typename most<U, O...>::big>;
    };

    template<typename... T>
    using biggest_t = typename most<T...>::big;

} // namespace helper
} // namespace optie

#endif // ENN_OPTIE_HELPER_HPP
