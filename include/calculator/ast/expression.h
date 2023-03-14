#ifndef CALCULATOR_AST_EXPRESSION_H
#define CALCULATOR_AST_EXPRESSION_H 1

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdio.h>

typedef int number_t;

enum ast_expression_type
{
	AST_EXPRESSION_UNARY,
	AST_EXPRESSION_BINARY,
	AST_EXPRESSION_NUMBER,
	AST_EXPRESSION_NONE,
};

enum ast_unary_expression_type
{
	AST_UNARY_EXPRESSION_BITWISE_NOT,
	AST_UNARY_EXPRESSION_NEGATE,
	AST_UNARY_EXPRESSION_NONE,
};

enum ast_binary_expression_type
{
	AST_BINARY_EXPRESSION_ADD,  /* `+` */
	AST_BINARY_EXPRESSION_DIV,  /* `/` */
	AST_BINARY_EXPRESSION_MOD,  /* `%` */
	AST_BINARY_EXPRESSION_MUL,  /* `*` */
	AST_BINARY_EXPRESSION_SUB,  /* `-` */
	AST_BINARY_EXPRESSION_NONE, /* ??? */
};

struct ast_expression;
struct ast_unary_expression
{
	enum ast_unary_expression_type type;
	struct ast_expression* lhs;
	struct ast_expression* rhs;
};

struct ast_binary_expression
{
	enum ast_binary_expression_type type;
	struct ast_expression* lhs;
	struct ast_expression* rhs;
};

struct ast_number_expression
{
	number_t lhs;
};

struct ast_expression
{
	enum ast_expression_type type;
	union ast_sub_expression
	{
		struct ast_unary_expression* unary;
		struct ast_binary_expression* binary;
		struct ast_number_expression* number;
	} expression;
};

struct ast_expression* ast_make_unary_expression(
	enum ast_unary_expression_type type,
	struct ast_expression* lhs);

struct ast_expression* ast_make_binary_expression(
	enum ast_binary_expression_type type,
	struct ast_expression* lhs,
	struct ast_expression* rhs);

struct ast_expression* ast_make_number_expression(
	number_t lhs);

void ast_free_expression(struct ast_expression* expression);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
