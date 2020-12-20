This project aims to implement a minimum viable programming language on top of [liblgpp](https://github.com/codr7/liblgpp).

```
Alang v0.1

Press Return on empty line to eval.
Empty eval clears stack and Ctrl+D exits.

: label foo
  42
  
[42]
: 
[]
: go foo
  
[42]
: go foo
  
[42 42]
```
