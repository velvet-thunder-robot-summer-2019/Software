#include "DecisionMaking/Navigation.h"
#include "AllPurposeInclude.h"

int ramp_reached()
{
    return FALSE;
}

int move_to(location next_location)
{
    // each 'move_to' targeting a post ends when the back sensors detect the back of it
    if (run_status.bot_position.last_location == POST_1 && run_status.bot_position.next_location == POST_2 ||
        run_status.bot_position.last_location == POST_2 && run_status.bot_position.next_location == POST_1) {
        // i.e if we're between posts 1 and 2
        switch(next_location) {
            case POST_1:
                // turn around (pi) if last if POST_1
                // follow_tape till next intersection
                // update position to last = 1, next = METHANOS_INTERSECTION
                break;
            case POST_2:
                // turn around (pi) if last if POST_2
                // follow tape till next intersection
                // update position to last = 2, next = 3
                break;
            case POST_3:
                // call move_to(POST_2)
                // call move_to(POST_3), update position to last = 3, next = 4
                break;
            case POST_4:
                // call move_to(POST_3)
                // call move_to(POST_4)
                // update position to last = 4, next = 5
                break;
            case POST_5:
                // call move_to(POST_6)
                // call move_to(POST_5)
                // update position to last = 6, next = 5
                break;
            case POST_6:
                //rotate till you face "up"
                //cross country till you hit tape
                // turn to face right
                // update position to last = METHANOS_INTERSECTION, next = 6
                // call move_to(POST_6)
                break;
            case THANOS_INTERSECTION:
                // call move_to(POST_5)
                // call move_to(THANOS_INTERSECTION)
                break;
            case THANOS_GAUNTLET:
                // call move_to(THANOS_INTERSECTION), updates position to 
                // call move_to(THANOS_GAUNTLET)
                break;
            case METHANOS_INTERSECTION:
                // turn until you face up
                // cross country till you hit tape
                // turn to face left
                // update position to prev = 6, next = methanos intersection
                // call move_to(METHANOS_INTERSECTION)
            case METHANOS_GAUNTLET:
                // call move_to(METHANOS_INTERSECTION)
                // turn up branch, follow to gauntlet
                // update position
                break;
        }
    } else if (run_status.bot_position.last_location == POST_2 && run_status.bot_position.next_location == POST_3 ||
        run_status.bot_position.last_location == POST_3 && run_status.bot_position.next_location == POST_2) {
        // i.e if we're between posts 2 and 3
        switch(next_location) {
            case POST_1:
                // call move_to(POST_2), updates position: last = 2, next = 1
                // call move_to(POST_1);
                break;
            case POST_2:
                // turn around (pi) if last if POST_2
                // follow tape till next intersection
                // update position to last = 2, next = 1
                break;
            case POST_3:
                // turn around (pi) if last if POST_3
                // follow tape till next intersection
                // update position to last = 3, next = 4
                break;
            case POST_4:
                // call move_to(POST_3)
                // call move_to(POST_4)
                break;
            case POST_5:
                // rotate till you face "up" (positive y)
                // cross country till you hit tape
                // turn to face right
                // update position to last==6, next==5
                // call move_to(POST_5)
                break;
            case POST_6:
                //rotate till you face "up"
                //cross counTry till you hit tape
                // turn to face left
                // update position to last == 5, next == 6
                // call move_to(post_6)
                break;
            case THANOS_INTERSECTION:
                // move_to(POST_5);
                // move_to(THANOS_INTERSECTION);
                break;
            case THANOS_GAUNTLET:
                // move_to(THANOS_INTERSECTION);
                // move_to(THANOS_GAUNTLET);
                break;
            case METHANOS_INTERSECTION:
                // move_to(POST_6);
                // move_to(METHANOS_INTERSECTION);
            case METHANOS_GAUNTLET:
                // move_to(METHANOS_INTERSECTION);
                // move_to(METHANOS_GAUNTLET);
                break;
        }
    } else if (run_status.bot_position.last_location == POST_3 && run_status.bot_position.next_location == POST_4 ||
        run_status.bot_position.last_location == POST_4 && run_status.bot_position.next_location == POST_3) {
        // i.e if we're between posts 3 and 4
        switch(next_location) {
            case POST_1:
                // move_to(POST_2);
                // move_to(POST_1);
                break;
            case POST_2:
                // move_to(POST_3);
                // move_to(POST_2);
                break;
            case POST_3:
                // turn around (pi) if last if POST_3
                // follow tape till next intersection
                // update position to last = 3, next = 2
                break;
            case POST_4:
                // turn around (pi) if last if POST_4
                // follow tape till next intersection
                // update position to last = 4, next = 5
                break;
            case POST_5:
                // rotate till you face "up" (positive y)
                // cross country till you hit tape
                // turn to face left
                // update position to last = THANOS_INTERSECTION, next = 5
                // move_to(POST_5);
                break;
            case POST_6:
                // move_to(POST_5);
                // move_to(post_6)
                break;
            case THANOS_INTERSECTION:
                // turn till you face "up" (positive y)
                // cross country till you hit tape
                // turn to face right
                // update position to last = 5, next = THANOS_INTERSECTION
                // move_to(THANOS_INTERSECTION)
                break;
            case THANOS_GAUNTLET:
                // move_to(THANOS_INTERSECTION);
                // move_to(THANOS_GAUNTLET);
                break;
            case METHANOS_INTERSECTION:
                // rotate till you face "up" (positive y)
                // cross country till you hit tape
                // turn to face right
                // update position to last = 5, next = METHANOS_INTERSECTION
                // move_to(METHANOS_INTERSECTION);
                break;
            case METHANOS_GAUNTLET:
                // move_to(METHANOS_INTERSECTION);
                // call move_to(METHANOS_GAUNTLET);
                break;
        }
    } else if (run_status.bot_position.last_location == POST_1 && run_status.bot_position.next_location == METHANOS_INTERSECTION ||
        run_status.bot_position.last_location == METHANOS_INTERSECTION && run_status.bot_position.next_location == POST_3) {
        // i.e if we're between posts 3 and 4
        switch(next_location) {
            case POST_1:
                // turn around (pi) if last is POST_1
                // follow_tape till next intersection (that's the branch really)
                // turn left (i.e. towards neg y roughly)
                // follow tape till next intersection
                // update position
                break;
            case POST_2:
                // call move_to(POST_1)
                // update position
                // call move_to(POST_2)
                break;
            case POST_3:
                // call move_to(POST_2)
                // update position
                // call move_to(POST_3)
                break;
            case POST_4:
                // call move_to(POST_3)
                // update position
                // call move_to(POST_4)
                break;
            case POST_5:
                // call move_to(METHANOS_INTERSECTION)
                // hit interesection, turn to the right branch
                // update position, last == METHANOS_INTERSECTION, next == 6
                // call move_to(POST_5)
                break;
            case POST_6:
                // call move_to(METHANOS_INTERSECTION)
                // update position
                // call move_to(POST_6)
                // update position
                break;
            case THANOS_INTERSECTION:
                break;
            case THANOS_GAUNTLET:
                // call move_to(METHANOS_INTERSECTION)
                // update position
                // call move_to(POST_5)
                // update position
                break;
            case METHANOS_INTERSECTION:
                //
                break;
            case METHANOS_GAUNTLET:
                // turn until you face up
                // cross country till you hit tape
                // turn to face left
                // follow tape till next intersection
                // update position
                // call move_to(METHANOS_GAUNTLET)
                break;
        }
}

