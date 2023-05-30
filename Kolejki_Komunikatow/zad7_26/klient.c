#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define M_DATA 1
#define M_END 2
#define M_RESULT 3

struct msg_buffer {
    long msg_type;
    int msg_data;
} message;

int main() {
    key_t key;
    int msgid;
    int data[] = {1, 2, 3, 4, 5};

    key = 1234;
    msgid = msgget(key, 0666 | IPC_CREAT);

    // Wysylanie danych do serwera.
    for (int i = 0; i < sizeof(data) / sizeof(data[0]); i++) {
        message.msg_type = M_DATA;
        message.msg_data = data[i];
        msgsnd(msgid, &message, sizeof(message), 0);
    }

    // Wysylanie konca danych do serwera.
    message.msg_type = M_END;
    msgsnd(msgid, &message, sizeof(message), 0);

    // Odbieranie wyniku od serwera.
    msgrcv(msgid, &message, sizeof(message), M_RESULT, 0);
    printf("Suma otrzymana od serwera: %d\n", message.msg_data);

    return 0;
}
