/* pamięć współdzielona – program rodzic-potomek */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define BUFSIZE 49

int main() {
    char c;
    int shmid;
    key_t key;
    char *shm, *msg;
    int err;
    char buf[BUFSIZ];
    key = 1234;

    /* utworzenie i dołączenie segment pamięci współdzielonej */
    if ((shmid = shmget(key, BUFSIZE, IPC_CREAT | 0666)) < 0) {
        perror("serwer – błąd shmget: ");
        exit(1);
    } else {
        printf("Serwer: utworzono segment pamięci współdzielonej o kluczu: %d (hexadecimal: %#08x) i ID: %d\n", key, key, shmid);
    }
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("serwer – błąd shmat: ");
        exit(2);
    } else {
        printf("Serwer: segment pamięci współdzielonej dołączono pod adresem: %p\n", shm);
    }

    int pid = fork();
    if (pid == 0) { /* proces potomny */
        /* odczyt danych zapisanych przez proces rodzica do pamięci współdzielonej */
        printf("Potomek: Odczyt z pamięci współdzielonej: %s\n", shm);
        
        /* usunięcie segmentu pamięci współdzielonej */
        sleep(3);
        if (shmctl(shmid, IPC_RMID, NULL) == -1) {
            perror("potomek – błąd shmctl: ");
            exit(1);
        }
        printf("Potomek: Segment pamięci współdzielonej usunięty\n");
    } else { /* proces rodzica */
        /* zapisanie danych do pamięci współdzielonej */
        strcpy(shm, "Czy potrafisz tak współdzielić pamięć z przyjacielem jak ja?");
        
        system("ipcs -m | head -3");
        sprintf(buf, "ipcs -m | grep %d", shmid);
        system(buf);
    }

    return 0;
}
