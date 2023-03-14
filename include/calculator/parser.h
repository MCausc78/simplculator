#ifndef CALCULATOR_PARSER_H
#define CALCULATOR_PARSER_H 1

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "calculator/ast/expression.h"

#define TOKEN_PLUS             '+'
#define TOKEN_BITWISE_NOT      '~'
#define TOKEN_DIVIDE           '/'
#define TOKEN_MODULO           '%'
#define TOKEN_MULTIPLY         '*'
#define TOKEN_MINUS            '-'
#define TOKEN_PARENTHESS_BEGIN '('
#define TOKEN_PARENTHESS_END   ')'

enum parser_error
{
	PARSER_ERR_INVALID_CHARACTER,
	PARSER_ERR_INVALID_OPERATOR,
	PARSER_ERR_PARENTHESS_NOT_CLOSED,
	PARSER_ERR_PARENTHESS_NOT_OPENED,
	PARSER_ERR_NONE,
};

typedef struct calculator_parser
{
	const char* text;
	int deep_level;
	enum parser_error parser_errno;
} calculator_parser_t;

calculator_parser_t* calculator_parser_new(
	const char*);

void calculator_parser_free(
	calculator_parser_t*);

struct ast_expression* calculator_parser_parse(
	calculator_parser_t* self);

struct ast_expression* calculator_parser_parse_expression(
	calculator_parser_t* self);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CALCULATOR_EVALULATE_H */
