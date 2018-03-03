#ifndef _EVENT_PRODUCER_H
#define _EVENT_PRODUCER_H

#include <stdint.h>

#define GPIO_PLAY 0x01
#define GPIO_PAUSE 0x02
#define GPIO_STOP 0x04
#define GPIO_SWITCH_AUX 0x08
#define GPIO_ID_MASK 0X0F
#define GPIO_LEVEL_HIGH 0xF0

#define DEFAULT_ADC_VALUE 100

void event_producer_init(void);
void event_producer_gpio_interrupt_handler(uint8_t gpio);
void event_producer_adc_interrupt_handler(uint8_t adc, uint8_t adc_level);

#endif // _EVENT_PRODUCER_H
