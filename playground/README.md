[*] Files named `code#.c` are for stack experiment.

[*] Files named `heap#.c` are for heaps experiment.

Start the process in the background while the program is suspended waiting for input:

`gcc code.c -o code && ./code&`

cat /proc/{PROCESS_IDENTIFIER}/maps

Example: cat/proc/2708/maps

---

Bring the process back to the foreground, use `fg`

---

By convention the stack segment moves around a bit to avoid hackers exploiting stack overflow, if you want it to say:
`setarch $(uname -m) -R bash`

---

```
 heap.c: allocating then freeing heap memory
heap2.c: allocating then freeing then DANGER: accessing freed memory
heap3.c: used calloc() that will not only allocate the
         data structure but also set all its elements to zero
```

---

Shared Library

When we printed the memory map, there was a lot of junk similar to:
    `/lib/x86_64-linux-gnu/ld-2.23.so`
All of the segments are allocated for shared libraries, either the code or
areas used for dynamic data structures. We have been using library proce-
dures for printing messages, finding the process identifier and for allocating
memory etc. All of those routines are located somewhere in these segments.

---

