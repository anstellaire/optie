#ifndef ENN_OPTIE_RESULT_HPP
#define ENN_OPTIE_RESULT_HPP

#include "details/details.hpp"

#include <type_traits> // std::is_reference std::is_same
#include <utility>     // std::move

namespace optie {

    // -------------------------------------------------------------------------
    //   TAG TYPE CONSTRUCTORS
    // -------------------------------------------------------------------------

    template<typename T>
    details::ok_type<T&&> ok(T&& value) {
        return details::ok_type<T&&>{std::forward<T>(value)};
    }

    template<typename E>
    details::err_type<E&&> err(E&& value) {
        return details::err_type<E&&>{std::forward<E>(value)};
    }

    // -------------------------------------------------------------------------
    //   RESULT
    // -------------------------------------------------------------------------

    template<typename T, typename E>
    class result {

        static_assert(
            !std::is_reference<T>::value && !std::is_reference<E>::value,
            "&, && are not supported for T, E in result<T, E>");
        static_assert(
            !std::is_same<T, void>::value && !std::is_same<E, void>::value,
            "void is not supported for T, E in result<T, E>");

    private: // data members ---------------------------------------------------

        details::helpers::copy_adapter<details::result_impl<T, E>> storage_;

    public: // special methods -------------------------------------------------

        template<typename V, details::helpers::enable_if_convertible<V, T> = 0>
        result(details::ok_type<V>&& ok) : storage_(std::move(ok)) {};

        template<typename V, details::helpers::enable_if_convertible<V, E> = 0>
        result(details::err_type<V>&& err) : storage_(std::move(err)) {};

    public: // user api --------------------------------------------------------

        bool is_ok() const {
            return storage_.adaptee().is_ok();
        }

        T&& or_exit(char const* message = NULL) && {
            if (!is_ok())
                details::helpers::print_and_exit(message);

            return std::move(*storage_.adaptee().value());
        }

        T& or_exit(char const* message = NULL) & {
            if (!is_ok())
                details::helpers::print_and_exit(message);

            return *storage_.adaptee().value();
        }

        template<typename F>
        T or_call(F callback) && {
            if (!is_ok())
                return callback(std::move(*storage_.adaptee().error()));

            return std::move(*storage_.adaptee().value());
        }

        template<typename F>
        T or_call(F callback) & {
            if (!is_ok())
                return callback(*storage_.adaptee().value());

            return *storage_.adaptee().value();
        }

        template<typename V, details::helpers::enable_if_convertible<V, T> = 0>
        T or_default(V&& value) && {
            if (!is_ok())
                return std::forward<V>(value);

            return std::move(*storage_.adaptee().value());
        }

        template<typename V, details::helpers::enable_if_convertible<V, T> = 0>
        T or_default(V&& value) & {
            if (!is_ok())
                return std::forward<V>(value);

            return *storage_.adaptee().value();
        }

    };

} // namespace optie

#endif // ENN_OPTIE_RESULT_HPP
