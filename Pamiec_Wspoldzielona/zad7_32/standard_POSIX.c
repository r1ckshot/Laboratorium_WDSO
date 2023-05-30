#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

#define SIZE 49

int main() {
    char msg[SIZE] = "Can you share a memory with a friend like I do?";
    void* shmem = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
    int pid = fork();
    
    if (pid == 0) { /* proces potomny */
        sleep(1);
        printf("Potomek odczytał: %s\n", shmem);
    } else { /* proces rodzic */
        memcpy(shmem, msg, sizeof(msg));
        printf("Rodzic zapisał: %s pod adresem %p\n", shmem, &shmem);
        sleep(3);
        munmap(shmem, sizeof(msg));
    }
    
    return 0;
}
