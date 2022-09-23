/**
 * @file drv_sdio.c
 * @author 百问网开发团队
 * @brief 
 * @version 0.1
 * @date 2022-05-31
 * 
 * @copyright Copyright (c) 2022 深圳百问网科技有限公司
 * 
 */
#include "drv_sdio.h"
#include "drv_sysclk.h"
#include "hal_sdio.h"
#include "hal_rcc.h"
#include "hal_gpio.h"

#include <stdio.h>

#define SDCARD_RESP_BUF_LEN 4u
uint32_t sdcard_resp_buf[SDCARD_RESP_BUF_LEN];

/**
 * @brief 初始化SDIO的GPIO
 * 
 */
static void Drv_SDIO_GPIO_Init(void)
{
    GPIO_Init_Type gpio_init;
    SDIO_GPIO_CLK_EN();

    /* PC12 - SDIO_CLK. */
    gpio_init.Pins  = GPIO_PIN_12;
    gpio_init.PinMode  = GPIO_PinMode_AF_PushPull;
    gpio_init.Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &gpio_init);
    GPIO_PinAFConf(GPIOC, GPIO_PIN_12, GPIO_AF_12);

    /* PD2  - SDIO_CMD. */
    gpio_init.Pins  = GPIO_PIN_2;
    gpio_init.PinMode  = GPIO_PinMode_AF_PushPull;
    gpio_init.Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &gpio_init);
    GPIO_PinAFConf(GPIOD, GPIO_PIN_2, GPIO_AF_12);

    /* PC8  - SDIO_DAT0. */
    gpio_init.Pins  = GPIO_PIN_8;
    gpio_init.PinMode  = GPIO_PinMode_AF_PushPull;
    gpio_init.Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &gpio_init);
    GPIO_PinAFConf(GPIOC, GPIO_PIN_8, GPIO_AF_12);

    /* PC9  - SDIO_DAT1. */
    gpio_init.Pins  = GPIO_PIN_9;
    gpio_init.PinMode  = GPIO_PinMode_AF_PushPull;
    gpio_init.Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &gpio_init);
    GPIO_PinAFConf(GPIOC, GPIO_PIN_9, GPIO_AF_12);

    /* PC10 - SDIO_DAT2. */
    gpio_init.Pins  = GPIO_PIN_10;
    gpio_init.PinMode  = GPIO_PinMode_AF_PushPull;
    gpio_init.Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &gpio_init);
    GPIO_PinAFConf(GPIOC, GPIO_PIN_10, GPIO_AF_12);

    /* PC11 - SDIO_DAT3. */
    gpio_init.Pins  = GPIO_PIN_11;
    gpio_init.PinMode  = GPIO_PinMode_AF_PushPull;
    gpio_init.Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &gpio_init);
    GPIO_PinAFConf(GPIOC, GPIO_PIN_11, GPIO_AF_12);
}

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
 * @return 0 
 * @return -1 
 */
int Drv_SDIO_Init(sdcard_t * card, void * iodev)
{
    card->iodev = iodev;

    Drv_SDIO_GPIO_Init();
    
    RCC_EnableAHB1Periphs(RCC_AHB1_PERIPH_SDIO, true);
    SDIO_Init_Type sdio_init;

    sdio_init.BusClkHz = Drv_GetSysClockFreq();
    sdio_init.BaseClkSrc = SDIO_BaseClkSrc_1MHz;
    sdio_init.ClkLineSpeedDiv = SDIO_ClkLineSpeedDiv_Div2; /* set the card freq to 400khz. */

    SDIO_Init(card->iodev, &sdio_init);
    SDIO_Enable(card->iodev, true);

    /* cmd0. */
    for (uint32_t i = 0u; i < 74; i++)
    {
        SDIO_ExecuteCmd(card->iodev, 0, 0, 0);
    }

    /* cmd8. */
    while (1)
    {
        SDIO_ExecuteCmd(card->iodev, 8, 0x1AA, 0);
        SDIO_RequestResp(card->iodev, SDIO_RespType_R7, sdcard_resp_buf);
        printf("cmd8: ");
        for (uint32_t i = 0u; i < SDCARD_RESP_BUF_LEN; i++)
        {
            printf("%2X, ", (unsigned)sdcard_resp_buf[i]);
        }
        printf("\r\n");

        if (sdcard_resp_buf[0] == 0x1AA)
        {
            break; /* init done. */
        }
    }

    /* cmd55 + acmd41. */
    while (1)
    {
        /* cmd55. */
        SDIO_ExecuteCmd(card->iodev, 55, 0, 0);
        SDIO_RequestResp(card->iodev, SDIO_RespType_R1, sdcard_resp_buf);
        printf("cmd55: ");
        for (uint32_t i = 0u; i < SDCARD_RESP_BUF_LEN; i++)
        {
            printf("%2X, ", (unsigned)sdcard_resp_buf[i]);
        }
        printf("\r\n");


        /* acmd41. */
        SDIO_ExecuteCmd(card->iodev, 41, 0xC0100000, 0);
        SDIO_RequestResp(card->iodev, SDIO_RespType_R3, sdcard_resp_buf);
        printf("acmd41: ");
        for (uint32_t i = 0u; i < SDCARD_RESP_BUF_LEN; i++)
        {
            printf("%2X, ", (unsigned)sdcard_resp_buf[i]);
        }
        printf("\r\n");

        if ( 0u != ( sdcard_resp_buf[0] & (1u << 30)) )
        {
            card->cardtype = sdcard_cardtype_sdhc;
            printf("sdhc.\r\n");
        }
        else
        {
            card->cardtype = sdcard_cardtype_sdsc;
        }

        if ( 0u != ( sdcard_resp_buf[0] & (1u << 31)) )
        {
            printf("initialization done.\r\n");
            break;
        }
    }

    /* cmd2. ALL_SEND_CID. Card Identification Number, card individual number for identification.*/
    SDIO_ExecuteCmd(card->iodev, 2, 0, 0);
    SDIO_RequestResp(card->iodev, SDIO_RespType_R2, sdcard_resp_buf);
    printf("cmd2: ");
    for (uint32_t i = 0u; i < SDCARD_RESP_BUF_LEN; i++)
    {
        printf("%2X, ", (unsigned)sdcard_resp_buf[i]);
    }
    printf("\r\n");

    /* cmd3. SEND_RELATIVE_ADDR. Ask the card to publish a new relative address (RCA). */
    SDIO_ExecuteCmd(card->iodev, 3, 0, 0);
    SDIO_RequestResp(card->iodev, SDIO_RespType_R6, sdcard_resp_buf);
    printf("cmd3: ");
    for (uint32_t i = 0u; i < SDCARD_RESP_BUF_LEN; i++)
    {
        printf("%2X, ", (unsigned)sdcard_resp_buf[i]);
    }
    printf("\r\n");

    card->rca = sdcard_resp_buf[0] >> 16u;

    printf("card->rca = 0x%X\r\n", (unsigned)card->rca);

    /* cmd9 + RCA -> CSD. SEND_CSD. Addressed card sends its card-specific data (CSD) on the cmd line. */
    SDIO_ExecuteCmd(card->iodev, 9, card->rca << 16u, 0);
    SDIO_RequestResp(card->iodev, SDIO_RespType_R2, sdcard_resp_buf);

    printf("cmd9: ");
    for (uint32_t i = 0u; i < SDCARD_RESP_BUF_LEN; i++)
    {
        printf("%2X, ", (unsigned)sdcard_resp_buf[i]);
    }
    printf("\r\n");

    if (((sdcard_resp_buf[3] >> 22) & 0x3) == 0x1) /* CSD in version 2.0 */
    {
        uint32_t c_size = (sdcard_resp_buf[1] >> 8) & 0x3fffff; /* memory capacity = (C_SIZE+1) * 512K byte. */
        card->block_cnt = c_size << 10u; /* when the block size is 512. */
        card->block_len = (1u << 9u);
    }
    else if (((sdcard_resp_buf[3] >> 22) & 0x3) == 0x0) /* CSD in version 1.0 */
    {
        uint32_t c_size = (sdcard_resp_buf[1] >> 22) | ((sdcard_resp_buf[2] & 0x3) << 10);
        uint32_t c_size_mult = ((sdcard_resp_buf[1] >> 7) & 0x7);
        uint32_t block_len = (sdcard_resp_buf[2] >> 6) & 0xf;

        card->block_cnt = (c_size + 1) << (c_size_mult + 2);
        card->block_len = block_len;
    }
    printf("card->block_cnt = %u\r\n", (unsigned)card->block_cnt);

    /* cmd13 Send card status. check if the card is switched to stand-by status.  */
    while (1)
    {
        SDIO_ExecuteCmd(card->iodev, 13, card->rca << 16u, 0);
        SDIO_RequestResp(card->iodev, SDIO_RespType_R1, sdcard_resp_buf);
        printf("cmd13: ");
        for (uint32_t i = 0u; i < SDCARD_RESP_BUF_LEN; i++)
        {
            printf("%2X, ", (unsigned)sdcard_resp_buf[i]);
        }
        printf("\r\n");

        uint32_t current_status = (sdcard_resp_buf[0] >> 9)  & 0xf;
        uint32_t error_status   = (sdcard_resp_buf[0] >> 19) & 0x1;

        if ( (current_status == 3) || (error_status == 1) )
        {
            if (current_status == 3) /* stand-by status. */
            {
                printf("standby.\r\n");
            }
            if (error_status == 1)
            {
                printf("error.\r\n");
            }
            break;
        }
    }

    /* cmd4. setup the card freq. */


    /* cmd7. assert or deassert the card with RCA. */
    SDIO_ExecuteCmd(card->iodev, 7, card->rca << 16u, 0);
    SDIO_RequestResp(card->iodev, SDIO_RespType_R1, sdcard_resp_buf);

    /* cmd13. send card status. check if the card is switched to tran mode. */
    while (1)
    {
        SDIO_ExecuteCmd(card->iodev, 13, card->rca << 16u, 0);
        SDIO_RequestResp(card->iodev, SDIO_RespType_R1, sdcard_resp_buf);
        printf("cmd13: ");
        for (uint32_t i = 0u; i < SDCARD_RESP_BUF_LEN; i++)
        {
            printf("%2X, ", (unsigned)sdcard_resp_buf[i]);
        }
        printf("\r\n");

        uint32_t current_status = (sdcard_resp_buf[0] >> 9)  & 0xf;
        uint32_t error_status   = (sdcard_resp_buf[0] >> 19) & 0x1;

        if ( (current_status == 4) || (error_status == 1) )
        {
            if (current_status == 4) /* tran mode. */
            {
                printf("tran.\r\n");
            }
            if (error_status == 1)
            {
                printf("error.\r\n");
            }
            break;
        }
    }

    /* cmd55 + acmd6. setup card data bus width. */
    SDIO_ExecuteCmd(card->iodev, 55, card->rca << 16, 0);
    SDIO_RequestResp(card->iodev, SDIO_RespType_R1, sdcard_resp_buf);
    printf("cmd55\r\n");

    SDIO_ExecuteCmd(card->iodev, 6, 0x2, 0); /* 4-bit. */
    SDIO_RequestResp(card->iodev, SDIO_RespType_R1, sdcard_resp_buf);
    printf("acmd6\r\n");

    SDIO_SetDataBusWidth(card->iodev, SDIO_DataBusWidth_4b);

    return 0;
}

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
 * @return 0 
 * @return -1 
 */
int Drv_SDIO_Write_SingleBlock(sdcard_t * card, uint32_t blk_idx, uint8_t * out_buf)
{
    /* cmd16. */
    SDIO_ExecuteCmd(card->iodev, 16, SDCARD_BLOCK_SIZE, 0);
    SDIO_RequestResp(card->iodev, SDIO_RespType_R1, sdcard_resp_buf);

    /* cmd24. write block. */
    SDIO_ExecuteCmd(card->iodev, 24, blk_idx, 0);
    SDIO_RequestResp(card->iodev, SDIO_RespType_R1, sdcard_resp_buf);

    /* cmd13. send card status. check if the card is switch to recv mode. */
    while (1)
    {
        SDIO_ExecuteCmd(card->iodev, 13, card->rca << 16u, 0);
        SDIO_RequestResp(card->iodev, SDIO_RespType_R1, sdcard_resp_buf);

        uint32_t current_status = (sdcard_resp_buf[0] >> 9)  & 0xf;
        uint32_t error_status   = (sdcard_resp_buf[0] >> 19) & 0x1;

        if ( (current_status == 6) || (error_status == 1) )
        {
            if (current_status == 6) /* recv mode. */
            {
                printf("recv.\r\n");
            }
            if (error_status == 1)
            {
                printf("error.\r\n");
            }
            break;
        }
    }

    /* write one block data. 512 Byte. */
    SDIO_SwitchFifoWrite(card->iodev, true);
    SDIO_ClearFifoData(card->iodev);

    for (uint32_t i = 0u; i < 128u; i++)
    {
        SDIO_PutFifoData(card->iodev, *((uint32_t *)out_buf));
        out_buf += 4u;
    }

    SDIO_ExecuteData(card->iodev, SDIO_CMD_FLAG_WRITE_BLOCK);

    /* pending for the xfer done. */
    while ( 0u == (SDIO_STATUS_DAT_DONE & SDIO_GetStatus(card->iodev)) )
    {}
    SDIO_ClearStatus(card->iodev, SDIO_STATUS_DAT_DONE);

    /* cmd12. stop transmission. */
    SDIO_ExecuteCmd(card->iodev, 12, 0, 0);
    SDIO_RequestResp(card->iodev, SDIO_RespType_R1, sdcard_resp_buf);

    /* cmd13. send card status. check if the card is switch from prg mode to . */
    while (1)
    {
        SDIO_ExecuteCmd(card->iodev, 13, card->rca << 16u, 0);
        SDIO_RequestResp(card->iodev, SDIO_RespType_R1, sdcard_resp_buf);

        uint32_t current_status = (sdcard_resp_buf[0] >> 9)  & 0xf;
        uint32_t error_status   = (sdcard_resp_buf[0] >> 19) & 0x1;

        printf("current_status: %u\r\n", (unsigned)current_status);

        if (   ( (current_status != 7) & (current_status != 6) )
            || (error_status == 1) )
        {
            if (error_status == 1)
            {
                printf("error.\r\n");
            }
            else
            {
                printf("prg done.\r\n");
            }
            break;
        }
    }

    return 0;
}

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
 * @return 0 
 * @return -1 
 */
int Drv_SDIO_Write_MultiBlocks(sdcard_t * card, uint32_t blk_idx, uint32_t blk_cnt, uint8_t * out_buf)
{
    /* cmd16. */
    SDIO_ExecuteCmd(card->iodev, 16, SDCARD_BLOCK_SIZE, 0);
    SDIO_RequestResp(card->iodev, SDIO_RespType_R1, sdcard_resp_buf);

    /* cmd25. write multiple block. */
    SDIO_ExecuteCmd(card->iodev, 25, blk_idx, 0);
    SDIO_RequestResp(card->iodev, SDIO_RespType_R1, sdcard_resp_buf);

    /* cmd13. send card status. check if the card is switch to recv mode. */
    while (1)
    {
        SDIO_ExecuteCmd(card->iodev, 13, card->rca << 16u, 0);
        SDIO_RequestResp(card->iodev, SDIO_RespType_R1, sdcard_resp_buf);

        uint32_t current_status = (sdcard_resp_buf[0] >> 9)  & 0xf;
        uint32_t error_status   = (sdcard_resp_buf[0] >> 19) & 0x1;

        if ( (current_status == 6) || (error_status == 1) )
        {
            if (current_status == 6) /* recv mode. */
            {
                printf("recv.\r\n");
            }
            if (error_status == 1)
            {
                printf("error.\r\n");
            }
            break;
        }
    }

    /* setup fifo for write. */
    SDIO_SwitchFifoWrite(card->iodev, true);
    SDIO_ClearFifoData(card->iodev);
    SDIO_SetFifoWatermark(card->iodev, SDCARD_BLOCK_SIZE >> 2u);

    /* setup multiple block count. */
    SDIO_SetMultiBlockCount(card->iodev, blk_cnt);

    while (blk_cnt-- )
    {
        /* prepare the data to be sent. */
        for (uint32_t i = 0u; i < 128u; i++)
        {
            SDIO_PutFifoData(card->iodev, *((uint32_t *)out_buf));
            out_buf += 4u;
        }

        while (1)
        {
            /* cmd13. check if the receive buff is available. */
            SDIO_ExecuteCmd(card->iodev, 13, card->rca << 16u, 0);
            SDIO_RequestResp(card->iodev, SDIO_RespType_R1, sdcard_resp_buf);
            uint32_t ready_for_data = (sdcard_resp_buf[0] >> 8u) & 0x1;
            if (ready_for_data == 1)
            {
                break;
            }
        }

        /* send data. */
        SDIO_ExecuteData(card->iodev, SDIO_CMD_FLAG_WRITE_BLOCK);

        /* waiting for the xfer done. */
        while ( 0u == (SDIO_STATUS_DAT_DONE & SDIO_GetStatus(card->iodev)) )
        {}
        SDIO_ClearStatus(card->iodev, SDIO_STATUS_DAT_DONE);
    }

    /* cmd12. stop transmission. */
    SDIO_ExecuteCmd(card->iodev, 12, 0, 0);
    SDIO_RequestResp(card->iodev, SDIO_RespType_R1, sdcard_resp_buf);

    /* cmd13. send card status. check if the card is switch from prg mode to . */
    while (1)
    {
        SDIO_ExecuteCmd(card->iodev, 13, card->rca << 16u, 0);
        SDIO_RequestResp(card->iodev, SDIO_RespType_R1, sdcard_resp_buf);

        uint32_t current_status = (sdcard_resp_buf[0] >> 9)  & 0xf;
        uint32_t error_status   = (sdcard_resp_buf[0] >> 19) & 0x1;

        printf("current_status: %u\r\n", (unsigned)current_status);

        if (   ( (current_status != 7) & (current_status != 6) )
            || (error_status == 1) )
        {
            if (error_status == 1)
            {
                printf("error.\r\n");
            }
            else
            {
                printf("prg done.\r\n");
            }
            break;
        }
    }

    return 0;
}

/**
 * @brief 从SD卡的某个内存块读出数据
 * setup fifo.
 * cmd17.
 * 
 * @param card 
 * @param blk_idx 
 * @param in_buf 
 * @return 0 
 * @return -1 
 */
int Drv_SDIO_Read_SingleBlock(sdcard_t * card, uint32_t blk_idx, uint8_t * in_buf)
{
    /* setup fifo. */
    SDIO_SwitchFifoWrite(card->iodev, false); /* fifo read. */
    SDIO_ClearFifoData(card->iodev);
    SDIO_SetFifoWatermark(card->iodev, SDCARD_BLOCK_SIZE >> 2u);

    /* cmd17. read single block. */
    SDIO_ExecuteCmd(card->iodev, 17, blk_idx, SDIO_CMD_FLAG_READ_BLOCK | SDIO_CMD_FLAG_ENABLE_DATA_XFER);
    SDIO_RequestResp(card->iodev, SDIO_RespType_R1, sdcard_resp_buf);

    printf("cmd17: ");
    for (uint32_t i = 0u; i < SDCARD_RESP_BUF_LEN; i++)
    {
        printf("%2X, ", (unsigned)sdcard_resp_buf[i]);
    }
    printf("\r\n");

    /* read data from fifo. */
    SDIO_ExecuteData(card->iodev, SDIO_CMD_FLAG_READ_BLOCK);

    // while (0u == (SDIO_STATUS_DAT_BUF_FULL & SDIO_GetStatus(card->iodev)) )
    while (0u == (SDIO_STATUS_DAT_DONE & SDIO_GetStatus(card->iodev)) )
    {}
    SDIO_ClearStatus(card->iodev, SDIO_STATUS_DAT_DONE);

    for (uint32_t i = 0u; i < 128u; i++)
    {
        *((uint32_t *)in_buf) = SDIO_GetFifoData(card->iodev);
        in_buf += 4;
    }

    printf("\r\n\r\n");

    return 0;
}

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
 * @return 0 
 * @return -1 
 */
int Drv_SDIO_Read_MultiBlocks(sdcard_t * card, uint32_t blk_idx, uint32_t blk_cnt, uint8_t * in_buf)
{
    /* setup fifo. */
    SDIO_SwitchFifoWrite(card->iodev, false); /* fifo read. */
    SDIO_ClearFifoData(card->iodev);
    SDIO_SetFifoWatermark(card->iodev, SDCARD_BLOCK_SIZE >> 2u);

    /* setup multiple block count. */
    SDIO_SetMultiBlockCount(card->iodev, blk_cnt);

    /* cmd18. read multi blocks. */
    SDIO_ExecuteCmd(card->iodev, 18, blk_idx, 0u);
    SDIO_RequestResp(card->iodev, SDIO_RespType_R1, sdcard_resp_buf);

    /* start reading data. */
    //SDIO_ExecuteData(card->iodev, SDIO_CMD_FLAG_READ_BLOCKS);
    SDIO_EnableFifoReadWait(card->iodev, false);

    /* wait for the transfer and read data blocks. */
    while (blk_cnt--)
    {
        /* release the read wait, and read more data. */
        //SDIO_EnableFifoReadWait(card->iodev, false);
        //SDIO_EnableFifoReadWait(card->iodev, true); /* wait when the current block is done. */

        /* trigger one block and update blk_count register. */
        SDIO_ExecuteData(card->iodev, SDIO_CMD_FLAG_READ_BLOCK);

        /* wait for a block of data. */
        while (0u == (SDIO_STATUS_DAT_DONE & SDIO_GetStatus(card->iodev)) )
        {}
        SDIO_ClearStatus(card->iodev, SDIO_STATUS_DAT_DONE);

        /* read data from fifo. */
        for (uint32_t i = 0u; i < 128u; i++)
        {
            *((uint32_t *)in_buf) = SDIO_GetFifoData(card->iodev);
            in_buf += 4;
        }
    }

    SDIO_ClearStatus(card->iodev, SDIO_STATUS_DAT_MULTI_BLOCKS_DONE);

    /* cmd12. stop trans from multiple read process. */
    SDIO_ExecuteCmd(card->iodev, 12u, 0u, 0u);
    SDIO_RequestResp(card->iodev, SDIO_RespType_R1, sdcard_resp_buf);

    return 0;
}
