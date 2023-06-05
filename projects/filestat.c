#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

int main()
{
    FILE *file = fopen("salam.txt", "r");

    struct stat fileStat;
    const char *filename = "salam.txt";

    // Dosya istatistiklerini al
    if (stat(filename, &fileStat) == 0)
    {
        printf("Dosya Adı: %s\n", filename);
        printf("Dosya Boyutu: %lld bytes\n", fileStat.st_size);

        // Zaman değerlerini karakter dizisine dönüştür
        char accessTime[26];
        char modifyTime[26];
        char createTime[26];
        ctime_s(accessTime, sizeof(accessTime), &fileStat.st_atime);
        ctime_s(modifyTime, sizeof(modifyTime), &fileStat.st_mtime);
        ctime_s(createTime, sizeof(createTime), &fileStat.st_ctime);

        printf("Son Erişim Tarihi: %s", accessTime);
        printf("Son Değişiklik Tarihi: %s", modifyTime);
        printf("Son Oluşturma Tarihi: %s", createTime);
    }
    else
    {
        printf("Dosya istatistikleri alınamadı.\n");
    }
    fclose(file);
    return 0;
}
