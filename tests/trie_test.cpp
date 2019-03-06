#include "trie_test.hpp"

greatest_test_res trie_init() {
  libab_trie trie;
  libab_trie_init(&trie);
  ASSERT_EQ(trie.head, NULL);
  ASSERT_EQ(trie.empty_list.head, NULL);
  ASSERT_EQ(trie.empty_list.tail, NULL);
  PASS();
}

greatest_test_res trie_clear() {
  libab_trie trie;

  libab_trie_init(&trie);

  ASSERT_EQ(libab_trie_put(&trie, "derp", &trie), LIBAB_SUCCESS);
  GREATEST_ASSERT(trie.head != NULL);  
  ASSERT_EQ(trie.empty_list.head, NULL);
  ASSERT_EQ(trie.empty_list.tail, NULL);

  ASSERT_EQ(libab_trie_put(&trie, "doge", &trie), LIBAB_SUCCESS);
  GREATEST_ASSERT(trie.head != NULL);  
  ASSERT_EQ(trie.empty_list.head, NULL);
  ASSERT_EQ(trie.empty_list.tail, NULL);

  libab_trie_clear(&trie);
  ASSERT_EQ(trie.head, NULL);
  ASSERT_EQ(trie.empty_list.head, NULL);
  ASSERT_EQ(trie.empty_list.tail, NULL);
  
  PASS();
}

greatest_test_res trie_put_get() {
  libab_trie trie;
  libab_trie_init(&trie);

  ASSERT_EQ(libab_trie_put(&trie, "derp", &trie), LIBAB_SUCCESS);
  GREATEST_ASSERT(trie.head != NULL);  
  ASSERT_EQ(trie.empty_list.head, NULL);
  ASSERT_EQ(trie.empty_list.tail, NULL);

  ASSERT_EQ(libab_trie_put(&trie, "doge", &trie), LIBAB_SUCCESS);
  GREATEST_ASSERT(trie.head != NULL);  
  ASSERT_EQ(trie.empty_list.head, NULL);
  ASSERT_EQ(trie.empty_list.tail, NULL);
   
  ll *ptr = (ll*) libab_trie_get(&trie, "doge"); 
  ASSERT_EQ(ptr->head->data, &trie);

  ptr = (ll*) libab_trie_get(&trie, "derp");
  ASSERT_EQ(ptr->head->data, &trie);

  ptr = (ll*) libab_trie_get(&trie, "notinside");
  ASSERT_EQ(ptr->head, NULL);
  ASSERT_EQ(ptr->tail, NULL);

  libab_trie_clear(&trie);

  PASS();
}

SUITE(trie_suite) {
  RUN_TEST(trie_init);
  RUN_TEST(trie_clear);
  RUN_TEST(trie_put_get);
}
