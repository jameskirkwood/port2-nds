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

#endif
