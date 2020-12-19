# tsv-js-minifier

"Tree-sitter-visitor javascript minifier" is a JS minifier implemented on top of
tree-sitter-visitor, a small layer of abstraction that allows to traverse a
tree-sitter tree in pre order.

This is a library created to test tree-sitter-visitor

## To improve

[ ] Remove " " after anonymous function. ex.: `var a = function (){}`
[ ] Remove " " after break. ex.: `break ;`
[ ] Remove " " after continue. ex.: `continue ;`
[ ] Transform scoped parameters identifiers in shorter versions. ex.: `function(foo){foo()}` => `function(a){a()}`

## How I tested this

As this is just a POC, I used jsminify to shorten "particle.js", "phaser.js"
and "phaser-arcade-physics.js" and then used Prettier to 'unminify' it

If the code goes both ways, there probably isn't a syntax error in the minifier.
(at least not for these cases)
