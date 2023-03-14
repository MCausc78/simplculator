#!/bin/bash

FILES="$(cat <<EOF
calculator/ast/expression.c
calculator/evalulate.c
calculator/main.c
calculator/parser.c
EOF
)"

CC=clang
CCFLAGS="-std=gnu11 -Wall -Werror -O3"
CPPFLAGS="-I. -I./include"

LD=clang
LDFLAGS=

rm -fr bin/ obj/
mkdir -p bin/ obj/

for file in $FILES
do
	echo " CC $file"
	mkdir -p "obj/$(dirname $file)"
	$CC $CPPFLAGS -c $CCFLAGS -o obj/$file.o src/$file
done

shopt -s globstar
echo " LD"
$LD $LDFLAGS -o bin/calculator obj/**/**.o
