#ifndef PORT2_H
#define PORT2_H

// Module initialisation
extern void Port2_init();

// Attempt to interrupt a Port2 peripheral and wait for an interrupt in return.
// Returns the round trip response time of the peripheral (including the libnds
// interrupt dispatch latency) in bus clock ticks, if there is a response.
// Returns -1 if there is no response.
extern int Port2_echo();

#endif
