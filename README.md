
## DatabaseYonetim
Bu proje üç adet programdan oluşmaktadır. Programın temel amacı Named pipe ile farklı
terminallerden çalıştırdığımız process’ler ile haberleşme yapılmasını ve fork, pipe ve exec ile
de child process ile haberleşme yapılarak programların senkronize bir şekilde çalışmasını
öğrenmektedir.
Program.c programı çalıştığı zaman kullanıcıdan bir sorgu girmesini bekler. Gelen sorguyu
Named Pipe ile Database.c programına gönderir. Database.c programından gelen bilgiyi okur,
gelen bilgi NULL değilse kullanıcıya “sorgu sonucu kaydedilsin mi? e/h” diye sorar. Kullanıcı
evet derse fork yapılarak pipe ile child process’den exec ile Kaydet.c process’ine bu bilgiyi
gönderir.
Database.c programı program.c process’inden gelen sorguyu parse edip istenilen datayı
veri1.txt veya veri2.txt dosyasında arar. Sorgu sonucunu program.c dosyasına geri gönderir.
Kaydet.c processi parant process olan Program.c den gelen datayı sonuc.txt dosyasına yazar.
Parent process’e işlemin başarılı bir şekilde yapıldığı mesajını gönderir.
Sorgu örnekleri:
select * from veri1.txt where ad=fatih
select number from veri2.txt where ad=selin
select ad from veri2.txt where number=2

![SS1](https://github.com/omerkucuker/DatabaseYonetim/blob/main/ss1.png?raw=true)
![SS2](https://github.com/omerkucuker/DatabaseYonetim/blob/main/ss2.png?raw=true)
![SS3](https://github.com/omerkucuker/DatabaseYonetim/blob/main/ss3.png?raw=true)



