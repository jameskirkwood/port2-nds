#ifndef PORT2_H
#define PORT2_H

#include <nds.h>

// Module initialisation
extern void Port2_init();

// Attempt to communicate with an external "Port2" controller
// Returns the round trip response time of the controller (including the libnds
// interrupt dispatch latency) in bus clock ticks, if there is a response
// Returns -1 if there is no response
extern int Port2_ping();

// Register a listener function to be called each time a byte is received
// Passing a null pointer causes any bytes sent by an external "Port2"
// controller to be ignored (interrupts disabled)
extern void Port2_listen(void (*listener)(u8));

#endif
