#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

/**
 * Определяет максимальный повторяющийся фрагмент с конца буфера
 *
 * Возвращает или размер фрагмента или 0 в случае неудачи.
 */
size_t find_max_fragment(char* buf, size_t size) {

	printf("%c %i\n", buf[size-2], size);

	size_t max = 0;
	char* end = buf+size;
	for(size_t i=9114, s=size/2; i<s; i++) {
		char* a = end-i;
		char* b = end-i*2;

		printf("%c==%c end %i\n", a[0], b[0], a[i-1]==b[i-1]);

		if(memcmp(a, b, i) == 0) max = i;

		//printf("START %c==%c\n", *a, *b);

		// int x = 0;
		// for(; a<end; a++, b++) {
		// 	//printf("%c==%c\n", *a, *b);
		// 	if(*a != *b) { x=1; break; }
		// }
		// if(x==0) max = i;

		break;
	}
	return max;
}

/** 
 * Нахождение минимального повторяющегося фрагмента внутри указанного фрагмента
 * 
 * Максимальный повторяющийся фрагмент может состоять из более мелких фрагментов.
 * Находим средний повторяющийся фрагмент.
 * Это нужно, чтобы захватить максимум повторяющихся фрагментов.
 */
size_t find_mid_fragment(char* fragment, size_t size) {
	
	size_t mid = size;

	for(size_t k=2; k<size; k++) {
		if(size % k != 0) continue; // только если делится без остатка

		// это - длина равных участков на которые делится найденный фрагмент (max байт с конца)
		size_t size_mid = size / k;

		printf("k=%i size_mid=%i\n", k, size_mid);

		// цикл по столбцам
		for(size_t c=0; c<size_mid; c++) {
			char first_byte = fragment[c];
			// цикл по участкам
			for(size_t u=0; u<k; u++)
				if(first_byte != fragment[u*size_mid+c]) goto NO;
		}

		mid = k;

		NO: (void) 0;
	}

	return mid;
}

/**
 * Cрезает из файла повторяющиеся фрагменты
 */
void cut_end_repeated_file(FILE* f, char* fragment, size_t size) {
	// определяем место с которого фрагмент начинает повторяться
	size_t pos;
	char* b = malloc(size);

	for(size_t i=size*4;; i+=size) {
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

/**
 * Точка входа
 */
int main(int ac, char** av) {
	int buf_size = 1024 * 1024;
	if(ac == 3) buf_size = atoi(av[2]);

	if(!(ac == 2 || ac == 3)) {
		fprintf(stderr, "using: dublios <file> [<buffer size>]\n");
		return 1; 
	}

	FILE* f = fopen(av[1], "rb");
	if(!f) { perror(av[1]); exit(errno); }

	fseek(f, 0, SEEK_END);
	size_t file_size = ftell(f);
	size_t len = file_size < buf_size? file_size: buf_size;

	// читаем с конца
	char* buf = malloc(len);
	#define RETURN(CODE)	fclose(f); free(buf); return CODE

	if(fseek(f, -len, SEEK_END)) { perror("Не могу переместиться к концу."); RETURN(1);}

	printf("pos=%i len=%i file_size=%i\n", ftell(f), len, file_size);

	size_t size = fread(buf, 1, len, f);
	if(size == 0) { perror("Последний фрагмент не считан."); RETURN(1);}


	size_t max = find_max_fragment(buf, size);



	if(max == 0) {
		fprintf(stderr, "Повторяющихся фрагментов не найдено.\n");
		RETURN(1);
	}

	char* end = buf+size;
	int n = 0;
	for(char* s=end-max; s<end; s++) if(*s=='\n') n++;

	printf("max=%i pos=%i newline.end=%i\n", max, file_size-max, n);

	size_t mid = find_mid_fragment(buf+size-max, max);

	n = 0;
	for(char* s=end-mid; s<end; s++) if(*s=='\n') n++;

	printf("mid=%i pos=%i newline.end=%i\n", max, file_size-mid, n);

	cut_end_repeated_file(f, buf+size-mid, mid);
	
	RETURN(0);
}
