# How .h Files, .c Files, Compilation, and Linking Work

## Can your program work without .h files?

Yes. You could put everything in one giant .c file and it would compile and run. Or you could have two .c files and copy-paste the function declarations into each one. Header files are not required — they're a convenience.

## So what's the point of .h files?

The problem shows up when you have multiple .c files that need to share things.

```
allocator.c  — has heap_init() defined in it
main.c       — wants to CALL heap_init()
```

main.c has no idea what heap_init is. Why? Because the compiler processes each .c file **completely alone, in isolation.** It doesn't look at other .c files.

## Why can't the compiler just look at other .c files?

Because of how C was designed in the 1970s. Computers had tiny amounts of RAM, so loading multiple source files at once wasn't practical. The compiler processes one .c file at a time, period. Modern languages (Python, Java, Go) look around and find things automatically. C doesn't.

## Could you do #include "allocator.c"?

Yes, it would work — the compiler would copy-paste the entire allocator.c into main.c, and now everything is in one file.

But there's a catch: `gcc *.c` compiles every .c file. So heap_init would exist twice — once from the copy-paste into main.c, and once from compiling allocator.c on its own. The linker would see two copies and give you an error: "multiple definition of heap_init."

That's why .h files only contain **declarations** (descriptions of what functions look like), not the actual function bodies.

## The full compilation process (3 steps)

When you run `gcc *.c -o main`, three things happen behind the scenes:

### Step 1: Compile main.c (alone)

The compiler opens main.c. ONLY main.c. It reads line by line:

```
#include <stdio.h>         -> copies printf declaration into this file
#include "allocator.h"     -> copies whatever is in allocator.h into this file
void *slab = heap_init();  -> what is heap_init?
```

At the heap_init line, the compiler asks: "do I know what heap_init is?" It checks everything it's seen so far — the stuff from stdio.h and allocator.h.

- If allocator.h has `void *heap_init();` in it — great, it knows the function returns void*.
- If allocator.h is empty — it guesses int (bad!).

The compiler doesn't know the **body** of the function (the actual code). It doesn't need it yet. It just writes a note in the output:

```
main.o contains:
    - compiled machine code for main()
    - a note: "I need to jump to heap_init but I don't know where it is yet"
```

The output is a .o file (object file) — machine code that is **incomplete**. It has holes in it.

### Step 2: Compile allocator.c (alone)

The compiler closes main.c, opens allocator.c. Fresh start. No memory of main.c.

```
allocator.o contains:
    - compiled machine code for heap_init()
    - a label: "heap_init's code lives HERE"
```

### Step 3: The linker glues them together

The linker is a separate program from the compiler. It opens both .o files:

```
main.o says:       "I NEED heap_init — where is it?"
allocator.o says:  "I HAVE heap_init — it's right here"
```

The linker fills in the hole — it writes the actual memory address of heap_init into main.o's function call. Now everything is connected. It outputs the final executable.

## What "address" means here

Your compiled code lives in the Text segment. Every function is machine code sitting at a location in memory. That location has an address.

```
Text segment:
address 0x1000:  [machine code for main()       ]
address 0x1080:  [machine code for heap_init()   ]
```

When main calls heap_init(), the CPU needs to **jump** to address 0x1080. But during compilation, main.o didn't know that address yet. So it left a blank:

```
main.o:     call ????      <- "where is heap_init?"
```

The linker fills it in:

```
executable: call 0x1080    <- "jump to heap_init at 0x1080"
```

## So why do we need the .h file if the linker figures it out?

The linker only cares about **finding** the function. It connects the jump. It doesn't care about types.

The **compiler** cares about types. When it sees:

```c
void *slab = heap_init();
```

It needs to know: does heap_init return a void*? An int? A double? Because the compiler generates **different machine code** depending on the return type.

Without the .h file, the compiler guesses int. So it generates machine code to catch a 4-byte int. But heap_init actually returns a void* (8 bytes on 64-bit). The top half of your pointer gets chopped off. Your address is corrupted. Your program crashes.

The .h file tells the compiler the correct types **before** it generates machine code.

## Summary

- **.h file** -> helps the **compiler** generate correct machine code (knows the right types)
- **linker** -> connects the function call to the actual function body (fills in addresses)

Both are needed. They solve different problems.

```
main.c ----compiler----> main.o (incomplete, has holes)
                                        |
                                      linker ----> main (complete executable)
                                        |
allocator.c --compiler--> allocator.o (incomplete)
```
