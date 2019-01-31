/**********************51单片机学习例程************************
*  platform:Keil U5 + STC89C52
*  name:UART发送字符串——Hello,World
*  author:Liya
*  date:2019-01-30
******************************************************************/

#include <reg52.h>
#define JINGZHEN     11059200UL                   /*使用11.0592M晶体*/         
#define BOTELV   9600UL                          /*波特率定义为9600*/
unsigned char String1[]="Hello,World!";                 //待显示字符串1
unsigned char String2[]="I will do my best!";           //待显示字符串2

void delay(unsigned char i)
{
    unsigned char j,k;
    for(j=i;j>0;j--)
        for(k=210;k>0;k--);
}

void init(void)                                //串口初始化
{
    EA=0; //暂时关闭中断
    TMOD&=0x0F;  //定时器1模式控制在高4位
    TMOD|=0x20;    //定时器1工作在模式2，自动重装模式
    SCON=0x50;     //串口工作在模式1
    TH1=256-JINGZHEN/(BOTELV*12*16);  //计算定时器重装值
    TL1=256-JINGZHEN/(BOTELV*12*16);
    PCON|=0x80;    //串口波特率加倍
    ES=1;         //串行中断允许
    TR1=1;        //启动定时器1
    REN=1;        //允许接收 
    EA=1;         //允许中断
}

void send(unsigned char d)  //发送一个字节的数据，形参d即为待发送数据。
{
 
    SBUF=d; //将数据写入到串口缓冲
    while(!TI);
}

void sendc(unsigned char * pd)
{
    while((*pd)!='\0') //发送字符串，直到遇到0才结束
    {
        send(*pd); //发送一个字符
        pd++;  //移动到下一个字符
    }
}

int main()
{
    init();

    sendc(String1);
    delay(2000);
    sendc(String2);

    return(0);
}

void uartinterrupt(void) interrupt 4                 //串口发送中断
{
    if (RI)      //收到数据
    {
        RI=0;   //清空收到数据标志
    }
    if (TI)     //发送完一字节数据
    {
        TI=0;   //清空发送数据标志
    }
}
