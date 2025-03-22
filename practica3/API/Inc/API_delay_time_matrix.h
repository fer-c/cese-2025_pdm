/*
 * API_delay_matrix.h
 *
 *  Created on: Mar 21, 2025
 *      Author: fcoronel
 */

#ifndef INC_API_DELAY_TIME_MATRIX_H_
#define INC_API_DELAY_TIME_MATRIX_H_

#include <stdarg.h>
#include "API_delay.h"

typedef uint8_t index_t;
typedef uint8_t times_t;

/**
 * @brief Tupla que representa una duración y un número de veces que se ejecutará.
 */
typedef struct {
	tick_t duration; /**< Duración. */
	index_t times; /**< Número de veces que se ejecutará la duración. */
} duration_times_t;

/**
 * @brief Estructura que representa los tiempos de duración de un delay.
 *
 * Esta estructura se utiliza para gestionar duraciones de ejecución
 * de un delay en base a una array de duraciones.
 *
 * Para ello se define un array de duraciones y un array de repeticiones para cada duración.
 *
 * De esta forma, se puede definir un delay que se ejecutará con una duración determinada
 * en un número de veces determinado.
 *
 * @note Se debe inicializar la matriz con la función delayMatrixInit.
 *
 */
typedef struct {
	index_t num_durations; /**< Número de duraciones. */
	duration_times_t *durations; /**< Array de duraciones. */
	index_t current_idx; /**< Índice en el array de duraciones que indica la duración actual. */
	times_t current_attempt; /**< cantidad de intentos en la duración actual. */
} delay_time_matrix_t;

/**
 * @brief Inicializa una matriz de duraciones con un array de repeticiones para cada duración.
 *
 * @param num_durations Número total de duraciones.
 * @param durations Array de duraciones.
 *
 * @return void
 *
 */
void delayTimeMatrixInit(delay_time_matrix_t *matrix, index_t num_durations, duration_times_t durations[]);

/**
 * @brief Devuelve la duración actual de la matriz.
 *
 * @param matrix Puntero a la matriz de duraciones.
 *
 * @return Duración actual.
 */
tick_t delayTimeMaxtrixCurrentDuration(delay_time_matrix_t *matrix);

/**
 * @brief Avanza la duración matriz y devuelve la nueva duración.
 *
 * @param matrix Puntero a la matriz de duraciones.
 *
 * @return void.
 */
void delayTimeMatrixNext(delay_time_matrix_t *matrix);

/**
 * @brief Devuelve la duración siguiente de la matriz
 * actualizando el estado de la matriz.
 *
 * Si la duración actual se ha ejecutado el número de veces determinado para esta,
 * se procede a la siguiente duración.
 *
 * @param matrix Puntero a la matriz de duraciones.
 *
 * @return `true` si se produzco un cambio de duración o `false` en caso de mantenerse.
 */
bool_t delayTimeMatrixRead(delay_time_matrix_t *matrix);

/**
 * @brief Inicializa un delay con una matriz de duraciones.
 *
 * @param delay Puntero al delay.
 * @param matrix Puntero a la matriz de duraciones.
 *
 * @return void
 */
void delayTimeMatrixDelayInit(delay_t *delay, delay_time_matrix_t *matrix);

#endif /* INC_API_DELAY_TIME_MATRIX_H_ */
