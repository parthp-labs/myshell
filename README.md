# Mini Shell (Learning Project)

A simple Unix-like shell written in C as part of my systems programming learning journey.

This project is being built incrementally to understand how Linux shells work internally using process creation, program execution, and inter-process communication.

---

## Current Status

### Implemented

- Custom shell prompt
- Command input using `fgets()`
- Process creation using `fork()`
- Parent-child process model
- Child process execution using `execl()`
- Parent waits for child using `wait()`

Current prototype:

``` 
[pwd]> ls