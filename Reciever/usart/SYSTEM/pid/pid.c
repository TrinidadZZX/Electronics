#include "pid.h"
#define ARR			1400			//计数值	10Hz
#define PSC			3600			//预分频	10kHz
#define restriction 20
#define x_init	950
#define y_init 1020
typedef struct {
    float kp;
    float ki;
    float kd;
} PID;
typedef struct{
	u16 x;
	u16 y;
}Locat;

extern int pid_count;
PID Inter_PID[2];			//X,Y
#define X 0
#define Y	1

//期望值
//激光笔 期望值
extern Locat rayLocat;
//记录期望值的变化
int x_new_expt[2];				//x轴角度 控制量		
int y_new_expt[2];				//y轴角度 控制量（弧度制）

double t_update = 0.01;

//double rate_0=0;					//平均转速占空比
//double rate_p=0;					//俯仰差动占空比
//double rate_r=0;					//侧翻差动占空比
//double rate_y=0;					//偏航差动占空比


//实际测量值
//小球实际坐标
extern Locat ballLocat;
//记录实际值的变化
float x_real[3]; 		//x角度 测量量（弧度制）
float y_real[3];	   	//y角度 测量量（弧度制）

float x_err[3];	 		//角度 偏差量（弧度制）
float y_err[3];	   	//角度 偏差量（弧度制）

float x_I=0;			 			//角度error 积分值（弧度制）
float y_I=0;			     	//角度error 积分值（弧度制）

float x_D;		   				//角度  微分值（弧度制）
float y_D;				 			//角度  微分值（弧度制）



void NewExpt(void)   //对 UpdateCtrl 函数给出的控制量进行补偿
{
    x_new_expt[1]=x_new_expt[0];
    y_new_expt[1]=y_new_expt[0];

    x_new_expt[0]=rayLocat.x;
    y_new_expt[1]=rayLocat.y;
}




void TIM2_PID_Init(void)  //PID TIM2 中断配置
{
    //中断基础配置
	float temp;

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    NVIC_InitTypeDef NVIC_InitStructure;		 //	  命名一优先级变量

    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseStructure. TIM_Period = ARR;		     //配置计数阈值，超过时，自动清零，并触发中断
    TIM_TimeBaseStructure.TIM_Prescaler = PSC;	   //时钟预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	 // 时钟分频倍数
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	 // 计数方式为向上计数

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);		 //初始化tim2
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);             //清除TIM2溢出中断标志
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);               //使能TIM2的溢出更新中断
    TIM_Cmd(TIM2,ENABLE);			     //使能TIM2

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; //TIM2	溢出更新中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //打断优先级为1，与上一个相同，不希望中断相互打断对方
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 	//响应优先级1，低于上一个，当两个中断同时来时，上一个先执行
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    //PID参数（固定）

    t_update=(PSC+1)*(ARR+1)/72000000.0;

    // X
    Inter_PID[X].kp=0.2;//0.8
    Inter_PID[X].ki=0;
    Inter_PID[X].kd=1;//1


    // Y
    Inter_PID[Y].kp=0.2;//0.8
    Inter_PID[Y].ki=0;
    Inter_PID[Y].kd=1;

}
int angle_x=0 , angle_y=0;//实际值
void TIM2_IRQHandler(void)        //计算各 PWM 增量
{
    int i=0;
		float temp;
		static int delta_x, delta_y;//增量
		
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);		 //清空TIM2溢出中断响应函数标志位
		pid_count++;
    //更新期望值
    NewExpt();

    //检测积分清零
    //Clear_all_I(5);

    //更新角度缓冲器
    for(i=2; i>0; i--)
    {
        x_real[i]=x_real[i-1];
        y_real[i]=y_real[i-1];
    }
		if(ballLocat.x==0 && ballLocat.y==0){
			TIM_SetCompare3(TIM4, x_init);
			TIM_SetCompare4(TIM4, y_init);
			return;
		}
		else{
			x_real[0]=ballLocat.x;
			y_real[0]=ballLocat.y;      //角度180°补偿模块自动实现
		}

    //更新角度偏差向量
    for(i=2; i>0; i--)
    {
      x_err[i]=x_err[i-1];
			y_err[i]=y_err[i-1];
    }
    x_err[0]=ballLocat.x-rayLocat.x;
    y_err[0]=ballLocat.y-rayLocat.y;



    //更新内环积分值 限幅？？？？？？？？？？？？

    //x_I += t_update*x_err[0];
    //if ((pitch_rate_I>20||pitch_rate_I<-20))
    //    pitch_rate_I = pitch_rate_I>20?20:-20;


    //y_I += t_update*y_err[0];
    //if ((roll_rate_I>20||roll_rate_I<-20))
    //    roll_rate_I = roll_rate_I>20?20:-20;

    //更新内环微分值：
    //x_D = (x_err[0]-x_err[1])/t_update;
    //y_D = (y_err[0]-y_err[1])/t_update;


    //解算各 PWM 增量
		
		angle_x =(int)(Inter_PID[X].kp * x_err[0] + Inter_PID[X].kd * (x_err[0] - x_err[1]));// + Inter_PID[X].ki * x_I + Inter_PID[X].kd * x_D;
		
		angle_x= angle_x>restriction?restriction:angle_x;
		angle_x = angle_x<-restriction?-restriction:angle_x;
 // y=11x+990      -90-90   0---2000 
		
		TIM_SetCompare3(TIM4,11*angle_x+x_init);
		angle_y =(int)(Inter_PID[Y].kp * y_err[0] + Inter_PID[Y].kd * (y_err[0] - y_err[1]));// + Inter_PID[X].ki * x_I + Inter_PID[X].kd * x_D;
		
		angle_y = angle_y>restriction?restriction:angle_y;
		angle_y = angle_y<-restriction?-restriction:angle_y;
		TIM_SetCompare4(TIM4, 11*angle_y+y_init);
}

