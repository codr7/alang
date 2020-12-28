This project aims to implement a minimal viable programming language on top of [liblgpp](https://github.com/codr7/liblgpp).

### setup
The project requires a C++17 compiler, CMake and [liblgpp](https://github.com/codr7/liblgpp) to build.

```
$ git clone https://github.com/codr7/alang.git
$ cd alang
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./alang
Alang v0.2

Press Return on empty line to eval.
Empty eval clears stack and Ctrl+D exits.

: 42
  
[42]
```

### stacks
The stack may be manipulated directly.

```
: 1 2 3

[1 2 3]
: cp

[1 2 3 3]
: rot

[1 3 3 2]
: d

[1 3 3]
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

### types
Types are first class.
`.isa` returns `T` if the first argument is derived from the second, `F` otherwise.

```
: Int .isa Stack

[F]
: Int .isa Num

[F T]
```

### calls
Labels may be called by appending an argument list to their names, unspecified arguments are taken from the stack.

```
: label foo 42
 
[42]
: 
[]
: 1 2
 
[1 2]
: foo(3 _ _ 4)
 
[3 1 2 4 42]
```
