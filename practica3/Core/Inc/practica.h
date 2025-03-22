/*
 * practica.h
 *
 *  Created on: Mar 22, 2025
 *      Author: fcoronel
 */

#ifndef INC_PRACTICA_H_
#define INC_PRACTICA_H_

#include "API_delay.h"
#include "API_delay_time_matrix.h"


/**
 * @brief Inicializa los recursos del ejercicio.
 * @return void
 */
void setupAll();

/**
 * @brief Ejecuta las actividades del ejercicio.
 *
 * @param delay Puntero al delay.
 * @param matrix Puntero a la matriz de duraciones.
 *
 * @return void
 */
void doTask(delay_t *delay, delay_time_matrix_t *matrix);

#endif /* INC_PRACTICA_H_ */
