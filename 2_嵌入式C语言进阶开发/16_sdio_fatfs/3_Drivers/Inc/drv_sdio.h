/**
 * @file drv_sdio.h
 * @author 百问网开发团队
 * @brief 
 * @version 0.1
 * @date 2022-05-31
 * 
 * @copyright Copyright (c) 2022 深圳百问网科技有限公司
 * 
 */

#ifndef __DRV_SDIO_H
#define __DRV_SDIO_H

#define SDIO_CLK_PIN                GPIO_PIN_12
#define SDIO_CLK_PORT               GPIOC

#define SDIO_CMD_PIN                GPIO_PIN_2
#define SDIO_CMD_PORT               GPIOD

#define SDIO_DATA_PIN               GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11
#define SDIO_DATA_PORT              GPIOC

#define SDIO_GPIO_CLK_EN()          {RCC_EnableAHB1Periphs(RCC_AHB1_PERIPH_GPIOC, true);RCC_EnableAHB1Periphs(RCC_AHB1_PERIPH_GPIOD, true);}
#define SDIO_CLK_EN()               RCC_EnableAHB1Periphs(RCC_AHB1_PERIPH_SDIO, true)

#define SDCARD_BLOCK_SIZE 512u

typedef enum
{
	sdcard_cardtype_sdsc,
    sdcard_cardtype_sdhc,
    sdcard_cardtype_sdxc,
} sdcard_cardtype_t;

typedef struct
{
	void * iodev;
    sdcard_cardtype_t cardtype;
    unsigned int block_cnt;
    unsigned int block_len;
    unsigned int rca; /* relative card address. */

} sdcard_t;

/**
 * @brief 初始化SDIO
 * cmd0.
 * cmd8.
 * cmd55 + acmd41.
 * cmd2.
 * cmd3.
 * cmd9.
 * cmd13.
 * cmd4.
 * cmd7.
 * cmd13.
 * cmd55 + acmd6
 * cmd16
 * @param card 
 * @param iodev 
 * @return true 
 * @return false 
 */
int Drv_SDIO_Init(sdcard_t * card, void * iodev);


/**
 * @brief 往SD卡某个内存块写数据
 * cmd16.
 * cmd24.
 * cmd13 for recv state.
 * write block data.
 * cmd12.
 * cmd13.

 * @param card 
 * @param blk_idx 
 * @param out_buf 
 * @return true 
 * @return false 
 */
int Drv_SDIO_Write_SingleBlock(sdcard_t * card, unsigned int blk_idx, unsigned char * out_buf);

/**
 * @brief 往SD卡的多个内存块写入数据
 * cmd16.
 * cmd25.
 * cmd13 for recv state.
 * write block data.
 * cmd12.
 * cmd13.
 * 
 * @param card 
 * @param blk_idx 
 * @param blk_cnt 
 * @param out_buf 
 * @return true 
 * @return false 
 */
int Drv_SDIO_Write_MultiBlocks(sdcard_t * card, unsigned int blk_idx, unsigned int blk_cnt, unsigned char * out_buf);

/**
 * @brief 从SD卡的某个内存块读出数据
 * setup fifo.
 * cmd17.
 * 
 * @param card 
 * @param blk_idx 
 * @param in_buf 
 * @return true 
 * @return false 
 */
int Drv_SDIO_Read_SingleBlock(sdcard_t * card, unsigned int blk_idx, unsigned char * in_buf);

/**
 * @brief 从SD卡的多个内存块读出数据
 * cmd18.
 * read blocks.
 * cmd12.
 * 
 * @param card 
 * @param blk_idx 
 * @param blk_cnt 
 * @param in_buf 
 * @return true 
 * @return false 
 */
int Drv_SDIO_Read_MultiBlocks(sdcard_t * card, unsigned int blk_idx, unsigned int blk_cnt, unsigned char * in_buf);

#endif /* __DRV_SDIO_H */
