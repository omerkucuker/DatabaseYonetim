#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

//string compare
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
    int npipefd;
    //named pipe adresi
    char *myfifo = "/tmp/myfifo";
    //adresi pipe yapıyoruz 0666 rwx-rwx-rwx >0-110-110-110
    mkfifo(myfifo, 0666);

    char *gelen = (char *)calloc(80, sizeof(char)); //[80];
    char *giden = (char *)calloc(80, sizeof(char));
    int count = 1;
    int arraytemp = 0;
    char delim[] = " ";
    char sorguAyir[6][10];

    char dbAyir[2][10];

    while (1)
    {
        //bekleme
        printf("Waiting sorgu...\n");
        //gelen sorguyu oku
        npipefd = open(myfifo, O_RDONLY);
        read(npipefd, gelen, 80);
        printf("Gelen sorgu: %s\n", gelen);

        char *ptr = strtok(gelen, delim);
        while (ptr != NULL)
        {
            strcpy(sorguAyir[arraytemp], ptr);
            ptr = strtok(NULL, delim);
            arraytemp++;
        }
        //printf("Gelen sorgu: %s %s %s %s %s %s\n", sorguAyir[0], sorguAyir[1], sorguAyir[2], sorguAyir[3], sorguAyir[4],sorguAyir[5]);
        close(npipefd);
        char ad[3];
        char number[7];

        int index;
        int sorgulen;
        char *pch;
        pch = strchr(sorguAyir[5], '=');
        if (pch == NULL)
        {
            printf("= işareti yok. Doğru formatta sorgu yazınız. \n");
            strcat(giden, "NULL");
        }
        else
        {
            index = pch - sorguAyir[5];
            sorgulen = strlen(sorguAyir[5]) - index;
            char adSorgu[sorgulen];
            char numbSorgu[sorgulen];

            if (index == 2)
            {
                memcpy(ad, &sorguAyir[5][0], index);
                ad[2] = '\0';

                memcpy(adSorgu, &sorguAyir[5][index + 1], sorgulen - 1);
                adSorgu[sorgulen - 1] = '\0';
                printf("db den bakılacak: %s", adSorgu);
            }
            else if (index == 6)
            {
                memcpy(number, &sorguAyir[5][0], index);
                number[6] = '\0';

                memcpy(numbSorgu, &sorguAyir[5][index + 1], sorgulen - 1);
                numbSorgu[sorgulen - 1] = '\0';
                printf("db den bakılacak: %s", numbSorgu);
            }
            else
            {
                printf(" = yanlış yerde.. \n");
                strcat(giden, "NULL");
            }

            if (strcmp(sorguAyir[0], "select") == 0 && strcmp(sorguAyir[2], "from") == 0 && strcmp(sorguAyir[4], "where") == 0 &&
                (strcmp(sorguAyir[3], "veri1.txt") == 0 || strcmp(sorguAyir[3], "veri2.txt") == 0) &&
                (strcmp(ad, "ad") == 0 || strcmp(number, "number") == 0))
            {

                if (strcmp(sorguAyir[1], "*") == 0)
                {

                    if (index == 2)
                    {
                        FILE *fp;
                        char str[1000];
                        fp = fopen(sorguAyir[3], "r");
                        int flag = 0;
                        while (fgets(str, 1000, fp) != NULL)
                        {
                            int index2 = 0;
                            char *ptr2 = strtok(str, delim);

                            while (ptr2 != NULL)
                            {
                                strcpy(dbAyir[index2], ptr2);
                                ptr2 = strtok(NULL, delim);
                                index2++;
                            }

                            //printf("veriden okunan: %s , %s \n",dbAyir[0], dbAyir[1]);
                            //printf("sorgu uzunluk: %d , veri uzunluk: %d \n",strlen(adSorgu),strlen(dbAyir[0]));
                            if (compareStrings(adSorgu, dbAyir[0]) == 0)
                            {
                                printf("db karşılaştırma başarılı \n");
                                strcat(giden, dbAyir[0]);
                                strcat(giden, " ");
                                strcat(giden, dbAyir[1]);
                                flag = 1;
                            }
                        }
                        if (flag != 1)
                        {
                            strcpy(giden, "sorgu bulunamadı. \n");
                        }

                        fclose(fp);
                    }
                    else if (index == 6)
                    {
                        FILE *fp;
                        char str[1000];
                        fp = fopen(sorguAyir[3], "r");
                        int flag = 0;
                        while (fgets(str, 1000, fp) != NULL)
                        {
                            int index2 = 0;
                            char *ptr2 = strtok(str, delim);

                            while (ptr2 != NULL)
                            {
                                strcpy(dbAyir[index2], ptr2);
                                ptr2 = strtok(NULL, delim);
                                index2++;
                            }

                            //printf("veriden okunan: %s , %s \n",dbAyir[0], dbAyir[1]);
                            //printf("sorgu uzunluk: %d , veri uzunluk: %d \n",strlen(adSorgu),strlen(dbAyir[0]));
                            //printf("sorgu : %s , veri : %s \n", numbSorgu , dbAyir[1]);
                            int var1 = atoi(numbSorgu);
                            int var2 = atoi(dbAyir[1]);
                            if (var1 == var2)
                            {
                                //printf("db karşılaştırma başarılı \n");
                                strcat(giden, dbAyir[0]);
                                strcat(giden, " ");
                                strcat(giden, dbAyir[1]);
                                flag = 1;
                            }
                        }
                        if (flag != 1)
                        {
                            strcpy(giden, "sorgu bulunamadı. \n");
                        }

                        fclose(fp);
                    }
                    else
                    {
                        printf("hatalı parametre");
                    }
                }
                else if (strcmp(sorguAyir[1], "ad") == 0)
                {
                    if (index == 2)
                    {
                        FILE *fp;
                        char str[1000];
                        fp = fopen(sorguAyir[3], "r");
                        int flag = 0;
                        while (fgets(str, 1000, fp) != NULL)
                        {
                            int index2 = 0;
                            char *ptr2 = strtok(str, delim);

                            while (ptr2 != NULL)
                            {
                                strcpy(dbAyir[index2], ptr2);
                                ptr2 = strtok(NULL, delim);
                                index2++;
                            }

                            //printf("veriden okunan: %s , %s \n",dbAyir[0], dbAyir[1]);
                            //printf("sorgu uzunluk: %d , veri uzunluk: %d \n",strlen(adSorgu),strlen(dbAyir[0]));
                            if (compareStrings(adSorgu, dbAyir[0]) == 0)
                            {
                                //printf("db karşılaştırma başarılı \n");
                                strcat(giden, dbAyir[0]);
                                strcat(giden, " ");
                                //strcat(giden, dbAyir[1]);
                                flag = 1;
                            }
                        }
                        if (flag != 1)
                        {
                            strcpy(giden, "sorgu bulunamadı. \n");
                        }

                        fclose(fp);
                    }
                    else if (index == 6)
                    {
                        FILE *fp;
                        char str[1000];
                        fp = fopen(sorguAyir[3], "r");
                        int flag = 0;
                        while (fgets(str, 1000, fp) != NULL)
                        {
                            int index2 = 0;
                            char *ptr2 = strtok(str, delim);

                            while (ptr2 != NULL)
                            {
                                strcpy(dbAyir[index2], ptr2);
                                ptr2 = strtok(NULL, delim);
                                index2++;
                            }

                            //printf("veriden okunan: %s , %s \n",dbAyir[0], dbAyir[1]);
                            //printf("sorgu uzunluk: %d , veri uzunluk: %d \n",strlen(adSorgu),strlen(dbAyir[0]));
                            //printf("sorgu : %s , veri : %s \n", numbSorgu , dbAyir[1]);
                            int var1 = atoi(numbSorgu);
                            int var2 = atoi(dbAyir[1]);
                            if (var1 == var2)
                            {
                                //printf("db karşılaştırma başarılı \n");
                                strcat(giden, dbAyir[0]);
                                strcat(giden, " ");
                                //strcat(giden, dbAyir[1]);
                                flag = 1;
                            }
                        }
                        if (flag != 1)
                        {
                            strcpy(giden, "sorgu bulunamadı. \n");
                        }

                        fclose(fp);
                    }
                    else
                    {
                        printf("hatalı parametre");
                    }
                }
                else if (strcmp(sorguAyir[1], "number") == 0)
                {
                    if (index == 2)
                    {
                        FILE *fp;
                        char str[1000];
                        fp = fopen(sorguAyir[3], "r");
                        int flag = 0;
                        while (fgets(str, 1000, fp) != NULL)
                        {
                            int index2 = 0;
                            char *ptr2 = strtok(str, delim);

                            while (ptr2 != NULL)
                            {
                                strcpy(dbAyir[index2], ptr2);
                                ptr2 = strtok(NULL, delim);
                                index2++;
                            }

                            //printf("veriden okunan: %s , %s \n",dbAyir[0], dbAyir[1]);
                            //printf("sorgu uzunluk: %d , veri uzunluk: %d \n",strlen(adSorgu),strlen(dbAyir[0]));
                            if (compareStrings(adSorgu, dbAyir[0]) == 0)
                            {
                                //printf("db karşılaştırma başarılı \n");
                                //strcat(giden, dbAyir[0]);                                
                                strcat(giden, dbAyir[1]);
                                strcat(giden, " ");
                                flag = 1;
                            }
                        }
                        if (flag != 1)
                        {
                            strcpy(giden, "sorgu bulunamadı. \n");
                        }

                        fclose(fp);
                    }
                    else if (index == 6)
                    {
                        FILE *fp;
                        char str[1000];
                        fp = fopen(sorguAyir[3], "r");
                        int flag = 0;
                        while (fgets(str, 1000, fp) != NULL)
                        {
                            int index2 = 0;
                            char *ptr2 = strtok(str, delim);

                            while (ptr2 != NULL)
                            {
                                strcpy(dbAyir[index2], ptr2);
                                ptr2 = strtok(NULL, delim);
                                index2++;
                            }

                            //printf("veriden okunan: %s , %s \n",dbAyir[0], dbAyir[1]);
                            //printf("sorgu uzunluk: %d , veri uzunluk: %d \n",strlen(adSorgu),strlen(dbAyir[0]));
                            //printf("sorgu : %s , veri : %s \n", numbSorgu , dbAyir[1]);
                            int var1 = atoi(numbSorgu);
                            int var2 = atoi(dbAyir[1]);
                            if (var1 == var2)
                            {
                                //printf("db karşılaştırma başarılı \n");
                                //strcat(giden, dbAyir[0]);                                
                                strcat(giden, dbAyir[1]);
                                strcat(giden, " ");
                                flag = 1;
                            }
                        }
                        if (flag != 1)
                        {
                            strcpy(giden, "sorgu bulunamadı. \n");
                        }

                        fclose(fp);
                    }
                    else
                    {
                        printf("hatalı parametre");
                    }
                }
                else
                {
                    printf("select'ten sonra arada boşluk olacak şekilde *,number veya ad ile dosya adını doğru girdiğinizden emin olunuz! \n");
                    strcat(giden, "NULL");
                }
            }
            else
            {
                printf("Sorgu formatı 'select * from veri1.txt where ad=omer' şeklinde olmalıdır.. \n");
                strcat(giden, "NULL");
            }
        }

        //giden = "databasincevabı";
        printf("db den giden: %s\n", giden);
        //sonucu pipe yaz
        npipefd = open(myfifo, O_WRONLY);
        write(npipefd, giden, strlen(giden) + 1);
        close(npipefd);
        count++;
        arraytemp = 0;
        strcpy(giden, "");
    }
    return 0;
}
