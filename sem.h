#ifndef FILE_SEM_H
#define FILE_SEM_H

#define  MAX_NOM 20
#define  MAX_PID 10

typedef struct Tinfo {
    char nom[ MAX_NOM]; // le nom du semaphore
    uid_t prop; // uid du process qui a crée le semaphore
    pid_t pids_wait[MAX_PID]; // tableau des pid en attente
    int pids_wait_nb; //taille
    int value;  // nombre de ressources du sema
    pid_t pids_att[MAX_PID]; // pids attachés
    int pids_att_nb; //taille
} Tinfo ;

#ifndef SEM_KERNEL
extern int sem_list(Tinfo * t, int tnb);
#endif

#endif //  FILE_SEM_H
