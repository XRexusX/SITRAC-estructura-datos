// Utilidades.h - Lectura segura de entradas y funciones auxiliares de texto.
// Ninguna lectura puede dejar el programa en un ciclo infinito: si la entrada
// es invalida se informa y la operacion se cancela (el menu sigue vivo).
#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <string>

namespace util {

// false cuando la entrada estandar se cerro (Ctrl+Z / fin de archivo).
bool hayEntrada();

// Lee una linea completa. Devuelve false si no hay mas entrada.
bool leerLinea(const std::string& prompt, std::string& salida);

// Lee una opcion de menu en [minimo, maximo].
// Devuelve -1 si es invalida (ya informo el error) y 0 si se acabo la entrada,
// para que todos los menus terminen limpiamente.
int leerOpcion(const std::string& prompt, int minimo, int maximo);

// Lee un entero en [minimo, maximo]. false si es invalido (ya informo).
bool leerEntero(const std::string& prompt, int minimo, int maximo, int& salida);

// Lee un numero decimal finito (acepta negativos; la regla de dominio la
// valida quien llama). false si no es un numero (ya informo).
bool leerDecimal(const std::string& prompt, double& salida);

// Lee un texto no vacio, sin '|' ni caracteres de control, maximo 40 caracteres.
bool leerTexto(const std::string& prompt, std::string& salida);

// Utilidades de texto.
std::string recortar(const std::string& s);
std::string minusculas(const std::string& s);
bool iguales(const std::string& a, const std::string& b);   // sin distinguir mayusculas
bool contiene(const std::string& texto, const std::string& parte);  // idem
bool textoValido(const std::string& s);

// Conversion estricta (toda la cadena debe ser el numero).
bool parsearEntero(const std::string& s, int& salida);
bool parsearDecimal(const std::string& s, double& salida);

// Divide 'linea' por 'sep' en 'campos' (maximo 'maximo'). Devuelve la cantidad.
int dividir(const std::string& linea, char sep, std::string campos[], int maximo);

// Numero -> texto (sin ceros sobrantes).
std::string numeroATexto(double valor);

}  // namespace util

#endif
