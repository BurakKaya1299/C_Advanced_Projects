#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX 1024

struct message_buffer
{
    int msg_type;
    char msg_text[MAX];
};

int read_line(char text[], int n);
void writer(int msg_id);
void reader(int msg_id);

int main()
{
    int msg_id;
    pid_t pid;

    // Mesaj kuyruğunu oluştur
    msg_id = msgget(IPC_PRIVATE, 0600);

    /*

    IPC_PRIVATE => Mesaj kuyruğu için benzersiz değeri atar.
    0600 => Mesaj kuyrupunun iznini belirler(Okuma ve Yazma).

    */

    if (msg_id == -1)
    {
        perror("msgget");
    }

    pid = fork();
    switch (pid)
    {
    case -1:
        perror("fork");
        break;
    case 0:
        reader(msg_id); // Fork çocuk süreç için 0,
        break;
    default:
        writer(msg_id); // Ebeveyn için çocuk sürecin PID'sini döndürür.
        break;
    }

    // Mesaj kuyruğunu sil
    msgctl(msg_id, IPC_RMID, NULL);

    /*

    msg_id => Silinecek mesaj kuyruğu
    IPC_RMID => Mesaj kuyruğunu silen komut(aynı parametre kontrol için de kullanılır).
    NULL => Ek ayar.

    */

    return 0;
}

// Klavyeden satır okuma
int read_line(char text[], int n)
{
    char *ptext;
    int return_value;
    int length;
    ptext = fgets(text, n, stdin);

    /*

    text => Okunacak karakterler buraya kopyalanır.
    n => Okunacak karakter limiti. Son karakter \n olur.
    stdin => Girdi kaynağı olarak kullanılan dosya akışını temsil eden bir işaretçidir.

    */

    if (ptext == NULL)
    {
        return_value = EOF;
    }
    else
    {
        length = strlen(text);
        if (length > 0 && text[length - 1] == '\n')
        {
            text[length - 1] = '\0'; // Girdinin sadece metin kısmını alabilmek için,
        }                            // \n \0 ile değiştirilir.
        return_value = !EOF;
    }
    printf("Return value is %d\n", return_value);
    return return_value;
}

// Yazıcı işlemi
void writer(int msg_id)
{
    struct message_buffer m;
    m.msg_type = 1;

    while (read_line(m.msg_text, MAX) != EOF)
    {
        int length;
        length = strlen(m.msg_text);

        // Mesajı mesaj kuyruğuna gönder
        if (msgsnd(msg_id, &m, length, 0) == -1)

        /*

        msg_id => Mesajın gönderileceği mesaj kuyruğu numarası.
        &m => Gönderilmek istenen mesajın bellek adresini temsil eder.
        length => Mesaj uzunluğu
        0 => Flag seçeneği(default).

        */

        {
            perror("msgsnd");
            exit(1);
        }

        // Boş mesajı göndererek okuyucunun mesaj almasını sağla
        if (msgsnd(msg_id, &m, 0, 0) == -1)
        {
            perror("msgsnd");
            exit(1);
        }
    }
}

// Okuyucu işlemi
void reader(int msg_id)
{
    int length, n;
    n = 0;
    struct message_buffer m;

    // Mesajları al
    while ((length = msgrcv(msg_id, &m, MAX, 0, 0)) > 0)

    /*

    msg_id => Alınacak mesaj kuyrupunun id'si.
    m& => Alınacak mesajın bellek adresi.
    MAX => Alınacak mesaj uzunluğu
    0 => Alıncak mesaj tipi(Herhangi bir mesaj).
    0 => Mesaj alma şekli-flag(default).

    */

    {
        n += length; // Alınan mesaj büyüklüğü kadar n artar.
    }

    if (length == -1)
    {
        perror("msgrcv");
        exit(1);
    }

    printf("Received number of bytes is %d\n", n);
}
