#ifdef __linux
char os[50] = "Apollo 13 successfully landed on the moon";
#else
char os[50] = "Houston, we have a problem";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define BUFSIZE 49

// struktura dla kolejki komunikatów
struct msg_buffer {
    long msg_type;
    char msg_text[BUFSIZE+1];
} message;

int main() {
    key_t key;
    int msgid;
    char buf[BUFSIZ];

    // utworzenie kolejki komunikatów
    key = 1234;
    msgid = msgget(key, 0666 | IPC_CREAT);
    message.msg_type = 1;

    int pid = fork();
    if (pid == 0) { /* proces potomny */
        // wysłanie wiadomości do kolejki
        strcpy(message.msg_text,os);
        msgsnd(msgid, &message, sizeof(message), 0);

        system("ipcs -q | head -3");
        sprintf(buf, "ipcs -q | grep %d", msgid);
        system(buf);
    } else { /* proces rodzic */
        sleep(3);
        // odczytujemy wiadomość
        msgrcv(msgid, &message, sizeof(message), 1, 0);

        printf("\033[1;31m"); /*drukujemy wiadomość na czerwono*/
        // wyświetlamy wiadomość
        printf("Data Received is : %s\n", message.msg_text);
        printf("\033[0m"); /* powracamy do domyślnych kolorów */

        // kasujemy kolejkę komunikatów
        msgctl(msgid, IPC_RMID, NULL);
    }
    return 0;
}
