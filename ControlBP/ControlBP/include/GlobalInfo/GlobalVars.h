#ifndef GLOBAL_VARS_H_
#define GLOBAL_VARS_H_

#define TOTAL_NUM_STONE 6

// defines for DEBUG board
#define COMP 0
#define DEV 1

typedef enum
{
    MENU,
    DEBUG,
    REACH_RAMP,
    ASCEND_RAMP,
    CALIBRATE,
    FIND_POST,
    GET_INFINITY_STONE,
    RETURN_TO_GAUNTLET,
    FIT_TO_GAUNTLET,
    HANDLE_COLLISION,
    GOODNIGHT_SWEET_PRINCE
}state;

typedef enum
{
    UNKNOWN,
    COLLECTED,
    MISSING
}stone_status;

typedef enum
{
    METHANOS_START,
    METHANOS_GAUNTLET,
    METHANOS_INTERSECTION,
    POST_1,
    POST_2,
    POST_3,
    POST_4,
    POST_5,
    POST_6,
    THANOS_INTERSECTION,
    THANOS_GAUNTLET,
    THANOS_START
}location;

typedef enum
{
    THANOS,
    METHANOS
}identity;

typedef enum
{
    NO_COLLISION,
    LEFT_COLLISION,
    RIGHT_COLLISION,
    FRONT_COLLISION,
    BACK_COLLISION
}collision_direction;

typedef struct
{
    /* data */
    location last_location;
    location next_location;
    float left_wheel_rotations; // x dist in cm from last location
    float right_wheel_rotations; // y dist

     // either last_location or next_location
    // int fraction_to_next;  //fraction of the dist made to the next location
    // int wheel_rot_since_last_left;
    // int wheel_rot_since_last_right;
}position;

typedef struct
{
    /* data */
    state bot_state;
    state bot_previous_state;
    stone_status stones_status[TOTAL_NUM_STONE];
    position bot_position;
    identity bot_identity;
    collision_direction last_collision;
}status_summary;

extern volatile status_summary run_status;

/**
 * Gets called from MENU mode for testing
 * Params: start_position - location at which robot is beginning
 *         start_state    - state in which the robot is beginning
 */
void initialise_competition_data(position start_position);

/**
 * Initialisation from competition mode
 * Sets up my state as REACH_RAMP, my position as either METHANOS_START or THANOS_START depending on MvT
 */
void initialise_competition_data();


#endif //GLOBAL_VARS_H_