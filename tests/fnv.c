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

START_TEST (test_1_256)
{
  const char *str = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  uint8_t hash[32];

  fnv1_256 ((const uint8_t *) str, strlen (str), hash);
  fail_unless (hash[0] == 0x73);
  fail_unless (hash[1] == 0x5a);
  fail_unless (hash[2] == 0x7c);
  fail_unless (hash[3] == 0x96);
  fail_unless (hash[4] == 0x69);
  fail_unless (hash[5] == 0x33);
  fail_unless (hash[6] == 0x79);
  fail_unless (hash[7] == 0x25);
  fail_unless (hash[8] == 0xdd);
  fail_unless (hash[9] == 0x4a);
  fail_unless (hash[10] == 0xa3);
  fail_unless (hash[11] == 0xbc);
  fail_unless (hash[12] == 0x1f);
  fail_unless (hash[13] == 0xae);
  fail_unless (hash[14] == 0xde);
  fail_unless (hash[15] == 0xc3);
  fail_unless (hash[16] == 0x3d);
  fail_unless (hash[17] == 0x6b);
  fail_unless (hash[18] == 0x2a);
  fail_unless (hash[19] == 0xe9);
  fail_unless (hash[20] == 0xb0);
  fail_unless (hash[21] == 0x28);
  fail_unless (hash[22] == 0x71);
  fail_unless (hash[23] == 0x6d);
  fail_unless (hash[24] == 0xe7);
  fail_unless (hash[25] == 0x65);
  fail_unless (hash[26] == 0x2a);
  fail_unless (hash[27] == 0x29);
  fail_unless (hash[28] == 0xbf);
  fail_unless (hash[29] == 0xb6);
  fail_unless (hash[30] == 0x99);
  fail_unless (hash[31] == 0x19);
}

END_TEST;

START_TEST (test_1a_256)
{
  const char *str = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  uint8_t hash[32];

  fnv1a_256 ((const uint8_t *) str, strlen (str), hash);
  fail_unless (hash[0] == 0xc2);
  fail_unless (hash[1] == 0x5f);
  fail_unless (hash[2] == 0xb3);
  fail_unless (hash[3] == 0xfd);
  fail_unless (hash[4] == 0x87);
  fail_unless (hash[5] == 0xc6);
  fail_unless (hash[6] == 0x79);
  fail_unless (hash[7] == 0xaf);
  fail_unless (hash[8] == 0x4b);
  fail_unless (hash[9] == 0x26);
  fail_unless (hash[10] == 0x16);
  fail_unless (hash[11] == 0x29);
  fail_unless (hash[12] == 0x02);
  fail_unless (hash[13] == 0xc6);
  fail_unless (hash[14] == 0xef);
  fail_unless (hash[15] == 0x4f);
  fail_unless (hash[16] == 0x3d);
  fail_unless (hash[17] == 0x23);
  fail_unless (hash[18] == 0x65);
  fail_unless (hash[19] == 0x92);
  fail_unless (hash[20] == 0x57);
  fail_unless (hash[21] == 0x71);
  fail_unless (hash[22] == 0xab);
  fail_unless (hash[23] == 0x30);
  fail_unless (hash[24] == 0xe3);
  fail_unless (hash[25] == 0x73);
  fail_unless (hash[26] == 0x9f);
  fail_unless (hash[27] == 0x47);
  fail_unless (hash[28] == 0x42);
  fail_unless (hash[29] == 0x53);
  fail_unless (hash[30] == 0x9b);
  fail_unless (hash[31] == 0x2d);
}

END_TEST;

START_TEST (test_1_512)
{
  const char *str = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  uint8_t hash[64];

  fnv1_512 ((const uint8_t *) str, strlen (str), hash);
  fail_unless (hash[0] == 0x31);
  fail_unless (hash[1] == 0xd6);
  fail_unless (hash[2] == 0x3b);
  fail_unless (hash[3] == 0x43);
  fail_unless (hash[4] == 0x3c);
  fail_unless (hash[5] == 0x2e);
  fail_unless (hash[6] == 0x8d);
  fail_unless (hash[7] == 0xb9);
  fail_unless (hash[8] == 0x43);
  fail_unless (hash[9] == 0x7b);
  fail_unless (hash[10] == 0xb7);
  fail_unless (hash[11] == 0x96);
  fail_unless (hash[12] == 0x95);
  fail_unless (hash[13] == 0xd1);
  fail_unless (hash[14] == 0x95);
  fail_unless (hash[15] == 0xb0);
  fail_unless (hash[16] == 0xc9);
  fail_unless (hash[17] == 0x91);
  fail_unless (hash[18] == 0x38);
  fail_unless (hash[19] == 0xaf);
  fail_unless (hash[20] == 0xe4);
  fail_unless (hash[21] == 0x43);
  fail_unless (hash[22] == 0xa1);
  fail_unless (hash[23] == 0x8e);
  fail_unless (hash[24] == 0xbc);
  fail_unless (hash[25] == 0x7b);
  fail_unless (hash[26] == 0xf3);
  fail_unless (hash[27] == 0x79);
  fail_unless (hash[28] == 0xee);
  fail_unless (hash[29] == 0x8f);
  fail_unless (hash[30] == 0xf4);
  fail_unless (hash[31] == 0xe6);
  fail_unless (hash[32] == 0xbb);
  fail_unless (hash[33] == 0xe8);
  fail_unless (hash[34] == 0xbf);
  fail_unless (hash[35] == 0xcf);
  fail_unless (hash[36] == 0xc9);
  fail_unless (hash[37] == 0x25);
  fail_unless (hash[38] == 0xfa);
  fail_unless (hash[39] == 0xd7);
  fail_unless (hash[40] == 0x5f);
  fail_unless (hash[41] == 0x3d);
  fail_unless (hash[42] == 0x2b);
  fail_unless (hash[43] == 0xff);
  fail_unless (hash[44] == 0x43);
  fail_unless (hash[45] == 0x28);
  fail_unless (hash[46] == 0xcb);
  fail_unless (hash[47] == 0x72);
  fail_unless (hash[48] == 0x97);
  fail_unless (hash[49] == 0xba);
  fail_unless (hash[50] == 0xdc);
  fail_unless (hash[51] == 0x4f);
  fail_unless (hash[52] == 0x3b);
  fail_unless (hash[53] == 0x4a);
  fail_unless (hash[54] == 0x60);
  fail_unless (hash[55] == 0xe3);
  fail_unless (hash[56] == 0x83);
  fail_unless (hash[57] == 0xdb);
  fail_unless (hash[58] == 0x49);
  fail_unless (hash[59] == 0xfe);
  fail_unless (hash[60] == 0xab);
  fail_unless (hash[61] == 0xc5);
  fail_unless (hash[62] == 0x6c);
  fail_unless (hash[63] == 0x51);
}

END_TEST;

START_TEST (test_1a_512)
{
  const char *str = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  uint8_t hash[64];

  fnv1a_512 ((const uint8_t *) str, strlen (str), hash);
  fail_unless (hash[0] == 0x13);
  fail_unless (hash[1] == 0x92);
  fail_unless (hash[2] == 0x20);
  fail_unless (hash[3] == 0xfc);
  fail_unless (hash[4] == 0x4c);
  fail_unless (hash[5] == 0xd6);
  fail_unless (hash[6] == 0x48);
  fail_unless (hash[7] == 0x3d);
  fail_unless (hash[8] == 0xd3);
  fail_unless (hash[9] == 0x15);
  fail_unless (hash[10] == 0x39);
  fail_unless (hash[11] == 0x42);
  fail_unless (hash[12] == 0xb7);
  fail_unless (hash[13] == 0x1a);
  fail_unless (hash[14] == 0x92);
  fail_unless (hash[15] == 0x0d);
  fail_unless (hash[16] == 0xb5);
  fail_unless (hash[17] == 0x63);
  fail_unless (hash[18] == 0xe3);
  fail_unless (hash[19] == 0x0e);
  fail_unless (hash[20] == 0xca);
  fail_unless (hash[21] == 0x43);
  fail_unless (hash[22] == 0xa1);
  fail_unless (hash[23] == 0x8e);
  fail_unless (hash[24] == 0xbc);
  fail_unless (hash[25] == 0x56);
  fail_unless (hash[26] == 0xc5);
  fail_unless (hash[27] == 0x39);
  fail_unless (hash[28] == 0x75);
  fail_unless (hash[29] == 0x3c);
  fail_unless (hash[30] == 0x7d);
  fail_unless (hash[31] == 0x71);
  fail_unless (hash[32] == 0x1d);
  fail_unless (hash[33] == 0x74);
  fail_unless (hash[34] == 0xd7);
  fail_unless (hash[35] == 0x6e);
  fail_unless (hash[36] == 0xd2);
  fail_unless (hash[37] == 0x15);
  fail_unless (hash[38] == 0x9c);
  fail_unless (hash[39] == 0x76);
  fail_unless (hash[40] == 0xd1);
  fail_unless (hash[41] == 0x8f);
  fail_unless (hash[42] == 0x91);
  fail_unless (hash[43] == 0x30);
  fail_unless (hash[44] == 0xf0);
  fail_unless (hash[45] == 0xf3);
  fail_unless (hash[46] == 0x76);
  fail_unless (hash[47] == 0x18);
  fail_unless (hash[48] == 0xf1);
  fail_unless (hash[49] == 0x29);
  fail_unless (hash[50] == 0xb6);
  fail_unless (hash[51] == 0x94);
  fail_unless (hash[52] == 0x5b);
  fail_unless (hash[53] == 0x9f);
  fail_unless (hash[54] == 0x82);
  fail_unless (hash[55] == 0xef);
  fail_unless (hash[56] == 0x26);
  fail_unless (hash[57] == 0xba);
  fail_unless (hash[58] == 0xcd);
  fail_unless (hash[59] == 0xf6);
  fail_unless (hash[60] == 0x66);
  fail_unless (hash[61] == 0xaa);
  fail_unless (hash[62] == 0xc2);
  fail_unless (hash[63] == 0x1d);
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
  tcase_add_test (tc_general, test_1_256);
  tcase_add_test (tc_general, test_1a_256);
  tcase_add_test (tc_general, test_1_512);
  tcase_add_test (tc_general, test_1a_512);
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
