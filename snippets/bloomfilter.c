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

#include <snippets/bloomfilter.h>
#include <snippets/fnv.h>

#include <math.h>

#ifndef M_LN2
#define M_LN2 0.69314718055994530942
#endif

#include <assert.h>

/* This implements a bloom filter, see
 *
 * http://en.wikipedia.org/wiki/Bloom_filter
 * or
 * "Space/Time Trade-offs in Hash Coding with Allowable Errors"
 * by Burton H. Bloom in Communications of the ACM Volume 13,
 * Number 7 (1970).
 * 
 * The hashing implemented here is the enhanced double hashing
 * as proposed in "Bloom Filters in Probabilistic Verification"
 * by Peter C. Dillinger and Panagiotis Manolios.
 *
 * The formula for the optimal number of hash functions and
 * the false positive probability comes from their paper too.
 */

struct _SnippetsBloomFilter
{
  uint8_t *filter;
  uint32_t size;
  unsigned int n_hash_functions;
  unsigned int hash_size;

  void (*hash) (const uint8_t * data, size_t size, uint8_t * hash);
};

SnippetsBloomFilter *
snippets_bloom_filter_new (uint32_t size, unsigned int n_hash_functions,
    unsigned int hash_size)
{
  SnippetsBloomFilter *filter;

  assert (size > 0);
  assert (n_hash_functions > 0);
  assert (hash_size > 0 && hash_size <= 1024);

  /* Round up to next byte multiple */
  size = (((size) + 7) & ~7);

  filter = calloc (sizeof (SnippetsBloomFilter) + size / 8, 1);

  if (hash_size <= 64) {
    filter->hash_size = 64;
    filter->hash = snippets_fnv1a_64;
  } else if (hash_size <= 128) {
    filter->hash_size = 128;
    filter->hash = snippets_fnv1a_128;
  } else if (hash_size <= 256) {
    filter->hash_size = 256;
    filter->hash = snippets_fnv1a_256;
  } else if (hash_size <= 512) {
    filter->hash_size = 512;
    filter->hash = snippets_fnv1a_512;
  } else if (hash_size <= 1024) {
    filter->hash_size = 1024;
    filter->hash = snippets_fnv1a_1024;
  }

  filter->filter = ((uint8_t *) filter) + sizeof (SnippetsBloomFilter);
  filter->size = size;
  filter->n_hash_functions = n_hash_functions;

  return filter;
}

#define SET_BIT(filter, bit) (filter[bit / 8] |= (1 << (bit % 8)))
#define GET_BIT(filter, bit) (filter[bit / 8] & (1 << (bit % 8)))

static int
snippets_bloom_filter_hash (SnippetsBloomFilter * filter, const uint8_t * data,
    size_t length, int set)
{
  uint32_t hash[32];
  unsigned int n_hash_functions, first_n_hash_functions;
  unsigned int hash_index;
  unsigned int hash_values;
  unsigned int functions_per_value;
  unsigned int last_functions;
  uint32_t x, y;
  uint32_t size;
  int i, j;

  size = filter->size;

  n_hash_functions = filter->n_hash_functions;
  hash_values = filter->hash_size / 32;

  if (n_hash_functions <= hash_values / 2) {
    first_n_hash_functions = n_hash_functions;
    functions_per_value = 1;
    last_functions = 0;
  } else {
    functions_per_value = n_hash_functions / (hash_values / 2);
    last_functions = functions_per_value + n_hash_functions % (hash_values / 2);

    if (last_functions == functions_per_value)
      last_functions = 0;
    first_n_hash_functions = n_hash_functions - last_functions;
  }

  filter->hash (data, length, (uint8_t *) hash);

  hash_index = 0;

  /* If there are less hash functions than hash value pairs
   * or the number of hash functions is an integer multiple
   * of the number of hsah value pairs: Handle all hash
   * functions in this loop.
   *
   * Otherwise handle all hash functions in this loop
   * except the ones for the last hash value pair */
  for (i = 0; i < first_n_hash_functions; i += functions_per_value) {
    x = hash[hash_index] % size;
    y = hash[hash_index + hash_values / 2] % size;

    if (functions_per_value == 1)
      x = (x + y) % size;

    if (set)
      SET_BIT (filter->filter, x);
    else if (!GET_BIT (filter->filter, x))
      return FALSE;

    for (j = 0; j < functions_per_value - 1; j++) {
      x = (x + y) % size;
      y = (y + i) % size;

      if (set)
        SET_BIT (filter->filter, x);
      else if (!GET_BIT (filter->filter, x))
        return FALSE;
    }
    hash_index++;
  }

  /* If there are hash functions left handle
   * them here
   */
  if (last_functions) {
    x = hash[hash_index] % size;
    y = hash[hash_index + hash_values / 2] % size;

    if (set)
      SET_BIT (filter->filter, x);
    else if (!GET_BIT (filter->filter, x))
      return FALSE;

    for (j = 0; j < last_functions - 1; j++) {
      x = (x + y) % size;
      y = (y + i) % size;

      if (set)
        SET_BIT (filter->filter, x);
      else if (!GET_BIT (filter->filter, x))
        return FALSE;
    }
  }

  return TRUE;
}

void
snippets_bloom_filter_insert (SnippetsBloomFilter * filter,
    const uint8_t * data, size_t length)
{
  assert (filter != NULL);
  assert (data != NULL);

  snippets_bloom_filter_hash (filter, data, length, TRUE);
}

int
snippets_bloom_filter_contains (SnippetsBloomFilter * filter,
    const uint8_t * data, size_t length)
{
  assert (filter != NULL);
  assert (data != NULL);

  return snippets_bloom_filter_hash (filter, data, length, FALSE);
}

void
snippets_bloom_filter_free (SnippetsBloomFilter * filter)
{
  assert (filter != NULL);

  free (filter);
}

unsigned int
snippets_bloom_filter_optimal_n_hash_functions (uint32_t filter_size,
    uint64_t n_elements)
{
  double m, n;

  assert (n_elements != 0);

  m = filter_size;
  n = n_elements;

  return (unsigned int) (pow (3.8, 1.0 / (m / n + 4.2)) * (m / n) * M_LN2);
}
