// ColaSolicitudes.h - Cola FIFO enlazada PROPIA (sin std::queue) con punteros
// a frente y fin: encolar y atender son O(1).
#ifndef COLA_SOLICITUDES_H
#define COLA_SOLICITUDES_H

#include <string>

struct Solicitud {
    int idPasajero;
    std::string origen;
    std::string destino;
};

struct NodoSolicitud {
    Solicitud dato;
    NodoSolicitud* sig;
};

class ColaSolicitudes {
public:
    ColaSolicitudes();
    ~ColaSolicitudes();
    ColaSolicitudes(const ColaSolicitudes&) = delete;
    ColaSolicitudes& operator=(const ColaSolicitudes&) = delete;

    void encolar(const Solicitud& s);
    // Saca el frente. false si la cola esta vacia (subdesbordamiento controlado).
    bool atender(Solicitud& salida);
    // Copia el frente sin sacarlo. false si esta vacia.
    bool verFrente(Solicitud& salida) const;
    void mostrar() const;

    // Elimina todas las solicitudes de un pasajero. Devuelve cuantas quito.
    int eliminarDePasajero(int idPasajero);
    // true si alguna solicitud usa esa ciudad como origen o destino.
    bool usaCiudad(const std::string& nombre) const;

    int tamano() const { return cantidad; }
    bool estaVacia() const { return frente == nullptr; }
    void vaciar();
    void intercambiar(ColaSolicitudes& otra);
    const NodoSolicitud* primero() const { return frente; }

private:
    NodoSolicitud* frente;
    NodoSolicitud* fin;
    int cantidad;
};

#endif
