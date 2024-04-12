# bf-interpreter
This program is a Brainfuck interpreter written in C. Brainfuck is a minimalist programming language that operates on an array of memory cells, each initially set to zero. The language consists of eight simple commands, a data pointer, and an instruction pointer. The commands are:

- \>: increment the data pointer.
- <: decrement the daa pointer.
- +: incrememnt the byte at the data pointer.
- -: decrement the byte at the data pointer.
- .: output the byte at the data pointer.
- ,: accept the byte at the data pointer.
- [: if the byte at the data pointer is zero, then jump the pointer forward to
  the command after the matching ] command.
- ]: if the byte at the data pointer is nonzero than jump back to the command
  after the matching [ command.

## How to run
### Compiling the C File

Before running the program, you need to compile the C file using a C compiler such as GCC.

```bash
gcc -o out src/bf.c
```
### Running
After the file has been compiled run it with
```bash
./out /path/to/brainfuck/source_file.bf
```