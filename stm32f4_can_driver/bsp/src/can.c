/*
 * can.c
 *
 *  Created on: Oct 9, 2025
 *      Author: dams09
 */

#include "can.h"
#include "main.h"
#include <string.h>

// --- Ring Buffer for CAN message reception ---
#define CAN_RX_BUFFER_SIZE 32
static struct can_msg rx_buffer[CAN_RX_BUFFER_SIZE];
static volatile uint32_t rx_head = 0;
static volatile uint32_t rx_tail = 0;
// --- Transmission Variables (now local to this file) ---
static CAN_TxHeaderTypeDef   TxHeader;
static uint32_t              TxMailbox;

// The HAL CAN handle is defined in main.c, so we declare it as extern.
extern CAN_HandleTypeDef hcan1;

/**
 * @brief Initializes the custom CAN driver layer.
 */
void can_init(void)
{
    // 1. Configure the CAN filter to accept all standard IDs.
    CAN_FilterTypeDef canfilterconfig;
    canfilterconfig.FilterActivation = CAN_FILTER_ENABLE;
    canfilterconfig.FilterBank = 0;
    canfilterconfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    canfilterconfig.FilterIdHigh = 0x0000;
    canfilterconfig.FilterIdLow = 0x0000;
    canfilterconfig.FilterMaskIdHigh = 0x0000;
    canfilterconfig.FilterMaskIdLow = 0x0000;
    canfilterconfig.FilterMode = CAN_FILTERMODE_IDMASK;
    canfilterconfig.FilterScale = CAN_FILTERSCALE_32BIT;
    canfilterconfig.SlaveStartFilterBank = 14;
    if (HAL_CAN_ConfigFilter(&hcan1, &canfilterconfig) != HAL_OK)
    {
      Error_Handler();
    }

    // 2. Start the CAN peripheral.
    if (HAL_CAN_Start(&hcan1) != HAL_OK)
    {
      Error_Handler();
    }

    // 3. Enable the CAN RX message pending interrupt.
    if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
    {
      Error_Handler();
    }
}

/**
 * @brief De-initializes the custom CAN driver layer.
 */
void can_fini(void)
{
    HAL_CAN_DeInit(&hcan1);
}

/**
 * @brief Receives up to n CAN messages from the receive buffer.
 * @param ptr   A pointer to an array of can_msg structs to be filled.
 * @param n     The maximum number of messages to receive.
 * @return      The number of messages actually received.
 */
size_t can_recv(struct can_msg *ptr, size_t n)
{
    size_t count = 0;
    uint32_t current_tail = rx_tail;

    while (count < n && current_tail != rx_head)
    {
        memcpy(ptr, &rx_buffer[current_tail], sizeof(struct can_msg));
        current_tail = (current_tail + 1) % CAN_RX_BUFFER_SIZE;
        ptr++;
        count++;
    }

    rx_tail = current_tail;
    return count;
}

/**
 * @brief Sends up to n CAN messages to the bus.
 * @param ptr   A pointer to an array of can_msg structs to be sent.
 * @param n     The number of messages to send.
 * @return      The number of messages successfully queued for transmission.
 */
size_t can_send(const struct can_msg *ptr, size_t n)
{
	if (n == 0 || ptr == NULL) {
	        return 0;
	    }
	    TxHeader.StdId = ptr->id;
	    TxHeader.ExtId = 0;
	    TxHeader.RTR = CAN_RTR_DATA;
	    TxHeader.IDE = CAN_ID_STD;
	    TxHeader.DLC = ptr->len;
	    TxHeader.TransmitGlobalTime = DISABLE;
	    while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0);
	    if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, (uint8_t*)ptr->data, &TxMailbox) == HAL_OK) {
	        return 1;
	    }
	    return 0;
}

/**
 * @brief This is the interrupt callback function executed when a message arrives.
 *        It is called by the HAL driver.
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];

    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data) == HAL_OK)
    {
        uint32_t next_head = (rx_head + 1) % CAN_RX_BUFFER_SIZE;
        if (next_head != rx_tail)
        {
            rx_buffer[rx_head].id = rx_header.StdId;
            rx_buffer[rx_head].len = rx_header.DLC;
            rx_buffer[rx_head].flags = 0;
            memcpy(rx_buffer[rx_head].data, rx_data, rx_header.DLC);
            rx_head = next_head;
        }
    }
}
