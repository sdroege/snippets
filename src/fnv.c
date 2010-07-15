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

#include "fnv.h"

/* Implementations of the Fowler–Noll–Vo hash function.
 * This implements FNV 1 and 1A for various bit depths
 *
 * For more details see:
 * http://www.isthe.com/chongo/tech/comp/fnv/index.html
 */

static const uint32_t FNV_prime_32 = 16777619U;
static const uint32_t FNV_offset_32 = 2166136261U;

void
fnv1_32 (const uint8_t *data, size_t len, uint8_t hash[4])
{
  size_t i;
  uint32_t tmp = FNV_offset_32;

  for (i = 0; i < len; i++) {
    tmp = tmp * FNV_prime_32; /* Multiplication mod 2^32 as per C standard */
    tmp = tmp ^ *data;
    data++;
  }
  
  hash[0] = (tmp >> 24) & 0xff;
  hash[1] = (tmp >> 16) & 0xff;
  hash[2] = (tmp >> 8) & 0xff;
  hash[3] = (tmp >> 0) & 0xff;
}

void
fnv1a_32 (const uint8_t *data, size_t len, uint8_t hash[4])
{
  size_t i;
  uint32_t tmp = FNV_offset_32;

  for (i = 0; i < len; i++) {
    tmp = tmp ^ *data;
    tmp = tmp * FNV_prime_32; /* Multiplication mod 2^32 as per C standard */
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
fnv1_64 (const uint8_t *data, size_t len, uint8_t hash[8])
{
  size_t i;
  uint64_t tmp = FNV_offset_64;

  for (i = 0; i < len; i++) {
    tmp = tmp * FNV_prime_64; /* Multiplication mod 2^64 as per C standard */
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
fnv1a_64 (const uint8_t *data, size_t len, uint8_t hash[8])
{
  size_t i;
  uint64_t tmp = FNV_offset_64;

  for (i = 0; i < len; i++) {
    tmp = tmp ^ *data;
    tmp = tmp * FNV_prime_64; /* Multiplication mod 2^64 as per C standard */
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
fnv1_128 (const uint8_t *data, size_t len, uint8_t hash[16])
{
  size_t i;
  uint64_t tmp[4], tmp2[7];

  /* 128 bit number, 32 bit per field, highest 32 bit first */
  tmp[0] = 0x6c62272e;
  tmp[1] = 0x07bb0142;
  tmp[2] = 0x62b82175;
  tmp[3] = 0x6295c58d;

  for (i = 0; i < len; i++) {
    /* Multiplication */

    /* lowest 32 bits */

    /* multiply and keep carries forward */
    tmp2[6] = tmp[3] * 0x0000013b;
    tmp2[5] = tmp[2] * 0x0000013b + (tmp2[6] >> 32);
    tmp2[4] = tmp[1] * 0x0000013b + (tmp2[5] >> 32);
    tmp2[3] = tmp[0] * 0x0000013b + (tmp2[4] >> 32);
    tmp2[2] = /* 0 + */ tmp2[3] >> 32;
    /* drop carries */
    tmp2[6] &= 0xffffffff;
    tmp2[5] &= 0xffffffff;
    tmp2[4] &= 0xffffffff;
    tmp2[3] &= 0xffffffff;

    /* highest 32 bits */

    /* multiply and keep carries forward */
    tmp2[4] += tmp[3] * 0x01000000;
    tmp2[3] += tmp[2] * 0x01000000 + (tmp2[4] >> 32);
#if 0
    /* For completeness */
    tmp2[2] += tmp[1] * 0x01000000 + (tmp2[3] >> 32);
    tmp2[1] += tmp[0] * 0x01000000 + (tmp2[2] >> 32);
    tmp2[0] = /* 0 + */ tmp2[1] >> 32;
#endif
    /* drop carries */
    tmp2[4] &= 0xffffffff;
    tmp2[3] &= 0xffffffff;
#if 0
    /* For completeness */
    tmp2[2] &= 0xffffffff;
    tmp2[1] &= 0xffffffff;
#endif

    tmp[3] = tmp2[6] ^ *data;
    tmp[2] = tmp2[5];
    tmp[1] = tmp2[4];
    tmp[0] = tmp2[3];

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
fnv1a_128 (const uint8_t *data, size_t len, uint8_t hash[16])
{
  size_t i;
  uint64_t tmp[4], tmp2[7];

  /* 128 bit number, 32 bit per field, highest 32 bit first */
  tmp[0] = 0x6c62272e;
  tmp[1] = 0x07bb0142;
  tmp[2] = 0x62b82175;
  tmp[3] = 0x6295c58d;

  for (i = 0; i < len; i++) {
    /* Multiplication */

    /* lowest 32 bits */

    /* multiply and keep carries forward */
    tmp2[6] = (*data ^ tmp[3]) * 0x0000013b;
    tmp2[5] = tmp[2] * 0x0000013b + (tmp2[6] >> 32);
    tmp2[4] = tmp[1] * 0x0000013b + (tmp2[5] >> 32);
    tmp2[3] = tmp[0] * 0x0000013b + (tmp2[4] >> 32);
    tmp2[2] = /* 0 + */ tmp2[3] >> 32;
    /* drop carries */
    tmp2[6] &= 0xffffffff;
    tmp2[5] &= 0xffffffff;
    tmp2[4] &= 0xffffffff;
    tmp2[3] &= 0xffffffff;

    /* highest 32 bits */

    /* multiply and keep carries forward */
    tmp2[4] += tmp[3] * 0x01000000;
    tmp2[3] += tmp[2] * 0x01000000 + (tmp2[4] >> 32);
#if 0
    /* For completeness */
    tmp2[2] += tmp[1] * 0x01000000 + (tmp2[3] >> 32);
    tmp2[1] += tmp[0] * 0x01000000 + (tmp2[2] >> 32);
    tmp2[0] = /* 0 + */ tmp2[1] >> 32;
#endif
    /* drop carries */
    tmp2[4] &= 0xffffffff;
    tmp2[3] &= 0xffffffff;
#if 0
    /* For completeness */
    tmp2[2] &= 0xffffffff;
    tmp2[1] &= 0xffffffff;
#endif

    tmp[3] = tmp2[6];
    tmp[2] = tmp2[5];
    tmp[1] = tmp2[4];
    tmp[0] = tmp2[3];

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

/*
 * 00000000
 * 00000000
 * 00000100
 * 00000000
 * 00000000000000000000000000000163
 */
void
fnv1_256 (const uint8_t *data, size_t len, uint8_t hash[32])
{
  size_t i;
  uint64_t tmp[8], tmp2[15];

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
    tmp2[14] = tmp[7] * 0x00000163;
    tmp2[13] = tmp[6] * 0x00000163 + (tmp2[14] >> 32);
    tmp2[12] = tmp[5] * 0x00000163 + (tmp2[13] >> 32);
    tmp2[11] = tmp[4] * 0x00000163 + (tmp2[12] >> 32);
    tmp2[10] = tmp[3] * 0x00000163 + (tmp2[11] >> 32);
    tmp2[9] = tmp[2] * 0x00000163 + (tmp2[10] >> 32);
    tmp2[8] = tmp[1] * 0x00000163 + (tmp2[9] >> 32);
    tmp2[7] = tmp[0] * 0x00000163 + (tmp2[8] >> 32);
    tmp2[6] = /* 0 + */ tmp2[7] >> 32;
    /* drop carries */
    tmp2[14] &= 0xffffffff;
    tmp2[13] &= 0xffffffff;
    tmp2[12] &= 0xffffffff;
    tmp2[11] &= 0xffffffff;
    tmp2[10] &= 0xffffffff;
    tmp2[9] &= 0xffffffff;
    tmp2[8] &= 0xffffffff;
    tmp2[7] &= 0xffffffff;

    /* highest 32 bits */

    /* multiply and keep carries forward */
    tmp2[9] += tmp[7] * 0x00000100;
    tmp2[8] += tmp[6] * 0x00000100 + (tmp2[9] >> 32);
    tmp2[7] += tmp[5] * 0x00000100 + (tmp2[8] >> 32);

    /* drop carries */
    tmp2[9] &= 0xffffffff;
    tmp2[8] &= 0xffffffff;
    tmp2[7] &= 0xffffffff;

    tmp[7] = tmp2[14] ^ *data;
    tmp[6] = tmp2[13];
    tmp[5] = tmp2[12];
    tmp[4] = tmp2[11];
    tmp[3] = tmp2[10];
    tmp[2] = tmp2[9];
    tmp[1] = tmp2[8];
    tmp[0] = tmp2[7];

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
fnv1a_256 (const uint8_t *data, size_t len, uint8_t hash[32])
{
  size_t i;
  uint64_t tmp[8], tmp2[15];

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
    tmp2[14] = (tmp[7] ^ *data) * 0x00000163;
    tmp2[13] = tmp[6] * 0x00000163 + (tmp2[14] >> 32);
    tmp2[12] = tmp[5] * 0x00000163 + (tmp2[13] >> 32);
    tmp2[11] = tmp[4] * 0x00000163 + (tmp2[12] >> 32);
    tmp2[10] = tmp[3] * 0x00000163 + (tmp2[11] >> 32);
    tmp2[9] = tmp[2] * 0x00000163 + (tmp2[10] >> 32);
    tmp2[8] = tmp[1] * 0x00000163 + (tmp2[9] >> 32);
    tmp2[7] = tmp[0] * 0x00000163 + (tmp2[8] >> 32);
    tmp2[6] = /* 0 + */ tmp2[7] >> 32;
    /* drop carries */
    tmp2[14] &= 0xffffffff;
    tmp2[13] &= 0xffffffff;
    tmp2[12] &= 0xffffffff;
    tmp2[11] &= 0xffffffff;
    tmp2[10] &= 0xffffffff;
    tmp2[9] &= 0xffffffff;
    tmp2[8] &= 0xffffffff;
    tmp2[7] &= 0xffffffff;

    /* highest 32 bits */

    /* multiply and keep carries forward */
    tmp2[9] += tmp[7] * 0x00000100;
    tmp2[8] += tmp[6] * 0x00000100 + (tmp2[9] >> 32);
    tmp2[7] += tmp[5] * 0x00000100 + (tmp2[8] >> 32);

    /* drop carries */
    tmp2[9] &= 0xffffffff;
    tmp2[8] &= 0xffffffff;
    tmp2[7] &= 0xffffffff;

    tmp[7] = tmp2[14];
    tmp[6] = tmp2[13];
    tmp[5] = tmp2[12];
    tmp[4] = tmp2[11];
    tmp[3] = tmp2[10];
    tmp[2] = tmp2[9];
    tmp[1] = tmp2[8];
    tmp[0] = tmp2[7];

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
