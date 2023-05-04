#include "calculator/parser.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

calculator_parser_t* calculator_parser_new(
	const char* text)
{
	calculator_parser_t* parser = malloc(sizeof(calculator_parser_t));
	if (parser != NULL) {
		parser->text = text;
		parser->deep_level = 0;
		parser->parser_errno = PARSER_ERR_NONE;
	}
	return parser;
}

void calculator_parser_free(
	calculator_parser_t* self)
{
	if (self == NULL) {
		return;
	}
	free(self);
}

void calculator_parser_skip_ws(
	calculator_parser_t* self)
{
	if (*self->text == '\0') {
		return;
	}
	while (strchr("\t\n\v\f\r ", *self->text) != NULL) {
		++self->text;
	}
}

struct ast_expression* calculator_parser_parse(
	calculator_parser_t* self)
{
	calculator_parser_skip_ws(self);
	switch (*(self->text++)) {
	case TOKEN_MINUS:
		;
		/* Negate expression */
		{
			calculator_parser_skip_ws(self);
			struct ast_expression* lhs = calculator_parser_parse(self);
			if (lhs == NULL) {
				return NULL;
			}
			return ast_make_unary_expression(AST_UNARY_EXPRESSION_NEGATE, lhs);
		}
		break;
	case TOKEN_PARENTHESS_BEGIN:
		;
		/* Higher priority */
		{
			++self->deep_level;
			struct ast_expression* lhs = calculator_parser_parse_expression(self);
			if (lhs == NULL) {
				--self->deep_level;
				return NULL;
			}
			return lhs;
		}
		break;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		;
		/* Number */
		{
			number_t lhs = *(self->text - 1) - '0';
			for (; isdigit((unsigned char) *self->text); ++self->text)
			{
				lhs = lhs * 10 + ((*self->text) - '0');
			}
			return ast_make_number_expression(lhs);
		}
		break;
	case TOKEN_BITWISE_NOT:
		;
		/* Bitwise NOT */
		{
			struct ast_expression* lhs = calculator_parser_parse(self);
			if (lhs == NULL) {
				return NULL;
			}
			return ast_make_unary_expression(AST_UNARY_EXPRESSION_BITWISE_NOT, lhs);
		}
		break;
	}
	return NULL;
}

struct ast_expression* calculator_parser_parse_expression(
	calculator_parser_t* self)
{
	calculator_parser_skip_ws(self);
	struct ast_expression* lhs = calculator_parser_parse(self);
	if (lhs == NULL) {
		self->parser_errno = PARSER_ERR_INVALID_CHARACTER;
		return NULL;
	}
	enum ast_binary_expression_type type = AST_BINARY_EXPRESSION_NONE;
	while (*self->text != '\0' && *self->text == ' ') {
		++self->text;
	}
	switch (*(self->text++)) {
	case TOKEN_PARENTHESS_END:
		if (self->deep_level <= 0) {
			ast_free_expression(lhs);
			self->parser_errno = PARSER_ERR_INVALID_OPERATOR;
			return NULL;
		}
		--self->deep_level;
		return lhs;
	case TOKEN_PLUS:
		type = AST_BINARY_EXPRESSION_ADD;
		break;
	case TOKEN_DIVIDE:
		type = AST_BINARY_EXPRESSION_DIV;
		break;
	case TOKEN_MODULO:
		type = AST_BINARY_EXPRESSION_MOD;
		break;
	case TOKEN_MULTIPLY:
		type = AST_BINARY_EXPRESSION_MUL;
		break;
	case TOKEN_MINUS:
		type = AST_BINARY_EXPRESSION_SUB;
		break;
	case '\0':
	case '\t':
	case '\n':
	case '\v':
	case '\f':
	case '\r':
	case ' ':
		return lhs;
	default:
		ast_free_expression(lhs);
		return NULL;
	}
	struct ast_expression* rhs = calculator_parser_parse(self);
	if (rhs == NULL) {
		ast_free_expression(lhs);
		return NULL;
	}
	enum ast_binary_expression_type second_type = AST_BINARY_EXPRESSION_NONE;
	while (*self->text != '\0' && *self->text == ' ') {
		++self->text;
	}
	switch (*(self->text++)) {
	case TOKEN_PARENTHESS_END:
		if (self->deep_level <= 0) {
			self->parser_errno = PARSER_ERR_PARENTHESS_NOT_OPENED;
			ast_free_expression(lhs);
			return NULL;
		}
		--self->deep_level;
	case '\0':
	case '\n':
	case '\f':
	case '\r':
		return ast_make_binary_expression(type, lhs, rhs);
	case TOKEN_PLUS:
		second_type = AST_BINARY_EXPRESSION_ADD;
		break;
	case TOKEN_DIVIDE:
		second_type = AST_BINARY_EXPRESSION_DIV;
		break;
	case TOKEN_MODULO:
		second_type = AST_BINARY_EXPRESSION_MOD;
		break;
	case TOKEN_MULTIPLY:
		second_type = AST_BINARY_EXPRESSION_MUL;
		break;
	case TOKEN_MINUS:
		second_type = AST_BINARY_EXPRESSION_SUB;
		break;
	default:
		ast_free_expression(lhs);
		ast_free_expression(rhs);

		self->parser_errno = PARSER_ERR_INVALID_OPERATOR;
		return NULL;
	}
	lhs = ast_make_binary_expression(type, lhs, rhs);
	calculator_parser_skip_ws(self);
	rhs = calculator_parser_parse_expression(self);
	if (rhs == NULL) {
		ast_free_expression(lhs);
		return NULL;
	}
	return ast_make_binary_expression(second_type, lhs, rhs);
}
