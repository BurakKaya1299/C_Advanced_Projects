#include <windows.h>
#include <stdio.h>

int main() {
    FILETIME ft;
    ULARGE_INTEGER uli;

    GetSystemTimeAsFileTime(&ft);
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;

    ULONGLONG microseconds = uli.QuadPart / 10;
    ULONGLONG seconds = microseconds / 1000000;
    ULONGLONG microsecondsRemainder = microseconds % 1000000;

    printf("Seconds: %llu\n", seconds);
    printf("Microseconds: %llu\n", microsecondsRemainder);

    return 0;
}
