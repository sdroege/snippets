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

#include "fnv.h"

START_TEST (test_1_32)
{
  const char *str = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  uint8_t hash[4];

  fnv1_32 ((const uint8_t *) str, strlen (str), hash);
  fail_unless (hash[0] == 0x94);
  fail_unless (hash[1] == 0x41);
  fail_unless (hash[2] == 0x79);
  fail_unless (hash[3] == 0xf9);
}
END_TEST;

START_TEST (test_1a_32)
{
  const char *str = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  uint8_t hash[4];

  fnv1a_32 ((const uint8_t *) str, strlen (str), hash);
  fail_unless (hash[0] == 0x3e);
  fail_unless (hash[1] == 0xda);
  fail_unless (hash[2] == 0x61);
  fail_unless (hash[3] == 0xcd);
}
END_TEST;

START_TEST (test_1_64)
{
  const char *str = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  uint8_t hash[8];

  fnv1_64 ((const uint8_t *) str, strlen (str), hash);
  fail_unless (hash[0] == 0x94);
  fail_unless (hash[1] == 0xd9);
  fail_unless (hash[2] == 0x0d);
  fail_unless (hash[3] == 0x92);
  fail_unless (hash[4] == 0x6c);
  fail_unless (hash[5] == 0x9d);
  fail_unless (hash[6] == 0xe7);
  fail_unless (hash[7] == 0x79);
}
END_TEST;

START_TEST (test_1a_64)
{
  const char *str = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  uint8_t hash[8];

  fnv1a_64 ((const uint8_t *) str, strlen (str), hash);
  fail_unless (hash[0] == 0x37);
  fail_unless (hash[1] == 0x22);
  fail_unless (hash[2] == 0xd9);
  fail_unless (hash[3] == 0x2f);
  fail_unless (hash[4] == 0x94);
  fail_unless (hash[5] == 0xba);
  fail_unless (hash[6] == 0x1e);
  fail_unless (hash[7] == 0x8d);
}
END_TEST;

START_TEST (test_1_128)
{
  const char *str = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  uint8_t hash[16];

  fnv1_128 ((const uint8_t *) str, strlen (str), hash);
  fail_unless (hash[0] == 0xa5);
  fail_unless (hash[1] == 0xfe);
  fail_unless (hash[2] == 0xd2);
  fail_unless (hash[3] == 0x7e);
  fail_unless (hash[4] == 0x13);
  fail_unless (hash[5] == 0x09);
  fail_unless (hash[6] == 0x0b);
  fail_unless (hash[7] == 0x4e);
  fail_unless (hash[8] == 0x6f);
  fail_unless (hash[9] == 0x62);
  fail_unless (hash[10] == 0x3a);
  fail_unless (hash[11] == 0xd0);
  fail_unless (hash[12] == 0x29);
  fail_unless (hash[13] == 0xaf);
  fail_unless (hash[14] == 0x7d);
  fail_unless (hash[15] == 0xd9);
}
END_TEST;

START_TEST (test_1a_128)
{
  const char *str = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  uint8_t hash[16];

  fnv1a_128 ((const uint8_t *) str, strlen (str), hash);
  fail_unless (hash[0] == 0x6f);
  fail_unless (hash[1] == 0x36);
  fail_unless (hash[2] == 0x56);
  fail_unless (hash[3] == 0xc6);
  fail_unless (hash[4] == 0x0d);
  fail_unless (hash[5] == 0xf8);
  fail_unless (hash[6] == 0x43);
  fail_unless (hash[7] == 0x48);
  fail_unless (hash[8] == 0x2c);
  fail_unless (hash[9] == 0xaf);
  fail_unless (hash[10] == 0xae);
  fail_unless (hash[11] == 0x3d);
  fail_unless (hash[12] == 0x95);
  fail_unless (hash[13] == 0x63);
  fail_unless (hash[14] == 0x4d);
  fail_unless (hash[15] == 0xad);
}
END_TEST;

static Suite *
fnv_suite (void)
{
  Suite *s = suite_create ("FNV");

  /* Core test case */
  TCase *tc_general = tcase_create ("general");
  tcase_add_test (tc_general, test_1_32);
  tcase_add_test (tc_general, test_1a_32);
  tcase_add_test (tc_general, test_1_64);
  tcase_add_test (tc_general, test_1a_64);
  tcase_add_test (tc_general, test_1_128);
  tcase_add_test (tc_general, test_1a_128);
  suite_add_tcase (s, tc_general);

  return s;
}

int
main (void)
{
  int number_failed;
  Suite *s = fnv_suite ();
  SRunner *sr = srunner_create (s);
  srunner_run_all (sr, CK_NORMAL);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

