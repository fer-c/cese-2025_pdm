/*
 * API_debounce.h
 *
 *  Created on: Mar 28, 2025
 *      Author: fcoronel
 */

#ifndef INC_API_DEBOUNCE_H_
#define INC_API_DEBOUNCE_H_

#include "API_delay.h"
#include "API_delay_time_matrix.h"


/**
 * @brief Enumeración de estados para la FSM de debounce.
 */
typedef enum {
	BUTTON_UP, BUTTON_FALLING, BUTTON_DOWN, BUTTON_RAISING,
} debounceState_t;

/**
 * @brief Estructura que representa la máquina de estados para el debounce.
 *
 * Esta estructura contiene el estado actual de la máquina de estados,
 * un delay para el debounce y una variable que indica si el botón fue presionado.
 *
 * @note Se debe inicializar la máquina de estados con la función debounceFSM_init.
 */
typedef struct {
	debounceState_t state; /**< Estado actual de la FSM. */
	delay_t debounce_delay; /**< Delay para el debounce. */
	bool_t btn_pressed; /**< Variable que indica si el botón fue presionado. */
} debounce_t;

/**
 * @brief  Inicializa el estado de la FSM
 * @param  fsm Puntero a la máquina de estados a inicializar
 * @return None
 */
void debounceFSM_init(debounce_t *fsm);

/**
 * @brief  Actualiza el estado de la FSM
 * @param  fsm Puntero a la máquina de estados a actualizar
 * @return None
 */
void debounceFSM_update(debounce_t *fsm);

/**
 * @brief  Devuelve true si el botón fue presionado desde la última vez
 * 	   que se llamó a la función
 * @param  fsm Puntero a la máquina de estados
 * @return true si el botón fue presionado, false en caso contrario
 */
bool_t readKey(debounce_t *fsm);

/**
 * @brief  Callback que se ejecuta cuando el FSM pasa al estado BUTTON_DOWN.
 * 	Esto se ejecuta en el flanco de transición.
 * @param  fsm Puntero a la máquina de estados
 * @return None
 */
void buttonPressed(debounce_t *fsm);

/**
 * @brief  Callback que se ejecuta cuando el FSM pasa al estado BUTTON_UP.
 *  Esto se ejecuta en el flanco de transición
 * @param  fsm Puntero a la máquina de estados
 * @return None
 */
void buttonReleased(debounce_t *fsm);

#endif /* INC_API_DEBOUNCE_H_ */
