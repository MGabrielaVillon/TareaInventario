#ifndef PRODUCTOS_H
#define PRODUCTOS_H

#include <time.h>

// Estructura para los productos
typedef struct{

    char codigo[20];
    char nombre[50];

    int cantidad;

    float costo;

    float precioVenta;

}Producto;

// Estructura para las ventas
typedef struct{

    int factura;

    char codigo[20];

    char nombre[50];

    int cantidad;

    float costo;

    float precioVenta;

    char fecha[11];

}Venta;

// Prototipos
Producto buscarProducto(char codigo[]);

void registrarVenta();

void actualizarInventario(char codigo[], int cantidadVendida);

void obtenerFecha(char fecha[]);

int obtenerNumeroFactura();

void guardarVenta(int numFactura, const char codigo[], const char nombre[], int cantidad, float costo, float precioVenta, const char fecha[]);

#endif