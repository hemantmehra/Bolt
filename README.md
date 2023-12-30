# Bolt

An experimental programming language which compiles to x86_64 assembly code. The syntax is inspired from LISP S-Expression.

## Build and Run

### Build
```
cmake -S . -Bbuild -GNinja
ninja -C build/
```

### Compile a bolt program
```
./build/bolt ./examples/01_add.bolt
```

## Example

### Code
example.bolt
```
(let a 10)
(while a)
{
    (print a)
    (dec a)
}
(exit 42)
```
### Compile to assembly
```
$ ./build/bolt example.bolt > output.asm
```

### Build and Run
```
$ ./make.sh
10
9
8
7
6
5
4
3
2
1
```

```
$ echo $?
42
```
