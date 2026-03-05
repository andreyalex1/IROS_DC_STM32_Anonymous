/*
 * motion_lib.h
 *
 *  Motion control library
 */

#ifndef INC_MOTION_LIB_H_
#define INC_MOTION_LIB_H_

#include "stm32f4xx_hal.h"

typedef struct stepper {
	//command variables
	float target_velocity;			// in deg/s
	float target_position;			// in degrees
	int enable;						//if stepper coils are to be energized at all
	//sensor variables
	float current_position;			// measured angle in degrees
	//config variables
	int direction_fix;				// +-1 to align encoder and stepper directions (angle should increase when velocity is positive)
	int raw_angle_offset; 			//set zero for encoder, offset is edited at the raw ADC level
	float hold_position_tolerance;	// acceptable position error when holding
	float drive_position_tolerance; // acceptable position error when driving, should be less than hold for correct operation
	float gear_ratio;				// gear ratio of this stepper (assumed 200 steps per rotation, 100KHz PWM clock frequency [100 000 = system_clock / (timer_prescaler + 1)]
	int is_backdriveable;			//if 0 the stepper will be powered down when holding to coserve energy. Useful for worm gearboxes.
	TIM_HandleTypeDef *timer;		// timer used to generate PWM, can use 1 timer for several steppers
	uint16_t channel;				// timer channel, 1 channel per stepper
	GPIO_TypeDef* en_port;			// driver enable GPIO port
	uint16_t en_pin;				// driver enable GPIO pin
	GPIO_TypeDef* dir_port;			// driver dir GPIO port
	uint16_t dir_pin;				// driver dir GPIO pin
	ADC_HandleTypeDef* adc;			// ADC used to calculate encoder angle
	uint32_t adc_buffer[adc_buffer_len];	// ADC measurements buffer. Needs adc_buffer_len defined
	//derieved varibles (not to be edited by the user, used mid-computation, can be monitored for debug)
	int precalc_period;
	float current_velocity;
	long ticks_last;
	long ticks_total;
}stepper;






typedef struct dc_motor {
	//command variables
	float target_velocity;			// target velocity in degrees/second
	//sensor variables
	float raw_velocity;				// measured unfiltered velocity in degrees/second
	float filtered_velocity;		// measured filtered velocity in degrees/second
	float filtered_velocity_prev;	// previous measured filtered velocity, used to calculate acceleration
	float acceleration_filtered;	// filtered acceleration, deg/s^2
	float acceleration;				// raw acceleration, deg/s^2
	//config variables
	float voltage_limit;			// 0 to 1 depending of what PWM duty сyсle is considered maximum
	float gear_ratio;				// motor gear ratio
	float encoder_resolution;		// number of encoder ticks per rotation
	float low_pass_gain;			// gain of the low-pass velocity and acceleration filter
	float velocity_correction_gain;	// a gain that velocity is multiplied by when its computed. Determined experimentally to account for various errors. Set to 1 by default.
	float gain_p, gain_i, gain_d;	// PID gains
	float integral_fade_gain;		//integral error is multiplied by this gain on every control cycle.
	TIM_HandleTypeDef *pwm_timer;	// PWM timer, can be same for several motors
	uint16_t pwm_channel;			// PWM channel for single-channel drivers, otherwise channels 1 and 2 are used
	TIM_HandleTypeDef *encoder_timer;	//Timer used in encoder mode, 1 per motor
	TIM_HandleTypeDef *clock_timer;	// a system-wide clock timer, 100KHz after prescaler
	GPIO_TypeDef* A1_port;			// for a driver with A1A2 interface and single PWM pin
	uint16_t A1_pin;
	GPIO_TypeDef* A2_port;
	uint16_t A2_pin;
	//derieved varibles (not to be edited by the user)
	float pid_value;
	float pulse;
	int clock_prev;
	int encoder_prev;
	float integral_error;
	float period_seconds;
	//debug variables
	int encoder_ticks_debug;
}dc_motor;


int dc_init(struct dc_motor *motor);				// initialize  peripherals
int dc_set_pulse(struct dc_motor *motor);			// set pusle for a twin-channel DC driver
int dc_set_pulse_A1A2(struct dc_motor *motor);		// set pulse for a single-channel DC driver
int dc_calculate_period(struct dc_motor *motor);	// calculate time period that elapsed
int dc_calculate_velocity(struct dc_motor *motor);	// calculate velocity after a period is known
int dc_pid(struct dc_motor *motor);					// calculate motor inputs via PID

int stepper_init(struct stepper *stp);				// initialize  peripherals
int stepper_set(struct stepper *stp);				// apply en, dir, step to motor
int stepper_calculate_angle(struct stepper *stp);	// calculate current stepper angle from encoder
int stepper_logic(struct stepper *stp); 			// interprets angles and sets target velocity to arrive at target angle and stay there

float abs_float(float val);							// calculates absolute value of float
int sign_float(float val);							// returns sign of given float
float clamp_float(float val, float min, float max);	// clamps float to range



#endif /* INC_MOTION_LIB_H_ */
