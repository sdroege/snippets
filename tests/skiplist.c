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
#include <string.h>
#include <time.h>

#include <snippets/skiplist.h>
#include <snippets/rand.h>

static void
copy_string (void *dest, const void *src)
{
  char **d = dest;
  const char *s = src;

  *d = strdup (s);
}

static int
compare_string (const void *a, const void *b, void *user_data)
{
  return strcmp (a, b);
}

START_TEST (test_insert_remove_find)
{
  SnippetsSkipList *list;
  SnippetsSkipListNode *node;
  char **str, **str2;

  list =
      snippets_skip_list_new_pointer (10, 0.5, copy_string, free,
      compare_string, NULL, NULL);

  snippets_skip_list_insert (list, (void *) "abc");
  snippets_skip_list_insert (list, (void *) "bcd");
  snippets_skip_list_insert (list, (void *) "cde");
  snippets_skip_list_insert (list, (void *) "aaa");
  snippets_skip_list_insert (list, (void *) "xyz");
  snippets_skip_list_insert (list, (void *) "jkl");
  snippets_skip_list_insert (list, (void *) "uvw");
  snippets_skip_list_insert (list, (void *) "bcc");

  fail_unless (snippets_skip_list_length (list) == 8);

  node = snippets_skip_list_head (list);
  fail_unless (node != NULL);
  str = snippets_skip_list_node_get (node, char *);
  fail_unless (str && *str && strcmp (*str, "aaa") == 0);

  node = snippets_skip_list_node_next (node);
  fail_unless (node != NULL);
  str = snippets_skip_list_node_get (node, char *);
  fail_unless (str && *str && strcmp (*str, "abc") == 0);

  node = snippets_skip_list_node_next (node);
  fail_unless (node != NULL);
  str = snippets_skip_list_node_get (node, char *);
  fail_unless (str && *str && strcmp (*str, "bcc") == 0);

  node = snippets_skip_list_node_next (node);
  fail_unless (node != NULL);
  str = snippets_skip_list_node_get (node, char *);
  fail_unless (str && *str && strcmp (*str, "bcd") == 0);

  node = snippets_skip_list_node_next (node);
  fail_unless (node != NULL);
  str = snippets_skip_list_node_get (node, char *);
  fail_unless (str && *str && strcmp (*str, "cde") == 0);

  node = snippets_skip_list_node_next (node);
  fail_unless (node != NULL);
  str = snippets_skip_list_node_get (node, char *);
  fail_unless (str && *str && strcmp (*str, "jkl") == 0);

  node = snippets_skip_list_node_next (node);
  fail_unless (node != NULL);
  str = snippets_skip_list_node_get (node, char *);
  fail_unless (str && *str && strcmp (*str, "uvw") == 0);

  node = snippets_skip_list_node_next (node);
  fail_unless (node != NULL);
  str = snippets_skip_list_node_get (node, char *);
  fail_unless (str && *str && strcmp (*str, "xyz") == 0);


  node = snippets_skip_list_find (list, "cde", FALSE);
  fail_unless (node != NULL);
  str = snippets_skip_list_node_get (node, char *);
  fail_unless (str && *str && strcmp (*str, "cde") == 0);

  node = snippets_skip_list_find (list, "cdd", FALSE);
  fail_unless (node != NULL);
  str = snippets_skip_list_node_get (node, char *);
  fail_unless (str && *str && strcmp (*str, "bcd") == 0);

  node = snippets_skip_list_find (list, "cde", TRUE);
  fail_unless (node != NULL);
  str = snippets_skip_list_node_get (node, char *);
  fail_unless (str && *str && strcmp (*str, "cde") == 0);

  node = snippets_skip_list_find (list, "cdd", TRUE);
  fail_unless (node == NULL);

  snippets_skip_list_remove_value (list, "cde");
  fail_unless (snippets_skip_list_length (list) == 7);
  node = snippets_skip_list_find (list, "cde", TRUE);
  fail_unless (node == NULL);

  node = snippets_skip_list_find (list, "abc", TRUE);
  fail_unless (node != NULL);
  snippets_skip_list_remove (list, node);
  fail_unless (snippets_skip_list_length (list) == 6);
  node = snippets_skip_list_find (list, "abc", TRUE);
  fail_unless (node == NULL);

  str = NULL;
  for (node = snippets_skip_list_head (list); node;
      node = snippets_skip_list_node_next (node)) {
    str2 = snippets_skip_list_node_get (node, char *);
    fail_unless (str2 != NULL);
    if (str) {
      fail_unless (strcmp (*str, *str2) < 0);
    }
    str = str2;
  }

  str = NULL;
  for (node = snippets_skip_list_tail (list); node;
      node = snippets_skip_list_node_prev (node)) {
    str2 = snippets_skip_list_node_get (node, char *);
    fail_unless (str2 != NULL);
    if (str) {
      fail_unless (strcmp (*str, *str2) > 0);
    }
    str = str2;
  }

  snippets_skip_list_remove_value (list, "jkl");
  fail_unless (snippets_skip_list_length (list) == 5);
  node = snippets_skip_list_find (list, "jkl", TRUE);
  fail_unless (node == NULL);

  node = snippets_skip_list_find (list, "uvw", TRUE);
  fail_unless (node != NULL);
  snippets_skip_list_remove (list, node);
  fail_unless (snippets_skip_list_length (list) == 4);
  node = snippets_skip_list_find (list, "uvw", TRUE);
  fail_unless (node == NULL);

  snippets_skip_list_remove_value (list, "bcc");
  fail_unless (snippets_skip_list_length (list) == 3);
  node = snippets_skip_list_find (list, "bcc", TRUE);
  fail_unless (node == NULL);

  node = snippets_skip_list_find (list, "aaa", TRUE);
  fail_unless (node != NULL);
  snippets_skip_list_remove (list, node);
  fail_unless (snippets_skip_list_length (list) == 2);
  node = snippets_skip_list_find (list, "aaa", TRUE);
  fail_unless (node == NULL);

  node = snippets_skip_list_find (list, "xyz", FALSE);
  fail_unless (node != NULL);
  str = snippets_skip_list_node_get (node, char *);
  fail_unless (str && *str && strcmp (*str, "xyz") == 0);

  node = snippets_skip_list_find (list, "bcd", FALSE);
  fail_unless (node != NULL);
  str = snippets_skip_list_node_get (node, char *);
  fail_unless (str && *str && strcmp (*str, "bcd") == 0);

  snippets_skip_list_free (list);
}

END_TEST;

START_TEST (test_copy)
{
  SnippetsSkipList *list, *copy;
  SnippetsSkipListNode *node, *node2;
  char **str, **str2;

  list =
      snippets_skip_list_new_pointer (10, 0.5, copy_string, free,
      compare_string, NULL, NULL);

  snippets_skip_list_insert (list, (void *) "abc");
  snippets_skip_list_insert (list, (void *) "bcd");
  snippets_skip_list_insert (list, (void *) "cde");
  snippets_skip_list_insert (list, (void *) "aaa");
  /* Will be free'd because it already exists */
  snippets_skip_list_insert (list, (void *) strdup ("abc"));
  snippets_skip_list_insert (list, (void *) "xyz");
  snippets_skip_list_insert (list, (void *) "jkl");
  snippets_skip_list_insert (list, (void *) "uvw");
  snippets_skip_list_insert (list, (void *) "bcc");

  copy = snippets_skip_list_copy (list, NULL);

  fail_unless (snippets_skip_list_length (list) == 8);
  fail_unless (snippets_skip_list_length (copy) == 8);

  for (node = snippets_skip_list_head (list), node2 =
      snippets_skip_list_head (copy); node || node2;
      node = snippets_skip_list_node_next (node), node2 =
      snippets_skip_list_node_next (node2)) {
    fail_unless (node != NULL && node2 != NULL);

    str = snippets_skip_list_node_get (node, char *);
    str2 = snippets_skip_list_node_get (node2, char *);

    fail_unless (str != NULL && str2 != NULL);
    fail_unless (str != str2);
    fail_unless (*str != NULL && *str2 != NULL);
    fail_unless (*str != *str2);
    fail_unless (strcmp (*str, *str2) == 0);
  }

  snippets_skip_list_free (list);
  snippets_skip_list_free (copy);
}

END_TEST;

typedef struct _TestData TestData;
struct _TestData
{
  uint32_t val[4];
};

static int
compare_data (const void *o1, const void *o2, void *user_data)
{
  const uint32_t *a = ((TestData *) o1)->val;
  const uint32_t *b = ((TestData *) o2)->val;
  uint64_t *calls = (uint64_t *) user_data;

  if (calls)
    *calls = *calls + 1;

  if (a[0] < b[0])
    return -1;
  if (a[0] > b[0])
    return 1;
  if (a[1] < b[1])
    return -1;
  if (a[1] > b[1])
    return 1;
  if (a[2] < b[2])
    return -1;
  if (a[2] > b[2])
    return 1;
  if (a[3] < b[3])
    return -1;
  if (a[3] > b[3])
    return 1;
  return 0;
}

START_TEST (test_non_pointer)
{
  SnippetsSkipList *list;
  SnippetsSkipListNode *node;
  TestData test_data = { {0,}
  };
  TestData *data1, *data2;

  list =
      snippets_skip_list_new (10, 0.5, sizeof (TestData), NULL, NULL,
      compare_data, NULL, NULL);

  test_data.val[0] = 1;
  snippets_skip_list_insert (list, &test_data);

  test_data.val[0] = 2;
  snippets_skip_list_insert (list, &test_data);

  test_data.val[0] = 3;
  snippets_skip_list_insert (list, &test_data);

  test_data.val[0] = 4;
  snippets_skip_list_insert (list, &test_data);

  fail_unless (snippets_skip_list_length (list) == 4);

  data1 = NULL;
  for (node = snippets_skip_list_head (list); node;
      node = snippets_skip_list_node_next (node)) {
    data2 = snippets_skip_list_node_get (node, TestData);
    fail_unless (data2 != NULL);
    if (data1)
      fail_unless (compare_data (data1, data2, NULL) < 0);
    data1 = data2;
  }

  snippets_skip_list_free (list);
}

END_TEST;

START_TEST (test_find_performance)
{
  SnippetsRand *rand;
  SnippetsSkipList *list;
  SnippetsSkipListNode *node;
  TestData *test_data;
  static const int N = 32000;
  static const int M = 16000;
  int i;
  uint64_t calls;
  TestData **data1, **data2;

  rand = snippets_rand_new (SNIPPETS_RAND_MODE_MT19937, time (0));

  test_data = malloc (N * sizeof (TestData));
  for (i = 0; i < N; i++) {
    test_data[i].val[0] = snippets_rand_uint32 (rand);
    test_data[i].val[1] = snippets_rand_uint32 (rand);
    test_data[i].val[2] = snippets_rand_uint32 (rand);
    test_data[i].val[3] = snippets_rand_uint32 (rand);
  }

  list =
      snippets_skip_list_new_pointer (10, 0.5, NULL, NULL, compare_data, &calls,
      NULL);

  calls = 0;
  for (i = 0; i < N; i++)
    snippets_skip_list_insert (list, &test_data[i]);
  fail_unless (((double) calls) / ((double) N) <= 48);

  data1 = NULL;
  for (node = snippets_skip_list_head (list); node;
      node = snippets_skip_list_node_next (node)) {
    data2 = snippets_skip_list_node_get (node, TestData *);
    fail_unless (data2 != NULL);
    if (data1)
      fail_unless (compare_data (*data1, *data2, NULL) < 0);
    data1 = data2;
  }

  calls = 0;
  for (i = 0; i < M; i++) {
    uint32_t r = snippets_rand_uint32_range (rand, 0, N);

    node = snippets_skip_list_find (list, &test_data[r], TRUE);
    fail_unless (node != NULL);
    data1 = snippets_skip_list_node_get (node, TestData *);
    fail_unless (compare_data (*data1, &test_data[r], NULL) == 0);
  }
  fail_unless (((double) calls) / ((double) N) <= 48);

  snippets_skip_list_free (list);
  free (test_data);
  snippets_rand_free (rand);
}

END_TEST;

static Suite *
skiplist_suite (void)
{
  Suite *s = suite_create ("SkipList");

  /* Core test case */
  TCase *tc_general = tcase_create ("general");
  tcase_set_timeout(tc_general, 60);
  tcase_add_test (tc_general, test_insert_remove_find);
  tcase_add_test (tc_general, test_copy);
  tcase_add_test (tc_general, test_non_pointer);
  tcase_add_test (tc_general, test_find_performance);
  suite_add_tcase (s, tc_general);

  return s;
}

int
main (void)
{
  int number_failed;
  Suite *s = skiplist_suite ();
  SRunner *sr = srunner_create (s);
  srunner_run_all (sr, CK_NORMAL);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
