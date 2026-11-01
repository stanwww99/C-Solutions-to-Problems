#pragma GCC optimize("O0")
#include <stdio.h>
#include <stdlib.h>

void
win()
{
    char buf[64];
    FILE *f = fopen("flag", "r");
    if (f == NULL)
      {
        printf("Failed to open flag file.\n");
        exit(1);
      }

    fgets(buf, 63, f);
    fprintf(stderr, buf);
}

// https://github.com/openbsd/src/commit/643a55bc0a80dff073fa29d08d77374db6b16f73
char *
gets(char *buf)
{
    int c;
    char *s;
    for (s = buf; (c = getchar()) != '\n';)
        if (c == EOF)
            if (s == buf) {
                return (NULL);
            } else
                break;
        else
            *s++ = c;
    *s = '\0';
    return (buf);
}

void
vuln()
{
    char buf[100];
    gets(buf);
    puts(buf);
}

int
main()
{
    printf("%p\n", win);
    fflush(stdout);
    vuln();
    return 0;
}

