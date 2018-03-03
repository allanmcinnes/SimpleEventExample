// A basic system that demonstrates gluing together all of the modules
//
// The MIT License (MIT)
//
// Copyright (c) 2018 Allan McInnes
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <stdio.h>
#include <stdlib.h>
#include "event_queue.h"
#include "event_executive.h"
#include "event_producer.h"
#include "event_consumer.h"

int main(int argc, char* argv) {

  char outbuf[256];

  printf("Simple Event Producer/Consumer Example\n");
  printf("--------------------------------------\n");
  printf("Initializing event system\n");
  event_queue_init();
  event_producer_init();
  event_consumer_init();
  event_executive_init();
  event_executive_registerConsumer(event_consumer_sendEvent);
  event_consumer_stateToString(&outbuf[0], sizeof(outbuf));
  printf("%s\n", outbuf);

  printf("Simulating interrupts\n");
  event_producer_gpio_interrupt_handler(GPIO_LEVEL_HIGH | GPIO_PLAY);
  event_producer_adc_interrupt_handler(1, 50);
  event_producer_gpio_interrupt_handler(GPIO_LEVEL_HIGH | GPIO_SWITCH_AUX);

  printf("Running executive\n");
  event_executive_run(); // In  practice, this would be in a while(1) loop
  event_consumer_stateToString(&outbuf[0], sizeof(outbuf));
  printf("%s\n", outbuf);

  exit(0);
}
