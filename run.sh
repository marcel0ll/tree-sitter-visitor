clang                                   \
  -I ../tree-sitter/lib/include            \
  tree-sitter-visitor.c                    \
  ../tree-sitter/libtree-sitter.a          \
  ../tree-sitter-javascript/src/parser.c         \
  ../tree-sitter-javascript/src/scanner.c         \
  ./libs/hashmap.c -c

