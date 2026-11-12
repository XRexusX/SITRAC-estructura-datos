#include "ListaPasajeros.h"

#include <iostream>

#include "Utilidades.h"

ListaPasajeros::ListaPasajeros() : cabeza(nullptr), cantidad(0) {}

ListaPasajeros::~ListaPasajeros() { vaciar(); }

void ListaPasajeros::vaciar() {
    while (cabeza != nullptr) {
        NodoPasajero* borrar = cabeza;
        cabeza = cabeza->sig;
        delete borrar;
    }
    cantidad = 0;
}

void ListaPasajeros::intercambiar(ListaPasajeros& otra) {
    NodoPasajero* c = cabeza;
    cabeza = otra.cabeza;
    otra.cabeza = c;
    int n = cantidad;
    cantidad = otra.cantidad;
    otra.cantidad = n;
}

bool ListaPasajeros::insertar(const Pasajero& p) {
    // Un solo recorrido: detecta duplicados y deja 'ultimo' apuntando al final.
    NodoPasajero* ultimo = nullptr;
    for (NodoPasajero* a = cabeza; a != nullptr; a = a->sig) {
        if (a->dato.id == p.id) return false;  // ID duplicado: no se crea nodo
        ultimo = a;
    }
    NodoPasajero* nuevo = new NodoPasajero;
    nuevo->dato = p;
    nuevo->sig = nullptr;
    if (ultimo == nullptr)
        cabeza = nuevo;
    else
        ultimo->sig = nuevo;
    ++cantidad;
    return true;
}

const Pasajero* ListaPasajeros::buscar(int id) const {
    for (const NodoPasajero* a = cabeza; a != nullptr; a = a->sig)
        if (a->dato.id == id) return &a->dato;
    return nullptr;
}

bool ListaPasajeros::actualizar(int id, const std::string& nombre, int edad) {
    for (NodoPasajero* a = cabeza; a != nullptr; a = a->sig) {
        if (a->dato.id == id) {
            a->dato.nombre = nombre;
            a->dato.edad = edad;
            return true;
        }
    }
    return false;
}

bool ListaPasajeros::eliminar(int id) {
    NodoPasajero* previo = nullptr;
    NodoPasajero* actual = cabeza;
    while (actual != nullptr && actual->dato.id != id) {
        previo = actual;
        actual = actual->sig;
    }
    if (actual == nullptr) return false;  // no existe: la lista no se toca
    if (previo == nullptr)
        cabeza = actual->sig;  // era la cabeza
    else
        previo->sig = actual->sig;
    delete actual;
    --cantidad;
    return true;
}

void ListaPasajeros::listar() const {
    if (cabeza == nullptr) {
        std::cout << "  (no hay pasajeros registrados)\n";
        return;
    }
    for (const NodoPasajero* a = cabeza; a != nullptr; a = a->sig)
        std::cout << "  ID " << a->dato.id << " | " << a->dato.nombre << " | "
                  << a->dato.edad << " anios\n";
    std::cout << "  Total: " << cantidad << "\n";
}

int ListaPasajeros::listarPorNombre(const std::string& fragmento) const {
    int encontrados = 0;
    for (const NodoPasajero* a = cabeza; a != nullptr; a = a->sig) {
        if (util::contiene(a->dato.nombre, fragmento)) {
            std::cout << "  ID " << a->dato.id << " | " << a->dato.nombre << " | "
                      << a->dato.edad << " anios\n";
            ++encontrados;
        }
    }
    return encontrados;
}
