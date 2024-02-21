#ifndef BURNWIRE_H
#define BURNWIRE_H

#include "main.hpp"
#include "tasks.hpp"
#include "driver/gpio.h"
#include "pindefs.hpp"

static const char *BURN_TAG = "Burn";

void burn_manager(void * param);

#endif