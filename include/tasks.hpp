#ifndef TASKS_H
#define TASKS_H

#include <Arduino.h>
#include "XBEE_Rx.hpp"
#include "XBEE_Tx.hpp"
#include "startup.hpp"
#include "acc.hpp"
#include "burnwire.hpp"
#include "heartbeat.hpp"

void startTasks();

#endif