#include "unity.h"
#include "event_executive.h"
#include "event_queue.h"

#define LOG_SIZE EVENT_QUEUE_SIZE

typedef struct {
  event_type_t events[LOG_SIZE];
  int index;
} consumer_log_t;

static consumer_log_t consumer_A_log;
static consumer_log_t consumer_B_log;

static void consumer_log_init(consumer_log_t* log) {
  for (int i = 0; i < LOG_SIZE; i++) {
    log->events[i] = 0;
  }
  log->index = 0;
}

static void add_events_to_queue(event_t events[], int length) {
  for (int i = 0; i < length; i++) {
    event_queue_append(&events[i]);
  }
}

static void consumer_log_verify(event_t events[], consumer_log_t* log, int length) {
  for (int i = 0; i < length; i++) {
    TEST_ASSERT(events[i].type == log->events[i]);
  }
}

static void failing_consumer(event_t* event) {
  TEST_FAIL();
}

static void working_consumer_A(event_t* event) {
  consumer_A_log.events[consumer_A_log.index++] = event->type;
}

static void working_consumer_B(event_t* event) {
  consumer_B_log.events[consumer_B_log.index++] = event->type;
}

void setUp(void)
{
  event_queue_init();
  event_executive_init();
  consumer_log_init(&consumer_A_log);
  consumer_log_init(&consumer_B_log);
}

void tearDown(void)
{
}

void test_event_executive_GivenEmptyQueue_DoesNothing(void)
{
  event_executive_registerConsumer(failing_consumer);
  event_executive_run();
}

void test_event_executive_GivenEnqueuedEvents_EmptiesQueue(void)
{
  event_t test_event;
  for (int i = 0; i < EVENT_QUEUE_SIZE; i++) {
    event_queue_append(&test_event);
  }
  event_executive_run();
  TEST_ASSERT(event_queue_isEmpty());
}

void test_event_executive_GivenEnqueuedEventsAndRegisteredConsumer_DispatchesEventsToConsumer(void)
{
  event_t test_events[] = {
    { .type = EVENT_BUTTON_PRESSED },
    { .type = EVENT_BUTTON_RELEASED },
    { .type = EVENT_KNOB_SET },
    { .type = EVENT_SWITCH_SET },
    { .type = EVENT_BUTTON_RELEASED },
  };

  // Given
  event_executive_registerConsumer(working_consumer_A);
  add_events_to_queue(test_events, sizeof(test_events)/sizeof(event_t));

  // When
  event_executive_run();

  // Then
  TEST_ASSERT(event_queue_isEmpty());
  consumer_log_verify(test_events, &consumer_A_log, sizeof(test_events)/sizeof(event_t));
}

void test_event_executive_GivenMultipleRegisteredConsumers_DispatchesToAllConsumers(void)
{
  event_t test_events[] = {
    { .type = EVENT_BUTTON_PRESSED },
    { .type = EVENT_BUTTON_RELEASED },
    { .type = EVENT_KNOB_SET },
    { .type = EVENT_SWITCH_SET },
    { .type = EVENT_BUTTON_RELEASED },
  };

  // Given
  event_executive_registerConsumer(working_consumer_A);
  event_executive_registerConsumer(working_consumer_B);
  add_events_to_queue(test_events, sizeof(test_events)/sizeof(event_t));

  // When
  event_executive_run();

  // Then
  TEST_ASSERT(event_queue_isEmpty());
  consumer_log_verify(test_events, &consumer_A_log, sizeof(test_events)/sizeof(event_t));
  consumer_log_verify(test_events, &consumer_B_log, sizeof(test_events)/sizeof(event_t));  
}
