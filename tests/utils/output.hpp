#ifndef ENN_OPTIE_TESTS_UTILS_OUTPUT_HPP
#define ENN_OPTIE_TESTS_UTILS_OUTPUT_HPP

#include <cstdio>

#ifdef ENN_OPTIE_ENABLE_LOGGING
    #define ENN_OPTIE_TESTS_LOG(message) \
        std::fprintf(stderr, "[LOG] > %s\n", (message))
#else
    #define ENN_OPTIE_TESTS_LOG(message)
#endif

#define ENN_OPTIE_TESTS_ASSERT(expr, message)                                \
    if (!(expr)) {                                                           \
        std::fprintf(stderr, "[TEST] > (%s:%d) (ASSERTION FAILED: %s)\n",    \
                    __FILE__, __LINE__, (message));                          \
        std::abort();                                                        \
    }

#endif // ENN_OPTIE_TESTS_UTILS_OUTPUT_HPP
