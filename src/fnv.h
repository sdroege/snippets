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

#ifndef __FNV_H__
#define __FNV_H__

#include "utils.h"

void fnv1_32 (const uint8_t *data, size_t len, uint8_t hash[4]);
void fnv1_64 (const uint8_t *data, size_t len, uint8_t hash[4]);
void fnv1a_32 (const uint8_t *data, size_t len, uint8_t hash[8]);
void fnv1a_64 (const uint8_t *data, size_t len, uint8_t hash[8]);
void fnv1_128 (const uint8_t *data, size_t len, uint8_t hash[16]);
void fnv1a_128 (const uint8_t *data, size_t len, uint8_t hash[16]);

#endif /* __FNV_H__ */
