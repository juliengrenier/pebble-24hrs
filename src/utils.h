#pragma once

#include "pebble_os.h"

/***************************
 *
 * if clock_angle then we apply a -270 rotation to the angle to match the clock movement.
 * Return the nth angle
 *
 ***************************/
int32_t get_nth_angle(int32_t nth, int32_t number_of_section, bool clock_angle);


/***************************
 * 
 * covert polar coordinate to cartesian point and transpose based on the origin
 *
 ***************************/
GPoint polar_to_cartesian(int32_t radius, int32_t angle, GPoint origin); 
