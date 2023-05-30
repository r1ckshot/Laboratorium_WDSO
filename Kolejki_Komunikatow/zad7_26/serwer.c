#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// Odpowiednie wartosci liczbowe okreslajace,
// co nalezy odczytac z kolejki komunikatow.
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
    int sum = 0;

    key = 1234;
    msgid = msgget(key, 0666 | IPC_CREAT);

    // Odbieranie danych od klienta.
    while (1) {
        msgrcv(msgid, &message, sizeof(message), 0, 0); //Dowolny typ wiadomosci.
        if (message.msg_type == M_END)
            break;
        sum += message.msg_data;
    }

    // Wysylanie wyniku do klienta.
    message.msg_type = M_RESULT;
    message.msg_data = sum;
    msgsnd(msgid, &message, sizeof(message), 0);

    // Usuwanie kolejki.
    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}
