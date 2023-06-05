#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024

void handleClient(SOCKET clientSocket);

int main()
{
    printf("sa");
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    int clientAddressLength;

    // Winsock başlatma
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        perror("Winsock başlatma hatası");
        exit(EXIT_FAILURE);
    }

    // Sunucu soketi oluştur
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    /*

    AF_INET => IPv4 adres ailesini temsil eder.
    SOCK_STREAM => Verilerin ardışık bir akış halinde aktarılacağı bir iletişim kanalıdır.
    0 => Varsayılan soket protokolü.

    */

    if (serverSocket == INVALID_SOCKET)
    {
        perror("Sunucu soketi oluşturulamadı");
        exit(EXIT_FAILURE);
    }

    // Sunucu adresi ayarla
    serverAddress.sin_family = AF_INET;         // Adres ailesi yani IPv4'ü simgeler.
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Sunucunun tüm bağlantıları kabul etmesini sağlar.
    serverAddress.sin_port = htons(8080);       // Dinlenecek port numarası.

    /*

    serverAddress.sin_addr.s_addr = inet_addr("192.168.0.100"); Belirli bir IP adresi

    struct hostent *host;
    host = gethostbyname("example.com"); Host adını IP adresine çevir
    serverAddress.sin_addr = *((struct in_addr *)host->h_addr);

    */

    // Sunucuyu bağla
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
    {
        perror("Sunucu bağlantısı hatası");
        exit(EXIT_FAILURE);
    }

    // İstemci bağlantılarını dinle
    if (listen(serverSocket, MAX_CLIENTS) == SOCKET_ERROR)
    {
        perror("Dinleme hatası");
        exit(EXIT_FAILURE);
    }

    getchar();
    printf("Sunucu başlatıldı. İstemci bağlantılarını dinliyor...\n");
    

    // İstemci bağlantılarını kabul et
    while (1)
    {
        clientAddressLength = sizeof(clientAddress);
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
        if (clientSocket == INVALID_SOCKET)
        {
            perror("İstemci bağlantısı kabul edilemedi");
            exit(EXIT_FAILURE);
        }

        printf("Yeni bir istemci bağlandı: %s:%d\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));

        // İstemci işleme fonksiyonunu başlat
        handleClient(clientSocket);
    }

    // Sunucu soketini kapat
    closesocket(serverSocket);

    // Winsock kapatma
    WSACleanup();

    return 0;
}

void handleClient(SOCKET clientSocket)
{
    char buffer[BUFFER_SIZE];
    int bytesRead;
    int continueLoop = 1; // Döngü kontrol değişkeni

    // İstemciden veri al ve işle
    while (continueLoop && (bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0)) > 0)
    {
        // Gelen veriyi ekrana yaz
        printf("İstemciden gelen veri: %.*s\n", bytesRead, buffer);

        // İstemciye geri gönder
        if (send(clientSocket, buffer, bytesRead, 0) == SOCKET_ERROR)
        {
            perror("Veri gönderme hatası");
            exit(EXIT_FAILURE);
        }

        // Belleği temizle
        memset(buffer, 0, BUFFER_SIZE);

        // Döngüyü sonlandırmak için continueLoop değerini 0 yap
        if (bytesRead == 0)
        {
            // İstemci bağlantısı kapatıldı
            printf("İstemci bağlantısı sonlandırıldı\n");
            continueLoop = 0;
        }
        else if (bytesRead == SOCKET_ERROR)
        {
            // Hata oluştu
            perror("Veri alma hatası");
            exit(EXIT_FAILURE);
        }
    }

    // İstemci soketini kapat
    closesocket(clientSocket);
}
