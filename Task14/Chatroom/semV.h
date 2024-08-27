#ifndef _SEMV_H_
#define _SEMV_H_

#include "common.h"
#include <sys/sem.h>

void sem_init (int _semid, int _sem_num);
void sem_reserve (int _semid, int _sem_num);
void sem_release (int _semid, int _sem_num); 

#endif