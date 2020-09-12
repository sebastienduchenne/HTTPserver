#define _POSIX_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char* argv[]){
	printf("Content-Type: text/plain; charset=utf-8\n");
	sleep(20);
	return 0;
}
