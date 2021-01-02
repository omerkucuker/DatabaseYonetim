#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

int compareStrings(char *x, char *y)
{
  int flag = 0;

  // Iterate a loop till the end
  // of both the strings
  while (*x != '\0' || *y != '\0')
  {
    if (*x == *y)
    {
      x++;
      y++;
    }

    // If two characters are not same
    // print the difference and exit
    else if ((*x == '\0' && *y != '\0') || (*x != '\0' && *y == '\0') || *x != *y)
    {
      flag = 1;
      //printf("Uequal Strings\n");
      break;
    }
  }

  // If two strings are exactly same
  if (flag == 0)
  {
    return 0;
  }
}

int main(int argc, char *argv[])
{
  //named pipe adresi

  char *myfifo = "/tmp/myfifo"; //adresi pipe yapıyoruz 0666 rwx-rwx-rwx >0-110-110-110
  mkfifo(myfifo, 0666);
  int npipefd;
  char gelen[80], giden[80];
  char *childeGiden[80];
  char pipesonuc[80];
  int pipefd[2];

  if (pipe(pipefd) < 0)
  {
    perror("pipe");
    exit(1);
  }
  while (1)
  {

    printf("Sorgu giriniz = ");
    /*command = readLine();
        param = splitLine(command);
        //printf("%s %s \n",param[0], param[1]);
        for(int i=1;i<6;i++){//terminalden girilen sorguyu parse edip tek bir string şekline iletiyoruz.
            strcat(param[0], param[i]);
        }        
       printf("%s",param[0]);*/

    //sorguyu gonder, database.c ye giden kısım
    npipefd = open(myfifo, O_WRONLY);
    fgets(giden, 80, stdin);
    write(npipefd, giden, strlen(giden) + 1);
    close(npipefd);

    //database.c den gelen kısım

    npipefd = open(myfifo, O_RDONLY);
    read(npipefd, gelen, sizeof(gelen));
    close(npipefd);
    strcpy(childeGiden, gelen);
    if (strcmp(gelen, "NULL") != 0)
    {

      char yanit[2];
      printf("Sorgu başarılı: %s \nSonuç kaydedilsin mi? Evet için e ; Hayır için h giriniz: ", childeGiden);
      fgets(yanit, 80, stdin);
      printf("Seçiminiz: %s", yanit);
      if (compareStrings(yanit, "e") == 0)
      {

        int c, pid;
        //pipe olustur

        pid = fork();

        if (pid == 0)
        {
          //fflush(stdout);
          //gelen cevabı pipe yaz
          printf("childe parentten giden: %s \n", childeGiden);

          write(pipefd[1], childeGiden, strlen(childeGiden) + 1);
          // kaydet programini cagir
          c = execv("kaydet", NULL);
          // hata olusursa
          perror("");
        }
        else
        {

          wait(&c);                                      //kaydet programinin cagirildigi prosesi bekle
          read(pipefd[0], pipesonuc, sizeof(pipesonuc)); //sonucu pipe oku
          printf("child cevabı: %s\n", pipesonuc);       //ekrana yazdır
        }
      }
      else
      {
        printf("Sorgu sonucu kaydedilmedi veya 'e' 'h' harici seçim yaptiniz. Sonuc: %s \n", gelen);
      }
    }
    else
    {
      printf("Sorgu bulunamadı. Sorguyu doğru formatta giriniz. Gelen cevap: %s \n", gelen);
    }
  }
  close(pipefd[0]);
  close(pipefd[1]);
  return 0;
}
