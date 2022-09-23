/**
 * @file drv_buffer.h
 * @author 百问网开发团队
 * @brief 
 * @version 0.1
 * @date 2022-05-31
 * 
 * @copyright Copyright (c) 2022 深圳百问网科技有限公司
 * 
 */

#ifndef __DRV_BUFFER_H
#define __DRV_BUFFER_H

typedef struct{
    unsigned char *fifo;
    unsigned short pw;
    unsigned short pr;
    unsigned short buf_size;
}RingBuffer, *ptRingBuffer;

/*
    函数名：Driver_Buffer_Init
    功能：初始化一个指定的环形缓冲区
    输入参数：buffer->指向目标缓冲区；size->表示缓冲区分配的内存大小，单位是字节
    输出参数：无
    返回值：-1->表示错误；0->表示成功
*/
int Drv_Buffer_Init(ptRingBuffer buffer, unsigned short size);

/*
    函数名：Driver_Buffer_Write
    功能：往指定的环形缓冲区写入一个字节的数据
    输入参数：buffer->指向目标缓冲区；data->写入的数据
    输出参数：无
    返回值：-1->表示错误；0->表示成功
*/
int Drv_Buffer_Write(ptRingBuffer buffer, const unsigned char data);

/*
    函数名：Driver_Buffer_WriteBytes
    功能：往指定的环形缓冲区写入多个字节的数据
    输入参数：buffer->指向目标缓冲区；data_stream->写入的数据流；len->写入的数据个数，单位是字节
    输出参数：无
    返回值：-1->表示错误；0->表示成功
*/
int Drv_Buffer_WriteBytes(ptRingBuffer buffer, const unsigned char *data_stream, unsigned char len);

/*
    函数名：Driver_Buffer_Read
    功能：从指定的环形缓冲区读出一个字节的数据
    输入参数：buffer->指向目标缓冲区；
    输出参数：data->读出的数据
    返回值：-1->表示错误；0->表示成功
*/
int Drv_Buffer_Read(ptRingBuffer buffer, unsigned char *data);

/*
    函数名：Driver_Buffer_ReadBytes
    功能：往指定的环形缓冲区读出多个字节的数据
    输入参数：buffer->指向目标缓冲区；len->读出的数据个数，单位是字节
    输出参数：data_stream->读出的数据流
    返回值：-1->表示错误；0->表示成功
*/
int Drv_Buffer_ReadBytes(ptRingBuffer buffer, unsigned char *data_stream, unsigned char len);

/*
    函数名：Driver_Buffer_Clean
    功能：清除指定的环形缓冲区
    输入参数：buffer->指向目标缓冲区
    输出参数：无
    返回值：-1->表示错误；0->表示成功
*/
int Drv_Buffer_Clean(ptRingBuffer buffer);

#endif /* __DRIVER_BUFFER_H */

