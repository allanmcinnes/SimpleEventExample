#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "event_consumer.h"

// This might be a more complex state machine.

static struct {
  play_state_t play_state;
  int volume_left;
  int volume_right;
  bool aux_output;
} system_state;

static void update_knob(event_knob_set_t* knob_event);
static void update_play_state(button_t button, bool is_pressed);

void event_consumer_init(void) {
  system_state.play_state = STATE_STOPPED;
  system_state.volume_left = 50;
  system_state.volume_right = 50;
  system_state.aux_output = false;
}

void event_consumer_sendEvent(event_t* event)
{
  switch (event->type)
  {
    case EVENT_BUTTON_PRESSED:
    {
      update_play_state(event->button, true);
      break;
    }
    case EVENT_BUTTON_RELEASED:
    {
      update_play_state(event->button, false);
      break;
    }
    case EVENT_KNOB_SET:
    {
      event_knob_set_t e = event->knob_set;
      update_knob(&e);
      break;
    }
    case EVENT_SWITCH_SET:
    {
      event_switch_set_t e = event->switch_set;
      system_state.aux_output = e.on;
      break;
    }
    default:
      break;
  }
}


void event_consumer_stateToString(char* stringbuf, size_t bufsize)
{
  int len = 0;

  len += snprintf((stringbuf + len), bufsize, "System state:\n");
  len += snprintf((stringbuf + len), bufsize, "\tPlay state = ");
  switch(system_state.play_state) {
    case STATE_PAUSED:
      len += snprintf((stringbuf + len), bufsize, "Paused\n");
      break;
    case STATE_PLAYING:
      len += snprintf((stringbuf + len), bufsize, "Playing\n");
      break;
    case STATE_STOPPED:
      len += snprintf((stringbuf + len), bufsize, "Stopped\n");
      break;
  }
  len += snprintf((stringbuf + len), bufsize,
    "\tVolume left = %d, Volume right = %d\n",
    system_state.volume_left,
    system_state.volume_right);
  len += snprintf((stringbuf + len), bufsize,
    "\t Aux Output = %s\n",
      system_state.aux_output ? "On" : "Off");
}

play_state_t event_consumer_currentPlayState(void) {
  return system_state.play_state;
}

int event_consumer_getVolumeLeft(void) {
  return system_state.volume_left;
}

int event_consumer_getVolumeRight(void) {
  return system_state.volume_right;
}

bool event_consumer_isAuxOn(void) {
  return system_state.aux_output;
}

static void update_knob(event_knob_set_t* knob_event)
{
  switch(knob_event->knob) {
    case KNOB_VOLUME_LEFT:
      system_state.volume_left = knob_event->percent_of_max;
      break;
    case KNOB_VOLUME_RIGHT:
      system_state.volume_right = knob_event->percent_of_max;
      break;
    default:
      break;
  }
}


// Could be broken up into state-handler functions
static void update_play_state(button_t button, bool is_pressed) {
    switch(system_state.play_state) {
      case STATE_PLAYING:
        switch(button) {
          case BUTTON_STOP:
            if (is_pressed) {
              system_state.play_state = STATE_STOPPED;
            }
            break;
          case BUTTON_PAUSE:
            if (is_pressed) {
              system_state.play_state = STATE_PAUSED;
            }
            break;
          default:
            // Do nothing
            break;
        }
        break;
      case STATE_PAUSED:
        switch(button) {
          case BUTTON_PAUSE:
            if (!is_pressed) {
              system_state.play_state = STATE_PLAYING;
            }
            break;
          default:
              // Do nothing
              break;
        }
        break;
      case STATE_STOPPED:
        switch(button) {
          case BUTTON_PLAY:
            if (is_pressed) {
              system_state.play_state = STATE_PLAYING;
            }
            break;
          default:
            // Do nothing
            break;
        }
        break;
      default:
        break;
    }
}
