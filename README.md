# Container-with-GNU-C

A container libarary immitating C++ STL. Implemented with C and some gcc extensions.

Now only the "list.h" is finished

有一部分代码丢掉了,只剩下刚开始写的一部分了(悲

------------------------------------

1. void * pointer used, so there is no type system like STL template, instead of pass value directly, you should pass an address

2. this-pointer built-in

```c
list L = newList(int);      // initialize
int number = 1;
L.push_front(&L, &number);  // traditional method
L.push_front(&number);      // with my library
```

3. some of the operation(return void in STL) now return the container itself, this means
you can call the operation to the container serially (like in java)

```c
L.setcmp(integercmp).sort().reverse().traverse(print);
```

------------------------------------

Basically using GNU compound-statement-expressions to create a temporary "object"
to realize the functionality like this-pointer and anonymous function (or you can call them lambda expressions if you like)

But there's bunches of bugs using this technique, most of which was caused by the **scope of varibles in a compound-statement-expression**. Using them from out of the expression is likely an UB for gcc, propably they only promise that those varibles
can be used in scope of the function calling the compound-statement-expressions (I found a blog whose author said he
asked one of the developer of GCC about the question) and they may also disturb the runtime safety (another blog exprimented
the code using this feature and found it drives GCC compiler to turn of the NX protection)

Anyway, tho it may not be a good library for production, constructing a C language container similar to C++ STL is still thrilling.

At least it looks pretty cool~

## Of course, if you got an idea to fix this all, please don't hesitate to let me know
