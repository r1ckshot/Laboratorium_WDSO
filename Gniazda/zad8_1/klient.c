#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

#define BUF_SIZE 1024  // Zdefiniowanie rozmiaru bufora
#define RND_STRING_LEN 20  // Długość generowanego losowego ciągu znaków

void error_handling(char* message) {  // Funkcja obsługi błędów
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

// Funkcja generująca losowy ciąg znaków
char* generate_random_string(int length) {
    char* str = (char*) malloc((length + 1) * sizeof(char));
    const char charset[] = "abcdefghijklmnopqrstuvwxyz";

    if (str) {
        for (int i = 0; i < length; i++) {
            int key = rand() % (sizeof(charset) - 1);
            str[i] = charset[key];
        }
        str[length] = '\0';
    }
    return str;
}

int main(int argc, char* argv[]) {
    int sock;  // Deklaracja gniazda klienta
    char message[BUF_SIZE];  // Bufor na wiadomości od serwera
    int str_len;  // Długość odczytanej wiadomości
    struct sockaddr_in serv_adr;  // Struktura przechowująca adres serwera

    if(argc != 3) {  // Sprawdzenie czy podano adres IP i port jako argumenty
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    // Tworzenie gniazda klienta
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) error_handling("socket() error");

    // Przygotowanie struktury z adresem serwera
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    // Nawiązywanie połączenia z serwerem
    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error");
    else puts("Connected..........");

    // Generowanie losowego ciągu znaków i wysłanie go do serwera
    srand((unsigned int)time(0));
    char* rnd_str = generate_random_string(RND_STRING_LEN);
    write(sock, rnd_str, strlen(rnd_str));

    // Odczytywanie wiadomości od serwera
    str_len = read(sock, message, sizeof(message)-1);
    if(str_len == -1) error_handling("read() error");

    printf("Message from server: %s \n", message);

    // Zamknięcie gniazda klienta
    close(sock);
    return 0;
}
