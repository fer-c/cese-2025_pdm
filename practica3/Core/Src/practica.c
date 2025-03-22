/*
 * practica.c
 *
 *  Created on: Mar 22, 2025
 *      Author: fcoronel
 */

#include "practica.h"
#include "stm32f4xx_hal.h"

#define BTN_Pin GPIO_PIN_13
#define BTN_GPIO_Port GPIOC
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA

/**
 * @brief Actualiza el delay en base a la matriz de duraciones.
 * Si la duración actual se completó, se pasa a la siguiente duración.
 *
 * @param delay Puntero al delay.
 * @param matrix Puntero a la matriz de duraciones.
 *
 * @return void
 */
static void updateDelay(delay_t *delay, delay_time_matrix_t *matrix);

/**
 * @brief Actualiza el estado del led en base a un delay.
 * Si el ciclo fue completado (es decir el led esta en estado RESET nuevamente),
 * se actualiza el delay en base a la matriz de duraciones.
 *
 * @param delay Puntero al delay.
 * @param matrix Puntero a la matriz de duraciones.
 *
 * @return void
 */
static void updateLedOnDelay(delay_t *delay, delay_time_matrix_t *matrix);

void setupAll() {
	/* Inicializa el LD2 en estado RESET */
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
}

void doTask(delay_t *delay, delay_time_matrix_t *matrix) {
	assert_param(delay != NULL);
	assert_param(matrix != NULL);

	if (delayRead(delay)) { // Si el delay fue completado.
		updateLedOnDelay(delay, matrix); // Actualiza el estado del led en base al delay.
	}

}

/* funciones privadas */

static void updateDelay(delay_t *delay, delay_time_matrix_t *matrix) {
	assert_param(delay != NULL);
	assert_param(matrix != NULL);

	if (delayTimeMatrixRead(matrix)) {
		tick_t newDuration = delayTimeMaxtrixCurrentDuration(matrix);
		delayWrite(delay, newDuration);
	}
}

static void updateLedOnDelay(delay_t *delay, delay_time_matrix_t *matrix) {
	if (HAL_GPIO_ReadPin(LD2_GPIO_Port, LD2_Pin) == GPIO_PIN_RESET) {
		updateDelay(delay, matrix);
	}
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}
