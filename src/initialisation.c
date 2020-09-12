#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

#include "../include/initialisation.h"
#include "../include/globals.h"


char* getType(char *ext){
	int indice = 0;

	while(indice <= size_mimes){
		if(strcmp(ext, mimes[indice].ext) == 0){
		return mimes[indice].type;
	}
	else
		indice++;
	}

	return NULL;
}

int addTypes(){
	int i = 0, j = 0, retour_read = 0;
	int fd;
	char char_lu;
	int presence_extension = 0;

	char type[100];
	char extension[100];

	memset(mimes, 0, sizeof(mimes));
	memset(type, 0, sizeof(type));
	memset(extension, 0, sizeof(extension));

	if((fd = open("mimes-types", O_RDWR, 0644)) == -1){
		perror("Erreur ouverture du fichier");
		return errno;
	}

	while( (retour_read = read(fd, &char_lu, sizeof(char))) > 0){

		if(retour_read == -1){
			perror("Erreur de lecture\n");
			return errno;
		}
		/*Ligne de commentaire dans le mime.types*/
		if(char_lu == '#'){
			while(char_lu != '\n'){
				read(fd, &char_lu, sizeof(char));
			}
			continue;
		}

		if(char_lu != '\n'){
			if(char_lu == '\t'){
				presence_extension = 1;
			}
			else if(presence_extension == 0){
				type[i] = char_lu;
				i++;
			}

			if(presence_extension == 1 && char_lu != '\t'){
				if(char_lu == ' '){
					j = 0;
					strcpy(mimes[size_mimes].ext, extension);
					strcpy(mimes[size_mimes].type, type);
					memset(extension, 0, sizeof(extension));
					size_mimes++;
				}
			    	else {
					extension[j] = char_lu;
					j++;
				}
			}
		}
    		else{
			i = 0;
			j = 0;
			if(presence_extension == 1){
				strcpy(mimes[size_mimes].ext, extension);
				strcpy(mimes[size_mimes].type, type);
				size_mimes++;
			}
			memset(type, 0, sizeof(type));
			memset(extension, 0, sizeof(extension));
			presence_extension = 0;
		}
	}

	return 1;
}

/*Recuperer le nom du chemin a partir du message envoye par le client qui se presente sous la forme :
  GET /chemin HTTP/1.1, on ne veut pas du premier '/' car il indique le repertoire racine du serveur */
char* getFilepath(char* client_msg){
	int prem_slash = 0; /*Si prem_slash == 1 alors le premier / a ete lu*/
	int i = 0, j = 0;
	char char_lu;
	char* filepath;

	filepath = malloc(sizeof(char) * 50);
	memset(filepath, 0, sizeof(char) * 50);

	while( (char_lu = client_msg[i]) != '\0' ){
		if(char_lu == '/' && prem_slash == 0){
			prem_slash++;
			i++;
			continue;
		}

		if(prem_slash == 1){
			if(char_lu != ' '){
				filepath[j] = char_lu;
				j++;
			}
			else
				break;
		}
		i++;
	}
	filepath[i] = '\0';
	return filepath;
}

/*Lui donner un filepath copie, car strtok va detruire le filepath completement,
  le filepath ne contiendra plus que la premiere chaine de caractere, celle qui
  se trouve avant le premier slash.
  Ex : samples/toto/file.pdf => filepath ne contiendra plus que samples a la fin
  */
char* getExtension(char* filepath){
	const char slash[2] = "/";
	const char point[2] = ".";
	char *token_filename, *token_extension;
	char filename[50], *extension;

	extension = malloc(sizeof(char) * 50);

	/*Recuperer nom du fichier*/
	token_filename = strtok(filepath, slash);
	while(token_filename != NULL){
		strcpy(filename, token_filename);
		token_filename = strtok(NULL, slash);
	}

	/*Recuperer extension*/
	token_extension = strtok(filename, point);
	while(token_extension != NULL){
		strcpy(extension, token_extension);
		token_extension = strtok(NULL, point);
	}

	return extension;
}
