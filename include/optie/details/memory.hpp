#ifndef ENN_OPTIE_DETAILS_MEMORY_HPP
#define ENN_OPTIE_DETAILS_MEMORY_HPP

#include "helpers.hpp"

#include <cstddef> // std::size_t
#include <utility> // std::forward

namespace optie {
namespace details {
namespace memory {

    template<std::size_t S, std::size_t A>
    class storage : helpers::nocopy, helpers::nomove {

        alignas(A) unsigned char raw_[S];

    public:

        template<typename T>
        T const* get_as() const {
            return reinterpret_cast<T const*>(&raw_);
        }

        template<typename T>
        T* get_as() {
            return reinterpret_cast<T*>(&raw_);
        }

        template<typename T, typename V>
        void create(V&& value) {
            new (get_as<T>()) T(std::forward<V>(value));
        }

        template<typename T>
        void destroy() {
            get_as<T>()->~T();
        }

    };

} // namespace memory
} // namespace details
} // namespace optie

#endif // ENN_OPTIE_DETAILS_MEMORY_HPP
