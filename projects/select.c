#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>

int main()
{
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    int maxfd = STDIN_FILENO + 1;

    while (1)
    {
        printf("Bir giriş bekleniyor...\n");

        int activity = select(maxfd, &readfds, NULL, NULL, NULL); // Activity, işlem yapılmış dosya tanımlayıcısı sayısına eşit olur.

        /*

        maxfd => İzelencek dosya tanımlıyıcalırının en büyük değeri. Hepsinin zilenebilmesi için en büyüğüne 1 eklenir.
        &readfds => İzlenecek dosya tanımlayıcılarının seti.
        NULL => Hangilerinin yazmaolayları kontrol edileceği ile ilgili kısım(writefds). Yazma olayları kontrol edilmeyeceği için NULL.
        NULL => İstisani olaylar için kontrol edilecek dosya tanımlayıcıları(exceptfds). Olmadığı için NULL.
        NULL => Giriş olayı için beklenecek süre. NULL sınırsız bekleyiş.

        */

        if (activity == -1)
        {
            perror("select");
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(STDIN_FILENO, &readfds)) // Okuma olayı gerçekleştiyse bloğa girilir(Kontrol sağlanır).
        {
            printf("Klavyeden giriş yapıldı!\n");

            char buffer[100];
            if (fgets(buffer, sizeof(buffer), stdin) != NULL) // stdin => Klavyeden girilen verileri okuyarak buffer'a ekler.
            {
                printf("Girilen metin: %s", buffer);

                // Eğer Enter tuşuna basıldıysa çıkış yap
                if (buffer[0] == '\n')
                {
                    break;
                }
            }
        }

        // readfds kümesini sıfırla
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
    }

    printf("Program sonlandırıldı.\n");

    return 0;
}
