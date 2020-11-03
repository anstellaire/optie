#ifndef ENN_OPTIE_MEMORY_HPP
#define ENN_OPTIE_MEMORY_HPP

#include "helper.hpp"

#include <cstddef>     // std::size_t
#include <utility>     // std::forward
#include <type_traits> // std::decay

namespace optie {
namespace memory {

    // -------------------------------------------------------------------------
    //   ALIGNED RAW MEMORY WRAPPER
    // -------------------------------------------------------------------------

    template<std::size_t S, std::size_t A>
    class storage : helper::nocopy {

    private:

        alignas(A) unsigned char _raw[S];

    public:

        template<typename T>
        T const* get_as() const {
            return reinterpret_cast<T const*>(&_raw);
        }

        template<typename T>
        T* get_as() {
            return reinterpret_cast<T*>(&_raw);
        }

        template<typename T>
        void create(T&& value) {
            using DecT = typename std::decay<T>::type;

            new (get_as<DecT>()) DecT(std::forward<T>(value));
        }

        template<typename T>
        void destroy() {
            using DecT = typename std::decay<T>::type;

            get_as<DecT>()->~DecT();
        }
    };

} // namespace memory
} // namespace optie

#endif // ENN_OPTIE_MEMORY_HPP
