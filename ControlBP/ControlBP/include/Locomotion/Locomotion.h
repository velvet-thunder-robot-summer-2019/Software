#ifndef LOCOMOTION_H_
#define LOCOMOTION_H_

/**
 * Performs necessary initialisation for tape following
 * Initialises PID and tape-sensing. Must be called before follow_tape
 */
void init_tape_following();

/**
 * Adjusts PWM of both motors once
 * Returns:     SUCCESS if successful
 *              TAPE_NOT_FOUND if tape repeatedly fails to be found
 */
int follow_tape();


#endif // LOCOMOTION_H_