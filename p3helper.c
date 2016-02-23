/* p3helper.c
 * Qian Yao
 * Program 3
 * CS570
 * John Carroll 
 * SDSU
 * Fall 2015
 *
 * This is the only file you may change. (In fact, the other files should
 * be symbolic links to:
 *   ~cs570/Three/p3main.c
 *   ~cs570/Three/p3.h
 *   ~cs570/Three/Makefile
 *   ~cs570/Three/CHK.h    )
 *
 */
#include "p3.h"

/* You may put any semaphore (or other global) declarations/definitions here: */
static sem_t mutex;/*guard exclusive access to reader and writer*/
static sem_t resources;/*guard exclusive access to shared resources*/
int readers;/*count the number of reader*/
int writers;/*count the number of writer*/
/* General documentation for the following functions is in p3.h
 * Here you supply the code:
 */
void initstudentstuff(void) {
  CHK(sem_init(&mutex,LOCAL,1));/*initialization of mutex semaphore*/
  CHK(sem_init(&resources,LOCAL,1));/*initialization of resources semaphore */
  writers = 0;/*initialization of writer*/
  readers = 0;/*initialization of reader*/
}
void prolog(int kind) {
    if(kind == 1){/*if the writer calls*/
       CHK(sem_wait(&mutex));/*get exclusive access to writer*/
       writers++;/*One more writer*/
       CHK(sem_post(&mutex));/*release exclusive access to writer*/
       CHK(sem_wait(&resources));/*get exclusive access to shared resources*/
    }
    if(kind == 0){/*if the reader calls*/
       CHK(sem_wait(&mutex));/*get exclusive access to reader*/
       if(writers > 0 || readers == 0){/*if this is the first reader or there is a writer calling to access to shared resources*/
          CHK(sem_post(&mutex));
          CHK(sem_wait(&resources));/*get exclusive access to resources*/
          CHK(sem_wait(&mutex));
       }
       readers++;/*One more reader*/
       CHK(sem_post(&mutex));
    }
}

void epilog(int kind) {
    if(kind == 1){/*if the writer calls*/
       CHK(sem_wait(&mutex));/*get exclusive access to writer*/
       writers--;
       CHK(sem_post(&mutex));/*release exclusive access to writer*/
       CHK(sem_post(&resources));/*release exclusive access to shared resources*/
    }
    if(kind == 0){/*if the reader calls*/
       CHK(sem_wait(&mutex));/*get exclusive access to reader*/
       readers--;
       if(readers == 0){/*if this is the last reader*/
         CHK(sem_post(&resources));/*release exclusive access to shared resources*/
       }
       CHK(sem_post(&mutex));/*release exclusive access to reader*/
      }
}  

