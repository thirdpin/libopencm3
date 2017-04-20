/** @defgroup sdio_defines SDIO Defines
 *
 * @ingroup STM32_defines
 *
 * @brief Defined Constants, Types n Functions for the STM32 SDIO
 *
 * @version 1.0.0
 *
 * @date 18 April 2017
 *
 * LGPL License Terms @ref lgpl_license
 */

/*
 * This file is part of the libopencm3 project.
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifdef LIBOPENCM3_SDIO_H

#ifndef LIBOPENCM3_SDIO_COMMON_ALL_H
#define LIBOPENCM3_SDIO_COMMON_ALL_H

/* --- SDIO registers ------------------------------------------------------ */

/* SDIO power control register (SDIO_POWER) */
#define SDIO_POWER			MMIO32(SDIO_BASE + 0x00)
/* SDI clock control register (SDIO_CLKCR) */
#define SDIO_CLKCR			MMIO32(SDIO_BASE + 0x04)
/* SDIO argument register (SDIO_ARG) */
#define SDIO_ARG			MMIO32(SDIO_BASE + 0x08)
/* SDIO command register (SDIO_CMD) */
#define SDIO_CMD			MMIO32(SDIO_BASE + 0x0C)
/* SDIO command response register (SDIO_RESPCMD) */
#define SDIO_RESPCMD		MMIO32(SDIO_BASE + 0x10)
/* SDIO response 1..4 register (SDIO_RESPx) */
#define SDIO_RESP1			MMIO32(SDIO_BASE + 0x14)
#define SDIO_RESP2			MMIO32(SDIO_BASE + 0x18)
#define SDIO_RESP3			MMIO32(SDIO_BASE + 0x1C)
#define SDIO_RESP4			MMIO32(SDIO_BASE + 0x20)
/* SDIO data timer register (SDIO_DTIMER) */
#define SDIO_DTIMER			MMIO32(SDIO_BASE + 0x24)
/* SDIO data length register (SDIO_DLEN) */
#define SDIO_DLEN			MMIO32(SDIO_BASE + 0x28)
/* SDIO data control register (SDIO_DCTRL) */
#define SDIO_DCTRL			MMIO32(SDIO_BASE + 0x2C)
/* SDIO data counter register (SDIO_DCOUNT) */
#define SDIO_DCOUNT			MMIO32(SDIO_BASE + 0x30)
/* SDIO status register (SDIO_STA) */
#define SDIO_STA			MMIO32(SDIO_BASE + 0x34)
/* SDIO interrupt clear register (SDIO_ICR) */
#define SDIO_ICR			MMIO32(SDIO_BASE + 0x38)
/* SDIO mask register (SDIO_MASK) */
#define SDIO_MASK			MMIO32(SDIO_BASE + 0x3C)
/* SDIO FIFO counter register (SDIO_FIFOCNT) */
#define SDIO_FIFOCNT		MMIO32(SDIO_BASE + 0x48)
/* SDIO data FIFO register (SDIO_FIFO) */
/* the SDIO data FIFO is 32 32bit words long, beginning at this address */
#define SDIO_FIFO			MMIO32(SDIO_BASE + 0x80)

/* --- SDIO_POWER values -------------------------------------------------- */
#define SDIO_POWER_PWRCTRL_SHIFT		0
#define SDIO_POWER_PWRCTRL_MASK	    	(0x3 << SDIO_POWER_PWRCTRL_SHIFT)
#define SDIO_POWER_PWRCTRL_PWROFF		(0x0 << SDIO_POWER_PWRCTRL_SHIFT)
#define SDIO_POWER_PWRCTRL_PWRON		(0x3 << SDIO_POWER_PWRCTRL_SHIFT)

/* --- SDIO_POWER values -------------------------------------------------- */
#define SDIO_CLKCR_HWFC_EN				(1 << 14)
#define SDIO_CLKCR_NEGEDGE				(1 << 13)
#define SDIO_CLKCR_WIDBUS_SHIFT			11
#define SDIO_CLKCR_WIDBUS_MASK      	(0x3 << SDIO_CLKCR_WIDBUS_SHIFT)
#define SDIO_CLKCR_BYPASS				(1 << 10)
#define SDIO_CLKCR_PWRSAV				(1 << 9)
#define SDIO_CLKCR_CLKEN				(1 << 8)
#define SDIO_CLKCR_CLKDIV_MSK			(0xFF << 0)

/* --- SDIO_CMD values ---------------------------------------------------- */
#define SDIO_CMD_ATACMD					(1 << 14)
#define SDIO_CMD_NIEN					(1 << 13)
#define SDIO_CMD_ENCMDCOMPL				(1 << 12)
#define SDIO_CMD_SDIOSUSPEND			(1 << 11)
#define SDIO_CMD_CPSMEN					(1 << 10)
#define SDIO_CMD_WAITPEND				(1 << 9)
#define SDIO_CMD_WAITINT				(1 << 8)
#define SDIO_CMD_WAITRESP_SHIFT			6
#define SDIO_CMD_WAITRESP_MSK       	(0x3 << SDIO_CMD_WAITRESP_SHIFT)
#define SDIO_CMD_CMDINDEX_MSK			(0x3F << 0)

/* --- SDIO_RESPCMD values ------------------------------------------------ */
#define SDIO_RESPCMD_MSK				(0x3F << 0)

/* --- SDIO_DLEN values --------------------------------------------------- */
#define SDIO_DLEN_MSK			    	0x1FFFFFF

/* --- SDIO_DCTRL values -------------------------------------------------- */
#define SDIO_DCTRL_SDIOEN				(1 << 11)
#define SDIO_DCTRL_RWMOD				(1 << 10)
#define SDIO_DCTRL_RWSTOP				(1 << 9)
#define SDIO_DCTRL_RWSTART				(1 << 8)
#define SDIO_DCTRL_DBLOCKSIZE_SHIFT		4
#define SDIO_DCTRL_DBLOCKSIZE_MSK		(0xF << SDIO_DCTRL_DBLOCKSIZE_SHIFT)
#define SDIO_DCTRL_DMAEN				(1 << 3)
#define SDIO_DCTRL_DTMODE				(1 << 2)
#define SDIO_DCTRL_DTDIR				(1 << 1)
#define SDIO_DCTRL_DTEN					(1 << 0)

/* --- SDIO_MASK values ------------------------------------------------ */
#define SDIO_MASK_MSK			    	0xFFFFFF

/* --- SDIO_FIFOCNT values ------------------------------------------------ */
#define SDIO_FIFOCNT_MSK			    0xFFFFFF

typedef enum
{
	SDIO_CK_RISING_EDGE,
	SDIO_CK_FALLING_EDGE,
}SDIO_CK_Polarity;

typedef enum
{
	WIDE_BUS_1 = (uint32_t)(0x0 << SDIO_CLKCR_WIDBUS_SHIFT),
	WIDE_BUS_4 = (uint32_t)(0x1 << SDIO_CLKCR_WIDBUS_SHIFT),
	WIDE_BUS_8 = (uint32_t)(0x2 << SDIO_CLKCR_WIDBUS_SHIFT),
}SDIO_WideBusMode;

typedef enum 
{
	WAIT_NO,
	WAIT_IT,
	WAIT_PEND,
}SDIO_WaitInterrupt;

typedef enum 
{
	RESPONSE_NO    = (uint32_t)(0x0 << SDIO_CMD_WAITRESP_SHIFT),
	RESPONSE_SHORT = (uint32_t)(0x1 << SDIO_CMD_WAITRESP_SHIFT),
	RESPONSE_LONG  = (uint32_t)(0x3 << SDIO_CMD_WAITRESP_SHIFT),
}SDIO_ResponseType;

typedef enum 
{
	CONTROLLER_TO_CARD,
	CARD_TO_CONTROLLER,
}SDIO_TransferDirection;

typedef enum 
{
	BLOCK_DATA_TRANSFER,
	STREAM_DATA_TRANSFER,
}SDIO_TransferMode;

typedef enum 
{
	BLOCK_LENGTH_1_B     = (uint32_t)(0x0 << SDIO_DCTRL_DBLOCKSIZE_SHIFT),
	BLOCK_LENGTH_2_B     = (uint32_t)(0x1 << SDIO_DCTRL_DBLOCKSIZE_SHIFT),
	BLOCK_LENGTH_4_B     = (uint32_t)(0x2 << SDIO_DCTRL_DBLOCKSIZE_SHIFT),
	BLOCK_LENGTH_8_B     = (uint32_t)(0x3 << SDIO_DCTRL_DBLOCKSIZE_SHIFT),
	BLOCK_LENGTH_16_B    = (uint32_t)(0x4 << SDIO_DCTRL_DBLOCKSIZE_SHIFT),
	BLOCK_LENGTH_32_B    = (uint32_t)(0x5 << SDIO_DCTRL_DBLOCKSIZE_SHIFT),
	BLOCK_LENGTH_64_B    = (uint32_t)(0x6 << SDIO_DCTRL_DBLOCKSIZE_SHIFT),
	BLOCK_LENGTH_128_B   = (uint32_t)(0x7 << SDIO_DCTRL_DBLOCKSIZE_SHIFT),
	BLOCK_LENGTH_256_B   = (uint32_t)(0x8 << SDIO_DCTRL_DBLOCKSIZE_SHIFT),
	BLOCK_LENGTH_512_B   = (uint32_t)(0x9 << SDIO_DCTRL_DBLOCKSIZE_SHIFT),
	BLOCK_LENGTH_1024_B  = (uint32_t)(0xA << SDIO_DCTRL_DBLOCKSIZE_SHIFT),
	BLOCK_LENGTH_2048_B  = (uint32_t)(0xB << SDIO_DCTRL_DBLOCKSIZE_SHIFT),
	BLOCK_LENGTH_4096_B  = (uint32_t)(0xC << SDIO_DCTRL_DBLOCKSIZE_SHIFT),
	BLOCK_LENGTH_8192_B  = (uint32_t)(0xD << SDIO_DCTRL_DBLOCKSIZE_SHIFT),
	BLOCK_LENGTH_16384_B = (uint32_t)(0xE << SDIO_DCTRL_DBLOCKSIZE_SHIFT),
}SDIO_BlockSize;

typedef enum 
{
	SDIO_CEATAEND	= (uint32_t)(1 << 23), //CE-ATA command completion signal received for CMD61
	SDIO_SDIOIT		= (uint32_t)(1 << 22), //SD I/O received
	SDIO_RXDAVL		= (uint32_t)(1 << 21), //Data available in receive FIFO
	SDIO_TXDAVL		= (uint32_t)(1 << 20), //Data available in transmit FIFO 
	SDIO_RXFIFOE	= (uint32_t)(1 << 19), //Receive FIFO empty 
	SDIO_TXFIFOE	= (uint32_t)(1 << 18), //Transmit FIFO empty 
	SDIO_RXFIFOF	= (uint32_t)(1 << 17), //Receive FIFO full 
	SDIO_TXFIFOF	= (uint32_t)(1 << 16), //Transmit FIFO full 
	SDIO_RXFIFOHF	= (uint32_t)(1 << 15), //Receive FIFO Half Full 
	SDIO_TXFIFOHE	= (uint32_t)(1 << 14), //Transmit FIFO Half Empty 
	SDIO_RXACT		= (uint32_t)(1 << 13), //Data receive in progress 
	SDIO_TXACT		= (uint32_t)(1 << 12), //Data transmit in progress 
	SDIO_CMDACT		= (uint32_t)(1 << 11), //Command transfer in progress 
	SDIO_DBCKEND	= (uint32_t)(1 << 10), //Data block sent/received (CRC check passed) 
	SDIO_STBITERR	= (uint32_t)(1 << 9),  //Start bit not detected on all data signals in wide bus mode 
	SDIO_DATAEND	= (uint32_t)(1 << 8),  //Data end (data counter, SDIDCOUNT, is zero) 
	SDIO_CMDSENT	= (uint32_t)(1 << 7),  //Command sent (no response required) 
	SDIO_CMDREND	= (uint32_t)(1 << 6),  //Command response received (CRC check passed) 
	SDIO_RXOVERR	= (uint32_t)(1 << 5),  //Received FIFO overrun error 
	SDIO_TXUNDERR	= (uint32_t)(1 << 4),  //Transmit FIFO underrun error 
	SDIO_DTIMEOUT	= (uint32_t)(1 << 3),  //Data timeout 
	SDIO_CTIMEOUT	= (uint32_t)(1 << 2),  //Command response timeout 
	SDIO_DCRCFAIL	= (uint32_t)(1 << 1),  //Data block sent/received (CRC check failed) 
	SDIO_CCRCFAIL	= (uint32_t)(1 << 0),  //Command response received (CRC check failed) 
}SDIO_Interrupt;

typedef enum 
{
	READ_WAIT_DATA2,
	READ_WAIT_CLK,
}SDIO_ReadWaitMode;

/* --- Function prototypes -------------------------------------------------------------------------- */
BEGIN_DECLS

/* --- SDIO_POWER ----------------------------------------------------------------------------------- */
/**
@brief Power-on: the card is clocked.
*/
void sdio_enable(void);
/**
@brief Power-off: the clock to card is stopped.
*/
void sdio_disable(void);

/* --- SDIO_CLKCR ----------------------------------------------------------------------------------- */
/**
@brief HW Flow Control is enabled.
*/
void sdio_flow_control_enable(void);
/**
@brief HW Flow Control is disabled.
*/
void sdio_flow_control_disable(void);
/**
@brief SDIO_CK dephasing selection bit.
@param pol SDIO_CK generated on the rising or falling edge of the master 
clock SDIOCLK.
*/
void sdio_set_ck_polarity(SDIO_CK_Polarity pol);
/**
@brief Wide bus mode selection.
@param mode 1-, 4- or 8-wide bus mode.
*/
void sdio_set_wide_bus_mode(SDIO_WideBusMode mode);
/**
@brief SDIOCLK directly drives the SDIO_CK output signal.
*/
void sdio_bypass_enable(void);
/**
@brief SDIOCLK is divided according to the CLKDIV value before driving the
SDIO_CK output signal.
*/
void sdio_bypass_disable(void);
/**
@brief SDIO_CK is only enabled when the bus is active.
*/
void sdio_power_saving_enable(void);
/**
@brief SDIO_CK clock is always enabled.
*/
void sdio_power_saving_disable(void);
/**
@brief SDIO_CK clock is enabled.
*/
void sdio_ck_enable(void);
/**
@brief SDIO_CK clock is disabled.
*/
void sdio_ck_disable(void);
/**
@brief The divide factor between the input clock (SDIOCLK) and the output 
clock (SDIO_CK): SDIO_CK frequency = SDIOCLK / [CLKDIV + 2].
*/
void sdio_set_clock_divider(uint8_t divider);
/**
@brief Gets clock divider.
@retval Clock divider
*/
uint8_t sdio_get_clock_divider(void);

/* --- SDIO_ARG ------------------------------------------------------------------------------------- */
/**
@brief Command argument sent to a card as part of a command message. If a command contains
an argument, it must be loaded into this register before writing a command to the command
register.
*/
void sdio_set_command_arg(uint32_t arg);

/* --- SDIO_CMD ------------------------------------------------------------------------------------- */
/**
@brief Enables the CE-ATA command.
*/
void sdio_CEATA_cmd_enable(void);
/**
@brief Disables the CE-ATA command.
*/
void sdio_CEATA_cmd_disable(void);
/**
@brief Enables the CE-ATA interrupt.
*/
void sdio_CEATA_interrupt_enable(void);
/**
@brief Disables the CE-ATA interrupt.
*/
void sdio_CEATA_interrupt_disable(void);
/**
@brief Enables command completion signal.
*/
void sdio_completion_cmd_enable(void);
/**
@brief Disables command completion signal.
*/
void sdio_completion_cmd_disable(void);
/**
@brief Enables suspend command to the SDIO.
*/
void sdio_suspend_cmd_enable(void);
/**
@brief Disables suspend command to the SDIO.
*/
void sdio_suspend_cmd_disable(void);
/**
@brief Enables command path state machine.
*/
void sdio_CPSM_enable(void);
/**
@brief Disables command path state machine.
*/
void sdio_CPSM_disable(void);
/**
@brief CPSM waits for ends of data transfer, or for interrupt request, or nothing.
@param wait_mode WAIT_PEND, WAIT_IT or WAIT_NO.
*/
void sdio_set_wait_it(SDIO_WaitInterrupt wait_mode);
/**
@brief Configures whether the CPSM is to wait for a response, and if yes, which
kind of response.
@param response RESPONSE_NO, RESPONSE_SHORT or RESPONSE_LONG.
*/
void sdio_set_wait_response(SDIO_ResponseType response);
/**
@brief The command index is sent to the card as part of a command message.
@param index Zero-based index of a command
*/
void sdio_set_cmd_index(uint8_t index);

/* --- SDIO_RESPCMD ---------------------------------------------------------------------------------- */
/**
@brief The SDIO_RESPCMD register contains the command index field of the last command
response received. If the command response transmission does not contain the command
index field (long or OCR response), the RESPCMD field is unknown, although it must
contain 111111b (the value of the reserved field from the response).
*/
uint8_t sdio_get_cmd_response(void);

/* --- SDIO_RESP[1..4] ------------------------------------------------------------------------------- */
/* 
---------------------------------------------------------
| Register   | Short response    | Long response        |
---------------------------------------------------------
| SDIO_RESP1 | Card Status[31:0] | Card Status [127:96] |
---------------------------------------------------------
| SDIO_RESP2 | Unused            | Card Status [95:64]  |
---------------------------------------------------------
| SDIO_RESP3 | Unused            | Card Status [63:32]  |
---------------------------------------------------------
| SDIO_RESP4 | Unused            | Card Status [31:1]   | 
---------------------------------------------------------
*/
uint32_t sdio_get_card_status_1(void);
uint32_t sdio_get_card_status_2(void);
uint32_t sdio_get_card_status_3(void);
uint32_t sdio_get_card_status_4(void);

/* --- SDIO_DTIMER ----------------------------------------------------------------------------------- */
/**
@brief The SDIO_DTIMER register contains the data timeout period, in card bus clock periods.
A counter loads the value from the SDIO_DTIMER register, and starts decrementing when
the data path state machine (DPSM) enters the Wait_R or Busy state. If the timer reaches 0
while the DPSM is in either of these states, the timeout status flag is set.
@param period Data timeout period expressed in card bus clock periods.
*/
void sdio_set_timeout_period(uint32_t period);
/**
@brief Reads data timeout period
@retval Data timeout period
*/
uint32_t sdio_get_timeout_period(void);

/* --- SDIO_DLEN ------------------------------------------------------------------------------------- */
/**
@brief The SDIO_DLEN register contains the number of data bytes to be transferred. 
The value is loaded into the data counter when data transfer starts.
@param length Number of data bytes to be transferred.
*/
void sdio_set_data_length(uint32_t length);

/* --- SDIO_DCTRL ------------------------------------------------------------------------------------ */
/**
@brief Enables data transfer.
*/
void sdio_data_transfer_enable(void);
/**
@brief Disables data transfer.
*/
void sdio_data_transfer_disable(void);
/**
@brief Selects data transfer direction.
@param direction Data transfer direction (CONTROLLER_TO_CARD or CARD_TO_CONTROLLER).
*/
void sdio_set_data_transfer_direction(SDIO_TransferDirection direction);
/**
@brief Selects data transfer mode.
@param mode Data transfer mode (BLOCK_DATA_TRANSFER or STREAM_DATA_TRANSFER).
*/
void sdio_set_data_transfer_mode(SDIO_TransferMode mode);
/**
@brief Enables SDIO DMA.
*/
void sdio_enable_dma(void);
/**
@brief Disables SDIO DMA.
*/
void sdio_disable_dma(void);
/**
@brief Sets data block size.
@param size Data block size.
*/
void sdio_set_data_block_size(SDIO_BlockSize size);
/**
@brief Enables the SD I/O Read Wait Start operation.
*/
void sdio_read_wait_start_enable(void);
/**
@brief Disables the SD I/O Read Wait Start operation.
*/
void sdio_read_wait_start_disable(void);
/**
@brief Enables the SD I/O Read Wait Stop operation.
*/
void sdio_read_wait_stop_enable(void);
/**
@brief Disables the SD I/O Read Wait Stop operation.
*/
void sdio_read_wait_stop_disable(void);
/**
@brief Sets one of the two options of inserting read wait interval.
@param mode Read wait mode (READ_WAIT_DATA2 or READ_WAIT_CLK)
*/
void sdio_set_read_wait_mode(SDIO_ReadWaitMode mode);
/**
@brief Enables the SD I/O Mode Operation.
*/
void sdio_operation_enable(void);
/**
@brief Disables the SD I/O Mode Operation.
*/
void sdio_operation_disable(void);

/* --- SDIO_DCOUNT ----------------------------------------------------------------------------------- */
/**
@brief The SDIO_DCOUNT register loads the value from the data length register 
when the DPSM moves from the Idle state to the Wait_R or Wait_S state. As
data is transferred, the counter decrements the value until it reaches 0. The DPSM then
moves to the Idle state and the data status end flag, DATAEND, is set.
@retval Data counter
*/
uint32_t sdio_get_data_counter(void);

/* --- SDIO_STA -------------------------------------------------------------------------------------- */
/**
@brief Checks whether the specified SDIO flag is set or not.
@param flag Flag name
*/
bool sdio_get_flag_status(SDIO_Interrupt flag);

/* --- SDIO_ICR -------------------------------------------------------------------------------------- */
/**
@brief Clears the specified SDIO flag.
@param flag Flag name
*/
void sdio_clear_flag_status(SDIO_Interrupt flag);
/**
@brief Clears the specified SDIO flags.
@param mask Flags to clear
*/
void sdio_clear_all_flag_status(uint32_t mask);

/* --- SDIO_MASK ------------------------------------------------------------------------------------- */
/**
@brief Enables SDIO interrupt generation.
@param interrupt Interrupt name
*/
void sdio_enable_interrupt(SDIO_Interrupt interrupt);
/**
@brief Disables SDIO interrupt generation.
@param interrupt Interrupt name
*/
void sdio_disable_interrupt(SDIO_Interrupt interrupt);
/**
@brief Enables all SDIO interrupts.
*/
void sdio_enable_all_interrupts(void);
/**
@brief Disables all SDIO interrupts.
*/
void sdio_disable_all_interrupts(void);

/* --- SDIO_FIFOCNT ---------------------------------------------------------------------------------- */
/**
@brief The SDIO_FIFOCNT register contains the remaining number of words to be written to or
read from the FIFO. The FIFO counter loads the value from the data length register 
when the data transfer enable bit, DTEN, is set in the data control register
and the DPSM is at the Idle state. If the data length is not wordaligned (multiple of 4), 
the remaining 1 to 3 bytes are regarded as a word.
@retval FIFO counter
*/
uint32_t sdio_get_fifo_counter(void);

/* --- SDIO_FIFO ------------------------------------------------------------------------------------- */
/**
@brief Reads one data word from Rx FIFO.
@retval Data received
*/
uint32_t sdio_read_fifo_data(void);
/**
@brief Write one data word to Tx FIFO.
@param data 32-bit data word to write.
*/
void sdio_write_fifo_data(uint32_t data);

/* --- SDIO Additional API ---------------------------------------------------------------------------- */
void sdio_send_command(uint8_t cmd_index, 
	                   uint32_t cmd_arg, 
	                   SDIO_ResponseType resp, 
	                   SDIO_WaitInterrupt wait, 
	                   bool enable_CPSM);
END_DECLS

#endif

#else
#warning "sdio_common_all.h should not be included explicitly, only via sdio.h"
#endif