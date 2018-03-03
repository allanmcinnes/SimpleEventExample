// Events used in this system. Based on the sample code
// at http://www.electronvector.com/blog/event-based-interfaces-for-testability,
// although I used a tagged union because it simplified the event queue code.



#ifndef _EVENTS_H
#define _EVENTS_H

#include <stdbool.h>

// Event tags
typedef enum {
  EVENT_NULL = 0,
  EVENT_BUTTON_PRESSED,
  EVENT_BUTTON_RELEASED,
  EVENT_KNOB_SET,
  EVENT_SWITCH_SET,
} event_type_t;

// --- Event data ---

typedef enum { BUTTON_PLAY, BUTTON_PAUSE, BUTTON_STOP } button_t;
typedef enum { KNOB_VOLUME_LEFT, KNOB_VOLUME_RIGHT } knob_t;
typedef enum { SWITCH_AUX } switch_t;

typedef struct {
  knob_t knob;
  int percent_of_max;
} event_knob_set_t;

typedef struct {
  switch_t switch_id; // Can't use 'switch' since it's a reserved keyword in C
  bool on;
} event_switch_set_t;

// --- Event type ---
typedef struct {
  event_type_t type;
  union {
    button_t button;
    event_knob_set_t knob_set;
    event_switch_set_t switch_set;
  };
} event_t;



#endif // _EVENTS_H
