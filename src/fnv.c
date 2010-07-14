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

/* Implementations of FNV 1 and 1A for various bit depths
 * http://www.isthe.com/chongo/tech/comp/fnv/index.html
 */

static const uint32_t FNV_prime_32 = 16777619;
static const uint32_t FNV_offset_32 = 2166136261;

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

/* 128 bit prime = 309485009821345068724781371 = 0x1000000000000000000013b
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

