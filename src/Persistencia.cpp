#include "Persistencia.h"

#include <fstream>
#include <sstream>

#include "Utilidades.h"

bool guardarSistema(const Sistema& sis, const std::string& archivo, std::string& error) {
    std::ofstream out(archivo.c_str());
    if (!out) {
        error = "no se pudo abrir '" + archivo + "' para escribir";
        return false;
    }
    out << "SITRAC|1\n";

    // Ciudades
    for (const Ciudad* c = sis.grafo.primera(); c != nullptr; c = c->sig)
        out << "C|" << c->nombre << "\n";

    // Rutas (cada una una sola vez)
    for (const Ciudad* c = sis.grafo.primera(); c != nullptr; c = c->sig) {
        for (const Arista* a = c->aristas; a != nullptr; a = a->sig) {
            if (util::minusculas(c->nombre) < util::minusculas(a->destino->nombre))
                out << "R|" << c->nombre << "|" << a->destino->nombre << "|"
                    << util::numeroATexto(a->distancia) << "\n";
        }
    }

    // Pasajeros
    for (const NodoPasajero* p = sis.pasajeros.primero(); p != nullptr; p = p->sig)
        out << "P|" << p->dato.id << "|" << p->dato.nombre << "|" << p->dato.edad << "\n";

    // Cola (frente -> fin)
    for (const NodoSolicitud* s = sis.cola.primero(); s != nullptr; s = s->sig)
        out << "Q|" << s->dato.idPasajero << "|" << s->dato.origen << "|"
            << s->dato.destino << "\n";

    // Historial (tope -> fondo)
    for (const NodoViaje* v = sis.historial.primero(); v != nullptr; v = v->sig)
        out << "H|" << v->dato.idPasajero << "|" << v->dato.nombrePasajero << "|"
            << v->dato.origen << "|" << v->dato.destino << "\n";

    out.flush();
    if (!out) {
        error = "fallo la escritura en '" + archivo + "'";
        return false;
    }
    return true;
}

static std::string errorLinea(int numero, const std::string& motivo) {
    std::ostringstream os;
    os << "linea " << numero << ": " << motivo;
    return os.str();
}

bool cargarSistema(Sistema& sis, const std::string& archivo, std::string& error) {
    std::ifstream in(archivo.c_str());
    if (!in) {
        error = "no se pudo abrir '" + archivo + "' (existe?)";
        return false;
    }

    Sistema temp;  // se llena aparte: si algo falla no se toca 'sis'
    std::string linea;
    int numero = 0;
    bool encabezado = false;
    const int MAXC = 5;
    std::string f[MAXC];

    while (std::getline(in, linea)) {
        ++numero;
        if (!linea.empty() && linea[linea.size() - 1] == '\r')
            linea.erase(linea.size() - 1);  // archivos con fin de linea de Windows
        if (util::recortar(linea).empty()) continue;

        int n = util::dividir(linea, '|', f, MAXC);
        if (n > MAXC) {
            error = errorLinea(numero, "demasiados campos");
            return false;
        }
        if (!encabezado) {
            if (n != 2 || f[0] != "SITRAC" || f[1] != "1") {
                error = errorLinea(numero, "no es un archivo SITRAC valido");
                return false;
            }
            encabezado = true;
            continue;
        }

        if (f[0] == "C" && n == 2) {
            if (!util::textoValido(f[1]) || !temp.grafo.agregarCiudad(f[1])) {
                error = errorLinea(numero, "ciudad invalida o repetida");
                return false;
            }
        } else if (f[0] == "R" && n == 4) {
            double d = 0;
            if (!util::parsearDecimal(f[3], d) ||
                temp.grafo.agregarRuta(f[1], f[2], d) != RUTA_OK) {
                error = errorLinea(numero, "ruta invalida (ciudades, distancia o duplicada)");
                return false;
            }
        } else if (f[0] == "P" && n == 4) {
            Pasajero p;
            if (!util::parsearEntero(f[1], p.id) || p.id <= 0 || !util::textoValido(f[2]) ||
                !util::parsearEntero(f[3], p.edad) || p.edad < 0 || p.edad > 120) {
                error = errorLinea(numero, "datos de pasajero invalidos");
                return false;
            }
            p.nombre = f[2];
            if (!temp.pasajeros.insertar(p)) {
                error = errorLinea(numero, "ID de pasajero duplicado");
                return false;
            }
        } else if (f[0] == "Q" && n == 4) {
            Solicitud s;
            const Ciudad* co = temp.grafo.buscar(f[2]);
            const Ciudad* cd = temp.grafo.buscar(f[3]);
            if (!util::parsearEntero(f[1], s.idPasajero) ||
                temp.pasajeros.buscar(s.idPasajero) == nullptr || co == nullptr ||
                cd == nullptr) {
                error = errorLinea(numero, "solicitud con pasajero o ciudad inexistente");
                return false;
            }
            s.origen = co->nombre;
            s.destino = cd->nombre;
            temp.cola.encolar(s);
        } else if (f[0] == "H" && n == 5) {
            Viaje v;
            if (!util::parsearEntero(f[1], v.idPasajero) || !util::textoValido(f[2]) ||
                !util::textoValido(f[3]) || !util::textoValido(f[4])) {
                error = errorLinea(numero, "registro de historial invalido");
                return false;
            }
            v.nombrePasajero = f[2];
            v.origen = f[3];
            v.destino = f[4];
            temp.historial.agregarAlFondo(v);
        } else {
            error = errorLinea(numero, "registro desconocido o mal formado");
            return false;
        }
    }

    if (!encabezado) {
        error = "el archivo esta vacio";
        return false;
    }

    // Todo valido: intercambiar. El destructor de 'temp' libera los datos viejos.
    sis.pasajeros.intercambiar(temp.pasajeros);
    sis.grafo.intercambiar(temp.grafo);
    sis.cola.intercambiar(temp.cola);
    sis.historial.intercambiar(temp.historial);
    return true;
}
