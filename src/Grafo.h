// Grafo.h - Grafo NO dirigido y ponderado con LISTA DE ADYACENCIA propia.
// Cada ciudad es un nodo de una lista enlazada; cada ciudad tiene su propia
// lista enlazada de aristas (vecinos + distancia en km).
// Una ruta A-B se guarda dos veces (A->B y B->A) para poder consultar vecinos
// en O(grado) desde cualquier extremo.
#ifndef GRAFO_H
#define GRAFO_H

#include <string>

struct Ciudad;

struct Arista {
    Ciudad* destino;
    double distancia;
    Arista* sig;
};

struct Ciudad {
    std::string nombre;
    Arista* aristas;
    Ciudad* sig;
};

enum ResultadoRuta {
    RUTA_OK,
    RUTA_CIUDAD_INEXISTENTE,
    RUTA_MISMA_CIUDAD,
    RUTA_DISTANCIA_INVALIDA,
    RUTA_YA_EXISTE,
    RUTA_NO_EXISTE
};

class Grafo {
public:
    Grafo();
    ~Grafo();
    Grafo(const Grafo&) = delete;
    Grafo& operator=(const Grafo&) = delete;

    // ---- Ciudades ----
    // false si ya existe (sin distinguir mayusculas): no se crea ningun nodo.
    bool agregarCiudad(const std::string& nombre);
    // Elimina la ciudad y TODAS las aristas que la apuntan (sin referencias
    // colgantes). 'rutasEliminadas' = cuantas rutas se quitaron. false si no existe.
    bool eliminarCiudad(const std::string& nombre, int& rutasEliminadas);
    const Ciudad* buscar(const std::string& nombre) const;
    void listarCiudades() const;

    // ---- Rutas ----
    ResultadoRuta agregarRuta(const std::string& a, const std::string& b, double distancia);
    ResultadoRuta eliminarRuta(const std::string& a, const std::string& b);
    // false si la ciudad no existe.
    bool listarVecinos(const std::string& nombre) const;
    void listarRutas() const;

    // ---- Consultas ----
    // Dijkstra O(V^2). false si alguna ciudad no existe o no hay camino.
    bool rutaMasCorta(const std::string& origen, const std::string& destino,
                      double& total, std::string& camino) const;

    int cantidadCiudades() const { return cantidad; }
    int cantidadRutas() const;
    double kilometrosTotales() const;
    static int grado(const Ciudad* c);

    void vaciar();
    void intercambiar(Grafo& otro);
    const Ciudad* primera() const { return cabeza; }

private:
    Ciudad* buscarMutable(const std::string& nombre) const;
    static Arista* buscarArista(const Ciudad* desde, const Ciudad* hacia);
    static void insertarArista(Ciudad* desde, Ciudad* hacia, double distancia);
    static bool quitarArista(Ciudad* desde, const Ciudad* hacia);

    Ciudad* cabeza;
    int cantidad;
};

#endif
