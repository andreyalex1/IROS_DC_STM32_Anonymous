/*
 * motion_lib.c
 *
 *  Motion control library
 */
#include "main.h"

int dc_init(struct dc_motor *motor) {
	HAL_TIM_PWM_Start(motor->pwm_timer, TIM_CHANNEL_1);			// BTS3_LPWM
	HAL_TIM_PWM_Start(motor->pwm_timer, TIM_CHANNEL_2);			// BTS3_RPWM

	HAL_TIM_Encoder_Start(motor->encoder_timer, TIM_CHANNEL_ALL);
	return 1;
}

int dc_set_pulse(struct dc_motor *motor) {
	int int_pulse = (int) (motor->pulse * 100);
	if (int_pulse == 0) {
		__HAL_TIM_SET_COMPARE(motor->pwm_timer, TIM_CHANNEL_1, 0);
		__HAL_TIM_SET_COMPARE(motor->pwm_timer, TIM_CHANNEL_2, 0);
	}
	if (int_pulse < 0) {
		__HAL_TIM_SET_COMPARE(motor->pwm_timer, TIM_CHANNEL_1,
				-2999 * motor->pulse);
		__HAL_TIM_SET_COMPARE(motor->pwm_timer, TIM_CHANNEL_2, 0);
	}
	if (int_pulse > 0) {
		__HAL_TIM_SET_COMPARE(motor->pwm_timer, TIM_CHANNEL_1, 0);
		__HAL_TIM_SET_COMPARE(motor->pwm_timer, TIM_CHANNEL_2,
				2999 * motor->pulse);
	}
	return 1;
}

int dc_set_pulse_A1A2(struct dc_motor *motor) {
	int int_pulse = (int) (motor->pulse * 100);
	if (int_pulse == 0) {
		__HAL_TIM_SET_COMPARE(motor->pwm_timer, motor->pwm_channel, 0);
		HAL_GPIO_WritePin(motor->A1_port, motor->A1_pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(motor->A2_port, motor->A2_pin, GPIO_PIN_RESET);
	}
	if (int_pulse < 0) {
		HAL_GPIO_WritePin(motor->A1_port, motor->A1_pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(motor->A2_port, motor->A2_pin, GPIO_PIN_RESET);
		__HAL_TIM_SET_COMPARE(motor->pwm_timer, motor->pwm_channel, -2999 * motor->pulse);
	}
	if (int_pulse > 0) {
		HAL_GPIO_WritePin(motor->A1_port, motor->A1_pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(motor->A2_port, motor->A2_pin, GPIO_PIN_SET);
		__HAL_TIM_SET_COMPARE(motor->pwm_timer, motor->pwm_channel, 2999 * motor->pulse);
	}
	return 1;
}

int dc_calculate_period(struct dc_motor *motor) {
	int clock_period, clock_current;
	int clock_max = motor->clock_timer->Instance->ARR;
	clock_current = motor->clock_timer->Instance->CNT;
	clock_period = (clock_current - motor->clock_prev + clock_max) % clock_max;
	motor->period_seconds = clock_period / 100000.0;//clocks are ticking at 100Khz
	motor->clock_prev = clock_current;
	return 1;
}

int dc_calculate_velocity(struct dc_motor *motor) {
	int encoder_current, encoder_distance;
	float degrees_distance;
	int encoder_max = motor->encoder_timer->Instance->ARR;
	float ticks_per_rotation = motor->gear_ratio * motor->encoder_resolution;

	encoder_current = motor->encoder_timer->Instance->CNT;
	// no counter reset
	if (abs(encoder_current - motor->encoder_prev) < encoder_max / 2) {
		encoder_distance = encoder_current - motor->encoder_prev;
	}
	//counter was reset
	else {
		// going backwards
		if (encoder_current > motor->encoder_prev) {
			encoder_distance = encoder_max - encoder_current
					+ motor->encoder_prev;
		}
		//going forwards
		else {
			encoder_distance = encoder_current + encoder_max
					- motor->encoder_prev;
		}
	}
	degrees_distance = motor->velocity_correction_gain * encoder_distance * 360.0 / ticks_per_rotation;
	motor->encoder_prev = encoder_current;

	//for debug purposes
	motor->encoder_ticks_debug = encoder_distance;


	if (motor->period_seconds > 0) {
		motor->raw_velocity = clamp_float(degrees_distance / motor->period_seconds, -360.0, 360.0);
		motor->filtered_velocity = motor->filtered_velocity
				* motor->low_pass_gain
				+ motor->raw_velocity * (1 - motor->low_pass_gain);
		motor->filtered_velocity = clamp_float(motor->filtered_velocity, -360.0, 360.0);
		motor->acceleration = (motor->filtered_velocity
				- motor->filtered_velocity_prev) / motor->period_seconds;
		motor->acceleration_filtered = motor->acceleration_filtered
				* motor->low_pass_gain
				+ motor->acceleration * (1 - motor->low_pass_gain);
	}
	return 1;
}

int dc_pid(struct dc_motor *motor) {
	float velocity_error;
	velocity_error = motor->target_velocity - motor->filtered_velocity;
	if(!((velocity_error > 0 && motor->pid_value > motor->voltage_limit) || (velocity_error < 0 && motor->pid_value < -motor->voltage_limit))){
		motor->integral_error = motor->integral_error * motor->integral_fade_gain
				+ velocity_error * motor->period_seconds;
	}
	motor->pid_value = motor->gain_p * velocity_error
			+ motor->gain_i * motor->integral_error
			- motor->gain_d * motor->acceleration_filtered;
	return 1;
}

int stepper_init(struct stepper *stp) {
	HAL_TIM_PWM_Start(stp->timer, stp->channel);
	HAL_ADC_Start_DMA(stp->adc, stp->adc_buffer, adc_buffer_len);// enable analog encoder ADC
	return 1;
}

int stepper_set(struct stepper *stp) {
	int vel_int = abs((int) ((stp->current_velocity) * 100));
	int precalc_period = 1000000;
	if (vel_int > 0) {
		// assumed 2mhz timer clock after prescaler and 200 ticks per motor rotation
		//current velocity is to be in degrees
		precalc_period = (int) (40000.0 * 100
				/ (stp->gear_ratio * vel_int));
		stp->precalc_period = precalc_period;
	}
	//enabls-disable
	if (stp->enable > 0) {
		HAL_GPIO_WritePin(stp->en_port, stp->en_pin, GPIO_PIN_SET); //STP1_EN
	} else {
		HAL_GPIO_WritePin(stp->en_port, stp->en_pin, GPIO_PIN_RESET);	//STP1_EN
	}
	//set direction
	if (stp->current_velocity > 0) {
		HAL_GPIO_WritePin(stp->dir_port, stp->dir_pin, GPIO_PIN_RESET);//STP1_DIR
	} else {
		HAL_GPIO_WritePin(stp->dir_port, stp->dir_pin, GPIO_PIN_SET);	//STP1_DIR
	}
	//set velocity
	if (precalc_period > 65000 || precalc_period < 1) {
		__HAL_TIM_SET_COMPARE(stp->timer, stp->channel, 0);
	//	stp->
	} else {
		__HAL_TIM_SET_AUTORELOAD(stp->timer, precalc_period);
		__HAL_TIM_SET_COMPARE(stp->timer, stp->channel, precalc_period / 2);
		if(stp->timer->Instance->CNT > precalc_period){
			stp->timer->Instance->CNT = 0;
		}
	}
	return 1;
}

int stepper_calculate_angle(struct stepper *stp) {
	int c;
	int sum = 0, avg;
	for (c = 0; c < adc_buffer_len; ++c) {
		sum += (stp->adc_buffer[c] + stp->raw_angle_offset) % 4096;
	}
	avg = sum / adc_buffer_len;
	stp->current_position = avg * 360.0 / 4096.0 - 180.0;
	return 1;
}

int stepper_logic(struct stepper *stp) {
	float position_error;
	float current_velocity;
	position_error = stp->current_position - stp->target_position;
	current_velocity = abs_float(stp->target_velocity)
			* sign_float(position_error);
	// if stepper is driving, adjust direction and drive until its within drive_position_tolerance
	if (abs_float(stp->current_velocity) > 0.01) {
		// normal velocity value
		if (abs_float(position_error)
				> abs_float(stp->drive_position_tolerance)) {
			stp->current_velocity = current_velocity;
		} else {
			stp->current_velocity = 0.0;
		}
	}
	// stepper is stationary, engage drive only if position error exceeds hold_position_tolerance
	else {
		if (abs_float(position_error)
				> abs_float(stp->hold_position_tolerance)) {
			stp->current_velocity = current_velocity;
		} else {
			stp->current_velocity = 0.0;
		}
	}
	stp->current_velocity *= stp->direction_fix;
	return 1;
}

float abs_float(float val) {
	if (val < 0) {
		return -val;
	}
	return val;
}

int sign_float(float val) {
	if (val > 0) {
		return 1;
	}
	return -1;
}

float clamp_float(float val, float min, float max) {
	if (val > max) {
		val = max;
	}
	if (val < min) {
		val = min;
	}
	return val;
}

