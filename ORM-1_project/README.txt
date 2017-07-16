UPUSTVO ZA PREVODJENJE I POKRETANJE PROGRAMSKOG KODA RESENJA

Svi source i make fajlovi se nalaze u src direktorijumu.

1) Potrebno je uci u zeljeni modul zadataka (svaki modul se nalazi u posebnom direktorijumu).
   Moduli:
   -Client
   -Server
   -SMTPserver
2) Nakon odabranog modula potrebno je prevesti modul ako vec nije preveden (u svakom modulu se nalazi makefile).
   Komanda u terminalu:
   -Client 
     make client
   -Server 
     make server
   -SMTPserver
     make SMTPserver
   Ako je modul vec preveden moguce ga je ukloniti i ponovo prevesti.
   Komanda u terminalu:
   -Client 
     make client_clean
   -Server 
     make server_clean
   -SMTPserver
     make SMTPserver_clean
3) Nakon prevodjenja modul se moze pokrenuti (ocitati IP adresu i proslediti kao argument komandne linije).
   Komanda u terminalu:
   -Client 
     ./client 192.168.0.12
   -Server 
     ./server 192.168.0.10 192.168.0.12
   -SMTPserver
     ./SMTPserver 192.168.0.10

NAPOMENA: 
   Da bi sistem radio ispravno potrebno je prvo pokrenuti serverski modul a zatim SMTP server koji se konektuje na njega,a zatim 
   pokrenuti klijentsku stranu.