#include<reg51.h>
#include <intrins.h>

sfr ISP_DATA =  0xe2; //Flash数据寄存器地址（E2h）
sfr ISP_ADDRH = 0xe3;//Flash高字节寄存器地址（E3h）
sfr ISP_ADDRL = 0xe4;//Flash低字节寄存器地址（E4h）
sfr ISP_CMD =   0xe5;  //Flash命令模式寄存器地址（E5h）
sfr ISP_TRIG =  0xe6; //Flash命令触发寄存器地址（E6h）
sfr ISP_CONTR = 0xe7;//Flash控制寄存器地址（E7h）
#define uchar unsigned char
#define WaitTime 0x81 //定义CPU的等待时间  在ISPEN=1时,对ISP_TRIG （命令触发寄存器）先写入46h，再写入B9h，ISP/IAP命令才会生效。
#define RdCmd 0x01 //定义ISP的操作命令 
#define PrgCmd 0x02 
#define EraseCmd 0x03

void ISP_IAP_enable(void);                  /* ================ 打开 ISP,IAP 功能 ================= */
void ISP_IAP_disable(void);                 /* =============== 关闭 ISP,IAP 功能 ================== */ 
void ISP_trig(void);                        /* ================ 公用的触发代码 ==================== */
uchar byte_read(unsigned int byte_addr);    /* ============ 字节读 ============= */ 
void SectorErase(unsigned int sector_addr); /* =========== 扇区擦除 ============ */ 
void byte_write(unsigned int byte_addr, unsigned char dat); /* ==== 字节写 ==== */

void ISP_IAP_enable(void) /*================ 打开 ISP,IAP 功能 =================*/ 
{
	EA = 0;                             //关中断
	ISP_CONTR = ISP_CONTR & 0x18;       //ISP/IAP控制寄存器复位 
	ISP_CONTR = ISP_CONTR | WaitTime;   //(10000001) ISPEN = 1写入硬件延时
} 
void ISP_IAP_disable(void) /*=============== 关闭 ISP,IAP 功能 ==================*/ 
{ 
	ISP_CONTR = ISP_CONTR & 0x7f;       //(01111111)ISPEN = 0
	ISP_TRIG = 0x00; 					//清除ISP_TRIG寄存器
	EA = 1;                             //开中断
} 
void ISP_trig(void) /*================触发代码 ====================*/ 
{ 
	ISP_IAP_enable(); 
	ISP_TRIG = 0x46; //触发ISP_IAP命令高字节ISP_ADDRH写入     对ISP_TRIG先写入46h，
	ISP_TRIG = 0xb9; //触发ISP_IAP命令低字节ISP_ADDRL写入     再写入B9h，ISP/IAP命令才会生效。 
	_nop_(); 
} 
unsigned char byte_read(unsigned int byte_addr) /*========= 字节读 =============*/ 
{ 
	ISP_ADDRH = (unsigned char)(byte_addr >> 8);     //读出byte_addr的高八位
	ISP_ADDRL = (unsigned char)(byte_addr & 0x00ff); //读出byte_addr的低八位
	ISP_CMD   = ISP_CMD & 0xf8;   //(11111000)清除低3位 
	ISP_CMD   = ISP_CMD | RdCmd;  //写入读命令  
	ISP_trig();                   //触发执行 
	ISP_IAP_disable();            //关闭ISP,IAP功能 
	return (ISP_DATA);            //返回读到的数据 
} 
void SectorErase(unsigned int sector_addr) /*=========== 扇区擦除 ============*/ 
{ 
	unsigned int iSectorAddr; 
	iSectorAddr = (sector_addr & 0xfe00); //取扇区地址 
	ISP_ADDRH = (unsigned char)(iSectorAddr >> 8); 
	ISP_ADDRL = 0x00; 
	ISP_CMD = ISP_CMD & 0xf8;     //(11111000)清除低3位
	ISP_CMD = ISP_CMD | EraseCmd; //写入扇区擦除命令 
	ISP_trig();       			  //触发执行 
	ISP_IAP_disable();            //关闭ISP,IAP功能
} 
void byte_write(unsigned int byte_addr, unsigned char dat) /*==== 字节写 ====*/ 
{ 
	ISP_ADDRH = (unsigned char)(byte_addr >> 8);     //读出byte_addr的高八位
	ISP_ADDRL = (unsigned char)(byte_addr & 0x00ff); //读出byte_addr的低八位
	ISP_CMD  = ISP_CMD & 0xf8;    //(11111000)清除低3位
	ISP_CMD  = ISP_CMD | PrgCmd;  //写入写命令 
	ISP_DATA = dat;               //写入数据准备 
	ISP_trig();                   //触发执行 
	ISP_IAP_disable();            //关闭ISP,IAP功能 
}