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
#include <snippets/linkedlist.h>

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

static const char *string_a = "abc";
static const char *string_b = "def";
static const char *string_c = "ghi";
static const char *string_d = "jkl";

START_TEST (test_string_copy)
{
  SnippetsLinkedList *list, *copy;
  SnippetsLinkedListNode *n1, *n2, *n3, *n4, *l;
  char **str, **str2;

  list = snippets_linked_list_new_pointer (copy_string, free);

  fail_unless (snippets_linked_list_length (list) == 0);
  n1 = snippets_linked_list_append (list, (void *) string_a);
  fail_unless (n1 != NULL);
  fail_unless (snippets_linked_list_length (list) == 1);
  n2 = snippets_linked_list_append (list, (void *) string_b);
  fail_unless (n2 != NULL);
  fail_unless (snippets_linked_list_length (list) == 2);
  n3 = snippets_linked_list_append (list, (void *) string_c);
  fail_unless (n3 != NULL);
  fail_unless (snippets_linked_list_length (list) == 3);
  n4 = snippets_linked_list_append (list, (void *) string_d);
  fail_unless (n4 != NULL);
  fail_unless (snippets_linked_list_length (list) == 4);

  fail_unless (snippets_linked_list_head (list) == n1);
  fail_unless (snippets_linked_list_node_prev (n1) == NULL);
  fail_unless (snippets_linked_list_node_next (n1) == n2);
  fail_unless (snippets_linked_list_node_prev (n2) == n1);
  fail_unless (snippets_linked_list_node_next (n2) == n3);
  fail_unless (snippets_linked_list_node_prev (n3) == n2);
  fail_unless (snippets_linked_list_node_next (n3) == n4);
  fail_unless (snippets_linked_list_node_prev (n4) == n3);
  fail_unless (snippets_linked_list_node_next (n4) == NULL);
  fail_unless (snippets_linked_list_tail (list) == n4);

  str = snippets_linked_list_node_get (n1, char *);
  fail_unless (str != NULL);
  fail_if (*str == string_a);
  fail_unless (strcmp (*str, string_a) == 0);

  str = snippets_linked_list_node_get (n2, char *);
  fail_unless (str != NULL);
  fail_if (*str == string_b);
  fail_unless (strcmp (*str, string_b) == 0);

  str = snippets_linked_list_node_get (n3, char *);
  fail_unless (str != NULL);
  fail_if (*str == string_c);
  fail_unless (strcmp (*str, string_c) == 0);

  str = snippets_linked_list_node_get (n4, char *);
  fail_unless (str != NULL);
  fail_if (*str == string_d);
  fail_unless (strcmp (*str, string_d) == 0);

  l = snippets_linked_list_find (list, string_a, compare_string, NULL);
  fail_unless (l != NULL);
  fail_unless (l != n3);

  l = snippets_linked_list_find (list, "test", compare_string, NULL);
  fail_unless (l == NULL);

  copy = snippets_linked_list_copy (list);
  fail_unless (copy != NULL);
  fail_unless (snippets_linked_list_length (copy) ==
      snippets_linked_list_length (list));

  l = snippets_linked_list_head (copy);
  fail_unless (l != NULL);
  str = snippets_linked_list_node_get (n1, char *);
  fail_unless (str != NULL && *str != NULL);
  str2 = snippets_linked_list_node_get (l, char *);
  fail_unless (str2 != NULL && *str2 != NULL);
  fail_unless (str != str2);
  fail_if (*str2 == *str);
  fail_unless (strcmp (*str, *str2) == 0);

  l = snippets_linked_list_node_next (l);
  fail_unless (l != NULL);
  str = snippets_linked_list_node_get (n2, char *);
  fail_unless (str != NULL && *str != NULL);
  str2 = snippets_linked_list_node_get (l, char *);
  fail_unless (str2 != NULL && *str2 != NULL);
  fail_unless (str != str2);
  fail_if (*str2 == *str);
  fail_unless (strcmp (*str, *str2) == 0);

  l = snippets_linked_list_node_next (l);
  fail_unless (l != NULL);
  str = snippets_linked_list_node_get (n3, char *);
  fail_unless (str != NULL && *str != NULL);
  str2 = snippets_linked_list_node_get (l, char *);
  fail_unless (str2 != NULL && *str2 != NULL);
  fail_unless (str != str2);
  fail_if (*str2 == *str);
  fail_unless (strcmp (*str, *str2) == 0);

  l = snippets_linked_list_node_next (l);
  fail_unless (l != NULL);
  fail_unless (snippets_linked_list_tail (copy) == l);
  str = snippets_linked_list_node_get (n4, char *);
  fail_unless (str != NULL && *str != NULL);
  str2 = snippets_linked_list_node_get (l, char *);
  fail_unless (str2 != NULL && *str2 != NULL);
  fail_unless (str != str2);
  fail_if (*str2 == *str);
  fail_unless (strcmp (*str, *str2) == 0);

  l = snippets_linked_list_node_next (l);
  fail_unless (l == NULL);

  snippets_linked_list_free (list);
  snippets_linked_list_free (copy);
}

END_TEST;

START_TEST (test_string_nocopy)
{
  SnippetsLinkedList *list, *copy;
  SnippetsLinkedListNode *n1, *n2, *n3, *n4, *l;
  char **str, **str2;

  list = snippets_linked_list_new_pointer (NULL, NULL);

  fail_unless (snippets_linked_list_length (list) == 0);
  n1 = snippets_linked_list_append (list, (void *) string_a);
  fail_unless (n1 != NULL);
  fail_unless (snippets_linked_list_length (list) == 1);
  n2 = snippets_linked_list_append (list, (void *) string_b);
  fail_unless (n2 != NULL);
  fail_unless (snippets_linked_list_length (list) == 2);
  n3 = snippets_linked_list_append (list, (void *) string_c);
  fail_unless (n3 != NULL);
  fail_unless (snippets_linked_list_length (list) == 3);
  n4 = snippets_linked_list_append (list, (void *) string_d);
  fail_unless (n4 != NULL);
  fail_unless (snippets_linked_list_length (list) == 4);

  fail_unless (snippets_linked_list_head (list) == n1);
  fail_unless (snippets_linked_list_node_prev (n1) == NULL);
  fail_unless (snippets_linked_list_node_next (n1) == n2);
  fail_unless (snippets_linked_list_node_prev (n2) == n1);
  fail_unless (snippets_linked_list_node_next (n2) == n3);
  fail_unless (snippets_linked_list_node_prev (n3) == n2);
  fail_unless (snippets_linked_list_node_next (n3) == n4);
  fail_unless (snippets_linked_list_node_prev (n4) == n3);
  fail_unless (snippets_linked_list_node_next (n4) == NULL);
  fail_unless (snippets_linked_list_tail (list) == n4);

  str = snippets_linked_list_node_get (n1, char *);
  fail_unless (str != NULL);
  fail_unless (*str == string_a);
  fail_unless (strcmp (*str, string_a) == 0);

  str = snippets_linked_list_node_get (n2, char *);
  fail_unless (str != NULL);
  fail_unless (*str == string_b);
  fail_unless (strcmp (*str, string_b) == 0);

  str = snippets_linked_list_node_get (n3, char *);
  fail_unless (str != NULL);
  fail_unless (*str == string_c);
  fail_unless (strcmp (*str, string_c) == 0);

  str = snippets_linked_list_node_get (n4, char *);
  fail_unless (str != NULL);
  fail_unless (*str == string_d);
  fail_unless (strcmp (*str, string_d) == 0);

  l = snippets_linked_list_find (list, string_a, compare_string, NULL);
  fail_unless (l != NULL);
  fail_unless (l != n3);

  l = snippets_linked_list_find (list, "test", compare_string, NULL);
  fail_unless (l == NULL);

  copy = snippets_linked_list_copy (list);
  fail_unless (copy != NULL);
  fail_unless (snippets_linked_list_length (copy) ==
      snippets_linked_list_length (list));

  l = snippets_linked_list_head (copy);
  fail_unless (l != NULL);
  str = snippets_linked_list_node_get (n1, char *);
  fail_unless (str != NULL && *str != NULL);
  str2 = snippets_linked_list_node_get (l, char *);
  fail_unless (str2 != NULL && *str2 != NULL);
  fail_unless (str != str2);
  fail_unless (*str2 == *str);

  l = snippets_linked_list_node_next (l);
  fail_unless (l != NULL);
  str = snippets_linked_list_node_get (n2, char *);
  fail_unless (str != NULL && *str != NULL);
  str2 = snippets_linked_list_node_get (l, char *);
  fail_unless (str2 != NULL && *str2 != NULL);
  fail_unless (str != str2);
  fail_unless (*str2 == *str);

  l = snippets_linked_list_node_next (l);
  fail_unless (l != NULL);
  str = snippets_linked_list_node_get (n3, char *);
  fail_unless (str != NULL && *str != NULL);
  str2 = snippets_linked_list_node_get (l, char *);
  fail_unless (str2 != NULL && *str2 != NULL);
  fail_unless (str != str2);
  fail_unless (*str2 == *str);

  l = snippets_linked_list_node_next (l);
  fail_unless (l != NULL);
  fail_unless (snippets_linked_list_tail (copy) == l);
  str = snippets_linked_list_node_get (n4, char *);
  fail_unless (str != NULL && *str != NULL);
  str2 = snippets_linked_list_node_get (l, char *);
  fail_unless (str2 != NULL && *str2 != NULL);
  fail_unless (str != str2);
  fail_unless (*str2 == *str);

  l = snippets_linked_list_node_next (l);
  fail_unless (l == NULL);

  snippets_linked_list_free (list);
  snippets_linked_list_free (copy);
}

END_TEST;

typedef struct _TestData TestData;
struct _TestData
{
  int x, y;
  char *str;
};

static void
copy_struct (void *dest, const void *src)
{
  TestData *d = dest;
  const TestData *s = src;

  d->x = s->x;
  d->y = s->y;
  d->str = strdup (s->str);
}

static void
free_struct (void *o)
{
  TestData *d = o;

  free (d->str);
}

START_TEST (test_struct_deep_copy)
{
  SnippetsLinkedList *list, *copy;
  SnippetsLinkedListNode *n1, *n2, *n3, *n4, *l;
  TestData d1 = { 1, 4, (char *) "1:4" };
  TestData d2 = { 2, 3, (char *) "2:3" };
  TestData d3 = { 3, 2, (char *) "3:2" };
  TestData d4 = { 4, 1, (char *) "4:1" };
  TestData *d, *dd;

  list = snippets_linked_list_new (sizeof (TestData), copy_struct, free_struct);

  fail_unless (snippets_linked_list_length (list) == 0);
  n1 = snippets_linked_list_append (list, &d1);
  fail_unless (n1 != NULL);
  fail_unless (snippets_linked_list_length (list) == 1);
  n2 = snippets_linked_list_append (list, &d2);
  fail_unless (n2 != NULL);
  fail_unless (snippets_linked_list_length (list) == 2);
  n3 = snippets_linked_list_append (list, &d3);
  fail_unless (n3 != NULL);
  fail_unless (snippets_linked_list_length (list) == 3);
  n4 = snippets_linked_list_append (list, &d4);
  fail_unless (n4 != NULL);
  fail_unless (snippets_linked_list_length (list) == 4);

  fail_unless (snippets_linked_list_head (list) == n1);
  fail_unless (snippets_linked_list_node_prev (n1) == NULL);
  fail_unless (snippets_linked_list_node_next (n1) == n2);
  fail_unless (snippets_linked_list_node_prev (n2) == n1);
  fail_unless (snippets_linked_list_node_next (n2) == n3);
  fail_unless (snippets_linked_list_node_prev (n3) == n2);
  fail_unless (snippets_linked_list_node_next (n3) == n4);
  fail_unless (snippets_linked_list_node_prev (n4) == n3);
  fail_unless (snippets_linked_list_node_next (n4) == NULL);
  fail_unless (snippets_linked_list_tail (list) == n4);

  d = snippets_linked_list_node_get (n1, TestData);
  fail_unless (d != NULL);
  fail_if (d == &d1);
  fail_unless (d->x == d1.x);
  fail_unless (d->y == d1.y);
  fail_if (d->str == d1.str);
  fail_unless (strcmp (d->str, d1.str) == 0);

  d = snippets_linked_list_node_get (n2, TestData);
  fail_unless (d != NULL);
  fail_if (d == &d2);
  fail_unless (d->x == d2.x);
  fail_unless (d->y == d2.y);
  fail_if (d->str == d2.str);
  fail_unless (strcmp (d->str, d2.str) == 0);

  d = snippets_linked_list_node_get (n3, TestData);
  fail_unless (d != NULL);
  fail_if (d == &d3);
  fail_unless (d->x == d3.x);
  fail_unless (d->y == d3.y);
  fail_if (d->str == d3.str);
  fail_unless (strcmp (d->str, d3.str) == 0);

  d = snippets_linked_list_node_get (n4, TestData);
  fail_unless (d != NULL);
  fail_if (d == &d4);
  fail_unless (d->x == d4.x);
  fail_unless (d->y == d4.y);
  fail_if (d->str == d4.str);
  fail_unless (strcmp (d->str, d4.str) == 0);

  copy = snippets_linked_list_copy (list);
  fail_unless (copy != NULL);
  fail_unless (snippets_linked_list_length (copy) ==
      snippets_linked_list_length (list));

  l = snippets_linked_list_head (copy);
  fail_unless (l != NULL);
  d = snippets_linked_list_node_get (n1, TestData);
  dd = snippets_linked_list_node_get (l, TestData);
  fail_unless (d != dd);
  fail_unless (d->x == dd->x);
  fail_unless (d->y == dd->y);
  fail_if (d->str == dd->str);
  fail_unless (strcmp (d->str, dd->str) == 0);

  l = snippets_linked_list_node_next (l);
  fail_unless (l != NULL);
  d = snippets_linked_list_node_get (n2, TestData);
  dd = snippets_linked_list_node_get (l, TestData);
  fail_unless (d != dd);
  fail_unless (d->x == dd->x);
  fail_unless (d->y == dd->y);
  fail_if (d->str == dd->str);
  fail_unless (strcmp (d->str, dd->str) == 0);

  l = snippets_linked_list_node_next (l);
  fail_unless (l != NULL);
  d = snippets_linked_list_node_get (n3, TestData);
  dd = snippets_linked_list_node_get (l, TestData);
  fail_unless (d != dd);
  fail_unless (d->x == dd->x);
  fail_unless (d->y == dd->y);
  fail_if (d->str == dd->str);
  fail_unless (strcmp (d->str, dd->str) == 0);

  l = snippets_linked_list_node_next (l);
  fail_unless (l != NULL);
  fail_unless (snippets_linked_list_tail (copy) == l);
  d = snippets_linked_list_node_get (n4, TestData);
  dd = snippets_linked_list_node_get (l, TestData);
  fail_unless (d != dd);
  fail_unless (d->x == dd->x);
  fail_unless (d->y == dd->y);
  fail_if (d->str == dd->str);
  fail_unless (strcmp (d->str, dd->str) == 0);

  l = snippets_linked_list_node_next (l);
  fail_unless (l == NULL);

  snippets_linked_list_free (list);
  snippets_linked_list_free (copy);
}

END_TEST;

START_TEST (test_struct)
{
  SnippetsLinkedList *list, *copy;
  SnippetsLinkedListNode *n1, *n2, *n3, *n4, *l;
  TestData d1 = { 1, 4, (char *) "1:4" };
  TestData d2 = { 2, 3, (char *) "2:3" };
  TestData d3 = { 3, 2, (char *) "3:2" };
  TestData d4 = { 4, 1, (char *) "4:1" };
  TestData *d, *dd;

  list = snippets_linked_list_new (sizeof (TestData), NULL, NULL);

  fail_unless (snippets_linked_list_length (list) == 0);
  n1 = snippets_linked_list_append (list, &d1);
  fail_unless (n1 != NULL);
  fail_unless (snippets_linked_list_length (list) == 1);
  n2 = snippets_linked_list_append (list, &d2);
  fail_unless (n2 != NULL);
  fail_unless (snippets_linked_list_length (list) == 2);
  n3 = snippets_linked_list_append (list, &d3);
  fail_unless (n3 != NULL);
  fail_unless (snippets_linked_list_length (list) == 3);
  n4 = snippets_linked_list_append (list, &d4);
  fail_unless (n4 != NULL);
  fail_unless (snippets_linked_list_length (list) == 4);

  fail_unless (snippets_linked_list_head (list) == n1);
  fail_unless (snippets_linked_list_node_prev (n1) == NULL);
  fail_unless (snippets_linked_list_node_next (n1) == n2);
  fail_unless (snippets_linked_list_node_prev (n2) == n1);
  fail_unless (snippets_linked_list_node_next (n2) == n3);
  fail_unless (snippets_linked_list_node_prev (n3) == n2);
  fail_unless (snippets_linked_list_node_next (n3) == n4);
  fail_unless (snippets_linked_list_node_prev (n4) == n3);
  fail_unless (snippets_linked_list_node_next (n4) == NULL);
  fail_unless (snippets_linked_list_tail (list) == n4);

  d = snippets_linked_list_node_get (n1, TestData);
  fail_unless (d != NULL);
  fail_unless (memcmp (d, &d1, sizeof (TestData)) == 0);

  d = snippets_linked_list_node_get (n2, TestData);
  fail_unless (d != NULL);
  fail_unless (memcmp (d, &d2, sizeof (TestData)) == 0);

  d = snippets_linked_list_node_get (n3, TestData);
  fail_unless (d != NULL);
  fail_unless (memcmp (d, &d3, sizeof (TestData)) == 0);

  d = snippets_linked_list_node_get (n4, TestData);
  fail_unless (d != NULL);
  fail_unless (memcmp (d, &d4, sizeof (TestData)) == 0);

  copy = snippets_linked_list_copy (list);
  fail_unless (copy != NULL);
  fail_unless (snippets_linked_list_length (copy) ==
      snippets_linked_list_length (list));

  l = snippets_linked_list_head (copy);
  fail_unless (l != NULL);
  d = snippets_linked_list_node_get (n1, TestData);
  dd = snippets_linked_list_node_get (l, TestData);
  fail_unless (d != dd);
  fail_unless (d->x == dd->x);
  fail_unless (d->y == dd->y);
  fail_unless (d->str == dd->str);

  l = snippets_linked_list_node_next (l);
  fail_unless (l != NULL);
  d = snippets_linked_list_node_get (n2, TestData);
  dd = snippets_linked_list_node_get (l, TestData);
  fail_unless (d != dd);
  fail_unless (d->x == dd->x);
  fail_unless (d->y == dd->y);
  fail_unless (d->str == dd->str);

  l = snippets_linked_list_node_next (l);
  fail_unless (l != NULL);
  d = snippets_linked_list_node_get (n3, TestData);
  dd = snippets_linked_list_node_get (l, TestData);
  fail_unless (d != dd);
  fail_unless (d->x == dd->x);
  fail_unless (d->y == dd->y);
  fail_unless (d->str == dd->str);

  l = snippets_linked_list_node_next (l);
  fail_unless (l != NULL);
  fail_unless (snippets_linked_list_tail (copy) == l);
  d = snippets_linked_list_node_get (n4, TestData);
  dd = snippets_linked_list_node_get (l, TestData);
  fail_unless (d != dd);
  fail_unless (d->x == dd->x);
  fail_unless (d->y == dd->y);
  fail_unless (d->str == dd->str);

  l = snippets_linked_list_node_next (l);
  fail_unless (l == NULL);

  snippets_linked_list_free (list);
  snippets_linked_list_free (copy);
}

END_TEST;

START_TEST (test_append_prepend_remove)
{
  SnippetsLinkedList *list;
  SnippetsLinkedListNode *n1, *n2, *n3, *n4;
  void **p;

  list = snippets_linked_list_new_pointer (NULL, NULL);

  fail_unless (snippets_linked_list_length (list) == 0);
  n1 = snippets_linked_list_append (list, (void *) 0);
  fail_unless (n1 != NULL);
  fail_unless (snippets_linked_list_length (list) == 1);

  n2 = snippets_linked_list_prepend (list, (void *) 1);
  fail_unless (n2 != NULL);
  fail_unless (snippets_linked_list_length (list) == 2);

  p = snippets_linked_list_node_get (snippets_linked_list_head (list), void *);
  fail_unless (*p == (void *) 1);

  p = snippets_linked_list_node_get (snippets_linked_list_tail (list), void *);
  fail_unless (*p == (void *) 0);

  fail_unless (snippets_linked_list_node_prev (n1) == n2);
  fail_unless (snippets_linked_list_node_next (n1) == NULL);
  fail_unless (snippets_linked_list_node_prev (n2) == NULL);
  fail_unless (snippets_linked_list_node_next (n2) == n1);

  fail_unless (snippets_linked_list_length (list) == 2);
  n3 = snippets_linked_list_insert_after (list, n2, (void *) 3);
  fail_unless (n3 != NULL);
  fail_unless (snippets_linked_list_length (list) == 3);

  fail_unless (snippets_linked_list_node_prev (n1) == n3);
  fail_unless (snippets_linked_list_node_next (n1) == NULL);
  fail_unless (snippets_linked_list_node_prev (n2) == NULL);
  fail_unless (snippets_linked_list_node_next (n2) == n3);
  fail_unless (snippets_linked_list_node_prev (n3) == n2);
  fail_unless (snippets_linked_list_node_next (n3) == n1);

  fail_unless (snippets_linked_list_length (list) == 3);
  n4 = snippets_linked_list_insert_before (list, n3, (void *) 4);
  fail_unless (n4 != NULL);
  fail_unless (snippets_linked_list_length (list) == 4);

  fail_unless (snippets_linked_list_node_prev (n1) == n3);
  fail_unless (snippets_linked_list_node_next (n1) == NULL);
  fail_unless (snippets_linked_list_node_prev (n2) == NULL);
  fail_unless (snippets_linked_list_node_next (n2) == n4);
  fail_unless (snippets_linked_list_node_prev (n3) == n4);
  fail_unless (snippets_linked_list_node_next (n3) == n1);
  fail_unless (snippets_linked_list_node_prev (n4) == n2);
  fail_unless (snippets_linked_list_node_next (n4) == n3);

  fail_unless (snippets_linked_list_length (list) == 4);
  snippets_linked_list_remove (list, n4);
  fail_unless (snippets_linked_list_length (list) == 3);

  fail_unless (snippets_linked_list_node_prev (n1) == n3);
  fail_unless (snippets_linked_list_node_next (n1) == NULL);
  fail_unless (snippets_linked_list_node_prev (n2) == NULL);
  fail_unless (snippets_linked_list_node_next (n2) == n3);
  fail_unless (snippets_linked_list_node_prev (n3) == n2);
  fail_unless (snippets_linked_list_node_next (n3) == n1);

  fail_unless (snippets_linked_list_length (list) == 3);
  snippets_linked_list_remove (list, n1);
  fail_unless (snippets_linked_list_length (list) == 2);

  fail_unless (snippets_linked_list_node_prev (n2) == NULL);
  fail_unless (snippets_linked_list_node_next (n2) == n3);
  fail_unless (snippets_linked_list_node_prev (n3) == n2);
  fail_unless (snippets_linked_list_node_next (n3) == NULL);

  fail_unless (snippets_linked_list_length (list) == 2);
  snippets_linked_list_remove (list, n2);
  fail_unless (snippets_linked_list_length (list) == 1);

  fail_unless (snippets_linked_list_node_prev (n3) == NULL);
  fail_unless (snippets_linked_list_node_next (n3) == NULL);
  fail_unless (snippets_linked_list_head (list) == n3);
  fail_unless (snippets_linked_list_tail (list) == n3);

  fail_unless (snippets_linked_list_length (list) == 1);
  snippets_linked_list_remove (list, n3);
  fail_unless (snippets_linked_list_length (list) == 0);

  fail_unless (snippets_linked_list_head (list) == NULL);
  fail_unless (snippets_linked_list_tail (list) == NULL);

  fail_unless (snippets_linked_list_length (list) == 0);

  snippets_linked_list_free (list);
}

END_TEST;

static Suite *
linkedlist_suite (void)
{
  Suite *s = suite_create ("LinkedList");

  /* Core test case */
  TCase *tc_general = tcase_create ("general");
  tcase_add_test (tc_general, test_string_copy);
  tcase_add_test (tc_general, test_string_nocopy);
  tcase_add_test (tc_general, test_struct_deep_copy);
  tcase_add_test (tc_general, test_struct);
  tcase_add_test (tc_general, test_append_prepend_remove);
  suite_add_tcase (s, tc_general);

  return s;
}

int
main (void)
{
  int number_failed;
  Suite *s = linkedlist_suite ();
  SRunner *sr = srunner_create (s);
  srunner_run_all (sr, CK_NORMAL);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
