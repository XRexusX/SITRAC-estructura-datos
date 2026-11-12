// ListaPasajeros.h - Lista simplemente enlazada PROPIA de pasajeros (sin std::list).
#ifndef LISTA_PASAJEROS_H
#define LISTA_PASAJEROS_H

#include <string>

struct Pasajero {
    int id;
    std::string nombre;
    int edad;
};

struct NodoPasajero {
    Pasajero dato;
    NodoPasajero* sig;
};

class ListaPasajeros {
public:
    ListaPasajeros();
    ~ListaPasajeros();
    ListaPasajeros(const ListaPasajeros&) = delete;
    ListaPasajeros& operator=(const ListaPasajeros&) = delete;

    // false si el ID ya existe (no crea ningun nodo).
    bool insertar(const Pasajero& p);
    // nullptr si no existe.
    const Pasajero* buscar(int id) const;
    // false si el ID no existe.
    bool actualizar(int id, const std::string& nombre, int edad);
    // false si el ID no existe (la lista queda intacta).
    bool eliminar(int id);
    void listar() const;
    // Imprime los pasajeros cuyo nombre contiene 'fragmento'. Devuelve cuantos.
    int listarPorNombre(const std::string& fragmento) const;

    int tamano() const { return cantidad; }
    bool estaVacia() const { return cantidad == 0; }
    void vaciar();
    void intercambiar(ListaPasajeros& otra);
    const NodoPasajero* primero() const { return cabeza; }

private:
    NodoPasajero* cabeza;
    int cantidad;
};

#endif
