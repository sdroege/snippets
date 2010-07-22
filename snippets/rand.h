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

#ifndef __SNIPPETS_RAND_H__
#define __SNIPPETS_RAND_H__

#include <snippets/utils.h>

SNIPPETS_BEGIN_DECLS

typedef struct _SnippetsRand SnippetsRand;

SnippetsRand * snippets_rand_new          (uint32_t seed);
void           snippets_rand_free         (SnippetsRand *rand);

uint32_t       snippets_rand_uint32       (SnippetsRand *rand);
uint32_t       snippets_rand_uint32_range (SnippetsRand *rand, uint32_t min, uint32_t max);
double         snippets_rand_double       (SnippetsRand *rand);
double         snippets_rand_double_range (SnippetsRand *rand, double min, double max);

SNIPPETS_END_DECLS

#endif /* __SNIPPETS_RAND_H__ */
