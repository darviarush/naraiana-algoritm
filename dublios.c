#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define BUF_SIZE (1*1024*1024)


// срезает из файла повторяющийся фрагмент
void cut_end_repeated_file(FILE* f, char* fragment, size_t size) {
	// определяем место с которого фрагмент начинает повторяться
	size_t i = size*4, pos;
	char* b = malloc(size);

	for(;; i+=size) {
		if(fseek(f, -i, SEEK_END)) 			break;
		if(fread(b, 1, size, f) != size)	break;
		if(memcmp(fragment, b, size)) 		break;
		pos = i;
	}

	// усекаем файл
	//ftruncate(f, i);
	printf("pos: %i\n", pos);
	free(b);
}


int main(int ac, char** av) {
	if(ac != 2) { fprintf(stderr, "using: dublios <file>\n"); return 1; }

	FILE* f = fopen(av[1], "rb");
	if(!f) { perror(av[1]); exit(errno); }

	fseek(f, 0, SEEK_END);
	size_t len = ftell(f);
	size_t file_size = len;
	if(len > BUF_SIZE) len = BUF_SIZE;

	// читаем с конца
	char buf[BUF_SIZE];

	if(fseek(f, -len, SEEK_END)) { perror("Не могу переместиться к концу."); fclose(f); return 1;}

	printf("pos=%i len=%i file_size=%i\n", ftell(f), len, file_size);

	size_t size = fread(buf, 1, len, f);
	if(size == 0) { perror("Последний фрагмент не считан."); fclose(f); return 1;}

	// определяем максимальный повторяющийся фрагмент:
	size_t max = 0;
	char* end = buf+size-1;
	for(size_t i=1, s=size/2; i<s; i++) {
		char* a = end-i;
		char* b = end-i*2;

		int x = 0;
		for(; a<end; a++, b++) if(*a != *b) { x=1; break; }
		if(x==0) max = i;
	}

	if(max == 0) {
		fprintf(stderr, "Повторяющихся фрагментов не найдено.\n");
		fclose(f);
		return 1;
	}

	// максимальный повторяющийся фрагмент может состоять из более мелких фрагментов
	// находим средний повторяющийся фрагмент
	// это нужно, чтобы захватить максимум повторяющихся фрагментов
	for(size_t k=2; k<max; k++) {
		if(max % k != 0) continue; // только если делится без остатка
		size_t size_mid = max / k;

		for(size_t r=0; r<k; r++) 
	}


	printf("max=%i pos=%i\n", max, file_size-max);

	cut_end_repeated_file(f, buf+size-max, max);
	fclose(f);
	return 0;
}
