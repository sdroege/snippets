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
  SnippetsRandMode mode;
  uint32_t mt[N];
  unsigned int mti;

    uint32_t (*genrand) (SnippetsRand * rand);
};

#include "mt19937.c"

static void
lcg_init (SnippetsRand * rand, uint32_t seed)
{
  rand->mt[N - 1] = seed;
  rand->mti = N;
}

static uint32_t
lcg_genrand_uint32 (SnippetsRand * rand)
{
  uint32_t *mt = rand->mt;
  unsigned int mti = rand->mti;
  uint32_t y;

  if (mti >= N) {
    mt[0] = mt[N - 1] * 1103515245 + 12345;
    for (mti = 1; mti < N; mti++)
      mt[mti] = mt[mti - 1] * 1103515245 + 12345;
    mti = 0;
  }

  y = mt[mti++];
  rand->mti = mti;

  return y;
}

SnippetsRand *
snippets_rand_new (SnippetsRandMode mode, uint32_t seed)
{
  SnippetsRand *rand = calloc (sizeof (SnippetsRand), 1);

  rand->mode = mode;

  switch (mode) {
    default:
    case SNIPPETS_RAND_MODE_MT19937:
      mt19937_init (rand, seed);
      rand->genrand = mt19937_genrand_uint32;
      break;
    case SNIPPETS_RAND_MODE_LINEAR_CONGRUENTIAL_GENERATOR:
      lcg_init (rand, seed);
      rand->genrand = lcg_genrand_uint32;
      break;
  }

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

  return rand->genrand (rand);
}

uint32_t
snippets_rand_uint32_range (SnippetsRand * rand, uint32_t min, uint32_t max)
{
  uint64_t a;

  assert (rand != NULL);

  a = rand->genrand (rand);
  a = (a * (max - min)) / 0xffffffff + min;

  return (uint32_t) a;
}

#define DOUBLE_TRANSFORM 2.3283064365386962890625e-10
double
snippets_rand_double (SnippetsRand * rand)
{
  double a;

  assert (rand != NULL);

  a = rand->genrand (rand) * DOUBLE_TRANSFORM;
  a = (a + rand->genrand (rand)) * DOUBLE_TRANSFORM;

  /* a >= 1.0 might happen due to rare rounding errors */
  return (a >= 1.0) ? snippets_rand_double (rand) : a;
}

double
snippets_rand_double_range (SnippetsRand * rand, double min, double max)
{
  assert (rand != NULL);

  return snippets_rand_double (rand) * (max - min) + min;
}
