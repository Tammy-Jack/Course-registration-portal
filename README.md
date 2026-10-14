# Student Course Management System

A menu-driven console application written in **C** that lets a student register their semester courses, search them by course code, calculate total credit units, and save or load their records from a file so data persists between sessions.

## Compile & Run

No dependencies, just a C compiler (gcc):

```bash
gcc -Wall -Wextra -std=c99 course_management.c -o course_management
./course_management        # Windows: .\course_management
```

## What I Learned

- Structuring a C program with `struct`s and small, single-purpose functions
- Handling user input safely: combining `scanf`/`fgets` correctly, trimming and normalizing strings, rejecting invalid values
- Persisting data with file I/O and checking for failures instead of crashing
- Applying recursion in a real feature (course lookup by code)
