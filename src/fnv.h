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

/** fnv1_32:
 *  @data: Data to be hashed
 *  @len: Length of @data in bytes
 *  @hash: Pointer to a 4 byte array for the calculated hash
 *
 *  Calculates the FNV1 32 bit hash from @data and puts it
 *  into @hash.
 */
void fnv1_32 (const uint8_t *data, size_t len, uint8_t hash[4]);

/** fnv1a_32:
 *  @data: Data to be hashed
 *  @len: Length of @data in bytes
 *  @hash: Pointer to a 4 byte array for the calculated hash
 *
 *  Calculates the FNV1A 32 bit hash from @data and puts it
 *  into @hash.
 */
void fnv1a_32 (const uint8_t *data, size_t len, uint8_t hash[8]);

/** fnv1_64:
 *  @data: Data to be hashed
 *  @len: Length of @data in bytes
 *  @hash: Pointer to a 8 byte array for the calculated hash
 *
 *  Calculates the FNV1 64 bit hash from @data and puts it
 *  into @hash.
 */
void fnv1_64 (const uint8_t *data, size_t len, uint8_t hash[4]);

/** fnv1a_64:
 *  @data: Data to be hashed
 *  @len: Length of @data in bytes
 *  @hash: Pointer to a 8 byte array for the calculated hash
 *
 *  Calculates the FNV1A 64 bit hash from @data and puts it
 *  into @hash.
 */
void fnv1a_64 (const uint8_t *data, size_t len, uint8_t hash[8]);

/** fnv1_128:
 *  @data: Data to be hashed
 *  @len: Length of @data in bytes
 *  @hash: Pointer to a 16 byte array for the calculated hash
 *
 *  Calculates the FNV1 128 bit hash from @data and puts it
 *  into @hash.
 */
void fnv1_128 (const uint8_t *data, size_t len, uint8_t hash[16]);

/** fnv1a_128:
 *  @data: Data to be hashed
 *  @len: Length of @data in bytes
 *  @hash: Pointer to a 16 byte array for the calculated hash
 *
 *  Calculates the FNV1A 128 bit hash from @data and puts it
 *  into @hash.
 */
void fnv1a_128 (const uint8_t *data, size_t len, uint8_t hash[16]);

/** fnv1_256:
 *  @data: Data to be hashed
 *  @len: Length of @data in bytes
 *  @hash: Pointer to a 32 byte array for the calculated hash
 *
 *  Calculates the FNV1 256 bit hash from @data and puts it
 *  into @hash.
 */
void fnv1_256 (const uint8_t *data, size_t len, uint8_t hash[32]);

/** fnv1a_256:
 *  @data: Data to be hashed
 *  @len: Length of @data in bytes
 *  @hash: Pointer to a 32 byte array for the calculated hash
 *
 *  Calculates the FNV1A 256 bit hash from @data and puts it
 *  into @hash.
 */
void fnv1a_256 (const uint8_t *data, size_t len, uint8_t hash[32]);

/** fnv1_512:
 *  @data: Data to be hashed
 *  @len: Length of @data in bytes
 *  @hash: Pointer to a 64 byte array for the calculated hash
 *
 *  Calculates the FNV1 512 bit hash from @data and puts it
 *  into @hash.
 */
void fnv1_512 (const uint8_t *data, size_t len, uint8_t hash[64]);

/** fnv1a_512:
 *  @data: Data to be hashed
 *  @len: Length of @data in bytes
 *  @hash: Pointer to a 64 byte array for the calculated hash
 *
 *  Calculates the FNV1A 512 bit hash from @data and puts it
 *  into @hash.
 */
void fnv1a_512 (const uint8_t *data, size_t len, uint8_t hash[64]);

#endif /* __FNV_H__ */
