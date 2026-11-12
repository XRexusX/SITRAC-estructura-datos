# Evidencias de ejecución

Compilado con `g++ 16.2.0 (MinGW-w64)`: `g++ -std=c++11 -Wall -Wextra -static -o sitrac.exe src/*.cpp`
→ 0 errores, 0 advertencias.

Ejecución: `sitrac.exe < tests\entrada_pruebas.txt > tests\salida_pruebas.txt`
(la salida completa está en `tests/salida_pruebas.txt`).

## Casos obligatorios de la guía

| Caso | Acción | Resultado obtenido | ¿Pasa? |
|---|---|---|---|
| CP-01 | Registrar `Bogota` dos veces | `[!] La ciudad 'Bogota' ya existe. No se registro de nuevo.` y el listado muestra 3 ciudades (no hay nodo extra) | Sí |
| CP-02 | Eliminar pasajero ID 99 inexistente | `[!] No existe un pasajero con ID 99. No se elimino nada.`; después se inserta y lista normalmente | Sí |
| CP-03 | Atender con la cola vacía (y otra vez tras vaciarla) | `[!] La cola esta vacia: no hay solicitudes por atender.` | Sí |
| CP-04 | Ruta Bogota–Cali con distancia `-5` | `[!] Distancia invalida: debe ser un numero mayor que 0.` | Sí |
| CP-05 | Eliminar `Bogota` (conectada a 2 ciudades) | `[OK] Ciudad eliminada junto con 2 ruta(s) que la conectaban`; luego "Listar rutas" → `(no hay rutas registradas)` y `Medellin no tiene conexiones.` | Sí |
| CP-06 | Letras donde se pide número (`abc` como opción, `hola` como ID, `abc` como distancia) | `[!] Opcion invalida…` / `[!] Entrada invalida: se esperaba un numero entero.` y vuelve al menú | Sí |

## Pruebas adicionales (módulos reales)

- **Encolar** valida pasajero, ciudades y que exista ruta:
  `Solicitud encolada (posicion 2). Ruta: Medellin -> Bogota -> Cali (875 km)`
  (Dijkstra pasa por Bogotá: 415 + 460).
- **Atender** respeta FIFO y alimenta el historial (pila): el historial muestra primero el viaje más reciente.
- **Duplicado de ID** de pasajero: `[!] Ya existe un pasajero con ID 1. No se inserto.`
- **Guardar y cargar** (reiniciando el programa): se restauran ciudades, ruta de `415.5` km, pasajero y la solicitud pendiente; "Ruta mas corta" devuelve `Bogota -> Medellin (415.5 km)`.
- **Archivo dañado** (`ruta` a una ciudad inexistente): `[!] No se pudo cargar (los datos actuales no cambiaron): linea 3: ruta invalida…` y los datos actuales quedan intactos.
- **Archivo inexistente**: `[!] No se pudo cargar … no se pudo abrir 'no_existe.txt'`.
- **Fin de entrada abrupto** (Ctrl+Z / archivo que se acaba): el programa se cierra limpio con `Hasta pronto.`, sin ciclo infinito.

## Entradas inválidas probadas (mínimo 2 exigidas por la guía)

1. Opción de menú no numérica (`abc`) → se informa y se vuelve a mostrar el menú.
2. ID no numérico (`hola`) → operación cancelada, menú intacto.
3. Distancia no numérica (`abc`) y negativa (`-5`) → rechazadas con mensajes distintos.
4. ID duplicado / ciudad duplicada → rechazados sin crear nodos.

> Para la entrega, agreguen además capturas de pantalla propias de la ejecución
> interactiva (la guía pide evidencia de su propia ejecución).
