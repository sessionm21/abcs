#include "gc_test.hpp"
#include <iostream>

greatest_test_res gc_init() {
  libab_gc_list list;
  libab_gc_list_init(&list);
  ASSERT_EQ(list.head_sentinel.next, &list.tail_sentinel);
  ASSERT_EQ(list.tail_sentinel.prev, &list.head_sentinel);
  PASS();
}

greatest_test_res gc_add() {
  libab_ref ref;
  libab_gc_list list;
  libab_gc_list_init(&list);
  libab_ref_new(&ref, new int, [](void* v) { delete ((int*) v); });
  libab_gc_add(&ref, NULL, &list);

  ASSERT_EQ(list.head_sentinel.next, ref.count);
  ASSERT_EQ(list.tail_sentinel.prev, ref.count);
  ASSERT_EQ(ref.count->next, &list.tail_sentinel);
  ASSERT_EQ(ref.count->prev, &list.head_sentinel);
  ASSERT_EQ(ref.count->visit_children, NULL);

  libab_ref_free(&ref);
  PASS();
}

greatest_test_res gc_run_basic() {
  // would also benefit from a test with valgrind
  libab_gc_list list;
  libab_ref ref1;
  libab_ref ref2;
  auto free_function = [](void* v) { delete[] ((int*) v); };
  auto visit_function = [](void*, libab_visitor_function_ptr, void*) {};

  libab_gc_list_init(&list);
  libab_ref_new(&ref1, new int[5], free_function);
  libab_ref_new(&ref2, new int[5], free_function);
  libab_gc_add(&ref1, visit_function, &list);
  libab_gc_add(&ref2, visit_function, &list);
  libab_gc_run(&list);

  ASSERT_EQ(list.head_sentinel.next, ref1.count);
  ASSERT_EQ(list.head_sentinel.next->next, ref2.count);

  libab_ref_free(&ref1);
  libab_ref_free(&ref2);

  PASS();
}

struct dumb_container {
    bool freed;
    libab_ref child;
    
    dumb_container() { freed = false; libab_ref_null(&child); }
    void replace_child(const libab_ref* with) { libab_ref_free(&child); libab_ref_copy(with, &child); }
    void mock_free() { freed = true; libab_ref_free(&child); }
};

greatest_test_res gc_run_robust() {
    libab_ref ref1;
    libab_ref ref2;
    libab_gc_list list;
    auto c1 = new dumb_container;
    auto c2 = new dumb_container;
    auto free_func = [](void* c) { ((dumb_container*) c)->mock_free(); };
    auto visit_func = [](void* self, libab_visitor_function_ptr f, void* data) {
        libab_gc_visit(&((dumb_container*) self)->child, f, data);
    };

    libab_gc_list_init(&list);
    libab_ref_new(&ref1, c1, free_func);
    libab_ref_new(&ref2, c2, free_func);
    c1->replace_child(&ref2);
    c2->replace_child(&ref1);

    libab_gc_add(&ref1, visit_func, &list);
    libab_gc_add(&ref2, visit_func, &list);

    ASSERT_EQ(c1->freed, false);
    ASSERT_EQ(c1->freed, false);

    libab_ref_free(&ref1);
    libab_ref_free(&ref2);

    ASSERT_EQ(c1->freed, false);
    ASSERT_EQ(c1->freed, false);

    libab_gc_run(&list);
    ASSERT_EQ(c1->freed, true);
    ASSERT_EQ(c1->freed, true);

    delete c1;
    delete c2;

    PASS();
}

SUITE(gc_suite) {
  RUN_TEST(gc_init);
  RUN_TEST(gc_add);
  RUN_TEST(gc_run_basic);
  RUN_TEST(gc_run_robust);
}
