/*组员：柴凯昕、高心如、邱卓、朱颜泽
	时间：2020/12/4
	单片机版本：STC89C52
	晶振频率：32M（重要！该项目对时序要求严格）
	烧录程序时需要打开 6T:双倍速模式
	单片机机器周期0.375us
	打开6T模式后，机器周期（相比12T模式）缩减一倍1.875*10^-7s
*/

#include <REG52.H>
#include <intrins.h>
#define WSCOUNT 20 //灯带LED数
sbit k=P1^0;//灯带接口
sbit hongwai = P3^2;//红外遥控接收端接口

unsigned char recieve[3];	//储存用户码、用户反码、键数据码、键数据反码
unsigned char WS2812_BUffer[WSCOUNT][3];//储存每个灯的RGB数


/*延时函数 delay
作用：输入x，延时时间：x*没调出来呢 ms
*/
void delay(unsigned char x)
{
  unsigned char i,j;
  for(i=x;i>0;i--)
         for(j=11;j>0;j--);
}

/*红外接收器专用的延时函数 IRdelay
作用：延时x*0.14MS
注：一个for循环占用8个机器周期，93是慢慢调出来的。。
*/
void IRdelay(unsigned char x)    
{
	unsigned char i;
  while(x--)
	{
		for (i = 0; i<93; i++);
	}
}

/*向灯带传输数据的函数 WS2812_write_date()
	使用另一个WS2812_Ser()函数来达到向WS2812传输数据的时序控制要求 
*/
void WS2812_write_date()
{	
		bit w0,w1,w2,w3,w4,w5,w6,w7;
		unsigned char i,j,shu;
		for(i=0;i<WSCOUNT;i++)//对每个灯依次传输数据
	{
		for(j=0;j<3;j++)//对一个灯的R、G、B数依次传输数据
		{
				switch(j)//对shu赋值R/G/B值
				{
					case 0:shu=WS2812_BUffer[i][1];break;
					case 1:shu=WS2812_BUffer[i][0];break;
					case 2:shu=WS2812_BUffer[i][2];break;
				}
				//移位，数字转化为bit
				w7=shu&0x80;w6=shu&0x40;//取最高位(0或1)赋给w7
				w5=shu&0x20;w4=shu&0x10;
				w3=shu&0x08;w2=shu&0x04;
				w1=shu&0x02;w0=shu&0x01;
			  //时序分析
				k=0;
				if(w7) k=1;//p1^0高电平
				else k=0;
				k=1;
				_nop_();
				
				k=0;
				if(w6) k=1;
				else k=0;
				k=1; 
				_nop_();
				
				k=0;
				if(w5) k=1;
				else k=0;
				k=1; 
				_nop_();
				
				k=0;
				if(w4) k=1;
				else k=0;
				k=1;
				_nop_();

				k=0;
				if(w3) k=1;
				else k=0;
				k=1; 
				_nop_();
				
				k=0;
				if(w2) k=1;
				else k=0;
				k=1; 
				_nop_();
				
				k=0;
				if(w1) k=1;
				else k=0;
				k=1; 
				_nop_();
				
				k=0;
				if(w0) 
				k=1;
				else k=0;
				k=1; 
				_nop_();
				k=0;
		}
}}


/*为满足时序要求，写的延时复位函数WS2812_Ser()
*/
void WS2812_Ser()
{
	static unsigned char count=0;
	count++;
	switch(count)
	{
		case 1:k=0;break;//复位
		case 50:EA=0;WS2812_write_date();EA=1;break;//当count=50时传输数据
		case 51:k=0;break;
	}
	count%=60;
}


/*使灯全亮的函数WS2812_Allot()
	没啥用。。。注释掉了
*/

/*void WS2812_Allot()
{
		unsigned char j;
	  //unsigned char i;
		static unsigned char count=0;
		count++;
		if(count==60)
		{
					WS2812_BUffer[j][0]=320;//R
					WS2812_BUffer[j][1]=320;//G
					WS2812_BUffer[j][2]=320;//B
				for(j=0;j<WSCOUNT;j++)
				{
					WS2812_BUffer[j][0]=WS2812_BUffer[j-1][0]+10;//R
					WS2812_BUffer[j][1]=WS2812_BUffer[j-1][1]+10;//G
					WS2812_BUffer[j][2]=WS2812_BUffer[j-1][2]+10;//B
				}
			
		}
		WS2812_Ser();
}
*/



/*开机自检函数WS2812_in_turn_on()
作用：使灯带LED从内向外依次亮起，亮起的灯不灭
*/
void WS2812_in_turn_on()//
{
	static unsigned char count=0,j=0;//j控制着颜色一直改变，所以设成static
	count++;//我也不知道为什么有这句:D哈哈,但是没有的话会出错，灯会不规则闪动
	if(count==60)
	{ //控制颜色,可以每个灯的颜色一直在改变
		//对RGB的颜色调节可以随意改变
		WS2812_BUffer[j%WSCOUNT][0]=50+8*j;//R
		WS2812_BUffer[j%WSCOUNT][1]=50+10*j;//G
		WS2812_BUffer[j%WSCOUNT][2]=50+15*j;//B
		j++;
		if(j>100)j=0;
	}
	WS2812_Ser();
	delay(20);
}

void WS2812_in_turn_on2()//
{
		  unsigned char i;
		static unsigned char count=0;
		count++;
		
		if(count==60)
		{
			for(i=0;i<WSCOUNT;i++){
				
				  WS2812_BUffer[i][0]=104;//R
					WS2812_BUffer[i][1]=34;//G
					WS2812_BUffer[i][2]=139;//B
			}
		}
		WS2812_Ser();
		delay(40);

}



/*渐变色*/
void WS2812_jianbian()//
{
	
	static unsigned char count=0,j=0,i=0;//j控制着颜色一直改变，所以设成static
	unsigned char a1[10][3]={{238,44,44},{255,127,36},{255,215,0},{0,255,0},{0,191,255},{65,105,225},{160,32,240},{153,50,204},{255,20,147},{178,34,34}};
	count++;
	if(count==60)
	{ 
		WS2812_BUffer[j%WSCOUNT][0]=a1[((j%WSCOUNT)/2+i)%10][0];//R
		WS2812_BUffer[j%WSCOUNT][1]=a1[((j%WSCOUNT)/2+i)%10][1];//G
		WS2812_BUffer[j%WSCOUNT][2]=a1[((j%WSCOUNT)/2+i)%10][2];//B
		j++;
		if(j%WSCOUNT==0){
		i=i+3;}
	}
	WS2812_Ser();
	delay(9);
}

/*形成从内向外的同心圆的函数WS2812_concentric_circles_out()
	实质是LED从内向外依次亮起
*/
void WS2812_concentric_circles_out()
{
	  unsigned char i;
		static unsigned char count=0,j=0;//j为第几个灯
		count++;
		if(count==60)
		{
			for(i=0;i<WSCOUNT;i++){
				  WS2812_BUffer[i][0]=0;//R
					WS2812_BUffer[i][1]=0;//G
					WS2812_BUffer[i][2]=0;//B
			}
					WS2812_BUffer[j%WSCOUNT][0]=5+5*j;//R
					WS2812_BUffer[j%WSCOUNT][1]=5+6*j;//G
					WS2812_BUffer[j%WSCOUNT][2]=5+8*j;//B
			j++;
		}
		WS2812_Ser();
		delay(70);
		return;
}


/*形成从外向内的同心圆的函数WS2812_concentric_circles_in()
	实质是LED从外向内依次亮起
*/
void WS2812_concentric_circles_in()
{		
	  unsigned char i;
		static unsigned char count=0,j=WSCOUNT-1;
		count++;
		if(count==60)
		{
			for(i=0;i<WSCOUNT;i++){
				  WS2812_BUffer[i][0]=0;//R
					WS2812_BUffer[i][1]=0;//G
					WS2812_BUffer[i][2]=0;//B
			}
			
					WS2812_BUffer[j%WSCOUNT][0]=238;//R
					WS2812_BUffer[j%WSCOUNT][1]=99;//G
					WS2812_BUffer[j%WSCOUNT][2]=99;//B
			
			j--;
			if(j<0) j=WSCOUNT-1;//重返顶端
		}
		WS2812_Ser();
		delay(50);
}

void WS2812_concentric_circles_in2()
{		
	  unsigned char i;
		static unsigned char count=0,j=0;
		count++;
		
		if(count==60)
		{j++;
			for(i=0;i<WSCOUNT;i++){
				if(i%3==0 && j%3==0){
				  WS2812_BUffer[i][0]=0;//R
					WS2812_BUffer[i][1]=0;//G
					WS2812_BUffer[i][2]=0;//B
					}
				else{
				  WS2812_BUffer[i][0]=30;//R
					WS2812_BUffer[i][1]=144;//G
					WS2812_BUffer[i][2]=255;//B
					}
			}
		}
		WS2812_Ser();
		delay(150);

}



/*接收转换红外信号的解码函数IR_IN()
	利用外部中断函数interrupt 0进行数据解码，
	其任务是检验并接收正确的数据信号，对用户码和数据码进行分析，
	即通过检测高低电平持续的时间来判断相应的位是“0”还是“1”
	然后得出相对应的键值，把键值对应的数储存在recieve[2]中
*/
void IR_IN() interrupt 0 //外部中断0，即红外接收P3.2
{
	unsigned char j,k,count=0;
	EX0 = 0; //关闭外部中断0，即停止接收红外         
	IRdelay(5);
	if (hongwai==1)
	{
		EX0 =1;
		return;
	} //确认IR信号出现
	while (!hongwai)//等IR变为高电平，跳过9ms的前导低电平信号。
	{
		IRdelay(1);
	}
	for (j=0;j<4;j++)//收集四组数据
	{
		for (k=0;k<8;k++)//每组数据有8位
		{
			while (hongwai)//等 IR 变为低电平，跳过4.5ms的前导高电平 信号。
			{
				IRdelay(1);
			}
			while (!hongwai)//等 IR 变为高电平
			{
				IRdelay(1);}
			while (hongwai)//计算IR高电平时长
			{
				IRdelay(1);
				count++;           
				if (count>=30)
				{
					EX0=1;
					return;
				}//0.14ms计数过长自动离开。
			}//高电平计数完毕               
			recieve [j]= recieve [j] >> 1;//数据最高位补"0"
      if (count>=8)
			{
				recieve [j] = recieve [j] | 0x80;
			}//数据最高位补1
			count=0;
		}//end for k        
	}//end for j   
  if (recieve [2]!=~recieve [3])//校验键值码与键值反码的值，
	{//如果键值码与键值反码取反后相等，则开外部中断中断0，开始接收下一个红外信号
		EX0=1;    
		return;      
	}      
	EX0 = 1;
}


/*选择灯带闪烁模式的函数select_mode(unsigned char key)
	调用IR_IN接收数据，使用switch找到recieve[2]对应的模式，
	case(键值): 调用模式函数
*/
void select_mode(unsigned char key)//传入键值
{
	switch(key)    //判断按键键码值
  {
		case 0x0D:break;  //(#)
		case 0x19:WS2812_in_turn_on2();break;  //（0）
		case 0x40:WS2812_jianbian();break;  //（5）
		case 0x43:WS2812_jianbian();break;  //（6）
		case 0x44:WS2812_jianbian();break;  //（4）
		case 0x45:WS2812_in_turn_on();break;  //（1）
		case 0x46:WS2812_concentric_circles_out();break;  //（2）
		case 0x47:WS2812_concentric_circles_in();break;  //（3）
		case 0x07:WS2812_concentric_circles_in2();break;  //（7）
		case 0x18:WS2812_concentric_circles_in();break;  //（上）
		case 0x09:WS2812_concentric_circles_in();break;  //（9）
		case 0x08:WS2812_concentric_circles_in();break;  //（左）
		case 0x13:break;  //（ok）
		case 0x51:break;  //（右）
		case 0x15:break;  //（8）
		case 0x52:break;  //（下）
		case 0x16:WS2812_concentric_circles_in();break;  // （*）
		default :break; 
	}
}



/*主函数：
初始化红外接收引脚（把P3.2外部中断引脚作为红外接收引脚）、打开外部中断允许位；
*/
void main()
{
	EA=1;//开放全局中断
	EX0=1;//开外部中断0
	IT0=1;//触发方式为脉冲负边沿触发
	hongwai=1;
	while(1)
	{ select_mode(recieve[2]);
	}
}