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

#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdlib.h>

#include <snippets/snippets-stdint.h>

#ifndef NULL
#define NULL ((void *) 0)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef TRUE
#define TRUE (!FALSE)
#endif

#ifdef  __cplusplus
# define SNIPPETS_BEGIN_DECLS  extern "C" {
# define SNIPPETS_END_DECLS    }
#else
# define SNIPPETS_BEGIN_DECLS
# define SNIPPETS_END_DECLS
#endif

SNIPPETS_BEGIN_DECLS

static inline void *
snippets_malloc (size_t size)
{
  return size ? malloc (size) : NULL;
}

static inline void *
snippets_malloc0 (size_t size)
{
  return size ? calloc (size, 0) : NULL;
}

static inline void
snippets_free (void * ptr)
{
  if (ptr)
    free (ptr);
}

SNIPPETS_END_DECLS

#endif /* __UTILS_H__ */
