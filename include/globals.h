#include "struct.h" /*TODO : A revoir surement pas la maniere optimale de faire ca*/
#include <mqueue.h>

#include <semaphore.h>

extern mime mimes[800];
extern int size_mimes;
extern int fd_log;
extern int seuil_octets;
extern pthread_mutex_t fd_log_mutex;
extern char filename_log[100];

extern mqd_t mq_des;
extern struct mq_attr attr;

extern list liste_ip;
extern list liste_req;

extern sem_t semaphore1;
extern sem_t semaphore2;
extern int cpt_ip; /*compte le nombre de threads restantes sur une liste avant de pouvoir ajouter ou supprimer un elem*/
extern pthread_mutex_t cpt_ip_mutex;

extern sem_t semaphore3;
