#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

    char oku[80], gonder[80], yaz[80];

    FILE *fp;
    fp = fopen("sonuc.txt", "a");

    read(3, oku, 80); // ilk once pipe oku
    printf("child processin yazacağı: %s \n", oku);
    //fflush(stdout);
    strcpy(yaz, oku);
    fprintf(fp, "%s \n", yaz); //dosyaya yaz
    printf("dosyaya yazıldı..");
    
    // pipe sonucu ilet
    strcpy(gonder, "kayıt başarılı");
    write(4, gonder, 80);
    fclose(fp);
    return 0;
}