/*
 *
 * Utilizar el pulsador (B1) para controlar el tiempo de encendido.
 * Cada vez que se presiona el pulsador el tiempo de encendido
 * debe alternar entre 200 ms y 500 ms.
 *
 * Implementación usando la interrupción generada al presionar el botón
 *
 */

/* descomentar para probar la solución al punto 2 mediante interrupciones */
#define PRACTICA1_PUNTO2_INTER
#ifdef PRACTICA1_PUNTO2_INTER

#include "main.h"

/*
 * Índice actual dentro de cycle_times
 * Declarado como volatile debido a que se modifica concurrentemente por la interrupción
 */
volatile static int cycle_idx = 0;

/*
 * Tiempos entre los que alterna al precionar el botón
 */
static const int cycle_times[] = { 200, 500};

/*
 * Implementación a base de interrupciones
 */
void practica1() {
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	HAL_Delay(cycle_times[cycle_idx]);
}

/*
 * alterna entre los diferentes índices posibles de cycle_times
 */
void cycle_time_index() {
	cycle_idx ^= 1;
}

/*
 *  HAL_GPIO_EXTI_Callback capturar las interrupciones GPIO
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	/* Nos importa solamente las interrupciones del botón BTN_Pin */
	if ( GPIO_Pin == BTN_Pin ) {
		cycle_time_index();
	}
}

#endif /* PRACTICA1_PUNTO2_INTER */

