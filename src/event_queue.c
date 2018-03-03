
#include <stdint.h>
#include <string.h>
#include "event_queue.h"

// Note 1: Implementation not thread-safe! Add interrupt enable/disable or
// mutexes to allow use from interrupt contexts

// Note 2: Getting an element from the queue returns a pointer to "front"
// that remains valid until the front element is removed. This keeps the
// implementation simpler and saves on memory. But it does mean that
// you need to be careful not to retain pointers to events after they have
// been "consumed".

// The event queue is just a simple ringbuffer
static struct {
  uint32_t head;
  uint32_t tail;
  event_t buffer[EVENT_QUEUE_SIZE + 1]; // Need one extra element to act as a sentinel for empty queue
} queue;

// Determine the index of the next element based on a current index
static inline uint32_t nextElementIndex(uint32_t current_index) {
  return ((current_index + 1) % (sizeof(queue.buffer)/sizeof(event_t)));
}

// Empty if head and tail are the same
static inline bool isEmpty(void) {
  return (queue.head == queue.tail);
}

// Full if the head would advance into the tail
static inline bool isFull() {
  return (nextElementIndex(queue.head)  == queue.tail);
}

void event_queue_init(void) {
  queue.head = 0;
  queue.tail = 0;
}

bool event_queue_isEmpty(void) {
  return isEmpty();
}

bool event_queue_append(event_t* event) {
  bool can_append = !isFull();
  if (can_append) {
    queue.head = nextElementIndex(queue.head);
    memcpy(&(queue.buffer[queue.head]), event, sizeof(event_t));
  }
  return can_append;
}

event_t* event_queue_getNextEvent(void) {
  event_t* event = NULL;
  if (!isEmpty()) {
    event = &queue.buffer[nextElementIndex(queue.tail)];
  }
  return event;
}

void event_queue_consumeNextEvent(void) {
  if (!isEmpty()) {
    queue.tail = nextElementIndex(queue.tail);
  }
}
