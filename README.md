# Bolt

A x86_64 assembly compiler for an experimental programming language. The syntax is inspired from LISP S-Expression.

1. Simple Expression
```
(add 3 4)
```

2. Expression with block
```
(while a)
{
    (dec a)
}
```

## Build and Run

### Build
```
cmake -S . -Bbuild -GNinja
ninja -C build/
```

### Compile a bolt program
```
./build/bolt ./examples/07_func2.bolt
```

## Example

### Code
example.bolt
```
(defun main)
{
    (let a 42)
    (inc a)
    (print a)
}
```
### Compile to assembly
```
$ ./build/bolt example.bolt
```

### Build and Run
```
$ ./make.sh
43
```

```
$ echo $?
0
```
