// Sistema.h - Agrupa las estructuras de datos del programa.
#ifndef SISTEMA_H
#define SISTEMA_H

#include "ColaSolicitudes.h"
#include "Grafo.h"
#include "Historial.h"
#include "ListaPasajeros.h"

struct Sistema {
    ListaPasajeros pasajeros;   // lista enlazada
    Grafo grafo;                // lista de adyacencia
    ColaSolicitudes cola;       // cola enlazada
    Historial historial;        // pila enlazada
};

#endif
