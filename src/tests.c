#include "fslib.h"
#include <stddef.h>
#include <stdio.h>

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
FS_DECLARE_VEC(char *, str);

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

FS_DECLARE_QUEUE(int, int);
FS_DECLARE_QUEUE(char *, str);

void test_queue_int(void)
{
	Queue_int *q = queue_int_init();

	print_header("Testing Queue_int");
	for (int i = 1; i <= 12; i++)
	{
		printf("\e[32mpushing\e[m: %d\n", i );
		queue_int_push_tail(q, i);
	}

	while (q->size > 6)
		printf("\e[31mpopping\e[m: %d\n", queue_int_pop_head(q));

	for (int i = 13; i <= 20; i++)
	{
		printf("\e[32mpushing\e[m %d\n", i);
		queue_int_push_tail(q, i);
	}
	while (q->size > 0)
		printf("\e[31mpopping\e[m: %d\n", queue_int_pop_head(q));

	printf("head: %ld\ntail: %ld\n", q->head, q->tail);
	queue_int_free(q, NULL);
	print_sep();

	q = queue_int_init();
	for (int i = 0; i < 20; i += 2)
	{
		printf("\e[32mpushing head\e[m: %d\n", i);
		queue_int_push_head(q, i);
		printf("\e[34mpushing tail\e[m: %d\n", i + 1);
		queue_int_push_tail(q, i + 1);
	}
	while (1)
	{
		if (q->size == 0)
			break;
		printf("\e[33mpopping tail\e[m: %d\n", queue_int_pop_tail(q));
		if (q->size == 0)
			break;
		printf("\e[31mpopping head\e[m: %d\n", queue_int_pop_head(q));
	}

	printf("head: %ld\ntail: %ld\n", q->head, q->tail);
	queue_int_free(q, NULL);
}

void test_queue_str(int argc, char **argv)
{
	Queue_str *q = queue_str_init();

	print_header("Testing Queue_str");
	for (int i = 0; i < argc; i++)
	{
		printf("\e[32mpushing head\e[m: %s\n", argv[i]);
		queue_str_push_head(q, strdup(argv[i]));
	}
	while (1)
	{
		if (q->size == 0)
			break;
		char *str = queue_str_pop_tail(q);
		printf("\e[33mpopping tail\e[m: %s\n", str);
		free(str);
		if (q->size == 0)
			break;
		str = queue_str_pop_head(q);
		printf("\e[31mpopping head\e[m: %s\n", str);
		free(str);
	}

	printf("head: %ld\ntail: %ld\n", q->head, q->tail);
	queue_str_free(q, (void (*)(char *))free);
}

FS_DECLARE_BSTSET(char *, str, strcmp);

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

int main(int argc, char **argv)
{
	printf("Tests running...\n");

	test_vec_int();
	test_vec_str(argc, argv);
	test_queue_int();
	test_queue_str(argc, argv);
	test_bstset_str(argc, argv);
}
