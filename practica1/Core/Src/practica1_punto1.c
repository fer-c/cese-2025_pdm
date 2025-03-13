/*
 *
 * El led debe permanecer encendido 200 ms con duty cycle 50%.
 *
 */

/* descomentar para probar la solución al punto 1 */

//#define PRACTICA1_PUNTO1
#ifdef PRACTICA1_PUNTO1

#include "main.h"

void practica1() {
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	HAL_Delay(200);
}

#endif /* PRACTICA1_PUNTO1 */
