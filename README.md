# toy-lang
toy-lang is currently a tree-walking interpreter, which transpiles to C++. In the future, for instance, a VM could be used to increase performance.

### Features
* Dynamic typing. Has types: int (32-bit), float (64-bit), boolean and string.
* Data structures: array and associative array.
* Functions.
* Builtin functions for arrays, strings, etc.
* Conditional statements.

### Quick demo of the REPL

```
>> let add = func(x, y, z) { x + y + z }
>> add(10, 5, -1)
14
>> let min = func(a, b) { if (a < b) { a } else { b } }
>> min(5, 1)
1
>> min(15, add(2, 9, 5))
15
>> let fib = func(n) { if (n==0) { 0 } else { if (n==1) { 1 } else { fib(n-1) + fib(n-2) } } }
>> fib(7)
13
>> let a = 4
>> let b = 4
>> a == b
true
>> 9*5 > 455-2
false
>> let a = "blaah"
>> len(a)
5
>> let arr = [1, 3.25, true, "Hello" + " world!"]
>> arr[1]
3.25
>> arr[3]
'Hello world!'
>> len(arr[3])
12
>> let str = "hello"
>> str[1]
'e'
>> str[1] + "ve"
'eve'
>> let pair = {"first": [1, 2, 3], 19: false}
>> pair["first"]
[1, 2, 3]
>> pair[19]
false
```

### Build and run

Use: ```make run```.

### Testing

Before testing install gtest with ```sudo apt-get install libgtest-dev```. Then just use: ```make run-tests```.

### Debug

When recursion is used a lot, LLDB debugger can become quite convenient. Debug with: ```make debug```.

### Detect memory issues with Valgrind

Use: ```make check-leak-run```.

### Literature

* [Ball, T. (2018). Writing An Interpreter In Go.](https://interpreterbook.com/)
* [Nystrom, R. (2021). Crafting interpreters. Genever Benning.](https://craftinginterpreters.com/)

### TODO:

* Many things, almost everything still.
