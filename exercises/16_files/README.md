# Files

`<stdio.h>` handles files through `FILE *` streams:

```c
FILE *f = fopen("notes.txt", "w"); // "r" read, "w" write, "a" append
if (f == NULL) { /* handle the error (errno says why) */ }
fprintf(f, "%d apples\n", 3);
fclose(f);
```

Reading is done with `fgets` (one line at a time), `fscanf`, `fgetc` or
`fread`.

Streams are *buffered*: what you write is collected in memory and only
written to the file when the buffer fills up, when you call `fflush`, or
when you `fclose` the stream. Always close files you open!

The tests in this section create their files in `.clings/build/`.

## Further information

- [cppreference: File input/output](https://en.cppreference.com/w/c/io)
- [cppreference: fopen](https://en.cppreference.com/w/c/io/fopen)
- [cppreference: fgets](https://en.cppreference.com/w/c/io/fgets)
