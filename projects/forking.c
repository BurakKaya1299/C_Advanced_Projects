#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

// ONLY WORKS FOR UNİX!!!
int main()
{
    pid_t pid = fork();

    if (pid == -1)
    {
        // Fork hatası
        perror("Fork hatası");
        return 1;
    }
    else if (pid == 0)
    {
        // Çocuk işlem
        printf("Ben çocuk işlemim, PID: %d\n", getpid());
        printf("Ebeveyn işlem PID: %d\n", getppid());
        sleep(5); // Çocuk işlemi 5 saniye uyut
        printf("Çocuk işlem tamamlandı\n");
    }
    else
    {
        // Ebeveyn işlem
        printf("Ben ebeveyn işlemim, PID: %d\n", getpid());
        printf("Yeni oluşturulan çocuk işlem PID: %d\n", pid);
        sleep(2); // Ebeveyn işlemi 2 saniye uyut
        printf("Ebeveyn işlem tamamlandı\n");
    }

    return 0;
}
