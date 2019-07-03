#include "Locomotion/Locomotion.h"
#include "Initialisation/HardwareDefs.h"
#include "Locomotion/PID.h"
#include "Locomotion/TapeSensor.h"

/**
 * Performs necessary initialisation for tape following
 * Initialises PID and tape-sensing. Must be called before follow_tape
 */
void init_tape_following()
{
    init_PID();
}

/**
 * Adjusts PWM of both motors once
 * Returns:     SUCCESS if successful
 *              TAPE_NOT_FOUND if tape repeatedly fails to be found
 */
int follow_tape()
{
    //code
    return SUCCESS;
}