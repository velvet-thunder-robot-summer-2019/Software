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

typedef struct
{
    /* data */
    location last_location;
    location next_location;
}position;

typedef enum
{
    UPPER,
    LOWER
}branch;

typedef struct
{
    /* data */
    stone_status stones_status[TOTAL_NUM_STONE];
    position bot_position;
    identity bot_identity;
    branch target_branch; // can be upper or lower

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