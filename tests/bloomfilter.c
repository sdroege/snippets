/* This file is part of libsnippets
 *
 * Copyright (C) 2010 Sebastian Dröge <slomo@circular-chaos.org>
 * 
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <check.h>

#include <snippets/bloomfilter.h>
#include <snippets/rand.h>
#include <snippets/skiplist.h>

typedef struct _TestData TestData;
struct _TestData
{
  uint32_t data[16];
};

static int
compare_data (const void *o1, const void *o2, void *user_data)
{
  int i;
  const TestData *a = o1;
  const TestData *b = o2;

  for (i = 0; i < 16; i++) {
    if (a->data[i] < b->data[i])
      return -1;
    else if (a->data[i] > b->data[i])
      return 1;
  }

  return 0;
}

START_TEST (test_general)
{
  SnippetsBloomFilter *filter =
      snippets_bloom_filter_new (100 * 1000 * 8, 16, 64);
  SnippetsRand *rand = snippets_rand_new (0xdeadbeef);
  SnippetsSkipList *list =
      snippets_skip_list_new (8, 0.25, sizeof (TestData), NULL, NULL,
      compare_data, NULL, NULL, NULL);
  const int N = 10000;
  TestData data, *data2;
  int i, j;
  SnippetsSkipListNode *n;

  fail_unless (snippets_bloom_filter_n_hash_functions (filter) == 16);
  fail_unless (snippets_bloom_filter_size (filter) == 100 * 1000 * 8);
  fail_unless (snippets_bloom_filter_n_elements (filter) == 0);
  fail_unless (snippets_bloom_filter_false_positive_rate (filter) == 0);

  for (i = 0; i < N; i++) {
    for (j = 0; j < 16; j++)
      data.data[j] = snippets_rand_uint32 (rand);

    snippets_skip_list_insert (list, &data);
  }

  /* There are no false positives in this example! */
  for (n = snippets_skip_list_head (list); n;
      n = snippets_skip_list_node_next (n)) {
    data2 = snippets_skip_list_node_get (n, TestData);
    fail_if (snippets_bloom_filter_contains (filter, (uint8_t *) data2->data,
            sizeof (TestData)));
    snippets_bloom_filter_insert (filter, (uint8_t *) data2->data,
        sizeof (TestData));
    fail_unless (snippets_bloom_filter_contains (filter,
            (uint8_t *) data2->data, sizeof (TestData)));
  }

  fail_unless (snippets_bloom_filter_n_elements (filter) ==
      snippets_skip_list_length (list));

  snippets_skip_list_free (list);
  snippets_rand_free (rand);
  snippets_bloom_filter_free (filter);
}

END_TEST;

#define CREATE_TEST(filter_size, n_hash_functions, hash_size, elements) \
START_TEST (test_##filter_size##_##n_hash_functions##_##hash_size##_##elements) \
{ \
  SnippetsBloomFilter *filter = snippets_bloom_filter_new (filter_size, n_hash_functions, hash_size); \
  SnippetsRand *rand = snippets_rand_new (0xdeadbeef); \
  SnippetsSkipList *list = snippets_skip_list_new (8, 0.25, sizeof (TestData), NULL, NULL, compare_data, NULL, NULL, NULL); \
  const int N = elements; \
  const int N2 = 10000; \
  TestData data, *data2; \
  int i, j; \
  SnippetsSkipListNode *n; \
  double false_positives = 0; \
  double max_fp_rate; \
  \
  for (i = 0; i < N; i++) { \
    for (j = 0; j < 16; j++) \
      data.data[j] = snippets_rand_uint32 (rand); \
    \
    snippets_skip_list_insert (list, &data); \
  } \
  \
  for (n = snippets_skip_list_head (list); n; n = snippets_skip_list_node_next (n)) { \
    data2 = snippets_skip_list_node_get (n, TestData); \
    snippets_bloom_filter_insert (filter, (uint8_t *) data2->data, sizeof (TestData)); \
  } \
  \
  fail_unless (snippets_bloom_filter_n_elements (filter) == snippets_skip_list_length (list)); \
  max_fp_rate = snippets_bloom_filter_false_positive_rate (filter); \
  \
  for (n = snippets_skip_list_head (list); n; n = snippets_skip_list_node_next (n)) { \
    data2 = snippets_skip_list_node_get (n, TestData); \
    fail_unless (snippets_bloom_filter_contains (filter, (uint8_t *) data2->data, sizeof (TestData))); \
  } \
  \
  for (i = 0; i < N2; i++) { \
    while (TRUE) { \
      for (j = 0; j < 16; j++) \
        data.data[j] = snippets_rand_uint32 (rand); \
      if (snippets_skip_list_find (list, &data, TRUE) == NULL) \
        break; \
    } \
    \
    if (snippets_bloom_filter_contains (filter, (uint8_t *) data.data, sizeof (TestData))) \
      false_positives++; \
  } \
  \
  fail_unless (false_positives / N2 <= max_fp_rate * 1.08, \
    "expected: %lf  was: %lf", max_fp_rate * 1.08, \
    false_positives / N2); \
  \
  snippets_skip_list_free (list); \
  snippets_rand_free (rand); \
  snippets_bloom_filter_free (filter); \
} \
\
END_TEST;

CREATE_TEST (15000, 5, 64, 2000);
CREATE_TEST (15000, 5, 128, 2000);
CREATE_TEST (150000, 11, 128, 10000);
CREATE_TEST (150000, 11, 256, 10000);
CREATE_TEST (150000, 11, 512, 10000);
CREATE_TEST (150000, 11, 1024, 1000);
CREATE_TEST (100000, 70, 1024, 1000);

START_TEST (test_optimal_n_hash_functions)
{
  fail_unless (snippets_bloom_filter_optimal_n_hash_functions (15000,
          2000) == 5);
  fail_unless (snippets_bloom_filter_optimal_n_hash_functions (150000,
          10000) == 11);
  fail_unless (snippets_bloom_filter_optimal_n_hash_functions (100000,
          1000) == 70);
}

END_TEST;

static Suite *
bloomfilter_suite (void)
{
  Suite *s = suite_create ("BloomFilter");

  /* Core test case */
  TCase *tc_general = tcase_create ("general");
  tcase_add_test (tc_general, test_general);
  tcase_add_test (tc_general, test_15000_5_64_2000);
  tcase_add_test (tc_general, test_15000_5_128_2000);
  tcase_add_test (tc_general, test_150000_11_128_10000);
  tcase_add_test (tc_general, test_150000_11_256_10000);
  tcase_add_test (tc_general, test_150000_11_512_10000);
  tcase_add_test (tc_general, test_150000_11_1024_1000);
  tcase_add_test (tc_general, test_100000_70_1024_1000);
  tcase_add_test (tc_general, test_optimal_n_hash_functions);
  suite_add_tcase (s, tc_general);

  return s;
}

int
main (void)
{
  int number_failed;
  Suite *s = bloomfilter_suite ();
  SRunner *sr = srunner_create (s);
  srunner_run_all (sr, CK_NORMAL);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
