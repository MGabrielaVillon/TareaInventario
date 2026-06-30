#include <stdio.h>
#include <string.h>

#include "usuarios.h"

// Función que valida el usuario y la contraseña leyendo el archivo vendedores.txt
int login(){

    Usuario vendedor;

    FILE *archivo;

    char usuario[30];
    char clave[30];

    int intentos = 0;

    while(intentos < 3){

        printf("Usuario: ");
        scanf("%29s", usuario);

        printf("Clave: ");
        scanf("%29s", clave);

        // Abrir el archivo de vendedores
        archivo = fopen("vendedores.txt", "r");

        if(archivo == NULL){
            printf("Error al abrir el archivo de vendedores.\n");
            return 0;
        }

        // Aquí se lee el archivo vendedores.txt
        while(fscanf(archivo, "%29[^,], %29[^,], %49[^\n]", vendedor.usuario, vendedor.clave, vendedor.nombre) == 3){
            if(strcmp(usuario, vendedor.usuario) == 0 && strcmp(clave, vendedor.clave) == 0){
                printf("Inicio de sesión exitoso. Bienvenido %s\n", vendedor.nombre);
                fclose(archivo);
                return 1;
            }
        }

        fclose(archivo);

        intentos++;

        printf("Usuario o clave incorrectos.\n");
        printf("Intentos restantes: %d\n", 3 - intentos);
    }

    printf("Ha superado el número máximo de intentos.\n");

    return 0;
}