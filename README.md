# Simple Events Example

A very basic example of using events and event queues to decouple modules in
embedded systems. This example is based on the code presented by Matt Chernosky in http://www.electronvector.com/blog/event-based-interfaces-for-testability, but extends it
into a complete executable example with unit tests.

The interesting thing about this example that *none* of the unit tests require
any *mocks* to execute (well, mostly--the tests for the executive include some fake
functions that are something like mocks). The need for mocks is a sign that you
have a tight coupling between modules. Here, the event queue breaks that coupling.
It also gives you a uniform way to manage interrupts, handle events from multiple sources,
and to implement a system that sleeps between interrupts. The tradeoff is that
you have a somewhat more complex system than you'd get with direct function calls,
and you lose some static type-safety guarantees.

The ideas illustrated here are similar in principle to the [Quantum Platform](https://www.state-machine.com/), and
also to the [TinyOS](https://github.com/tinyos/tinyos-main) task management system. 

This is not production-ready code (in particular, it doesn't include concurrency
protection--such as interrupt disabling or mutexes--around queue actions). But
you could easily build a production-ready system from this starting point.

## Dependencies

Uses the [Ceedling](https://github.com/ThrowTheSwitch/Ceedling) suite for unit testing.
