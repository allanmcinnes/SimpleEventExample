// Simple FIFO queue of events

#ifndef _EVENT_QUEUE_H
#define _EVENT_QUEUE_H

#include <stdbool.h>

#include "events.h"

// Maximum number of events in the queue
#define EVENT_QUEUE_SIZE 5

// Ready the event queue for use
void event_queue_init(void);

// Returns true if event queue is empty
bool event_queue_isEmpty(void);

// Adds an event to the queue
// Returns false if the queue is full
bool event_queue_append(event_t* event);

// Gets the event at the front of the queue
event_t* event_queue_getNextEvent(void);

// Removes the from of the queue
void event_queue_consumeNextEvent(void);

#endif // _EVENT_QUEUE_H
