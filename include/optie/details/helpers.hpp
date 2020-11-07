#ifndef ENN_OPTIE_DETAILS_HELPERS_HPP
#define ENN_OPTIE_DETAILS_HELPERS_HPP

#include <cstdlib>     // std::exit
#include <iostream>    // std::cerr
#include <exception>   // std::exception
#include <type_traits> // std::is_convertible

namespace optie {
namespace details {
namespace helpers {

    // -------------------------------------------------------------------------
    //   NOCOPY HELPER
    // -------------------------------------------------------------------------

    struct nocopy {
        nocopy() = default;

        nocopy(nocopy const&) = delete;
        nocopy(nocopy&&) = default;

        nocopy& operator=(nocopy const&) = delete;
        nocopy& operator=(nocopy&&) = default;
    };

    struct nomove {
        nomove() = default;

        nomove(nomove const&) = default;
        nomove(nomove&&) = delete;

        nomove& operator=(nomove const&) = default;
        nomove& operator=(nomove&&) = delete;
    };

    // -------------------------------------------------------------------------
    //   TAGGED VALUE HELPER
    // -------------------------------------------------------------------------

    template<typename Value, typename Tag>
    struct tagged_value {
        Value value;
    };

    template<typename Tag>
    struct tagged_value<void, Tag> {};

    // -------------------------------------------------------------------------
    //   COPY ADAPTER HELPER
    // -------------------------------------------------------------------------

    template<typename T>
    class copy_adapter {

        T value_;

    public:

        template<typename V>
        copy_adapter(V&& value) {
            value_.init_from(std::forward<V>(value));
        }

        copy_adapter(copy_adapter const& other) {
            value_.init_from_other_by_copy(other.value_);
        }

        copy_adapter(copy_adapter&& other) {
            value_.init_from_other_by_move(std::move(other.value_));
        }

        copy_adapter& operator=(copy_adapter const& other) {
            value_.deinit();
            value_.init_from_other_by_copy(other.value_);
        }

        copy_adapter& operator=(copy_adapter&& other) {
            value_.deinit();
            value_.init_from_other_by_move(std::move(other.value_));
        }

        ~copy_adapter() {
            value_.deinit();
        }

        T& adaptee() {
            return value_;
        }

        T const& adaptee() const {
            return value_;
        }

    };

    // -------------------------------------------------------------------------
    //   ERROR HANDLE HELPER
    // -------------------------------------------------------------------------

    [[noreturn]] void print_and_exit(char const* message) {
        if (message != NULL)
            std::cerr << message << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // -------------------------------------------------------------------------
    //   SFINAE HELPER
    // -------------------------------------------------------------------------

    template<typename S, typename T>
    using enable_if_convertible =
        typename std::enable_if<std::is_convertible<S, T>::value, int>::type;

    // -------------------------------------------------------------------------
    //   IFELSE HELPER
    // -------------------------------------------------------------------------

    template<typename, typename, bool>
    struct if_else;

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

    template<typename...>
    struct biggest;

    template<typename T>
    struct biggest<T> {
        using type = T;
    };

    template<typename T, typename U, typename... O>
    struct biggest<T, U, O...> {
        using type = greater_equal_t<T, typename biggest<U, O...>::type>;
    };

    template<typename... T>
    using biggest_t = typename biggest<T...>::type;

} // namespace helpers
} // namespace details
} // namespace optie

#endif // ENN_OPTIE_DETAILS_HELPERS_HPP
