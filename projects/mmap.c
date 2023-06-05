#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FILE_SIZE 4096

int main()
{
    int fd;
    char *mapped_address;

    // Okuma ve yazma işlemleri için bir dosya açılır
    fd = open("mmap_example.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    /*

    "mmap_example.txt" => Oluşturulacak dosya adı-yolu.
    O_RDWR | O_CREAT => Dosyanın okunabilir,yazılabilir ve oluşturulacağını belirtir.
    S_IRUSR | S_IWUSR => Dosyayı kullanıcının okuyup yazabileceğini belirtir.

    */

    // S_IRUSR = Kullanıcı, S_IWUSR = Grup ve S_IWGRP = Diğerleri.

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    // Dosya boyutu belirlenir
    if (ftruncate(fd, FILE_SIZE) == -1) // fd dosyasının boyutunu FILE_SIZE'a eşitler.
    {
        perror("ftruncate");
        return 1;
    }

    // Belleğe dosya haritalanır
    mapped_address = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    /*

    NULL => Haritalanacak bölgenin başlangıç adresi. NULL ise işletim sistemi belirler.
    FILE_SIZE => Boyuru belirtir.
    PROT_READ | PROT_WRITE => Belleğin okuabilir ve yazılabilir olduğunu gösterir.
    MAP_SHARED => Belleğin paylaşılabilir olduğunu söyler.
    fd => Hangi dosyanın bu belleğe yerleştirileceği.
    0 => Yerleştirilecek dosyanın nereden yerleştirilmeye başlanacağı(baştan sona).

    */

    if (mapped_address == MAP_FAILED)
    {
        perror("mmap");
        return 1;
    }

    // Haritalanan bellek bölgesine veri yazılır
    strcpy(mapped_address, "Merhaba, dünya!");

    // Haritalanmış bellek bölgesi yazdırılır
    printf("Haritalanmış Adres: %s\n", mapped_address);

    // Bellek haritalaması kaldırılır
    if (munmap(mapped_address, FILE_SIZE) == -1)
    {
        perror("munmap");
        return 1;
    }

    // Dosya kapatılır
    if (close(fd) == -1)
    {
        perror("close");
        return 1;
    }

    return 0;
}
