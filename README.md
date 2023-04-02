# simplculator

Toy calculator written in pure C language

# Build

```bash
$ mkdir build
$ cd build
$ cmake ..
...
$ make
...
$ cd ..
$ # A executable is in `build/simplculator`
```

# Examples

```bash
$ # By default, simplculator runs in REPL mode, so disable interactive mode by `-i`
$
$ echo "1+1" | ./build/simplculator -i
2
$ echo "1 + 1" | ./build/simplculator -i
1
$ echo "1+2*3" | ./build/simplculator -i
9
$ echo "1 + 2 * 3" | ./build/simplculator -i
9
$ echo "2 * (1 + 2)" | ./build/simplculator -i
6
$ # You also can view AST:
$ echo "1+1" | ./build/simplculator -d -i
AstBinaryExpression(
 operator=ADD,
 left=AstNumber(1),
 right=AstNumber(1)
)
$ echo "1+2*3" |./build/simplculator -d -i
AstBinaryExpression(
 operator=MUL,
 left=AstBinaryExpression(
  operator=ADD,
  left=AstNumber(1),
  right=AstNumber(2)
 ),
 right=AstNumber(3)
)
$ echo "2 * (1 + 2)" | ./build/simplculator -d -i
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
