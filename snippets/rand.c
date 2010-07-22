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

#include <snippets/rand.h>

#include <assert.h>

#define N 624

struct _SnippetsRand
{
  uint32_t mt[N];
  unsigned int mti;
};

#include "mt19937.c"

SnippetsRand *
snippets_rand_new (uint32_t seed)
{
  SnippetsRand *rand = calloc (sizeof (SnippetsRand), 1);

  mt19937_init (rand, seed);

  return rand;
}

void
snippets_rand_free (SnippetsRand * rand)
{
  assert (rand != NULL);
  free (rand);
}

uint32_t
snippets_rand_uint32 (SnippetsRand * rand)
{
  assert (rand != NULL);

  return mt19937_genrand_uint32 (rand);
}

uint32_t
snippets_rand_uint32_range (SnippetsRand * rand, uint32_t min, uint32_t max)
{
  uint64_t a;

  assert (rand != NULL);

  a = mt19937_genrand_uint32 (rand);
  a = (a * (max - min)) / 0xffffffff + min;

  return (uint32_t) a;
}

#define DOUBLE_TRANSFORM 2.3283064365386962890625e-10
double
snippets_rand_double (SnippetsRand * rand)
{
  double a;

  assert (rand != NULL);

  a = mt19937_genrand_uint32 (rand) * DOUBLE_TRANSFORM;
  a = (a + mt19937_genrand_uint32 (rand)) * DOUBLE_TRANSFORM;

  /* a >= 1.0 might happen due to rare rounding errors */
  return (a >= 1.0) ? snippets_rand_double (rand) : a;
}

double
snippets_rand_double_range (SnippetsRand * rand, double min, double max)
{
  assert (rand != NULL);

  return snippets_rand_double (rand) * (max - min) + min;
}
