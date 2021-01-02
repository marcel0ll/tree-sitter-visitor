#ifndef TREE_SITTER_VISITOR_H
#define TREE_SITTER_VISITOR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tree_sitter/api.h>
#include "../hashmap.c/hashmap.h"

// struct to hold the visiting context(.source, .visitors)
struct visit_context;

// struct to hold a visitor(.type, .visit)
struct visitor;

// creates new visit_context
struct visit_context * context_new(const char * source, int debug);

// frees memory for context
void context_delete(struct visit_context * context);

// returns source from visit_context
const char * context_get_source(struct visit_context * context);

// set both type 'enter' and 'exit' visit function
bool context_set_type_visitor(struct visit_context * context, const char * type, void (*enter)(), void (*exit)());
// set for each type in types both 'enter' and 'exit' visit function
void context_set_types_visitor(struct visit_context * context, const char * types[], void (*enter)(), void (*exit)());

// returns visitors hashmap from a visit_context
struct hashmap * context_get_visitors(struct visit_context * context);

// function to return a string from a bigger string from index start to end
char * get_text(uint32_t start, uint32_t end, const char * source);

// function to return the source text from a node 
char * ts_node_text (TSNode node, struct visit_context * source);

// visit the parsed tree in pre order
void visit_tree (TSNode node, struct visit_context * context);

// Opens a file and returns its content
const char * get_source(const char * path);

#endif
