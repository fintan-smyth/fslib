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
    Vec_##tag *vec = (Vec_##tag *)calloc(1, sizeof(Vec_##tag));                        \
                                                                                       \
	vec->capacity = FS_VEC_INITSIZE;                                                   \
    vec->data = (T *)calloc(FS_VEC_INITSIZE, sizeof(T));                               \
	return vec;                                                                        \
}                                                                                      \
                                                                                       \
static inline void vec_##tag##_push(Vec_##tag *v, T value)                             \
{                                                                                      \
    if (v->size >= v->capacity)                                                        \
	{                                                                                  \
        v->capacity *= 2;                                                              \
        v->data = (T *)realloc(v->data, v->capacity * sizeof(T));                      \
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

FS_DECLARE_VEC(char *, str);

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

typedef enum { RBT_RED = 0, RBT_BLACK } RBTColor;

#define FS_DECLARE_BSTSET(T, tag, cmp)                                                 \
                                                                                       \
                                                                                       \
typedef struct BSTSet_##tag##_node                                                     \
{                                                                                      \
	T							val;                                                   \
	RBTColor					col;                                                   \
	struct BSTSet_##tag##_node	*left;                                                 \
	struct BSTSet_##tag##_node	*right;                                                \
	struct BSTSet_##tag##_node	*parent;                                               \
}	BSTSet_##tag##_node;                                                               \
                                                                                       \
typedef struct BSTSet_##tag                                                            \
{                                                                                      \
	BSTSet_##tag##_node	*tree;                                                         \
	size_t				size;                                                          \
}	BSTSet_##tag;                                                                      \
                                                                                       \
static inline void _bstset_##tag##_depth_r(BSTSet_##tag##_node *node,                  \
												int depth, int *max)                   \
{                                                                                      \
	if (node == NULL)                                                                  \
		return ;                                                                       \
	if (depth > *max)                                                                  \
		*max = depth;                                                                  \
	_bstset_##tag##_depth_r(node->left, depth + 1, max);                               \
	_bstset_##tag##_depth_r(node->right, depth + 1, max);                              \
}                                                                                      \
                                                                                       \
static inline int _bstset_##tag##_depth(BSTSet_##tag *set)                             \
{                                                                                      \
	int depth = 0;                                                                     \
                                                                                       \
	_bstset_##tag##_depth_r(set->tree, 0, &depth);                                     \
	return depth;                                                                      \
}                                                                                      \
                                                                                       \
void	draw_bstset_##tag##_r(BSTSet_##tag##_node *node, int inv_depth, int x, int y, Vec_str *canvas)\
{                                                                                      \
	if (node == NULL)                                                                  \
		return ;                                                                       \
	canvas->data[y][x] = (node->col == RBT_RED) ? 'R' : 'B';                           \
	int	x_diff = 2;                                                                    \
	int	y_diff = 1;                                                                    \
	for (int i = 1; i < inv_depth; i++)                                                \
	{                                                                                  \
		x_diff *= 2;                                                                   \
		y_diff *= 2;                                                                   \
	}                                                                                  \
	if (node->right != NULL)                                                           \
	{                                                                                  \
		for (int i = 1; i < x_diff; i++)                                               \
			canvas->data[y][x + i] = '-';                                              \
		draw_bstset_##tag##_r(node->right, inv_depth - 1, x + x_diff, y, canvas);      \
	}                                                                                  \
	if (node->left != NULL)                                                            \
	{                                                                                  \
		for (int i = 1; i < y_diff; i++)                                               \
			canvas->data[y + i][x] = '|';                                              \
		draw_bstset_##tag##_r(node->left, inv_depth - 1, x, y + y_diff, canvas);       \
	}                                                                                  \
}                                                                                      \
                                                                                       \
Vec_str	*draw_bstset_##tag(BSTSet_##tag *set)                                          \
{                                                                                      \
	Vec_str *canvas = vec_str_init();                                                  \
                                                                                       \
	int max_depth = _bstset_##tag##_depth(set);                                        \
	int width = 1;                                                                     \
	int height = 1;                                                                    \
	int scale_x = 2;                                                                   \
	int scale_y = 1;                                                                   \
                                                                                       \
	for (int i = 1; i <= max_depth; i++)                                               \
	{                                                                                  \
		width += scale_x;                                                              \
		height += scale_y;                                                             \
		scale_x *= 2;                                                                  \
		scale_y *= 2;                                                                  \
	}                                                                                  \
	                                                                                   \
	for (int i = 0; i < height; i++)                                                   \
	{                                                                                  \
		vec_str_push(canvas, calloc(width + 1, sizeof(char)));                         \
		memset(canvas->data[i], ' ', width);                                           \
	}                                                                                  \
	                                                                                   \
	draw_bstset_##tag##_r(set->tree, max_depth, 0, 0, canvas);                         \
                                                                                       \
	size_t i = 0;                                                                      \
	for (; i < canvas->size; i++)                                                      \
	{                                                                                  \
		int j = width - 1;                                                             \
		for (; j >= 0 && canvas->data[i][j] == ' '; j--)                               \
			canvas->data[i][j] = '\0';                                                 \
		if (j == -1)                                                                   \
		{                                                                              \
			size_t size = canvas->size;                                                \
			while (i < size)                                                           \
			{                                                                          \
				free(vec_str_pop(canvas));                                             \
				i++;                                                                   \
			}                                                                          \
			break ;                                                                    \
		}                                                                              \
	}                                                                                  \
                                                                                       \
	return canvas;                                                                     \
}                                                                                      \
                                                                                       \
void	bstset_##tag##_print_rb(BSTSet_##tag *set)                                     \
{                                                                                      \
	Vec_str *canvas = draw_bstset_##tag(set);                                          \
                                                                                       \
	for (size_t i = 0; i < canvas->size; i++)                                          \
	{                                                                                  \
		for (int j = 0; canvas->data[i][j]; j++)                                       \
		{                                                                              \
			char c = canvas->data[i][j];                                               \
			if (c == 'R')                                                              \
				printf("\e[41;30mR\e[m");                                              \
			else if (c == 'B')                                                         \
				printf("\e[47;30mR\e[m");                                              \
			else                                                                       \
				printf("%c", c);                                                       \
		}                                                                              \
		printf("\n");                                                                  \
	}                                                                                  \
	vec_str_free(canvas, (void (*)(char *))free);                                      \
}                                                                                      \
                                                                                       \
static inline BSTSet_##tag *bstset_##tag##_init()                                      \
{                                                                                      \
	BSTSet_##tag *set = calloc(1, sizeof(*set));                                       \
                                                                                       \
	return set;                                                                        \
}                                                                                      \
                                                                                       \
static inline void _bstset_##tag##_rotate_left(BSTSet_##tag *set,                      \
												BSTSet_##tag##_node *node)             \
{                                                                                      \
	BSTSet_##tag##_node *right = node->right;                                          \
                                                                                       \
	node->right = right->left;                                                         \
	if (right->left != NULL)                                                           \
		right->left->parent = node;                                                    \
                                                                                       \
	right->parent = node->parent;                                                      \
	if (node->parent == NULL)                                                          \
		set->tree = right;                                                             \
	else if (node == node->parent->left)                                               \
		node->parent->left = right;                                                    \
	else                                                                               \
		node->parent->right = right;                                                   \
                                                                                       \
	right->left = node;                                                                \
	node->parent = right;                                                              \
}                                                                                      \
                                                                                       \
static inline void _bstset_##tag##_rotate_right(BSTSet_##tag *set,                     \
												BSTSet_##tag##_node *node)             \
{                                                                                      \
	BSTSet_##tag##_node *left = node->left;                                            \
                                                                                       \
	node->left = left->right;                                                          \
	if (left->right != NULL)                                                           \
		left->right->parent = node;                                                    \
                                                                                       \
	left->parent = node->parent;                                                       \
	if (node->parent == NULL)                                                          \
		set->tree = left;                                                              \
	else if (node == node->parent->left)                                               \
		node->parent->left = left;                                                     \
	else                                                                               \
		node->parent->right = left;                                                    \
                                                                                       \
	left->right = node;                                                                \
	node->parent = left;                                                               \
}                                                                                      \
                                                                                       \
/* static inline void print_bstset_##tag(BSTSet_##tag *set) */                         \
                                                                                       \
static inline void _bstset_##tag##_balance_insert(BSTSet_##tag *set,                   \
													BSTSet_##tag##_node *node)         \
{                                                                                      \
	while (node->parent != NULL && node->parent->col == RBT_RED)                       \
	{                                                                                  \
		if (node->parent == node->parent->parent->left)                                \
		{                                                                              \
			BSTSet_##tag##_node *uncle = node->parent->parent->right;                  \
			if (uncle != NULL && uncle->col == RBT_RED)                                \
			{                                                                          \
				node->parent->col = RBT_BLACK;                                         \
				uncle->col = RBT_BLACK;                                                \
				node->parent->parent->col = RBT_RED;                                   \
				node = node->parent->parent;                                           \
			}                                                                          \
			else                                                                       \
			{                                                                          \
				if (node == node->parent->right)                                       \
				{                                                                      \
					node = node->parent;                                               \
					_bstset_##tag##_rotate_left(set, node);                            \
				}                                                                      \
				node->parent->col = RBT_BLACK;                                         \
				node->parent->parent->col = RBT_RED;                                   \
				_bstset_##tag##_rotate_right(set, node->parent->parent);               \
			}                                                                          \
		}                                                                              \
		else                                                                           \
		{                                                                              \
			BSTSet_##tag##_node *uncle = node->parent->parent->left;                   \
			if (uncle != NULL && uncle->col == RBT_RED)                                \
			{                                                                          \
				node->parent->col = RBT_BLACK;                                         \
				uncle->col = RBT_BLACK;                                                \
				node->parent->parent->col = RBT_RED;                                   \
				node = node->parent->parent;                                           \
			}                                                                          \
			else                                                                       \
			{                                                                          \
				if (node == node->parent->left)                                        \
				{                                                                      \
					node = node->parent;                                               \
					_bstset_##tag##_rotate_right(set, node);                           \
				}                                                                      \
				node->parent->col = RBT_BLACK;                                         \
				node->parent->parent->col = RBT_RED;                                   \
				_bstset_##tag##_rotate_left(set, node->parent->parent);                \
			}                                                                          \
		}                                                                              \
	}                                                                                  \
	set->tree->col = RBT_BLACK;                                                        \
}                                                                                      \
                                                                                       \
static inline int bstset_##tag##_insert(BSTSet_##tag *set, T val)                      \
{                                                                                      \
	BSTSet_##tag##_node *current = set->tree;                                          \
	BSTSet_##tag##_node *parent = NULL;                                                \
	int (*cmpf)(T, T) = (int (*)(T, T))cmp;                                            \
	int	diff = 0;                                                                      \
                                                                                       \
	while (current != NULL)                                                            \
	{                                                                                  \
		parent = current;                                                              \
		diff = (cmpf == NULL)                                                          \
			? SAFE_CMP(val, current->val)                                              \
			: cmpf(val, current->val);                                                 \
		if (diff > 0)                                                                  \
			current = current->right;                                                  \
		else if (diff < 0)                                                             \
			current = current->left;                                                   \
		else                                                                           \
			return 1;                                                                  \
	}                                                                                  \
                                                                                       \
	BSTSet_##tag##_node *new = calloc(1, sizeof(*new));                                \
	new->val = val;                                                                    \
	new->parent = parent;                                                              \
	if (parent == NULL)                                                                \
		set->tree = new;                                                               \
	else if (diff > 0)                                                                 \
		parent->right = new;                                                           \
	else                                                                               \
		parent->left = new;                                                            \
	set->size++;                                                                       \
                                                                                       \
	/* printf("\e[?25l");                                                                 \
	printf("\e[2J\e[H");                                                               \
	bstset_##tag##_print_rb(set);                                                      \
	getchar(); */                                                                         \
	_bstset_##tag##_balance_insert(set, new);                                          \
	/* printf("\e[2J\e[H");                                                               \
	bstset_##tag##_print_rb(set);                                                      \
	getchar();                                                                         \
	printf("\e[?25h"); */                                                                \
	                                                                                   \
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
