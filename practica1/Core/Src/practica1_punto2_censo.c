/*
 *
 * Utilizar el pulsador (B1) para controlar el tiempo de encendido.
 * Cada vez que se presiona el pulsador el tiempo de encendido
 * debe alternar entre 200 ms y 500 ms.
 *
 * Implementación censando el valor del boton antes de la transición
 * de estados
 *
 */

/* descomentar para probar la solución al punto 2 mediante censo */
//#define PRACTICA1_PUNTO2_CENSO
#ifdef PRACTICA1_PUNTO2_CENSO

#include "main.h"

/*
 * Índice actual dentro de cycle_times
 * volatile debido a que se modifica concurrentemente por la interrupción
 */
static int cycle_idx = 0;

/*
 * Tiempos entre los que alterna al precionar el botón
 */
static const int cycle_times[] = { 200, 500};

/*
 * alterna entre los diferentes índices posibles de cycle_times
 */
void cycle_time_index() {
	cycle_idx ^= 1;
}

void practica1() {

	if ( HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin) == GPIO_PIN_RESET )
	{
	  cycle_time_index();
	}
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	HAL_Delay(cycle_times[cycle_idx]);
}


#endif /* PRACTICA1_PUNTO2_CENSO */
