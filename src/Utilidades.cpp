#include "Utilidades.h"

#include <cctype>
#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>

namespace util {

static bool entradaAbierta = true;

bool hayEntrada() { return entradaAbierta; }

bool leerLinea(const std::string& prompt, std::string& salida) {
    if (!entradaAbierta) return false;
    std::cout << prompt;
    std::cout.flush();
    if (!std::getline(std::cin, salida)) {
        entradaAbierta = false;
        std::cout << "\n[Fin de la entrada]\n";
        return false;
    }
    return true;
}

std::string recortar(const std::string& s) {
    std::size_t ini = 0;
    std::size_t fin = s.size();
    while (ini < fin && std::isspace(static_cast<unsigned char>(s[ini]))) ++ini;
    while (fin > ini && std::isspace(static_cast<unsigned char>(s[fin - 1]))) --fin;
    return s.substr(ini, fin - ini);
}

std::string minusculas(const std::string& s) {
    std::string r = s;
    for (std::size_t i = 0; i < r.size(); ++i)
        r[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(r[i])));
    return r;
}

bool iguales(const std::string& a, const std::string& b) {
    return minusculas(a) == minusculas(b);
}

bool contiene(const std::string& texto, const std::string& parte) {
    return minusculas(texto).find(minusculas(parte)) != std::string::npos;
}

bool textoValido(const std::string& s) {
    if (s.empty() || s.size() > 40) return false;
    for (std::size_t i = 0; i < s.size(); ++i) {
        unsigned char c = static_cast<unsigned char>(s[i]);
        if (c < 32 || c == '|') return false;
    }
    return true;
}

bool parsearEntero(const std::string& texto, int& salida) {
    std::string s = recortar(texto);
    if (s.empty()) return false;
    errno = 0;
    char* fin = nullptr;
    long v = std::strtol(s.c_str(), &fin, 10);
    if (fin == s.c_str() || *fin != '\0') return false;
    if (errno == ERANGE || v < -2147483647L || v > 2147483647L) return false;
    salida = static_cast<int>(v);
    return true;
}

bool parsearDecimal(const std::string& texto, double& salida) {
    std::string s = recortar(texto);
    if (s.empty()) return false;
    bool hayDigito = false;
    for (std::size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (c == ',') s[i] = c = '.';
        if (std::isdigit(static_cast<unsigned char>(c))) {
            hayDigito = true;
        } else if (c == '.') {
            // permitido
        } else if ((c == '-' || c == '+') && i == 0) {
            // signo inicial permitido
        } else {
            return false;  // rechaza letras, 'inf', 'nan', notacion hexadecimal, etc.
        }
    }
    if (!hayDigito) return false;
    errno = 0;
    char* fin = nullptr;
    double v = std::strtod(s.c_str(), &fin);
    if (fin == s.c_str() || *fin != '\0') return false;
    if (errno == ERANGE || !std::isfinite(v)) return false;
    salida = v;
    return true;
}

int leerOpcion(const std::string& prompt, int minimo, int maximo) {
    std::string linea;
    if (!leerLinea(prompt, linea)) return 0;
    int v = 0;
    if (!parsearEntero(linea, v) || v < minimo || v > maximo) {
        std::cout << "  [!] Opcion invalida. Elija un numero entre " << minimo
                  << " y " << maximo << ".\n";
        return -1;
    }
    return v;
}

bool leerEntero(const std::string& prompt, int minimo, int maximo, int& salida) {
    std::string linea;
    if (!leerLinea(prompt, linea)) return false;
    int v = 0;
    if (!parsearEntero(linea, v)) {
        std::cout << "  [!] Entrada invalida: se esperaba un numero entero.\n";
        return false;
    }
    if (v < minimo || v > maximo) {
        std::cout << "  [!] Valor fuera de rango: debe estar entre " << minimo
                  << " y " << maximo << ".\n";
        return false;
    }
    salida = v;
    return true;
}

bool leerDecimal(const std::string& prompt, double& salida) {
    std::string linea;
    if (!leerLinea(prompt, linea)) return false;
    double v = 0;
    if (!parsearDecimal(linea, v)) {
        std::cout << "  [!] Entrada invalida: se esperaba un numero (ej. 125.5).\n";
        return false;
    }
    salida = v;
    return true;
}

bool leerTexto(const std::string& prompt, std::string& salida) {
    std::string linea;
    if (!leerLinea(prompt, linea)) return false;
    linea = recortar(linea);
    if (!textoValido(linea)) {
        std::cout << "  [!] Texto invalido: no puede estar vacio, ni pasar de 40 "
                     "caracteres, ni contener '|'.\n";
        return false;
    }
    salida = linea;
    return true;
}

int dividir(const std::string& linea, char sep, std::string campos[], int maximo) {
    int n = 0;
    std::string actual;
    for (std::size_t i = 0; i <= linea.size(); ++i) {
        if (i == linea.size() || linea[i] == sep) {
            if (n < maximo) campos[n] = actual;
            ++n;
            actual.clear();
        } else {
            actual += linea[i];
        }
    }
    return n;  // puede ser > maximo: quien llama lo detecta como linea mal formada
}

std::string numeroATexto(double valor) {
    std::ostringstream os;
    os.precision(10);
    os << valor;
    return os.str();
}

}  // namespace util
