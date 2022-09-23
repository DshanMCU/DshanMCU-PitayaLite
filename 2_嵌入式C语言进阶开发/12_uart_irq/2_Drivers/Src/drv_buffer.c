/**
 * @file drv_buffer.c
 * @author 百问网开发团队
 * @brief 
 * @version 0.1
 * @date 2022-05-31
 * 
 * @copyright Copyright (c) 2022 深圳百问网科技有限公司
 * 
 */

#include "drv_buffer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
    函数名：Driver_Buffer_Init
    功能：初始化一个指定的环形缓冲区
    输入参数：buffer->指向目标缓冲区；size->表示缓冲区分配的内存大小，单位是字节
    输出参数：无
    返回值：-1->表示错误；0->表示成功
*/
int Drv_Buffer_Init(ptRingBuffer buffer, unsigned short size)
{
    if(buffer == NULL || size == 0) return -1;
    
    if(buffer->fifo == NULL)
    {
        buffer->fifo = (unsigned char*)malloc(size);
        if(buffer->fifo == NULL)    
        {
            printf("Malloc %d bytes failed.\r\n", size);
            return -1;
        }
    }
    buffer->pw = buffer->pr = 0;
    buffer->buf_size = size;
    
    return 0;
}    

/*
    函数名：Driver_Buffer_Write
    功能：往指定的环形缓冲区写入一个字节的数据
    输入参数：buffer->指向目标缓冲区；data->写入的数据
    输出参数：无
    返回值：-1->表示错误；0->表示成功
*/
int Drv_Buffer_Write(ptRingBuffer buffer, const unsigned char data)
{
    if(buffer == NULL || buffer->fifo==NULL)    return -1;
    int i = (buffer->pw + 1) % buffer->buf_size;
    if(i != buffer->pr)
    {
        buffer->fifo[buffer->pw] = data;
        buffer->pw = i;
        
        return 0;
    }
    
    return -1;
}

/*
    函数名：Driver_Buffer_WriteBytes
    功能：往指定的环形缓冲区写入多个字节的数据
    输入参数：buffer->指向目标缓冲区；data_stream->写入的数据流；len->写入的数据个数，单位是字节
    输出参数：无
    返回值：-1->表示错误；0->表示成功
*/
int Drv_Buffer_WriteBytes(ptRingBuffer buffer, const unsigned char *data_stream, unsigned char len)
{
    int i = 0;
    if(buffer == NULL || buffer->fifo==NULL)    return -1;
    if(data_stream == NULL)     return -1;
    if(len == 0)    return -1;
    for(i=0; i<len; i++)
    {
        if(Drv_Buffer_Write(buffer, data_stream[i]) != 0)    break;
    }
    
    return i;
}

/*
    函数名：Driver_Buffer_Read
    功能：从指定的环形缓冲区读出一个字节的数据
    输入参数：buffer->指向目标缓冲区；
    输出参数：data->读出的数据
    返回值：-1->表示错误；0->表示成功
*/
int Drv_Buffer_Read(ptRingBuffer buffer, unsigned char *data)
{
    if(buffer == NULL || buffer->fifo==NULL)    return -1;
    if(data == NULL)     return -1;
    if(buffer->pr == buffer->pw)    return -1;
    
    *data = buffer->fifo[buffer->pr];
    buffer->pr = (buffer->pr + 1) % buffer->buf_size;
    
    return 0;
}

/*
    函数名：Driver_Buffer_ReadBytes
    功能：往指定的环形缓冲区读出多个字节的数据
    输入参数：buffer->指向目标缓冲区；len->读出的数据个数，单位是字节
    输出参数：data_stream->读出的数据流
    返回值：-1->表示错误；0->表示成功
*/
int Drv_Buffer_ReadBytes(ptRingBuffer buffer, unsigned char *data_stream, unsigned char len)
{
    int i = 0;
    if(buffer == NULL || buffer->fifo==NULL)    return -1;
    if(data_stream == NULL)     return -1;
    if(len == 0)    return -1;
    for(i=0; i<len; i++)
    {
        if(Drv_Buffer_Read(buffer, &data_stream[i]) != 0)    break;
    }
    
    return i;
}

/*
    函数名：Driver_Buffer_Clean
    功能：清除指定的环形缓冲区
    输入参数：buffer->指向目标缓冲区
    输出参数：无
    返回值：-1->表示错误；0->表示成功
*/
int Drv_Buffer_Clean(ptRingBuffer buffer)
{
    if(buffer == NULL || buffer->fifo==NULL)    return -1;
    memset(buffer->fifo, 0, buffer->buf_size);
    buffer->pw = buffer->pr = 0;
    
    return 0;
}
