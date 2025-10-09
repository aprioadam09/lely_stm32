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

// The HAL CAN handle is defined in main.c, so we declare it as extern.
extern CAN_HandleTypeDef hcan1;

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

void can_fini(void)
{
    HAL_CAN_DeInit(&hcan1);
}

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

// NOTE: This function is left empty for this commit.
size_t can_send(const struct can_msg *ptr, size_t n)
{
    (void)ptr;
    (void)n;
    return 0;
}

// This HAL callback is automatically called by the HAL_CAN_IRQHandler
// when a message is pending in FIFO0.
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
