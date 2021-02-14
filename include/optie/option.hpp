#ifndef ENN_OPTIE_OPTION_HPP
#define ENN_OPTIE_OPTION_HPP

#include "details/details.hpp"

#include <type_traits> // std::is_reference std::is_same
#include <utility>     // std::move

namespace optie {

    // -------------------------------------------------------------------------
    //   TAG TYPE CONSTRUCTORS
    // -------------------------------------------------------------------------

    template<typename V>
    details::some_type<V&&> some(V&& value) {
        return details::some_type<V&&>{std::forward<V>(value)};
    }

    details::none_type none() {
        return details::none_type{};
    }

    // -------------------------------------------------------------------------
    //   OPTION
    // -------------------------------------------------------------------------

    template<typename T>
    class option {

        static_assert(
            !std::is_reference<T>::value,
            "&, && are not supported for T in option<T>");
        static_assert(
            !std::is_same<T, void>::value,
            "void is not supported for T in option<T>");

    private: // data members ---------------------------------------------------

        details::helpers::copy_adapter<details::option_impl<T>> storage_;

    public: // special methods -------------------------------------------------

        template<typename V, details::helpers::enable_if_convertible<V, T> = 0>
        option<T>(details::some_type<V>&& some) : storage_(std::move(some)) {};

        option<T>(details::none_type&& none) : storage_(std::move(none)) {};

    public: // user api --------------------------------------------------------

        bool is_some() const {
            return storage_.adaptee().is_some();
        }

        T&& or_exit(char const* message = NULL) && {
            if (!is_some())
                details::helpers::print_and_exit(message);

            return std::move(*storage_.adaptee().value());
        }

        T& or_exit(char const* message = NULL) & {
            if (!is_some())
                details::helpers::print_and_exit(message);

            return *storage_.adaptee().value();
        }

        template<typename F>
        T or_call(F callback) && {
            if (!is_some())
                return callback();

            return std::move(*storage_.adaptee().value());
        }

        template<typename F>
        T or_call(F callback) & {
            if (!is_some())
                return callback();

            return *storage_.adaptee().value();
        }

        template<typename V, details::helpers::enable_if_convertible<V, T> = 0>
        T or_default(V&& value) && {
            if (!is_some())
                return std::forward<V>(value);

            return std::move(*storage_.adaptee().value());
        }

        template<typename V, details::helpers::enable_if_convertible<V, T> = 0>
        T or_default(V&& value) & {
            if (!is_some())
                return std::forward<V>(value);

            return *storage_.adaptee().value();
        }

    };

} // namespace optie

#endif // ENN_OPTIE_OPTION_HPP
