#include <string.h>
#include <stdio.h>
#include <tree_sitter/api.h>
#include "libs/hashmap.h"
#include "tree-sitter-visitor.h"

void node_op (TSNode node, struct visit_context * context) {
  char * text = ts_node_text(node, context);
  printf("Found: %s\n", text);
  free(text);
}

int main(int argc, char * argv[]) {
  char * file_path = argv[1];
  if (file_path == NULL) {
    printf("No file passed...\n");
    return 1;
  }
  printf("File passed: %s\n", file_path);

  TSLanguage *tree_sitter_javascript();
  TSParser *parser = ts_parser_new();
  ts_parser_set_language(parser, tree_sitter_javascript());

  const char *source_code = get_source(file_path);

  TSTree *tree = ts_parser_parse_string(
    parser,
    NULL,
    source_code,
    strlen(source_code)
  );

  size_t size = visitor_size();
  struct hashmap *visitors = hashmap_new(size, 0, 0, 0, visitor_hash,
      visitor_compare, NULL);

  TSNode root_node = ts_tree_root_node(tree);

  struct visitor * iden = visitor_new("identifier", node_op);
  hashmap_set(visitors, iden);

  struct visit_context *context = context_new(source_code, visitors);
  visit_tree(root_node, context);

  ts_tree_delete(tree);
  ts_parser_delete(parser);
  return 0;
}


