#ifndef GAUNTLET_CONTROLLER_H_
#define GAUNTLET_CONTROLLER_H_

/** 
 * This module positions the gauntlet to different positions
 */

#include "Arduino.h"
#include "GlobalInfo/HardwareDefs.h"

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
byte gauntlet_deploy_position(void);

#endif