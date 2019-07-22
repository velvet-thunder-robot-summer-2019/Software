#ifndef GLOBAL_VARS_H_
#define GLOBAL_VARS_H_

enum motor_direction 
{
    CLOCKWISE,
    ANTI_CLOCKWISE
};

struct pwm_response 
{
    motor_direction dir;
    float   pwm_val;
};

struct coordinate 
{
    float x;
    float y;
    float z;
};

#endif
