#include "Menus.h"

#include <iostream>
#include <string>

#include "Persistencia.h"
#include "Utilidades.h"

using std::cout;
using std::string;

namespace {

const char* ARCHIVO_POR_DEFECTO = "sitrac_datos.txt";

// ===================================================================== CIUDADES

void ciudadAgregar(Sistema& sis) {
    string nombre;
    if (!util::leerTexto("  Nombre de la ciudad: ", nombre)) return;
    if (sis.grafo.agregarCiudad(nombre))
        cout << "  [OK] Ciudad '" << nombre << "' registrada.\n";
    else
        cout << "  [!] La ciudad '" << nombre << "' ya existe. No se registro de nuevo.\n";
}

void ciudadEliminar(Sistema& sis) {
    string nombre;
    if (!util::leerTexto("  Ciudad a eliminar: ", nombre)) return;
    if (sis.grafo.buscar(nombre) == nullptr) {
        cout << "  [!] La ciudad '" << nombre << "' no existe.\n";
        return;
    }
    // No se puede dejar una solicitud pendiente apuntando a una ciudad borrada.
    if (sis.cola.usaCiudad(nombre)) {
        cout << "  [!] No se puede eliminar: hay solicitudes en cola que usan esa ciudad. "
                "Atiendalas primero.\n";
        return;
    }
    int rutas = 0;
    sis.grafo.eliminarCiudad(nombre, rutas);
    cout << "  [OK] Ciudad eliminada junto con " << rutas
         << " ruta(s) que la conectaban (sin referencias colgantes).\n";
}

void menuCiudades(Sistema& sis) {
    int op;
    do {
        cout << "\n--- CIUDADES ---\n"
                " 1. Agregar ciudad\n"
                " 2. Eliminar ciudad\n"
                " 3. Listar ciudades\n"
                " 0. Volver\n";
        op = util::leerOpcion("Opcion: ", 0, 3);
        switch (op) {
            case 1: ciudadAgregar(sis); break;
            case 2: ciudadEliminar(sis); break;
            case 3: sis.grafo.listarCiudades(); break;
            default: break;
        }
    } while (op != 0);
}

// ======================================================================= RUTAS

void mensajeRuta(ResultadoRuta r) {
    switch (r) {
        case RUTA_OK: cout << "  [OK] Operacion realizada.\n"; break;
        case RUTA_CIUDAD_INEXISTENTE: cout << "  [!] Alguna de las ciudades no existe.\n"; break;
        case RUTA_MISMA_CIUDAD: cout << "  [!] Una ruta debe unir dos ciudades distintas.\n"; break;
        case RUTA_DISTANCIA_INVALIDA:
            cout << "  [!] Distancia invalida: debe ser un numero mayor que 0.\n";
            break;
        case RUTA_YA_EXISTE: cout << "  [!] Esa ruta ya existe.\n"; break;
        case RUTA_NO_EXISTE: cout << "  [!] Esa ruta no existe.\n"; break;
    }
}

void rutaAgregar(Sistema& sis) {
    string a, b;
    double d = 0;
    if (!util::leerTexto("  Ciudad A: ", a)) return;
    if (!util::leerTexto("  Ciudad B: ", b)) return;
    if (!util::leerDecimal("  Distancia (km): ", d)) return;
    mensajeRuta(sis.grafo.agregarRuta(a, b, d));
}

void rutaEliminar(Sistema& sis) {
    string a, b;
    if (!util::leerTexto("  Ciudad A: ", a)) return;
    if (!util::leerTexto("  Ciudad B: ", b)) return;
    mensajeRuta(sis.grafo.eliminarRuta(a, b));
}

void rutaVecinos(Sistema& sis) {
    string nombre;
    if (!util::leerTexto("  Ciudad: ", nombre)) return;
    if (!sis.grafo.listarVecinos(nombre))
        cout << "  [!] La ciudad '" << nombre << "' no existe.\n";
}

void menuRutas(Sistema& sis) {
    int op;
    do {
        cout << "\n--- RUTAS ---\n"
                " 1. Agregar ruta\n"
                " 2. Eliminar ruta\n"
                " 3. Consultar vecinos de una ciudad\n"
                " 4. Listar todas las rutas\n"
                " 0. Volver\n";
        op = util::leerOpcion("Opcion: ", 0, 4);
        switch (op) {
            case 1: rutaAgregar(sis); break;
            case 2: rutaEliminar(sis); break;
            case 3: rutaVecinos(sis); break;
            case 4: sis.grafo.listarRutas(); break;
            default: break;
        }
    } while (op != 0);
}

// ==================================================================== PASAJEROS

bool leerDatosPasajero(string& nombre, int& edad) {
    if (!util::leerTexto("  Nombre: ", nombre)) return false;
    return util::leerEntero("  Edad (0-120): ", 0, 120, edad);
}

void pasajeroInsertar(Sistema& sis) {
    Pasajero p;
    if (!util::leerEntero("  ID (entero positivo): ", 1, 2147483647, p.id)) return;
    if (sis.pasajeros.buscar(p.id) != nullptr) {  // evita pedir mas datos
        cout << "  [!] Ya existe un pasajero con ID " << p.id << ". No se inserto.\n";
        return;
    }
    if (!leerDatosPasajero(p.nombre, p.edad)) return;
    if (sis.pasajeros.insertar(p))
        cout << "  [OK] Pasajero " << p.id << " registrado.\n";
    else
        cout << "  [!] ID duplicado. No se inserto.\n";
}

void pasajeroBuscar(Sistema& sis) {
    int id;
    if (!util::leerEntero("  ID a buscar: ", 1, 2147483647, id)) return;
    const Pasajero* p = sis.pasajeros.buscar(id);
    if (p == nullptr)
        cout << "  [!] No existe un pasajero con ID " << id << ".\n";
    else
        cout << "  ID " << p->id << " | " << p->nombre << " | " << p->edad << " anios\n";
}

void pasajeroActualizar(Sistema& sis) {
    int id;
    if (!util::leerEntero("  ID a actualizar: ", 1, 2147483647, id)) return;
    if (sis.pasajeros.buscar(id) == nullptr) {
        cout << "  [!] No existe un pasajero con ID " << id << ".\n";
        return;
    }
    string nombre;
    int edad;
    if (!leerDatosPasajero(nombre, edad)) return;
    sis.pasajeros.actualizar(id, nombre, edad);
    cout << "  [OK] Pasajero " << id << " actualizado.\n";
}

void pasajeroEliminar(Sistema& sis) {
    int id;
    if (!util::leerEntero("  ID a eliminar: ", 1, 2147483647, id)) return;
    if (!sis.pasajeros.eliminar(id)) {
        cout << "  [!] No existe un pasajero con ID " << id << ". No se elimino nada.\n";
        return;
    }
    // Sus solicitudes pendientes quedarian apuntando a un pasajero inexistente.
    int quitadas = sis.cola.eliminarDePasajero(id);
    cout << "  [OK] Pasajero " << id << " eliminado.";
    if (quitadas > 0) cout << " Tambien se retiraron " << quitadas << " solicitud(es) de la cola.";
    cout << "\n";
}

void menuPasajeros(Sistema& sis) {
    int op;
    do {
        cout << "\n--- PASAJEROS ---\n"
                " 1. Insertar pasajero\n"
                " 2. Buscar por ID\n"
                " 3. Actualizar pasajero\n"
                " 4. Eliminar pasajero\n"
                " 5. Listar pasajeros\n"
                " 0. Volver\n";
        op = util::leerOpcion("Opcion: ", 0, 5);
        switch (op) {
            case 1: pasajeroInsertar(sis); break;
            case 2: pasajeroBuscar(sis); break;
            case 3: pasajeroActualizar(sis); break;
            case 4: pasajeroEliminar(sis); break;
            case 5: sis.pasajeros.listar(); break;
            default: break;
        }
    } while (op != 0);
}

// ======================================================================== COLA

void colaEncolar(Sistema& sis) {
    int id;
    string origen, destino;
    if (!util::leerEntero("  ID del pasajero: ", 1, 2147483647, id)) return;
    if (sis.pasajeros.buscar(id) == nullptr) {
        cout << "  [!] No existe un pasajero con ID " << id << ".\n";
        return;
    }
    if (!util::leerTexto("  Ciudad de origen: ", origen)) return;
    if (!util::leerTexto("  Ciudad de destino: ", destino)) return;
    const Ciudad* co = sis.grafo.buscar(origen);
    const Ciudad* cd = sis.grafo.buscar(destino);
    if (co == nullptr || cd == nullptr) {
        cout << "  [!] Origen o destino no existen.\n";
        return;
    }
    if (co == cd) {
        cout << "  [!] El origen y el destino deben ser distintos.\n";
        return;
    }
    double km = 0;
    string camino;
    if (!sis.grafo.rutaMasCorta(co->nombre, cd->nombre, km, camino)) {
        cout << "  [!] No existe ruta entre " << co->nombre << " y " << cd->nombre
             << ". Solicitud rechazada.\n";
        return;
    }
    Solicitud s;
    s.idPasajero = id;
    s.origen = co->nombre;
    s.destino = cd->nombre;
    sis.cola.encolar(s);
    cout << "  [OK] Solicitud encolada (posicion " << sis.cola.tamano() << "). Ruta: " << camino
         << " (" << util::numeroATexto(km) << " km)\n";
}

void colaAtender(Sistema& sis) {
    Solicitud s;
    if (!sis.cola.atender(s)) {
        cout << "  [!] La cola esta vacia: no hay solicitudes por atender.\n";
        return;
    }
    Viaje v;
    v.idPasajero = s.idPasajero;
    const Pasajero* p = sis.pasajeros.buscar(s.idPasajero);
    v.nombrePasajero = (p != nullptr) ? p->nombre : string("(desconocido)");
    v.origen = s.origen;
    v.destino = s.destino;
    sis.historial.apilar(v);
    cout << "  [OK] Atendida: " << v.nombrePasajero << " (ID " << v.idPasajero << "), "
         << v.origen << " -> " << v.destino << ". Registrada en el historial.\n";
}

void colaFrente(Sistema& sis) {
    Solicitud s;
    if (!sis.cola.verFrente(s)) {
        cout << "  [!] La cola esta vacia.\n";
        return;
    }
    cout << "  Frente: pasajero " << s.idPasajero << ", " << s.origen << " -> " << s.destino
         << "\n";
}

void menuCola(Sistema& sis) {
    int op;
    do {
        cout << "\n--- COLA DE SOLICITUDES ---\n"
                " 1. Encolar solicitud de viaje\n"
                " 2. Atender solicitud (frente)\n"
                " 3. Mostrar frente\n"
                " 4. Mostrar toda la cola\n"
                " 0. Volver\n";
        op = util::leerOpcion("Opcion: ", 0, 4);
        switch (op) {
            case 1: colaEncolar(sis); break;
            case 2: colaAtender(sis); break;
            case 3: colaFrente(sis); break;
            case 4: sis.cola.mostrar(); break;
            default: break;
        }
    } while (op != 0);
}

// ================================================================== HISTORIAL

void menuHistorial(Sistema& sis) {
    int op;
    do {
        cout << "\n--- HISTORIAL DE VIAJES ATENDIDOS ---\n"
                " 1. Ver historial (mas reciente primero)\n"
                " 2. Ver ultimo viaje atendido\n"
                " 3. Borrar historial\n"
                " 0. Volver\n";
        op = util::leerOpcion("Opcion: ", 0, 3);
        switch (op) {
            case 1: sis.historial.mostrar(); break;
            case 2: {
                Viaje v;
                if (sis.historial.verTope(v))
                    cout << "  Ultimo: " << v.nombrePasajero << " (ID " << v.idPasajero << "), "
                         << v.origen << " -> " << v.destino << "\n";
                else
                    cout << "  [!] El historial esta vacio.\n";
                break;
            }
            case 3:
                sis.historial.vaciar();
                cout << "  [OK] Historial borrado.\n";
                break;
            default: break;
        }
    } while (op != 0);
}

// =================================================================== BUSQUEDAS

void buscarPorNombre(Sistema& sis) {
    string frag;
    if (!util::leerTexto("  Texto a buscar en el nombre: ", frag)) return;
    if (sis.pasajeros.listarPorNombre(frag) == 0)
        cout << "  [!] Ningun pasajero coincide con '" << frag << "'.\n";
}

void buscarRutaCorta(Sistema& sis) {
    string a, b;
    if (!util::leerTexto("  Ciudad de origen: ", a)) return;
    if (!util::leerTexto("  Ciudad de destino: ", b)) return;
    if (sis.grafo.buscar(a) == nullptr || sis.grafo.buscar(b) == nullptr) {
        cout << "  [!] Origen o destino no existen.\n";
        return;
    }
    double km = 0;
    string camino;
    if (sis.grafo.rutaMasCorta(a, b, km, camino))
        cout << "  Ruta mas corta: " << camino << " (" << util::numeroATexto(km) << " km)\n";
    else
        cout << "  [!] No existe ruta entre esas ciudades.\n";
}

void menuBusquedas(Sistema& sis) {
    int op;
    do {
        cout << "\n--- BUSQUEDAS ---\n"
                " 1. Pasajero por ID\n"
                " 2. Pasajero por nombre (contiene)\n"
                " 3. Ruta mas corta entre dos ciudades (Dijkstra)\n"
                " 0. Volver\n";
        op = util::leerOpcion("Opcion: ", 0, 3);
        switch (op) {
            case 1: pasajeroBuscar(sis); break;
            case 2: buscarPorNombre(sis); break;
            case 3: buscarRutaCorta(sis); break;
            default: break;
        }
    } while (op != 0);
}

// =================================================================== REPORTES

void reporteResumen(Sistema& sis) {
    cout << "  Ciudades registradas : " << sis.grafo.cantidadCiudades() << "\n"
         << "  Rutas registradas    : " << sis.grafo.cantidadRutas() << " ("
         << util::numeroATexto(sis.grafo.kilometrosTotales()) << " km en total)\n"
         << "  Pasajeros            : " << sis.pasajeros.tamano() << "\n"
         << "  Solicitudes en cola  : " << sis.cola.tamano() << "\n"
         << "  Viajes atendidos     : " << sis.historial.tamano() << "\n";

    const Ciudad* mejor = nullptr;
    int gMejor = 0;
    int aisladas = 0;
    for (const Ciudad* c = sis.grafo.primera(); c != nullptr; c = c->sig) {
        int g = Grafo::grado(c);
        if (g == 0) ++aisladas;
        if (mejor == nullptr || g > gMejor) {
            mejor = c;
            gMejor = g;
        }
    }
    if (mejor != nullptr)
        cout << "  Ciudad mas conectada : " << mejor->nombre << " (" << gMejor
             << " conexiones)\n"
             << "  Ciudades aisladas    : " << aisladas << "\n";

    if (sis.pasajeros.tamano() > 0) {
        long suma = 0;
        for (const NodoPasajero* p = sis.pasajeros.primero(); p != nullptr; p = p->sig)
            suma += p->dato.edad;
        cout << "  Edad promedio        : "
             << util::numeroATexto(static_cast<double>(suma) / sis.pasajeros.tamano())
             << " anios\n";
    }
}

void menuReportes(Sistema& sis) {
    int op;
    do {
        cout << "\n--- REPORTES ---\n"
                " 1. Resumen general\n"
                " 2. Todas las rutas\n"
                " 3. Pasajeros registrados\n"
                " 0. Volver\n";
        op = util::leerOpcion("Opcion: ", 0, 3);
        switch (op) {
            case 1: reporteResumen(sis); break;
            case 2: sis.grafo.listarRutas(); break;
            case 3: sis.pasajeros.listar(); break;
            default: break;
        }
    } while (op != 0);
}

// ==================================================================== ARCHIVOS

bool pedirNombreArchivo(string& archivo) {
    string linea;
    if (!util::leerLinea(string("  Archivo [Enter = ") + ARCHIVO_POR_DEFECTO + "]: ", linea))
        return false;
    linea = util::recortar(linea);
    archivo = linea.empty() ? string(ARCHIVO_POR_DEFECTO) : linea;
    return true;
}

void menuArchivos(Sistema& sis) {
    int op;
    do {
        cout << "\n--- GUARDAR / CARGAR ---\n"
                " 1. Guardar datos\n"
                " 2. Cargar datos (reemplaza los actuales)\n"
                " 0. Volver\n";
        op = util::leerOpcion("Opcion: ", 0, 2);
        if (op != 1 && op != 2) continue;
        string archivo, error;
        if (!pedirNombreArchivo(archivo)) return;
        if (op == 1) {
            if (guardarSistema(sis, archivo, error))
                cout << "  [OK] Datos guardados en '" << archivo << "'.\n";
            else
                cout << "  [!] No se pudo guardar: " << error << "\n";
        } else {
            if (cargarSistema(sis, archivo, error))
                cout << "  [OK] Datos cargados desde '" << archivo << "'.\n";
            else
                cout << "  [!] No se pudo cargar (los datos actuales no cambiaron): " << error
                     << "\n";
        }
    } while (op != 0);
}

}  // namespace

// ================================================================== PRINCIPAL

void menuPrincipal(Sistema& sis) {
    int op;
    do {
        cout << "\n=====================================================\n"
                "  SITRAC - Transporte y Gestion de Rutas\n"
                "=====================================================\n"
                " 1. Ciudades\n"
                " 2. Rutas\n"
                " 3. Pasajeros\n"
                " 4. Cola de solicitudes\n"
                " 5. Historial\n"
                " 6. Busquedas\n"
                " 7. Reportes\n"
                " 8. Guardar / cargar\n"
                " 0. Salir\n";
        op = util::leerOpcion("Opcion: ", 0, 8);
        switch (op) {
            case 1: menuCiudades(sis); break;
            case 2: menuRutas(sis); break;
            case 3: menuPasajeros(sis); break;
            case 4: menuCola(sis); break;
            case 5: menuHistorial(sis); break;
            case 6: menuBusquedas(sis); break;
            case 7: menuReportes(sis); break;
            case 8: menuArchivos(sis); break;
            default: break;
        }
    } while (op != 0);
    cout << "\nHasta pronto.\n";
}
