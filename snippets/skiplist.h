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

#ifndef __SNIPPETS_SKIP_LIST_H__
#define __SNIPPETS_SKIP_LIST_H__

#include <snippets/utils.h>

SNIPPETS_BEGIN_DECLS

typedef struct _SnippetsSkipList SnippetsSkipList;
typedef struct _SnippetsSkipListNode SnippetsSkipListNode;

SnippetsSkipList * snippets_skip_list_new (unsigned int max_level, double p, size_t data_size, SnippetsCopyToFunction copy_func, SnippetsFreeFunction free_func, SnippetsCompareFunction compare_func, void *user_data, SnippetsCopyFunction user_data_copy, SnippetsFreeFunction user_data_free);
SnippetsSkipList * snippets_skip_list_new_pointer (unsigned int max_level, double p, SnippetsCopyToFunction copy_func, SnippetsFreeFunction free_func, SnippetsCompareFunction compare_func, void *user_data, SnippetsCopyFunction user_data_copy, SnippetsFreeFunction user_data_free);
void snippets_skip_list_free (SnippetsSkipList *list);

SnippetsSkipList * snippets_skip_list_copy (const SnippetsSkipList *list);

SnippetsSkipListNode * snippets_skip_list_insert (SnippetsSkipList *list, void *data);
void snippets_skip_list_remove (SnippetsSkipList *list, SnippetsSkipListNode *node);
void snippets_skip_list_remove_value (SnippetsSkipList *list, const void *data);
SnippetsSkipListNode * snippets_skip_list_find (SnippetsSkipList *list, const void *data, int exact);

SnippetsSkipListNode * snippets_skip_list_head (SnippetsSkipList *list);
SnippetsSkipListNode * snippets_skip_list_tail (SnippetsSkipList *list);
size_t snippets_skip_list_length (SnippetsSkipList *list);

SnippetsSkipListNode * snippets_skip_list_node_next (SnippetsSkipListNode *node);
SnippetsSkipListNode * snippets_skip_list_node_prev (SnippetsSkipListNode *node);

#define snippets_skip_list_node_get(node, __type) \
  ((__type *) snippets_skip_list_node_get_ (node));
void * snippets_skip_list_node_get_ (SnippetsSkipListNode *node);

SNIPPETS_END_DECLS

#endif /* __SNIPPETS_SKIP_LIST_H__ */
