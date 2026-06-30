#include <stdio.h>

#include "usuarios.h"
#include "productos.h"

int main(){

    printf("-o-o-o-o-o-o-o-o-o-o-o-o-o-o-o-o-o-o-o-\n");
    printf(" Sistema de Ventas Electronicos\n");
    printf("-o-o-o-o-o-o-o-o-o-o-o-o-o-o-o-o-o-o-o-\n\n");

    if(login()){

        registrarVenta();

    }else{

        printf("\nCerrando el sistema...\n");

    }

    return 0;

}