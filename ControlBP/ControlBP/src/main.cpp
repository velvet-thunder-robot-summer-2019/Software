#include <Arduino.h>
#include "AllPurposeInclude.h"
#include "GlobalInfo/Interrupts.h"

#include "Debugging/Debug.h"
#include "Debugging/Menu.h"

#include "StateControl/ReachRamp.h"
#include "StateControl/AscendRamp.h"
#include "StateControl/Calibrate.h"
#include "StateControl/FindPost.h"
#include "StateControl/GetInfinityStone.h"
#include "StateControl/ReturnToGauntlet.h"
#include "StateControl/FitToGauntlet.h"
#include "StateControl/HandleCollision.h"
#include "StateControl/GoodnightSweetPrince.h"

#include "Communication/ControlCommunication.h"

void reset();

/**
 * Calls all initialisation routines (vary between DEV and COMP modes)
 * Calls initialise_global_variables
 * Sets starting state
 * TODO: Calls initialise_interrupt, call THANOS v METHANOS inits, send init_command to infinity
 * 
 */
void setup() {
  // put your setup code here, to run once:
  // Initialisation of everything here
  delay(3000);
  Serial.begin(9600);
  Serial.println("begin setup");
}

void loop() {
  // MAIN CONTROL LOOP
  reset();
  Serial.println("Begin control loop");
  state bot_state = robot_state();  
  switch(bot_state) {
    case MENU :
      menu();
      break;

    case DEBUG :
      debug();
      break;
    
    case REACH_RAMP :
      reach_ramp();
      break;
    
    case ASCEND_RAMP :
      ascend_ramp();
      break;

    case CALIBRATE :
      calibrate();
      break;

    case FIND_POST :
      find_post();
      break;
    
    case GET_INFINITY_STONE :
      get_infinity_stone();
      break;
    
    case RETURN_TO_GAUNTLET :
      return_to_gauntlet();
      break;
    
    case FIT_TO_GAUNTLET :
      fit_to_gauntlet();
      break;
    
    case HANDLE_COLLISION :
      handle_collision();
      break;

    case GOODNIGHT_SWEET_PRINCE :
      goodnight_sweet_prince();
      break;
  }
  
  delay(1000); // just for now, so that Julie can see what went down
}

void reset() 
{
  Serial.println(digitalRead(MASTER_SWITCH) == DEV);
  Serial.println(digitalRead(MASTER_SWITCH) == COMP);

  if (digitalRead(MASTER_SWITCH) == COMP) {
    Serial.println("COMP MODE");
    initialise_competition_data();
    init_robot_state(REACH_RAMP);
  } else if (digitalRead(MASTER_SWITCH) == DEV) {
    Serial.println("DEV MODE");
    init_robot_state(MENU);
  }
  init_menu();
  init_tape_following();
  init_interrupts();
  init_communication();
}