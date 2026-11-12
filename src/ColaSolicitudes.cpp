#include "ColaSolicitudes.h"

#include <iostream>

#include "Utilidades.h"

ColaSolicitudes::ColaSolicitudes() : frente(nullptr), fin(nullptr), cantidad(0) {}

ColaSolicitudes::~ColaSolicitudes() { vaciar(); }

void ColaSolicitudes::vaciar() {
    while (frente != nullptr) {
        NodoSolicitud* borrar = frente;
        frente = frente->sig;
        delete borrar;
    }
    fin = nullptr;
    cantidad = 0;
}

void ColaSolicitudes::intercambiar(ColaSolicitudes& otra) {
    NodoSolicitud* f = frente;
    frente = otra.frente;
    otra.frente = f;
    NodoSolicitud* e = fin;
    fin = otra.fin;
    otra.fin = e;
    int n = cantidad;
    cantidad = otra.cantidad;
    otra.cantidad = n;
}

void ColaSolicitudes::encolar(const Solicitud& s) {
    NodoSolicitud* nuevo = new NodoSolicitud;
    nuevo->dato = s;
    nuevo->sig = nullptr;
    if (fin == nullptr)
        frente = nuevo;  // cola vacia: el nodo es frente y fin
    else
        fin->sig = nuevo;
    fin = nuevo;
    ++cantidad;
}

bool ColaSolicitudes::atender(Solicitud& salida) {
    if (frente == nullptr) return false;
    NodoSolicitud* sacar = frente;
    salida = sacar->dato;
    frente = frente->sig;
    if (frente == nullptr) fin = nullptr;  // quedo vacia: no dejar 'fin' colgando
    delete sacar;
    --cantidad;
    return true;
}

bool ColaSolicitudes::verFrente(Solicitud& salida) const {
    if (frente == nullptr) return false;
    salida = frente->dato;
    return true;
}

void ColaSolicitudes::mostrar() const {
    if (frente == nullptr) {
        std::cout << "  (la cola esta vacia)\n";
        return;
    }
    int pos = 1;
    for (const NodoSolicitud* a = frente; a != nullptr; a = a->sig, ++pos)
        std::cout << "  " << pos << ". Pasajero " << a->dato.idPasajero << ": "
                  << a->dato.origen << " -> " << a->dato.destino << "\n";
    std::cout << "  Total en cola: " << cantidad << "\n";
}

int ColaSolicitudes::eliminarDePasajero(int idPasajero) {
    int quitadas = 0;
    NodoSolicitud* previo = nullptr;
    NodoSolicitud* actual = frente;
    while (actual != nullptr) {
        if (actual->dato.idPasajero == idPasajero) {
            NodoSolicitud* borrar = actual;
            actual = actual->sig;
            if (previo == nullptr)
                frente = actual;
            else
                previo->sig = actual;
            if (borrar == fin) fin = previo;  // se borro el ultimo: actualizar 'fin'
            delete borrar;
            --cantidad;
            ++quitadas;
        } else {
            previo = actual;
            actual = actual->sig;
        }
    }
    return quitadas;
}

bool ColaSolicitudes::usaCiudad(const std::string& nombre) const {
    for (const NodoSolicitud* a = frente; a != nullptr; a = a->sig)
        if (util::iguales(a->dato.origen, nombre) || util::iguales(a->dato.destino, nombre))
            return true;
    return false;
}
