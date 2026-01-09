/**
 * file: todo.c
 * author: Weston Bishop
 * License: MIT
 * description: Simple CLI todo list app in C.
 */

/* Headers */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Macros */
#define MAX_TODO_LEN 256

/* Types */

/* Function Declarations */
static int add(const char *item);
static int list(void);
static int rm(int index);
static int print_usage(void);

/* Global Variables */

/* Function Definitions */
/* Adds an item to the todo list */
static int add(const char *item)
{
	FILE *fp = fopen("todo.txt", "a");
	if (fp == NULL) {
		perror("Error opening file");
		return 1;
	}

	/* Append content to the file including a newline character */
	fprintf(fp, "%s\n", item);

	fclose(fp);

	printf("Item added to Todo successfully: %s\n", item);
	return 0;
}

/* Lists items in todo list */
static int list()
{
	char buf[MAX_TODO_LEN];

	FILE *fp = fopen("todo.txt", "r");
	if (fp == NULL) {
		perror("Error opening file");
		return 1;
	}

	/* Iterates through file, printing an index with each line */
	int i = 1;
	while (fgets(buf, sizeof buf, fp) != NULL) {
		printf("%d. %s", i, buf);
		i++;
	}

	fclose(fp);

	return 0;
}

/* Removes an item from the todo list using its index */
static int rm(int index)
{
	char buf[MAX_TODO_LEN];

	if (index <= 0) {
		fprintf(stderr, "Error: index must be >= 1\n");
		return 1;
	}
	FILE *in = fopen("todo.txt", "r");
	if (in == NULL) {

		perror("Error opening file");
		return 1;
	}
	FILE *out = fopen("todo.txt.tmp", "w");
	if (out == NULL) {

		perror ("Error opening temp file");
		fclose(in);
		return 1;
	}

	/* Iterates through list and copies lines to temp file, skipping the line with index matching rm's index parameter */
	int i = 1;
	int success = 0;
	while (fgets(buf, sizeof buf, in) != NULL) {
		if (i == index) {
			success = 1;
		} else {
			fputs(buf, out);
		}
		i++;
	}

	fclose(in);
	fclose(out);

	if (!success) {
		fprintf(stderr, "Error: no todo item at index %d\n", index);
		remove("todo.txt.tmp");
		return 1;
	}
	if (rename("todo.txt.tmp", "todo.txt") != 0) {
		perror("Error replacing todo.txt");
		return 1;
	}
	printf("Removed todo item %d\n", index);
	return 0;
}

/* Prints usage */
static int print_usage()
{
	printf(
			"Usage:\n"
			"  todo <command> [arguments]\n"
			"\n"
			"Commands:\n"
			"  add <item>         Add a new todo item\n"
			"  list               List all todo items and their indices\n"
			"  rm <index>         Remove a todo item by index\n"
			"\n"
			"Examples:\n"
			"  todo add \"Buy milk\"\n"
			"  todo list\n"
			"  todo rm 2\n"
	      );

	return 1;
}

/* main */
int main(int argc, char *argv[])
{
	if (argc == 2) {
		if (strcmp(argv[1], "list") == 0) {
			return list();
		} else {
			return print_usage();
		}
	} else if (argc > 2) {
		if (strcmp(argv[1], "add") == 0) {
			return add(argv[2]);
		} else if (strcmp(argv[1], "rm") == 0) {
			int index = atoi(argv[2]);
			if (index == 0){ /* either arg was 0 or was not an integer */
				fprintf(stderr, "Error: Not a valid index\n");
			} else {
				return rm(index);
			}
		} else {
			return print_usage();
		}
	} else {
		return print_usage();
	}
}
