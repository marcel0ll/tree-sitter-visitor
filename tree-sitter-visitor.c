#include <string.h>
#include <stdio.h>
#include <tree_sitter/api.h>
#include "libs/hashmap.h"

const int DEBUG = 0;

struct visit_context {
  const char * source;
  struct hashmap * visitors;
};

struct visitor {
  const char * type;
  void * visit;
};

int visitor_compare(const void *a, const void *b, void *data) {
  const struct visitor *va = a;
  const struct visitor *vb = b;
  return strcmp(va->type, vb->type);
}

uint64_t visitor_hash(const void *item, uint64_t seed0, uint64_t seed1) {
    const struct visitor *visitor = item;
    return hashmap_sip(visitor->type, strlen(visitor->type), seed0, seed1);
}

bool visitor_iter(const void *item, void *udata) {
  const struct visitor *visitor = item;
  printf("TYPE: %s\n", visitor->type);
  return true;
}

struct visit_context * context_new(const char * source) {
  struct hashmap * visitors = hashmap_new(sizeof(struct visitor), 0, 0, 0, visitor_hash,
      visitor_compare, NULL);

  struct visit_context * cont = malloc(sizeof (struct visit_context));
  cont->source = source;
  cont->visitors = visitors;

  return cont;
}

void context_delete(struct visit_context * context) {
  hashmap_free(context->visitors);
  free(context->source);
  free(context);
}

bool context_add_visitor(struct visit_context * context, struct visitor * visitor) {
  return hashmap_set(context->visitors, visitor);
}

bool context_add_multiple_visitors(struct visit_context * context, char * types[], void (*visit)()) {
  for (int i = 0; types[i] != NULL; i++) {
    struct visitor * v = malloc(sizeof(struct visitor));
    char * type = (char *)types[i];

    v->type = malloc(strlen(type) + 1);
    strcpy(v->type, type);
    v->visit = visit;

    hashmap_set(context->visitors, v);
  }

  return 1;
}

const char * context_get_source(struct visit_context * context) {
  return context->source;
}

struct hashmap * context_get_visitors(struct visit_context * context) {
  return context->visitors;
}

struct visitor * visitor_new(const char * type, void (*visit)()) {
  struct visitor * v = malloc(sizeof(struct visitor));
  v->type = type;
  v->visit = visit;

  return v;
}

char * get_text (uint32_t start, uint32_t end, const char * source) {
  size_t len = (end - start);
  char * text = malloc((len + 1));

  for (size_t i = 0; i < len; i++) {
    text[i] = source[start + i];
  }
  text[len] = '\0';

  return text;
}

char * ts_node_text (TSNode node, struct visit_context * context) {
  char * code = get_text(ts_node_start_byte(node), ts_node_end_byte(node), context->source);
  return code;
}

int once = 1;
// visit pre order
void visit_tree (TSNode node, struct visit_context * context) {

  if (ts_node_is_null(node)) return;

  if (DEBUG) {
    /* printf("Visiting node: %u\n", (unsigned int) node.id); */
    /* printf("Visiting: %s\n", ts_node_type(node)); */

    if(once-- > 0) {
      hashmap_scan(context->visitors, visitor_iter, NULL);
    }
  }
  const char * type = ts_node_type(node);

  struct visitor * visitor = hashmap_get(context->visitors, &(struct visitor){ .type=type});
  if (visitor != NULL) {
    void (*visitor_fn)() = visitor->visit; 
    visitor_fn(node, context);
  } else {
    if (DEBUG) {
      /* printf("\nMissing: %s\n", ts_node_type(node)); */
    }
  }

  unsigned child_count = ts_node_child_count(node);
  for (unsigned i = 0; i < child_count; i++) {
    TSNode child_node = ts_node_child(node, i);
    visit_tree(child_node, context);
  }

  char *type_out = malloc(strlen(type) + 5);
  strcpy(type_out, type);
  strcat(type_out, "_out");

  struct visitor * visitor_out = hashmap_get(context->visitors, &(struct visitor){ .type=type_out});
  if (visitor_out != NULL) {
    void (*visitor_out_fn)() = visitor_out->visit; 
    visitor_out_fn(node, context);
    /* printf("Visiting: %s\n", type_out); */
  }
}

const char * get_source(const char * path) {
  FILE *file;
  file = fopen(path, "rb");
  fseek(file, 0, SEEK_END);
  long fsize = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *source_code = malloc(fsize + 1);
  fread(source_code, 1, fsize, file);

  return (const char *) source_code;
}
