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

#include <snippets/fnv.h>

/* Implementations of the Fowler–Noll–Vo hash function.
 * This implements FNV 1 and 1A for various bit depths
 *
 * For more details see:
 * http://www.isthe.com/chongo/tech/comp/fnv/index.html
 */

static const uint32_t FNV_prime_32 = 16777619U;
static const uint32_t FNV_offset_32 = 2166136261U;

void
snippets_fnv1_32 (const uint8_t * data, size_t len, uint8_t hash[4])
{
  size_t i;
  uint32_t tmp = FNV_offset_32;

  for (i = 0; i < len; i++) {
    tmp = tmp * FNV_prime_32;   /* Multiplication mod 2^32 as per C standard */
    tmp = tmp ^ *data;
    data++;
  }

  hash[0] = (tmp >> 24) & 0xff;
  hash[1] = (tmp >> 16) & 0xff;
  hash[2] = (tmp >> 8) & 0xff;
  hash[3] = (tmp >> 0) & 0xff;
}

void
snippets_fnv1a_32 (const uint8_t * data, size_t len, uint8_t hash[4])
{
  size_t i;
  uint32_t tmp = FNV_offset_32;

  for (i = 0; i < len; i++) {
    tmp = tmp ^ *data;
    tmp = tmp * FNV_prime_32;   /* Multiplication mod 2^32 as per C standard */
    data++;
  }

  hash[0] = (tmp >> 24) & 0xff;
  hash[1] = (tmp >> 16) & 0xff;
  hash[2] = (tmp >> 8) & 0xff;
  hash[3] = (tmp >> 0) & 0xff;
}

static const uint64_t FNV_prime_64 = 1099511628211ULL;
static const uint64_t FNV_offset_64 = 14695981039346656037ULL;

void
snippets_fnv1_64 (const uint8_t * data, size_t len, uint8_t hash[8])
{
  size_t i;
  uint64_t tmp = FNV_offset_64;

  for (i = 0; i < len; i++) {
    tmp = tmp * FNV_prime_64;   /* Multiplication mod 2^64 as per C standard */
    tmp = tmp ^ *data;
    data++;
  }

  hash[0] = (tmp >> 56) & 0xff;
  hash[1] = (tmp >> 48) & 0xff;
  hash[2] = (tmp >> 40) & 0xff;
  hash[3] = (tmp >> 32) & 0xff;
  hash[4] = (tmp >> 24) & 0xff;
  hash[5] = (tmp >> 16) & 0xff;
  hash[6] = (tmp >> 8) & 0xff;
  hash[7] = (tmp >> 0) & 0xff;
}

void
snippets_fnv1a_64 (const uint8_t * data, size_t len, uint8_t hash[8])
{
  size_t i;
  uint64_t tmp = FNV_offset_64;

  for (i = 0; i < len; i++) {
    tmp = tmp ^ *data;
    tmp = tmp * FNV_prime_64;   /* Multiplication mod 2^64 as per C standard */
    data++;
  }

  hash[0] = (tmp >> 56) & 0xff;
  hash[1] = (tmp >> 48) & 0xff;
  hash[2] = (tmp >> 40) & 0xff;
  hash[3] = (tmp >> 32) & 0xff;
  hash[4] = (tmp >> 24) & 0xff;
  hash[5] = (tmp >> 16) & 0xff;
  hash[6] = (tmp >> 8) & 0xff;
  hash[7] = (tmp >> 0) & 0xff;
}

/* 128 bit prime  =             309485009821345068724781371 = 0x0000000001000000000000000000013b
 * 128 bit offset = 144066263297769815596495629667062367629 = 0x6c62272e07bb014262b821756295c58d
 */
void
snippets_fnv1_128 (const uint8_t * data, size_t len, uint8_t hash[16])
{
  size_t i;
  uint64_t tmp[4], tmp2[4];

  /* 128 bit number, 32 bit per field, highest 32 bit first */
  tmp[0] = 0x6c62272e;
  tmp[1] = 0x07bb0142;
  tmp[2] = 0x62b82175;
  tmp[3] = 0x6295c58d;

  for (i = 0; i < len; i++) {
    /* Multiplication */

    /* lowest 32 bits */

    /* multiply and keep carries forward */
    tmp2[3] = tmp[3] * 0x0000013b;
    tmp2[2] = tmp[2] * 0x0000013b + (tmp2[3] >> 32);
    tmp2[1] = tmp[1] * 0x0000013b + (tmp2[2] >> 32);
    tmp2[0] = tmp[0] * 0x0000013b + (tmp2[1] >> 32);
    /* drop carries */
    tmp2[3] &= 0xffffffff;
    tmp2[2] &= 0xffffffff;
    tmp2[1] &= 0xffffffff;
    tmp2[0] &= 0xffffffff;

    /* highest 32 bits */

    /* multiply and keep carries forward */
    tmp2[1] += tmp[3] * 0x01000000;
    tmp2[0] += tmp[2] * 0x01000000 + (tmp2[1] >> 32);
    /* drop carries */
    tmp2[1] &= 0xffffffff;
    tmp2[0] &= 0xffffffff;

    tmp[3] = tmp2[3] ^ *data;
    tmp[2] = tmp2[2];
    tmp[1] = tmp2[1];
    tmp[0] = tmp2[0];

    data++;
  }

  hash[0] = (tmp[0] >> 24) & 0xff;
  hash[1] = (tmp[0] >> 16) & 0xff;
  hash[2] = (tmp[0] >> 8) & 0xff;
  hash[3] = (tmp[0] >> 0) & 0xff;
  hash[4] = (tmp[1] >> 24) & 0xff;
  hash[5] = (tmp[1] >> 16) & 0xff;
  hash[6] = (tmp[1] >> 8) & 0xff;
  hash[7] = (tmp[1] >> 0) & 0xff;
  hash[8] = (tmp[2] >> 24) & 0xff;
  hash[9] = (tmp[2] >> 16) & 0xff;
  hash[10] = (tmp[2] >> 8) & 0xff;
  hash[11] = (tmp[2] >> 0) & 0xff;
  hash[12] = (tmp[3] >> 24) & 0xff;
  hash[13] = (tmp[3] >> 16) & 0xff;
  hash[14] = (tmp[3] >> 8) & 0xff;
  hash[15] = (tmp[3] >> 0) & 0xff;
}

void
snippets_fnv1a_128 (const uint8_t * data, size_t len, uint8_t hash[16])
{
  size_t i;
  uint64_t tmp[4], tmp2[4];

  /* 128 bit number, 32 bit per field, highest 32 bit first */
  tmp[0] = 0x6c62272e;
  tmp[1] = 0x07bb0142;
  tmp[2] = 0x62b82175;
  tmp[3] = 0x6295c58d;

  for (i = 0; i < len; i++) {
    /* Multiplication */

    /* lowest 32 bits */

    /* multiply and keep carries forward */
    tmp2[3] = (*data ^ tmp[3]) * 0x0000013b;
    tmp2[2] = tmp[2] * 0x0000013b + (tmp2[3] >> 32);
    tmp2[1] = tmp[1] * 0x0000013b + (tmp2[2] >> 32);
    tmp2[0] = tmp[0] * 0x0000013b + (tmp2[1] >> 32);
    /* drop carries */
    tmp2[3] &= 0xffffffff;
    tmp2[2] &= 0xffffffff;
    tmp2[1] &= 0xffffffff;
    tmp2[0] &= 0xffffffff;

    /* highest 32 bits */

    /* multiply and keep carries forward */
    tmp2[1] += tmp[3] * 0x01000000;
    tmp2[0] += tmp[2] * 0x01000000 + (tmp2[1] >> 32);
    /* drop carries */
    tmp2[1] &= 0xffffffff;
    tmp2[0] &= 0xffffffff;

    tmp[3] = tmp2[3];
    tmp[2] = tmp2[2];
    tmp[1] = tmp2[1];
    tmp[0] = tmp2[0];

    data++;
  }

  hash[0] = (tmp[0] >> 24) & 0xff;
  hash[1] = (tmp[0] >> 16) & 0xff;
  hash[2] = (tmp[0] >> 8) & 0xff;
  hash[3] = (tmp[0] >> 0) & 0xff;
  hash[4] = (tmp[1] >> 24) & 0xff;
  hash[5] = (tmp[1] >> 16) & 0xff;
  hash[6] = (tmp[1] >> 8) & 0xff;
  hash[7] = (tmp[1] >> 0) & 0xff;
  hash[8] = (tmp[2] >> 24) & 0xff;
  hash[9] = (tmp[2] >> 16) & 0xff;
  hash[10] = (tmp[2] >> 8) & 0xff;
  hash[11] = (tmp[2] >> 0) & 0xff;
  hash[12] = (tmp[3] >> 24) & 0xff;
  hash[13] = (tmp[3] >> 16) & 0xff;
  hash[14] = (tmp[3] >> 8) & 0xff;
  hash[15] = (tmp[3] >> 0) & 0xff;
}

/* 256 bit prime  =                             374144419156711147060143317175368453031918731002211
 *                = 0x0000000000000000000001000000000000000000000000000000000000000163
 * 256 bit offset = 100029257958052580907070968620625704837092796014241193945225284501741471925557
 *                = 0xdd268dbcaac550362d98c384c4e576ccc8b1536847b6bbb31023b4c8caee0535
 */
void
snippets_fnv1_256 (const uint8_t * data, size_t len, uint8_t hash[32])
{
  size_t i;
  uint64_t tmp[8], tmp2[8];

  /* 256 bit number, 32 bit per field, highest 32 bit first */
  tmp[0] = 0xdd268dbc;
  tmp[1] = 0xaac55036;
  tmp[2] = 0x2d98c384;
  tmp[3] = 0xc4e576cc;
  tmp[4] = 0xc8b15368;
  tmp[5] = 0x47b6bbb3;
  tmp[6] = 0x1023b4c8;
  tmp[7] = 0xcaee0535;

  for (i = 0; i < len; i++) {
    /* Multiplication */

    /* lowest 32 bits */

    /* multiply and keep carries forward */
    tmp2[7] = tmp[7] * 0x00000163;
    tmp2[6] = tmp[6] * 0x00000163 + (tmp2[7] >> 32);
    tmp2[5] = tmp[5] * 0x00000163 + (tmp2[6] >> 32);
    tmp2[4] = tmp[4] * 0x00000163 + (tmp2[5] >> 32);
    tmp2[3] = tmp[3] * 0x00000163 + (tmp2[4] >> 32);
    tmp2[2] = tmp[2] * 0x00000163 + (tmp2[3] >> 32);
    tmp2[1] = tmp[1] * 0x00000163 + (tmp2[2] >> 32);
    tmp2[0] = tmp[0] * 0x00000163 + (tmp2[1] >> 32);
    /* drop carries */
    tmp2[7] &= 0xffffffff;
    tmp2[6] &= 0xffffffff;
    tmp2[5] &= 0xffffffff;
    tmp2[4] &= 0xffffffff;
    tmp2[3] &= 0xffffffff;
    tmp2[2] &= 0xffffffff;
    tmp2[1] &= 0xffffffff;
    tmp2[0] &= 0xffffffff;

    /* highest 32 bits */

    /* multiply and keep carries forward */
    tmp2[2] += tmp[7] * 0x00000100;
    tmp2[1] += tmp[6] * 0x00000100 + (tmp2[2] >> 32);
    tmp2[0] += tmp[5] * 0x00000100 + (tmp2[1] >> 32);
    /* drop carries */
    tmp2[2] &= 0xffffffff;
    tmp2[1] &= 0xffffffff;
    tmp2[0] &= 0xffffffff;

    tmp[7] = tmp2[7] ^ *data;
    tmp[6] = tmp2[6];
    tmp[5] = tmp2[5];
    tmp[4] = tmp2[4];
    tmp[3] = tmp2[3];
    tmp[2] = tmp2[2];
    tmp[1] = tmp2[1];
    tmp[0] = tmp2[0];

    data++;
  }

  hash[0] = (tmp[0] >> 24) & 0xff;
  hash[1] = (tmp[0] >> 16) & 0xff;
  hash[2] = (tmp[0] >> 8) & 0xff;
  hash[3] = (tmp[0] >> 0) & 0xff;
  hash[4] = (tmp[1] >> 24) & 0xff;
  hash[5] = (tmp[1] >> 16) & 0xff;
  hash[6] = (tmp[1] >> 8) & 0xff;
  hash[7] = (tmp[1] >> 0) & 0xff;
  hash[8] = (tmp[2] >> 24) & 0xff;
  hash[9] = (tmp[2] >> 16) & 0xff;
  hash[10] = (tmp[2] >> 8) & 0xff;
  hash[11] = (tmp[2] >> 0) & 0xff;
  hash[12] = (tmp[3] >> 24) & 0xff;
  hash[13] = (tmp[3] >> 16) & 0xff;
  hash[14] = (tmp[3] >> 8) & 0xff;
  hash[15] = (tmp[3] >> 0) & 0xff;
  hash[16] = (tmp[4] >> 24) & 0xff;
  hash[17] = (tmp[4] >> 16) & 0xff;
  hash[18] = (tmp[4] >> 8) & 0xff;
  hash[19] = (tmp[4] >> 0) & 0xff;
  hash[20] = (tmp[5] >> 24) & 0xff;
  hash[21] = (tmp[5] >> 16) & 0xff;
  hash[22] = (tmp[5] >> 8) & 0xff;
  hash[23] = (tmp[5] >> 0) & 0xff;
  hash[24] = (tmp[6] >> 24) & 0xff;
  hash[25] = (tmp[6] >> 16) & 0xff;
  hash[26] = (tmp[6] >> 8) & 0xff;
  hash[27] = (tmp[6] >> 0) & 0xff;
  hash[28] = (tmp[7] >> 24) & 0xff;
  hash[29] = (tmp[7] >> 16) & 0xff;
  hash[30] = (tmp[7] >> 8) & 0xff;
  hash[31] = (tmp[7] >> 0) & 0xff;
}

void
snippets_fnv1a_256 (const uint8_t * data, size_t len, uint8_t hash[32])
{
  size_t i;
  uint64_t tmp[8], tmp2[8];

  /* 256 bit number, 32 bit per field, highest 32 bit first */
  tmp[0] = 0xdd268dbc;
  tmp[1] = 0xaac55036;
  tmp[2] = 0x2d98c384;
  tmp[3] = 0xc4e576cc;
  tmp[4] = 0xc8b15368;
  tmp[5] = 0x47b6bbb3;
  tmp[6] = 0x1023b4c8;
  tmp[7] = 0xcaee0535;

  for (i = 0; i < len; i++) {
    /* Multiplication */

    /* lowest 32 bits */

    /* multiply and keep carries forward */
    tmp2[7] = (tmp[7] ^ *data) * 0x00000163;
    tmp2[6] = tmp[6] * 0x00000163 + (tmp2[7] >> 32);
    tmp2[5] = tmp[5] * 0x00000163 + (tmp2[6] >> 32);
    tmp2[4] = tmp[4] * 0x00000163 + (tmp2[5] >> 32);
    tmp2[3] = tmp[3] * 0x00000163 + (tmp2[4] >> 32);
    tmp2[2] = tmp[2] * 0x00000163 + (tmp2[3] >> 32);
    tmp2[1] = tmp[1] * 0x00000163 + (tmp2[2] >> 32);
    tmp2[0] = tmp[0] * 0x00000163 + (tmp2[1] >> 32);
    /* drop carries */
    tmp2[7] &= 0xffffffff;
    tmp2[6] &= 0xffffffff;
    tmp2[5] &= 0xffffffff;
    tmp2[4] &= 0xffffffff;
    tmp2[3] &= 0xffffffff;
    tmp2[2] &= 0xffffffff;
    tmp2[1] &= 0xffffffff;
    tmp2[0] &= 0xffffffff;

    /* highest 32 bits */

    /* multiply and keep carries forward */
    tmp2[2] += tmp[7] * 0x00000100;
    tmp2[1] += tmp[6] * 0x00000100 + (tmp2[2] >> 32);
    tmp2[0] += tmp[5] * 0x00000100 + (tmp2[1] >> 32);

    /* drop carries */
    tmp2[2] &= 0xffffffff;
    tmp2[1] &= 0xffffffff;
    tmp2[0] &= 0xffffffff;

    tmp[7] = tmp2[7];
    tmp[6] = tmp2[6];
    tmp[5] = tmp2[5];
    tmp[4] = tmp2[4];
    tmp[3] = tmp2[3];
    tmp[2] = tmp2[2];
    tmp[1] = tmp2[1];
    tmp[0] = tmp2[0];

    data++;
  }

  hash[0] = (tmp[0] >> 24) & 0xff;
  hash[1] = (tmp[0] >> 16) & 0xff;
  hash[2] = (tmp[0] >> 8) & 0xff;
  hash[3] = (tmp[0] >> 0) & 0xff;
  hash[4] = (tmp[1] >> 24) & 0xff;
  hash[5] = (tmp[1] >> 16) & 0xff;
  hash[6] = (tmp[1] >> 8) & 0xff;
  hash[7] = (tmp[1] >> 0) & 0xff;
  hash[8] = (tmp[2] >> 24) & 0xff;
  hash[9] = (tmp[2] >> 16) & 0xff;
  hash[10] = (tmp[2] >> 8) & 0xff;
  hash[11] = (tmp[2] >> 0) & 0xff;
  hash[12] = (tmp[3] >> 24) & 0xff;
  hash[13] = (tmp[3] >> 16) & 0xff;
  hash[14] = (tmp[3] >> 8) & 0xff;
  hash[15] = (tmp[3] >> 0) & 0xff;
  hash[16] = (tmp[4] >> 24) & 0xff;
  hash[17] = (tmp[4] >> 16) & 0xff;
  hash[18] = (tmp[4] >> 8) & 0xff;
  hash[19] = (tmp[4] >> 0) & 0xff;
  hash[20] = (tmp[5] >> 24) & 0xff;
  hash[21] = (tmp[5] >> 16) & 0xff;
  hash[22] = (tmp[5] >> 8) & 0xff;
  hash[23] = (tmp[5] >> 0) & 0xff;
  hash[24] = (tmp[6] >> 24) & 0xff;
  hash[25] = (tmp[6] >> 16) & 0xff;
  hash[26] = (tmp[6] >> 8) & 0xff;
  hash[27] = (tmp[6] >> 0) & 0xff;
  hash[28] = (tmp[7] >> 24) & 0xff;
  hash[29] = (tmp[7] >> 16) & 0xff;
  hash[30] = (tmp[7] >> 8) & 0xff;
  hash[31] = (tmp[7] >> 0) & 0xff;
}

/* 512 bit prime  = 3583591587484486736891907648909510844994632795575439255839
 *                  9825615420669938882575126094039892345713852759
 *                = 0x01000000000000000000000000000000000000000000000000000000
 *                  00000000000000000000000000000157
 * 512 bit offset = 9659303129496669498009435400716310466090418745672637896108
 *                  3743294344626579945829321977164384498130518922065398057844
 *                  95328239340083876191928701583869517785
 *                = 0xb86db0b1171f4416dca1e50f309990acac87d059c900000000000000
 *                  00000d21e948f68a34c192f62ea79bc942dbe7ce182036415f56e34bac
 *                  982aac4afe9fd9
 */
void
snippets_fnv1_512 (const uint8_t * data, size_t len, uint8_t hash[64])
{
  size_t i;
  uint64_t tmp[16], tmp2[16];

  /* 512 bit number, 32 bit per field, highest 32 bit first */
  tmp[0] = 0xb86db0b1;
  tmp[1] = 0x171f4416;
  tmp[2] = 0xdca1e50f;
  tmp[3] = 0x309990ac;
  tmp[4] = 0xac87d059;
  tmp[5] = 0xc9000000;
  tmp[6] = 0x00000000;
  tmp[7] = 0x00000d21;
  tmp[8] = 0xe948f68a;
  tmp[9] = 0x34c192f6;
  tmp[10] = 0x2ea79bc9;
  tmp[11] = 0x42dbe7ce;
  tmp[12] = 0x18203641;
  tmp[13] = 0x5f56e34b;
  tmp[14] = 0xac982aac;
  tmp[15] = 0x4afe9fd9;

  for (i = 0; i < len; i++) {
    /* Multiplication */

    /* lowest 32 bits */

    /* multiply and keep carries forward */
    tmp2[15] = tmp[15] * 0x00000157;
    tmp2[14] = tmp[14] * 0x00000157 + (tmp2[15] >> 32);
    tmp2[13] = tmp[13] * 0x00000157 + (tmp2[14] >> 32);
    tmp2[12] = tmp[12] * 0x00000157 + (tmp2[13] >> 32);
    tmp2[11] = tmp[11] * 0x00000157 + (tmp2[12] >> 32);
    tmp2[10] = tmp[10] * 0x00000157 + (tmp2[11] >> 32);
    tmp2[9] = tmp[9] * 0x00000157 + (tmp2[10] >> 32);
    tmp2[8] = tmp[8] * 0x00000157 + (tmp2[9] >> 32);
    tmp2[7] = tmp[7] * 0x00000157 + (tmp2[8] >> 32);
    tmp2[6] = tmp[6] * 0x00000157 + (tmp2[7] >> 32);
    tmp2[5] = tmp[5] * 0x00000157 + (tmp2[6] >> 32);
    tmp2[4] = tmp[4] * 0x00000157 + (tmp2[5] >> 32);
    tmp2[3] = tmp[3] * 0x00000157 + (tmp2[4] >> 32);
    tmp2[2] = tmp[2] * 0x00000157 + (tmp2[3] >> 32);
    tmp2[1] = tmp[1] * 0x00000157 + (tmp2[2] >> 32);
    tmp2[0] = tmp[0] * 0x00000157 + (tmp2[1] >> 32);
    /* drop carries */
    tmp2[15] &= 0xffffffff;
    tmp2[14] &= 0xffffffff;
    tmp2[13] &= 0xffffffff;
    tmp2[12] &= 0xffffffff;
    tmp2[11] &= 0xffffffff;
    tmp2[10] &= 0xffffffff;
    tmp2[9] &= 0xffffffff;
    tmp2[8] &= 0xffffffff;
    tmp2[7] &= 0xffffffff;
    tmp2[6] &= 0xffffffff;
    tmp2[5] &= 0xffffffff;
    tmp2[4] &= 0xffffffff;
    tmp2[3] &= 0xffffffff;
    tmp2[2] &= 0xffffffff;
    tmp2[1] &= 0xffffffff;
    tmp2[0] &= 0xffffffff;

    /* highest 32 bits */

    /* multiply and keep carries forward */
    tmp2[5] += tmp[15] * 0x01000000;
    tmp2[4] += tmp[14] * 0x01000000 + (tmp2[5] >> 32);
    tmp2[3] += tmp[13] * 0x01000000 + (tmp2[4] >> 32);
    tmp2[2] += tmp[12] * 0x01000000 + (tmp2[3] >> 32);
    tmp2[1] += tmp[11] * 0x01000000 + (tmp2[2] >> 32);
    tmp2[0] += tmp[10] * 0x01000000 + (tmp2[1] >> 32);
    /* drop carries */
    tmp2[5] &= 0xffffffff;
    tmp2[4] &= 0xffffffff;
    tmp2[3] &= 0xffffffff;
    tmp2[2] &= 0xffffffff;
    tmp2[1] &= 0xffffffff;
    tmp2[0] &= 0xffffffff;

    tmp[15] = tmp2[15] ^ *data;
    tmp[14] = tmp2[14];
    tmp[13] = tmp2[13];
    tmp[12] = tmp2[12];
    tmp[11] = tmp2[11];
    tmp[10] = tmp2[10];
    tmp[9] = tmp2[9];
    tmp[8] = tmp2[8];
    tmp[7] = tmp2[7];
    tmp[6] = tmp2[6];
    tmp[5] = tmp2[5];
    tmp[4] = tmp2[4];
    tmp[3] = tmp2[3];
    tmp[2] = tmp2[2];
    tmp[1] = tmp2[1];
    tmp[0] = tmp2[0];

    data++;
  }

  hash[0] = (tmp[0] >> 24) & 0xff;
  hash[1] = (tmp[0] >> 16) & 0xff;
  hash[2] = (tmp[0] >> 8) & 0xff;
  hash[3] = (tmp[0] >> 0) & 0xff;
  hash[4] = (tmp[1] >> 24) & 0xff;
  hash[5] = (tmp[1] >> 16) & 0xff;
  hash[6] = (tmp[1] >> 8) & 0xff;
  hash[7] = (tmp[1] >> 0) & 0xff;
  hash[8] = (tmp[2] >> 24) & 0xff;
  hash[9] = (tmp[2] >> 16) & 0xff;
  hash[10] = (tmp[2] >> 8) & 0xff;
  hash[11] = (tmp[2] >> 0) & 0xff;
  hash[12] = (tmp[3] >> 24) & 0xff;
  hash[13] = (tmp[3] >> 16) & 0xff;
  hash[14] = (tmp[3] >> 8) & 0xff;
  hash[15] = (tmp[3] >> 0) & 0xff;
  hash[16] = (tmp[4] >> 24) & 0xff;
  hash[17] = (tmp[4] >> 16) & 0xff;
  hash[18] = (tmp[4] >> 8) & 0xff;
  hash[19] = (tmp[4] >> 0) & 0xff;
  hash[20] = (tmp[5] >> 24) & 0xff;
  hash[21] = (tmp[5] >> 16) & 0xff;
  hash[22] = (tmp[5] >> 8) & 0xff;
  hash[23] = (tmp[5] >> 0) & 0xff;
  hash[24] = (tmp[6] >> 24) & 0xff;
  hash[25] = (tmp[6] >> 16) & 0xff;
  hash[26] = (tmp[6] >> 8) & 0xff;
  hash[27] = (tmp[6] >> 0) & 0xff;
  hash[28] = (tmp[7] >> 24) & 0xff;
  hash[29] = (tmp[7] >> 16) & 0xff;
  hash[30] = (tmp[7] >> 8) & 0xff;
  hash[31] = (tmp[7] >> 0) & 0xff;

  hash[32] = (tmp[8] >> 24) & 0xff;
  hash[33] = (tmp[8] >> 16) & 0xff;
  hash[34] = (tmp[8] >> 8) & 0xff;
  hash[35] = (tmp[8] >> 0) & 0xff;
  hash[36] = (tmp[9] >> 24) & 0xff;
  hash[37] = (tmp[9] >> 16) & 0xff;
  hash[38] = (tmp[9] >> 8) & 0xff;
  hash[39] = (tmp[9] >> 0) & 0xff;
  hash[40] = (tmp[10] >> 24) & 0xff;
  hash[41] = (tmp[10] >> 16) & 0xff;
  hash[42] = (tmp[10] >> 8) & 0xff;
  hash[43] = (tmp[10] >> 0) & 0xff;
  hash[44] = (tmp[11] >> 24) & 0xff;
  hash[45] = (tmp[11] >> 16) & 0xff;
  hash[46] = (tmp[11] >> 8) & 0xff;
  hash[47] = (tmp[11] >> 0) & 0xff;
  hash[48] = (tmp[12] >> 24) & 0xff;
  hash[49] = (tmp[12] >> 16) & 0xff;
  hash[50] = (tmp[12] >> 8) & 0xff;
  hash[51] = (tmp[12] >> 0) & 0xff;
  hash[52] = (tmp[13] >> 24) & 0xff;
  hash[53] = (tmp[13] >> 16) & 0xff;
  hash[54] = (tmp[13] >> 8) & 0xff;
  hash[55] = (tmp[13] >> 0) & 0xff;
  hash[56] = (tmp[14] >> 24) & 0xff;
  hash[57] = (tmp[14] >> 16) & 0xff;
  hash[58] = (tmp[14] >> 8) & 0xff;
  hash[59] = (tmp[14] >> 0) & 0xff;
  hash[60] = (tmp[15] >> 24) & 0xff;
  hash[61] = (tmp[15] >> 16) & 0xff;
  hash[62] = (tmp[15] >> 8) & 0xff;
  hash[63] = (tmp[15] >> 0) & 0xff;
}

void
snippets_fnv1a_512 (const uint8_t * data, size_t len, uint8_t hash[64])
{
  size_t i;
  uint64_t tmp[16], tmp2[16];

  /* 512 bit number, 32 bit per field, highest 32 bit first */
  tmp[0] = 0xb86db0b1;
  tmp[1] = 0x171f4416;
  tmp[2] = 0xdca1e50f;
  tmp[3] = 0x309990ac;
  tmp[4] = 0xac87d059;
  tmp[5] = 0xc9000000;
  tmp[6] = 0x00000000;
  tmp[7] = 0x00000d21;
  tmp[8] = 0xe948f68a;
  tmp[9] = 0x34c192f6;
  tmp[10] = 0x2ea79bc9;
  tmp[11] = 0x42dbe7ce;
  tmp[12] = 0x18203641;
  tmp[13] = 0x5f56e34b;
  tmp[14] = 0xac982aac;
  tmp[15] = 0x4afe9fd9;

  for (i = 0; i < len; i++) {
    /* Multiplication */

    /* lowest 32 bits */

    /* multiply and keep carries forward */
    tmp2[15] = (*data ^ tmp[15]) * 0x00000157;
    tmp2[14] = tmp[14] * 0x00000157 + (tmp2[15] >> 32);
    tmp2[13] = tmp[13] * 0x00000157 + (tmp2[14] >> 32);
    tmp2[12] = tmp[12] * 0x00000157 + (tmp2[13] >> 32);
    tmp2[11] = tmp[11] * 0x00000157 + (tmp2[12] >> 32);
    tmp2[10] = tmp[10] * 0x00000157 + (tmp2[11] >> 32);
    tmp2[9] = tmp[9] * 0x00000157 + (tmp2[10] >> 32);
    tmp2[8] = tmp[8] * 0x00000157 + (tmp2[9] >> 32);
    tmp2[7] = tmp[7] * 0x00000157 + (tmp2[8] >> 32);
    tmp2[6] = tmp[6] * 0x00000157 + (tmp2[7] >> 32);
    tmp2[5] = tmp[5] * 0x00000157 + (tmp2[6] >> 32);
    tmp2[4] = tmp[4] * 0x00000157 + (tmp2[5] >> 32);
    tmp2[3] = tmp[3] * 0x00000157 + (tmp2[4] >> 32);
    tmp2[2] = tmp[2] * 0x00000157 + (tmp2[3] >> 32);
    tmp2[1] = tmp[1] * 0x00000157 + (tmp2[2] >> 32);
    tmp2[0] = tmp[0] * 0x00000157 + (tmp2[1] >> 32);
    /* drop carries */
    tmp2[15] &= 0xffffffff;
    tmp2[14] &= 0xffffffff;
    tmp2[13] &= 0xffffffff;
    tmp2[12] &= 0xffffffff;
    tmp2[11] &= 0xffffffff;
    tmp2[10] &= 0xffffffff;
    tmp2[9] &= 0xffffffff;
    tmp2[8] &= 0xffffffff;
    tmp2[7] &= 0xffffffff;
    tmp2[6] &= 0xffffffff;
    tmp2[5] &= 0xffffffff;
    tmp2[4] &= 0xffffffff;
    tmp2[3] &= 0xffffffff;
    tmp2[2] &= 0xffffffff;
    tmp2[1] &= 0xffffffff;
    tmp2[0] &= 0xffffffff;

    /* highest 32 bits */

    /* multiply and keep carries forward */
    tmp2[5] += tmp[15] * 0x01000000;
    tmp2[4] += tmp[14] * 0x01000000 + (tmp2[5] >> 32);
    tmp2[3] += tmp[13] * 0x01000000 + (tmp2[4] >> 32);
    tmp2[2] += tmp[12] * 0x01000000 + (tmp2[3] >> 32);
    tmp2[1] += tmp[11] * 0x01000000 + (tmp2[2] >> 32);
    tmp2[0] += tmp[10] * 0x01000000 + (tmp2[1] >> 32);
    /* drop carries */
    tmp2[5] &= 0xffffffff;
    tmp2[4] &= 0xffffffff;
    tmp2[3] &= 0xffffffff;
    tmp2[2] &= 0xffffffff;
    tmp2[1] &= 0xffffffff;
    tmp2[0] &= 0xffffffff;

    tmp[15] = tmp2[15];
    tmp[14] = tmp2[14];
    tmp[13] = tmp2[13];
    tmp[12] = tmp2[12];
    tmp[11] = tmp2[11];
    tmp[10] = tmp2[10];
    tmp[9] = tmp2[9];
    tmp[8] = tmp2[8];
    tmp[7] = tmp2[7];
    tmp[6] = tmp2[6];
    tmp[5] = tmp2[5];
    tmp[4] = tmp2[4];
    tmp[3] = tmp2[3];
    tmp[2] = tmp2[2];
    tmp[1] = tmp2[1];
    tmp[0] = tmp2[0];

    data++;
  }

  hash[0] = (tmp[0] >> 24) & 0xff;
  hash[1] = (tmp[0] >> 16) & 0xff;
  hash[2] = (tmp[0] >> 8) & 0xff;
  hash[3] = (tmp[0] >> 0) & 0xff;
  hash[4] = (tmp[1] >> 24) & 0xff;
  hash[5] = (tmp[1] >> 16) & 0xff;
  hash[6] = (tmp[1] >> 8) & 0xff;
  hash[7] = (tmp[1] >> 0) & 0xff;
  hash[8] = (tmp[2] >> 24) & 0xff;
  hash[9] = (tmp[2] >> 16) & 0xff;
  hash[10] = (tmp[2] >> 8) & 0xff;
  hash[11] = (tmp[2] >> 0) & 0xff;
  hash[12] = (tmp[3] >> 24) & 0xff;
  hash[13] = (tmp[3] >> 16) & 0xff;
  hash[14] = (tmp[3] >> 8) & 0xff;
  hash[15] = (tmp[3] >> 0) & 0xff;
  hash[16] = (tmp[4] >> 24) & 0xff;
  hash[17] = (tmp[4] >> 16) & 0xff;
  hash[18] = (tmp[4] >> 8) & 0xff;
  hash[19] = (tmp[4] >> 0) & 0xff;
  hash[20] = (tmp[5] >> 24) & 0xff;
  hash[21] = (tmp[5] >> 16) & 0xff;
  hash[22] = (tmp[5] >> 8) & 0xff;
  hash[23] = (tmp[5] >> 0) & 0xff;
  hash[24] = (tmp[6] >> 24) & 0xff;
  hash[25] = (tmp[6] >> 16) & 0xff;
  hash[26] = (tmp[6] >> 8) & 0xff;
  hash[27] = (tmp[6] >> 0) & 0xff;
  hash[28] = (tmp[7] >> 24) & 0xff;
  hash[29] = (tmp[7] >> 16) & 0xff;
  hash[30] = (tmp[7] >> 8) & 0xff;
  hash[31] = (tmp[7] >> 0) & 0xff;

  hash[32] = (tmp[8] >> 24) & 0xff;
  hash[33] = (tmp[8] >> 16) & 0xff;
  hash[34] = (tmp[8] >> 8) & 0xff;
  hash[35] = (tmp[8] >> 0) & 0xff;
  hash[36] = (tmp[9] >> 24) & 0xff;
  hash[37] = (tmp[9] >> 16) & 0xff;
  hash[38] = (tmp[9] >> 8) & 0xff;
  hash[39] = (tmp[9] >> 0) & 0xff;
  hash[40] = (tmp[10] >> 24) & 0xff;
  hash[41] = (tmp[10] >> 16) & 0xff;
  hash[42] = (tmp[10] >> 8) & 0xff;
  hash[43] = (tmp[10] >> 0) & 0xff;
  hash[44] = (tmp[11] >> 24) & 0xff;
  hash[45] = (tmp[11] >> 16) & 0xff;
  hash[46] = (tmp[11] >> 8) & 0xff;
  hash[47] = (tmp[11] >> 0) & 0xff;
  hash[48] = (tmp[12] >> 24) & 0xff;
  hash[49] = (tmp[12] >> 16) & 0xff;
  hash[50] = (tmp[12] >> 8) & 0xff;
  hash[51] = (tmp[12] >> 0) & 0xff;
  hash[52] = (tmp[13] >> 24) & 0xff;
  hash[53] = (tmp[13] >> 16) & 0xff;
  hash[54] = (tmp[13] >> 8) & 0xff;
  hash[55] = (tmp[13] >> 0) & 0xff;
  hash[56] = (tmp[14] >> 24) & 0xff;
  hash[57] = (tmp[14] >> 16) & 0xff;
  hash[58] = (tmp[14] >> 8) & 0xff;
  hash[59] = (tmp[14] >> 0) & 0xff;
  hash[60] = (tmp[15] >> 24) & 0xff;
  hash[61] = (tmp[15] >> 16) & 0xff;
  hash[62] = (tmp[15] >> 8) & 0xff;
  hash[63] = (tmp[15] >> 0) & 0xff;
}

/* 1024 bit prime  = 501645651011311865543459881103527895503076534540479074
 *                   430301752383111205510814745150915769222029538271616265
 *                   187852689524938529229181652437508374669137180409427187
 *                   3160484737966720260389217684476157468082573
 *                 = 0x1000000000000000000000000000000000000000000000000000
 *                   000000000000000000000000000000000000000000000000000000
 *                   000000000000000000000000000000000000000000000000000000
 *                   0000000018d
 * 1024 bit offset = 141977950649476210687220706414032183208806227954419339
 *                   608784749146175827232522967323037177221508640965212023
 *                   555493656281746691085718147604710150761480297559698040
 *                   773201576924585630032153049571501574036444603635505054
 *                   127112859663616102678680828938239637904393364110868845
 *                   84107735010676915
 *                 = 0x5f7a76758ecc4d32e56d5a591028b74b29fc4223fdada16c3bf3
 *                   4eda3674da9a21d900000000000000000000000000000000000000
 *                   000000000000000000000000000000000000000000000000000000
 *                   0000000004c6d7eb6e73802734510a555f256cc005ae556bde8cc9
 *                   c6a93b21aff4b16c71ee90b3
 */
void
snippets_fnv1_1024 (const uint8_t * data, size_t len, uint8_t hash[128])
{
  size_t i;
  uint64_t tmp[32], tmp2[32];

  /* 1024 bit number, 32 bit per field, highest 32 bit first */
  tmp[0] = 0x00000000;
  tmp[1] = 0x00000000;
  tmp[2] = 0x005f7a76;
  tmp[3] = 0x758ecc4d;
  tmp[4] = 0x32e56d5a;
  tmp[5] = 0x591028b7;
  tmp[6] = 0x4b29fc42;
  tmp[7] = 0x23fdada1;
  tmp[8] = 0x6c3bf34e;
  tmp[9] = 0xda3674da;
  tmp[10] = 0x9a21d900;
  tmp[11] = 0x00000000;
  tmp[12] = 0x00000000;
  tmp[13] = 0x00000000;
  tmp[14] = 0x00000000;
  tmp[15] = 0x00000000;
  tmp[16] = 0x00000000;
  tmp[17] = 0x00000000;
  tmp[18] = 0x00000000;
  tmp[19] = 0x00000000;
  tmp[20] = 0x00000000;
  tmp[21] = 0x00000000;
  tmp[22] = 0x00000000;
  tmp[23] = 0x0004c6d7;
  tmp[24] = 0xeb6e7380;
  tmp[25] = 0x2734510a;
  tmp[26] = 0x555f256c;
  tmp[27] = 0xc005ae55;
  tmp[28] = 0x6bde8cc9;
  tmp[29] = 0xc6a93b21;
  tmp[30] = 0xaff4b16c;
  tmp[31] = 0x71ee90b3;

  for (i = 0; i < len; i++) {
    /* Multiplication */

    /* lowest 32 bits */

    /* multiply and keep carries forward */
    tmp2[31] = tmp[31] * 0x0000018d;
    tmp2[30] = tmp[30] * 0x0000018d + (tmp2[31] >> 32);
    tmp2[29] = tmp[29] * 0x0000018d + (tmp2[30] >> 32);
    tmp2[28] = tmp[28] * 0x0000018d + (tmp2[29] >> 32);
    tmp2[27] = tmp[27] * 0x0000018d + (tmp2[28] >> 32);
    tmp2[26] = tmp[26] * 0x0000018d + (tmp2[27] >> 32);
    tmp2[25] = tmp[25] * 0x0000018d + (tmp2[26] >> 32);
    tmp2[24] = tmp[24] * 0x0000018d + (tmp2[25] >> 32);
    tmp2[23] = tmp[23] * 0x0000018d + (tmp2[24] >> 32);
    tmp2[22] = tmp[22] * 0x0000018d + (tmp2[23] >> 32);
    tmp2[21] = tmp[21] * 0x0000018d + (tmp2[22] >> 32);
    tmp2[20] = tmp[20] * 0x0000018d + (tmp2[21] >> 32);
    tmp2[19] = tmp[19] * 0x0000018d + (tmp2[20] >> 32);
    tmp2[18] = tmp[18] * 0x0000018d + (tmp2[19] >> 32);
    tmp2[17] = tmp[17] * 0x0000018d + (tmp2[18] >> 32);
    tmp2[16] = tmp[16] * 0x0000018d + (tmp2[17] >> 32);
    tmp2[15] = tmp[15] * 0x0000018d + (tmp2[16] >> 32);
    tmp2[14] = tmp[14] * 0x0000018d + (tmp2[15] >> 32);
    tmp2[13] = tmp[13] * 0x0000018d + (tmp2[14] >> 32);
    tmp2[12] = tmp[12] * 0x0000018d + (tmp2[13] >> 32);
    tmp2[11] = tmp[11] * 0x0000018d + (tmp2[12] >> 32);
    tmp2[10] = tmp[10] * 0x0000018d + (tmp2[11] >> 32);
    tmp2[9] = tmp[9] * 0x0000018d + (tmp2[10] >> 32);
    tmp2[8] = tmp[8] * 0x0000018d + (tmp2[9] >> 32);
    tmp2[7] = tmp[7] * 0x0000018d + (tmp2[8] >> 32);
    tmp2[6] = tmp[6] * 0x0000018d + (tmp2[7] >> 32);
    tmp2[5] = tmp[5] * 0x0000018d + (tmp2[6] >> 32);
    tmp2[4] = tmp[4] * 0x0000018d + (tmp2[5] >> 32);
    tmp2[3] = tmp[3] * 0x0000018d + (tmp2[4] >> 32);
    tmp2[2] = tmp[2] * 0x0000018d + (tmp2[3] >> 32);
    tmp2[1] = tmp[1] * 0x0000018d + (tmp2[2] >> 32);
    tmp2[0] = tmp[0] * 0x0000018d + (tmp2[1] >> 32);
    /* drop carries */
    tmp2[31] &= 0xffffffff;
    tmp2[30] &= 0xffffffff;
    tmp2[29] &= 0xffffffff;
    tmp2[28] &= 0xffffffff;
    tmp2[27] &= 0xffffffff;
    tmp2[26] &= 0xffffffff;
    tmp2[25] &= 0xffffffff;
    tmp2[24] &= 0xffffffff;
    tmp2[23] &= 0xffffffff;
    tmp2[22] &= 0xffffffff;
    tmp2[21] &= 0xffffffff;
    tmp2[20] &= 0xffffffff;
    tmp2[19] &= 0xffffffff;
    tmp2[18] &= 0xffffffff;
    tmp2[17] &= 0xffffffff;
    tmp2[16] &= 0xffffffff;
    tmp2[15] &= 0xffffffff;
    tmp2[14] &= 0xffffffff;
    tmp2[13] &= 0xffffffff;
    tmp2[12] &= 0xffffffff;
    tmp2[11] &= 0xffffffff;
    tmp2[10] &= 0xffffffff;
    tmp2[9] &= 0xffffffff;
    tmp2[8] &= 0xffffffff;
    tmp2[7] &= 0xffffffff;
    tmp2[6] &= 0xffffffff;
    tmp2[5] &= 0xffffffff;
    tmp2[4] &= 0xffffffff;
    tmp2[3] &= 0xffffffff;
    tmp2[2] &= 0xffffffff;
    tmp2[1] &= 0xffffffff;
    tmp2[0] &= 0xffffffff;

    /* highest 32 bits */

    /* multiply and keep carries forward */
    tmp2[10] += tmp[31] * 0x00000100;
    tmp2[9] += tmp[30] * 0x00000100 + (tmp2[10] >> 32);
    tmp2[8] += tmp[29] * 0x00000100 + (tmp2[9] >> 32);
    tmp2[7] += tmp[28] * 0x00000100 + (tmp2[8] >> 32);
    tmp2[6] += tmp[27] * 0x00000100 + (tmp2[7] >> 32);
    tmp2[5] += tmp[26] * 0x00000100 + (tmp2[6] >> 32);
    tmp2[4] += tmp[25] * 0x00000100 + (tmp2[5] >> 32);
    tmp2[3] += tmp[24] * 0x00000100 + (tmp2[4] >> 32);
    tmp2[2] += tmp[23] * 0x00000100 + (tmp2[3] >> 32);
    tmp2[1] += tmp[22] * 0x00000100 + (tmp2[2] >> 32);
    tmp2[0] += tmp[21] * 0x00000100 + (tmp2[1] >> 32);
    /* drop carries */
    tmp2[10] &= 0xffffffff;
    tmp2[9] &= 0xffffffff;
    tmp2[8] &= 0xffffffff;
    tmp2[7] &= 0xffffffff;
    tmp2[6] &= 0xffffffff;
    tmp2[5] &= 0xffffffff;
    tmp2[4] &= 0xffffffff;
    tmp2[3] &= 0xffffffff;
    tmp2[2] &= 0xffffffff;
    tmp2[1] &= 0xffffffff;
    tmp2[0] &= 0xffffffff;

    tmp[31] = tmp2[31] ^ *data;
    tmp[30] = tmp2[30];
    tmp[29] = tmp2[29];
    tmp[28] = tmp2[28];
    tmp[27] = tmp2[27];
    tmp[26] = tmp2[26];
    tmp[25] = tmp2[25];
    tmp[24] = tmp2[24];
    tmp[23] = tmp2[23];
    tmp[22] = tmp2[22];
    tmp[21] = tmp2[21];
    tmp[20] = tmp2[20];
    tmp[19] = tmp2[19];
    tmp[18] = tmp2[18];
    tmp[17] = tmp2[17];
    tmp[16] = tmp2[16];
    tmp[15] = tmp2[15];
    tmp[14] = tmp2[14];
    tmp[13] = tmp2[13];
    tmp[12] = tmp2[12];
    tmp[11] = tmp2[11];
    tmp[10] = tmp2[10];
    tmp[9] = tmp2[9];
    tmp[8] = tmp2[8];
    tmp[7] = tmp2[7];
    tmp[6] = tmp2[6];
    tmp[5] = tmp2[5];
    tmp[4] = tmp2[4];
    tmp[3] = tmp2[3];
    tmp[2] = tmp2[2];
    tmp[1] = tmp2[1];
    tmp[0] = tmp2[0];

    data++;
  }

  hash[0] = (tmp[0] >> 24) & 0xff;
  hash[1] = (tmp[0] >> 16) & 0xff;
  hash[2] = (tmp[0] >> 8) & 0xff;
  hash[3] = (tmp[0] >> 0) & 0xff;
  hash[4] = (tmp[1] >> 24) & 0xff;
  hash[5] = (tmp[1] >> 16) & 0xff;
  hash[6] = (tmp[1] >> 8) & 0xff;
  hash[7] = (tmp[1] >> 0) & 0xff;
  hash[8] = (tmp[2] >> 24) & 0xff;
  hash[9] = (tmp[2] >> 16) & 0xff;
  hash[10] = (tmp[2] >> 8) & 0xff;
  hash[11] = (tmp[2] >> 0) & 0xff;
  hash[12] = (tmp[3] >> 24) & 0xff;
  hash[13] = (tmp[3] >> 16) & 0xff;
  hash[14] = (tmp[3] >> 8) & 0xff;
  hash[15] = (tmp[3] >> 0) & 0xff;
  hash[16] = (tmp[4] >> 24) & 0xff;
  hash[17] = (tmp[4] >> 16) & 0xff;
  hash[18] = (tmp[4] >> 8) & 0xff;
  hash[19] = (tmp[4] >> 0) & 0xff;
  hash[20] = (tmp[5] >> 24) & 0xff;
  hash[21] = (tmp[5] >> 16) & 0xff;
  hash[22] = (tmp[5] >> 8) & 0xff;
  hash[23] = (tmp[5] >> 0) & 0xff;
  hash[24] = (tmp[6] >> 24) & 0xff;
  hash[25] = (tmp[6] >> 16) & 0xff;
  hash[26] = (tmp[6] >> 8) & 0xff;
  hash[27] = (tmp[6] >> 0) & 0xff;
  hash[28] = (tmp[7] >> 24) & 0xff;
  hash[29] = (tmp[7] >> 16) & 0xff;
  hash[30] = (tmp[7] >> 8) & 0xff;
  hash[31] = (tmp[7] >> 0) & 0xff;

  hash[32] = (tmp[8] >> 24) & 0xff;
  hash[33] = (tmp[8] >> 16) & 0xff;
  hash[34] = (tmp[8] >> 8) & 0xff;
  hash[35] = (tmp[8] >> 0) & 0xff;
  hash[36] = (tmp[9] >> 24) & 0xff;
  hash[37] = (tmp[9] >> 16) & 0xff;
  hash[38] = (tmp[9] >> 8) & 0xff;
  hash[39] = (tmp[9] >> 0) & 0xff;
  hash[40] = (tmp[10] >> 24) & 0xff;
  hash[41] = (tmp[10] >> 16) & 0xff;
  hash[42] = (tmp[10] >> 8) & 0xff;
  hash[43] = (tmp[10] >> 0) & 0xff;
  hash[44] = (tmp[11] >> 24) & 0xff;
  hash[45] = (tmp[11] >> 16) & 0xff;
  hash[46] = (tmp[11] >> 8) & 0xff;
  hash[47] = (tmp[11] >> 0) & 0xff;
  hash[48] = (tmp[12] >> 24) & 0xff;
  hash[49] = (tmp[12] >> 16) & 0xff;
  hash[50] = (tmp[12] >> 8) & 0xff;
  hash[51] = (tmp[12] >> 0) & 0xff;
  hash[52] = (tmp[13] >> 24) & 0xff;
  hash[53] = (tmp[13] >> 16) & 0xff;
  hash[54] = (tmp[13] >> 8) & 0xff;
  hash[55] = (tmp[13] >> 0) & 0xff;
  hash[56] = (tmp[14] >> 24) & 0xff;
  hash[57] = (tmp[14] >> 16) & 0xff;
  hash[58] = (tmp[14] >> 8) & 0xff;
  hash[59] = (tmp[14] >> 0) & 0xff;
  hash[60] = (tmp[15] >> 24) & 0xff;
  hash[61] = (tmp[15] >> 16) & 0xff;
  hash[62] = (tmp[15] >> 8) & 0xff;
  hash[63] = (tmp[15] >> 0) & 0xff;

  hash[64] = (tmp[16] >> 24) & 0xff;
  hash[65] = (tmp[16] >> 16) & 0xff;
  hash[66] = (tmp[16] >> 8) & 0xff;
  hash[67] = (tmp[16] >> 0) & 0xff;
  hash[68] = (tmp[17] >> 24) & 0xff;
  hash[69] = (tmp[17] >> 16) & 0xff;
  hash[70] = (tmp[17] >> 8) & 0xff;
  hash[71] = (tmp[17] >> 0) & 0xff;
  hash[72] = (tmp[18] >> 24) & 0xff;
  hash[73] = (tmp[18] >> 16) & 0xff;
  hash[74] = (tmp[18] >> 8) & 0xff;
  hash[75] = (tmp[18] >> 0) & 0xff;
  hash[76] = (tmp[19] >> 24) & 0xff;
  hash[77] = (tmp[19] >> 16) & 0xff;
  hash[78] = (tmp[19] >> 8) & 0xff;
  hash[79] = (tmp[19] >> 0) & 0xff;
  hash[80] = (tmp[20] >> 24) & 0xff;
  hash[81] = (tmp[20] >> 16) & 0xff;
  hash[82] = (tmp[20] >> 8) & 0xff;
  hash[83] = (tmp[20] >> 0) & 0xff;
  hash[84] = (tmp[21] >> 24) & 0xff;
  hash[85] = (tmp[21] >> 16) & 0xff;
  hash[86] = (tmp[21] >> 8) & 0xff;
  hash[87] = (tmp[21] >> 0) & 0xff;
  hash[88] = (tmp[22] >> 24) & 0xff;
  hash[89] = (tmp[22] >> 16) & 0xff;
  hash[90] = (tmp[22] >> 8) & 0xff;
  hash[91] = (tmp[22] >> 0) & 0xff;
  hash[92] = (tmp[23] >> 24) & 0xff;
  hash[93] = (tmp[23] >> 16) & 0xff;
  hash[94] = (tmp[23] >> 8) & 0xff;
  hash[95] = (tmp[23] >> 0) & 0xff;

  hash[96] = (tmp[24] >> 24) & 0xff;
  hash[97] = (tmp[24] >> 16) & 0xff;
  hash[98] = (tmp[24] >> 8) & 0xff;
  hash[99] = (tmp[24] >> 0) & 0xff;
  hash[100] = (tmp[25] >> 24) & 0xff;
  hash[101] = (tmp[25] >> 16) & 0xff;
  hash[102] = (tmp[25] >> 8) & 0xff;
  hash[103] = (tmp[25] >> 0) & 0xff;
  hash[104] = (tmp[26] >> 24) & 0xff;
  hash[105] = (tmp[26] >> 16) & 0xff;
  hash[106] = (tmp[26] >> 8) & 0xff;
  hash[107] = (tmp[26] >> 0) & 0xff;
  hash[108] = (tmp[27] >> 24) & 0xff;
  hash[109] = (tmp[27] >> 16) & 0xff;
  hash[110] = (tmp[27] >> 8) & 0xff;
  hash[111] = (tmp[27] >> 0) & 0xff;
  hash[112] = (tmp[28] >> 24) & 0xff;
  hash[113] = (tmp[28] >> 16) & 0xff;
  hash[114] = (tmp[28] >> 8) & 0xff;
  hash[115] = (tmp[28] >> 0) & 0xff;
  hash[116] = (tmp[29] >> 24) & 0xff;
  hash[117] = (tmp[29] >> 16) & 0xff;
  hash[118] = (tmp[29] >> 8) & 0xff;
  hash[119] = (tmp[29] >> 0) & 0xff;
  hash[120] = (tmp[30] >> 24) & 0xff;
  hash[121] = (tmp[30] >> 16) & 0xff;
  hash[122] = (tmp[30] >> 8) & 0xff;
  hash[123] = (tmp[30] >> 0) & 0xff;
  hash[124] = (tmp[31] >> 24) & 0xff;
  hash[125] = (tmp[31] >> 16) & 0xff;
  hash[126] = (tmp[31] >> 8) & 0xff;
  hash[127] = (tmp[31] >> 0) & 0xff;
}

void
snippets_fnv1a_1024 (const uint8_t * data, size_t len, uint8_t hash[128])
{
  size_t i;
  uint64_t tmp[32], tmp2[32];

  /* 1024 bit number, 32 bit per field, highest 32 bit first */
  tmp[0] = 0x00000000;
  tmp[1] = 0x00000000;
  tmp[2] = 0x005f7a76;
  tmp[3] = 0x758ecc4d;
  tmp[4] = 0x32e56d5a;
  tmp[5] = 0x591028b7;
  tmp[6] = 0x4b29fc42;
  tmp[7] = 0x23fdada1;
  tmp[8] = 0x6c3bf34e;
  tmp[9] = 0xda3674da;
  tmp[10] = 0x9a21d900;
  tmp[11] = 0x00000000;
  tmp[12] = 0x00000000;
  tmp[13] = 0x00000000;
  tmp[14] = 0x00000000;
  tmp[15] = 0x00000000;
  tmp[16] = 0x00000000;
  tmp[17] = 0x00000000;
  tmp[18] = 0x00000000;
  tmp[19] = 0x00000000;
  tmp[20] = 0x00000000;
  tmp[21] = 0x00000000;
  tmp[22] = 0x00000000;
  tmp[23] = 0x0004c6d7;
  tmp[24] = 0xeb6e7380;
  tmp[25] = 0x2734510a;
  tmp[26] = 0x555f256c;
  tmp[27] = 0xc005ae55;
  tmp[28] = 0x6bde8cc9;
  tmp[29] = 0xc6a93b21;
  tmp[30] = 0xaff4b16c;
  tmp[31] = 0x71ee90b3;

  for (i = 0; i < len; i++) {
    /* Multiplication */

    /* lowest 32 bits */

    /* multiply and keep carries forward */
    tmp2[31] = (*data ^ tmp[31]) * 0x0000018d;
    tmp2[30] = tmp[30] * 0x0000018d + (tmp2[31] >> 32);
    tmp2[29] = tmp[29] * 0x0000018d + (tmp2[30] >> 32);
    tmp2[28] = tmp[28] * 0x0000018d + (tmp2[29] >> 32);
    tmp2[27] = tmp[27] * 0x0000018d + (tmp2[28] >> 32);
    tmp2[26] = tmp[26] * 0x0000018d + (tmp2[27] >> 32);
    tmp2[25] = tmp[25] * 0x0000018d + (tmp2[26] >> 32);
    tmp2[24] = tmp[24] * 0x0000018d + (tmp2[25] >> 32);
    tmp2[23] = tmp[23] * 0x0000018d + (tmp2[24] >> 32);
    tmp2[22] = tmp[22] * 0x0000018d + (tmp2[23] >> 32);
    tmp2[21] = tmp[21] * 0x0000018d + (tmp2[22] >> 32);
    tmp2[20] = tmp[20] * 0x0000018d + (tmp2[21] >> 32);
    tmp2[19] = tmp[19] * 0x0000018d + (tmp2[20] >> 32);
    tmp2[18] = tmp[18] * 0x0000018d + (tmp2[19] >> 32);
    tmp2[17] = tmp[17] * 0x0000018d + (tmp2[18] >> 32);
    tmp2[16] = tmp[16] * 0x0000018d + (tmp2[17] >> 32);
    tmp2[15] = tmp[15] * 0x0000018d + (tmp2[16] >> 32);
    tmp2[14] = tmp[14] * 0x0000018d + (tmp2[15] >> 32);
    tmp2[13] = tmp[13] * 0x0000018d + (tmp2[14] >> 32);
    tmp2[12] = tmp[12] * 0x0000018d + (tmp2[13] >> 32);
    tmp2[11] = tmp[11] * 0x0000018d + (tmp2[12] >> 32);
    tmp2[10] = tmp[10] * 0x0000018d + (tmp2[11] >> 32);
    tmp2[9] = tmp[9] * 0x0000018d + (tmp2[10] >> 32);
    tmp2[8] = tmp[8] * 0x0000018d + (tmp2[9] >> 32);
    tmp2[7] = tmp[7] * 0x0000018d + (tmp2[8] >> 32);
    tmp2[6] = tmp[6] * 0x0000018d + (tmp2[7] >> 32);
    tmp2[5] = tmp[5] * 0x0000018d + (tmp2[6] >> 32);
    tmp2[4] = tmp[4] * 0x0000018d + (tmp2[5] >> 32);
    tmp2[3] = tmp[3] * 0x0000018d + (tmp2[4] >> 32);
    tmp2[2] = tmp[2] * 0x0000018d + (tmp2[3] >> 32);
    tmp2[1] = tmp[1] * 0x0000018d + (tmp2[2] >> 32);
    tmp2[0] = tmp[0] * 0x0000018d + (tmp2[1] >> 32);
    /* drop carries */
    tmp2[31] &= 0xffffffff;
    tmp2[30] &= 0xffffffff;
    tmp2[29] &= 0xffffffff;
    tmp2[28] &= 0xffffffff;
    tmp2[27] &= 0xffffffff;
    tmp2[26] &= 0xffffffff;
    tmp2[25] &= 0xffffffff;
    tmp2[24] &= 0xffffffff;
    tmp2[23] &= 0xffffffff;
    tmp2[22] &= 0xffffffff;
    tmp2[21] &= 0xffffffff;
    tmp2[20] &= 0xffffffff;
    tmp2[19] &= 0xffffffff;
    tmp2[18] &= 0xffffffff;
    tmp2[17] &= 0xffffffff;
    tmp2[16] &= 0xffffffff;
    tmp2[15] &= 0xffffffff;
    tmp2[14] &= 0xffffffff;
    tmp2[13] &= 0xffffffff;
    tmp2[12] &= 0xffffffff;
    tmp2[11] &= 0xffffffff;
    tmp2[10] &= 0xffffffff;
    tmp2[9] &= 0xffffffff;
    tmp2[8] &= 0xffffffff;
    tmp2[7] &= 0xffffffff;
    tmp2[6] &= 0xffffffff;
    tmp2[5] &= 0xffffffff;
    tmp2[4] &= 0xffffffff;
    tmp2[3] &= 0xffffffff;
    tmp2[2] &= 0xffffffff;
    tmp2[1] &= 0xffffffff;
    tmp2[0] &= 0xffffffff;

    /* highest 32 bits */

    /* multiply and keep carries forward */
    tmp2[10] += tmp[31] * 0x00000100;
    tmp2[9] += tmp[30] * 0x00000100 + (tmp2[10] >> 32);
    tmp2[8] += tmp[29] * 0x00000100 + (tmp2[9] >> 32);
    tmp2[7] += tmp[28] * 0x00000100 + (tmp2[8] >> 32);
    tmp2[6] += tmp[27] * 0x00000100 + (tmp2[7] >> 32);
    tmp2[5] += tmp[26] * 0x00000100 + (tmp2[6] >> 32);
    tmp2[4] += tmp[25] * 0x00000100 + (tmp2[5] >> 32);
    tmp2[3] += tmp[24] * 0x00000100 + (tmp2[4] >> 32);
    tmp2[2] += tmp[23] * 0x00000100 + (tmp2[3] >> 32);
    tmp2[1] += tmp[22] * 0x00000100 + (tmp2[2] >> 32);
    tmp2[0] += tmp[21] * 0x00000100 + (tmp2[1] >> 32);
    /* drop carries */
    tmp2[10] &= 0xffffffff;
    tmp2[9] &= 0xffffffff;
    tmp2[8] &= 0xffffffff;
    tmp2[7] &= 0xffffffff;
    tmp2[6] &= 0xffffffff;
    tmp2[5] &= 0xffffffff;
    tmp2[4] &= 0xffffffff;
    tmp2[3] &= 0xffffffff;
    tmp2[2] &= 0xffffffff;
    tmp2[1] &= 0xffffffff;
    tmp2[0] &= 0xffffffff;

    tmp[31] = tmp2[31];
    tmp[30] = tmp2[30];
    tmp[29] = tmp2[29];
    tmp[28] = tmp2[28];
    tmp[27] = tmp2[27];
    tmp[26] = tmp2[26];
    tmp[25] = tmp2[25];
    tmp[24] = tmp2[24];
    tmp[23] = tmp2[23];
    tmp[22] = tmp2[22];
    tmp[21] = tmp2[21];
    tmp[20] = tmp2[20];
    tmp[19] = tmp2[19];
    tmp[18] = tmp2[18];
    tmp[17] = tmp2[17];
    tmp[16] = tmp2[16];
    tmp[15] = tmp2[15];
    tmp[14] = tmp2[14];
    tmp[13] = tmp2[13];
    tmp[12] = tmp2[12];
    tmp[11] = tmp2[11];
    tmp[10] = tmp2[10];
    tmp[9] = tmp2[9];
    tmp[8] = tmp2[8];
    tmp[7] = tmp2[7];
    tmp[6] = tmp2[6];
    tmp[5] = tmp2[5];
    tmp[4] = tmp2[4];
    tmp[3] = tmp2[3];
    tmp[2] = tmp2[2];
    tmp[1] = tmp2[1];
    tmp[0] = tmp2[0];

    data++;
  }

  hash[0] = (tmp[0] >> 24) & 0xff;
  hash[1] = (tmp[0] >> 16) & 0xff;
  hash[2] = (tmp[0] >> 8) & 0xff;
  hash[3] = (tmp[0] >> 0) & 0xff;
  hash[4] = (tmp[1] >> 24) & 0xff;
  hash[5] = (tmp[1] >> 16) & 0xff;
  hash[6] = (tmp[1] >> 8) & 0xff;
  hash[7] = (tmp[1] >> 0) & 0xff;
  hash[8] = (tmp[2] >> 24) & 0xff;
  hash[9] = (tmp[2] >> 16) & 0xff;
  hash[10] = (tmp[2] >> 8) & 0xff;
  hash[11] = (tmp[2] >> 0) & 0xff;
  hash[12] = (tmp[3] >> 24) & 0xff;
  hash[13] = (tmp[3] >> 16) & 0xff;
  hash[14] = (tmp[3] >> 8) & 0xff;
  hash[15] = (tmp[3] >> 0) & 0xff;
  hash[16] = (tmp[4] >> 24) & 0xff;
  hash[17] = (tmp[4] >> 16) & 0xff;
  hash[18] = (tmp[4] >> 8) & 0xff;
  hash[19] = (tmp[4] >> 0) & 0xff;
  hash[20] = (tmp[5] >> 24) & 0xff;
  hash[21] = (tmp[5] >> 16) & 0xff;
  hash[22] = (tmp[5] >> 8) & 0xff;
  hash[23] = (tmp[5] >> 0) & 0xff;
  hash[24] = (tmp[6] >> 24) & 0xff;
  hash[25] = (tmp[6] >> 16) & 0xff;
  hash[26] = (tmp[6] >> 8) & 0xff;
  hash[27] = (tmp[6] >> 0) & 0xff;
  hash[28] = (tmp[7] >> 24) & 0xff;
  hash[29] = (tmp[7] >> 16) & 0xff;
  hash[30] = (tmp[7] >> 8) & 0xff;
  hash[31] = (tmp[7] >> 0) & 0xff;

  hash[32] = (tmp[8] >> 24) & 0xff;
  hash[33] = (tmp[8] >> 16) & 0xff;
  hash[34] = (tmp[8] >> 8) & 0xff;
  hash[35] = (tmp[8] >> 0) & 0xff;
  hash[36] = (tmp[9] >> 24) & 0xff;
  hash[37] = (tmp[9] >> 16) & 0xff;
  hash[38] = (tmp[9] >> 8) & 0xff;
  hash[39] = (tmp[9] >> 0) & 0xff;
  hash[40] = (tmp[10] >> 24) & 0xff;
  hash[41] = (tmp[10] >> 16) & 0xff;
  hash[42] = (tmp[10] >> 8) & 0xff;
  hash[43] = (tmp[10] >> 0) & 0xff;
  hash[44] = (tmp[11] >> 24) & 0xff;
  hash[45] = (tmp[11] >> 16) & 0xff;
  hash[46] = (tmp[11] >> 8) & 0xff;
  hash[47] = (tmp[11] >> 0) & 0xff;
  hash[48] = (tmp[12] >> 24) & 0xff;
  hash[49] = (tmp[12] >> 16) & 0xff;
  hash[50] = (tmp[12] >> 8) & 0xff;
  hash[51] = (tmp[12] >> 0) & 0xff;
  hash[52] = (tmp[13] >> 24) & 0xff;
  hash[53] = (tmp[13] >> 16) & 0xff;
  hash[54] = (tmp[13] >> 8) & 0xff;
  hash[55] = (tmp[13] >> 0) & 0xff;
  hash[56] = (tmp[14] >> 24) & 0xff;
  hash[57] = (tmp[14] >> 16) & 0xff;
  hash[58] = (tmp[14] >> 8) & 0xff;
  hash[59] = (tmp[14] >> 0) & 0xff;
  hash[60] = (tmp[15] >> 24) & 0xff;
  hash[61] = (tmp[15] >> 16) & 0xff;
  hash[62] = (tmp[15] >> 8) & 0xff;
  hash[63] = (tmp[15] >> 0) & 0xff;

  hash[64] = (tmp[16] >> 24) & 0xff;
  hash[65] = (tmp[16] >> 16) & 0xff;
  hash[66] = (tmp[16] >> 8) & 0xff;
  hash[67] = (tmp[16] >> 0) & 0xff;
  hash[68] = (tmp[17] >> 24) & 0xff;
  hash[69] = (tmp[17] >> 16) & 0xff;
  hash[70] = (tmp[17] >> 8) & 0xff;
  hash[71] = (tmp[17] >> 0) & 0xff;
  hash[72] = (tmp[18] >> 24) & 0xff;
  hash[73] = (tmp[18] >> 16) & 0xff;
  hash[74] = (tmp[18] >> 8) & 0xff;
  hash[75] = (tmp[18] >> 0) & 0xff;
  hash[76] = (tmp[19] >> 24) & 0xff;
  hash[77] = (tmp[19] >> 16) & 0xff;
  hash[78] = (tmp[19] >> 8) & 0xff;
  hash[79] = (tmp[19] >> 0) & 0xff;
  hash[80] = (tmp[20] >> 24) & 0xff;
  hash[81] = (tmp[20] >> 16) & 0xff;
  hash[82] = (tmp[20] >> 8) & 0xff;
  hash[83] = (tmp[20] >> 0) & 0xff;
  hash[84] = (tmp[21] >> 24) & 0xff;
  hash[85] = (tmp[21] >> 16) & 0xff;
  hash[86] = (tmp[21] >> 8) & 0xff;
  hash[87] = (tmp[21] >> 0) & 0xff;
  hash[88] = (tmp[22] >> 24) & 0xff;
  hash[89] = (tmp[22] >> 16) & 0xff;
  hash[90] = (tmp[22] >> 8) & 0xff;
  hash[91] = (tmp[22] >> 0) & 0xff;
  hash[92] = (tmp[23] >> 24) & 0xff;
  hash[93] = (tmp[23] >> 16) & 0xff;
  hash[94] = (tmp[23] >> 8) & 0xff;
  hash[95] = (tmp[23] >> 0) & 0xff;

  hash[96] = (tmp[24] >> 24) & 0xff;
  hash[97] = (tmp[24] >> 16) & 0xff;
  hash[98] = (tmp[24] >> 8) & 0xff;
  hash[99] = (tmp[24] >> 0) & 0xff;
  hash[100] = (tmp[25] >> 24) & 0xff;
  hash[101] = (tmp[25] >> 16) & 0xff;
  hash[102] = (tmp[25] >> 8) & 0xff;
  hash[103] = (tmp[25] >> 0) & 0xff;
  hash[104] = (tmp[26] >> 24) & 0xff;
  hash[105] = (tmp[26] >> 16) & 0xff;
  hash[106] = (tmp[26] >> 8) & 0xff;
  hash[107] = (tmp[26] >> 0) & 0xff;
  hash[108] = (tmp[27] >> 24) & 0xff;
  hash[109] = (tmp[27] >> 16) & 0xff;
  hash[110] = (tmp[27] >> 8) & 0xff;
  hash[111] = (tmp[27] >> 0) & 0xff;
  hash[112] = (tmp[28] >> 24) & 0xff;
  hash[113] = (tmp[28] >> 16) & 0xff;
  hash[114] = (tmp[28] >> 8) & 0xff;
  hash[115] = (tmp[28] >> 0) & 0xff;
  hash[116] = (tmp[29] >> 24) & 0xff;
  hash[117] = (tmp[29] >> 16) & 0xff;
  hash[118] = (tmp[29] >> 8) & 0xff;
  hash[119] = (tmp[29] >> 0) & 0xff;
  hash[120] = (tmp[30] >> 24) & 0xff;
  hash[121] = (tmp[30] >> 16) & 0xff;
  hash[122] = (tmp[30] >> 8) & 0xff;
  hash[123] = (tmp[30] >> 0) & 0xff;
  hash[124] = (tmp[31] >> 24) & 0xff;
  hash[125] = (tmp[31] >> 16) & 0xff;
  hash[126] = (tmp[31] >> 8) & 0xff;
  hash[127] = (tmp[31] >> 0) & 0xff;
}
