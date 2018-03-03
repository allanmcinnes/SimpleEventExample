#ifndef _EVENT_CONSUMER_H
#define _EVENT_CONSUMER_H

#include <stdbool.h>
#include "events.h"

// Sample event consumer. In this case, a simple state machine, plus some
// other simple state handling.
typedef enum {
  STATE_STOPPED,
  STATE_PLAYING,
  STATE_PAUSED,
} play_state_t;

void event_consumer_init(void);
void event_consumer_sendEvent(event_t* event);
void event_consumer_stateToString(char* stringbuf, size_t bufsize);
play_state_t event_consumer_currentPlayState(void);
int event_consumer_getVolumeLeft(void);
int event_consumer_getVolumeRight(void);
bool event_consumer_isAuxOn(void);

#endif // _EVENT_CONSUMER_H
