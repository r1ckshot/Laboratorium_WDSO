#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024  // Zdefiniowanie rozmiaru bufora
#define PORT 9734  // Zdefiniowanie portu serwera

void error_handling(char* message) {  // Funkcja obsługi błędów
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char* argv[]) {
    int serv_sock, clnt_sock;  // Deklaracja gniazd serwera i klienta
    char message[BUF_SIZE];  // Bufor na wiadomości od klienta
    int str_len, i;  // Długość odczytanej wiadomości i iterator

    struct sockaddr_in serv_adr, clnt_adr;  // Struktury przechowujące adresy serwera i klienta
    socklen_t clnt_adr_sz;  // Rozmiar struktury adresowej klienta

    // Tworzenie gniazda serwera
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1) error_handling("socket() error");

    // Przygotowanie struktury z adresem serwera
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(PORT);

    // Przypisanie adresu do gniazda
    if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    // Przygotowanie gniazda do odbierania połączeń
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    clnt_adr_sz = sizeof(clnt_adr);

    // Obsługa połączeń od klientów
    for(i = 0; i < 5; i++) {
        // Akceptacja połączenia od klienta
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        if(clnt_sock == -1) error_handling("accept() error");
        else {
            printf("Connected client %d \n", i+1);
            printf("Client IP: %s \n", inet_ntoa(clnt_adr.sin_addr)); //Wyswietlenie adresu klienta
        }

        // Odczytywanie wiadomości od klienta i wysyłanie ich z powrotem do niego w formie wielkich liter
        while((str_len = read(clnt_sock, message, BUF_SIZE)) != 0) {
            for(i = 0; i < str_len; i++) {
                message[i] = toupper(message[i]);
            }
            write(clnt_sock, message, str_len);
        }

        // Zamknięcie gniazda klienta
        close(clnt_sock);
    }
    // Zamknięcie gniazda serwera
    close(serv_sock);
    return 0;
}
