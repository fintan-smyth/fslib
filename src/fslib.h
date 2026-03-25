//      ___________ ___ __       ____   ___
//     / ____/ ___// (_) /_     / __ \ <  /
//    / /_   \__ \/ / / __ \   / / / / / /
//   / __/  ___/ / / / /_/ /  / /_/ / / /
//  /_/    /____/_/_/_.___/   \____(_)_/

#ifndef _FSLIB_H
# define _FSLIB_H

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

// ### Generic Dynamic array ###
//

#define SAFE_CMP(X, Y) ((X > Y) - (X < Y))

#define FS_VEC_INITSIZE 32

#define FS_DECLARE_VEC(T, tag)                                                         \
                                                                                       \
typedef struct Vec_##tag                                                               \
{                                                                                      \
    T		*data;                                                                     \
    size_t	size;                                                                      \
    size_t	capacity;                                                                  \
} Vec_##tag;                                                                           \
                                                                                       \
static inline Vec_##tag *vec_##tag##_init()                                            \
{                                                                                      \
    Vec_##tag *vec = calloc(1, sizeof(Vec_##tag));                                     \
                                                                                       \
	vec->capacity = FS_VEC_INITSIZE;                                                   \
    vec->data = calloc(FS_VEC_INITSIZE, sizeof(Vec_##tag));                            \
	return vec;                                                                        \
}                                                                                      \
                                                                                       \
static inline void vec_##tag##_push(Vec_##tag *v, T value)                             \
{                                                                                      \
    if (v->size >= v->capacity)                                                        \
	{                                                                                  \
        v->capacity *= 2;                                                              \
        v->data = (T*)realloc(v->data, v->capacity * sizeof(T));                       \
    }                                                                                  \
    v->data[v->size++] = value;                                                        \
}                                                                                      \
                                                                                       \
static inline void vec_##tag##_free(Vec_##tag *v, void (*del)(T))                      \
{                                                                                      \
    if (del != NULL)                                                                   \
	{                                                                                  \
		for (size_t i = 0; i < v->size; i++)                                           \
			del(v->data[i]);                                                           \
	}                                                                                  \
    free(v->data);                                                                     \
	free(v);                                                                           \
}                                                                                      \
                                                                                       \
static inline T vec_##tag##_pop(Vec_##tag *v)                                          \
{                                                                                      \
	return v->data[--v->size];                                                         \
}                                                                                      \

#ifdef DEBUG
	#define ASSERT_QUEUE_POP() assert(q->size > 0 && "Popping empty queue!")
#else
	#define ASSERT_QUEUE_POP() do {} while (0)
#endif

// ### Generic Double Ended Queue (Linked list implementation) ###
//
#define FS_DECLARE_QUEUE(T, tag)                                                       \
                                                                                       \
typedef struct Queue_##tag##_node                                                      \
{                                                                                      \
	struct Queue_##tag##_node	*next;                                                 \
	struct Queue_##tag##_node	*prev;                                                 \
	T							val;                                                   \
}	Queue_##tag##_node;                                                                \
                                                                                       \
typedef struct Queue_##tag                                                             \
{                                                                                      \
	Queue_##tag##_node 	*head;                                                         \
	size_t				size;                                                          \
}	Queue_##tag;                                                                       \
                                                                                       \
static inline Queue_##tag *queue_##tag##_init(void)                                    \
{                                                                                      \
	return calloc(1, sizeof(Queue_##tag));                                             \
}                                                                                      \
                                                                                       \
static inline void queue_##tag##_push_head(Queue_##tag *q, T val)                      \
{                                                                                      \
	Queue_##tag##_node *new = calloc(1, sizeof(*new));                                 \
                                                                                       \
	new->val = val;                                                                    \
	if (q->head == NULL)                                                               \
	{                                                                                  \
		new->next = new;                                                               \
		new->prev = new;                                                               \
		q->head = new;                                                                 \
		q->size++;                                                                     \
		return ;                                                                       \
	}                                                                                  \
	Queue_##tag##_node *tmp = q->head->prev;                                           \
	new->next = q->head;                                                               \
	q->head->prev = new;                                                               \
	new->prev = tmp;                                                                   \
	tmp->next = new;                                                                   \
	q->head = new;                                                                     \
	q->size++;                                                                         \
}                                                                                      \
                                                                                       \
static inline void queue_##tag##_push_tail(Queue_##tag *q, T val)                      \
{                                                                                      \
	Queue_##tag##_node *new = calloc(1, sizeof(*new));                                 \
                                                                                       \
	new->val = val;                                                                    \
	if (q->head == NULL)                                                               \
	{                                                                                  \
		new->next = new;                                                               \
		new->prev = new;                                                               \
		q->head = new;                                                                 \
		q->size++;                                                                     \
		return ;                                                                       \
	}                                                                                  \
	Queue_##tag##_node *tmp = q->head->prev;                                           \
	new->next = q->head;                                                               \
	q->head->prev = new;                                                               \
	new->prev = tmp;                                                                   \
	tmp->next = new;                                                                   \
	q->size++;                                                                         \
}                                                                                      \
                                                                                       \
static inline T queue_##tag##_pop_head(Queue_##tag *q)                                 \
{                                                                                      \
	ASSERT_QUEUE_POP();                                                                \
	T out = q->head->val;                                                              \
                                                                                       \
	if (q->size <= 1)                                                                  \
	{                                                                                  \
		free(q->head);                                                                 \
		q->head = NULL;                                                                \
		q->size = 0;                                                                   \
		return out;                                                                    \
	}                                                                                  \
                                                                                       \
	Queue_##tag##_node *head = q->head;                                                \
	head->prev->next = head->next;                                                     \
	head->next->prev = head->prev;                                                     \
	q->head = head->next;                                                              \
	free(head);                                                                        \
	q->size--;                                                                         \
	return out;                                                                        \
}                                                                                      \
                                                                                       \
static inline T queue_##tag##_pop_tail(Queue_##tag *q)                                 \
{                                                                                      \
	ASSERT_QUEUE_POP();                                                                \
	T out = q->head->prev->val;                                                        \
                                                                                       \
	if (q->size <= 1)                                                                  \
	{                                                                                  \
		free(q->head);                                                                 \
		q->head = NULL;                                                                \
		q->size = 0;                                                                   \
		return out;                                                                    \
	}                                                                                  \
                                                                                       \
	Queue_##tag##_node *tail = q->head->prev;                                          \
	tail->next->prev = tail->prev;                                                     \
	tail->prev->next = tail->next;                                                     \
	free(tail);                                                                        \
	q->size--;                                                                         \
	return out;                                                                        \
}                                                                                      \
                                                                                       \
static inline void queue_##tag##_clear(Queue_##tag *q, void (*del)(T))                 \
{                                                                                      \
	if (del != NULL)                                                                   \
	{                                                                                  \
		while (q->size > 0)                                                            \
			del(queue_##tag##_pop_head(q));                                            \
	}                                                                                  \
	else                                                                               \
	{                                                                                  \
		while (q->size > 0)                                                            \
			queue_##tag##_pop_head(q);                                                 \
	}                                                                                  \
}                                                                                      \



// ### Generic Double Ended Queue (Array implementation) ###
//

#define FS_ARRQUEUE_INITSIZE 32
#define FS_DECLARE_ARRQUEUE(T, tag)                                                    \
                                                                                       \
typedef struct ArrQueue_##tag                                                          \
{                                                                                      \
	T		*data;                                                                     \
	ssize_t	head;                                                                      \
	ssize_t	tail;                                                                      \
	size_t	size;                                                                      \
	size_t	capacity;                                                                  \
}	ArrQueue_##tag;                                                                    \
                                                                                       \
static inline ArrQueue_##tag *arrqueue_##tag##_init()                                  \
{                                                                                      \
	ArrQueue_##tag *q = calloc(1, sizeof(*q));                                         \
                                                                                       \
	q->capacity = FS_ARRQUEUE_INITSIZE;                                                \
	q->data = calloc(FS_ARRQUEUE_INITSIZE, sizeof(T));                                 \
	q->head = 1;                                                                       \
	q->tail = 0;                                                                       \
	return q;                                                                          \
}                                                                                      \
                                                                                       \
static inline void _arrqueue_##tag##_grow(ArrQueue_##tag *q)                           \
{                                                                                      \
	T *new_data = malloc(q->capacity * 2 * sizeof(T));                                 \
                                                                                       \
	size_t chunk_size = q->capacity - q->head;                                         \
	memcpy(new_data, &q->data[q->head], chunk_size * sizeof(T));                       \
	memcpy(&new_data[chunk_size], q->data, (q->capacity - chunk_size) * sizeof(T));    \
	free(q->data);                                                                     \
	q->head = 0;                                                                       \
	q->tail = q->capacity - 1;                                                         \
	q->capacity *= 2;                                                                  \
	q->data = new_data;                                                                \
}                                                                                      \
                                                                                       \
static inline void arrqueue_##tag##_push_tail(ArrQueue_##tag *q, T val)                \
{                                                                                      \
	if (q->size >= q->capacity)                                                        \
		_arrqueue_##tag##_grow(q);                                                     \
                                                                                       \
    q->tail = (q->tail + 1) % q->capacity;                                             \
	q->data[q->tail] = val;                                                            \
	q->size++;                                                                         \
}                                                                                      \
                                                                                       \
static inline void arrqueue_##tag##_push_head(ArrQueue_##tag *q, T val)                \
{                                                                                      \
	if (q->size >= q->capacity)                                                        \
		_arrqueue_##tag##_grow(q);                                                     \
                                                                                       \
    q->head = (q->head + q->capacity - 1) % q->capacity;                               \
	q->data[q->head] = val;                                                            \
	q->size++;                                                                         \
}                                                                                      \
                                                                                       \
static inline T arrqueue_##tag##_pop_head(ArrQueue_##tag *q)                           \
{                                                                                      \
	ASSERT_QUEUE_POP();                                                                \
	T out = q->data[q->head];                                                          \
	q->head = (q->head + 1) % q->capacity;                                             \
	q->size--;                                                                         \
	return out;                                                                        \
}                                                                                      \
                                                                                       \
static inline T arrqueue_##tag##_pop_tail(ArrQueue_##tag *q)                           \
{                                                                                      \
	ASSERT_QUEUE_POP();                                                                \
	T out = q->data[q->tail];                                                          \
	q->tail = (q->tail + q->capacity - 1) % q->capacity;                               \
	q->size--;                                                                         \
	return out;                                                                        \
}                                                                                      \
                                                                                       \
static inline void arrqueue_##tag##_free(ArrQueue_##tag *q, void (*del)(T))            \
{                                                                                      \
	if (del != NULL && q->size > 0)                                                    \
	{                                                                                  \
		for (size_t i = q->head; q->size-- > 0; i = (i + 1) % q->capacity)             \
			del(q->data[i]);                                                           \
	}                                                                                  \
	free(q->data);                                                                     \
	free(q);                                                                           \
}                                                                                      \

// ### Generic BSTSet ###
//
#define FS_DECLARE_BSTSET(T, tag, cmp)                                                 \
                                                                                       \
typedef struct BSTSet_##tag##_node                                                     \
{                                                                                      \
	T							val;                                                   \
	struct BSTSet_##tag##_node	*left;                                                 \
	struct BSTSet_##tag##_node	*right;                                                \
}	BSTSet_##tag##_node;                                                               \
                                                                                       \
typedef struct BSTSet_##tag                                                            \
{                                                                                      \
	BSTSet_##tag##_node	*tree;                                                         \
	size_t				size;                                                          \
}	BSTSet_##tag;                                                                      \
                                                                                       \
static inline BSTSet_##tag *bstset_##tag##_init()                                      \
{                                                                                      \
	BSTSet_##tag *set = calloc(1, sizeof(*set));                                       \
                                                                                       \
	return set;                                                                        \
}                                                                                      \
                                                                                       \
static inline int bstset_##tag##_insert(BSTSet_##tag *set, T val)                      \
{                                                                                      \
	BSTSet_##tag##_node *current = set->tree;                                          \
	BSTSet_##tag##_node **addr = &set->tree;                                           \
	int (*cmpf)(T, T) = (int (*)(T, T))cmp;                                            \
                                                                                       \
	while (current != NULL)                                                            \
	{                                                                                  \
		int diff = (cmpf == NULL)                                                      \
			? SAFE_CMP(val, current->val)                                              \
			: cmpf(val, current->val);                                                 \
		if (diff > 0)                                                                  \
			addr = &current->right;                                                    \
		else if (diff < 0)                                                             \
			addr = &current->left;                                                     \
		else                                                                           \
			return 1;                                                                  \
		current = *addr;                                                               \
	}                                                                                  \
                                                                                       \
	BSTSet_##tag##_node *new = calloc(1, sizeof(*new));                                \
	new->val = val;                                                                    \
	*addr = new;                                                                       \
	set->size++;                                                                       \
	return 0;                                                                          \
}                                                                                      \
                                                                                       \
static inline BSTSet_##tag##_node *_bstset_##tag##_remove_r(                           \
									BSTSet_##tag##_node *node,                         \
									T val, void (*del)(T))                             \
{                                                                                      \
	if (node == NULL)                                                                  \
		return node;                                                                   \
                                                                                       \
	int (*cmpf)(T, T) = (int (*)(T, T))cmp;                                            \
	int diff = (cmpf == NULL) ? SAFE_CMP(val, node->val) : cmpf(val, node->val);       \
	if (diff < 0)                                                                      \
		node->left = _bstset_##tag##_remove_r(node->left, val, del);                   \
	else if (diff > 0)                                                                 \
		node->right = _bstset_##tag##_remove_r(node->right, val, del);                 \
	else                                                                               \
	{                                                                                  \
		if (node->left == NULL)                                                        \
		{                                                                              \
			BSTSet_##tag##_node *tmp = node->right;                                    \
			if (del != NULL)                                                           \
				del(node->val);                                                        \
			free(node);                                                                \
			return tmp;                                                                \
		}                                                                              \
		if (node->right == NULL)                                                       \
		{                                                                              \
			BSTSet_##tag##_node *tmp = node->left;                                     \
			if (del != NULL)                                                           \
				del(node->val);                                                        \
			free(node);                                                                \
			return tmp;                                                                \
		}                                                                              \
		BSTSet_##tag##_node *min = node->right;                                        \
		while (min->left != NULL)                                                      \
			min = min->left;                                                           \
		node->val = min->val;                                                          \
		node->right = _bstset_##tag##_remove_r(node->right, min->val, del);            \
	}                                                                                  \
	return node;                                                                       \
}                                                                                      \
                                                                                       \
static inline void bstset_##tag##_remove(BSTSet_##tag *set, T val, void (*del)(T))     \
{                                                                                      \
	set->tree = _bstset_##tag##_remove_r(set->tree, val, del);                         \
}                                                                                      \
                                                                                       \
static inline int bstset_##tag##_contains(BSTSet_##tag *set, T val)                    \
{                                                                                      \
	BSTSet_##tag##_node *current = set->tree;                                          \
	int (*cmpf)(T, T) = (int (*)(T, T))cmp;                                            \
                                                                                       \
	while (current != NULL)                                                            \
	{                                                                                  \
		int diff = (cmpf == NULL)                                                      \
			? SAFE_CMP(val, current->val)                                              \
			: cmpf(val, current->val);                                                 \
		if (diff > 0)                                                                  \
			current = current->right;                                                  \
		else if (diff < 0)                                                             \
			current = current->left;                                                   \
		else                                                                           \
			return true;                                                               \
	}                                                                                  \
                                                                                       \
	return false;                                                                      \
}                                                                                      \
                                                                                       \
static inline void _bstset_##tag##_clear_r(BSTSet_##tag##_node *node, void (*del)(T))  \
{                                                                                      \
	if (node == NULL)                                                                  \
		return ;                                                                       \
	_bstset_##tag##_clear_r(node->left, del);                                          \
	_bstset_##tag##_clear_r(node->right, del);                                         \
	if (del != NULL)                                                                   \
		del(node->val);                                                                \
	free(node);                                                                        \
}                                                                                      \
                                                                                       \
static inline void bstset_##tag##_clear(BSTSet_##tag *set, void (*del)(T))             \
{                                                                                      \
	_bstset_##tag##_clear_r(set->tree, del);                                           \
	set->tree = NULL;                                                                  \
	set->size = 0;                                                                     \
}                                                                                      \


#endif // _FSLIB_H
