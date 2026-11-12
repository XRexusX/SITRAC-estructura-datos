// Historial.h - Pila enlazada PROPIA con los viajes ya atendidos.
// El tope es el viaje mas reciente (LIFO). Guarda una copia del nombre del
// pasajero para que el historial siga siendo legible aunque luego se elimine.
#ifndef HISTORIAL_H
#define HISTORIAL_H

#include <string>

struct Viaje {
    int idPasajero;
    std::string nombrePasajero;
    std::string origen;
    std::string destino;
};

struct NodoViaje {
    Viaje dato;
    NodoViaje* sig;
};

class Historial {
public:
    Historial();
    ~Historial();
    Historial(const Historial&) = delete;
    Historial& operator=(const Historial&) = delete;

    void apilar(const Viaje& v);                 // nuevo tope
    void agregarAlFondo(const Viaje& v);         // usado al cargar desde archivo
    bool verTope(Viaje& salida) const;           // false si esta vacio
    void mostrar() const;                        // del mas reciente al mas antiguo

    int tamano() const { return cantidad; }
    bool estaVacio() const { return tope == nullptr; }
    void vaciar();
    void intercambiar(Historial& otro);
    const NodoViaje* primero() const { return tope; }

private:
    NodoViaje* tope;
    int cantidad;
};

#endif
