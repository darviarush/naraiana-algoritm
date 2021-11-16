#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "elsie-four/lc4.h"
 
int main(int ac, char **av)
{
// Шифрувальна програма має отримувати з командного рядка такі дані:
// 0) назва ключового файлу;
// 1) символ, який буде зашифровуватися;
// 2) кількість ітерацій шифрування симполу з попереднього пункту №1;
// 3) початковий рядок у ключовому файлі - перший ключ для шифрування;
// 4) кінцевий рядок у ключовому файлі - останній ключ для шифрування.

    if(ac != 6) {
        fprintf(stderr, "usage: %s file symbol iteration_count from_line to_line\n", av[0]);
        return 1;
    }

    char* file = av[1];
    int symbol = av[2][0];
    int iteration_count = atoi(av[3]);
    int from_line = atoi(av[4])-1;
    int to_line = atoi(av[5])-1;

    FILE* f = fopen(file, "rb");
    if(!f) { perror(file); return 1; }


    char out_file[1024];
    char key[39];
 
    int i, j, t, h, line;
    unsigned char b;

    for(line = 0; line <= to_line; line++) {

        if(fgets(key, sizeof(key), f) == NULL) {
            fprintf(stderr, "file %s line %i: end of file, but may be %i lines\n", file, line, to_line);
            return 1;
        }

        if(strlen(key) != 38) {
            fprintf(stderr, "file %s line %i: line is corrupt!\n", file, line);
            return 1;
        }

        if(key[37] != '\n' || key[36] != '\r') {
            fprintf(stderr, "file %s line %i: line is corrupt! No newline on end of line!\n", file, line);
            return 1;
        }

        if(line < from_line) continue;

        key[36] = '\0';
     
        snprintf(out_file, sizeof(out_file), "keys/%s %c.txt", key, symbol);
        FILE* q = fopen(out_file, "wb");
        if(!q) { perror(out_file); return 1; }


        struct lc4 lc4;
        memset(&lc4, 0, sizeof(lc4));
        lc4_init(&lc4, key);
        for (i = 0; i < iteration_count; i++)
            fprintf(q, "%c", lc4_encrypt(&lc4, symbol));

        fclose(q);
    }

    fclose(f);
    
    return 0;
}
