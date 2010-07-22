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
#include <snippets/rand.h>

START_TEST (test_rand_mt_uint32)
{
  SnippetsRand *rand = snippets_rand_new (0xdeadbeef);
  uint32_t vals[2048];
  unsigned int i;

  for (i = 0; i < 2048; i++)
    vals[i] = snippets_rand_uint32 (rand);

  /* Not testing much here... */

  snippets_rand_free (rand);
}

END_TEST;

START_TEST (test_rand_mt_double)
{
  SnippetsRand *rand = snippets_rand_new (0xdeadbeef);
  double vals[2048];
  unsigned int i;

  for (i = 0; i < 2048; i++) {
    vals[i] = snippets_rand_double (rand);
    fail_unless (vals[i] < 1.0 && vals[i] >= 0.0);
  }

  /* Not testing much here... */

  snippets_rand_free (rand);
}

END_TEST;

START_TEST (test_rand_mt_uint32_range_20_100)
{
  SnippetsRand *rand = snippets_rand_new (0xdeadbeef);
  uint32_t vals[2048];
  unsigned int i;

  for (i = 0; i < 2048; i++) {
    vals[i] = snippets_rand_uint32_range (rand, 20, 100);
    fail_unless (vals[i] >= 20 && vals[i] < 100);
  }

  /* Not testing much here... */

  snippets_rand_free (rand);
}

END_TEST;

START_TEST (test_rand_mt_double_range_20_100)
{
  SnippetsRand *rand = snippets_rand_new (0xdeadbeef);
  double vals[2048];
  unsigned int i;

  for (i = 0; i < 2048; i++) {
    vals[i] = snippets_rand_double_range (rand, 20, 100);
    fail_unless (vals[i] >= 20 && vals[i] < 100);
  }

  /* Not testing much here... */

  snippets_rand_free (rand);
}

END_TEST;

static Suite *
rand_suite (void)
{
  Suite *s = suite_create ("Rand");

  /* Core test case */
  TCase *tc_general = tcase_create ("general");
  tcase_add_test (tc_general, test_rand_mt_uint32);
  tcase_add_test (tc_general, test_rand_mt_double);
  tcase_add_test (tc_general, test_rand_mt_uint32_range_20_100);
  tcase_add_test (tc_general, test_rand_mt_double_range_20_100);
  suite_add_tcase (s, tc_general);

  return s;
}

int
main (void)
{
  int number_failed;
  Suite *s = rand_suite ();
  SRunner *sr = srunner_create (s);
  srunner_run_all (sr, CK_NORMAL);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
