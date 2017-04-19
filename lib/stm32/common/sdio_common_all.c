#include <libopencm3/stm32/sdio.h>

void sdio_enable()
{
	SDIO_POWER &= ~SDIO_POWER_PWRCTRL_MASK;
	SDIO_POWER |= SDIO_POWER_PWRCTRL_PWRON;
}

void sdio_disable()
{
	SDIO_POWER &= ~SDIO_POWER_PWRCTRL_MASK;
	SDIO_POWER |= SDIO_POWER_PWRCTRL_PWROFF;
}

void sdio_flow_control_enable()
{
	SDIO_CLKCR |= SDIO_CLKCR_HWFC_EN;
}

void sdio_flow_control_disable()
{
	SDIO_CLKCR &= ~SDIO_CLKCR_HWFC_EN;
}

void sdio_set_ck_polarity(SDIO_CK_Polarity pol)
{
	switch(pol)
	{
		case SDIO_CK_RISING_EDGE:
			SDIO_CLKCR &= ~SDIO_CLKCR_NEGEDGE;
			break;

		case SDIO_CK_FALLING_EDGE:
			SDIO_CLKCR |= SDIO_CLKCR_NEGEDGE;
			break;
	}
}

void sdio_set_wide_bus_mode(SDIO_WideBusMode mode)
{
	SDIO_CLKCR &= ~SDIO_CLKCR_WIDBUS_MASK;
	SDIO_CLKCR |= mode;
}

void sdio_bypass_enable()
{
	SDIO_CLKCR |= SDIO_CLKCR_BYPASS;
}

void sdio_bypass_disable()
{
	SDIO_CLKCR &= ~SDIO_CLKCR_BYPASS;
}

void sdio_power_saving_enable()
{
	SDIO_CLKCR |= SDIO_CLKCR_PWRSAV;
}

void sdio_power_saving_disable()
{
	SDIO_CLKCR &= ~SDIO_CLKCR_PWRSAV;
}

void sdio_ck_enable()
{
	SDIO_CLKCR |= SDIO_CLKCR_CLKEN;
}

void sdio_ck_disable()
{
	SDIO_CLKCR &= ~SDIO_CLKCR_CLKEN;
}

void sdio_set_clock_divider(uint8_t divider)
{
	SDIO_CLKCR &= ~SDIO_CLKCR_CLKDIV_MSK;
	SDIO_CLKCR |= divider;
}

void sdio_set_command_arg(uint32_t arg)
{
	SDIO_ARG = arg;
}

void sdio_CEATA_cmd_enable(void)
{
	SDIO_CMD |= SDIO_CMD_ATACMD;
}

void sdio_CEATA_cmd_disable(void)
{
	SDIO_CMD &= ~SDIO_CMD_ATACMD;
}

void sdio_CEATA_interrupt_enable(void)
{
	SDIO_CMD &= ~SDIO_CMD_NIEN;
}

void sdio_CEATA_interrupt_disable(void)
{
	SDIO_CMD |= SDIO_CMD_NIEN;
}

void sdio_completion_cmd_enable(void)
{
	SDIO_CMD |= SDIO_CMD_ENCMDCOMPL;
}

void sdio_completion_cmd_disable(void)
{
	SDIO_CMD &= ~SDIO_CMD_ENCMDCOMPL;
}

void sdio_suspend_cmd_enable(void)
{
	SDIO_CMD |= SDIO_CMD_SDIOSUSPEND;
}

void sdio_suspend_cmd_disable(void)
{
	SDIO_CMD &= ~SDIO_CMD_SDIOSUSPEND;
}

void sdio_CPSM_enable()
{
	SDIO_CMD |= SDIO_CMD_CPSMEN;
}

void sdio_CPSM_disable()
{
	SDIO_CMD &= ~SDIO_CMD_CPSMEN;
}

void sdio_set_wait_it(SDIO_WaitInterrupt wait_mode)
{
	SDIO_CMD &= ~SDIO_CMD_WAITPEND;
	SDIO_CMD &= ~SDIO_CMD_WAITINT;

	switch(wait_mode)
	{
		case WAIT_NO:
			break;
		case WAIT_IT:
			SDIO_CMD |= SDIO_CMD_WAITINT;
			break;
		case WAIT_PEND:
			SDIO_CMD |= SDIO_CMD_WAITPEND;
			break;
	}
}

void sdio_set_wait_response(SDIO_ResponseType response)
{
	SDIO_CMD &= ~SDIO_CMD_WAITRESP_MSK;
	SDIO_CMD |= response;
}

void sdio_set_cmd_index(uint8_t index)
{
	index &= SDIO_CMD_CMDINDEX_MSK;
	SDIO_CMD &= ~SDIO_CMD_CMDINDEX_MSK;
	SDIO_CMD |= index;
}

uint8_t sdio_get_cmd_response()
{
	return (uint8_t)(SDIO_RESPCMD & SDIO_RESPCMD_MSK);
}

uint32_t sdio_get_card_status_1()
{
	return SDIO_RESP1;
}

uint32_t sdio_get_card_status_2()
{
	return SDIO_RESP2;
}

uint32_t sdio_get_card_status_3()
{
	return SDIO_RESP3;
}

uint32_t sdio_get_card_status_4()
{
	return SDIO_RESP4;
}

void sdio_set_timeout_period(uint32_t period)
{
	SDIO_DTIMER = period;
}

uint32_t sdio_get_timeout_period()
{
	return SDIO_DTIMER;
}

void sdio_set_data_length(uint32_t length)
{
	SDIO_DLEN = (length & SDIO_DLEN_MSK);
}

void sdio_data_transfer_enable(void)
{
	SDIO_DCTRL |= SDIO_DCTRL_DTEN;
}

void sdio_data_transfer_disable(void)
{
	SDIO_DCTRL &= ~SDIO_DCTRL_DTEN;
}

void sdio_set_data_transfer_direction(SDIO_TransferDirection direction)
{
	switch(direction)
	{
		case CONTROLLER_TO_CARD:
			SDIO_DCTRL &= ~SDIO_DCTRL_DTDIR;
			break;
		case CARD_TO_CONTROLLER:
			SDIO_DCTRL |= SDIO_DCTRL_DTDIR;
			break;
	}
}

void sdio_set_data_transfer_mode(SDIO_TransferMode mode)
{
	switch(mode)
	{
		case BLOCK_DATA_TRANSFER:
			SDIO_DCTRL &= ~SDIO_DCTRL_DTMODE;
			break;
		case STREAM_DATA_TRANSFER:
			SDIO_DCTRL |= SDIO_DCTRL_DTMODE;
			break;
	}
}

void sdio_enable_dma(void)
{
	SDIO_DCTRL |= SDIO_DCTRL_DMAEN;
}

void sdio_disable_dma(void)
{
	SDIO_DCTRL &= ~SDIO_DCTRL_DMAEN;
}

void sdio_set_data_block_size(SDIO_BlockSize size)
{
	SDIO_DCTRL &= ~SDIO_DCTRL_DBLOCKSIZE_MSK;
	SDIO_DCTRL |= size;
}

void sdio_read_wait_start_enable(void)
{
	SDIO_DCTRL |= SDIO_DCTRL_RWSTART;
}

void sdio_read_wait_start_disable(void)
{
	SDIO_DCTRL &= ~SDIO_DCTRL_RWSTART;
}

void sdio_read_wait_stop_enable(void)
{
	SDIO_DCTRL |= SDIO_DCTRL_RWSTOP;
}

void sdio_read_wait_stop_disable(void)
{
	SDIO_DCTRL &= ~SDIO_DCTRL_RWSTOP;
}

void sdio_set_read_wait_mode(SDIO_ReadWaitMode mode)
{
	switch(mode)
	{
		case READ_WAIT_DATA2:
			SDIO_DCTRL &= ~SDIO_DCTRL_RWMOD;
			break;
		case READ_WAIT_CLK:
			SDIO_DCTRL |= SDIO_DCTRL_RWMOD;
			break;
	}
}

void sdio_operation_enable(void)
{
	SDIO_DCTRL |= SDIO_DCTRL_SDIOEN;
}

void sdio_operation_disable(void)
{
	SDIO_DCTRL &= ~SDIO_DCTRL_SDIOEN;
}

uint32_t sdio_get_data_counter(void)
{
	return (SDIO_DCOUNT);
}

bool sdio_get_flag_status(SDIO_Interrupt flag)
{
	return (bool)(SDIO_STA & flag);
}

void sdio_clear_flag_status(SDIO_Interrupt flag)
{
	SDIO_ICR |= flag;
}

void sdio_clear_all_flag_status(uint32_t mask)
{
	SDIO_ICR |= mask;
}

void sdio_enable_interrupt(SDIO_Interrupt interrupt)
{
	SDIO_MASK |= interrupt;
}

void sdio_disable_interrupt(SDIO_Interrupt interrupt)
{
	SDIO_MASK &= ~interrupt;
}

void sdio_enable_all_interrupts(void)
{
	SDIO_MASK |= SDIO_MASK_MSK;
}

void sdio_disable_all_interrupts(void)
{
	SDIO_MASK &= ~SDIO_MASK_MSK;
}

uint32_t sdio_get_fifo_counter(void)
{
	return (SDIO_FIFOCNT & SDIO_FIFOCNT_MSK);
}

uint32_t sdio_read_fifo_data(void)
{
	return (SDIO_FIFO);
}

void sdio_write_fifo_data(uint32_t data)
{
	SDIO_FIFO = data;
}

void sdio_send_command(uint8_t cmd_index, 
	                   uint32_t cmd_arg, 
	                   SDIO_ResponseType resp, 
	                   SDIO_WaitInterrupt wait, 
	                   bool enable_CPSM)
{
	sdio_set_cmd_index(cmd_index);
	sdio_set_command_arg(cmd_arg);
	sdio_set_wait_response(resp);
	sdio_set_wait_it(wait);
	if (enable_CPSM)
		sdio_CPSM_enable();
	else
		sdio_CPSM_disable();
}