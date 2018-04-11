#ifndef BALLGUN_H
#define BALLGUN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ch.h>

typedef enum {
    BALLGUN_DISABLED = 0,
    BALLGUN_RETRACTED,
    BALLGUN_DEPLOYED,
} ballgun_state_t;

typedef enum {
    BALLGUN_ARMED = 0,
    BALLGUN_CHARGING,
    BALLGUN_FIRING,
} ballgun_turbine_state_t;

typedef struct {
    void (*set_ballgun)(void*, float);
    void* ballgun_args;

    void (*set_turbine)(void*, float);
    void* turbine_args;

    void (*set_accelerator)(void*, float);
    void* accelerator_args;

    ballgun_state_t state;
    ballgun_turbine_state_t turbine_state;

    float servo_retracted_pwm;  // pwm duty cycle in seconds to retract servo
    float servo_deployed_pwm;   // pwm duty cycle in seconds to deploy servo

    float turbine_armed_pwm;    // pwm duty cycle in seconds to arm the turbine ESC
    float turbine_charge_pwm;   // pwm duty cycle in seconds to charge by sucking up
    float turbine_fire_pwm;     // pwm duty cycle in seconds to fire by sucking up

    float accelerator_charge;   // accelerator voltage to get balls in
    float accelerator_fire;     // accelerator voltage to get balls out

    mutex_t lock;
} ballgun_t;

/* Initialize ballgun */
void ballgun_init(ballgun_t* ballgun);
void ballgun_set_servo_range(ballgun_t* ballgun, float servo_retracted_pwm, float servo_deployed_pwm);
void ballgun_set_turbine_range(ballgun_t *ballgun, float turbine_armed_pwm, float turbine_charge_pwm,
                               float turbine_fire_pwm);
void ballgun_set_accelerator_range(ballgun_t *ballgun, float accelerator_charge, float accelerator_fire);

/* Actuator callbacks */
void ballgun_set_callbacks(ballgun_t* ballgun, void (*set_ballgun)(void*, float), void* ballgun_args);
void ballgun_set_turbine_callbacks(ballgun_t* ballgun, void (*set_turbine)(void*, float), void* turbine_args);
void ballgun_set_accelerator_callbacks(ballgun_t* ballgun, void (*set_accelerator)(void*, float),
                                       void* accelerator_args);

/* Deploy ballgun */
void ballgun_deploy(ballgun_t* ballgun);

/* Retract ballgun */
void ballgun_retract(ballgun_t* ballgun);

/* Turbine control: Arm, Charge, Fire */
void ballgun_arm(ballgun_t* ballgun);
void ballgun_charge(ballgun_t* ballgun);
void ballgun_fire(ballgun_t* ballgun);

/* Ballgun tidy : arm and retract */
void ballgun_tidy(ballgun_t* ballgun);

#ifdef __cplusplus
}
#endif

#endif /* BALLGUN_H */
