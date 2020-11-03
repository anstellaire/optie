#ifndef ENN_OPTIE_STATUS_HPP
#define ENN_OPTIE_STATUS_HPP

#include "helper.hpp"

#include <utility> // std::forward

namespace optie {

    // -------------------------------------------------------------------------
    //   STATUS WRAPPER
    // -------------------------------------------------------------------------

    template<typename T>
    struct ok_t {
        T value;
    };

    template<typename E>
    struct err_t {
        E value;
    };

    template<typename T>
    ok_t<helper::forward_reference<T>> ok(T&& value) {
        return ok_t<helper::forward_reference<T>>{std::forward<T>(value)};
    }

    template<typename E>
    err_t<helper::forward_reference<E>> err(E&& value) {
        return err_t<helper::forward_reference<E>>{std::forward<E>(value)};
    }

} // namespace optie

#endif // ENN_OPTIE_STATUS_HPP
