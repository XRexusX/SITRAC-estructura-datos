// Persistencia.h - Guardar y cargar el sistema en un archivo de texto.
//
// Formato (una entrada por linea, campos separados por '|'):
//   SITRAC|1
//   C|nombreCiudad
//   R|ciudadA|ciudadB|distancia
//   P|id|nombre|edad
//   Q|idPasajero|origen|destino        (de frente a fin)
//   H|idPasajero|nombre|origen|destino (de mas reciente a mas antiguo)
#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include <string>

#include "Sistema.h"

// true si se guardo. En caso contrario 'error' explica el motivo.
bool guardarSistema(const Sistema& sis, const std::string& archivo, std::string& error);

// Carga primero en un Sistema temporal y valida cada linea; solo si TODO es
// correcto reemplaza los datos actuales. Si algo falla, 'sis' queda intacto.
bool cargarSistema(Sistema& sis, const std::string& archivo, std::string& error);

#endif
