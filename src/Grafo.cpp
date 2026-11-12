#include "Grafo.h"

#include <cmath>
#include <iostream>

#include "Utilidades.h"

Grafo::Grafo() : cabeza(nullptr), cantidad(0) {}

Grafo::~Grafo() { vaciar(); }

void Grafo::vaciar() {
    while (cabeza != nullptr) {
        Ciudad* borrar = cabeza;
        cabeza = cabeza->sig;
        while (borrar->aristas != nullptr) {
            Arista* a = borrar->aristas;
            borrar->aristas = a->sig;
            delete a;
        }
        delete borrar;
    }
    cantidad = 0;
}

void Grafo::intercambiar(Grafo& otro) {
    Ciudad* c = cabeza;
    cabeza = otro.cabeza;
    otro.cabeza = c;
    int n = cantidad;
    cantidad = otro.cantidad;
    otro.cantidad = n;
}

// ---------------------------------------------------------------- auxiliares

Ciudad* Grafo::buscarMutable(const std::string& nombre) const {
    for (Ciudad* c = cabeza; c != nullptr; c = c->sig)
        if (util::iguales(c->nombre, nombre)) return c;
    return nullptr;
}

const Ciudad* Grafo::buscar(const std::string& nombre) const {
    return buscarMutable(nombre);
}

Arista* Grafo::buscarArista(const Ciudad* desde, const Ciudad* hacia) {
    for (Arista* a = desde->aristas; a != nullptr; a = a->sig)
        if (a->destino == hacia) return a;
    return nullptr;
}

void Grafo::insertarArista(Ciudad* desde, Ciudad* hacia, double distancia) {
    Arista* nueva = new Arista;
    nueva->destino = hacia;
    nueva->distancia = distancia;
    nueva->sig = desde->aristas;  // insercion al inicio: O(1)
    desde->aristas = nueva;
}

bool Grafo::quitarArista(Ciudad* desde, const Ciudad* hacia) {
    Arista* previa = nullptr;
    Arista* actual = desde->aristas;
    while (actual != nullptr && actual->destino != hacia) {
        previa = actual;
        actual = actual->sig;
    }
    if (actual == nullptr) return false;
    if (previa == nullptr)
        desde->aristas = actual->sig;
    else
        previa->sig = actual->sig;
    delete actual;
    return true;
}

int Grafo::grado(const Ciudad* c) {
    int g = 0;
    for (const Arista* a = c->aristas; a != nullptr; a = a->sig) ++g;
    return g;
}

// ------------------------------------------------------------------ ciudades

bool Grafo::agregarCiudad(const std::string& nombre) {
    Ciudad* ultima = nullptr;
    for (Ciudad* c = cabeza; c != nullptr; c = c->sig) {
        if (util::iguales(c->nombre, nombre)) return false;  // duplicada
        ultima = c;
    }
    Ciudad* nueva = new Ciudad;
    nueva->nombre = nombre;
    nueva->aristas = nullptr;
    nueva->sig = nullptr;
    if (ultima == nullptr)
        cabeza = nueva;
    else
        ultima->sig = nueva;
    ++cantidad;
    return true;
}

bool Grafo::eliminarCiudad(const std::string& nombre, int& rutasEliminadas) {
    rutasEliminadas = 0;
    Ciudad* previa = nullptr;
    Ciudad* objetivo = cabeza;
    while (objetivo != nullptr && !util::iguales(objetivo->nombre, nombre)) {
        previa = objetivo;
        objetivo = objetivo->sig;
    }
    if (objetivo == nullptr) return false;

    // 1) Quitar toda arista de las demas ciudades que apunte a 'objetivo'.
    for (Ciudad* c = cabeza; c != nullptr; c = c->sig) {
        if (c != objetivo && quitarArista(c, objetivo)) ++rutasEliminadas;
    }
    // 2) Liberar las aristas propias de 'objetivo'.
    while (objetivo->aristas != nullptr) {
        Arista* a = objetivo->aristas;
        objetivo->aristas = a->sig;
        delete a;
    }
    // 3) Desenlazar y liberar el nodo.
    if (previa == nullptr)
        cabeza = objetivo->sig;
    else
        previa->sig = objetivo->sig;
    delete objetivo;
    --cantidad;
    return true;
}

void Grafo::listarCiudades() const {
    if (cabeza == nullptr) {
        std::cout << "  (no hay ciudades registradas)\n";
        return;
    }
    for (const Ciudad* c = cabeza; c != nullptr; c = c->sig)
        std::cout << "  - " << c->nombre << " (" << grado(c) << " conexiones)\n";
    std::cout << "  Total: " << cantidad << "\n";
}

// -------------------------------------------------------------------- rutas

ResultadoRuta Grafo::agregarRuta(const std::string& a, const std::string& b,
                                 double distancia) {
    Ciudad* ca = buscarMutable(a);
    Ciudad* cb = buscarMutable(b);
    if (ca == nullptr || cb == nullptr) return RUTA_CIUDAD_INEXISTENTE;
    if (ca == cb) return RUTA_MISMA_CIUDAD;
    if (!std::isfinite(distancia) || distancia <= 0) return RUTA_DISTANCIA_INVALIDA;
    if (buscarArista(ca, cb) != nullptr) return RUTA_YA_EXISTE;
    insertarArista(ca, cb, distancia);
    insertarArista(cb, ca, distancia);
    return RUTA_OK;
}

ResultadoRuta Grafo::eliminarRuta(const std::string& a, const std::string& b) {
    Ciudad* ca = buscarMutable(a);
    Ciudad* cb = buscarMutable(b);
    if (ca == nullptr || cb == nullptr) return RUTA_CIUDAD_INEXISTENTE;
    if (!quitarArista(ca, cb)) return RUTA_NO_EXISTE;
    quitarArista(cb, ca);
    return RUTA_OK;
}

bool Grafo::listarVecinos(const std::string& nombre) const {
    const Ciudad* c = buscar(nombre);
    if (c == nullptr) return false;
    if (c->aristas == nullptr) {
        std::cout << "  " << c->nombre << " no tiene conexiones.\n";
        return true;
    }
    std::cout << "  Vecinos de " << c->nombre << ":\n";
    for (const Arista* a = c->aristas; a != nullptr; a = a->sig)
        std::cout << "    -> " << a->destino->nombre << " ("
                  << util::numeroATexto(a->distancia) << " km)\n";
    return true;
}

void Grafo::listarRutas() const {
    int mostradas = 0;
    for (const Ciudad* c = cabeza; c != nullptr; c = c->sig) {
        for (const Arista* a = c->aristas; a != nullptr; a = a->sig) {
            // Cada ruta esta guardada dos veces: se imprime solo en un sentido.
            if (util::minusculas(c->nombre) < util::minusculas(a->destino->nombre)) {
                std::cout << "  " << c->nombre << " <-> " << a->destino->nombre << " : "
                          << util::numeroATexto(a->distancia) << " km\n";
                ++mostradas;
            }
        }
    }
    if (mostradas == 0) std::cout << "  (no hay rutas registradas)\n";
}

int Grafo::cantidadRutas() const {
    int suma = 0;
    for (const Ciudad* c = cabeza; c != nullptr; c = c->sig) suma += grado(c);
    return suma / 2;
}

double Grafo::kilometrosTotales() const {
    double suma = 0;
    for (const Ciudad* c = cabeza; c != nullptr; c = c->sig)
        for (const Arista* a = c->aristas; a != nullptr; a = a->sig) suma += a->distancia;
    return suma / 2;
}

// ----------------------------------------------------------------- Dijkstra

bool Grafo::rutaMasCorta(const std::string& origen, const std::string& destino,
                         double& total, std::string& camino) const {
    const Ciudad* co = buscar(origen);
    const Ciudad* cd = buscar(destino);
    if (co == nullptr || cd == nullptr) return false;

    const int N = cantidad;
    const Ciudad** nodos = new const Ciudad*[N];
    double* dist = new double[N];
    int* previo = new int[N];
    bool* listo = new bool[N];
    const double INF = 1e300;

    int iOrigen = 0, iDestino = 0, k = 0;
    for (const Ciudad* c = cabeza; c != nullptr; c = c->sig, ++k) {
        nodos[k] = c;
        dist[k] = INF;
        previo[k] = -1;
        listo[k] = false;
        if (c == co) iOrigen = k;
        if (c == cd) iDestino = k;
    }
    dist[iOrigen] = 0;

    for (int paso = 0; paso < N; ++paso) {
        int u = -1;
        for (int i = 0; i < N; ++i)
            if (!listo[i] && dist[i] < INF && (u == -1 || dist[i] < dist[u])) u = i;
        if (u == -1) break;  // lo que queda es inalcanzable
        listo[u] = true;
        for (const Arista* a = nodos[u]->aristas; a != nullptr; a = a->sig) {
            int v = 0;
            while (nodos[v] != a->destino) ++v;
            if (!listo[v] && dist[u] + a->distancia < dist[v]) {
                dist[v] = dist[u] + a->distancia;
                previo[v] = u;
            }
        }
    }

    bool hayCamino = dist[iDestino] < INF;
    if (hayCamino) {
        total = dist[iDestino];
        int* pila = new int[N];
        int tope = 0;
        for (int x = iDestino; x != -1; x = previo[x]) pila[tope++] = x;
        camino.clear();
        for (int i = tope - 1; i >= 0; --i) {
            camino += nodos[pila[i]]->nombre;
            if (i > 0) camino += " -> ";
        }
        delete[] pila;
    }
    delete[] nodos;
    delete[] dist;
    delete[] previo;
    delete[] listo;
    return hayCamino;
}
