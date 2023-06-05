#include <stdio.h>
#include <windows.h>

DWORD WINAPI threadFunction(LPVOID lpParam)
{
    printf("Merhaba, ben bir thread'im!\n");
    return 0;
}

int main()
{
    HANDLE hThread; // özel windows thread veri tipi.
    DWORD threadId; // thread PID tutma değişkeni ve veri tipi.

    hThread = CreateThread(NULL, 0, threadFunction, NULL, 0, &threadId);

    /*
    
    NULL => Standart thread özellikleri kullanıldığını gösterir.
    0 => Yığın boyutunu gösterir. Varsayılan yığın boyutu için 0 kullanılır.
    threadFunction => Çalıştırılacak işlev.
    NULL => İşlev kısmına gönderilecek parametre. Eğer gönderilmeyecekse NULL.
    0 => İş parçacığı oluşturma seçeneği 0 default ayar.
    &threadId => PID nin depolandığı kısım
    
    */

    if (hThread == NULL)
    {
        printf("Thread oluşturma hatası\n");
        return 1;
    }

    // Diğer işlemlerin devam etmesi için thread'in bitmesini bekleyelim
    WaitForSingleObject(hThread, INFINITE);

    CloseHandle(hThread);

    printf("Ana program bitti.\n");
    return 0;
}
