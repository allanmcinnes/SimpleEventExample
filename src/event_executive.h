// Dispatches events from an event queue to registered event consumers
#ifndef _EVENT_EXECUTIVE_H
#define _EVENT_EXECUTIVE_H

#include "events.h"

// Consumer functions are the target of event dispatch
typedef void (*consumer_t)(event_t*);

void event_executive_init(void);

// Add a consumer
void event_executive_registerConsumer(consumer_t consumer);

// Dispatch all events currently in the queue. Would typically
// be called from `main()`:
// ```
// int main() {
//   while(1) {
//      event_executive_run();
//      sleep_processor_until_interrupt();
//   }
// }
// ```
void event_executive_run(void);

#endif // _EVENT_EXECUTIVE_H
