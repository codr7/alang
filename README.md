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
`if` may be used to branch on a condition, any value may be used.

```
: if 0 _ F
 
[F]
: if 42 T _

[F T]
: if [] _ F
 
[F T F]
: if [42] T _
 
[F T F T]
```

