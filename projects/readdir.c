#include <stdio.h>
#include <dirent.h>

int main() {
    char* path = "."; // Listelenecek dizin
    DIR* directory = opendir(path);
    
    if (directory) {
        struct dirent* entry;
        
        while ((entry = readdir(directory)) != NULL) {
            printf("File Name: %s\n", entry->d_name);
        }
        
        closedir(directory);
    } else {
        printf("Dizin açılamadı: %s\n", path);
        return 1;
    }

    return 0;
}
