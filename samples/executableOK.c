#define _POSIX_SOURCE 1

#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[]){
	printf("HTTP/1.1 200 OK\nContent-Type: text/html\n");
	return 0;
}
