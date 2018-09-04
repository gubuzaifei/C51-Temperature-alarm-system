#include<reg51.h>

#define uchar unsigned char
#define uint unsigned int
sbit DQ = P2^2;//温度传感器接口 
bit     f=0;						  //测量温度的标志位,0’表示“正温度”‘1’表示“负温度”）
bit     f_max=0;					  //上限温度的标志位‘0’表示“正温度”‘1’表示“负温度”）
bit     f_min=0;					  //下限温度的标志位‘0’表示“正温度”‘1’表示“负温度”）
extern uint wen_du;               //温度变量  
uchar max=0x00,min=0x00;		   //max是上限报警温度，min是下限报警温度
uchar   temp=0; 			          //测量温度的整数部分
uchar   temp_d=0; 		              //测量温度的小数部

/***********ds18b20延迟子函数*******/
void delay_18B20(uint i)
{
	while(i--);
}
/**********ds18b20初始化函数**********************/
void Init_DS18B20()
{
	uchar x=0;
	DQ=1;          //DQ复位
	delay_18B20(8);  //稍做延时
	DQ=0;          //单片机将DQ拉低
	delay_18B20(80); //精确延时 大于 480us
	DQ=1;          //拉高总线
	delay_18B20(14);
	x=DQ;            //稍做延时后 如果x=0则初始化成功 x=1则初始化失败
	delay_18B20(20);
}
/***********ds18b20读一个字节**************/  
uchar ReadOneChar()
{
	uchar i;
	uchar dat=0;
	for (i=8;i>0;i--)
	{
		DQ=0; // 给脉冲信号
		dat>>=1;
		DQ=1; // 给脉冲信号
		if(DQ)
		{
			dat|=0x80;
		}
		delay_18B20(4);
	}
	return(dat);
}
/*************ds18b20写一个字节****************/  
void WriteOneChar(uchar dat)
{
	uchar i;
	for (i=8;i>0;i--)
	{
		DQ=0;
		DQ=dat&0x01;
		delay_18B20(5);
		DQ=1;
		dat>>=1;
	}
}
/**************读取ds18b20当前温度************/
void ReadTemperature()
{
	float tt;
	uchar a=0,b=0;
	Init_DS18B20();
	WriteOneChar(0xCC);         // 跳过读序号列号的操作
	WriteOneChar(0x44);         // 启动温度转换
	delay_18B20(100);       
	Init_DS18B20();
	WriteOneChar(0xCC);         //跳过读序号列号的操作
	WriteOneChar(0xBE);         //读取温度寄存器等（共可读9个寄存器） 前两个就是温度
	delay_18B20(100);
	a=ReadOneChar();            //读取温度值低位
	b=ReadOneChar();            //读取温度值高位
	wen_du=b;
  	wen_du<<=8;             //two byte  compose a int variable
  	wen_du=wen_du|a;
  	tt=wen_du*0.0625;
  	wen_du=tt*10+0.5;
	//wen_du=((b*256+a)>>4);    //当前采集温度值除16得实际温度值
	/*wen_du=b;
	wen_du<<=8;             
	wen_du=wen_du|a;
	tt=wen_du*0.0625;
	wen_du=tt*10+0.5; */
}