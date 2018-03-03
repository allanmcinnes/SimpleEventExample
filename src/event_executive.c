#include <stdbool.h>
#include "event_executive.h"
#include "event_queue.h"

#define MAX_EVENT_CONSUMERS 3

static consumer_t registered_consumers[MAX_EVENT_CONSUMERS];
static int registered_consumer_count = 0;

void event_executive_init(void) {
  registered_consumer_count = 0;
}

void event_executive_registerConsumer(consumer_t consumer) {
  if (registered_consumer_count < MAX_EVENT_CONSUMERS) {
    registered_consumers[registered_consumer_count++] = consumer;
  }
}

void event_executive_run(void) {
  while (!event_queue_isEmpty()) {
    for (int i = 0; i < registered_consumer_count; i++) {
      registered_consumers[i](event_queue_getNextEvent());
    }
    event_queue_consumeNextEvent();
  }
}
