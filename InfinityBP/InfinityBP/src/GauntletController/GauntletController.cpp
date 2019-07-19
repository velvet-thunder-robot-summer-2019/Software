/** 
 * This module positions the gauntlet to different positions
 */

#include "CommandExecution.h"
#include "GauntletController/GauntletController.h"
#include "Servo.h"

Servo gauntlet_servo 0;

/** Initialize the gauntlet
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation failed
 */
byte init_gauntlet(void)
{
    gauntlet_servo.attach(GAUNTLET_SERVO_PIN);

    gauntlet_storage_position();

    return COMM_SUCCESS;
}

/** Place the gauntlet into the storage orientation for driving
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation failed 
 */
byte gauntlet_storage_position(void)
{
    gauntlet_servo.write(GAUNTLET_STORAGE_POS);

    return COMM_SUCCESS;
}

/** Place the gauntlet into the orientation for stone placement
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation failed
 */
byte gauntlet_open_position(void)
{
    gauntlet_servo.write(GAUNTLET_OPEN_POS);

    return COMM_SUCCESS;
}

/** Place the gauntlet into the endgame orientation
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation failed
 */
byte gauntlet_deploy_position(void)
{
    gauntlet_servo.write(GAUNTLET_DEPLOY_POS);

    return COMM_SUCCESS;
}

