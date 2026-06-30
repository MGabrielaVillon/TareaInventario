#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "productos.h"

Producto buscarProducto(char codigo[]){

    Producto producto;
    producto.codigo[0] = '\0';

    // Abrir el archivo de productos para buscar el código ingresado
    FILE *archivo = fopen("producto.txt", "r");

    if(archivo == NULL){
        printf("Error al abrir el archivo de productos.\n");
        return producto;
    }

    while(fscanf(archivo,
                 "%19[^,],%49[^,],%d,%f,%f",
                 producto.codigo,
                 producto.nombre,
                 &producto.cantidad,
                 &producto.costo,
                 &producto.precioVenta) == 5){

        if(strcmp(codigo, producto.codigo) == 0){

            fclose(archivo);
            return producto;

        }
    }

    fclose(archivo);

    return producto;
}

/* NUEVA FUNCIÓN */
void actualizarInventario(char codigo[], int cantidadVendida){

    FILE *archivo;
    FILE *temporal;

    Producto producto;

    archivo = fopen("producto.txt", "r");
    temporal = fopen("temp.txt", "w");

    if(archivo == NULL || temporal == NULL){
        printf("Error al actualizar el inventario.\n");
        return;
    }

    while(fscanf(archivo,
                 "%19[^,],%49[^,],%d,%f,%f",
                 producto.codigo,
                 producto.nombre,
                 &producto.cantidad,
                 &producto.costo,
                 &producto.precioVenta) == 5){

        if(strcmp(producto.codigo, codigo) == 0){
            producto.cantidad -= cantidadVendida;
        }

        fprintf(temporal,
                "%s,%s,%d,%.2f,%.2f\n",
                producto.codigo,
                producto.nombre,
                producto.cantidad,
                producto.costo,
                producto.precioVenta);
    }

    fclose(archivo);
    fclose(temporal);

    remove("producto.txt");
    rename("temp.txt", "producto.txt");
}

// Obtener la fecha actual en formato aaaa-mm-dd
void obtenerFecha(char fecha[]){
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(fecha, 11, "%Y-%m-%d", tm_info);
}

// Obtener el siguiente número de factura
int obtenerNumeroFactura(){
    FILE *archivo = fopen("ventas.txt", "r");
    int numFactura = 1;
    int ultimo = 0;
    char linea[200];

    if(archivo != NULL){
        while(fgets(linea, sizeof(linea), archivo) != NULL){
            if(sscanf(linea, "%d", &ultimo) == 1){
                numFactura = ultimo + 1;
            }
        }
        fclose(archivo);
    }

    return numFactura;
}

// Guardar una venta en el archivo ventas.txt
void guardarVenta(int numFactura, const char codigo[], const char nombre[], int cantidad, float costo, float precioVenta, const char fecha[]){
    FILE *archivo = fopen("ventas.txt", "a");

    if(archivo == NULL){
        printf("Error al guardar la venta.\n");
        return;
    }

    fprintf(archivo, "%d,%s,%s,%d,%.2f,%.2f,%s\n", numFactura, codigo, nombre, cantidad, costo, precioVenta, fecha);
    fclose(archivo);
}

void registrarVenta(){

    char codigo[20];
    int cantidadVenta;
    char continuar;
    int numFactura;
    char fecha[11];
    float totalVenta = 0;
    int productoCount = 0;
    
    // Estructuras para almacenar temporalmente los detalles de venta
    Venta ventas[100];
    Producto producto;

    // Obtener número de factura y fecha
    numFactura = obtenerNumeroFactura();
    obtenerFecha(fecha);

    printf("\n========================================\n");
    printf("        NUEVA VENTA\n");
    printf("Factura No: %d\n", numFactura);
    printf("Fecha: %s\n", fecha);
    printf("========================================\n\n");

    // Loop para agregar múltiples productos
    do {
        if(productoCount >= 100){
            printf("Se ha alcanzado el límite de productos por factura.\n");
            break;
        }

        printf("Ingrese el código del producto (o 'salir' para terminar): ");
        scanf("%19s", codigo);

        if(strcmp(codigo, "salir") == 0){
            break;
        }

        producto = buscarProducto(codigo);

        if(producto.codigo[0] == '\0'){
            printf("Producto no encontrado.\n\n");
            continue;
        }

        printf("\n=== Producto encontrado ===\n");
        printf("Código: %s\n", producto.codigo);
        printf("Nombre: %s\n", producto.nombre);
        printf("Cantidad disponible: %d\n", producto.cantidad);
        printf("Precio unitario: %.2f\n\n", producto.precioVenta);

        printf("Ingrese la cantidad a vender: ");
        scanf("%d", &cantidadVenta);

        if(cantidadVenta > producto.cantidad){
            printf("No hay suficiente inventario. Disponible: %d\n\n", producto.cantidad);
            continue;
        }

        if(cantidadVenta <= 0){
            printf("La cantidad debe ser mayor a 0.\n\n");
            continue;
        }

        // Guardar en el array temporal
        ventas[productoCount].factura = numFactura;
        strcpy(ventas[productoCount].codigo, producto.codigo);
        strcpy(ventas[productoCount].nombre, producto.nombre);
        ventas[productoCount].cantidad = cantidadVenta;
        ventas[productoCount].costo = producto.costo;
        ventas[productoCount].precioVenta = producto.precioVenta;
        strcpy(ventas[productoCount].fecha, fecha);

        totalVenta += cantidadVenta * producto.precioVenta;
        productoCount++;

        printf("Subtotal: %.2f\n\n", cantidadVenta * producto.precioVenta);

        printf("¿Desea agregar otro producto? (s/n): ");
        scanf(" %c", &continuar);

    } while(continuar == 's' || continuar == 'S');

    if(productoCount == 0){
        printf("\nNo se agregaron productos. Transacción cancelada.\n");
        return;
    }

    // Mostrar resumen
    printf("\n========================================\n");
    printf("          RESUMEN DE VENTA\n");
    printf("Factura No: %d\n", numFactura);
    printf("Fecha: %s\n", fecha);
    printf("========================================\n\n");

    printf("%-15s %-30s %10s %12s %12s\n", "Código", "Nombre", "Cantidad", "Precio Unit.", "Total");
    printf("---------------------------------------------------------------------------\n");

    for(int i = 0; i < productoCount; i++){
        float totalProducto = ventas[i].cantidad * ventas[i].precioVenta;
        printf("%-15s %-30s %10d %12.2f %12.2f\n",
               ventas[i].codigo,
               ventas[i].nombre,
               ventas[i].cantidad,
               ventas[i].precioVenta,
               totalProducto);
    }

    printf("---------------------------------------------------------------------------\n");
    printf("%65s %.2f\n", "TOTAL A PAGAR: $", totalVenta);
    printf("========================================\n\n");

    printf("¿Confirma la venta? (s/n): ");
    scanf(" %c", &continuar);

    if(continuar != 's' && continuar != 'S'){
        printf("Transacción cancelada.\n");
        return;
    }

    // Procesar la venta: actualizar inventario y guardar en archivo
    for(int i = 0; i < productoCount; i++){
        actualizarInventario(ventas[i].codigo, ventas[i].cantidad);
        guardarVenta(numFactura,
                     ventas[i].codigo,
                     ventas[i].nombre,
                     ventas[i].cantidad,
                     ventas[i].costo,
                     ventas[i].precioVenta,
                     ventas[i].fecha);
    }

    printf("\nVenta registrada exitosamente.\n");
    printf("Inventario actualizado correctamente.\n");
}