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

/**
 * @brief Actualiza el led esta en estado RESET.
 *
 * @return void.
 */
void updateLedStateReset();

/**
 * @brief Verifica si el led esta en estado RESET.
 *
 * @return `true` si el led esta en estado RESET, `false` en caso contrario.
 */
bool_t checkLedStateReset();

/**
 * @brief Cambia el estado del led.
 *
 * @return void.
 */
void toggleLedState();


void setupAll() {
	/* Inicializa el LD2 en estado RESET */
	updateLedStateReset();
}

void doTask(delay_t *delay, delay_time_matrix_t *matrix) {
	assert_param(delay != NULL);
	assert_param(matrix != NULL);

	if (delayRead(delay)) { // Si el delay fue completado.
		updateLedOnDelay(delay, matrix); // Actualiza el estado del led en base al delay.
	}

}

/* funciones privadas */

void updateLedStateReset() {
	/* Inicializa el LD2 en estado RESET */
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
}

static void updateDelay(delay_t *delay, delay_time_matrix_t *matrix) {
	assert_param(delay != NULL);
	assert_param(matrix != NULL);

	if (delayTimeMatrixRead(matrix)) {
		tick_t newDuration = delayTimeMaxtrixCurrentDuration(matrix);
		delayWrite(delay, newDuration);
	}
}

bool_t checkLedStateReset() {
	return HAL_GPIO_ReadPin(LD2_GPIO_Port, LD2_Pin) == GPIO_PIN_RESET;
}

void toggleLedState() {
	return HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}

static void updateLedOnDelay(delay_t *delay, delay_time_matrix_t *matrix) {
	if (checkLedStateReset()) {
		updateDelay(delay, matrix);
	}
	toggleLedState();
}
