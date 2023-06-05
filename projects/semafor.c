#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define KEY 1234

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void sem_wait(int semid)
{
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = -1; // Azaltma işlemi
    sb.sem_flg = 0;
    semop(semid, &sb, 1);
}

void sem_signal(int semid)
{
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = 1; // Artırma işlemi
    sb.sem_flg = 0;
    semop(semid, &sb, 1);
}

int main()
{
    int semid, pid;
    union semun arg;

    // Semafor oluşturma
    semid = semget(KEY, 1, IPC_CREAT | 0666);
    if (semid == -1)
    {
        perror("semget");
        exit(1);
    }

    // Semafor değerini 1 olarak ayarlama
    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1)
    {
        perror("semctl");
        exit(1);
    }

    // Çocuk süreç oluşturma
    pid = fork();

    if (pid == -1)
    {
        perror("fork");
        exit(1);
    }
    else if (pid == 0)
    {
        // Çocuk süreç
        printf("Çocuk süreç: Semafordan izin bekleniyor...\n");
        sem_wait(semid);
        printf("Çocuk süreç: Semafordan izin alındı. Kritik bölgeye giriş yapılıyor.\n");

        // Kritik bölge
        sleep(2);

        printf("Çocuk süreç: Kritik bölge işlemi tamamlandı. Semafora izin veriliyor.\n");
        sem_signal(semid);
    }
    else
    {
        // Ebeveyn süreç
        printf("Ebeveyn süreç: Semafordan izin bekleniyor...\n");
        sem_wait(semid);
        printf("Ebeveyn süreç: Semafordan izin alındı. Kritik bölgeye giriş yapılıyor.\n");

        // Kritik bölge
        sleep(2);

        printf("Ebeveyn süreç: Kritik bölge işlemi tamamlandı. Semafora izin veriliyor.\n");
        sem_signal(semid);
    }

    // Semaforı silme
    if (semctl(semid, 0, IPC_RMID) == -1)
    {
        perror("semctl");
        exit(1);
    }

    return 0;
}
