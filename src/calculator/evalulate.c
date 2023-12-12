#include "calculator/ast/expression.h"
#include <stdio.h>
#include <stdlib.h>

number_t ast_evalulate(struct ast_expression* expression, int* err)
{
	if (expression == NULL) {
		fprintf(stderr, "ast_evalulate: fatal error: null expression\n");
		exit(1);
	}
	switch (expression->type) {
	case AST_EXPRESSION_UNARY:
		switch (expression->expression.unary->type) {
		case AST_UNARY_EXPRESSION_BITWISE_NOT:
			return ~ast_evalulate(expression->expression.unary->lhs, err);
		case AST_UNARY_EXPRESSION_NEGATE:
			return -ast_evalulate(expression->expression.unary->lhs, err);
		default:
			fprintf(stderr, "ast_evalulate: fatal error: unknown unary expression type: %d", expression->expression.unary->type);
			exit(3);
		}
		break;
	case AST_EXPRESSION_BINARY:
		switch (expression->expression.binary->type) {
		case AST_BINARY_EXPRESSION_ADD:
			return ast_evalulate(expression->expression.binary->lhs, err) + ast_evalulate(expression->expression.binary->rhs, err);
		case AST_BINARY_EXPRESSION_DIV:
      {
        number_t x = ast_evalulate(expression->expression.binary->lhs, err);
        number_t y = ast_evalulate(expression->expression.binary->rhs, err);
        if (y == 0) {
          if (err)
            *err = 1;
          return 0;
        }
			  return x / y;
      }
		case AST_BINARY_EXPRESSION_MOD:
      {
        number_t x = ast_evalulate(expression->expression.binary->lhs, err);
        number_t y = ast_evalulate(expression->expression.binary->rhs, err);
        if (y == 0) {
          if (err)
            *err = 1;
          return 0;
        }
			  return x % y;
      }
		case AST_BINARY_EXPRESSION_MUL:
			return ast_evalulate(expression->expression.binary->lhs, err) * ast_evalulate(expression->expression.binary->rhs, err);
		case AST_BINARY_EXPRESSION_SUB:
			return ast_evalulate(expression->expression.binary->lhs, err) - ast_evalulate(expression->expression.binary->rhs, err);
		default:
			__builtin_unreachable();
		}
		break;
	case AST_EXPRESSION_NUMBER:
		return expression->expression.number->lhs;
	default:
		__builtin_unreachable();
	}
}

int deep_index = 0;

void ast_indent(FILE* out)
{
	for (int i = 0; i < deep_index; ++i) {
		fputc(' ', out);
	}
}

void ast_dump(FILE* out, struct ast_expression* expression)
{
	if (expression == NULL) {
		fprintf(out, "nil");
		return;
	}
	switch (expression->type) {
	case AST_EXPRESSION_UNARY:
		/* Unary expression */
		fputs("AstUnaryExpression(\n", out);
		++deep_index;
		ast_indent(out);
		fputs("operator=", out);
		struct ast_unary_expression* unary_expression = expression->expression.unary;
		switch (unary_expression->type) {
		case AST_UNARY_EXPRESSION_BITWISE_NOT:
			fputs("BITWISE_NOT", out);
			break;
		case AST_UNARY_EXPRESSION_NEGATE:
			fputs("NEGATE", out);
			break;
		default:
			fprintf(out, "UNKNOWN(%d)", unary_expression->type);
		}
		fputs(",\n", out);
		ast_indent(out);
		fputs("operand=", out);
		ast_dump(out, unary_expression->lhs);
		fputc('\n', out);
		--deep_index;
		ast_indent(out);
		fputc(')', out);
		break;
	case AST_EXPRESSION_BINARY:
		/* Binary expression */
		fputs("AstBinaryExpression(\n", out);
		++deep_index;
		ast_indent(out);
		fputs("operator=", out);
		struct ast_binary_expression* binary_expression = expression->expression.binary;
		switch (binary_expression->type) {
		case AST_BINARY_EXPRESSION_ADD:
			fputs("ADD", out);
			break;
		case AST_BINARY_EXPRESSION_DIV:
			fputs("DIV", out);
			break;
		case AST_BINARY_EXPRESSION_MOD:
			fputs("MOD", out);
			break;
		case AST_BINARY_EXPRESSION_MUL:
			fputs("MUL", out);
			break;
		case AST_BINARY_EXPRESSION_SUB:
			fputs("SUB", out);
			break;
		default:
			fprintf(out, "UNKNOWN(%d)", binary_expression->type);
		}
		fputs(",\n", out);
		ast_indent(out);
		fputs("left=", out);
		ast_dump(out, binary_expression->lhs);
		fputs(",\n", out);
		ast_indent(out);
		fputs("right=", out);
		ast_dump(out, binary_expression->rhs);
		fputc('\n', out);
		--deep_index;
		ast_indent(out);
		fputc(')', out);
		break;
	case AST_EXPRESSION_NUMBER:
		fprintf(out, "AstNumber(%d)", expression->expression.number->lhs);
		break;
	default:
		fprintf(out, "Error(%d)", expression->type);
	}
}
