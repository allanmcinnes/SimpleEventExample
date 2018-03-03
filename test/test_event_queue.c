#include "unity.h"
#include "event_queue.h"


static void add_events_to_queue(event_t events[], int length) {
  for (int i = 0; i < length; i++) {
    event_queue_append(&events[i]);
  }
}

void setUp(void)
{
  event_queue_init();
}

void tearDown(void)
{
}

void test_event_queue_GivenEmptyQueue_IsEmptyIsTrue(void)
{
  TEST_ASSERT(event_queue_isEmpty());
}

void test_event_queue_GivenEmptyQueue_NextEventIsNull(void)
{
  TEST_ASSERT_NULL(event_queue_getNextEvent());
}

void test_event_queue_WhenEventIsAppended_IsEmptyIsFalse(void)
{
  event_t test_event;
  event_queue_append(&test_event);
  TEST_ASSERT_FALSE(event_queue_isEmpty());
}

void test_event_queue_WhenEventIsAppended_AppendReturnsTrue(void)
{
  event_t test_event;
  TEST_ASSERT(event_queue_append(&test_event));
}

void test_event_queue_WhenEventsAreAppended_TheyAreConsumedInAppendededOrder(void)
{
  event_t test_events[] = {
    { .type = EVENT_BUTTON_PRESSED, .button = BUTTON_PLAY },
    { .type = EVENT_BUTTON_RELEASED, .button = BUTTON_PLAY  },
    { .type = EVENT_KNOB_SET, .knob_set = { .knob = KNOB_VOLUME_LEFT, .percent_of_max = 80 } },
    { .type = EVENT_SWITCH_SET, .switch_set = { .switch_id = SWITCH_AUX, true } },
    { .type = EVENT_BUTTON_RELEASED, .button = BUTTON_PLAY },
  };

  add_events_to_queue(test_events, sizeof(test_events)/sizeof(event_t));

  // Verify
  for (int i = 0; i < sizeof(test_events)/sizeof(event_t); i++) {
    TEST_ASSERT_EQUAL_MEMORY(&test_events[i], event_queue_getNextEvent(), sizeof(event_t));
    event_queue_consumeNextEvent();
  }
}

void test_event_queue_GivenFullQueue_AppendOperationReturnsFalse(void)
{
  event_t test_events[EVENT_QUEUE_SIZE];
  event_t test_event;
  add_events_to_queue(test_events, sizeof(test_events)/sizeof(event_t));
  TEST_ASSERT_FALSE(event_queue_append(&test_event));
}
