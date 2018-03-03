#include "unity.h"
#include "event_producer.h"
#include "event_queue.h"

void setUp(void)
{
  event_queue_init();
}

void tearDown(void)
{
}

void test_event_producer_WhenGPIOInterruptPlayPressed_EnqueuesPlayPressEvent(void)
{
    event_producer_gpio_interrupt_handler(GPIO_PLAY | GPIO_LEVEL_HIGH);

    TEST_ASSERT(!event_queue_isEmpty());
    event_t* event = event_queue_getNextEvent();
    TEST_ASSERT(event->type == EVENT_BUTTON_PRESSED);
    TEST_ASSERT(event->button == BUTTON_PLAY);
}

void test_event_producer_WhenGPIOInterruptPlayReleased_EnqueuesPlayReleasedEvent(void)
{
    event_producer_gpio_interrupt_handler(GPIO_PLAY);

    TEST_ASSERT(!event_queue_isEmpty());
    event_t* event = event_queue_getNextEvent();
    TEST_ASSERT(event->type == EVENT_BUTTON_RELEASED);
    TEST_ASSERT(event->button == BUTTON_PLAY);
}

void test_event_producer_WhenMultipleGPIOInterrupts_EnqueuesMultipleEventsInInterruptOrder(void)
{
    event_producer_gpio_interrupt_handler(GPIO_PLAY | GPIO_LEVEL_HIGH);
    event_producer_gpio_interrupt_handler(GPIO_SWITCH_AUX );

    event_t* event;
    TEST_ASSERT(!event_queue_isEmpty());
    event = event_queue_getNextEvent();
    TEST_ASSERT(event->type == EVENT_BUTTON_PRESSED);
    TEST_ASSERT(event->button == BUTTON_PLAY);
    event_queue_consumeNextEvent();

    event = event_queue_getNextEvent();
    TEST_ASSERT(event->type == EVENT_SWITCH_SET);
    TEST_ASSERT(event->switch_set.switch_id == SWITCH_AUX);
    TEST_ASSERT(event->switch_set.on == false);
}

void test_event_producer_WhenADCLevelIsConstant_DoesNotGenerateEvents(void)
{
  event_producer_adc_interrupt_handler(1, DEFAULT_ADC_VALUE);
  event_producer_adc_interrupt_handler(2, DEFAULT_ADC_VALUE);
  event_producer_adc_interrupt_handler(1, DEFAULT_ADC_VALUE);
  event_producer_adc_interrupt_handler(2, DEFAULT_ADC_VALUE);

  TEST_ASSERT(event_queue_isEmpty());
}


void test_event_producer_WhenADCLevelChanges_GeneratesVolumeEvent(void)
{
  event_producer_adc_interrupt_handler(1, DEFAULT_ADC_VALUE);
  event_producer_adc_interrupt_handler(2, DEFAULT_ADC_VALUE);
  event_producer_adc_interrupt_handler(1, DEFAULT_ADC_VALUE + 100);
  event_producer_adc_interrupt_handler(2, DEFAULT_ADC_VALUE);

  event_t* event;
  TEST_ASSERT(!event_queue_isEmpty());
  event = event_queue_getNextEvent();
  TEST_ASSERT(event->type == EVENT_KNOB_SET);
  TEST_ASSERT(event->knob_set.knob == KNOB_VOLUME_LEFT);
  TEST_ASSERT(event->knob_set.percent_of_max == 78);
  event_queue_consumeNextEvent();
  TEST_ASSERT(event_queue_isEmpty());
}
