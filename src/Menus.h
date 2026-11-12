// Menus.h - Interfaz de consola. Aqui solo hay interaccion; la logica de datos
// vive en las estructuras (ListaPasajeros, Grafo, ColaSolicitudes, Historial).
#ifndef MENUS_H
#define MENUS_H

#include "Sistema.h"

// Menu principal persistente: vuelve a mostrarse hasta elegir 0 (Salir).
void menuPrincipal(Sistema& sis);

#endif
