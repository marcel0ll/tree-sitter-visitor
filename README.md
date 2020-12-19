# tree-sitter-visitor

A hashmap implementation of the visitor pattern for tree-sitter.

## Dependencies

[hashmap](https://github.com/tidwall/hashmap.c)

## Usage

Check js minifier for how to use this

## TO DO

- [x] better API to register multiple types for the same function
- [ ] can this be "imported" as an npm package, similar to 'tree-sitter'?
- [ ] Can context be extensible? should it?
- [ ] better AST context awareness while visiting a node (is this node inside X type of node?)

- [ ] Diff file against a cached version(might even be a file stored in a SHA-1 git ref),
used it as 'edits' regenerate just parts of the out file (not sure how to relate AST to generated file)...
if for some reason you have to regenerate entire file to create mapping, ignore this

## API

```c
// struct to hold the visiting context(.source, .visitors)
struct visit_context;

// struct to hold a visitor(.type, .visit)
struct visitor;

// creates new visit_context
struct visit_context * context_new(const char * source);

// returns source from visit_context
const char * context_get_source(struct visit_context * context);

// adds visitor to visit_context
bool context_add_visitor(struct visit_context * context, struct visitor * visitor);

// returns visitors hashmap from a visit_context
struct hashmap * context_get_visitors(struct visit_context * context);

// creates new struct to hold a visitor which holds a type and a visit function
struct visitor * visitor_new(const char * type, void (*visit)());

// function to return a string from a bigger string from index start to end
char * get_text(uint32_t start, uint32_t end, const char * source);

// function to return the source text from a node 
char * ts_node_text (TSNode node, struct visit_context * source);

// The 'main' function to start visiting the tree
// it visits in "pre order" 
void visit_tree (TSNode node, struct visit_context * context);

// Opens a file and returns its content
const char * get_source(const char * path);
```







