/*
 * API_debounce.c
 *
 *  Created on: Mar 28, 2025
 *      Author: fcoronel
 */

#include "API_debounce.h"
#include "main.h"

static const tick_t DEBOUNCE_DELAY_TIME = 40; /**< Tiempo de debounce en milisegundos */


/**
 * @brief  Error por transición invalida en la FSM.
 * @param  fsm Puntero a la máquina de estados
 * @return None
 */
void debounceFSM_error(debounce_t *fsm);


/**
 * @brief  Logica de la FSM para el estado BUTTON_UP.
 * @param  fsm Puntero a la máquina de estados
 * @retval None
 */
void updateStateButtonUp(debounce_t *fsm);

/**
 * @brief  Logica de la FSM para el estado BUTTON_FALLING
 * @param  fsm Puntero a la máquina de estados
 * @retval None
 */
void updateStateButtonFalling(debounce_t *fsm);

/**
 * @brief  Logica de la FSM para el estado BUTTON_DOWN
 * @param  fsm Puntero a la máquina de estados
 * @retval None
 */
void updateStateButtonDown(debounce_t *fsm);

/**
 * @brief  Logica de la FSM para el estado BUTTON_RAISING
 * @param  fsm Puntero a la máquina de estados
 * @retval None
 */
void updateStateButtonRaising(debounce_t *fsm);

// Impl debounceFSM_init
void debounceFSM_init(debounce_t *fsm) {
	fsm->state = BUTTON_UP;
	fsm->btn_pressed = false; // Inicializa el estado del botón como no presionado
	delayInit(&(fsm->debounce_delay), DEBOUNCE_DELAY_TIME); //definir const
}

// Impl readKey
bool_t readKey(debounce_t *fsm) {
	bool_t result = fsm->btn_pressed;
	fsm->btn_pressed = false; // Reinicia el estado del botón al leerlo
	return result;
}

// Impl debounceFSM_error
void debounceFSM_error(debounce_t *fsm) {
	// Manejo de error: reiniciar la FSM
	debounceFSM_init(fsm);
}

// Impl debounceFSM_update
void debounceFSM_update(debounce_t *fsm) {
	switch (fsm->state) {
	case BUTTON_UP:
		updateStateButtonUp(fsm);
		break;
	case BUTTON_FALLING:
		updateStateButtonFalling(fsm);
		break;
	case BUTTON_DOWN:
		updateStateButtonDown(fsm);
		break;
	case BUTTON_RAISING:
		updateStateButtonRaising(fsm);
		break;
	default:
		debounceFSM_error(fsm);
		break;
	}
}

// -- Private functions --

void updateStateButtonUp(debounce_t *fsm) {
	if (HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin)) {
		delayRead(&fsm->debounce_delay);
		fsm->state = BUTTON_FALLING;
	}
}

void updateStateButtonFalling(debounce_t *fsm) {
	if (delayRead(&fsm->debounce_delay)) {
		if (HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin)) {
			fsm->state = BUTTON_DOWN;
			/** El botón se considera como presionado en el flanco de transición
			 * y se ejecuta la función callback buttonPressed.
			 */
			buttonPressed(fsm);
		} else {
			fsm->state = BUTTON_UP;
		}
	}
}

void updateStateButtonDown(debounce_t *fsm) {
	if (!HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin)) {
		delayRead(&fsm->debounce_delay);
		fsm->state = BUTTON_RAISING;
	}
}

void updateStateButtonRaising(debounce_t *fsm) {
	if (delayRead(&fsm->debounce_delay)) {
		if (!HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin)) {
			fsm->state = BUTTON_UP;
			/** El botón se considera como presionado en el flanco de transición
			 * y se ejecuta la función callback buttonPressed.
			 */
			buttonReleased(fsm);
		} else {
			fsm->state = BUTTON_DOWN;
		}
	}
}

__weak void buttonPressed(debounce_t *fsm) {
	fsm->btn_pressed = true; // El botón fue presionado
}

__weak void buttonReleased(debounce_t *fsm) {
}
