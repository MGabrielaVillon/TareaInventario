#ifndef USUARIOS_H
#define USUARIOS_H

typedef struct{

    char usuario[30];
    char clave[30];
    char nombre[50];

}Usuario;

int login();

#endif