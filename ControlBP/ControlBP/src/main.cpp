#include <Arduino.h>
#include "Initialisation/HardwareDefs.h"
#include "Initialisation/State.h"

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



void setup() {
  // put your setup code here, to run once:
  // Initialisation of everything here
  if (digitalRead(MASTER_SWITCH) == COMP) {
    bot_state = REACH_RAMP;
    initialise_global_variables();
  } else {
    bot_state = MENU;
  }
}

void loop() {
  // MAIN CONTROL LOOP
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
}