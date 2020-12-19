# tsv-js-minifier

"Tree-sitter-visitor javascript minifier" is a JS minifier implemented on top of
tree-sitter-visitor, a small layer of abstraction that allows to traverse a
tree-sitter tree in pre order.

This is a library created to test tree-sitter-visitor

## To Do

- [ ] Test on Win
- [ ] Test on Mac
- [ ] Remove " " after anonymous function. ex.: `var a = function (){}`
- [ ] Remove " " after break. ex.: `break ;`
- [ ] Remove " " after continue. ex.: `continue ;`
- [ ] Transform scoped parameters identifiers in shorter versions. ex.: `function(foo){foo()}` => `function(a){a()}`
- [ ] Remove unecessary ";" on last block statement
- [ ] Improve code
  * [ ] DEBUG flag
  * [ ] check memory allocation/disallocation
  * [ ] are the "includes" done right?
- [ ] Time this tool and compare against other tools (uglifyjs? minifyjs?)
- [ ] Check if this can be used as a npm module somehow with bindings (no idea how this work)
- [ ] Write some tests

## How I tested this

As this is just a POC, I used jsminify to shorten "particle.js", "phaser.js"
and "phaser-arcade-physics.js" and then used Prettier to 'unminify' it

If the code goes both ways, there probably isn't a syntax error in the minifier.
(at least not for these cases)
