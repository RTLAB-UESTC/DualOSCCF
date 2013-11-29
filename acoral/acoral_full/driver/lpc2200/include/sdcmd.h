#ifndef __SDCMD_H__
#define __SDCMD_H__

/*
 *******************************************************
	
		SD/MMC 卡的相关命令与响应宏定义
	
 *******************************************************
*/

/* 命令响应定义 define command's response */
#define R1 1
#define R1B 2
#define R2 3
#define R3 4

/* R1和R2高字节错误码 R1 and upper byte of R2 error code */
#define MSK_IDLE          		  0x01
#define MSK_ERASE_RST     		  0x02
#define MSK_ILL_CMD       		  0x04
#define MSK_CRC_ERR       		  0x08
#define MSK_ERASE_SEQ_ERR 		  0x10
#define MSK_ADDR_ERR      		  0x20
#define MSK_PARAM_ERR     		  0x40

/* R2低字节错误码 lower byte of R2 error code */
#define MSK_TOK_ERROR             0x01
#define MSK_TOK_CC_ERROR          0x02
#define MSK_TOK_ECC_FAILED        0x04
#define MSK_TOK_CC_OUTOFRANGE     0x08
#define MSK_TOK_CC_LOCKED         0x10



/* 数据令牌 Data Tokens */
#define SD_TOK_READ_STARTBLOCK    0xFE
#define SD_TOK_WRITE_STARTBLOCK   0xFE
#define SD_TOK_READ_STARTBLOCK_M  0xFE
#define SD_TOK_WRITE_STARTBLOCK_M 0xFC
#define SD_TOK_STOP_MULTI         0xFD

/* 数据响应令牌 Data Response Tokens */
#define SD_RESP_DATA_MSK		  0x0F		//数据响应掩码
#define SD_RESP_DATA_ACCETPTED	  0x05		//数据被接受
#define SD_RESP_DATA_REJECT_CRC	  0x0B      //由于CRC错误而被拒绝
#define SD_RESP_DATA_REJECT_WRITE 0x0D		//由于写错误而被拒绝

/* 等待类型 Wait Type */
#define SD_WAIT_READ			  0x00		//读等待
#define SD_WAIT_WRITE			  0x01		//写等待
#define SD_WAIT_ERASE		 	  0x02		//擦除等待


/*
*********************************************

     SD卡SPI模式下命令集

*********************************************
*/

/******************************** 基本命令集 Basic command set **************************/
/* 复位SD 卡 Reset cards to idle state */
#define CMD0 0
#define CMD0_R R1

/* 读OCR寄存器 Read the OCR (MMC mode, do not use for SD cards) */
#define CMD1 1
#define CMD1_R R1

/* 读CSD寄存器 Card sends the CSD */
#define CMD9 9
#define CMD9_R R1

/* 读CID寄存器 Card sends CID */
#define CMD10 10
#define CMD10_R R1

/* 停止读多块时的数据传输 Stop a multiple block (stream) read/write operation */
#define CMD12 12
#define CMD12_R R1B

/* 读 Card_Status 寄存器 Get the addressed card's status register */
#define CMD13 13
#define CMD13_R R2

/***************************** 块读命令集 Block read commands **************************/

/* 设置块的长度 Set the block length */
#define CMD16 16
#define CMD16_R R1

/* 读单块 Read a single block */
#define CMD17 17
#define CMD17_R R1

/* 读多块,直至主机发送CMD12为止 Read multiple blocks until a CMD12 */
#define CMD18 18
#define CMD18_R R1

/***************************** 块写命令集 Block write commands *************************/
/* 写单块 Write a block of the size selected with CMD16 */
#define CMD24 24
#define CMD24_R R1

/* 写多块 Multiple block write until a CMD12 */
#define CMD25 25
#define CMD25_R R1

/* 写CSD寄存器 Program the programmable bits of the CSD */
#define CMD27 27
#define CMD27_R R1

/***************************** 写保护 Write protection *****************************/
/* Set the write protection bit of the addressed group */
#define CMD28 28
#define CMD28_R R1B

/* Clear the write protection bit of the addressed group */
#define CMD29 29
#define CMD29_R R1B

/* Ask the card for the status of the write protection bits */
#define CMD30 30
#define CMD30_R R1

/***************************** 擦除命令 Erase commands *******************************/
/* 设置擦除块的起始地址(只用于SD卡) Set the address of the first write block to be erased(only for SD) */
#define CMD32 32
#define CMD32_R R1

/* 设置擦除块的终止地址(只用于SD卡) Set the address of the last write block to be erased(only for SD) */
#define CMD33 33
#define CMD33_R R1

/* 设置擦除块的起始地址(只用于MMC卡) Set the address of the first write block to be erased(only for MMC) */
#define CMD35 35
#define CMD35_R R1

/* 设置擦除块的终止地址(只用于MMC卡) Set the address of the last write block to be erased(only for MMC) */
#define CMD36 36
#define CMD36_R R1

/* 擦除所选择的块 Erase the selected write blocks */
#define CMD38 38
#define CMD38_R R1B

/***************************** 锁卡命令 Lock Card commands ***************************/
/* 设置/复位密码或上锁/解锁卡 Set/reset the password or lock/unlock the card */
#define CMD42 42
#define CMD42_R	R1B
/* Commands from 42 to 54, not defined here */

/***************************** 应用命令 Application-specific commands ****************/
/* 禁止下一个命令为应用命令  Flag that the next command is application-specific */
#define CMD55 55
#define CMD55_R R1

/* 应用命令的通用I/O  General purpose I/O for application-specific commands */
#define CMD56 56
#define CMD56_R R1

/* 读OCR寄存器  Read the OCR (SPI mode only) */
#define CMD58 58
#define CMD58_R R3

/* 使能或禁止 CRC Turn CRC on or off */
#define CMD59 59
#define CMD59_R R1

/***************************** 应用命令 Application-specific commands ***************/
/* 获取 SD Status寄存器 Get the SD card's status */
#define ACMD13 13
#define ACMD13_R R2

/* 得到已写入卡中的块的个数 Get the number of written write blocks (Minus errors ) */
#define ACMD22 22
#define ACMD22_R R1

/* 在写之前,设置预先擦除的块的个数 Set the number of write blocks to be pre-erased before writing */
#define ACMD23 23
#define ACMD23_R R1

/* 读取OCR寄存器 Get the card's OCR (SD mode) */
#define ACMD41 41
#define ACMD41_R R1

/* 连接/断开CD/DATA[3]引脚上的上拉电阻 Connect or disconnect the 50kOhm internal pull-up on CD/DAT[3] */
#define ACMD42 42
#define ACMD42_R R1

/* 读取SCR寄存器 Get the SD configuration register */
#define ACMD51 51
#define ACMD51_R R1



extern acoral_u8 SD_SendCmd(acoral_u8 cmd, acoral_u8 *param, acoral_u8 resptype, acoral_u8 *resp);  // 一个SPI命令   a SPI command
extern void SD_PackParam(acoral_u8 *parameter, acoral_u32 value);						// 封装参数	  	 pack the parameter
extern acoral_u8 SD_BlockCommand(acoral_u8 cmd, acoral_u8 resptype, acoral_u32 parameter);		// 块命令 		 block command 

	
extern acoral_u8 SD_ResetSD(void);											// 复位SD卡			reset SD Card

extern acoral_u8 SD_ReadCSD(acoral_u8 csdlen, acoral_u8 *recbuf);					// 读CSD 			read CSD register
extern acoral_u8 SD_ReadCID(acoral_u8 cidlen, acoral_u8 *recbuf);					// 读CID			read CID register
extern acoral_u8 SD_StopTransmission(void);									// 停止传输			stop transmission

extern acoral_u8 SD_ReadCard_Status(acoral_u8 len, acoral_u8 *buffer);				// 读Card Status	read Card Status register
extern acoral_u8 SD_SetBlockLen(acoral_u32 length);								// 设置块长度		set block length

extern acoral_u8 SD_ReadSingleBlock(acoral_u32 blockaddr);						// 读单块			read a single block
extern acoral_u8 SD_ReadMultipleBlock(acoral_u32 blockaddr);					// 读多块			read multiple block

extern acoral_u8 SD_WriteSingleBlock(acoral_u32 blockaddr);						// 写单块			write a single block
extern acoral_u8 SD_WriteMultipleBlock(acoral_u32 blockaddr);					// 写多块 			write multiple block


extern acoral_u8 SD_ProgramCSD(acoral_u8 len, acoral_u8 *buff);						// 写CSD寄存器  	write CSD register 

extern acoral_u8 SD_EraseStartBlock(acoral_u32 startblock);						// 擦块起始地址		erase start address
extern acoral_u8 SD_EraseEndBlock(acoral_u32 endblock);							// 擦块终止地址		erase end address
extern acoral_u8 SD_EraseSelectedBlock(void);								// 擦除所选的块		erase selected address


extern acoral_u8 SD_ReadOCR(acoral_u8 ocrlen,acoral_u8 *recbuf);					// 读OCR			read OCR register
extern acoral_u8 SD_EnableCRC(acoral_u8 bEnable);								// 使能CRC校验		enable CRC


// 特殊应用命令
extern acoral_u8 SD_ReadSD_Status(acoral_u8 sdslen, acoral_u8 *recbuf);				// 读SD_Status		read SD_Status 
extern acoral_u8 SD_GetNumWRBlcoks(acoral_u32 *blocknum);						// 得到正确写入块数 get block numbers that writed well
extern acoral_u8 SD_ReadSCR(acoral_u8 scrlen, acoral_u8 *recbuf);					// 读SCR 			read SCR register


// 数据流函数
extern acoral_u8 SD_ReadRegister(acoral_u32 len, acoral_u8 *recbuf);						// 读寄存器  read register 
extern acoral_u8 SD_ReadBlockData(acoral_u32 len, acoral_u8 *recbuf);						// 读块数据	 read block data 
extern acoral_u8 SD_WriteBlockData(acoral_u8 bmulti, acoral_u32 len, acoral_u8 *sendbuf);		// 写块数据	 write block data

// 其它函数
extern void SD_StopMultiToken(void);									// 停止多块写令牌	 stop token when write multiple block
extern acoral_u8 SD_WaitBusy(acoral_u8 waittype);								// 忙				 busy
extern void SD_SPIDelay(acoral_u8 value);									// 发生SPI时钟		 send SPI clock


#endif













