#include "pid.h"
#define ARR			1400			//����ֵ	10Hz
#define PSC			3600			//Ԥ��Ƶ	10kHz
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

//����ֵ
//����� ����ֵ
extern Locat rayLocat;
//��¼����ֵ�ı仯
int x_new_expt[2];				//x��Ƕ� ������		
int y_new_expt[2];				//y��Ƕ� �������������ƣ�

double t_update = 0.01;

//double rate_0=0;					//ƽ��ת��ռ�ձ�
//double rate_p=0;					//�����ռ�ձ�
//double rate_r=0;					//�෭�ռ�ձ�
//double rate_y=0;					//ƫ���ռ�ձ�


//ʵ�ʲ���ֵ
//С��ʵ������
extern Locat ballLocat;
//��¼ʵ��ֵ�ı仯
float x_real[3]; 		//x�Ƕ� �������������ƣ�
float y_real[3];	   	//y�Ƕ� �������������ƣ�

float x_err[3];	 		//�Ƕ� ƫ�����������ƣ�
float y_err[3];	   	//�Ƕ� ƫ�����������ƣ�

float x_I=0;			 			//�Ƕ�error ����ֵ�������ƣ�
float y_I=0;			     	//�Ƕ�error ����ֵ�������ƣ�

float x_D;		   				//�Ƕ�  ΢��ֵ�������ƣ�
float y_D;				 			//�Ƕ�  ΢��ֵ�������ƣ�



void NewExpt(void)   //�� UpdateCtrl ���������Ŀ��������в���
{
    x_new_expt[1]=x_new_expt[0];
    y_new_expt[1]=y_new_expt[0];

    x_new_expt[0]=rayLocat.x;
    y_new_expt[1]=rayLocat.y;
}




void TIM2_PID_Init(void)  //PID TIM2 �ж�����
{
    //�жϻ�������
	float temp;

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    NVIC_InitTypeDef NVIC_InitStructure;		 //	  ����һ���ȼ�����

    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseStructure. TIM_Period = ARR;		     //���ü�����ֵ������ʱ���Զ����㣬�������ж�
    TIM_TimeBaseStructure.TIM_Prescaler = PSC;	   //ʱ��Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	 // ʱ�ӷ�Ƶ����
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	 // ������ʽΪ���ϼ���

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);		 //��ʼ��tim2
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);             //���TIM2����жϱ�־
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);               //ʹ��TIM2����������ж�
    TIM_Cmd(TIM2,ENABLE);			     //ʹ��TIM2

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; //TIM2	��������ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //������ȼ�Ϊ1������һ����ͬ����ϣ���ж��໥��϶Է�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 	//��Ӧ���ȼ�1��������һ�����������ж�ͬʱ��ʱ����һ����ִ��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    //PID�������̶���

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
int angle_x=0 , angle_y=0;//ʵ��ֵ
void TIM2_IRQHandler(void)        //����� PWM ����
{
    int i=0;
		float temp;
		static int delta_x, delta_y;//����
		
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);		 //���TIM2����ж���Ӧ������־λ
		pid_count++;
    //��������ֵ
    NewExpt();

    //����������
    //Clear_all_I(5);

    //���½ǶȻ�����
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
			y_real[0]=ballLocat.y;      //�Ƕ�180�㲹��ģ���Զ�ʵ��
		}

    //���½Ƕ�ƫ������
    for(i=2; i>0; i--)
    {
      x_err[i]=x_err[i-1];
			y_err[i]=y_err[i-1];
    }
    x_err[0]=ballLocat.x-rayLocat.x;
    y_err[0]=ballLocat.y-rayLocat.y;



    //�����ڻ�����ֵ �޷�������������������������

    //x_I += t_update*x_err[0];
    //if ((pitch_rate_I>20||pitch_rate_I<-20))
    //    pitch_rate_I = pitch_rate_I>20?20:-20;


    //y_I += t_update*y_err[0];
    //if ((roll_rate_I>20||roll_rate_I<-20))
    //    roll_rate_I = roll_rate_I>20?20:-20;

    //�����ڻ�΢��ֵ��
    //x_D = (x_err[0]-x_err[1])/t_update;
    //y_D = (y_err[0]-y_err[1])/t_update;


    //����� PWM ����
		
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

