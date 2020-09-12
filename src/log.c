#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "../include/struct.h"
#include "../include/globals.h"
#include "../include/log.h"


void addLog(requete *req){
	char log[500], time_now[50], size[50], pid[50], tid[20], ip[20];
	time_t rawtime;
	struct tm *timeinfo;
	unsigned int i; 
	int j_date;

	memset(log, 0, sizeof(log));

	/*ip clt*/
	sprintf(ip, "%s", inet_ntoa(req->sin.sin_addr));
	strcat(log, ip);
	strcat(log, " ");

	/*date et heure*/
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	memset(time_now, 0, sizeof(time_now));
	strcpy(time_now, asctime (timeinfo));/*Ajoute /n, le while qui suit sert à le retirer*/

	i = 0;
	while(i < sizeof(time_now)){
		if(time_now[i] == '\n'){
			time_now[i] = '\0';
		}
		i++;
	}
	strcat(log, time_now);

	/*pid server*/
	sprintf(pid, "%d", getpid());
	strcat(log, pid);

	/*tid*/
	sprintf(tid, "%ld", (long)pthread_self());
	strcat(log, tid);
	strcat(log, " ");

	/*msg*/
	if(strlen(req->msg) != 0){
		i = 0;
		j_date = 0;
		while(log[i] != '\0'){
			i++;
		}
		while(req->msg[j_date] != '\n'  && req->msg[j_date] != '\r'){ /*C'est une sequence CRLF (\r CARRIAGE RETURN suivi de \n LINE FEED) qui separe les lignes*/
			log[i] = req->msg[j_date];
			i++;
			j_date++;
		}
	}

	/*http_code*/
	strcat(log, " ");
	strcat(log, req->http_code);

	/*size file*/
	sprintf(size, "%d", req->size_file);
	strcat(log, size);
	strcat(log, "\n");

	/*ecrire dans le fichier de log*/
	pthread_mutex_lock(&fd_log_mutex);
	i = 0;
	while(log[i] != '\0'){
		if(write(fd_log, &log[i], sizeof(char)) < 0){
			perror("Erreur d'ecriture dans le fichier de log\n");
			/*return errno;*/
		}
		i++;
	}
	pthread_mutex_unlock(&fd_log_mutex);

	printf("Logs ajoutés.\n\n");
}
