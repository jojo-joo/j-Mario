set path=%path%;C:\home\work\github\j-Mario\tools\tcc
del *.o
del *.exe
tcc.exe -c main.c
tcc.exe main.o -o main.exe
