#ifndef ENN_OPTIE_TESTS_UTILS_ENTITIES_HPP
#define ENN_OPTIE_TESTS_UTILS_ENTITIES_HPP

#include "output.hpp"

#include <utility> // std::move

namespace optie {
namespace tests {
namespace utils {
namespace entities {

    // -------------------------------------------------------------------------
    //   COPYMOVE COUNTER
    // -------------------------------------------------------------------------

    struct copymove_counter {
        unsigned moved;
        unsigned copied;

        copymove_counter() : moved(0), copied(0) {
            ENN_OPTIE_TESTS_LOG("COPYMOVE_COUNTER: ctor");
        }

        ~copymove_counter() {
            ENN_OPTIE_TESTS_LOG("COPYMOVE_COUNTER: dtor");
        }

        copymove_counter(copymove_counter const& other)
            : moved(other.moved), copied(other.copied + 1) {

            ENN_OPTIE_TESTS_LOG("COPYMOVE_COUNTER: ctor (copy)");
        }

        copymove_counter(copymove_counter&& other)
            : moved(other.moved + 1), copied(other.copied) {

            ENN_OPTIE_TESTS_LOG("COPYMOVE_COUNTER: ctor (move)");
        }

        copymove_counter& operator=(copymove_counter const& other) {
            moved = other.moved;
            copied = other.copied + 1;

            ENN_OPTIE_TESTS_LOG("COPYMOVE_COUNTER: asop (copy)");

            return *this;
        }

        copymove_counter& operator=(copymove_counter&& other) {
            moved = other.moved + 1;
            copied = other.copied;

            ENN_OPTIE_TESTS_LOG("COPYMOVE_COUNTER: asop (move)");

            return *this;
        }
    };

    // -------------------------------------------------------------------------
    //   COPYMOVE COUNTER WITH A VALUE
    // -------------------------------------------------------------------------

    template<typename T>
    struct copymove_holder {
        copymove_counter ctr;
        T value;

        copymove_holder(T&& value) : value(std::move(value)) {}
        copymove_holder(T& value) : value(value) {}
    };

} // namespace entities
} // namespace utils
} // namespace tests
} // namespace optie

#endif // ENN_OPTIE_TESTS_UTILS_ENTITIES_HPP
