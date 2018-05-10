// Simple FIFO queue of events
/*
A few things worth noting about this queue implementation:

- The append operation returns false if there's no room in the queue. This makes the append non-blocking, 
  but means that it's up to the caller to ensure an event actually gets into the queue.
  
- The queue implementation requires a caller to allocate and populate an event locally before it
  is copied into the queue during the append operation. An alternative would be to make the queue
  entirely responsible for manmaging event memory (making it a kind of memory pool). But trying to
  keep track of events that have been allocated and not yet enqueued would make for a more
  complex queue implementation. I opted to keep it simple here.

- Retrieving a value from the queue is split into two operations. The `getNextEvent` operation 
  returns a pointer to an event, but doesn't change the head of the queue (so multiple calls to 
  `getNextEvent` access the same value). The `consumeNextEvent` operation removes the head of 
  the queue. This split in operations is partly a design choice based on the Command-Query 
  Separation Principle, and partly a way to minimize copying.
 
- The queue is not thread-safe. To make it suitable for use in concurrent systems (or from an
   interrupt context) you will need to add some locks or calls to disable interrupts to
   the functions in the public interface.
*/

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
