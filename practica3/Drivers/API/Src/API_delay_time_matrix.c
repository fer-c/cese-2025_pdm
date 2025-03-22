/*
 * API_delay_matrix.c
 *
 *  Created on: Mar 21, 2025
 *      Author: fcoronel
 */
#include <API_delay_time_matrix.h>
#include "stm32f4xx_hal.h"

void delayTimeMatrixInit(delay_time_matrix_t *matrix, index_t num_durations, duration_times_t durations[]) {
	assert_param(matrix != NULL);
	assert_param(durations != NULL);

	matrix->num_durations = num_durations;
	matrix->current_idx = 0;
	matrix->current_attempt = 0;
	matrix->durations = durations;
}

tick_t delayTimeMaxtrixCurrentDuration(delay_time_matrix_t *matrix) {
	assert_param(matrix != NULL);

	return matrix->durations[matrix->current_idx].duration;
}

void delayTimeMatrixNext(delay_time_matrix_t *matrix) {
	assert_param(matrix != NULL);

	matrix->current_idx = (matrix->current_idx + 1) % matrix->num_durations;
	matrix->current_attempt = 0;
}

bool_t delayTimeMatrixRead(delay_time_matrix_t *matrix) {
	assert_param(matrix != NULL);

	bool_t result = false;
	matrix->current_attempt++;

	if (matrix->current_attempt >= matrix->durations[matrix->current_idx].times) {
		result = true;
		delayTimeMatrixNext(matrix);
	}

	return result;
}

void delayTimeMatrixDelayInit(delay_t *delay, delay_time_matrix_t *matrix) {
	assert_param(delay != NULL);
	assert_param(matrix != NULL);
	tick_t duration = delayTimeMaxtrixCurrentDuration(matrix);
	delayInit(delay, duration);
}


