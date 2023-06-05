#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void signalHandler(int signum) {
    printf("Sinyal (%d) alındı. Program kapatılıyor...\n", signum);
    exit(0);
}

int main() {
    // SIGINT sinyaline signalHandler işlevini atama
    signal(SIGINT, signalHandler);

    // Sonsuz döngü
    while (1) {
        // Programın çalışmasını devam ettir
        // Burada programın normal işlemlerini gerçekleştirebilirsiniz
    }

    return 0;
}
