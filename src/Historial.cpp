#include "Historial.h"

#include <iostream>

Historial::Historial() : tope(nullptr), cantidad(0) {}

Historial::~Historial() { vaciar(); }

void Historial::vaciar() {
    while (tope != nullptr) {
        NodoViaje* borrar = tope;
        tope = tope->sig;
        delete borrar;
    }
    cantidad = 0;
}

void Historial::intercambiar(Historial& otro) {
    NodoViaje* t = tope;
    tope = otro.tope;
    otro.tope = t;
    int n = cantidad;
    cantidad = otro.cantidad;
    otro.cantidad = n;
}

void Historial::apilar(const Viaje& v) {
    NodoViaje* nuevo = new NodoViaje;
    nuevo->dato = v;
    nuevo->sig = tope;
    tope = nuevo;
    ++cantidad;
}

void Historial::agregarAlFondo(const Viaje& v) {
    NodoViaje* nuevo = new NodoViaje;
    nuevo->dato = v;
    nuevo->sig = nullptr;
    if (tope == nullptr) {
        tope = nuevo;
    } else {
        NodoViaje* ultimo = tope;
        while (ultimo->sig != nullptr) ultimo = ultimo->sig;
        ultimo->sig = nuevo;
    }
    ++cantidad;
}

bool Historial::verTope(Viaje& salida) const {
    if (tope == nullptr) return false;
    salida = tope->dato;
    return true;
}

void Historial::mostrar() const {
    if (tope == nullptr) {
        std::cout << "  (el historial esta vacio)\n";
        return;
    }
    int pos = 1;
    for (const NodoViaje* a = tope; a != nullptr; a = a->sig, ++pos)
        std::cout << "  " << pos << ". " << a->dato.nombrePasajero << " (ID "
                  << a->dato.idPasajero << "): " << a->dato.origen << " -> "
                  << a->dato.destino << "\n";
    std::cout << "  Total atendidos: " << cantidad << "\n";
}
