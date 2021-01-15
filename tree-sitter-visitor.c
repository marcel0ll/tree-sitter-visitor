#include <string.h>
#include <stdio.h>
#include <tree_sitter/api.h>
#include "./tree-sitter-visitor.h"

struct visitor {
  void * enter;
  void * exit;
};

struct visit_context * context_new(TSLanguage * language, const char * source, int debug) {
  uint32_t visitor_count = ts_language_symbol_count(language);
  struct visitor ** visitors = malloc(sizeof(struct visitor *) * visitor_count);
  uint32_t i;
  for(i = 0; i < visitor_count; i++ ) visitors[i] = NULL;

  struct visit_context * cont = malloc(sizeof (struct visit_context));
  cont->language = language;
  cont->source = source;
  cont->visitors = visitors;
  cont->debug = debug;
  cont->cursor = NULL;

  return cont;
}

void context_delete(struct visit_context * context) {
  free(context->visitors);
  free((char *)context->source);
  free(context);
}

// set both type 'enter' and 'exit' visit function
bool context_set_type_visitor(struct visit_context * context, const char * type, void (*enter)(), void (*exit)()) {
  TSFieldId i = ts_language_symbol_for_name(context->language, type, strlen(type), true);
  if ( i == 0 ) {
    i = ts_language_symbol_for_name(context->language, type, strlen(type), false);
  }

  struct visitor * v = malloc(sizeof(struct visitor));
  v->enter = enter;
  v->exit = exit;
  context->visitors[i] = v;
  return true;
}

// set for each type in types both 'enter' and 'exit' visit function
void context_set_types_visitor(struct visit_context * context, const char * types[], void (*enter)(), void (*exit)()) {
  const char * type;
  struct visitor * v = malloc(sizeof(struct visitor));
  v->enter = enter;
  v->exit = exit;

  for (int i = 0; (type = types[i]) != NULL; i++) {
    TSFieldId i = ts_language_symbol_for_name(context->language, type, strlen(type), true);
    if ( i == 0 ) {
      i = ts_language_symbol_for_name(context->language, type, strlen(type), false);
    }
    context->visitors[i] = v;
  }
}

const char * context_get_source(struct visit_context * context) {
  return context->source;
}

struct visitor ** context_get_visitors(struct visit_context * context) {
  return context->visitors;
}

char * ts_node_text (TSNode node, struct visit_context * context) {
  uint32_t writer = ts_node_start_byte(node);
  uint32_t end = ts_node_end_byte(node);
  char * text = malloc(end - writer + 1);
  char * t = text;

  while (writer < end) {
    *t++ = context->source[writer++];
  }
  *t = '\0';

  return text;
}

void visit_tree_cursor (TSNode tree, struct visit_context * context) {
  TSTreeCursor cursor = ts_tree_cursor_new(tree);
  context->cursor = &cursor;
  TSNode node;
  struct visitor * visitor;
  void (*enter)() = NULL;
  void (*exit)() = NULL;
  bool recurse = true;

  while (true) {
    if (recurse && ts_tree_cursor_goto_first_child(context->cursor)) {
      recurse = true;
      node = ts_tree_cursor_current_node(context->cursor);
      visitor = context->visitors[ts_node_symbol(node)];
      if (visitor != NULL) {
        enter = visitor->enter;
        if (enter != NULL) {
          enter(node, context);
          /* ts_tree_cursor_reset(context->cursor, node); */
        }
      }
    } else {
      node = ts_tree_cursor_current_node(context->cursor);
      visitor = context->visitors[ts_node_symbol(node)];
      if (visitor != NULL) {
        exit = visitor->exit;
        if (exit != NULL) {
          exit(node, context);
          /* ts_tree_cursor_reset(context->cursor, node); */
        }
      }
      if (ts_tree_cursor_goto_next_sibling(context->cursor)) {
        recurse = true;
        node = ts_tree_cursor_current_node(context->cursor);
        visitor = context->visitors[ts_node_symbol(node)];
        if (visitor != NULL) {
          enter = visitor->enter;
          if (enter != NULL) {
            enter(node, context);
            /* ts_tree_cursor_reset(context->cursor, node); */
          }
        }
      } else if (ts_tree_cursor_goto_parent(context->cursor)) {
        recurse = false;
      } else {
        break;
      }
    }
  }
  context->cursor = NULL;
  ts_tree_cursor_delete(&cursor);
}

void _debug_tree (TSNode node, struct visit_context * context) {
  const char * type = ts_node_type(node);
  unsigned short sym = ts_node_symbol(node);
  struct visitor * visitor = context->visitors[sym];

  void (*enter)() = NULL;
  void (*exit)() = NULL;

  if (visitor != NULL) {
    if (visitor->enter != NULL) {
      enter = visitor->enter;
    }
    if (visitor->exit != NULL) {
      exit = visitor->exit;
    }
  }

  char * in = "-enter";
  char * out = "-exit";
  if (enter != NULL) {
    in = "+enter";
  }

  if (exit != NULL) {
    out = "+exit";
  }

  char * text = ts_node_text(node, context);
  printf("Id: %lu  %s  %s  %s  %hu\t%s\t", (uintptr_t) node.id, in, out, type, sym, text);

  if (enter != NULL) {
    enter(node, context);
  } 
  printf("\t");
  if (exit != NULL) {
    exit(node, context);
  }
  printf("\n");

  unsigned child_count = ts_node_child_count(node);
  for (unsigned i = 0; i < child_count; i++) {
    TSNode child_node = ts_node_child(node, i);
    _debug_tree(child_node, context);
  }
}
// visit pre order
void _visit_tree (TSNode node, struct visit_context * context) {
  TSSymbol sym = ts_node_symbol(node);
  struct visitor * visitor = context->visitors[sym];

  if (visitor != NULL) {
    void (*enter)() = NULL;
    void (*exit)() = NULL;

    if (visitor->enter != NULL) {
      enter = visitor->enter;
      enter(node, context);
    }

    unsigned child_count = ts_node_child_count(node);
    for (unsigned i = 0; i < child_count; i++) {
      TSNode child_node = ts_node_child(node, i);
      _visit_tree(child_node, context);
    }

    if (visitor->exit != NULL) {
      exit = visitor->exit;
      exit(node, context);
    }
  } else {
    unsigned child_count = ts_node_child_count(node);
    for (unsigned i = 0; i < child_count; i++) {
      TSNode child_node = ts_node_child(node, i);
      _visit_tree(child_node, context);
    }
  }
}

void visit_tree (TSNode node, struct visit_context * context) {
  if (context->debug) {
    printf("Registered visitor types:\n");
    printf("------\n");
    _debug_tree(node, context);
  } else {
    visit_tree_cursor(node, context);
  }
}

const char * get_source(const char * path) {
  FILE *file = fopen(path, "rb");
  if (file) {
    fseek(file, 0, SEEK_END);
    size_t length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *source_code = malloc(length + 1);
    if (source_code) {
      size_t bytes_read = fread(source_code, 1, length, file);
      if (bytes_read != length) {
        fputs ("Reading error",stderr); 
        exit (3);
      }

      source_code[length] = '\0';
    }

    return (const char *) source_code;
  }

  return "";
}
