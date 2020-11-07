#include "../include/optie/optie.hpp"

#include "utils/utils.hpp"

namespace optie {
namespace tests {

    static char const* EXIT_MESSAGE = "[TEST] > (" __FILE__ ") (ERROR: no value)";

    template<typename T, typename V>
    static optie::option<T> get_option(V&& value) {
        return std::forward<V>(value);
    }

    template<typename T, typename E, typename V>
    static optie::result<T, E> get_result(V&& value) {
        return std::forward<V>(value);
    }

    void test_copy_move_option() {
        using utils::entities::copymove_counter;
        using utils::entities::copymove_holder;

        { // BY MOVE
            auto&& rref = get_option<copymove_counter>(optie::some(copymove_counter{}))
                .or_exit(EXIT_MESSAGE);
            ENN_OPTIE_TESTS_ASSERT(rref.copied == 0 && rref.moved == 1,
                "test option by move failed");

            auto lval = get_option<copymove_counter>(optie::some(copymove_counter{}))
                .or_exit(EXIT_MESSAGE);
            ENN_OPTIE_TESTS_ASSERT(lval.copied == 0 && lval.moved == 2,
                "test option by move failed");
        }

        { // BY COPY
            copymove_counter ctr{};

            auto&& rref = get_option<copymove_counter>(optie::some(ctr))
                .or_exit(EXIT_MESSAGE);
            ENN_OPTIE_TESTS_ASSERT(rref.copied == 1 && rref.moved == 0,
                "test option by copy failed");

            auto lval = get_option<copymove_counter>(optie::some(ctr))
                .or_exit("ERROR: no value");
            ENN_OPTIE_TESTS_ASSERT(lval.copied == 1 && lval.moved == 1,
                "test option by copy failed");
        }

        { // BY EMPLACE
            char const* val = "Test c-string";

            auto&& rref = get_option<copymove_holder<char const*>>(optie::some(val))
                .or_exit(EXIT_MESSAGE);
            ENN_OPTIE_TESTS_ASSERT(rref.ctr.copied == 0 && rref.ctr.moved == 0,
                "test option by emplace failed");

            auto lval = get_option<copymove_holder<char const*>>(optie::some(val))
                .or_exit(EXIT_MESSAGE);
            ENN_OPTIE_TESTS_ASSERT(lval.ctr.copied == 0 && lval.ctr.moved == 1,
                "test option by emplace failed");
        }
    }

    void test_copy_move_result() {
        using utils::entities::copymove_counter;
        using utils::entities::copymove_holder;

        { // BY MOVE
            auto&& rref = get_result<copymove_counter, bool>(optie::ok(copymove_counter{}))
                .or_exit(EXIT_MESSAGE);
            ENN_OPTIE_TESTS_ASSERT(rref.copied == 0 && rref.moved == 1,
                "test result by move failed");

            auto lval = get_result<copymove_counter, bool>(optie::ok(copymove_counter{}))
                .or_exit(EXIT_MESSAGE);
            ENN_OPTIE_TESTS_ASSERT(lval.copied == 0 && lval.moved == 2,
                "test result by move failed");
        }

        { // BY COPY
            copymove_counter ctr{};

            auto&& rref = get_result<copymove_counter, bool>(optie::ok(ctr))
                .or_exit(EXIT_MESSAGE);
            ENN_OPTIE_TESTS_ASSERT(rref.copied == 1 && rref.moved == 0,
                "test result by copy failed");

            auto lval = get_result<copymove_counter, bool>(optie::ok(ctr))
                .or_exit(EXIT_MESSAGE);
            ENN_OPTIE_TESTS_ASSERT(lval.copied == 1 && lval.moved == 1,
                "test result by copy failed");
        }

        { // BY EMPLACE
            char const* val = "Test c-string";

            auto&& rref = get_result<copymove_holder<char const*>, bool>(optie::ok(val))
                .or_exit(EXIT_MESSAGE);
            ENN_OPTIE_TESTS_ASSERT(rref.ctr.copied == 0 && rref.ctr.moved == 0,
                "test result by emplace failed");

            auto lval = get_result<copymove_holder<char const*>, bool>(optie::ok(val))
                .or_exit(EXIT_MESSAGE);
            ENN_OPTIE_TESTS_ASSERT(lval.ctr.copied == 0 && lval.ctr.moved == 1,
                "test result by emplace failed");
        }
    }

} // namespace tests
} // namespace optie

int main() {
    optie::tests::test_copy_move_option();
    optie::tests::test_copy_move_result();
}
