#define _CRT_SECURE_NO_WARNINGS

#include "calculator/ast/expression.h"
#include "calculator/evalulate.h"
#include "calculator/parser.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FLAGS_INTERACTIVE (1 << 0)
#define FLAGS_DISPLAY_TREE (1 << 1)

#if defined(__clang__)
#define COMPILER "clang"
#define COMPILER_VERSION __clang_version__
#elif defined(__GNUC__)
#define COMPILER "GCC"
#define COMPILER_VERSION __VERSION__
#elif defined(_MSC_VER)
#define COMPILER "MSVC"
#define COMPILER_VERSION "?"
#else
#define COMPILER "Unknown"
#define COMPILER_VERSION "Unknown"
#endif

struct calculator_version
{
	int major;
	int minor;
};

const struct calculator_version version = { 1, 0 };

int main(int argc, char** argv)
{
	(void) argc;
	int flags = 0;
	for (; *++argv; ) {
		char* arg = *argv;
		if (*arg++ != '-') {
			fprintf(stderr, "invalid argument: \"%s\"\n", arg - 1);
			continue;
		}
		if (*arg == '-') {
			++arg;
		}
		if (strcmp(arg, "i") == 0 || strcmp(arg, "interactive") == 0) {
			flags |= FLAGS_INTERACTIVE;
			continue;
		}
		if (strcmp(arg, "d") == 0 ||
			strcmp(arg, "display-tree") == 0 ||
			strcmp(arg, "displaytree") == 0)
		{
			flags |= FLAGS_DISPLAY_TREE;
			continue;
		}
		fprintf(stderr, "invalid flag: \"%s\"\n", arg);
    return 1;
	}
	char buffera[255];
	char* buffer = buffera;
	calculator_parser_t* parser = calculator_parser_new(NULL);
	if (parser == NULL) {
		fprintf(stderr, "Failed to initialize parser: \"%s\"\n", strerror(errno));
		return -1;
	}
	if ((flags & FLAGS_INTERACTIVE) == 0) {
		printf("Calculator %d.%d (" __DATE__ ", "  __TIME__ ") [" COMPILER " v" COMPILER_VERSION "]\n",
			version.major,
			version.minor);
	}
	char* line = NULL;
	do {
		if ((flags & FLAGS_INTERACTIVE) == 0) {
			fputs("=> ", stdout);
		}
		line = fgets(buffer, 255, stdin);
		if (line == NULL) {
			break;
		}
		if (strcspn("\t\n\v\f\r ", buffer) == strlen(buffer)) {
			continue;
		}
		parser->text = buffer;
		parser->deep_level = 0;
		parser->parser_errno = PARSER_ERR_NONE;
		struct ast_expression* expression = calculator_parser_parse_expression(parser);
		if (expression != NULL) {
			if ((flags & FLAGS_DISPLAY_TREE) != 0) {
				ast_dump(stdout, expression);
				fputc('\n', stdout);
				ast_free_expression(expression);
				continue;
			}
      int err = 0;
      int r = ast_evalulate(expression, &err);
      if (err != 0) {
        static const char* errs[] = {
          "division by zero"
        };
        fprintf(stderr, "error: %s\n", errs[err - 1]);
      }
      else 
			  printf("%d\n", r);
			ast_free_expression(expression);
			continue;
		}
		fprintf(stderr, "invalid expression: ");
		switch (parser->parser_errno) {
		case PARSER_ERR_INVALID_CHARACTER:
			fprintf(stderr, "invalid character");
			break;
		case PARSER_ERR_INVALID_OPERATOR:
			fprintf(stderr, "invalid operator");
			break;
		case PARSER_ERR_PARENTHESS_NOT_CLOSED:
			fprintf(stderr, "parenthess not closed");
			break;
		case PARSER_ERR_PARENTHESS_NOT_OPENED:
			fprintf(stderr, "parenthess not opened");
			break;
		default:
			fprintf(stderr, "unknown errno: %d", parser->parser_errno);
		}
		fputc('\n', stderr);
	} while (line);
	calculator_parser_free(parser);
	return 0;
}
