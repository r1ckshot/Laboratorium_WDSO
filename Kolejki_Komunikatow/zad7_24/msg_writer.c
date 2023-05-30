#ifdef __linux
char os[50] = "Apollo 13 successfully landed on the moon";
#else
char os[50] = "Houston, we have a problem";
#endif

#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msgbuf {
    long msg_type;
    char msg_text[50];
} message;

int main() {
    key_t key;
    int msgid;
    size_t buf_length;

    key = 1234;
    
    msgid = msgget(key, 0666 | IPC_CREAT);
    message.msg_type = 1;

    strcpy(message.msg_text,os);

    msgsnd(msgid, &message, sizeof(message), 0);

    return 0;
}
