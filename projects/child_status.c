#include <windows.h>
#include <stdio.h>

int main()
{
    DWORD childStatus;
    HANDLE childHandle;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);

    /* Çocuk sürecin oluşturulması. Lütfen, çocuk sürecin oluşturulması için
    kullanacağınız parametreleri ve
    ayarları senaryonuza uygun olarak ayarlamanız gerektiğini unutmayın.
    */
    if (CreateProcess(NULL, "child_process.exe", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        // Çocuk sürecin handle'ının alınması
        childHandle = pi.hProcess;

        // Çocuk sürecin tamamlanmasının beklendiği işlem
        DWORD childPid = WaitForSingleObject(childHandle, INFINITE);
        printf("Child PID: %lu\n", childPid);

        // Çocuk sürecin handle'ının serbest bırakılması
        CloseHandle(childHandle);
    }
    else
    {
        printf("Çocuk sürecin oluşturulması başarısız oldu. Hata: %lu\n", GetLastError());
    }

    return 0;
}
