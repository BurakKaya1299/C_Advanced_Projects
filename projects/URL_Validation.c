#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <curl/curl.h>

bool validate_domain(const char *url)
{
    char domain[100];
    int port = 80;
    sscanf(url, "https://%99[^:]:%99d/", domain, &port);

    /*

    sscanf() => Girilen değeri analiz ederek parçalara ayırır.
    url => İncelenecek string değer.
    "https://%99[^:]:%99d/" => Analiz edilecek stringin yapısını belirtir.
    domain => %99[^:] ifadesiyle belirtilen veri, ilk ":" karakterine kadar atanır.
    &port => %99d kısmını porta atar.

    */

    // Örn: sscanf(url, "https://www.example.com:8080/", domain, &port);
    // domain = www.example.com, port: 8080.

    return strcmp("internal.service", domain) == 0; // domain adresi ile standart adres aynı mı karşılaştırılması yapılır.
}

int main(int argc, char **argv)
{
    // [...]
    const char *url = argv[1];

    if (!validate_domain(url))
    {
        printf("validate_domain failed\n");
        return 1;
    }

    CURL *curl;   // Libcurl'de işlem yapabilmek için kullanılan veri tipi.
    CURLcode res; // Libcurl işleminin dönüş kodunu tutar.

    if ((curl = curl_easy_init())) // Libcurl işlevlerini kullanabilmek için CURL işaretçisi oluşturur.
    {
        curl_easy_setopt(curl, CURLOPT_URL, url); // İşlem yapılacak hedef url'yi atar.

        /*

        curl => İşlemi yürütmek için kullanılan işaretçi.
        CURLOPT_URL => Belirtilen adrese istek yapılmasını sağlayan parametre.
        url => İşlemlerin gerçekleştirileceği adres.

        */

        // CURLOPT_URL harici diğer parametrelerden bazıları;

        /*

        CURLOPT_FOLLOWLOCATION: İsteğin takip etmesi gereken yönlendirme (redirect) işlemlerini belirler.
        CURLOPT_POST: İsteğin bir POST isteği olarak gönderilmesini belirler.
        CURLOPT_CUSTOMREQUEST: Özel bir HTTP isteği türü belirlemek için kullanılır (GET, POST, PUT, DELETE, vb.).
        CURLOPT_COOKIE: İsteğe cookie bilgileri eklemek için kullanılır.

        */

        res = curl_easy_perform(curl); // Yapılması planlanan CURL işleminin gerçekleşmesini sağlar ve bir CURLcode değeri döndürür.
        switch (res)
        {
        case CURLE_OK:
            printf("All good!\n");
            break;
        default:
            printf("Nope :(\n");
            break;
        }
    }

    // [...]
    return 0;
}