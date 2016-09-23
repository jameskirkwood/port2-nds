#ifndef PORT2_H
#define PORT2_H

#include <nds.h>

// Module initialisation
extern void Port2_init();

// Register a listener function to be called each time a byte is received.
// Passing a null pointer causes all bytes sent by a Port2 peripheral to be
// ignored (by disabling slot-2 interrupts).
extern void Port2_listen(void (*listener)(u8));

// Send a single byte to any connected Port2 peripheral.
extern void Port2_send(u8 data);

#endif
