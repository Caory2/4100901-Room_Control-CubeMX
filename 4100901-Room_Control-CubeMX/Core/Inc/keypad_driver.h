#ifndef KEYPAD_DRIVER_H  //Define la estructura y las funciones necesarias para controlar y escanear el teclado desde el código principal.
#define KEYPAD_DRIVER_H

#include "main.h"
#include <stdint.h>

#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

//Esta estructura almacena los puertos y pines de cada fila y columna del teclado,
// permitiendo que el driver sea flexible y funcione con cualquier conexión de pines.
typedef struct {
    GPIO_TypeDef* row_ports[KEYPAD_ROWS]; // Punteros a los puertos GPIO de las filas
    uint16_t row_pins[KEYPAD_ROWS];     // Pines de las filas
    // Punteros a los puertos GPIO de las columnas
    GPIO_TypeDef* col_ports[KEYPAD_COLS]; 
    uint16_t col_pins[KEYPAD_COLS];
} keypad_handle_t;

void keypad_init(keypad_handle_t* keypad); // Inicializa el keypad (pone las filas en el estado inicial).
char keypad_scan(keypad_handle_t* keypad, uint16_t col_pin); //Escanea el teclado cuando ocurre una interrupción en una columna y devuelve el carácter de la tecla presionada.

#endif // KEYPAD_DRIVER_H