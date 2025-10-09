/*
 * can.h
 *
 *  Created on: Oct 9, 2025
 *      Author: dams09
 */

#ifndef BSP_CAN_H_
#define BSP_CAN_H_

#include <lely/can/msg.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the CAN peripheral, filters, and interrupts.
 */
void can_init(void);

/**
 * @brief De-initializes the CAN peripheral.
 */
void can_fini(void);

/**
 * @brief Attempts to receive up to 'n' CAN messages from the software buffer.
 * @param ptr A pointer to an array of can_msg structs to be filled.
 * @param n   The maximum number of messages to retrieve.
 * @return The number of messages actually retrieved from the buffer.
 */
size_t can_recv(struct can_msg *ptr, size_t n);

/**
 * @brief Queues up to 'n' CAN messages for transmission on the bus.
 * @param ptr A pointer to an array of can_msg structs to be sent.
 * @param n   The number of messages to attempt to queue.
 * @return The number of messages successfully queued for transmission.
 */
size_t can_send(const struct can_msg *ptr, size_t n);

#ifdef __cplusplus
}
#endif

#endif // !BSP_CAN_H_
