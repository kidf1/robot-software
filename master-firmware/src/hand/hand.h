#ifndef HAND_H
#define HAND_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ch.h>

#include <aversive/position_manager/position_manager.h>
#include "scara/scara.h"

#include "hand_utils.h"


typedef struct {
    float heading;
    hand_coordinate_t coordinate_system;
} hand_waypoint_t;

typedef struct {
    /* Motor control callbacks */
    void (*set_wrist_position)(void*, float); /**< Callback function to set wrist position. */
    void (*set_fingers)(finger_state_t*);     /**< Callback function to set fingers position. */

    /* Motor feedback callbacks */
    float (*get_wrist_position)(void*); /**< Callback function to get wrist position. */

    /* Motor control args */
    void* wrist_args;

    /* Motor positions */
    float wrist_pos;
    finger_state_t fingers_open[4];

    /* Control info */
    bool enable_control;
    hand_waypoint_t last_waypoint;

    /* Robot information */
    struct robot_position *robot_pos;
    scara_t *arm;

    mutex_t lock;
} hand_t;


/* Initialize specific hand */
void hand_init(hand_t* hand);

/* Set motor functions */
void hand_set_wrist_callbacks(hand_t* hand, void (*set_wrist_position)(void*, float), float (*get_wrist_position)(void*), void* wrist_args);
void hand_set_fingers_callbacks(hand_t* hand, void (*set_fingers)(finger_state_t*));

/* Move hand */
void hand_goto(hand_t* hand, float heading, hand_coordinate_t system);

/* Manage hand system, should run in a loop */
void hand_manage(hand_t* hand);

/* Set finger position of a hand */
void hand_set_finger(hand_t* hand, int index, finger_state_t state);

/* Set robot information */
void hand_set_related_robot_pos(hand_t *hand, struct robot_position *pos);
void hand_set_related_arm(hand_t *hand, scara_t *arm);

/* Convert waypoint to hand coordinate system */
float hand_convert_waypoint_coordinate(hand_t *hand, hand_waypoint_t waypoint);

#ifdef __cplusplus
}
#endif

#endif /* HAND_H */