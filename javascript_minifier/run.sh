clang                                   \
  -I ../../tree-sitter/lib/include            \
  jsminify.c                    \
  ../../tree-sitter/libtree-sitter.a          \
  ../../tree-sitter-javascript/src/parser.c         \
  ../../tree-sitter-javascript/src/scanner.c         \
  ../libs/hashmap.c         \
  ../tree-sitter-visitor.c         \
  -o jsminify

