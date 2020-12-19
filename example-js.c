#include <string.h>
#include <stdio.h>
#include <tree_sitter/api.h>
#include "tree-sitter-visitor.h"

void node_keyword (TSNode node, char * text) {
  printf("%s", ts_node_type(node));
}

void node_spaced_keyword (TSNode node, char * text) {
  printf("%s ", ts_node_type(node));
}

int class_counter = 0;
void node_class (TSNode node, struct visit_context * context) {
  if (class_counter) {
    printf("%s", "class ");
  }
  class_counter = (class_counter + 1) % 2;
}

int function_counter = 0;
void node_function (TSNode node, struct visit_context * context) {
  if (function_counter) {
    printf("%s", "function");
  }
  function_counter = (function_counter + 1) % 2;
}

void node_statement_block(TSNode node, struct visit_context * context) {
  function_counter = 0;
}

void node_function_declaration (TSNode node, struct visit_context * context) {
  printf("%s", "function ");
}

void node_text (TSNode node, struct visit_context * context) {
  printf("%s", ts_node_text(node, context));
}

void node_spaced_text (TSNode node, struct visit_context * context) {
  printf(" %s", ts_node_text(node, context));
}

int main(int argc, char * argv[]) {
  char * file_path = argv[1];
  if (file_path == NULL) {
    printf("No file passed...\n");
    return 1;
  }
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

  TSNode root_node = ts_tree_root_node(tree);

  struct visit_context *context = context_new(source_code);
  context_add_visitor(context, visitor_new("class_declaration", node_class));
  context_add_visitor(context, visitor_new("class", node_class));
  context_add_visitor(context, visitor_new("function_declaration", node_function_declaration));
  context_add_visitor(context, visitor_new("function", node_function));
  context_add_visitor(context, visitor_new("statement_block", node_statement_block));
  context_add_visitor(context, visitor_new("import", node_spaced_keyword));
  context_add_visitor(context, visitor_new("export", node_spaced_keyword));
  context_add_visitor(context, visitor_new("default", node_spaced_keyword));
  context_add_visitor(context, visitor_new("const", node_spaced_keyword));
  context_add_visitor(context, visitor_new("new", node_spaced_keyword));
  context_add_visitor(context, visitor_new("var", node_spaced_keyword));
  context_add_visitor(context, visitor_new("let", node_spaced_keyword));
  context_add_visitor(context, visitor_new("for", node_keyword));
  context_add_visitor(context, visitor_new("do", node_keyword));
  context_add_visitor(context, visitor_new("while", node_keyword));
  context_add_visitor(context, visitor_new("this", node_keyword));
  context_add_visitor(context, visitor_new("if", node_keyword));
  context_add_visitor(context, visitor_new("in", node_keyword));
  context_add_visitor(context, visitor_new("instanceof", node_keyword));
  context_add_visitor(context, visitor_new("typeof", node_keyword));
  context_add_visitor(context, visitor_new("return", node_spaced_keyword));
  context_add_visitor(context, visitor_new("else", node_spaced_keyword));
  context_add_visitor(context, visitor_new("switch", node_keyword));
  context_add_visitor(context, visitor_new("case", node_keyword));
  context_add_visitor(context, visitor_new("break", node_keyword));
  context_add_visitor(context, visitor_new("undefined", node_keyword));
  context_add_visitor(context, visitor_new("null", node_keyword));
  context_add_visitor(context, visitor_new("!", node_keyword));
  context_add_visitor(context, visitor_new("?", node_keyword));
  context_add_visitor(context, visitor_new("/", node_keyword));
  context_add_visitor(context, visitor_new("*", node_keyword));
  context_add_visitor(context, visitor_new("**", node_keyword));
  context_add_visitor(context, visitor_new("++", node_keyword));
  context_add_visitor(context, visitor_new("+", node_keyword));
  context_add_visitor(context, visitor_new("--", node_keyword));
  context_add_visitor(context, visitor_new("-", node_keyword));
  context_add_visitor(context, visitor_new("~", node_keyword));
  context_add_visitor(context, visitor_new(".", node_keyword));
  context_add_visitor(context, visitor_new(":", node_keyword));
  context_add_visitor(context, visitor_new(";", node_keyword));
  context_add_visitor(context, visitor_new("=", node_keyword));
  context_add_visitor(context, visitor_new("==", node_keyword));
  context_add_visitor(context, visitor_new("+=", node_keyword));
  context_add_visitor(context, visitor_new("-=", node_keyword));
  context_add_visitor(context, visitor_new("*=", node_keyword));
  context_add_visitor(context, visitor_new("/=", node_keyword));
  context_add_visitor(context, visitor_new("&=", node_keyword));
  context_add_visitor(context, visitor_new("|=", node_keyword));
  context_add_visitor(context, visitor_new("<=", node_keyword));
  context_add_visitor(context, visitor_new(">=", node_keyword));
  context_add_visitor(context, visitor_new("!=", node_keyword));
  context_add_visitor(context, visitor_new("===", node_keyword));
  context_add_visitor(context, visitor_new("!==", node_keyword));
  context_add_visitor(context, visitor_new(",", node_keyword));
  context_add_visitor(context, visitor_new("(", node_keyword));
  context_add_visitor(context, visitor_new(")", node_keyword));
  context_add_visitor(context, visitor_new("<", node_keyword));
  context_add_visitor(context, visitor_new(">", node_keyword));
  context_add_visitor(context, visitor_new(">>", node_keyword));
  context_add_visitor(context, visitor_new("<<", node_keyword));
  context_add_visitor(context, visitor_new("[", node_keyword));
  context_add_visitor(context, visitor_new("]", node_keyword));
  context_add_visitor(context, visitor_new("{", node_keyword));
  context_add_visitor(context, visitor_new("}", node_keyword));
  context_add_visitor(context, visitor_new("||", node_keyword));
  context_add_visitor(context, visitor_new("&&", node_keyword));
  /* context_add_visitor(context, visitor_new("'", node_keyword)); */
  /* context_add_visitor(context, visitor_new("\"", node_keyword)); */
  context_add_visitor(context, visitor_new("true", node_keyword));
  context_add_visitor(context, visitor_new("false", node_keyword));
  context_add_visitor(context, visitor_new("regex", node_text));
  /* context_add_visitor(context, visitor_new("regex_flags", node_text)); */
  context_add_visitor(context, visitor_new("string", node_text));
  context_add_visitor(context, visitor_new("number", node_text));
  context_add_visitor(context, visitor_new("identifier", node_text));
  context_add_visitor(context, visitor_new("property_identifier", node_text));

  visit_tree(root_node, context);

  ts_tree_delete(tree);
  ts_parser_delete(parser);
  return 0;
}

