/* Shared memory client */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define BUFSIZE 49

int main() {
    int shmid;
    key_t key;
    char *shm, *msg;
    int err;

    key = 1234;
    /* tworzenie segment pamięci współdzielonej */
    if ((shmid = shmget(key, BUFSIZE, 0666)) < 0) {
        perror("klient – błąd shmget: ");
        exit(1);
    } else {
        printf("Klient: utworzono segment pamięci współdzielonej o kluczu: %d (hexadecimal: %#08x) i ID: %d\n" , key, key, shmid);
    }
    /* dołączenie segment pamięci współdzielonej */
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("klient – błąd shmat: ");
        exit(1);
    } else {
        printf("Klient: dołączony segment pamięci współdzielonej pod adresem: %p\n", shm);
    }
    /* odczyt danych zapisanych przez serwer do pamięci współdzielonej */
    for (msg = shm; *msg != '\0'; msg++)
        putchar(*msg);
    putchar('\n');
    /* odłączenie segment pamięci współdzielonej */
    if (shmdt(shm) == -1) {
        perror("klient – błąd shmdt: ");
        exit(1);
    }
    /* odłączenie segment pamięci współdzielonej */
    shmctl(shmid, IPC_RMID, NULL);
    exit(0);
}
