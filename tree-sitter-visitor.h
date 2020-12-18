#ifndef TREE_SITTER_VISITOR_H
#define TREE_SITTER_VISITOR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tree_sitter/api.h>
#include "libs/hashmap.h"

struct visit_context;
struct visitor;
int visitor_compare(const void *a, const void *b, void *data);
uint64_t visitor_hash(const void *item, uint64_t seed0, uint64_t seed1);
struct visit_context * context_new(const char * source, struct hashmap * visitors);
size_t visitor_size();
const char * context_get_source(struct visit_context * context);
struct visitor * visitor_new(const char * type, void (*visit)());

char * get_text(uint32_t start, uint32_t end, const char * source);
char * ts_node_text (TSNode node, struct visit_context * source);
void visit_tree (TSNode node, struct visit_context * context);
const char * get_source(const char * path);

#endif
