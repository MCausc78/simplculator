# simplculator

Toy calculator written in pure C language

# Examples

```bash
$ # By default, simplculator runs in REPL mode, so lets enable interactive mode by `-i` :)
$
$ echo "1+1" | ./bin/calculator -i
2
$ echo "1 + 1" | ./bin/calculator -i
1
$ echo "1+2*3" | ./bin/calculator -i
9
$ echo "1 + 2 * 3" | ./bin/calculator -i
9
$ echo "2 * (1 + 2)" | ./bin/calculator -i
6
$ # You also can view AST:
$ echo "1+1" | ./bin/calculator -d -i
AstBinaryExpression(
 operator=ADD,
 left=AstNumber(1),
 right=AstNumber(1)
)
$ echo "1+2*3" | ./bin/calculator -d -i
AstBinaryExpression(
 operator=MUL,
 left=AstBinaryExpression(
  operator=ADD,
  left=AstNumber(1),
  right=AstNumber(2)
 ),
 right=AstNumber(3)
)
$ echo "2 * (1 + 2)" | ./bin/calculator -d -i
AstBinaryExpression(
 operator=MUL,
 left=AstNumber(2),
 right=AstBinaryExpression(
  operator=ADD,
  left=AstNumber(1),
  right=AstNumber(2)
 )
)
```
