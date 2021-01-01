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
Alang v0.3

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
`isa` returns `T` if the first argument is derived from the second, `F` otherwise.

```
: Int .isa Stack

[F]
: Int .isa Num

[F T]
```

### bindings
Bindings may be created using `let` and are evaluated at compile time, which is why the following example complains about a missing value.

```
: 42 let foo _

Error: Stack is empty
[]
```

### subroutines
Subroutines are first class, may be defined using `sub` and called by suffixing their names with an argument list.

```
: let foo sub(return 42)

[]

: foo

[(Sub 0x7fa911d04530)]

: d foo()
 
[42]
```