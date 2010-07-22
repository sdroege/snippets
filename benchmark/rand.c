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
#include <time.h>
#include <sys/time.h>

#include <snippets/rand.h>

#define NRUNS 250000000

#define RUN(func, name) do { \
  uint64_t start, end, duration; \
  struct timeval tv_start, tv_end; \
  \
  gettimeofday (&tv_start, NULL); \
  func(); \
  gettimeofday (&tv_end, NULL); \
  \
  start = tv_start.tv_sec * 1000000 + tv_start.tv_usec; \
  end = tv_end.tv_sec * 1000000 + tv_end.tv_usec; \
  \
  duration = end - start; \
  printf (name ":\t%04lu.%06lus for %d runs\n", duration / 1000000, duration % 1000000, NRUNS); \
} while (0);

static void
mt19937_uint32 (void)
{
  SnippetsRand *rand = snippets_rand_new (time (0));
  unsigned int i;

  for (i = 0; i < NRUNS; i++)
    snippets_rand_uint32 (rand);

  snippets_rand_free (rand);
}

static void
mt19937_uint32_range (void)
{
  SnippetsRand *rand = snippets_rand_new (time (0));
  unsigned int i;

  for (i = 0; i < NRUNS; i++)
    snippets_rand_uint32_range (rand, 100, 1000);

  snippets_rand_free (rand);
}

static void
mt19937_double (void)
{
  SnippetsRand *rand = snippets_rand_new (time (0));
  unsigned int i;

  for (i = 0; i < NRUNS; i++)
    snippets_rand_double (rand);

  snippets_rand_free (rand);
}

static void
mt19937_double_range (void)
{
  SnippetsRand *rand = snippets_rand_new (time (0));
  unsigned int i;

  for (i = 0; i < NRUNS; i++)
    snippets_rand_double_range (rand, 100, 1000);

  snippets_rand_free (rand);
}

int
main (int argc, char **argv)
{
  RUN (mt19937_uint32, "MT19937 uint32       ");
  RUN (mt19937_uint32_range, "MT19937 uint32 range ");
  RUN (mt19937_double, "MT19937 double       ");
  RUN (mt19937_double_range, "MT19937 double range ");
  return 0;
}
