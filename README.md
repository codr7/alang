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

: let fib sub(
  if (cp .< 2) _ (
    + (.- 1 cp recall)
      (sw .- 1 recall)
  )
)
      
[]
: fib(20)
 
[6765]
```

### design
The language works more or less like a Forth, with a few minor syntactic additions.

Prefixing an identifier with a `.` allows it to be used infix, ie. the target will take it's first argument from the stack.

Suffixing an identifier with an argument list allows specifying arguments explicitly inline, passing `_` as parameter splices a value from the stack.

The point is not that Forth is better than any other language, it's just the lowest common denominator I've found; on top of this you can build pretty much any kind of language and still reuse most of the foundation.

```
: + 1 2

[3]
: 1 .+ 2

[3 3]
: 5 -(_ 2)

[3 3 3]
```

### stacks
The stack may be manipulated directly.

```
: 1 2 3

[1 2 3]
: sw

[1 3 2]
: cp

[1 3 2 2]
: rot

[1 2 2 3]
: d

[1 2 2]
```

Additional stacks may be created by enclosing any expresssion in brackets.

```
: [1 2 .+ 3]

[[1 5]]
```

### conditions

`=`, `<` and `>` may be used to compare values.

```
: 42 .= 42

[T]

: 1 .> 2

[T F]
```

`or` returns its first truthy argument, or the last argument if none.

```
: F .or 0 .or 42
 
[42]
```

`and` returns its first falsey argument, or the last argument if none.

```
: T .and 0 .and 42
 
[0]
```

`if` may be used to branch on any value.

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

`type` may be used to get the type of any value.

```
: 42 .type

[Int]
: .type

[Meta]

: .type

[Meta]
```

`isa` returns `T` if the first argument is derived from the second, `F` otherwise.

```
: Int .isa Stack

[F]
: Int .isa Num

[F T]
```

### bindings
Bindings may be created using `let`.


```
: let foo 42

[]
: foo

[42]
```

Bindings are evaluated at compile time.

```
: 42 let foo _

Error: Stack is empty
[]
```

`@` may be used to evaluate any expression at compile time.

```
: @42 let foo _

[]
: foo

[42]
```

### subroutines
Subroutines are first class, may be defined using `sub` and called by suffixing their names with an argument list.

```
: let foo sub(42)

[]

: foo

[(Sub 0x7fa911d04530)]

: d foo()
 
[42]
```

### io
`say` may be used to print any value to standard output.

```
: say(42)
 
42
[]
```