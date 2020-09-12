#ifndef STRUCT_H
#define STRUCT_H
#define BUFFERSIZE 200

#include <netinet/in.h>

typedef struct mime mime;
struct mime{
	char type[100];
	char ext[20];
};

typedef struct requete requete;
struct requete{
	struct sockaddr_in sin;
	char msg[2000];
	char http_code[10];
	int soc_com;
	int size_file;
	pthread_t thread_to_join; /*L'id du thread a join*/
};

typedef struct executable executable;
struct executable{
	int pid_fils;
	int killed;
};

typedef struct infos_watcher infos_watcher;
struct infos_watcher{
	char ip[20];
	int data; /*en octets*/
	int timer; /*10 secs pour liste_ip et 60 secs pour liste_req  */
	pthread_t tid;
	pthread_mutex_t case_mutex; /*Le thread watcher va initialiser le mutex PTHREAD_MUTEX_INITIALIZER*/
};

typedef struct elem elem;
struct elem{
	elem *prev;
	infos_watcher info;
	elem *next;
};

typedef struct list list;
struct list{
	elem *first;
	elem *last;
};


#endif
