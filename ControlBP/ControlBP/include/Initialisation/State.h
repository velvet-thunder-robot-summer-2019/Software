#ifndef STATE_H_
#define STATE_H_

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
    POST_1,
    POST_2,
    POST_3,
    POST_4,
    POST_5,
    POST_6,
    THANOS_GAUNTLET,
    THANOS_START
}location;

typedef enum
{
    THANOS,
    METHANOS
}identity;

typedef struct
{
    /* data */
    location last_location;
    location next_location;
    int fraction_to_next;  //fraction of the dist made to the next location
}position;


extern volatile state bot_state; // state of robot
extern volatile state bot_previous_state; //previous state of robot
extern volatile stone_status stones_status[TOTAL_NUM_STONE]; //tracking what we know about the infinity stones. I don't think it needs volatile?
extern volatile position bot_position;
extern identity bot_identity;

/**
 * Gets called from MENU mode for testing
 * Params: start_position - location at which robot is beginning
 *         start_state    - state in which the robot is beginning
 */
void initialise_global_variables(position start_position);

/**
 * Initialisation from competition mode
 * Sets up my state as REACH_RAMP, my position as either METHANOS_START or THANOS_START depending on MvT
 */
void initialise_global_variables();


#endif //STATE_H_