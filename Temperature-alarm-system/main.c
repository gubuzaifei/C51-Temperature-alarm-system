#include<reg51.h>
#include<math.h>
#define uchar unsigned char
#define uint unsigned int

sbit LEDRED = P1^0;
sbit LEDBLUE = P1^3;
sbit buzz = P2^3;//蜂鸣器控制端接口
sbit key1 = P3^0;//设置温度
sbit key2 = P3^1;//温度加
sbit key3 = P3^2;//温度减
sbit dula = P2^6;//数码管段选
sbit wela = P2^7;//数码管位选
/////共阴数码管段选//////////////////////////////////////////////
uchar table[22]=
{0x3F,0x06,0x5B,0x4F,0x66,		 //	0,1,2,3,4
0x6D,0x7D,0x07,0x7F,0x6F,		 //	5,6,7,8,9
0x77,0x7C,0x39,0x5E,0x79,0x71,	 //	A,B,C,D,E,F
0x40,0x38,0x76,0x00,0xff,0x37};//'-',L,H,灭,全亮，n    16-21
unsigned char code table1[]=
{0xBF,0x86,0xDB,0xCF,0xE6,
0xED,0xFD,0x87,0xFF,0xEF};
 
uchar byte_read(unsigned int byte_addr);     //字节读 
void SectorErase(unsigned int sector_addr);  //扇区擦除 
void byte_write(unsigned int byte_addr,char dat);  //字节写
extern void ReadTemperature();
int wen_du;     //温度变量  
int high,low;   //对比温度暂存变量
uchar flag,a_a;	//当前按键状态标志
uchar d1,d2,d2t,d3,d4; //显示数据暂存变量

void delay(uint ms)//延时函数，大约延时25us
{
	uchar x;
	for(ms;ms>0;ms--)
	{
		for(x=10;x>0;x--);
	}
}

void display()//显示实时温度
{       
	/*dula = 0;
	P0 = table[d1];
	delay(10);//第1位
	dula = 1;
	dula = 0;
	wela = 0;
	P0 = 0xfe;
	wela = 1;
	wela = 0;
	delay(1);*/
			  
	dula=0;
	P0=table[d1];
	delay(10);//第2位
	dula=1;
	dula=0;
	wela=0;
	P0=0xfd;
	wela=1;
	wela=0;
	delay(1);
	
	dula=0;
	P0=table1[d2];
	delay(10);//第3位
	dula=1;
	dula=0;
	wela=0;
	P0=0xfb;
	wela=1;
	wela=0;
	delay(1);
	
	dula=0;
	P0=table[d3];
	delay(10);//第4位
	dula=1;
	dula=0;
	wela=0;
	P0=0xf7;
	wela=1;
	wela=0;
	delay(1);
}
void display1()//显示调整温度  100-125	 -55-(-1)
{       
	dula = 0;
	P0 = table[d1];
	delay(10);//第1位
	dula = 1;
	dula = 0;
	wela = 0;
	P0 = 0xfe;
	wela = 1;
	wela = 0;
	delay(1);
			  
	dula=0;
	P0=table[d2];
	delay(10);//第2位
	dula=1;
	dula=0;
	wela=0;
	P0=0xfd;
	wela=1;
	wela=0;
	delay(1);
	
	dula=0;
	P0=table[d3];
	delay(10);//第3位
	dula=1;
	dula=0;
	wela=0;
	P0=0xfb;
	wela=1;
	wela=0;
	delay(1);
	
	dula=0;
	P0=table[d4];
	delay(10);//第4位
	dula=1;
	dula=0;
	wela=0;
	P0=0xf7;
	wela=1;
	wela=0;
	delay(1);
}
void display2()//显示调整温度  0-99
{       
	dula = 0;
	P0 = table[d1];
	delay(10);//第1位
	dula = 1;
	dula = 0;
	wela = 0;
	P0 = 0xfe;
	wela = 1;
	wela = 0;
	delay(1);
	
	dula=0;
	P0=table[d2];
	delay(10);//第3位
	dula=1;
	dula=0;
	wela=0;
	P0=0xfb;
	wela=1;
	wela=0;
	delay(1);
	
	dula=0;
	P0=table[d3];
	delay(10);//第4位
	dula=1;
	dula=0;
	wela=0;
	P0=0xf7;
	wela=1;
	wela=0;
	delay(1);
}

void keyscan()//按键扫描函数
{
	int i;
	if(key1==0)              //设置键按下
    {
    	delay(300);          //延时去抖
        if(key1==0)
		{
			flag=1;            //再次判断按键，按下的话进入设置状态
        }
		while(key1==0);         //松手检测                  
    }
    while(flag==1)              //进入设置上限状态
    {
    	d1=18;				 //显示字母H
		if(high>=100)
		{	
			d2=1;		     //数码管第2位显示数字1  
			d3=high%100/10;
			d4=high%100%10;     //上限温度值
        	display1();		   //调用显示函数
		}
		else if(high<100&&high>=0)
		{
			d2=high/10;
			d3=high%10;			 //上限温度值
			display2();			//调用显示函数
		}
		else
		{
			i=abs(high);		//取绝对值
			d2=16;				//数码管第2位显示负号-
			d3=i/10;
			d4=i%10;            //上限温度值
        	display1();		   //调用显示函数
		}              
        if(key1==0)             //判断设置键是否按下
        {
        	delay(300);         //延时去抖
            if(key1==0)
			{
				flag=2;         //按键按下，进入设置下限模式
            }
			while(key1==0);     //松手检测
        }
        if(key2==0)             //加键按下
        {
        	delay(300);         //延时去抖
            if(key2==0)         //加键按下
            {
            	high+=1;        //上限加1
                if(high>=125)
				{
					high=125;    //上限最大加到125
                }
			}
			while(key2==0);     //松手检测
        }
        if(key3==0)             //减键按下
        {
        	delay(300);          //延时去抖
            if(key3==0)         //减键按下
            {
            	high-=1;        //上限减1
                if(high<=-55)
				{
					high=-55;    //上限最小减到-55
                }
			}
			while(key3==0);     //松手检测
        }               
	}
    while(flag==2)              //设置下限
    {
    	d1=17;					//显示字母L
		if(low>=100)
		{	
			d2=1;		     //数码管第2位显示数字1  
			d3=low%100/10;
			d4=low%100%10;      //下限温度值
        	display1();		   //调用显示函数
		}
		else if(low<100&&low>=0)
		{
			d2=low/10;
			d3=low%10;		//下限温度值
			display2();		//调用显示函数
		}
		else
		{
			i=abs(low);		//取绝对值
			d2=16;
			d3=i/10;
			d4=i%10;        //下限温度值
        	display1();		//调用显示函数
		}
        if(key1==0)
        {
        	delay(300);
            if(key1==0)
			{
				flag=0;
			}
            while(key1==0);     //松手检测
        	SectorErase(0x2e00);//擦除扇区命令
	    	byte_write(0x2e00,high%256);//把high（温度上限）写入扇区
	    	byte_write(0x2e01,high/256);
	                
	   		byte_write(0x2e20,low%256); //把low（温度下限）写入扇区
	    	byte_write(0x2e21,low/256); 
	                         
	    	byte_write(0x2e55,a_a);  //把a_a的值写入扇区
		}
        if(key2==0)
        {
        	delay(300);
            if(key2==0)
            {
            	low+=1;
                if(low>=125)
				{
					low=125;
				}       
            }
			while(key2==0);      //松手检测
        }
        if(key3==0)
        {
        	delay(300);
            if(key3==0)
            {
            	low-=1;
                if(low<=-55)
				{
					low=-55;
				}       
            }
			while(key3==0);//松手检测
        }		               
	}
		
}
void zi_dong()//自动温控模式
{
	d1=wen_du/100;
   	d2t=wen_du%100;
   	d2=d2t/10;
   	d3=d2t%10;
    keyscan();		//按键扫描函数
    display();		//调用显示函数
    if((wen_du/10)<=low)	//低温警报
	{
		//buzz=0;
		LEDBLUE = 0;
	}			//低于下限 蜂鸣器响亮蓝灯
    if(((wen_du/10)>low)&&((wen_du/10)<high))//温度大于下限，小于上限 
    {                                                                                        
        buzz = 1;
		P1 = 0xff;                   
    }
    if((wen_du/10)>=high)	 //高温警报
	{
		//buzz = 0;
		LEDRED = 0;
	}			//高于上限 蜂鸣器响亮红灯
}
void main()                  //主函数
{	
	uint j;
	high = byte_read(0x2e01);//程序开始时读取EEPROM中的数据（读取上限） 
    high <<= 8;
    high |= byte_read(0x2e00);
    if(high>200)
	{
		high = high - 256;
	}
	low = byte_read(0x2e21);//        程序开始时读取EEPROM中的数据（读取下限）        
    low <<= 8;
    low |= byte_read(0x2e20);
    if(low>200)
	{
		low = low - 256;
	}
	a_a = byte_read(0x2e55);//         
    if(a_a!=22) //防止首次上电时读取出错 
    {
    	high=30;        //第一次上电温度上限为30.C    
        low=20;         //第一次上电温度下限为20.C
    	a_a=22;
   	}          
	delay(150);	
    for(j=0;j<80;j++)          //先读取温度值，防止开机显示85
    {
		ReadTemperature();
    }
	while(1)                          //进入while循环
    {       
    	ReadTemperature();        //读取温度值
        for(j=0;j<100;j++)
		{
			zi_dong();//温控模式
		}
    }
}
