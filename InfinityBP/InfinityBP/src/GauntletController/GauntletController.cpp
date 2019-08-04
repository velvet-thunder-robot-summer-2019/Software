/** 
 * This module positions the gauntlet to different positions
 */

#include "Communications/CommandExecution.h"
#include "GauntletController/GauntletController.h"
#include "Servo.h"

/** Initialize the gauntlet
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation failed
 */
byte init_gauntlet(void)
{

    #if !MOCK_HARDWARE

        pwm_start(GAUNTLET_SERVO_PIN, SERVO_FREQ, SERVO_PERIOD, GAUNTLET_STORAGE_POS, 1);

        gauntlet_storage_position();

    #endif

    #if DEBUG_ALL

        Serial.println("Gauntlet Initialized");

    #endif

    return COMM_SUCCESS;
}

/** Place the gauntlet into the storage orientation for driving
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation failed 
 */
byte gauntlet_storage_position(void)
{
    #if DEBUG_ALL
        Serial.println("move gauntlet into storage position");
    #endif

    #if !MOCK_HARDWARE

        pwm_start(GAUNTLET_SERVO_PIN, SERVO_FREQ, SERVO_PERIOD, GAUNTLET_STORAGE_POS, 0);

    #endif

    return COMM_SUCCESS;
}

/** Place the gauntlet into the orientation for stone placement
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation failed
 */
byte gauntlet_open_position(void)
{
    #if DEBUG_ALL

        Serial.println("move gauntlet into open position");

    #endif

    #if !MOCK_HARDWARE

         pwm_start(GAUNTLET_SERVO_PIN, SERVO_FREQ, SERVO_PERIOD, GAUNTLET_OPEN_POS, 0);

    #endif

    return COMM_SUCCESS;
}

/** Place the gauntlet into the endgame orientation
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation failed
 */
byte gauntlet_deploy_position(void)
{

    #if DEBUG_ALL

        Serial.println("move gauntlet into deployed position");

    #endif

    #if !MOCK_HARDWARE
        
        pwm_start(GAUNTLET_SERVO_PIN, SERVO_FREQ, SERVO_PERIOD, GAUNTLET_DEPLOY_POS, 0);

    #endif

    return COMM_SUCCESS;
}

