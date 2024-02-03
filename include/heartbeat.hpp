#ifndef HEARTBEAT_H
#define HEARTBEAT_H

typedef enum {
    CONN_NONE = 0,
    CONN_LOST,
    CONN_PRESENT
} conn_state_t;

#define MAX_MISSES 5;               //Num of consecutive misses definining lost connection
#define HEARTBEAT_TASK_DELAY_MS 50; //Num of ms to wait before counting a loss

void heartbeat(void * param);

#endif