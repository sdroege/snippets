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

#ifndef __SNIPPETS_LINKED_LIST_H__
#define __SNIPPETS_LINKED_LIST_H__

#include <snippets/utils.h>

SNIPPETS_BEGIN_DECLS

typedef struct _SnippetsLinkedList SnippetsLinkedList;
typedef struct _SnippetsLinkedListNode SnippetsLinkedListNode;

SnippetsLinkedList * snippets_linked_list_new (size_t data_size, SnippetsCopyToFunction copy_func, SnippetsFreeFunction free_func);
SnippetsLinkedList * snippets_linked_list_new_pointer (SnippetsCopyToFunction copy_func, SnippetsFreeFunction free_func);
void snippets_linked_list_free (SnippetsLinkedList *list);

SnippetsLinkedList * snippets_linked_list_copy (const SnippetsLinkedList *list);

SnippetsLinkedListNode * snippets_linked_list_append (SnippetsLinkedList *list, void *data);
SnippetsLinkedListNode * snippets_linked_list_prepend (SnippetsLinkedList *list, void *data);
SnippetsLinkedListNode * snippets_linked_list_insert_after (SnippetsLinkedList *list, SnippetsLinkedListNode *prev, void *data);
SnippetsLinkedListNode * snippets_linked_list_insert_before (SnippetsLinkedList *list, SnippetsLinkedListNode *next, void *data);
void snippets_linked_list_remove (SnippetsLinkedList *list, SnippetsLinkedListNode *node);

SnippetsLinkedListNode * snippets_linked_list_find (SnippetsLinkedList *list, const void *data, SnippetsCompareFunction compare_func, void *user_data);

SnippetsLinkedListNode * snippets_linked_list_head (SnippetsLinkedList *list);
SnippetsLinkedListNode * snippets_linked_list_tail (SnippetsLinkedList *list);
size_t snippets_linked_list_length (SnippetsLinkedList *list);

SnippetsLinkedListNode * snippets_linked_list_node_next (SnippetsLinkedListNode *node);
SnippetsLinkedListNode * snippets_linked_list_node_prev (SnippetsLinkedListNode *node);

#define snippets_linked_list_node_get(node, __type) \
  ((__type *) snippets_linked_list_node_get_ (node));
void * snippets_linked_list_node_get_ (SnippetsLinkedListNode *node);

SNIPPETS_END_DECLS

#endif /* __SNIPPETS_LINKED_LIST_H__ */
