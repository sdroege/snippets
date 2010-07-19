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

#include <snippets/linkedlist.h>

#include <assert.h>
#include <string.h>

struct _SnippetsLinkedList
{
  SnippetsLinkedListNode *head, *tail;
  size_t length;
  int pointer;

  size_t data_size;
  SnippetsCopyToFunction copy_func;
  SnippetsFreeFunction free_func;
};

struct _SnippetsLinkedListNode
{
  SnippetsLinkedList *list;
  SnippetsLinkedListNode *prev, *next;
  void *data;
};

#define STRUCT_ALIGNMENT (2 * sizeof (size_t))
#define STRUCT_ALIGN(offset) \
    ((offset + (STRUCT_ALIGNMENT - 1)) & -STRUCT_ALIGNMENT)

static SnippetsLinkedListNode *
snippets_linked_list_node_new (SnippetsLinkedList * list, size_t data_size,
    SnippetsCopyToFunction copy_func, void *data)
{
  SnippetsLinkedListNode *node;

  if (!list->pointer) {
    assert (data != NULL);

    node =
        calloc (STRUCT_ALIGN (sizeof (SnippetsLinkedListNode) + data_size), 1);
    node->data =
        ((uint8_t *) node) + STRUCT_ALIGN (sizeof (SnippetsLinkedListNode));
    if (copy_func)
      copy_func (node->data, data);
    else
      memcpy (node->data, data, data_size);
  } else {
    node = calloc (sizeof (SnippetsLinkedListNode), 1);
    if (copy_func)
      copy_func (&node->data, data);
    else
      node->data = data;
  }
  node->list = list;

  return node;
}

static void
snippets_linked_list_node_free (SnippetsLinkedListNode * node,
    SnippetsFreeFunction free_func)
{
  if (free_func && node->data)
    free_func (node->data);
  free (node);
}

SnippetsLinkedList *
snippets_linked_list_new (size_t data_size, SnippetsCopyToFunction copy_func,
    SnippetsFreeFunction free_func)
{
  SnippetsLinkedList *list;

  assert (data_size != 0);

  list = calloc (sizeof (SnippetsLinkedList), 1);

  list->data_size = data_size;
  list->copy_func = copy_func;
  list->free_func = free_func;
  list->pointer = FALSE;

  return list;
}

SnippetsLinkedList *
snippets_linked_list_new_pointer (SnippetsCopyToFunction copy_func,
    SnippetsFreeFunction free_func)
{
  SnippetsLinkedList *list = calloc (sizeof (SnippetsLinkedList), 1);

  list->data_size = 0;
  list->copy_func = copy_func;
  list->free_func = free_func;
  list->pointer = TRUE;

  return list;
}

void
snippets_linked_list_free (SnippetsLinkedList * list)
{
  SnippetsLinkedListNode *l, *m;

  assert (list != NULL);

  l = list->head;
  while (l) {
    m = l;
    l = l->next;
    snippets_linked_list_node_free (m, list->free_func);
  }
  free (list);
}

SnippetsLinkedList *
snippets_linked_list_copy (const SnippetsLinkedList * list)
{
  SnippetsLinkedList *copy;
  SnippetsLinkedListNode *l;

  assert (list != NULL);

  copy = calloc (sizeof (SnippetsLinkedList), 1);

  copy->data_size = list->data_size;
  copy->copy_func = list->copy_func;
  copy->free_func = list->free_func;
  copy->pointer = list->pointer;

  for (l = list->head; l; l = l->next)
    snippets_linked_list_append (copy, l->data);

  return copy;
}

SnippetsLinkedListNode *
snippets_linked_list_append (SnippetsLinkedList * list, void *data)
{
  SnippetsLinkedListNode *node;
  SnippetsLinkedListNode *prev;

  assert (list != NULL);

  node =
      snippets_linked_list_node_new (list, list->data_size, list->copy_func,
      data);

  if (!list->head) {
    list->head = list->tail = node;
  } else {
    prev = list->tail;
    prev->next = node;
    node->prev = prev;
    list->tail = node;
  }
  list->length++;

  return node;
}

SnippetsLinkedListNode *
snippets_linked_list_prepend (SnippetsLinkedList * list, void *data)
{
  SnippetsLinkedListNode *node;
  SnippetsLinkedListNode *next;

  assert (list != NULL);

  node =
      snippets_linked_list_node_new (list, list->data_size, list->copy_func,
      data);

  if (!list->head) {
    list->head = list->tail = node;
  } else {
    next = list->head;
    next->prev = node;
    node->next = next;
    list->head = node;
  }
  list->length++;

  return node;
}

SnippetsLinkedListNode *
snippets_linked_list_insert_after (SnippetsLinkedList * list,
    SnippetsLinkedListNode * prev, void *data)
{
  SnippetsLinkedListNode *node;
  SnippetsLinkedListNode *next;

  assert (list != NULL);

  if (!prev)
    return snippets_linked_list_append (list, data);

  assert (prev != NULL);
  assert (prev->list == list);

  node =
      snippets_linked_list_node_new (list, list->data_size, list->copy_func,
      data);
  next = prev->next;
  prev->next = node;
  next->prev = node;
  node->prev = prev;
  node->next = next;

  list->length++;

  return node;
}

SnippetsLinkedListNode *
snippets_linked_list_insert_before (SnippetsLinkedList * list,
    SnippetsLinkedListNode * next, void *data)
{
  SnippetsLinkedListNode *node;
  SnippetsLinkedListNode *prev;

  assert (list != NULL);

  if (!next)
    return snippets_linked_list_prepend (list, data);

  assert (next != NULL);
  assert (next->list == list);

  node =
      snippets_linked_list_node_new (list, list->data_size, list->copy_func,
      data);
  prev = next->prev;
  next->prev = node;
  prev->next = node;
  node->prev = prev;
  node->next = next;

  list->length++;

  return node;
}

void
snippets_linked_list_remove (SnippetsLinkedList * list,
    SnippetsLinkedListNode * node)
{
  SnippetsLinkedListNode *prev, *next;

  assert (list != NULL);
  assert (node != NULL);
  assert (node->list == list);

  if (node == list->head) {
    next = node->next;
    if (next)
      next->prev = NULL;
    else
      list->tail = NULL;
    list->head = next;
  } else if (node == list->tail) {
    prev = node->prev;
    if (prev)
      prev->next = NULL;
    list->tail = prev;
  } else {
    prev = node->prev;
    next = node->next;

    prev->next = next;
    next->prev = prev;
  }
  list->length--;
  snippets_linked_list_node_free (node, list->free_func);
}

SnippetsLinkedListNode *
snippets_linked_list_head (SnippetsLinkedList * list)
{
  assert (list != NULL);
  return list->head;
}

SnippetsLinkedListNode *
snippets_linked_list_tail (SnippetsLinkedList * list)
{
  assert (list != NULL);
  return list->tail;
}

size_t
snippets_linked_list_length (SnippetsLinkedList * list)
{
  assert (list != NULL);
  return list->length;
}

SnippetsLinkedListNode *
snippets_linked_list_node_next (SnippetsLinkedListNode * node)
{
  assert (node != NULL);
  return node->next;
}

SnippetsLinkedListNode *
snippets_linked_list_node_prev (SnippetsLinkedListNode * node)
{
  assert (node != NULL);
  return node->prev;
}

void *
snippets_linked_list_node_get_ (SnippetsLinkedListNode * node)
{
  assert (node != NULL);

  if (node->list->pointer)
    return &node->data;
  else
    return node->data;
}
