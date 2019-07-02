#ifndef GLOBALVARIABLES_H_
#define GLOBALVARIABLES_H_

#define TOTAL_NUM_STONE 6

// defines for DEBUG board
#define THANOS 0
#define METHANOS 1

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

typedef struct
{
    /* data */
    location last_location;
    location next_location;
    int fraction_to_next;  //fraction of the dist made to the next location
}position;


extern volatile state bot_state; // state of robot
extern volatile state bot_previous_state; //previous state of robot
extern stone_status stones_status[TOTAL_NUM_STONE]; //tracking what we know about the infinity stones. I don't think it needs volatile?
extern position bot_position; // ditto, pretty sure we don't need volatile

#endif //GLOBALVARIABLES_H_