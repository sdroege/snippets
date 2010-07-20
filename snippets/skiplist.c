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

#include <snippets/skiplist.h>
#include <snippets/rand.h>

#include <assert.h>
#include <string.h>
#include <time.h>

#define MAX_LEVELS 32

struct _SnippetsSkipList
{
  SnippetsSkipListNode *head, *tail;
  size_t length;
  int pointer;

  unsigned int max_level;
  double p;

  SnippetsRand *rand;

  size_t data_size;
  SnippetsCompareFunction compare_func;
  SnippetsCopyToFunction copy_func;
  SnippetsFreeFunction free_func;

  void *user_data;
  SnippetsCopyFunction user_data_copy;
  SnippetsFreeFunction user_data_free;
};

struct _SnippetsSkipListNode
{
  SnippetsSkipList *list;
  SnippetsSkipListNode *prev, *next;
  unsigned int level;
  SnippetsSkipListNode **skip_links;    /* level > 1 */
  void *data;
};

#define STRUCT_ALIGNMENT (2 * sizeof (size_t))
#define STRUCT_ALIGN(offset) \
    ((offset + (STRUCT_ALIGNMENT - 1)) & -STRUCT_ALIGNMENT)

static unsigned int
snippets_skip_list_get_random_level (SnippetsRand * rand,
    unsigned int max_level, double p)
{
  unsigned int level = 1;
  double r;

  while ((r = snippets_rand_double (rand)) < p && level < max_level)
    level++;

  return level;
}

static SnippetsSkipListNode *
snippets_skip_list_node_new (SnippetsSkipList * list, size_t data_size,
    SnippetsCopyToFunction copy_func, void *data, int level)
{
  SnippetsSkipListNode *node;

  if (!list->pointer) {
    node =
        calloc (STRUCT_ALIGN (sizeof (SnippetsSkipListNode)) +
        STRUCT_ALIGN ((level - 1) * sizeof (SnippetsSkipListNode *)) +
        data_size, 1);
    node->skip_links =
        (SnippetsSkipListNode **) (((uint8_t *) node) +
        STRUCT_ALIGN (sizeof (SnippetsSkipListNode)));
    node->data =
        ((uint8_t *) node) + STRUCT_ALIGN (sizeof (SnippetsSkipListNode)) +
        STRUCT_ALIGN ((level - 1) * sizeof (SnippetsSkipListNode *));

    if (data && copy_func)
      copy_func (node->data, data);
    else if (data)
      memcpy (node->data, data, data_size);
  } else {
    node =
        calloc (STRUCT_ALIGN (sizeof (SnippetsSkipListNode)) + (level -
            1) * sizeof (SnippetsSkipListNode *), 1);
    node->skip_links =
        (SnippetsSkipListNode **) (((uint8_t *) node) +
        STRUCT_ALIGN (sizeof (SnippetsSkipListNode)));
    if (copy_func)
      copy_func (&node->data, data);
    else
      node->data = data;
  }
  node->list = list;
  node->level = level;

  return node;
}

static void
snippets_skip_list_node_free (SnippetsSkipListNode * node,
    SnippetsFreeFunction free_func)
{
  if (free_func && node->data)
    free_func (node->data);
  free (node);
}

SnippetsSkipList *
snippets_skip_list_new (unsigned int max_level, double p, size_t data_size,
    SnippetsCopyToFunction copy_func, SnippetsFreeFunction free_func,
    SnippetsCompareFunction compare_func, void *user_data,
    SnippetsCopyFunction user_data_copy, SnippetsFreeFunction user_data_free)
{
  SnippetsSkipList *list;

  assert (max_level >= 2 && max_level <= MAX_LEVELS);
  assert (data_size != 0);
  assert (p > 0 && p < 1.0);
  assert (compare_func != NULL);

  list = calloc (sizeof (SnippetsSkipList), 1);

  list->max_level = max_level;
  list->p = p;
  list->data_size = data_size;
  list->copy_func = copy_func;
  list->free_func = free_func;
  list->compare_func = compare_func;
  list->pointer = FALSE;

  list->user_data = user_data;
  list->user_data_copy = user_data_copy;
  list->user_data_free = user_data_free;

  list->rand = snippets_rand_new (SNIPPETS_RAND_MODE_MT19937, time (0));

  list->head =
      snippets_skip_list_node_new (list, list->data_size, NULL, NULL,
      max_level);

  return list;
}

SnippetsSkipList *
snippets_skip_list_new_pointer (unsigned int max_level, double p,
    SnippetsCopyToFunction copy_func, SnippetsFreeFunction free_func,
    SnippetsCompareFunction compare_func, void *user_data,
    SnippetsCopyFunction user_data_copy, SnippetsFreeFunction user_data_free)
{
  SnippetsSkipList *list = calloc (sizeof (SnippetsSkipList), 1);

  assert (max_level >= 2 && max_level <= MAX_LEVELS);
  assert (p > 0 && p < 1.0);
  assert (compare_func != NULL);

  list->max_level = max_level;
  list->p = p;
  list->data_size = 0;
  list->copy_func = copy_func;
  list->free_func = free_func;
  list->compare_func = compare_func;
  list->pointer = TRUE;

  list->user_data = user_data;
  list->user_data_copy = user_data_copy;
  list->user_data_free = user_data_free;

  list->rand = snippets_rand_new (SNIPPETS_RAND_MODE_MT19937, time (0));

  list->head =
      snippets_skip_list_node_new (list, list->data_size, NULL, NULL,
      max_level);

  return list;
}

void
snippets_skip_list_free (SnippetsSkipList * list)
{
  SnippetsSkipListNode *l, *m;

  assert (list != NULL);

  l = list->head;
  while (l) {
    m = l;
    l = l->next;
    snippets_skip_list_node_free (m, list->free_func);
  }
  snippets_rand_free (list->rand);

  if (list->user_data && list->user_data_free)
    list->user_data_free (list->user_data);

  free (list);
}

SnippetsSkipList *
snippets_skip_list_copy (const SnippetsSkipList * list)
{
  SnippetsSkipList *copy;
  SnippetsSkipListNode *l;

  assert (list != NULL);

  copy = calloc (sizeof (SnippetsSkipList), 1);

  copy->max_level = list->max_level;
  copy->p = list->p;
  copy->data_size = list->data_size;
  copy->copy_func = list->copy_func;
  copy->free_func = list->free_func;
  copy->compare_func = list->compare_func;
  copy->pointer = list->pointer;

  if (list->user_data && list->user_data_copy) {
    copy->user_data = list->user_data_copy (list->user_data);
    copy->user_data_copy = list->user_data_copy;
    copy->user_data_free = list->user_data_free;
  }

  copy->rand = snippets_rand_new (SNIPPETS_RAND_MODE_MT19937, time (0));

  /* TODO: Could build perfect skip list here by
   * choosing the optimal levels
   */
  copy->head =
      snippets_skip_list_node_new (copy, copy->data_size, NULL, NULL,
      copy->max_level);
  for (l = list->head->next; l; l = l->next)
    snippets_skip_list_insert (copy, l->data);

  return copy;
}

static int
snippets_skip_list_find_internal (SnippetsSkipList * list, const void *data,
    SnippetsSkipListNode * nodes[MAX_LEVELS + 1])
{
  SnippetsSkipListNode *l;
  int i, tmp, res = -1;

  /* Nodes must be emptied! */

  l = list->head;

  /* Look at level max-1 to 1 */
  for (i = list->max_level - 2; i >= 0; i--) {
    while (l->skip_links[i]
        && (tmp = list->compare_func (l->skip_links[i]->data, data,
                list->user_data)) < 0) {
      l = l->skip_links[i];
      res = tmp;
    }
    nodes[i + 1] = l;
  }

  /* level 0 */
  while (l->next
      && (tmp =
          list->compare_func (l->next->data, data, list->user_data)) <= 0) {
    l = l->next;
    res = tmp;
  }
  nodes[0] = l;

  return (l == list->head) ? -1 : res;
}

SnippetsSkipListNode *
snippets_skip_list_insert (SnippetsSkipList * list, void *data)
{
  SnippetsSkipListNode *nodes[MAX_LEVELS + 1] = { NULL, };
  int i, res, level;
  SnippetsSkipListNode *node;

  assert (list != NULL);
  assert (data != NULL);

  res = snippets_skip_list_find_internal (list, data, nodes);
  if (res == 0) {
    if (list->free_func)
      list->free_func (data);
    return nodes[0];
  }

  level =
      snippets_skip_list_get_random_level (list->rand, list->max_level,
      list->p);
  node =
      snippets_skip_list_node_new (list, list->data_size, list->copy_func, data,
      level);

  node->prev = nodes[0];
  node->next = nodes[0]->next;
  if (nodes[0]->next)
    nodes[0]->next->prev = node;
  nodes[0]->next = node;

  if (!node->next)
    list->tail = node;

  for (i = 0; i < level - 1; i++) {
    node->skip_links[i] = nodes[i + 1]->skip_links[i];
    nodes[i + 1]->skip_links[i] = node;
  }

  list->length++;

  return node;
}

void
snippets_skip_list_remove (SnippetsSkipList * list, SnippetsSkipListNode * node)
{
  SnippetsSkipListNode *nodes[MAX_LEVELS + 1] = { NULL, };
  SnippetsSkipListNode *n;
  int i, j;

  assert (list != NULL);
  assert (node != NULL);
  assert (node->list == list);

  nodes[0] = node->prev;
  n = node->prev;

  if (node == list->tail)
    list->tail = n;

  i = 1;
  while (i < node->level) {
    assert (n);

    if (n->level > i) {
      for (j = i; j < n->level && j < node->level; j++)
        nodes[i++] = n;
    }
    n = n->prev;
  }

  nodes[0]->next = node->next;
  if (node->next)
    node->next->prev = nodes[0];

  for (i = 0; i + 1 < node->level; i++) {
    assert (nodes[i + 1]->skip_links[i] == node);
    nodes[i + 1]->skip_links[i] = node->skip_links[i];
  }

  list->length--;
  snippets_skip_list_node_free (node, list->free_func);
}

void
snippets_skip_list_remove_value (SnippetsSkipList * list, const void *data)
{
  SnippetsSkipListNode *nodes[MAX_LEVELS + 1] = { NULL, };
  int i, res;

  assert (list != NULL);
  assert (data != NULL);

  res = snippets_skip_list_find_internal (list, data, nodes);
  if (res != 0)
    return;
  assert (nodes[0] != NULL);

  if (nodes[0] == list->tail)
    list->tail = nodes[0]->prev;

  nodes[0]->prev->next = nodes[0]->next;
  if (nodes[0]->next)
    nodes[0]->next->prev = nodes[0]->prev;

  for (i = 0; i + 1 < nodes[0]->level; i++) {
    assert (nodes[i + 1]->skip_links[i] == nodes[0]);
    nodes[i + 1]->skip_links[i] = nodes[0]->skip_links[i];
  }

  list->length--;
  snippets_skip_list_node_free (nodes[0], list->free_func);
}

SnippetsSkipListNode *
snippets_skip_list_find (SnippetsSkipList * list, const void *data, int exact)
{
  SnippetsSkipListNode *nodes[MAX_LEVELS + 1] = { NULL, };
  int res;

  assert (list != NULL);
  assert (data != NULL);

  res = snippets_skip_list_find_internal (list, data, nodes);

  if (exact)
    return (res == 0) ? nodes[0] : NULL;
  return (nodes[0] == list->head) ? NULL : nodes[0];
}

SnippetsSkipListNode *
snippets_skip_list_head (SnippetsSkipList * list)
{
  assert (list != NULL);
  return list->head->next;
}

SnippetsSkipListNode *
snippets_skip_list_tail (SnippetsSkipList * list)
{
  assert (list != NULL);
  return list->tail;
}

size_t
snippets_skip_list_length (SnippetsSkipList * list)
{
  assert (list != NULL);
  return list->length;
}

SnippetsSkipListNode *
snippets_skip_list_node_next (SnippetsSkipListNode * node)
{
  assert (node != NULL);
  return node->next;
}

SnippetsSkipListNode *
snippets_skip_list_node_prev (SnippetsSkipListNode * node)
{
  assert (node != NULL);
  return (node->prev == node->list->head) ? NULL : node->prev;
}

void *
snippets_skip_list_node_get_ (SnippetsSkipListNode * node)
{
  assert (node != NULL);

  if (node->list->pointer)
    return &node->data;
  else
    return node->data;
}
