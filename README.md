This project aims to implement a minimal viable programming language on top of [liblgpp](https://github.com/codr7/liblgpp).

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

### conditions
`if` may be used to branch on a condition, all values have boolean representations.

```
: if 0 _ T
 
[T]
: if 42 T _

[T T]
: if [] _ T
 
[T T T]
: if [42] T _
 
[T T T T]
```

