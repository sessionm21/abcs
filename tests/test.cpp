#include <cstdlib>
#include "greatest.h"
#include "refcount_test.hpp"
#include "gc_test.hpp"
#include "trie_test.hpp"

/* Sanity check test */
TEST one_should_equal_one() {
    ASSERT_EQm("Reality is broken!", 1, 1);
    PASS();
}

GREATEST_MAIN_DEFS();

int main(int argc, char** argv) {
    GREATEST_MAIN_BEGIN();
    RUN_TEST(one_should_equal_one);
    RUN_SUITE(refcount_suite);
    RUN_SUITE(gc_suite);
    RUN_SUITE(trie_suite);
    GREATEST_MAIN_END();
}
