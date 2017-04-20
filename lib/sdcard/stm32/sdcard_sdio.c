#include <libopencm3/sdcard/sdcard.h>

#define NULL 0
#define SDIO_STATIC_FLAGS               ((uint32_t)0x000005FF)
#define SDIO_CMD0TIMEOUT                ((uint32_t)0x00010000)

/** 
* @brief  Mask for errors Card Status R1 (OCR Register) 
*/
#define SD_OCR_ADDR_OUT_OF_RANGE        ((uint32_t)0x80000000)
#define SD_OCR_ADDR_MISALIGNED          ((uint32_t)0x40000000)
#define SD_OCR_BLOCK_LEN_ERR            ((uint32_t)0x20000000)
#define SD_OCR_ERASE_SEQ_ERR            ((uint32_t)0x10000000)
#define SD_OCR_BAD_ERASE_PARAM          ((uint32_t)0x08000000)
#define SD_OCR_WRITE_PROT_VIOLATION     ((uint32_t)0x04000000)
#define SD_OCR_LOCK_UNLOCK_FAILED       ((uint32_t)0x01000000)
#define SD_OCR_COM_CRC_FAILED           ((uint32_t)0x00800000)
#define SD_OCR_ILLEGAL_CMD              ((uint32_t)0x00400000)
#define SD_OCR_CARD_ECC_FAILED          ((uint32_t)0x00200000)
#define SD_OCR_CC_ERROR                 ((uint32_t)0x00100000)
#define SD_OCR_GENERAL_UNKNOWN_ERROR    ((uint32_t)0x00080000)
#define SD_OCR_STREAM_READ_UNDERRUN     ((uint32_t)0x00040000)
#define SD_OCR_STREAM_WRITE_OVERRUN     ((uint32_t)0x00020000)
#define SD_OCR_CID_CSD_OVERWRIETE       ((uint32_t)0x00010000)
#define SD_OCR_WP_ERASE_SKIP            ((uint32_t)0x00008000)
#define SD_OCR_CARD_ECC_DISABLED        ((uint32_t)0x00004000)
#define SD_OCR_ERASE_RESET              ((uint32_t)0x00002000)
#define SD_OCR_AKE_SEQ_ERROR            ((uint32_t)0x00000008)
#define SD_OCR_ERRORBITS                ((uint32_t)0xFDFFE008)

/** 
* @brief  Masks for R6 Response 
*/
#define SD_R6_GENERAL_UNKNOWN_ERROR     ((uint32_t)0x00002000)
#define SD_R6_ILLEGAL_CMD               ((uint32_t)0x00004000)
#define SD_R6_COM_CRC_FAILED            ((uint32_t)0x00008000)

#define SD_VOLTAGE_WINDOW_SD            ((uint32_t)0x80100000)
#define SD_HIGH_CAPACITY                ((uint32_t)0x40000000)
#define SD_STD_CAPACITY                 ((uint32_t)0x00000000)
#define SD_CHECK_PATTERN                ((uint32_t)0x000001AA)

#define SD_MAX_VOLT_TRIAL               ((uint32_t)0x0000FFFF)
#define SD_ALLZERO                      ((uint32_t)0x00000000)

#define SD_WIDE_BUS_SUPPORT             ((uint32_t)0x00040000)
#define SD_SINGLE_BUS_SUPPORT           ((uint32_t)0x00010000)
#define SD_CARD_LOCKED                  ((uint32_t)0x02000000)

#define SD_DATATIMEOUT                  ((uint32_t)0xFFFFFFFF)
#define SD_0TO7BITS                     ((uint32_t)0x000000FF)
#define SD_8TO15BITS                    ((uint32_t)0x0000FF00)
#define SD_16TO23BITS                   ((uint32_t)0x00FF0000)
#define SD_24TO31BITS                   ((uint32_t)0xFF000000)
#define SD_MAX_DATA_LENGTH              ((uint32_t)0x01FFFFFF)

#define SD_HALFFIFO                     ((uint32_t)0x00000008)
#define SD_HALFFIFOBYTES                ((uint32_t)0x00000020)

/** 
* @brief  Command Class Supported 
*/
#define SD_CCCC_LOCK_UNLOCK             ((uint32_t)0x00000080)
#define SD_CCCC_WRITE_PROT              ((uint32_t)0x00000040)
#define SD_CCCC_ERASE                   ((uint32_t)0x00000020)

/** 
* @brief  Following commands are SD Card Specific commands.
*         SDIO_APP_CMD should be sent before sending these commands. 
*/
#define SDIO_SEND_IF_COND               ((uint32_t)0x00000008)

static uint32_t CardType =  SDIO_STD_CAPACITY_SD_CARD_V1_1;
static uint32_t CSD_Tab[4], CID_Tab[4], RCA = 0;
static uint8_t SDSTATUS_Tab[16];
uint32_t StopCondition = 0;
SDCARD_Result TransferError = SD_OK;
uint32_t TransferEnd = 0, DMAEndOfTransfer = 0;

SD_CardInfo SDCardInfo;

static SDCARD_Result sdcard_cmd_error(void);
static SDCARD_Result sdcard_cmd_R1_error(uint8_t cmd);
static SDCARD_Result sdcard_cmd_R7_error(void);
static SDCARD_Result sdcard_cmd_R3_error(void);
static SDCARD_Result sdcard_cmd_R2_error(void);
static SDCARD_Result sdcard_cmd_R6_error(uint8_t cmd, uint16_t *prca);
static SDCARD_Result sdcard_wide_bus(bool enable);
static SDCARD_Result sdcard_is_busy(uint8_t *pstatus);
static SDCARD_Result sdcard_find_SCR(uint16_t rca, uint32_t *pscr);
// uint8_t convert_from_bytes_to_power_of_two(uint16_t NumberOfBytes);

SDCARD_Result sdcard_init(uint32_t apb_freq, uint32_t sdcard_freq)
{
	SDCARD_Result result = SD_OK;

	sdio_set_clock_divider(GET_INIT_DIVIDER(apb_freq));
  	sdio_set_ck_polarity(SDIO_CK_RISING_EDGE);
  	sdio_bypass_disable();
  	sdio_power_saving_disable();
  	sdio_set_wide_bus_mode(WIDE_BUS_1);
  	sdio_flow_control_disable();

  	result = sdcard_power_on();

  	if (result != SD_OK)
  		return result;

  	result = sdcard_cards_init();

  	if (result != SD_OK)
  		return result;

  	sdio_set_clock_divider(GET_TRANSFER_DIVIDER(apb_freq, sdcard_freq));

   	result = sdcard_get_card_info(&SDCardInfo);

  	if (result == SD_OK)
    	result = sdcard_select_deselect((uint32_t)(SDCardInfo.RCA << 16));

  	if (result == SD_OK)
    	result = sdcard_enable_wide_bus_operation(WIDE_BUS_4);

	return (result);
}

SDCARD_Result sdcard_power_on(void)
{
	SDCARD_Result result = SD_OK;
  	uint32_t response = 0, count = 0, validvoltage = 0;
 	uint32_t SDType = SD_STD_CAPACITY;

  	sdio_enable();
  	sdio_ck_enable();

  	sdio_send_command(SD_CMD_GO_IDLE_STATE, 0, RESPONSE_NO, WAIT_NO, true);
  	result = sdcard_cmd_error();

  	if (result != SD_OK)
  		return result;

  	sdio_send_command(SDIO_SEND_IF_COND, SD_CHECK_PATTERN, RESPONSE_SHORT, WAIT_NO, true);
  	result = sdcard_cmd_R7_error();

  	if (result == SD_OK)
  	{
    	CardType = SDIO_STD_CAPACITY_SD_CARD_V2_0;
    	SDType = SD_HIGH_CAPACITY;
  	}

  	else
  	{
  		sdio_send_command(SD_CMD_APP_CMD, 0, RESPONSE_SHORT, WAIT_NO, true);
    	result = sdcard_cmd_R1_error(SD_CMD_APP_CMD);
  	}

  	sdio_send_command(SD_CMD_APP_CMD, 0, RESPONSE_SHORT, WAIT_NO, true);
  	result = sdcard_cmd_R1_error(SD_CMD_APP_CMD);

  	if (result == SD_OK)
  	{
    	while ((!validvoltage) && (count < SD_MAX_VOLT_TRIAL))
    	{
    		sdio_send_command(SD_CMD_APP_CMD, 0, RESPONSE_SHORT, WAIT_NO, true);
      		result = sdcard_cmd_R1_error(SD_CMD_APP_CMD);

  			if (result != SD_OK)
  				return result;

  			sdio_send_command(SD_CMD_SD_APP_OP_COND, (SD_VOLTAGE_WINDOW_SD | SDType), RESPONSE_SHORT, WAIT_NO, true);
      		result = sdcard_cmd_R3_error();

  			if (result != SD_OK)
  				return result;

      		response = sdio_get_card_status_1();
      		validvoltage = (((response >> 31) == 1) ? 1 : 0);
      		count++;
    	}

    	if (count >= SD_MAX_VOLT_TRIAL)
    	{
      		result = SD_INVALID_VOLTRANGE;
      		return(result);
    	}

    	if (response &= SD_HIGH_CAPACITY)
    	{
      		CardType = SDIO_HIGH_CAPACITY_SD_CARD;
    	}

	}

	return(result);
}

SDCARD_Result sdcard_power_off(void)
{
  	sdio_disable();
  	return SD_OK;
}

SDCARD_Result sdcard_cards_init(void)
{
	SDCARD_Result result = SD_OK;
  	uint16_t rca = 0x01;

  	if (SDIO_SECURE_DIGITAL_IO_CARD != CardType)
  	{
  		sdio_send_command(SD_CMD_ALL_SEND_CID, 0, RESPONSE_LONG, WAIT_NO, true);
    	result = sdcard_cmd_R2_error();

  		if (result != SD_OK)
  			return result;

    	CID_Tab[0] = sdio_get_card_status_1();
    	CID_Tab[1] = sdio_get_card_status_2();
    	CID_Tab[2] = sdio_get_card_status_3();
    	CID_Tab[3] = sdio_get_card_status_4();
  	}

  	if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) ||  
  		(SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) ||  
  		(SDIO_SECURE_DIGITAL_IO_COMBO_CARD == CardType) ||  
  		(SDIO_HIGH_CAPACITY_SD_CARD == CardType))
  	{
  		sdio_send_command(SD_CMD_SET_REL_ADDR, 0, RESPONSE_SHORT, WAIT_NO, true);
    	result = sdcard_cmd_R6_error(SD_CMD_SET_REL_ADDR, &rca);

  		if (result != SD_OK)
  			return result;
  	}

  	if (SDIO_SECURE_DIGITAL_IO_CARD != CardType)
  	{
    	RCA = rca;
    	sdio_send_command(SD_CMD_SEND_CSD, (uint32_t)(rca << 16), RESPONSE_LONG, WAIT_NO, true);
    	result = sdcard_cmd_R2_error();

  		if (result != SD_OK)
  			return result;

    	CSD_Tab[0] = sdio_get_card_status_1();
    	CSD_Tab[1] = sdio_get_card_status_2();
    	CSD_Tab[2] = sdio_get_card_status_3();
    	CSD_Tab[3] = sdio_get_card_status_4();
  	}

  	result = SD_OK; 
  	return(result);
}

SDCARD_Result sdcard_get_card_info(SD_CardInfo *cardinfo)
{
	SDCARD_Result result = SD_OK;
  	uint8_t tmp = 0;

  	cardinfo->CardType = (uint8_t)CardType;
  	cardinfo->RCA = (uint16_t)RCA;

  	/*!< Byte 0 */
  	tmp = (uint8_t)((CSD_Tab[0] & 0xFF000000) >> 24);
  	cardinfo->SD_csd.CSDStruct = (tmp & 0xC0) >> 6;
  	cardinfo->SD_csd.SysSpecVersion = (tmp & 0x3C) >> 2;
  	cardinfo->SD_csd.Reserved1 = tmp & 0x03;

  	/*!< Byte 1 */
  	tmp = (uint8_t)((CSD_Tab[0] & 0x00FF0000) >> 16);
  	cardinfo->SD_csd.TAAC = tmp;

  	/*!< Byte 2 */
  	tmp = (uint8_t)((CSD_Tab[0] & 0x0000FF00) >> 8);
  	cardinfo->SD_csd.NSAC = tmp;

  	/*!< Byte 3 */
  	tmp = (uint8_t)(CSD_Tab[0] & 0x000000FF);
  	cardinfo->SD_csd.MaxBusClkFrec = tmp;

  	/*!< Byte 4 */
  	tmp = (uint8_t)((CSD_Tab[1] & 0xFF000000) >> 24);
  	cardinfo->SD_csd.CardComdClasses = tmp << 4;

  	/*!< Byte 5 */
  	tmp = (uint8_t)((CSD_Tab[1] & 0x00FF0000) >> 16);
  	cardinfo->SD_csd.CardComdClasses |= (tmp & 0xF0) >> 4;
  	cardinfo->SD_csd.RdBlockLen = tmp & 0x0F;

  	/*!< Byte 6 */
  	tmp = (uint8_t)((CSD_Tab[1] & 0x0000FF00) >> 8);
  	cardinfo->SD_csd.PartBlockRead = (tmp & 0x80) >> 7;
  	cardinfo->SD_csd.WrBlockMisalign = (tmp & 0x40) >> 6;
  	cardinfo->SD_csd.RdBlockMisalign = (tmp & 0x20) >> 5;
  	cardinfo->SD_csd.DSRImpl = (tmp & 0x10) >> 4;
  	cardinfo->SD_csd.Reserved2 = 0; /*!< Reserved */

  	if ((CardType == SDIO_STD_CAPACITY_SD_CARD_V1_1) || (CardType == SDIO_STD_CAPACITY_SD_CARD_V2_0))
  	{
    	cardinfo->SD_csd.DeviceSize = (tmp & 0x03) << 10;

    	/*!< Byte 7 */
    	tmp = (uint8_t)(CSD_Tab[1] & 0x000000FF);
    	cardinfo->SD_csd.DeviceSize |= (tmp) << 2;

    	/*!< Byte 8 */
    	tmp = (uint8_t)((CSD_Tab[2] & 0xFF000000) >> 24);
    	cardinfo->SD_csd.DeviceSize |= (tmp & 0xC0) >> 6;

    	cardinfo->SD_csd.MaxRdCurrentVDDMin = (tmp & 0x38) >> 3;
    	cardinfo->SD_csd.MaxRdCurrentVDDMax = (tmp & 0x07);

    	/*!< Byte 9 */
    	tmp = (uint8_t)((CSD_Tab[2] & 0x00FF0000) >> 16);
    	cardinfo->SD_csd.MaxWrCurrentVDDMin = (tmp & 0xE0) >> 5;
    	cardinfo->SD_csd.MaxWrCurrentVDDMax = (tmp & 0x1C) >> 2;
    	cardinfo->SD_csd.DeviceSizeMul = (tmp & 0x03) << 1;

    	/*!< Byte 10 */
    	tmp = (uint8_t)((CSD_Tab[2] & 0x0000FF00) >> 8);
    	cardinfo->SD_csd.DeviceSizeMul |= (tmp & 0x80) >> 7;
    
    	cardinfo->CardCapacity = (cardinfo->SD_csd.DeviceSize + 1) ;
    	cardinfo->CardCapacity *= (1 << (cardinfo->SD_csd.DeviceSizeMul + 2));
    	cardinfo->CardBlockSize = 1 << (cardinfo->SD_csd.RdBlockLen);
    	cardinfo->CardCapacity *= cardinfo->CardBlockSize;
  	}
  	else if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
  	{
    	/*!< Byte 7 */
    	tmp = (uint8_t)(CSD_Tab[1] & 0x000000FF);
    	cardinfo->SD_csd.DeviceSize = (tmp & 0x3F) << 16;

    	/*!< Byte 8 */
    	tmp = (uint8_t)((CSD_Tab[2] & 0xFF000000) >> 24);

    	cardinfo->SD_csd.DeviceSize |= (tmp << 8);

    	/*!< Byte 9 */
    	tmp = (uint8_t)((CSD_Tab[2] & 0x00FF0000) >> 16);

    	cardinfo->SD_csd.DeviceSize |= (tmp);

    	/*!< Byte 10 */
    	tmp = (uint8_t)((CSD_Tab[2] & 0x0000FF00) >> 8);
    
    	cardinfo->CardCapacity = ((uint64_t)cardinfo->SD_csd.DeviceSize + 1) * 512 * 1024;
    	cardinfo->CardBlockSize = 512;    
  	}


  	cardinfo->SD_csd.EraseGrSize = (tmp & 0x40) >> 6;
  	cardinfo->SD_csd.EraseGrMul = (tmp & 0x3F) << 1;

  	/*!< Byte 11 */
  	tmp = (uint8_t)(CSD_Tab[2] & 0x000000FF);
  	cardinfo->SD_csd.EraseGrMul |= (tmp & 0x80) >> 7;
  	cardinfo->SD_csd.WrProtectGrSize = (tmp & 0x7F);

  	/*!< Byte 12 */
  	tmp = (uint8_t)((CSD_Tab[3] & 0xFF000000) >> 24);
  	cardinfo->SD_csd.WrProtectGrEnable = (tmp & 0x80) >> 7;
  	cardinfo->SD_csd.ManDeflECC = (tmp & 0x60) >> 5;
  	cardinfo->SD_csd.WrSpeedFact = (tmp & 0x1C) >> 2;
  	cardinfo->SD_csd.MaxWrBlockLen = (tmp & 0x03) << 2;

  	/*!< Byte 13 */
  	tmp = (uint8_t)((CSD_Tab[3] & 0x00FF0000) >> 16);
  	cardinfo->SD_csd.MaxWrBlockLen |= (tmp & 0xC0) >> 6;
  	cardinfo->SD_csd.WriteBlockPaPartial = (tmp & 0x20) >> 5;
  	cardinfo->SD_csd.Reserved3 = 0;
  	cardinfo->SD_csd.ContentProtectAppli = (tmp & 0x01);

  	/*!< Byte 14 */
  	tmp = (uint8_t)((CSD_Tab[3] & 0x0000FF00) >> 8);
  	cardinfo->SD_csd.FileFormatGrouop = (tmp & 0x80) >> 7;
  	cardinfo->SD_csd.CopyFlag = (tmp & 0x40) >> 6;
  	cardinfo->SD_csd.PermWrProtect = (tmp & 0x20) >> 5;
  	cardinfo->SD_csd.TempWrProtect = (tmp & 0x10) >> 4;
  	cardinfo->SD_csd.FileFormat = (tmp & 0x0C) >> 2;
  	cardinfo->SD_csd.ECC = (tmp & 0x03);

  	/*!< Byte 15 */
  	tmp = (uint8_t)(CSD_Tab[3] & 0x000000FF);
  	cardinfo->SD_csd.CSD_CRC = (tmp & 0xFE) >> 1;
  	cardinfo->SD_csd.Reserved4 = 1;

  	/*!< Byte 0 */
  	tmp = (uint8_t)((CID_Tab[0] & 0xFF000000) >> 24);
  	cardinfo->SD_cid.ManufacturerID = tmp;

  	/*!< Byte 1 */
  	tmp = (uint8_t)((CID_Tab[0] & 0x00FF0000) >> 16);
  	cardinfo->SD_cid.OEM_AppliID = tmp << 8;

  	/*!< Byte 2 */
  	tmp = (uint8_t)((CID_Tab[0] & 0x000000FF00) >> 8);
  	cardinfo->SD_cid.OEM_AppliID |= tmp;

  	/*!< Byte 3 */
  	tmp = (uint8_t)(CID_Tab[0] & 0x000000FF);
  	cardinfo->SD_cid.ProdName1 = tmp << 24;

  	/*!< Byte 4 */
  	tmp = (uint8_t)((CID_Tab[1] & 0xFF000000) >> 24);
  	cardinfo->SD_cid.ProdName1 |= tmp << 16;

  	/*!< Byte 5 */
  	tmp = (uint8_t)((CID_Tab[1] & 0x00FF0000) >> 16);
  	cardinfo->SD_cid.ProdName1 |= tmp << 8;

  	/*!< Byte 6 */
  	tmp = (uint8_t)((CID_Tab[1] & 0x0000FF00) >> 8);
  	cardinfo->SD_cid.ProdName1 |= tmp;

  	/*!< Byte 7 */
  	tmp = (uint8_t)(CID_Tab[1] & 0x000000FF);
  	cardinfo->SD_cid.ProdName2 = tmp;

  	/*!< Byte 8 */
  	tmp = (uint8_t)((CID_Tab[2] & 0xFF000000) >> 24);
  	cardinfo->SD_cid.ProdRev = tmp;

  	/*!< Byte 9 */
  	tmp = (uint8_t)((CID_Tab[2] & 0x00FF0000) >> 16);
  	cardinfo->SD_cid.ProdSN = tmp << 24;

  	/*!< Byte 10 */
  	tmp = (uint8_t)((CID_Tab[2] & 0x0000FF00) >> 8);
  	cardinfo->SD_cid.ProdSN |= tmp << 16;

  	/*!< Byte 11 */
  	tmp = (uint8_t)(CID_Tab[2] & 0x000000FF);
  	cardinfo->SD_cid.ProdSN |= tmp << 8;

  	/*!< Byte 12 */
  	tmp = (uint8_t)((CID_Tab[3] & 0xFF000000) >> 24);
  	cardinfo->SD_cid.ProdSN |= tmp;

  	/*!< Byte 13 */
  	tmp = (uint8_t)((CID_Tab[3] & 0x00FF0000) >> 16);
  	cardinfo->SD_cid.Reserved1 |= (tmp & 0xF0) >> 4;
  	cardinfo->SD_cid.ManufactDate = (tmp & 0x0F) << 8;

  	/*!< Byte 14 */
  	tmp = (uint8_t)((CID_Tab[3] & 0x0000FF00) >> 8);
  	cardinfo->SD_cid.ManufactDate |= tmp;

  	/*!< Byte 15 */
  	tmp = (uint8_t)(CID_Tab[3] & 0x000000FF);
  	cardinfo->SD_cid.CID_CRC = (tmp & 0xFE) >> 1;
  	cardinfo->SD_cid.Reserved2 = 1;
  
  return(result);
}

SDCARD_Result sdcard_select_deselect(uint64_t addr)
{
	SDCARD_Result result;
  	sdio_send_command(SD_CMD_SEL_DESEL_CARD, (uint32_t)addr, RESPONSE_SHORT, WAIT_NO, true);
  	result = sdcard_cmd_R1_error(SD_CMD_SEL_DESEL_CARD);
  	return(result);
}

SDCARD_Result sdcard_enable_wide_bus_operation(SDIO_WideBusMode mode)
{
	SDCARD_Result result = SD_OK;

  	if (SDIO_MULTIMEDIA_CARD == CardType)
  	{
    	result = SD_UNSUPPORTED_FEATURE;
    	return(result);
  	}

  	else if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType))
  	{
  		switch(mode)
  		{
  			case WIDE_BUS_8:
  				result = SD_UNSUPPORTED_FEATURE;
      			return(result);

      		case WIDE_BUS_4:
      			result = sdcard_wide_bus(true);
      			if (result == SD_OK)
        			sdio_set_wide_bus_mode(WIDE_BUS_4);
        		break;

        	case WIDE_BUS_1:
        	default:
        		result = sdcard_wide_bus(false);
        		if (result == SD_OK)
        			sdio_set_wide_bus_mode(WIDE_BUS_1);
        		break;
  		}
  	}

  	return(result);
}

static SDCARD_Result sdcard_cmd_error(void)
{
	SDCARD_Result result = SD_OK;
	uint32_t timeout;

  	timeout = SDIO_CMD0TIMEOUT; /*!< 10000 */

  	while ((timeout > 0) && !sdio_get_flag_status(SDIO_CMDSENT))
  	{
    	timeout--;
  	}

  	if (timeout == 0)
  	{
    	result = SD_CMD_RSP_TIMEOUT;
    	return(result);
  	}

  	sdio_clear_all_flag_status(SDIO_STATIC_FLAGS);
  	return (result);
}

static SDCARD_Result sdcard_cmd_R7_error(void)
{
  	SDCARD_Result result = SD_OK;
  	uint32_t timeout = SDIO_CMD0TIMEOUT;

  	while (!sdio_get_flag_status(SDIO_CCRCFAIL) &&
  		   !sdio_get_flag_status(SDIO_CMDREND)  &&
  		   !sdio_get_flag_status(SDIO_CTIMEOUT) &&
  		   (timeout > 0))

  	{
    	timeout--;
  	}

  	if ((timeout == 0) || sdio_get_flag_status(SDIO_CTIMEOUT))
  	{
    	result = SD_CMD_RSP_TIMEOUT;
    	sdio_clear_flag_status(SDIO_CTIMEOUT);
    	return(result);
  	}

  	if (sdio_get_flag_status(SDIO_CMDREND))
  	{
    	result = SD_OK;
    	sdio_clear_flag_status(SDIO_CMDREND);
    	return(result);
  	}

  	return (result);
}

static SDCARD_Result sdcard_cmd_R1_error(uint8_t cmd) 
{
  	SDCARD_Result result = SD_OK;
  	uint32_t response_r1;

    while (!sdio_get_flag_status(SDIO_CCRCFAIL) &&
  		   !sdio_get_flag_status(SDIO_CMDREND)  &&
  		   !sdio_get_flag_status(SDIO_CTIMEOUT));


  	if (sdio_get_flag_status(SDIO_CTIMEOUT))
  	{
    	result = SD_CMD_RSP_TIMEOUT;
    	sdio_clear_flag_status(SDIO_CTIMEOUT);
    	return(result);
  	}

  	else if (sdio_get_flag_status(SDIO_CCRCFAIL))
  	{
    	result = SD_CMD_CRC_FAIL;
    	sdio_clear_flag_status(SDIO_CCRCFAIL);
    	return(result);
  	}

  	if (sdio_get_cmd_response() != cmd)
  	{
    	result = SD_ILLEGAL_CMD;
    	return(result);
  	}

  	sdio_clear_all_flag_status(SDIO_STATIC_FLAGS);

  	response_r1 = sdio_get_card_status_1();

  	if ((response_r1 & SD_OCR_ERRORBITS) == SD_ALLZERO)
 	{
    	return(result);
  	}

  	if (response_r1 & SD_OCR_ADDR_OUT_OF_RANGE)
	{
		return(SD_ADDR_OUT_OF_RANGE);
	}

  	if (response_r1 & SD_OCR_ADDR_MISALIGNED)
  	{
   	 	return(SD_ADDR_MISALIGNED);
  	}

  	if (response_r1 & SD_OCR_BLOCK_LEN_ERR)
  	{
   	 	return(SD_BLOCK_LEN_ERR);
  	}

  	if (response_r1 & SD_OCR_ERASE_SEQ_ERR)
  	{
    	return(SD_ERASE_SEQ_ERR);
  	}

  	if (response_r1 & SD_OCR_BAD_ERASE_PARAM)
  	{
    	return(SD_BAD_ERASE_PARAM);
  	}

  	if (response_r1 & SD_OCR_WRITE_PROT_VIOLATION)
  	{
    	return(SD_WRITE_PROT_VIOLATION);
  	}

  	if (response_r1 & SD_OCR_LOCK_UNLOCK_FAILED)
  	{
    	return(SD_LOCK_UNLOCK_FAILED);
  	}

  	if (response_r1 & SD_OCR_COM_CRC_FAILED)
  	{
    	return(SD_COM_CRC_FAILED);
  	}

  	if (response_r1 & SD_OCR_ILLEGAL_CMD)
  	{
    	return(SD_ILLEGAL_CMD);
  	}

  	if (response_r1 & SD_OCR_CARD_ECC_FAILED)
  	{
    	return(SD_CARD_ECC_FAILED);
  	}

  	if (response_r1 & SD_OCR_CC_ERROR)
  	{
    	return(SD_CC_ERROR);
  	}

  	if (response_r1 & SD_OCR_GENERAL_UNKNOWN_ERROR)
  	{
    	return(SD_GENERAL_UNKNOWN_ERROR);
  	}

  	if (response_r1 & SD_OCR_STREAM_READ_UNDERRUN)
  	{
    	return(SD_STREAM_READ_UNDERRUN);
  	}

  	if (response_r1 & SD_OCR_STREAM_WRITE_OVERRUN)
  	{
    	return(SD_STREAM_WRITE_OVERRUN);
  	}

  	if (response_r1 & SD_OCR_CID_CSD_OVERWRIETE)
  	{
    	return(SD_CID_CSD_OVERWRITE);
  	}

  	if (response_r1 & SD_OCR_WP_ERASE_SKIP)
  	{
    	return(SD_WP_ERASE_SKIP);
  	}

  	if (response_r1 & SD_OCR_CARD_ECC_DISABLED)
  	{
    	return(SD_CARD_ECC_DISABLED);
  	}

  	if (response_r1 & SD_OCR_ERASE_RESET)
  	{
    	return(SD_ERASE_RESET);
  	}

  	if (response_r1 & SD_OCR_AKE_SEQ_ERROR)
  	{
    	return(SD_AKE_SEQ_ERROR);
  	}
  	
  	return(result);
}

static SDCARD_Result sdcard_cmd_R3_error(void)
{
  	SDCARD_Result result = SD_OK;

  	while (!sdio_get_flag_status(SDIO_CCRCFAIL) &&
  		   !sdio_get_flag_status(SDIO_CMDREND)  &&
  		   !sdio_get_flag_status(SDIO_CTIMEOUT));

  	if (sdio_get_flag_status(SDIO_CTIMEOUT))
 	{
    	result = SD_CMD_RSP_TIMEOUT;
    	sdio_clear_flag_status(SDIO_CTIMEOUT);
    	return(result);
  	}

  	sdio_clear_all_flag_status(SDIO_STATIC_FLAGS);
  	return(result);
}

static SDCARD_Result sdcard_cmd_R2_error(void) // sdcard_cmd_R2_error(void)
{
  	SDCARD_Result result = SD_OK;

  	while (!sdio_get_flag_status(SDIO_CCRCFAIL) &&
  		   !sdio_get_flag_status(SDIO_CMDREND)  &&
  		   !sdio_get_flag_status(SDIO_CTIMEOUT));

  	if (sdio_get_flag_status(SDIO_CTIMEOUT))
  	{
    	result = SD_CMD_RSP_TIMEOUT;
    	sdio_clear_flag_status(SDIO_CTIMEOUT);
    	return(result);
  	}

  	else if (sdio_get_flag_status(SDIO_CCRCFAIL))
  	{
    	result = SD_CMD_CRC_FAIL;
    	sdio_clear_flag_status(SDIO_CCRCFAIL);
    	return(result);
  	}

  	sdio_clear_all_flag_status(SDIO_STATIC_FLAGS);
  	return(result);
}

static SDCARD_Result sdcard_cmd_R6_error(uint8_t cmd, uint16_t *prca) //sdcard_cmd_R6_error(uint8_t cmd, uint16_t *prca)
{
  	SDCARD_Result result = SD_OK;
  	uint32_t response_r1;

  	while (!sdio_get_flag_status(SDIO_CCRCFAIL) &&
  		   !sdio_get_flag_status(SDIO_CMDREND)  &&
  		   !sdio_get_flag_status(SDIO_CTIMEOUT));


  	if (sdio_get_flag_status(SDIO_CTIMEOUT))
  	{
    	result = SD_CMD_RSP_TIMEOUT;
    	sdio_clear_flag_status(SDIO_CTIMEOUT);
    	return(result);
  	}

  	else if (sdio_get_flag_status(SDIO_CCRCFAIL))
  	{
    	result = SD_CMD_CRC_FAIL;
    	sdio_clear_flag_status(SDIO_CCRCFAIL);
    	return(result);
  	}

  	if (sdio_get_cmd_response() != cmd)
  	{
    	result = SD_ILLEGAL_CMD;
    	return(result);
  	}

  	sdio_clear_all_flag_status(SDIO_STATIC_FLAGS);
  	response_r1 = sdio_get_card_status_1();

  	if (SD_ALLZERO == (response_r1 & (SD_R6_GENERAL_UNKNOWN_ERROR | SD_R6_ILLEGAL_CMD | SD_R6_COM_CRC_FAILED)))
  	{
    	*prca = (uint16_t) (response_r1 >> 16);
    	return(result);
  	}

  	if (response_r1 & SD_R6_GENERAL_UNKNOWN_ERROR)
  	{
    	return(SD_GENERAL_UNKNOWN_ERROR);
  	}

  	if (response_r1 & SD_R6_ILLEGAL_CMD)
  	{
    	return(SD_ILLEGAL_CMD);
  	}

  	if (response_r1 & SD_R6_COM_CRC_FAILED)
  	{
    	return(SD_COM_CRC_FAILED);
  	}

  	return(result);
}

SDCARD_Result sdcard_read_block(uint8_t *readbuff, uint64_t ReadAddr, uint16_t BlockSize)
{
	SDCARD_Result result = SD_OK;

  	uint32_t count = 0, *tempbuff = (uint32_t *)readbuff;

  	TransferError = SD_OK;
  	TransferEnd = 0;
  	StopCondition = 0;

  	if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
  	{
    	BlockSize = 512;
    	ReadAddr /= 512;
  	}

  	sdio_send_command(SD_CMD_SET_BLOCKLEN, (uint32_t)BlockSize, RESPONSE_SHORT, WAIT_NO, true);
  	result = sdcard_cmd_R1_error(SD_CMD_SET_BLOCKLEN);

  	if (result != SD_OK)
  		return result;

	sdio_set_timeout_period(SD_DATATIMEOUT);
	sdio_set_data_length(BlockSize);
	sdio_set_data_block_size(BLOCK_LENGTH_512_B);
	sdio_set_data_transfer_direction(CONTROLLER_TO_CARD);
	sdio_set_data_transfer_mode(BLOCK_DATA_TRANSFER);
	sdio_data_transfer_enable();

	sdio_send_command(SD_CMD_READ_SINGLE_BLOCK, (uint32_t)ReadAddr, RESPONSE_SHORT, WAIT_NO, true);
  	result = sdcard_cmd_R1_error(SD_CMD_READ_SINGLE_BLOCK);

  	if (result != SD_OK)
  		return result;

  	while (!sdio_get_flag_status(SDIO_RXOVERR) && 
  		   !sdio_get_flag_status(SDIO_DCRCFAIL) &&
  		   !sdio_get_flag_status(SDIO_DTIMEOUT) &&
  		   !sdio_get_flag_status(SDIO_DBCKEND) &&  
  		   !sdio_get_flag_status(SDIO_STBITERR))
  	{
    	if (sdio_get_flag_status(SDIO_RXFIFOHF))
    	{
      		for (count = 0; count < 8; count++)
      		{
        		*(tempbuff + count) = sdio_read_fifo_data();
      		}
      		tempbuff += 8;
    	}
  	}

  	if (sdio_get_flag_status(SDIO_DTIMEOUT))
  	{
    	sdio_clear_flag_status(SDIO_DTIMEOUT);
    	result = SD_DATA_TIMEOUT;
    	return(result);
  	}
  	else if (sdio_get_flag_status(SDIO_DCRCFAIL))
  	{
    	sdio_clear_flag_status(SDIO_DCRCFAIL);
    	result = SD_DATA_CRC_FAIL;
    	return(result);
  	}
  	else if (sdio_get_flag_status(SDIO_RXOVERR))
  	{
  	 	sdio_clear_flag_status(SDIO_RXOVERR);
    	result = SD_RX_OVERRUN;
    	return(result);
  	}
  	else if (sdio_get_flag_status(SDIO_STBITERR))
  	{
    	sdio_clear_flag_status(SDIO_STBITERR);
    	result = SD_START_BIT_ERR;
    	return(result);
  	}
  	count = SD_DATATIMEOUT;
  	while (sdio_get_flag_status(SDIO_RXDAVL) && (count > 0))
  	{
    	*tempbuff = sdio_read_fifo_data();
    	tempbuff++;
    	count--;
  	}

	sdio_clear_all_flag_status(SDIO_STATIC_FLAGS);
  	return(result);
}

SDCARD_Result sdcard_write_block(uint8_t *writebuff, uint64_t WriteAddr, uint16_t BlockSize)
{
	SDCARD_Result result = SD_OK;

  	uint32_t bytestransferred = 0, count = 0, restwords = 0;
  	uint32_t *tempbuff = (uint32_t *)writebuff;

  	if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
  	{
    	BlockSize = 512;
    	WriteAddr /= 512;
  	}

 	sdio_send_command(SD_CMD_SET_BLOCKLEN, (uint32_t)BlockSize, RESPONSE_SHORT, WAIT_NO, true);
  	result = sdcard_cmd_R1_error(SD_CMD_SET_BLOCKLEN);

  	if (result != SD_OK)
    	return(result);

    sdio_send_command(SD_CMD_WRITE_SINGLE_BLOCK, (uint32_t)WriteAddr, RESPONSE_SHORT, WAIT_NO, true);
  	result = sdcard_cmd_R1_error(SD_CMD_WRITE_SINGLE_BLOCK);

  	if (result != SD_OK)
    	return(result);

    sdio_set_timeout_period(SD_DATATIMEOUT);
	sdio_set_data_length(BlockSize);
	sdio_set_data_block_size(BLOCK_LENGTH_512_B);
	sdio_set_data_transfer_direction(CARD_TO_CONTROLLER);
	sdio_set_data_transfer_mode(BLOCK_DATA_TRANSFER);
	sdio_data_transfer_enable();

	while (!sdio_get_flag_status(SDIO_DBCKEND) && 
  		   !sdio_get_flag_status(SDIO_TXUNDERR) &&
  		   !sdio_get_flag_status(SDIO_DCRCFAIL) &&
  		   !sdio_get_flag_status(SDIO_DTIMEOUT) &&  
  		   !sdio_get_flag_status(SDIO_STBITERR))
  	{
    	if (sdio_get_flag_status(SDIO_TXFIFOHE))
    	{
      		if ((512 - bytestransferred) < 32)
      		{
        		restwords = ((512 - bytestransferred) % 4 == 0) ? ((512 - bytestransferred) / 4) : (( 512 -  bytestransferred) / 4 + 1);
        		for (count = 0; count < restwords; count++, tempbuff++, bytestransferred += 4)
        		{
          			sdio_write_fifo_data(*tempbuff);
        		}
      		}
      		else
      		{
        		for (count = 0; count < 8; count++)
        		{
          			sdio_write_fifo_data(*(tempbuff + count));
        		}

        		tempbuff += 8;
        		bytestransferred += 32;
      		}
    	}
  	}

  	if (sdio_get_flag_status(SDIO_DTIMEOUT))
  	{
    	sdio_clear_flag_status(SDIO_DTIMEOUT);
    	result = SD_DATA_TIMEOUT;
    	return(result);
  	}
  	else if (sdio_get_flag_status(SDIO_DCRCFAIL))
  	{
    	sdio_clear_flag_status(SDIO_DCRCFAIL);
    	result = SD_DATA_CRC_FAIL;
    	return(result);
  	}
  	else if (sdio_get_flag_status(SDIO_TXUNDERR))
  	{
    	sdio_clear_flag_status(SDIO_TXUNDERR);
    	result = SD_TX_UNDERRUN;
    	return(result);
  	}
  	else if (sdio_get_flag_status(SDIO_STBITERR))
  	{
    	sdio_clear_flag_status(SDIO_STBITERR);
    	result = SD_START_BIT_ERR;
    	return(result);
  	}
  
  	return(result);
}

static SDCARD_Result sdcard_wide_bus(bool enable)
{
  	SDCARD_Result result = SD_OK;
  	uint32_t scr[2] = {0, 0};

  	if (sdio_get_card_status_1() & SD_CARD_LOCKED)
  	{
    	result = SD_LOCK_UNLOCK_FAILED;
    	return(result);
  	}

  	result = sdcard_find_SCR(RCA, scr);

  	if (result != SD_OK)
    	return(result);

  	if (enable)
  	{
    	/*!< If requested card supports wide bus operation */
    	if ((scr[1] & SD_WIDE_BUS_SUPPORT) != SD_ALLZERO)
    	{
    		sdio_send_command(SD_CMD_APP_CMD, (uint32_t)(RCA << 16), RESPONSE_SHORT, WAIT_NO, true);
      		result = sdcard_cmd_R1_error(SD_CMD_APP_CMD);

  			if (result != SD_OK)
    			return(result);

    		sdio_send_command(SD_CMD_APP_SD_SET_BUSWIDTH, (uint32_t)(0x2), RESPONSE_SHORT, WAIT_NO, true);
      		result = sdcard_cmd_R1_error(SD_CMD_APP_SD_SET_BUSWIDTH);

      		if (result != SD_OK)
    			return(result);

      		return(result);
    	}

    	else
    	{
      		result = SD_REQUEST_NOT_APPLICABLE;
      		return(result);
    	}
  	} 

 	else
  	{
    /*!< If requested card supports 1 bit mode operation */
	    if ((scr[1] & SD_SINGLE_BUS_SUPPORT) != SD_ALLZERO)
	    {
	    	sdio_send_command(SD_CMD_APP_CMD, (uint32_t)(RCA << 16), RESPONSE_SHORT, WAIT_NO, true);
      		result = sdcard_cmd_R1_error(SD_CMD_APP_CMD);

  			if (result != SD_OK)
    			return(result);

    		sdio_send_command(SD_CMD_APP_SD_SET_BUSWIDTH, (uint32_t)(0x0), RESPONSE_SHORT, WAIT_NO, true);
    		result = sdcard_cmd_R1_error(SD_CMD_APP_SD_SET_BUSWIDTH);

    		if (result != SD_OK)
    			return(result);

      		return(result);
    	}

    	else
    	{
      		result = SD_REQUEST_NOT_APPLICABLE;
      		return(result);
    	}
  	}
}

static SDCARD_Result sdcard_find_SCR(uint16_t rca, uint32_t *pscr)
{
	uint32_t index = 0;
  	SDCARD_Result result = SD_OK;
  	uint32_t tempscr[2] = {0, 0};

  	sdio_send_command(SD_CMD_SET_BLOCKLEN, (uint32_t)(0x8), RESPONSE_SHORT, WAIT_NO, true);
  	result = sdcard_cmd_R1_error(SD_CMD_SET_BLOCKLEN);

  	if (result != SD_OK)
    	return(result);

    sdio_send_command(SD_CMD_APP_CMD, (uint32_t)(rca << 16), RESPONSE_SHORT, WAIT_NO, true);
  	result = sdcard_cmd_R1_error(SD_CMD_APP_CMD);

  	if (result != SD_OK)
    	return(result);

    sdio_set_timeout_period(SD_DATATIMEOUT);
	sdio_set_data_length(8);
	sdio_set_data_block_size(BLOCK_LENGTH_8_B);
	sdio_set_data_transfer_direction(CONTROLLER_TO_CARD);
	sdio_set_data_transfer_mode(BLOCK_DATA_TRANSFER);
	sdio_data_transfer_enable();

	sdio_send_command(SD_CMD_SD_APP_SEND_SCR, (uint32_t)(0x0), RESPONSE_SHORT, WAIT_NO, true);
  	result = sdcard_cmd_R1_error(SD_CMD_SD_APP_SEND_SCR);

  	if (result != SD_OK)
    	return(result);

   	while (!sdio_get_flag_status(SDIO_RXOVERR) && 
  		   !sdio_get_flag_status(SDIO_DCRCFAIL) &&
  		   !sdio_get_flag_status(SDIO_DTIMEOUT) &&
  		   !sdio_get_flag_status(SDIO_DBCKEND) &&  
  		   !sdio_get_flag_status(SDIO_STBITERR))
  	{
    	if (sdio_get_flag_status(SDIO_RXDAVL))
    	{
      		*(tempscr + index) = sdio_read_fifo_data();
      		index++;
    	}
  	}

  	if (sdio_get_flag_status(SDIO_DTIMEOUT))
  	{
    	sdio_clear_flag_status(SDIO_DTIMEOUT);
    	result = SD_DATA_TIMEOUT;
    	return(result);
  	}

  	else if (sdio_get_flag_status(SDIO_DCRCFAIL))
  	{
    	sdio_clear_flag_status(SDIO_DCRCFAIL);
    	result = SD_DATA_CRC_FAIL;
    	return(result);
  	}
  	else if (sdio_get_flag_status(SDIO_RXOVERR))
  	{
    	sdio_clear_flag_status(SDIO_RXOVERR);
    	result = SD_RX_OVERRUN;
    	return(result);
  	}
  	else if (sdio_get_flag_status(SDIO_STBITERR))
  	{
    	sdio_clear_flag_status(SDIO_STBITERR);
    	result = SD_START_BIT_ERR;
    	return(result);
  	}

  	sdio_clear_all_flag_status(SDIO_STATIC_FLAGS);

  	*(pscr + 1) = ((tempscr[0] & SD_0TO7BITS) << 24) | ((tempscr[0] & SD_8TO15BITS) << 8) | ((tempscr[0] & SD_16TO23BITS) >> 8) | ((tempscr[0] & SD_24TO31BITS) >> 24);

  	*(pscr) = ((tempscr[1] & SD_0TO7BITS) << 24) | ((tempscr[1] & SD_8TO15BITS) << 8) | ((tempscr[1] & SD_16TO23BITS) >> 8) | ((tempscr[1] & SD_24TO31BITS) >> 24);

  	return(result);
}

uint8_t sdcard_detect(void)
{
  uint8_t status = SD_PRESENT;

  // /*!< Check GPIO to detect SD */
  // if (GPIO_ReadInputDataBit(SD_DETECT_GPIO_PORT, SD_DETECT_PIN) != Bit_RESET)
  // {
  //   status = SD_NOT_PRESENT;
  // }
  return status;
}

SDCARD_Result sdcard_get_card_status(SD_CardStatus *cardstatus)
{
	SDCARD_Result result = SD_OK;
  	uint8_t tmp = 0;

  	result = sdcard_send_status((uint32_t *)SDSTATUS_Tab);

  	if (result  != SD_OK)
    	return(result);

  	/*!< Byte 0 */
  	tmp = (uint8_t)((SDSTATUS_Tab[0] & 0xC0) >> 6);
  	cardstatus->DAT_BUS_WIDTH = tmp;

  	/*!< Byte 0 */
  	tmp = (uint8_t)((SDSTATUS_Tab[0] & 0x20) >> 5);
  	cardstatus->SECURED_MODE = tmp;

  	/*!< Byte 2 */
  	tmp = (uint8_t)((SDSTATUS_Tab[2] & 0xFF));
  	cardstatus->SD_CARD_TYPE = tmp << 8;

  	/*!< Byte 3 */
  	tmp = (uint8_t)((SDSTATUS_Tab[3] & 0xFF));
  	cardstatus->SD_CARD_TYPE |= tmp;

  	/*!< Byte 4 */
  	tmp = (uint8_t)(SDSTATUS_Tab[4] & 0xFF);
  	cardstatus->SIZE_OF_PROTECTED_AREA = tmp << 24;

  	/*!< Byte 5 */
  	tmp = (uint8_t)(SDSTATUS_Tab[5] & 0xFF);
  	cardstatus->SIZE_OF_PROTECTED_AREA |= tmp << 16;

  	/*!< Byte 6 */
  	tmp = (uint8_t)(SDSTATUS_Tab[6] & 0xFF);
  	cardstatus->SIZE_OF_PROTECTED_AREA |= tmp << 8;

  	/*!< Byte 7 */
  	tmp = (uint8_t)(SDSTATUS_Tab[7] & 0xFF);
  	cardstatus->SIZE_OF_PROTECTED_AREA |= tmp;

  	/*!< Byte 8 */
  	tmp = (uint8_t)((SDSTATUS_Tab[8] & 0xFF));
  	cardstatus->SPEED_CLASS = tmp;

  	/*!< Byte 9 */
  	tmp = (uint8_t)((SDSTATUS_Tab[9] & 0xFF));
  	cardstatus->PERFORMANCE_MOVE = tmp;

  	/*!< Byte 10 */
  	tmp = (uint8_t)((SDSTATUS_Tab[10] & 0xF0) >> 4);
  	cardstatus->AU_SIZE = tmp;

  	/*!< Byte 11 */
  	tmp = (uint8_t)(SDSTATUS_Tab[11] & 0xFF);
  	cardstatus->ERASE_SIZE = tmp << 8;

  	/*!< Byte 12 */
  	tmp = (uint8_t)(SDSTATUS_Tab[12] & 0xFF);
  	cardstatus->ERASE_SIZE |= tmp;

  	/*!< Byte 13 */
  	tmp = (uint8_t)((SDSTATUS_Tab[13] & 0xFC) >> 2);
  	cardstatus->ERASE_TIMEOUT = tmp;

  	/*!< Byte 13 */
  	tmp = (uint8_t)((SDSTATUS_Tab[13] & 0x3));
  	cardstatus->ERASE_OFFSET = tmp;
 
  	return(result);
}

SDCARD_Result sdcard_send_status(uint32_t *psdstatus)
{
	SDCARD_Result result = SD_OK;
  	uint32_t count = 0;

  	if (sdio_get_card_status_1() & SD_CARD_LOCKED)
  	{
    	result = SD_LOCK_UNLOCK_FAILED;
    	return(result);
  	}

  	sdio_send_command(SD_CMD_SET_BLOCKLEN, (uint32_t)(64), RESPONSE_SHORT, WAIT_NO, true);
  	result = sdcard_cmd_R1_error(SD_CMD_SET_BLOCKLEN);

  	if (result != SD_OK)
    	return(result);

    sdio_send_command(SD_CMD_APP_CMD, (uint32_t)(RCA << 16), RESPONSE_SHORT, WAIT_NO, true);
  	result = sdcard_cmd_R1_error(SD_CMD_APP_CMD);

  	if (result != SD_OK)
    return(result);

    sdio_set_timeout_period(SD_DATATIMEOUT);
	sdio_set_data_length(64);
	sdio_set_data_block_size(BLOCK_LENGTH_64_B);
	sdio_set_data_transfer_direction(CONTROLLER_TO_CARD);
	sdio_set_data_transfer_mode(BLOCK_DATA_TRANSFER);
	sdio_data_transfer_enable();

	sdio_send_command(SD_CMD_SD_APP_STAUS, (uint32_t)(0), RESPONSE_SHORT, WAIT_NO, true);
  	result = sdcard_cmd_R1_error(SD_CMD_SD_APP_STAUS);

  	if (result != SD_OK)
    	return(result);

    while (!sdio_get_flag_status(SDIO_RXOVERR) && 
  		   !sdio_get_flag_status(SDIO_DCRCFAIL) &&
  		   !sdio_get_flag_status(SDIO_DTIMEOUT) &&
  		   !sdio_get_flag_status(SDIO_DBCKEND) &&  
  		   !sdio_get_flag_status(SDIO_STBITERR))
  	{
    	if (sdio_get_flag_status(SDIO_RXFIFOHF))
    	{
      		for (count = 0; count < 8; count++)
      		{
        		*(psdstatus + count) = sdio_read_fifo_data();
      		}
      		psdstatus += 8;
    	}
  	}

  	if (sdio_get_flag_status(SDIO_DTIMEOUT))
  	{
    	sdio_clear_flag_status(SDIO_DTIMEOUT);
    	result = SD_DATA_TIMEOUT;
    	return(result);
  	}

  	else if (sdio_get_flag_status(SDIO_DCRCFAIL))
  	{
  		sdio_clear_flag_status(SDIO_DCRCFAIL);
    	result = SD_DATA_CRC_FAIL;
    	return(result);
  	}

  	else if (sdio_get_flag_status(SDIO_RXOVERR))
  	{
    	sdio_clear_flag_status(SDIO_RXOVERR);
    	result = SD_RX_OVERRUN;
    	return(result);
  	}

  	else if (sdio_get_flag_status(SDIO_STBITERR))
  	{
    	sdio_clear_flag_status(SDIO_STBITERR);
    	result = SD_START_BIT_ERR;
    	return(result);
  	}

  	count = SD_DATATIMEOUT;
  	while (sdio_get_flag_status(SDIO_RXDAVL) && (count > 0))
  	{
  		*psdstatus = sdio_read_fifo_data();
    	psdstatus++;
    	count--;
  	}

  	sdio_clear_all_flag_status(SDIO_STATIC_FLAGS);
  	return(result);
}

SDCARD_Result sdcard_erase(uint64_t startaddr, uint64_t endaddr)
{
	SDCARD_Result result = SD_OK;
  	uint32_t delay = 0;
  	uint32_t maxdelay = 0;
  	uint8_t cardstate = 0;

  	if (((CSD_Tab[1] >> 20) & SD_CCCC_ERASE) == 0)
  	{
    	result = SD_REQUEST_NOT_APPLICABLE;
    	return(result);
  	}

  	maxdelay = 120000 / (sdio_get_clock_divider() + 2);

  	if (sdio_get_card_status_1() & SD_CARD_LOCKED)
  	{
    	result = SD_LOCK_UNLOCK_FAILED;
    	return(result);
  	}

  	if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
  	{
    	startaddr /= 512;
    	endaddr /= 512;
  	}
  
  	if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType))
  	{
  		sdio_send_command(SD_CMD_SD_ERASE_GRP_START, (uint32_t)(startaddr), RESPONSE_SHORT, WAIT_NO, true);
  		result = sdcard_cmd_R1_error(SD_CMD_SD_ERASE_GRP_START);

   		if (result != SD_OK)
      		return(result);

      	sdio_send_command(SD_CMD_SD_ERASE_GRP_END, (uint32_t)(endaddr), RESPONSE_SHORT, WAIT_NO, true);
      	result = sdcard_cmd_R1_error(SD_CMD_SD_ERASE_GRP_END);

   		if (result != SD_OK)
      		return(result);
  	}

  	sdio_send_command(SD_CMD_ERASE, (uint32_t)(0), RESPONSE_SHORT, WAIT_NO, true);
  	result = sdcard_cmd_R1_error(SD_CMD_ERASE);

  	if (result != SD_OK)
    	return(result);

  	for (delay = 0; delay < maxdelay; delay++) { }

  	result = sdcard_is_busy(&cardstate);
  	delay = SD_DATATIMEOUT;

  	while ((delay > 0) && (result == SD_OK) && ((SD_CARD_PROGRAMMING == cardstate) || (SD_CARD_RECEIVING == cardstate)))
  	{
    	result = sdcard_is_busy(&cardstate);
    	delay--;
  	}

  	return(result);
}

static SDCARD_Result sdcard_is_busy(uint8_t *pstatus)
{
	SDCARD_Result result = SD_OK;
	uint32_t respR1 = 0;

	sdio_send_command(SD_CMD_SEND_STATUS, (uint32_t)(RCA << 16), RESPONSE_SHORT, WAIT_NO, true);

	while (!sdio_get_flag_status(SDIO_CCRCFAIL) && 
  		   !sdio_get_flag_status(SDIO_CMDREND) &&
  		   !sdio_get_flag_status(SDIO_CTIMEOUT));

  	if (sdio_get_flag_status(SDIO_CTIMEOUT))
  	{
    	result = SD_CMD_RSP_TIMEOUT;
    	sdio_clear_flag_status(SDIO_CTIMEOUT);
    	return(result);
  	}

  	else if (sdio_get_flag_status(SDIO_CCRCFAIL))
  	{
    	result = SD_CMD_CRC_FAIL;
    	sdio_clear_flag_status(SDIO_CCRCFAIL);
    	return(result);
  	}

  	if ((uint32_t)sdio_get_cmd_response() != SD_CMD_SEND_STATUS)
  	{
    	result = SD_ILLEGAL_CMD;
    	return(result);
  	}

  	sdio_clear_all_flag_status(SDIO_STATIC_FLAGS);

  	respR1 = sdio_get_card_status_1();
  	*pstatus = (uint8_t) ((respR1 >> 9) & 0x0000000F);

  	if ((respR1 & SD_OCR_ERRORBITS) == SD_ALLZERO)
  	{
    	return(result);
  	}

  	if (respR1 & SD_OCR_ADDR_OUT_OF_RANGE)
  	{
    	return(SD_ADDR_OUT_OF_RANGE);
  	}

  	if (respR1 & SD_OCR_ADDR_MISALIGNED)
  	{
    	return(SD_ADDR_MISALIGNED);
  	}

  	if (respR1 & SD_OCR_BLOCK_LEN_ERR)
  	{
    	return(SD_BLOCK_LEN_ERR);
  	}

  	if (respR1 & SD_OCR_ERASE_SEQ_ERR)
  	{
    	return(SD_ERASE_SEQ_ERR);
  	}

  	if (respR1 & SD_OCR_BAD_ERASE_PARAM)
  	{
    	return(SD_BAD_ERASE_PARAM);
  	}

  	if (respR1 & SD_OCR_WRITE_PROT_VIOLATION)
  	{
    	return(SD_WRITE_PROT_VIOLATION);
  	}

  	if (respR1 & SD_OCR_LOCK_UNLOCK_FAILED)
  	{
    	return(SD_LOCK_UNLOCK_FAILED);
  	}

  	if (respR1 & SD_OCR_COM_CRC_FAILED)
  	{
    	return(SD_COM_CRC_FAILED);
  	}

  	if (respR1 & SD_OCR_ILLEGAL_CMD)
  	{
    	return(SD_ILLEGAL_CMD);
  	}

  	if (respR1 & SD_OCR_CARD_ECC_FAILED)
  	{
    	return(SD_CARD_ECC_FAILED);
  	}

  	if (respR1 & SD_OCR_CC_ERROR)
  	{
    	return(SD_CC_ERROR);
  	}

  	if (respR1 & SD_OCR_GENERAL_UNKNOWN_ERROR)
  	{
    	return(SD_GENERAL_UNKNOWN_ERROR);
  	}

  	if (respR1 & SD_OCR_STREAM_READ_UNDERRUN)
  	{
    	return(SD_STREAM_READ_UNDERRUN);
  	}

  	if (respR1 & SD_OCR_STREAM_WRITE_OVERRUN)
  	{
    	return(SD_STREAM_WRITE_OVERRUN);
  	}

  	if (respR1 & SD_OCR_CID_CSD_OVERWRIETE)
  	{
    	return(SD_CID_CSD_OVERWRITE);
  	}

  	if (respR1 & SD_OCR_WP_ERASE_SKIP)
  	{
    	return(SD_WP_ERASE_SKIP);
  	}

  	if (respR1 & SD_OCR_CARD_ECC_DISABLED)
  	{
    	return(SD_CARD_ECC_DISABLED);
  	}

  	if (respR1 & SD_OCR_ERASE_RESET)
  	{
    	return(SD_ERASE_RESET);
  	}

  	if (respR1 & SD_OCR_AKE_SEQ_ERROR)
  	{
    	return(SD_AKE_SEQ_ERROR);
  	}

  	return(result);
}

SDTransferState sdcard_get_status(void)
{
	SDCardState cardstate =  SD_CARD_TRANSFER;
  	cardstate = sdcard_get_state();
  
  	if (cardstate == SD_CARD_TRANSFER)
  	{
    	return(SD_TRANSFER_OK);
  	}
  	else if(cardstate == SD_CARD_ERROR)
  	{
    	return (SD_TRANSFER_ERROR);
  	}
  	else
  	{
    	return(SD_TRANSFER_BUSY);
    }
}

SDCardState sdcard_get_state(void)
{
  uint32_t resp1 = 0;
  
  if(sdcard_detect()== SD_PRESENT)
  {
    if (sdcard_send_status(&resp1) != SD_OK)
    {
      return SD_CARD_ERROR;
    }
    else
    {
      return (SDCardState)((resp1 >> 9) & 0x0F);
    }
  }
  else
  {
    return SD_CARD_ERROR;
  }
}

