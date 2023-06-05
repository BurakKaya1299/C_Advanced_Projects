#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <time.h>
#include <string.h>
#include <limits.h>

char *logger_path, *cmd;

void rotate_log()
{
    char log_old[PATH_MAX], log_new[PATH_MAX], timestamp[0x100]; // 0x 16'lık sayı sistemini temsil eder.
    time_t t;
    time(&t);
    strftime(timestamp, sizeof(timestamp), "%FT%T", gmtime(&t)); // Saat türünü belirli bir formata dönüştürmeye yarar.

    /*

    timestamp => Dönüştürülmüş formun tutulacağı dizi.
    sizeof(timestamp) => Güvenlik için maksimum boyut.
    "%FT%T" => Dönüştürülecek form %F: (YYYY-MM-DD) %T: (HH:MM:SS).
    gmtime(&t) => time_t türünü struct tm'ye değiştirerek UTC olmasını sağlar. Zaman değerini alır.

    */

    snprintf(log_old, sizeof(log_old), "%s/../logs/global.log", logger_path); //  Bir karakter dizisine belirli bir biçimde biçimlendirilmiş verileri yazmak için kullanılır.

    /*

    log_old => Verilerin yazıldığı işaretçi.
    sizeof(log_old) => Güvenlik için boyut.
    "%s/../logs/global.log" => Verinin ekleneceği form.
    logger_path => Hangi değere ekleneceği.

    */

    snprintf(log_new, sizeof(log_new), "%s/../logs/global-%s.log", logger_path, timestamp); // Fark olarak sadecde isimlendirme açısından zaman ibaresi eklenmiştir.
    execl("/bin/cp", "/bin/cp", "-a", "--", log_old, log_new, NULL);                        // Bir programı başlatmak için kullanılır.

    /*

    "/bin/cp" => Çalıştırılacak komutun yolu.
    "/bin/cp" => Çalıştırılacak komutun ismi(Burada yol ve isim aynı).
    "-a" => Komutun içerisindeki bir seçenek. Dosyanın tüm özelliklerini koruyarak kopyalanmasını sağlar.
    "--" => Seçenek seçiminin bittiğini söyler. Yani bundan sonraki argümanlar bir işlev değil, nesne olacaktır.
    log_old => Kopyalanacak olan dosya yolu.
    log_new => Nereye kopyalanacapı.
    NULL => Verilen nesnelerin bittiğini gösteren değer. exectl()'de nesneler bittiğinde sonunda NULL olmalıdır.

    */
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: /opt/logger/bin/loggerctl <cmd>\n");
        return 1;
    }

    if (setuid(0) == -1)
        return 1;

    if (seteuid(0) == -1)
        return 1;

    char *executable_path = argv[0];
    logger_path = dirname(strdup(executable_path));

    // strdup yol dizinini kopyalar ve dirname onun üzerinde değişiklikler yapar böylece ana değişken sabit kalır.

    cmd = strdup(argv[1]);

    if (!strcmp(cmd, "rotate"))
        rotate_log();
    else
    {
        printf("Available commands:\n");
        printf("- rotate: Rotate the log file\n");
    }

    return 0;
}
