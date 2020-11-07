#ifndef ENN_OPTIE_DETAILS_OPTION_IMPL_HPP
#define ENN_OPTIE_DETAILS_OPTION_IMPL_HPP

#include "helpers.hpp"
#include "memory.hpp"

#include <utility> // std::move

namespace optie {
namespace details {

    // -------------------------------------------------------------------------
    //   TAG TYPES
    // -------------------------------------------------------------------------

    template<typename V>
    using some_type = helpers::tagged_value<V, class some_tag>;

    using none_type = helpers::tagged_value<void, class none_tag>;

    // -------------------------------------------------------------------------
    //   OPTION IMPL
    // -------------------------------------------------------------------------

    template<typename T>
    class option_impl : helpers::nocopy, helpers::nomove {

        memory::storage<sizeof(T), alignof(T)> value_;
        bool has_value_ = false;

    public: // init callback implementations -----------------------------------

        template<typename V>
        void init_from(some_type<V&>&& some) {
            has_value_ = true;
            value_.template create<T/*required*/>(some.value);
        }

        template<typename V>
        void init_from(some_type<V&&>&& some) {
            has_value_ = true;
            value_.template create<T/*required*/>(std::move(some.value));
        }

        void init_from(none_type) {
            has_value_ = false;
        }

        void init_from_other_by_copy(option_impl<T> const& other) {
            has_value_ = other.has_value_;
            if (other.has_value_)
                value_.template create<T>(*other.value());
        }

        void init_from_other_by_move(option_impl<T>&& other) {
            has_value_ = other.has_value_;
            if (other.has_value_)
                value_.template create<T>(std::move(*other.value()));
        }

        void deinit() {
            if (has_value_)
                value_.template destroy<T>();
        }

    public: // api implementations ---------------------------------------------

        T* value() {
            return value_.template get_as<T>();
        }

        T const* value() const {
            return value_.template get_as<T>();
        }

        bool is_some() const {
            return has_value_;
        }

    };

} // namespace details
} // namespace optie

#endif // ENN_OPTIE_DETAILS_OPTION_IMPL_HPP
