/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2012 Felix Held <felix-libopencm3@felixheld.de>
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

#ifndef LIBOPENCM3_SDIO_H
#define LIBOPENCM3_SDIO_H

#include <libopencm3/cm3/common.h>
#include <libopencm3/stm32/memorymap.h>

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
#define SDIO_RESPCMD			MMIO32(SDIO_BASE + 0x10)

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
/* read only, write has no effect */
#define SDIO_DCOUNT			MMIO32(SDIO_BASE + 0x30)

/* SDIO status register (SDIO_STA) */
#define SDIO_STA			MMIO32(SDIO_BASE + 0x34)

/* SDIO interrupt clear register (SDIO_ICR) */
#define SDIO_ICR			MMIO32(SDIO_BASE + 0x38)

/* SDIO mask register (SDIO_MASK) */
#define SDIO_MASK			MMIO32(SDIO_BASE + 0x3C)

/* SDIO FIFO counter register (SDIO_FIFOCNT) */
#define SDIO_FIFOCNT			MMIO32(SDIO_BASE + 0x48)

/* SDIO data FIFO register (SDIO_FIFO) */
/* the SDIO data FIFO is 32 32bit words long, beginning at this address */
#define SDIO_FIFO			MMIO32(SDIO_BASE + 0x80)


/* --- SDIO_POWER values --------------------------------------------------- */

#define SDIO_POWER_PWRCTRL_SHIFT	0
#define SDIO_POWER_PWRCTRL_MASK		0x3
#define SDIO_POWER_PWRCTRL_PWROFF	(0x0 << SDIO_POWER_PWRCTRL_SHIFT)
/* what does "10: Reserved power-up" mean? */
#define SDIO_POWER_PWRCTRL_RSVPWRUP	(0x2 << SDIO_POWER_PWRCTRL_SHIFT)
#define SDIO_POWER_PWRCTRL_PWRON	(0x3 << SDIO_POWER_PWRCTRL_SHIFT)


/* --- SDIO_CLKCR values --------------------------------------------------- */

/* HWFC_EN: HW Flow Control enable */
#define SDIO_CLKCR_HWFC_EN		(1 << 14)

/* NEGEDGE: SDIO_CK dephasing selection bit */
#define SDIO_CLKCR_NEGEDGE		(1 << 13)

/* WIDBUS: Wide bus mode enable bit */
/* set the width of the data bus */
#define SDIO_CLKCR_WIDBUS_SHIFT		11
#define SDIO_CLKCR_WIDBUS_MASK	0x3
#define SDIO_CLKCR_WIDBUS_1		(0x0 << SDIO_CLKCR_WIDBUS_SHIFT)
#define SDIO_CLKCR_WIDBUS_4		(0x1 << SDIO_CLKCR_WIDBUS_SHIFT)
#define SDIO_CLKCR_WIDBUS_8		(0x2 << SDIO_CLKCR_WIDBUS_SHIFT)

/* BYPASS: Clock divider bypass enable bit */
#define SDIO_CLKCR_BYPASS		(1 << 10)

/* PWRSAV: Power saving configuration bit */
#define SDIO_CLKCR_PWRSAV		(1 << 9)

/* CLKEN: Clock enable bit */
#define SDIO_CLKCR_CLKEN		(1 << 8)

/* CLKDIV: Clock divide factor */
#define SDIO_CLKCR_CLKDIV_SHIFT		0
#define SDIO_CLKCR_CLKDIV_MASK		0xFF


/* --- SDIO_CMD values ---------------------------------------------------- */

/* ATACMD: CE-ATA command */
#define SDIO_CMD_ATACMD			(1 << 14)

/* nIEN: not Interrupt Enable */
#define SDIO_CMD_NIEN			(1 << 13)

/* ENCMDcompl: Enable CMD completion */
#define SDIO_CMD_ENCMDCOMPL		(1 << 12)

/* SDIOSuspend: SD I/O suspend command */
#define SDIO_CMD_SDIOSUSPEND		(1 << 11)

/* CPSMEN: Command path state machine (CPSM) Enable bit */
#define SDIO_CMD_CPSMEN			(1 << 10)

/* WAITPEND: CPSM Waits for ends of data transfer (CmdPend internal signal) */
#define SDIO_CMD_WAITPEND		(1 << 9)

/* WAITINT: CPSM waits for interrupt request */
#define SDIO_CMD_WAITINT		(1 << 8)

/* WAITRESP: Wait for response bits */
#define SDIO_CMD_WAITRESP_SHIFT		6
#define SDIO_CMD_WAITRESP_MASK		0x3
/* 00: No response, expect CMDSENT flag */
#define SDIO_CMD_WAITRESP_NO_0		(0x0 << SDIO_CMD_WAITRESP_SHIFT)
/* 01: Short response, expect CMDREND or CCRCFAIL flag */
#define SDIO_CMD_WAITRESP_SHORT		(0x1 << SDIO_CMD_WAITRESP_SHIFT)
/* 10: No response, expect CMDSENT flag */
#define SDIO_CMD_WAITRESP_NO_2		(0x2 << SDIO_CMD_WAITRESP_SHIFT)
/* 11: Long response, expect CMDREND or CCRCFAIL flag */
#define SDIO_CMD_WAITRESP_LONG		(0x3 << SDIO_CMD_WAITRESP_SHIFT)

/* CMDINDEX: Command index */
#define SDIO_CMD_CMDINDEX_SHIFT		0
#define SDIO_CMD_CMDINDEX_MASK		0x3F


/* --- SDIO_RESPCMD values ------------------------------------------------ */

#define SDIO_RESPCMD_SHIFT		0
#define SDIO_RESPCMD_MASK		0x3F


/* --- SDIO_DLEN values --------------------------------------------------- */

/* DATALENGTH: Data length value */
#define SDIO_DLEN_DATALENGTH_SHIFT	0
#define SDIO_DLEN_DATALENGTH_MASK	0x1FFFFFF


/* --- SDIO_DCTRL values -------------------------------------------------- */

/* SDIOEN: SD I/O enable functions */
#define SDIO_DCTRL_SDIOEN		(1 << 11)

/* RWMOD: Read wait mode */
/* 0: Read Wait control stopping SDIO_D2
 * 1: Read Wait control using SDIO_CK
 */
#define SDIO_DCTRL_RWMOD		(1 << 10)

/* RWSTOP: Read wait stop */
/* 0: Read wait in progress if RWSTART bit is set
 * 1: Enable for read wait stop if RWSTART bit is set
 */
#define SDIO_DCTRL_RWSTOP		(1 << 9)

/* RWSTART: Read wait start */
#define SDIO_DCTRL_RWSTART		(1 << 8)

/* DBLOCKSIZE: Data block size */
/* SDIO_DCTRL_DBLOCKSIZE_n
 * block size is 2**n bytes with 0<=n<=14
 */
#define SDIO_DCTRL_DBLOCKSIZE_SHIFT	4
#define SDIO_DCTRL_DBLOCKSIZE_MASK	0xF
#define SDIO_DCTRL_DBLOCKSIZE_0		(0x0 << SDIO_DCTRL_DBLOCKSIZE_SHIFT)
#define SDIO_DCTRL_DBLOCKSIZE_1		(0x1 << SDIO_DCTRL_DBLOCKSIZE_SHIFT)
#define SDIO_DCTRL_DBLOCKSIZE_2		(0x2 << SDIO_DCTRL_DBLOCKSIZE_SHIFT)
#define SDIO_DCTRL_DBLOCKSIZE_3		(0x3 << SDIO_DCTRL_DBLOCKSIZE_SHIFT)
#define SDIO_DCTRL_DBLOCKSIZE_4		(0x4 << SDIO_DCTRL_DBLOCKSIZE_SHIFT)
#define SDIO_DCTRL_DBLOCKSIZE_5		(0x5 << SDIO_DCTRL_DBLOCKSIZE_SHIFT)
#define SDIO_DCTRL_DBLOCKSIZE_6		(0x6 << SDIO_DCTRL_DBLOCKSIZE_SHIFT)
#define SDIO_DCTRL_DBLOCKSIZE_7		(0x7 << SDIO_DCTRL_DBLOCKSIZE_SHIFT)
#define SDIO_DCTRL_DBLOCKSIZE_8		(0x8 << SDIO_DCTRL_DBLOCKSIZE_SHIFT)
#define SDIO_DCTRL_DBLOCKSIZE_9		(0x9 << SDIO_DCTRL_DBLOCKSIZE_SHIFT)
#define SDIO_DCTRL_DBLOCKSIZE_10	(0xA << SDIO_DCTRL_DBLOCKSIZE_SHIFT)
#define SDIO_DCTRL_DBLOCKSIZE_11	(0xB << SDIO_DCTRL_DBLOCKSIZE_SHIFT)
#define SDIO_DCTRL_DBLOCKSIZE_12	(0xC << SDIO_DCTRL_DBLOCKSIZE_SHIFT)
#define SDIO_DCTRL_DBLOCKSIZE_13	(0xD << SDIO_DCTRL_DBLOCKSIZE_SHIFT)
#define SDIO_DCTRL_DBLOCKSIZE_14	(0xE << SDIO_DCTRL_DBLOCKSIZE_SHIFT)

/* DMAEN: DMA enable bit */
#define SDIO_DCTRL_DMAEN		(1 << 3)

/* DTMODE: Data transfer mode selection 1: Stream or SDIO multi byte transfer */
#define SDIO_DCTRL_DTMODE		(1 << 2)

/* DTDIR: Data transfer direction selection */
/* 0: From controller to card.
 * 1: From card to controller.
 */
#define SDIO_DCTRL_DTDIR		(1 << 1)

/* DTEN: Data transfer enabled bit */
#define SDIO_DCTRL_DTEN			(1 << 0)


/* --- SDIO_STA values ---------------------------------------------------- */

/* CEATAEND: CE-ATA command completion signal received for CMD61 */
#define SDIO_STA_CEATAEND		(1 << 23)

/* SDIOIT: SDIO interrupt received */
#define SDIO_STA_SDIOIT			(1 << 22)

/* RXDAVL: Data available in receive FIFO */
#define SDIO_STA_RXDAVL			(1 << 21)

/* TXDAVL: Data available in transmit FIFO */
#define SDIO_STA_TXDAVL			(1 << 20)

/* RXFIFOE: Receive FIFO empty */
#define SDIO_STA_RXFIFOE		(1 << 19)

/* TXFIFOE: Transmit FIFO empty */
/* HW Flow Control enabled -> TXFIFOE signals becomes activated when the FIFO
 * contains 2 words.
 */
#define SDIO_STA_TXFIFOE		(1 << 18)

/* RXFIFOF: Receive FIFO full */
/* HW Flow Control  enabled => RXFIFOF signals becomes activated 2 words before
 * the FIFO is full.
 */
#define SDIO_STA_RXFIFOF		(1 << 17)

/* TXFIFOF: Transmit FIFO full */
#define SDIO_STA_TXFIFOF		(1 << 16)

/* RXFIFOHF: Receive FIFO half full: there are at least 8 words in the FIFO */
#define SDIO_STA_RXFIFOHF		(1 << 15)

/* TXFIFOHE: Transmit FIFO half empty: at least 8 words can be written into
 * the FIFO
 */
#define SDIO_STA_TXFIFOHE		(1 << 14)

/* RXACT: Data receive in progress */
#define SDIO_STA_RXACT			(1 << 13)

/* TXACT: Data transmit in progress */
#define SDIO_STA_TXACT			(1 << 12)

/* CMDACT: Command transfer in progress */
#define SDIO_STA_CMDACT			(1 << 11)

/* DBCKEND: Data block sent/received (CRC check passed) */
#define SDIO_STA_DBCKEND		(1 << 10)

/* STBITERR: Start bit not detected on all data signals in wide bus mode */
#define SDIO_STA_STBITERR		(1 << 9)

/* DATAEND: Data end (data counter, SDIDCOUNT, is zero) */
#define SDIO_STA_DATAEND		(1 << 8)

/* CMDSENT: Command sent (no response required) */
#define SDIO_STA_CMDSENT		(1 << 7)

/* CMDREND: Command response received (CRC check passed) */
#define SDIO_STA_CMDREND		(1 << 6)

/* RXOVERR: Received FIFO overrun error */
#define SDIO_STA_RXOVERR		(1 << 5)

/* TXUNDERR: Transmit FIFO underrun error */
#define SDIO_STA_TXUNDERR		(1 << 4)

/* DTIMEOUT: Data timeout */
#define SDIO_STA_DTIMEOUT		(1 << 3)

/* CTIMEOUT: Command response timeout */
#define SDIO_STA_CTIMEOUT		(1 << 2)

/* DCRCFAIL: Data block sent/received (CRC check failed) */
#define SDIO_STA_DCRCFAIL		(1 << 1)

/* CCRCFAIL: Command response received (CRC check failed) */
#define SDIO_STA_CCRCFAIL		(1 << 0)


/* --- SDIO_ICR values ---------------------------------------------------- */

/* CEATAENDC: CEATAEND flag clear bit */
#define SDIO_ICR_CEATAENDC		(1 << 23)

/* SDIOITC: SDIOIT flag clear bit */
#define SDIO_ICR_SDIOITC		(1 << 22)

/* DBCKENDC: DBCKEND flag clear bit */
#define SDIO_ICR_DBCKENDC		(1 << 10)

/* STBITERRC: STBITERR flag clear bit */
#define SDIO_ICR_STBITERRC		(1 << 9)

/* DATAENDC: DATAEND flag clear bit */
#define SDIO_ICR_DATAENDC		(1 << 8)

/* CMDSENTC: CMDSENT flag clear bit */
#define SDIO_ICR_CMDSENTC		(1 << 7)

/* CMDRENDC: CMDREND flag clear bit */
#define SDIO_ICR_CMDRENDC		(1 << 6)

/* RXOVERRC: RXOVERR flag clear bit */
#define SDIO_ICR_RXOVERRC		(1 << 5)

/* TXUNDERRC: TXUNDERR flag clear bit */
#define SDIO_ICR_TXUNDERRC		(1 << 4)

/* DTIMEOUTC: DTIMEOUT flag clear bit */
#define SDIO_ICR_DTIMEOUTC		(1 << 3)

/* CTIMEOUTC: CTIMEOUT flag clear bit */
#define SDIO_ICR_CTIMEOUTC		(1 << 2)

/* DCRCFAILC: DCRCFAIL flag clear bit */
#define SDIO_ICR_DCRCFAILC		(1 << 1)

/* CCRCFAILC: CCRCFAIL flag clear bit */
#define SDIO_ICR_CCRCFAILC		(1 << 0)


/* --- SDIO_MASK values --------------------------------------------------- */

/* CEATAENDIE: CE-ATA command completion signal received interrupt enable */
#define SDIO_MASK_CEATAENDIE		(1 << 23)

/* SDIOITIE: SDIO mode interrupt received interrupt enable */
#define SDIO_MASK_SDIOITIE		(1 << 22)

/* RXDAVLIE: Data available in Rx FIFO interrupt enable */
#define SDIO_MASK_RXDAVLIE		(1 << 21)

/* TXDAVLIE: Data available in Tx FIFO interrupt enable */
#define SDIO_MASK_TXDAVLIE		(1 << 20)

/* RXFIFOEIE: Rx FIFO empty interrupt enable */
#define SDIO_MASK_RXFIFOEIE		(1 << 19)

/* TXFIFOEIE: Tx FIFO empty interrupt enable */
#define SDIO_MASK_TXFIFOEIE		(1 << 18)

/* RXFIFOFIE: Rx FIFO full interrupt enable */
#define SDIO_MASK_RXFIFOFIE		(1 << 17)

/* TXFIFOFIE: Tx FIFO full interrupt enable */
#define SDIO_MASK_TXFIFOFIE		(1 << 16)

/* RXFIFOHFIE: Rx FIFO half full interrupt enable */
#define SDIO_MASK_RXFIFOHFIE		(1 << 15)

/* TXFIFOHEIE: Tx FIFO half empty interrupt enable */
#define SDIO_MASK_TXFIFOHEIE		(1 << 14)

/* RXACTIE: Data receive acting interrupt enable */
#define SDIO_MASK_RXACTIE		(1 << 13)

/* TXACTIE: Data transmit acting interrupt enable */
#define SDIO_MASK_TXACTIE		(1 << 12)

/* CMDACTIE: Command acting interrupt enable */
#define SDIO_MASK_CMDACTIE		(1 << 11)

/* DBCKENDIE: Data block end interrupt enable */
#define SDIO_MASK_DBCKENDIE		(1 << 10)

/* STBITERRIE: Start bit error interrupt enable */
#define SDIO_MASK_STBITERRIE		(1 << 9)

/* DATAENDIE: Data end interrupt enable */
#define SDIO_MASK_DATAENDIE		(1 << 8)

/* CMDSENTIE: Command sent interrupt enable */
#define SDIO_MASK_CMDSENTIE		(1 << 7)

/* CMDRENDIE: Command response received interrupt enable */
#define SDIO_MASK_CMDRENDIE		(1 << 6)

/* RXOVERRIE: Rx FIFO overrun error interrupt enable */
#define SDIO_MASK_RXOVERRIE		(1 << 5)

/* TXUNDERRIE: Tx FIFO underrun error interrupt enable */
#define SDIO_MASK_TXUNDERRIE		(1 << 4)

/* DTIMEOUTIE: Data timeout interrupt enable */
#define SDIO_MASK_DTIMEOUTIE		(1 << 3)

/* CTIMEOUTIE: Command timeout interrupt enable */
#define SDIO_MASK_CTIMEOUTIE		(1 << 2)

/* DCRCFAILIE: Data CRC fail interrupt enable */
#define SDIO_MASK_DCRCFAILIE		(1 << 1)

/* CCRCFAILIE: Command CRC fail interrupt enable */
#define SDIO_MASK_CCRCFAILIE		(1 << 0)


/* --- SDIO_FIFOCNT values ------------------------------------------------- */

/* FIFOCOUNT: Remaining number of words to be written to or read from the
 * FIFO
 */
#define SDIO_FIFOCNT_FIFOCOUNT_SHIFT	0
#define SDIO_FIFOCNT_FIFOCOUNT_MASK	0xFFFFFF


/* From here Thirdpin part is started */
/* --- SDIO registers ------------------------------------------------------ */
#define SDIO_POWER			MMIO32(SDIO_BASE + 0x00) /* SDIO power control register (SDIO_POWER) */
#define SDIO_CLKCR			MMIO32(SDIO_BASE + 0x04) /* SDIO clock control register (SDIO_CLKCR) */
#define SDIO_ARG			MMIO32(SDIO_BASE + 0x08) /* SDIO argument register (SDIO_ARG) */
#define SDIO_CMD			MMIO32(SDIO_BASE + 0x0C) /* SDIO command register (SDIO_CMD) */
#define SDIO_RESPCMD		MMIO32(SDIO_BASE + 0x10) /* SDIO command response register (SDIO_RESPCMD) */
#define SDIO_RESP1			MMIO32(SDIO_BASE + 0x14) /* SDIO response 1..4 register (SDIO_RESPx) */
#define SDIO_RESP2			MMIO32(SDIO_BASE + 0x18)
#define SDIO_RESP3			MMIO32(SDIO_BASE + 0x1C)
#define SDIO_RESP4			MMIO32(SDIO_BASE + 0x20)
#define SDIO_DTIMER			MMIO32(SDIO_BASE + 0x24) /* SDIO data timer register (SDIO_DTIMER) */
#define SDIO_DLEN			MMIO32(SDIO_BASE + 0x28) /* SDIO data length register (SDIO_DLEN) */
#define SDIO_DCTRL			MMIO32(SDIO_BASE + 0x2C) /* SDIO data control register (SDIO_DCTRL) */
#define SDIO_DCOUNT			MMIO32(SDIO_BASE + 0x30) /* SDIO data counter register (SDIO_DCOUNT) */
#define SDIO_STA			MMIO32(SDIO_BASE + 0x34) /* SDIO status register (SDIO_STA) */
#define SDIO_ICR			MMIO32(SDIO_BASE + 0x38) /* SDIO interrupt clear register (SDIO_ICR) */
#define SDIO_MASK			MMIO32(SDIO_BASE + 0x3C) /* SDIO mask register (SDIO_MASK) */
#define SDIO_FIFOCNT		MMIO32(SDIO_BASE + 0x48) /* SDIO FIFO counter register (SDIO_FIFOCNT) */
#define SDIO_FIFO			MMIO32(SDIO_BASE + 0x80) /* SDIO data FIFO register (SDIO_FIFO) */

/* --- SDIO_POWER values -------------------------------------------------- */
#define SDIO_CLKCR_CLKDIV_MSK			(uint32_t)(0xFF << 0)
#define SDIO_CLKCR_CLEAR_MASK         	((uint32_t)0xFFFF8100)

/* --- SDIO_CMD values ---------------------------------------------------- */
#define SDIO_CMD_WAITRESP_MSK       	(uint32_t)(0x3 << SDIO_CMD_WAITRESP_SHIFT)
#define SDIO_CMD_CMDINDEX_MSK			(uint32_t)(0x3F << 0)
#define SDIO_CMD_CLEAR_MASK             ((uint32_t)0xFFFFF800)

/* --- SDIO_RESPCMD values ------------------------------------------------ */
#define SDIO_RESPCMD_MSK				(uint32_t)(0x3F << 0)

/* --- SDIO_DLEN values --------------------------------------------------- */
#define SDIO_DLEN_MSK			    	(uint32_t)0x1FFFFFF

/* --- SDIO_DCTRL values -------------------------------------------------- */
#define SDIO_DCTRL_DBLOCKSIZE_MSK		((uint32_t)(0xF << SDIO_DCTRL_DBLOCKSIZE_SHIFT))
#define SDIO_DCTRL_CLEAR_MASK           ((uint32_t)0xFFFFFF08)

/* --- Function prototypes ------------------------------------------------- */

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
}SDIO_Flag;

typedef enum
{
	READ_WAIT_DATA2,
	READ_WAIT_CLK,
}SDIO_ReadWaitMode;

typedef struct
{
	SDIO_CK_Polarity polarity;
	bool bypass;
	bool power_save;
	SDIO_WideBusMode bus_wide;
	bool flow_control;
	uint8_t clock_divider;
} SDIO_config;

typedef struct
{
	uint8_t index;
	uint32_t arg;
	SDIO_ResponseType response;
	SDIO_WaitInterrupt wait;
	bool enable_CPSM;
} SDIO_command;

typedef struct
{
	uint32_t period;
	uint32_t length;
	SDIO_BlockSize size;
	SDIO_TransferDirection direction;
	SDIO_TransferMode mode;
	bool enable;
} SDIO_data_config;

/* --- Function prototypes -------------------------------------------------------------------------- */
BEGIN_DECLS
void sdio_deinit(void);
void sdio_config(SDIO_config *config);
void sdio_ck_enable(void);
void sdio_ck_disable(void);
void sdio_enable(void);
void sdio_disable(void);
void sdio_send_command(SDIO_command *cmd);
uint8_t sdio_get_cmd_response(void);
uint32_t sdio_get_card_status_1(void);
uint32_t sdio_get_card_status_2(void);
uint32_t sdio_get_card_status_3(void);
uint32_t sdio_get_card_status_4(void);
void sdio_data_config(SDIO_data_config *config);
uint32_t sdio_get_data_counter(void);
uint32_t sdio_read_fifo_data(void);
void sdio_write_fifo_data(uint32_t data);
uint32_t sdio_get_fifo_counter(void);
void sdio_read_wait_start_enable(void);
void sdio_read_wait_start_disable(void);
void sdio_read_wait_stop_enable(void);
void sdio_read_wait_stop_disable(void);
void sdio_set_read_wait_mode(SDIO_ReadWaitMode mode);
void sdio_operation_enable(void);
void sdio_operation_disable(void);
void sdio_suspend_cmd_enable(void);
void sdio_suspend_cmd_disable(void);
void sdio_completion_cmd_enable(void);
void sdio_completion_cmd_disable(void);
void sdio_CEATA_interrupt_enable(void);
void sdio_CEATA_interrupt_disable(void);
void sdio_CEATA_cmd_enable(void);
void sdio_CEATA_cmd_disable(void);
void sdio_enable_dma(void);
void sdio_disable_dma(void);
void sdio_enable_interrupt(uint32_t interrupt);
void sdio_disable_interrupt(uint32_t interrupt);
bool sdio_get_flag_status(uint32_t flag);
void sdio_clear_flag_status(uint32_t flag);
END_DECLS

#endif
