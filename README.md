# toy-lang
Currently toy-lang is A tree-walking interpreter, which transpiles to C++. In the future, for instance, a virtual machine could be used to increase performance.

### Build and run

Use: ```make run```.

### Testing

Before testing install gtest with ```sudo apt-get install libgtest-dev```. Then just use: ```make run-tests```.

### Debug

When recursion is used a lot, LLDB debugger can become quite convenient. Debug with: ```make debug```.

### Detect memory issues with Valgrind

Use: ```make run-with-memory-check```.

### Literature

* [Ball, T. (2018). Writing An Interpreter In Go.](https://interpreterbook.com/)
* [Nystrom, R. (2021). Crafting interpreters. Genever Benning.](https://craftinginterpreters.com/)

### TODO:

* Many things, almost everything still.