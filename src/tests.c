#include "fslib.h"
#include <stddef.h>
#include <stdio.h>
#include <time.h>

void	print_sep(void)
{
	printf("-----------------------------\n");
}

void	print_header(char *str)
{
	int len = strlen(str) + 4;
	printf("\n");
	for (int i = 0; i < len; i++)
		printf("#");
	printf("\n");
	printf("# %s #\n", str);
	for (int i = 0; i < len; i++)
		printf("#");
	printf("\n\n");
}

FS_DECLARE_VEC(int, int);
// FS_DECLARE_VEC(char *, str);

void	test_vec_int(void)
{
	Vec_int *vec = vec_int_init();
	
	print_header("Testing Vec_int");
	for (int i = 31; i >=0; i--)
	{
		vec_int_push(vec, i);
	}

	for (size_t i = 0; i < vec->size; i++)
		printf("vec[%ld]: \t%d\n", i, vec->data[i]);

	vec_int_free(vec, NULL);
}

void test_vec_str(int argc, char **argv)
{
	Vec_str *vec2 = vec_str_init();

	print_header("Testing Vec_str");
	for (int i = 0; i < argc; i++)
		vec_str_push(vec2, argv[i]);

	print_sep();
	for (size_t i = 0; i < vec2->size; i++)
		printf("vec[%ld]: \t%s\n", i, vec2->data[i]);

	print_sep();
	for (int i = 0; vec2->size > 0; i++)
		printf("pop no. %d:\t%s\n", i, vec_str_pop(vec2));

	vec_str_free(vec2, NULL);
}

FS_DECLARE_ARRQUEUE(int, int);
FS_DECLARE_ARRQUEUE(char *, str);

void test_arrqueue_int(void)
{
	ArrQueue_int *q = arrqueue_int_init();

	print_header("Testing ArrQueue_int");
	for (int i = 1; i <= 12; i++)
	{
		printf("\e[32mpushing\e[m: %d\n", i );
		arrqueue_int_push_tail(q, i);
	}

	while (q->size > 6)
		printf("\e[31mpopping\e[m: %d\n", arrqueue_int_pop_head(q));

	for (int i = 13; i <= 20; i++)
	{
		printf("\e[32mpushing\e[m %d\n", i);
		arrqueue_int_push_tail(q, i);
	}
	while (q->size > 0)
		printf("\e[31mpopping\e[m: %d\n", arrqueue_int_pop_head(q));

	printf("head: %ld\ntail: %ld\n", q->head, q->tail);
	arrqueue_int_free(q, NULL);
	print_sep();

	q = arrqueue_int_init();
	for (int i = 0; i < 20; i += 2)
	{
		printf("\e[32mpushing head\e[m: %d\n", i);
		arrqueue_int_push_head(q, i);
		printf("\e[34mpushing tail\e[m: %d\n", i + 1);
		arrqueue_int_push_tail(q, i + 1);
	}
	while (1)
	{
		if (q->size == 0)
			break;
		printf("\e[33mpopping tail\e[m: %d\n", arrqueue_int_pop_tail(q));
		if (q->size == 0)
			break;
		printf("\e[31mpopping head\e[m: %d\n", arrqueue_int_pop_head(q));
	}

	printf("head: %ld\ntail: %ld\n", q->head, q->tail);
	arrqueue_int_free(q, NULL);
}

void test_arrqueue_str(int argc, char **argv)
{
	ArrQueue_str *q = arrqueue_str_init();

	print_header("Testing ArrQueue_str");
	for (int i = 0; i < argc; i++)
	{
		printf("\e[32mpushing head\e[m: %s\n", argv[i]);
		arrqueue_str_push_head(q, strdup(argv[i]));
	}
	while (1)
	{
		if (q->size == 0)
			break;
		char *str = arrqueue_str_pop_tail(q);
		printf("\e[33mpopping tail\e[m: %s\n", str);
		free(str);
		if (q->size == 0)
			break;
		str = arrqueue_str_pop_head(q);
		printf("\e[31mpopping head\e[m: %s\n", str);
		free(str);
	}

	printf("head: %ld\ntail: %ld\n", q->head, q->tail);
	arrqueue_str_free(q, (void (*)(char *))free);
}

FS_DECLARE_BSTSET(char *, str, strcmp);
FS_DECLARE_BSTSET(int, int, NULL);

void	test_bstset_str(int argc, char **argv)
{
	BSTSet_str set = {};

	print_header("Testing BSTSet_str");
	for (int i = 0; i < argc; i++)
	{
		printf("\e[3%dmAdding to set:\e[m %s\n", 2 - bstset_str_insert(&set, strdup(argv[i])), argv[i]);
	}
	printf("\e[3%dmAdding to set:\e[m %s\n", 2 - bstset_str_insert(&set, "fintan"), "fintan");

	print_sep();
	printf("\e[3%dmSet contains:\e[m %s\n", 1 + bstset_str_contains(&set, "old"), "old");
	printf("\e[3%dmSet contains:\e[m %s\n", 1 + bstset_str_contains(&set, "27"), "27");
	printf("\e[3%dmSet contains:\e[m %s\n", 1 + bstset_str_contains(&set, "bob"), "bob");

	print_sep();
	printf("\e[31mRemoving from set:\e[m %s\n", "old");
	bstset_str_remove(&set, "old", (void (*)(char *))free);
	printf("\e[31mRemoving from set:\e[m %s\n", "27");
	bstset_str_remove(&set, "27", (void (*)(char *))free);
	printf("\e[31mRemoving from set:\e[m %s\n", "smyth");
	bstset_str_remove(&set, "smyth", (void (*)(char *))free);
	for (int i = 0; i < argc; i++)
	{
		printf("\e[3%dmSet contains:\e[m %s\n", 1 + bstset_str_contains(&set, argv[i]), argv[i]);
	}

	bstset_str_clear(&set, (void (*)(char *))free);
}

void print_bstset_int(BSTSet_int_node *node)
{
	if (node == NULL)
		return ;
	print_bstset_int(node->left);
	printf("%d\n", node->val);
	print_bstset_int(node->right);
}

void test_bstset_int(void)
{
	BSTSet_int set = {};

	srand(time(NULL));
	print_header("Testing BSTSet_int");
	for (int i = 0; i < 40; i++)
	{
		int num = rand();
		printf("\e[32mInserting:\e[m %d\n", num);
		bstset_int_insert(&set, num);
	}

	print_sep();
	print_bstset_int(set.tree);
	bstset_int_clear(&set, NULL);
}

FS_DECLARE_QUEUE(int, int);

void test_queue_int(void)
{
	Queue_int q = {};

	print_header("Testing Queue_int");
	for (int i = 1; i <= 20; i++)
	{
		printf("\e[32mPushing head:\e[m %d\n", i);
		queue_int_push_head(&q, i);
	}

	for (int i = 5; i > 0; i--)
		printf("\e[31mPopping head:\e[m %d\n", queue_int_pop_head(&q));
	for (int i = 5; i > 0; i--)
		printf("\e[33mPopping tail:\e[m %d\n", queue_int_pop_tail(&q));
	for (int i = 5; i > 0; i--)
		printf("\e[31mPopping head:\e[m %d\n", queue_int_pop_head(&q));
	while (q.size > 0)
		printf("\e[33mPopping tail:\e[m %d\n", queue_int_pop_tail(&q));
	queue_int_clear(&q, NULL);
}

// void	draw_bstset_int_r(BSTSet_int_node *node, int inv_depth, int x, int y, Vec_str *canvas)
// {
// 	if (node == NULL)
// 		return ;
// 	canvas->data[y][x] = (node->col == RBT_RED) ? 'R' : 'B';
// 	int	x_diff = 3;
// 	int	y_diff = 2;
// 	for (int i = 1; i < inv_depth; i++)
// 	{
// 		x_diff *= 2;
// 		y_diff *= 2;
// 	}
// 	if (node->right != NULL)
// 	{
// 		for (int i = 1; i < x_diff; i++)
// 			canvas->data[y][x + i] = '-';
// 		draw_bstset_int_r(node->right, inv_depth - 1, x + x_diff, y, canvas);
// 	}
// 	if (node->left != NULL)
// 	{
// 		for (int i = 1; i < y_diff; i++)
// 			canvas->data[y + i][x] = '|';
// 		draw_bstset_int_r(node->left, inv_depth - 1, x, y + y_diff, canvas);
// 	}
// }

// Vec_str	*draw_bstset_int(BSTSet_int *set)
// {
// 	Vec_str *canvas = vec_str_init();
//
// 	int max_depth = _bstset_int_depth(set);
// 	printf("depth: %d\n", max_depth);
//
// 	int width = 1;
// 	int height = 1;
// 	int scale_x = 3;
// 	int scale_y = 2;
// 	for (int i = 1; i <= max_depth; i++)
// 	{
// 		width += scale_x;
// 		height += scale_y;
// 		scale_x *= 2;
// 		scale_y *= 2;
// 	}
//
// 	for (int i = 0; i < height; i++)
// 	{
// 		vec_str_push(canvas, calloc(width + 1, sizeof(char)));
// 		memset(canvas->data[i], ' ', width);
// 		// printf("<%s>\n", canvas->data[i]);
// 	}
// 	printf("width: %d\nheight: %d\n", width, height);
//
// 	draw_bstset_int_r(set->tree, max_depth, 0, 0, canvas);
//
// 	size_t i = 0;
// 	for (; i < canvas->size; i++)
// 	{
// 		int j = width - 1;
// 		for (; j >= 0 && canvas->data[i][j] == ' '; j--)
// 			canvas->data[i][j] = '\0';
// 		if (j == -1)
// 		{
// 			size_t size = canvas->size;
// 			while (i < size)
// 			{
// 				free(vec_str_pop(canvas));
// 				i++;
// 			}
// 			break ;
// 		}
// 	}
//
// 	return canvas;
// }

// void	print_rb_tree(BSTSet_int *set)
// {
// 	Vec_str *canvas = draw_bstset_int(set);
//
// 	for (size_t i = 0; i < canvas->size; i++)
// 	{
// 		for (int j = 0; canvas->data[i][j]; j++)
// 		{
// 			char c = canvas->data[i][j];
// 			if (c == 'R')
// 				printf("\e[41;30mR\e[m");
// 			else if (c == 'B')
// 				printf("\e[47;30mR\e[m");
// 			else
// 				printf("%c", c);
// 		}
// 		printf("\n");
// 	}
// 	vec_str_free(canvas, (void (*)(char *))free);
// }

void	test_print_rb_tree(void)
{
	BSTSet_int set = {};

	print_header("Printing BSTSet_int");

	srand(time(NULL));
	printf("\e[?25l");
	for (int i = 0; i < 150; i++)
	{
		int num = rand();
		// printf("\e[32mInserting:\e[m %d\n", num);
		bstset_int_insert(&set, num);
		// printf("\e[2J\e[H");
		// bstset_int_print_rb(&set);
		// getchar();
	}
	printf("\e[?25h");

	bstset_int_print_rb(&set);
	// printf("max_depth: %d\n", _bstset_int_depth(&set));

	bstset_int_clear(&set, NULL);
}

int main(int argc, char **argv)
{
	printf("Tests running...\n");

	// test_vec_int();
	// test_vec_str(argc, argv);
	// test_arrqueue_int();
	// test_arrqueue_str(argc, argv);
	// test_bstset_str(argc, argv);
	// test_bstset_int();
	// test_queue_int();
	test_print_rb_tree();
}
