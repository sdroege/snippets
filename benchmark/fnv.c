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

#include <stdio.h>
#include <sys/time.h>

#include <snippets/fnv.h>

static uint8_t test_data[8096];

typedef void (*fnv_func) (const uint8_t * data, size_t len, uint8_t * hash);

static uint64_t
run (fnv_func func, int runs)
{
  struct timeval tv_start, tv_end;
  uint64_t start, end;
  int i;
  uint8_t hash[128];

  gettimeofday (&tv_start, NULL);
  for (i = 0; i < runs; i++)
    func (test_data, sizeof (test_data), hash);
  gettimeofday (&tv_end, NULL);

  start = tv_start.tv_sec * 1000000 + tv_start.tv_usec;
  end = tv_end.tv_sec * 1000000 + tv_end.tv_usec;

  return end - start;
}

#define RUN(func, runs) do { \
  uint64_t _duration; \
  _duration = run (func, runs); \
  printf (#func ":\t%04lu.%06lus for " #runs " runs\n", _duration / 1000000, _duration % 1000000); \
} while (0);

int
main (int argc, char **argv)
{
  int i;

  for (i = 0; i < sizeof (test_data); i++)
    test_data[i] = i & 0xff;

  RUN (snippets_fnv1_32, 100000);
  RUN (snippets_fnv1a_32, 100000);

  RUN (snippets_fnv1_64, 100000);
  RUN (snippets_fnv1a_64, 100000);

  RUN (snippets_fnv1_128, 100000);
  RUN (snippets_fnv1a_128, 100000);

  RUN (snippets_fnv1_256, 100000);
  RUN (snippets_fnv1a_256, 100000);

  RUN (snippets_fnv1_512, 100000);
  RUN (snippets_fnv1a_512, 100000);

  RUN (snippets_fnv1_1024, 100000);
  RUN (snippets_fnv1a_1024, 100000);

  return 0;
}
