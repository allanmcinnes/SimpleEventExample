#include "event_producer.h"
#include "event_queue.h"

#define ADC1 0x01
#define ADC2 0x02

// Maintain ADC state
static uint8_t current_adc1_level = DEFAULT_ADC_VALUE;
static uint8_t current_adc2_level = DEFAULT_ADC_VALUE;

// Test for GPIO transition-to-high interrupt
static inline bool is_high(uint8_t byte) {
  return (byte & GPIO_LEVEL_HIGH) != 0;
}

// Helper function for decoding button events
static inline void button_event(event_t* event, uint8_t gpio, button_t button) {
  event->type = EVENT_BUTTON_RELEASED;
  event->button = button;
  if (is_high(gpio)) {
    event->type = EVENT_BUTTON_PRESSED;
  }
}

// Helper function for decoding knob events
static inline void new_knob_event(knob_t knob, uint8_t level) {
  event_t event;
  int percent = (100 * level) / 255;
  event.type = EVENT_KNOB_SET;
  event.knob_set.knob = knob;
  event.knob_set.percent_of_max = percent;
  event_queue_append(&event);
}

void event_producer_init(void) {
  // Nothing
}

// Handle GPIOs
void event_producer_gpio_interrupt_handler(uint8_t gpio) {
  event_t new_event;
  switch (gpio & GPIO_ID_MASK) {
    case GPIO_PLAY: button_event(&new_event, gpio, BUTTON_PLAY); break;
    case GPIO_PAUSE: button_event(&new_event, gpio, BUTTON_PAUSE); break;
    case GPIO_STOP: button_event(&new_event, gpio, BUTTON_STOP); break;
    case GPIO_SWITCH_AUX:
      new_event.type = EVENT_SWITCH_SET;
      new_event.switch_set.switch_id = SWITCH_AUX;
      new_event.switch_set.on = is_high(gpio);
      break;
    default:
      break;
  }
  event_queue_append(&new_event);
}

// Handle ADC data when it's ready
void event_producer_adc_interrupt_handler(uint8_t adc, uint8_t adc_level) {
  switch (adc) {
    case ADC1:
      if (adc_level != current_adc1_level) {
        current_adc1_level = adc_level;
        new_knob_event(KNOB_VOLUME_LEFT, current_adc1_level);
      }
      break;
    case ADC2:
      if (adc_level != current_adc2_level) {
        current_adc2_level = adc_level;
        new_knob_event(KNOB_VOLUME_RIGHT, current_adc2_level);
      }
      break;
    default:
      break;
  }
}
