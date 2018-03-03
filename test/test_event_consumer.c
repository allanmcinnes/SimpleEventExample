#include "unity.h"
#include "event_consumer.h"

void setUp(void)
{
  event_consumer_init();
}

void tearDown(void)
{
}

event_t test_event_stop_press = { .type = EVENT_BUTTON_PRESSED, .button = BUTTON_STOP };
event_t test_event_stop_release = { .type = EVENT_BUTTON_RELEASED, .button = BUTTON_STOP };
event_t test_event_pause_press = { .type = EVENT_BUTTON_PRESSED, .button = BUTTON_PAUSE };
event_t test_event_pause_release = { .type = EVENT_BUTTON_RELEASED, .button = BUTTON_PAUSE };
event_t test_event_play_press = { .type = EVENT_BUTTON_PRESSED, .button = BUTTON_PLAY };
event_t test_event_play_release = { .type = EVENT_BUTTON_RELEASED, .button = BUTTON_PLAY };


void test_event_consumer_GivenStoppedState_IgnoresStopAndPauseButtons(void) {
  event_consumer_sendEvent(&test_event_stop_press);
  TEST_ASSERT(event_consumer_currentPlayState() == STATE_STOPPED);
  event_consumer_sendEvent(&test_event_stop_release);
  TEST_ASSERT(event_consumer_currentPlayState() == STATE_STOPPED);
  event_consumer_sendEvent(&test_event_pause_press);
  TEST_ASSERT(event_consumer_currentPlayState() == STATE_STOPPED);
  event_consumer_sendEvent(&test_event_pause_release);
  TEST_ASSERT(event_consumer_currentPlayState() == STATE_STOPPED);
}

void test_event_consumer_WhenPlayPressed_SystemStartsPlaying(void) {
  event_consumer_sendEvent(&test_event_play_press);
  TEST_ASSERT(event_consumer_currentPlayState() == STATE_PLAYING);
  event_consumer_sendEvent(&test_event_play_release);
  TEST_ASSERT(event_consumer_currentPlayState() == STATE_PLAYING);
}

void test_event_consumer_GivenPlaying_WhenPausePressed_SystemPauses(void) {
  event_consumer_sendEvent(&test_event_play_press);
  event_consumer_sendEvent(&test_event_pause_press);
  TEST_ASSERT(event_consumer_currentPlayState() == STATE_PAUSED);
}
