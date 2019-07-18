#ifndef POSITIONING_H_
#define POSITIONING_H_

#include "GlobalInfo/GlobalVars.h"

int ramp_reached();
void move_to(location next_location);
void update_position(location last_location, location next_location);

#endif // POSITIONING_H_