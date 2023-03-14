#ifndef CALCULATOR_EVALULATE_H
#define CALCULATOR_EVALULATE_H 1

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "calculator/ast/expression.h"

number_t ast_evalulate(struct ast_expression* expression);

extern int deep_index;

void ast_indent(FILE*);
void ast_dump(FILE*, struct ast_expression* expression);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CALCULATOR_EVALULATE_H */
