# hexify
is a program for displaying files in hex and also giving an ascii letter to each hex number in a nice terminal gui.
## Description

## Usage
hexify currently only has 2 ways to open a file.
- 1 hexify <file-name>
- 2 hexify -f <file-name>

There are two more flags `-v` and `-h` but they aren't very usefull currently.

### Dependencys
`ncurses`

### Compilation
`make`

If you see
```
make: ctags: No such file or directory
make: *** [Makefile:35: hexify] Error 127
```
dont worry the executable was still created but Makefile couldn't find `ctags`.
