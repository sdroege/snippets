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

#ifndef __SNIPPETS_BLOOM_FILTER_H__
#define __SNIPPETS_BLOOM_FILTER_H__

#include <snippets/utils.h>

typedef struct _SnippetsBloomFilter SnippetsBloomFilter;

SnippetsBloomFilter * snippets_bloom_filter_new       (uint32_t size, unsigned int n_hash_functions, unsigned int hash_size);
void          snippets_bloom_filter_insert    (SnippetsBloomFilter *filter, const uint8_t *data, size_t length);
int           snippets_bloom_filter_contains  (SnippetsBloomFilter *filter, const uint8_t *data, size_t length);
void          snippets_bloom_filter_free      (SnippetsBloomFilter *filter);

unsigned int  snippets_bloom_filter_optimal_n_hash_functions (uint32_t filter_size, uint64_t n_elements);

#endif /* __SNIPPETS_BLOOM_FILTER_H__ */
