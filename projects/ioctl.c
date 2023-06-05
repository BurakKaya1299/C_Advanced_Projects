#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main()
{
    struct winsize ws;

    // Terminal boyutunu al
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != -1)
    {
        int terminalWidth = ws.ws_col;
        int terminalHeight = ws.ws_row;

        printf("Terminal Genişliği: %d\n", terminalWidth);
        printf("Terminal Yüksekliği: %d\n", terminalHeight);
    }
    else
    {
        perror("ioctl");
        return 1;
    }

    return 0;
}
