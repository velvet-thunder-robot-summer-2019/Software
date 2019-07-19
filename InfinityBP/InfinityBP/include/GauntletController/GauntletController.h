/** 
 * This module positions the gauntlet to different positions
 */

#include "Arduino.h"

#define GAUNTLET_SERVO_PIN 0

#define GAUNTLET_STORAGE_POS 0
#define GAUNTLET_OPEN_POS 0
#define GAUNTLET_DEPLOY_POS 0

/** Initialize the gauntlet
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation failed
 */
byte init_gauntlet(void);

/** Place the gauntlet into the storage orientation for driving
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation failed 
 */
byte gauntlet_storage_position(void);

/** Place the gauntlet into the orientation for stone placement
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation failed
 */
byte gauntlet_open_position(void);

/** Place the gauntlet into the endgame orientation
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation failed
 */
byte gauntlet_deploy_position(void)