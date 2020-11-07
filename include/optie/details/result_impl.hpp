#ifndef ENN_OPTIE_DETAILS_RESULT_IMPL_HPP
#define ENN_OPTIE_DETAILS_RESULT_IMPL_HPP

#include "helpers.hpp"
#include "memory.hpp"

#include <utility> // std::move

namespace optie {
namespace details {

    // -------------------------------------------------------------------------
    //   TAG TYPES
    // -------------------------------------------------------------------------

    template<typename T>
    using ok_type = helpers::tagged_value<T, class ok_tag>;

    template<typename T>
    using err_type = helpers::tagged_value<T, class err_tag>;

    // -------------------------------------------------------------------------
    //   OPTION IMPL
    // -------------------------------------------------------------------------

    template<typename T, typename E>
    class result_impl : helpers::nocopy, helpers::nomove {

        using A = helpers::biggest_t<T, E>;

        memory::storage<sizeof(A), alignof(A)> value_;
        bool ok_;

    public: // init callback implementations -----------------------------------

        template<typename V>
        void init_from(ok_type<V&>&& ok) {
            ok_ = true;
            value_.template create<T/*required*/>(ok.value);
        }

        template<typename V>
        void init_from(ok_type<V&&>&& ok) {
            ok_ = true;
            value_.template create<T/*required*/>(std::move(ok.value));
        }

        template<typename V>
        void init_from(err_type<V&>&& err) {
            ok_ = false;
            value_.template create<E/*required*/>(err.value);
        }

        template<typename V>
        void init_from(err_type<V&&>&& err) {
            ok_ = false;
            value_.template create<E/*required*/>(std::move(err.value));
        }

        void init_from_other_by_copy(result_impl<T,E> const& other) {
            ok_ = other.ok_;
            if (other.ok_)
                value_.template create<T>(*other.value_.template get_as<T>());
            else
                value_.template create<E>(*other.value_.template get_as<E>());

        }

        void init_from_other_by_move(result_impl<T,E>&& other) {
            ok_ = other.ok_;
            if (other.ok_)
                value_.template create<T>(std::move(*other.value_.template get_as<T>()));
            else
                value_.template create<E>(std::move(*other.value_.template get_as<E>()));
        }

        void deinit() {
            if (ok_)
                value_.template destroy<T>();
            else
                value_.template destroy<E>();
        }

    public: // api implementations ---------------------------------------------

        T* value() {
            return value_.template get_as<T>();
        }

        T const* value() const {
            return value_.template get_as<T>();
        }

        E* error() {
            return value_.template get_as<E>();
        }

        E const* error() const {
            return value_.template get_as<E>();
        }

        bool is_ok() const {
            return ok_;
        }

    };

} // namespace details
} // namespace optie

#endif // ENN_OPTIE_DETAILS_RESULT_IMPL_HPP
