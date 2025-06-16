#include "keypad_driver.h"

static const char keypad_map[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

#define KEYPAD_DEBOUNCE_MS 500

// Variables estáticas para antirrebote por columna
static uint16_t last_pressed_col = 0xFFFF;
static uint32_t last_tick = 0;

// Función de antirrebote: solo permite una pulsación por columna cada KEYPAD_DEBOUNCE_MS
static uint8_t keypad_debounce(uint16_t col_pin) {
    uint32_t now = HAL_GetTick();
    if (last_pressed_col == col_pin) {
        if (now - last_tick < KEYPAD_DEBOUNCE_MS) {
            return 0; // Si no ha pasado el tiempo, ignora la pulsación (rebote)
        }
    }
    last_pressed_col = col_pin;
    last_tick = now;
    return 1; //Pulsación válida
}


// Inicializa el keypad: pone todas las filas en bajo
void keypad_init(keypad_handle_t* keypad) {
    for (int i = 0; i < KEYPAD_ROWS; i++) {
        HAL_GPIO_WritePin(keypad->row_ports[i], keypad->row_pins[i], GPIO_PIN_RESET);
    }
}

// Escanea filas para una columna activa y retorna el índice de la fila presionada, o -1 si ninguna
static int keypad_scan_row(keypad_handle_t* keypad, int col_idx) {
    for (int row = 0; row < KEYPAD_ROWS; row++) {
        // Poner todas las filas en bajo
        for (int i = 0; i < KEYPAD_ROWS; i++)
            HAL_GPIO_WritePin(keypad->row_ports[i], keypad->row_pins[i], GPIO_PIN_RESET);

        // Activar solo la fila actual
        HAL_GPIO_WritePin(keypad->row_ports[row], keypad->row_pins[row], GPIO_PIN_SET);

        // Pequeño retardo para estabilizar
        for (volatile int d = 0; d < 500; d++);

        // Si la columna está en alto, esa es la fila presionada
        if (HAL_GPIO_ReadPin(keypad->col_ports[col_idx], keypad->col_pins[col_idx]) == GPIO_PIN_SET) {
            // Restaura la fila antes de salir
            HAL_GPIO_WritePin(keypad->row_ports[row], keypad->row_pins[row], GPIO_PIN_RESET);
            return row;
        }
    }
    // Restaura todas las filas en bajo
    for (int i = 0; i < KEYPAD_ROWS; i++)
        HAL_GPIO_WritePin(keypad->row_ports[i], keypad->row_pins[i], GPIO_PIN_RESET);
    return -1;
}

// Escaneo principal del keypad: recibe el pin de columna que generó la interrupción
char keypad_scan(keypad_handle_t* keypad, uint16_t col_pin) {
    // Antirrebote por columna
    if (!keypad_debounce(col_pin)) {
        return '\0';
    }

    // Busca el índice de la columna
    int col_idx = -1;
    for (int i = 0; i < KEYPAD_COLS; i++) {
        if (keypad->col_pins[i] == col_pin) {
            col_idx = i;
            break;
        }
    }
    if (col_idx == -1) return '\0';

    // Escanea filas para esa columna
    int row_idx = keypad_scan_row(keypad, col_idx);
    if (row_idx >= 0 && row_idx < KEYPAD_ROWS) {

        // Si se encontró una fila presionada, retorna el carácter correspondiente
        return keypad_map[row_idx][col_idx];
    }
    return '\0';
}