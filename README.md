# Juego de la Vida — Programación I

Tablero: matriz bidimensional de `int` con 0 (muerta) y 1 (viva).
La interfaz gráfica X11, el controlador, la lectura de archivos y el Makefile vienen resueltos.
**Solo debe implementarse `model/model.c`: `countNeighbors` y `nextGeneration`.**

## Preparación en Ubuntu

```sh
sudo apt install build-essential libx11-dev
make
./life
```

En sistemas con soporte X11 o XWayland, se abre una ventana gráfica.

## Controles

- `Espacio` o `P`: pausar/reanudar
- `N` o flecha derecha: avanzar una generación y pausar
- `R`: reiniciar el patrón original
- `+` / `-`: aumentar/reducir velocidad (1–30 generaciones/s)
- `Q` o `Esc`: salir

La cabecera muestra generación, número de células vivas, estado y velocidad.
Las células que acaban de nacer se dibujan en ámbar; las demás vivas, en blanco.

## Probar patrones

```sh
./life -f patterns/block_stable.txt --fps 4
./life -f patterns/blinker_horizontal.txt --fps 3
./life -f patterns/glider_large.txt --fps 8
./life -f patterns/r_pentomino.txt --fps 10
./life -f patterns/gosper_glider_gun.txt --fps 12
make test
```

`--steps N` limita las generaciones; al llegar a N, queda en pausa y se puede reiniciar.

## Escenarios de partida

| Archivo | Qué observar |
| --- | --- |
| `empty_8x12.txt` | todas las células muertas (estado fijo) |
| `single_cell.txt` | aislamiento: muere en la siguiente generación |
| `block_stable.txt` | bloque 2x2 estable |
| `beehive_stable.txt` | colmena estable |
| `blinker_horizontal.txt` | oscilador de periodo 2 |
| `toad_oscillator.txt` | oscilador de periodo 2 |
| `beacon_oscillator.txt` | oscilador de periodo 2 |
| `glider_large.txt` | desplazamiento diagonal |
| `edge_corner.txt` | tratamiento de límites, sin envoltura |
| `full_5x5.txt` | sobrepoblación |
| `r_pentomino.txt` | evolución prolongada a partir de cinco células |
| `gosper_glider_gun.txt` | emisión periódica de planeadores |
| `blinker.txt`, `glider.txt` | ejemplos compactos originales |

Cada archivo contiene una matriz rectangular de caracteres `0` y `1`, una fila por línea.
El lector los convierte a valores `int`. **No hay vecinos que atraviesen los bordes**.
Los ejemplos compactos pueden cortar la evolución porque el tablero no crece;
para observar desplazamientos conviene usar `glider_large.txt` o crear matrices amplias.

## Recomendación para estudiantes

Antes de ejecutar, prediga la generación 1 y el número de células vivas.
Pruebe especialmente esquinas, bordes, una celda aislada, y patrones estables.
La simulación de la versión inicial compila pero no evoluciona correctamente
hasta completar `model/model.c`.
