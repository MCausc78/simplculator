#include <errno.h>
#include "calculator/ast/expression.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ast_free_expression(struct ast_expression* expression)
{
	if (expression == NULL) {
		fprintf(stderr, "ast_free_expression: fatal error: null expression (`expression` was %p)\n", (void*) expression);
		exit(6);
	}
	switch (expression->type) {
	case AST_EXPRESSION_UNARY:
		ast_free_expression(expression->expression.unary->lhs);
		free(expression);
		break;
	case AST_EXPRESSION_BINARY:
		;
		struct ast_expression* lhs = expression->expression.binary->lhs;
		if (lhs != NULL) {
			ast_free_expression(lhs);
		}
		struct ast_expression* rhs = expression->expression.binary->rhs;
		if (rhs != NULL) {
			ast_free_expression(rhs);
		}
		free(expression);
		break;
	case AST_EXPRESSION_NUMBER:
		free(expression->expression.number);
		break;
	default:
		fprintf(stderr, "ast_free_expression: fatal error: unknown expression type: %d\n", expression->type);
		exit(7);
	}
}

struct ast_expression* ast_make_unary_expression(
	enum ast_unary_expression_type type,
	struct ast_expression* lhs)
{
	struct ast_expression* expression = malloc(sizeof(struct ast_expression));
	if (expression == NULL) {
		return NULL;
	}
	expression->type = AST_EXPRESSION_UNARY;
	expression->expression.unary = malloc(sizeof(struct ast_unary_expression));
	if (expression->expression.unary == NULL) {
		free(expression);
		return NULL;
	}
	expression->expression.unary->type = type;
	expression->expression.unary->lhs = lhs;
	return expression;
}

struct ast_expression* ast_make_binary_expression(
	enum ast_binary_expression_type type,
	struct ast_expression* lhs,
	struct ast_expression* rhs)
{
	struct ast_expression* expression = malloc(sizeof(struct ast_expression));
	if (expression == NULL) {
		return NULL;
	}
	expression->type = AST_EXPRESSION_BINARY;
	expression->expression.binary = malloc(sizeof(struct ast_binary_expression));
	if (expression->expression.binary == NULL) {
		free(expression);
		return NULL;
	}
	expression->expression.binary->type = type;
	expression->expression.binary->lhs = lhs;
	expression->expression.binary->rhs = rhs;
	return expression;
}

struct ast_expression* ast_make_number_expression(
	number_t lhs)
{
	struct ast_expression* expression = malloc(sizeof(struct ast_expression));
	if (expression == NULL) {
		return NULL;
	}
	expression->type = AST_EXPRESSION_NUMBER;
	expression->expression.number = malloc(sizeof(struct ast_number_expression));
	if (expression->expression.number == NULL) {
		free(expression);
		return NULL;
	}
	expression->expression.number->lhs = lhs;
	return expression;
}
