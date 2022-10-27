# toy-lang
toy-lang is currently a tree-walking interpreter, which transpiles to C++. In the future, for instance, a VM could be used to increase performance.

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
```

### Build and run

Use: ```make run```.

### Testing

Before testing install gtest with ```sudo apt-get install libgtest-dev```. Then just use: ```make run-tests```.

### Debug

When recursion is used a lot, LLDB debugger can become quite convenient. Debug with: ```make debug```.

### Detect memory issues with Valgrind

Use: ```make check-leak-run```. Smart pointers don't eliminate all memory issues!

### Literature

* [Ball, T. (2018). Writing An Interpreter In Go.](https://interpreterbook.com/)
* [Nystrom, R. (2021). Crafting interpreters. Genever Benning.](https://craftinginterpreters.com/)

### TODO:

* Many things, almost everything still.