#include<stc15.h>
#include<math.h>
#include<intrins.h> 
#define KeyPort P0
#define Backspace P3^2
#define pi 3.1415926
    
sbit    P_HC595_SER   = P4^0;
sbit    P_HC595_RCLK  = P5^4;   
sbit    P_HC595_SRCLK = P4^3;   
sbit    SRAM=P2^7;
//     0   1    2    3    4    5    6    7    8    9    ��    .   -
unsigned char code duan[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00,0x80,0x40};
unsigned char code wei[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
unsigned char idata Data[8];
unsigned char idata Pos[8];
unsigned char idata arr[8];
unsigned char idata arrA[8];
unsigned char idata arrB[8];
unsigned char operator=0;
unsigned char i;

float numA=0;
float numB=0;
int count=0;
int countA=0;
int countB=0;
long int zheng=0;
int xiaoa=0;
float xiao=0;
float buffer=0;

int pointA=-1;   //AС����λ
int pointB=-1;   //BС����λ
int point=-1;
int pwei=0;
int pmore=0; //�������С����λ��
int pflag=0; //����С����
int poper=0; //���й�����
int fuA=0;  //�ж��Ƿ��Ǹ��� ����
int fuB=0;
int fu=0; //�ж��Ƿ��и���
unsigned char idata num;
void Delay2x(unsigned char t);
void Delayms(unsigned char t);
void Send_595(unsigned char dat, unsigned char dat2); 
unsigned char KeyScan(void);
unsigned char KeyPro(void);
unsigned int Get_ADC10bitResult(unsigned char channel);
void CalculateAdcKey();

unsigned int ADC_KeyState,ADC_KeyState1,ADC_KeyState2,ADC_KeyState3; //��״̬
unsigned int ADC_KeyHoldCnt;    //�����¼�ʱ
unsigned int KeyCode;   //���û�ʹ�õļ���, 1~16��Ч

/*
*Ҫע������������⣬��Ȼ���ݶ�����ˣ���������������е���ʾ�����������λ�õ���ʾ���Ӱ��
*/

void X0_ISR(void) interrupt 0{//"Backspace"����ǰλȡ����
    Delayms(10);
    if(Backspace==0){
        EX0=0;
        if(pflag==1 && (countA==pointA))//����if��ɾС��������,С���������һλʱ��ɾС����
        { 
			pflag=0;
            point=-1;
        }     
        else{
            Data[countA]=duan[10];
            arrA[countA]=0;
            countA=countA-1;
            count=countA;
            if(pflag==0){   //ֻɾ��ʱ��pointAҲҪ������
                pointA=countA;
            }
            if(countA==0){
                Data[0]=duan[0];      //��λ����ʾ0
                count=1;
            }
        }
    }
    if(Backspace==1)EX0=1;
}

void X1_ISR(void) interrupt 2{//"Reset"����λ��
        for(i=0;i<countA;i++){                       
            arrA[i]=0;          
            arr[i]=0;
        }           
        for(i=0;i<=countB;i++){
            arrB[i]=0;
        }

        Data[0]=duan[0];      //��λ����ʾ0
        count=1;
        countA=0;
        countB=0;
        numA=0;
        numB=0;
        operator=0;

        pointA=-1;   //AС����λ����ʼΪ-1��ʾ��С����
        pointB=-1;   //BС����λ����ʼΪ-1��ʾ��С����
        point=-1;
        fuA=0;
        fuB=0;
        fu=0;
        pwei=0;
        pmore=0; 
        pflag=0;  //����С����
        poper=0; 
}
      
void main(void)
{
    unsigned char j;

    IT0=1;  //���ж�
    EX0=1;
    IT1=1;
    EX1=1;
    EA=1;
    P1ASF = 0x10;       //P1.4��ADC
    ADC_CONTR = 0xE0;   //90T, ADC power on
    ADC_KeyState  = 0;
    ADC_KeyState1 = 0;
    ADC_KeyState2 = 0;
    ADC_KeyState3 = 0;  //��״̬
    ADC_KeyHoldCnt = 0; //�����¼�ʱ
    KeyCode = 0;    //���û�ʹ�õļ���, 1~16��Ч

    while(1)
    {
        num=KeyPro();
        CalculateAdcKey();//ɨ��ADC����

        if(num!=0xff || KeyCode>15){
            if(KeyCode==20){
                buffer=buffer+numA;
                KeyCode=0;
            }
            if(KeyCode==21){
                buffer=buffer-numA;
                KeyCode=0;
            }
            if(KeyCode==22){
                buffer=0;
                KeyCode=0;
            }
            if(KeyCode==23){
                numA=buffer;
                if(numA<0){
                    fuA=1;
                    numA=0-numA;
                }
                zheng=(long int) numA;
                xiao=numA-zheng;
                countA=0;
                pointA=-1;
                if(zheng==0){ //�����������
                    countA=1;
                    arr[0]=0;
                    pointA=countA;
                }
                else{
                    for(j=0;j<8;j++){
                        if(zheng!=0){
                            countA=countA+1;
                        }
                        arr[j]=zheng%10;
                        zheng=zheng/10;     
                    }
                    pointA=countA;
                }
                for(j=0;j<pointA;j++){
                    arrA[j]=arr[pointA-1-j];
                }
                if(xiao==0){//С���������
                     point=-1;
                }
                else{
                    for(j=pointA;j<7;j++){
                        if(xiao!=0){
                            countA=countA+1;
                        }
                        xiao=xiao*10;
                        xiaoa=(int) xiao;
                        xiao=xiao-xiaoa;
                        arrA[j]=xiaoa;
                    }
                    point=pointA;
                }

                for(j=0;j<countA;j++){ //�浽��ʾ��
                    Data[j]=duan[arrA[j]];
                }
                fu=fuA;
                count=countA;
                operator=0;//��֤�Ⱥ�֮����Լ�����������
                KeyCode=0;
            }   
            if(num<10){
                arrA[countA]=num;
                Data[countA]=duan[num];
                countA=countA+1;
                count=countA;
                if(pflag==0){   //�Ƿ񰴹�С����
                    pointA=countA;
                }        
            }  
            if(num==10){
                pointA=countA;
                pflag=1;
                point=pointA;
            }
            if(num>11 || (KeyCode<20) ){
                if(operator!=0){//���������ʱ�õ��ж�,Ҳ��ʵ�֡�=�������ֱ�ӼӼ�
                    operator=num;
                }
                if(operator==0 && countA==0){   //�������ж�һ��ʼ���븺��
                    fuA=1;
                    fu=1;      //������ʾ����
                    operator=0;
                }
                else{               
                    if(KeyCode<20){//�ж϶������������
                        operator=KeyCode;
                        KeyCode=0;
                    }
                    else{
                        operator=num;
                    }

                    for(j=0;j<countA;j++){
                        arrB[j]=arrA[j];
                        arrA[j]=0; //����
                    }
                    countB=countA;
                    pointB=pointA;
                    fuB=fuA;
                    countA=0;
                    pointA=-1;
                    fuA=0;      
                }
                pflag=0;
                poper=1;            
            }     
            if(num==11){
                numB=0;  //������Ļ��������������������
                numA=0; 
                            
                for(j=0;j<countB;j++){  //ת����float��
                    numB=numB*10+arrB[j];
                }
                pmore=countB-pointB;
                for(j=0;j<pmore;j++){
                    numB=numB/10;
                }
                pmore=0;
                if(fuB==1){    //�жϵ�һ����Ϊ����
                    numB=0-numB;
                }       
                for(j=0;j<countA;j++){
                    numA=numA*10+arrA[j];
                }
                pmore=countA-pointA;
                for(j=0;j<pmore;j++){
                    numA=numA/10;
                }
                pmore=0;            
                switch(operator){  //����
                    case 12:numA=numB+numA;break;
                    case 13:numA=numB-numA;break;
                    case 14:numA=numB*numA;break;
                    case 15:numA=numB/numA;break;    
                    case 16:{
                        if(numA==1){
                            numA=sin(numB*pi/180);break;//sin(x)
                        }
                        if(numA==2){
                            numA=sin(numB);break;//sin(x)
                        }
                    }
                    case 17:{
                        if(numA==1){
                            numA=cos(numB*pi/180);break;//sin(x)
                        }
                        if(numA==2){
                            numA=cos(numB);break;//sin(x)
                        }
                    }
                    case 18:numA=log(numA)/log(numB); break;//logx(y)
                    case 19:{
                        float temp=numA;
                        numA=numB;
                        for(j=0;j<(temp-1);j++){
                            numA=numA*numB;
                        }
                        break;//x^y
                    }
                          
                }
                                 
                if(numA<0){
                    fuA=1;
                    numA=0-numA;
                }
                zheng=(long int) numA;
                xiao=numA-zheng;
                countA=0;
                pointA=-1;
                if(zheng==0){ //�����������
                    countA=1;
                    arr[0]=0;
                    pointA=countA;
                }
                else{
                    for(j=0;j<8;j++){
                        if(zheng!=0){
                            countA=countA+1;
                        }
                        arr[j]=zheng%10;
                        zheng=zheng/10;     
                    }
                    pointA=countA;
                }
                for(j=0;j<pointA;j++){
                    arrA[j]=arr[pointA-1-j];
                }
                if(xiao==0){//С���������
                     point=-1;
                }
                else{
                    for(j=pointA;j<7;j++){
                        if(xiao!=0){ 
                            countA=countA+1;
                        }
                        xiao=xiao*10;
                        xiaoa=(int) xiao;
                        xiao=xiao-xiaoa;
                        arrA[j]=xiaoa;
                    }
                    point=pointA;
                }

                for(j=0;j<countA;j++){ //�浽��ʾ������
                    Data[j]=duan[arrA[j]];
                }
                fu=fuA;
                count=countA;
                operator=0;     //��֤�Ⱥ�֮����Լ�����������
            }
            pmore=0;
            
        }                
        //��ʾ
        if(fuA==1 || fu==1){    // ��ʾ����
            Send_595(wei[pwei],duan[12]);
            pwei=pwei+1;        
        }
        for(j=0;j<count;j++){   
            if(j+1==point){  //������ʾ˳�򣬾Ϳ��԰���.��ʱ������ʾ
                Send_595(wei[pwei],Data[j]);
                pwei=pwei+1;
                Send_595(wei[pwei],duan[11]);//��ʾС����                                
            }      
            else{
                Send_595(wei[pwei],Data[j]);
            }
            pwei=pwei+1;      
        }
        if(fuA==0 && countA!=0){// ���ų�����ʾ
             fu=0;
        }
        if(poper==1 && countA!=0){//�����ж�����countA!=0������������ʾС����
            point=-1;
            poper=0;
        }   
        pwei=0;     
    }
}

void Delay2x(unsigned char t)
{
    while(--t);
}

void Delayms(unsigned char t)
{
    while(t--)
    {
        Delay2x(245);
        Delay2x(245);
    }
}

void Send_595(unsigned char dat, unsigned char dat2)//�������ʾ
{
    unsigned char i;
    for(i=0; i<8; i++)
    {
        dat<<= 1;
        P_HC595_SER = CY;
        P_HC595_SRCLK = 1;
        P_HC595_SRCLK = 0;
    }
    for(i=0; i<8; i++)
    {
        dat2 <<= 1;
        P_HC595_SER = CY;
        P_HC595_SRCLK = 1;
        P_HC595_SRCLK = 0;
    }
    P_HC595_RCLK = 1;
    P_HC595_RCLK = 0;
}

unsigned char KeyScan(void)//����ɨ��
{
    unsigned char Val;
    KeyPort=0xf0;
    if(KeyPort!=0xf0)
    {
        Delayms(50);
        if(KeyPort!=0xf0)
        {
            KeyPort=0xfe;
            if(KeyPort!=0xfe)
            {
                Val=KeyPort&0xf0;
                Val+=0x0e;
                while(KeyPort!=0xfe);
                    Delayms(10);
                while(KeyPort!=0xfe);
                    return Val;
            }

            KeyPort=0xfd;
            if(KeyPort!=0xfd)
            {
                Val=KeyPort&0xf0;
                Val+=0x0d;
                while(KeyPort!=0xfd);
                    Delayms(10);
                while(KeyPort!=0xfd);
                    return Val;
            }

            KeyPort=0xfb;
            if(KeyPort!=0xfb)
            {
                Val=KeyPort&0xf0;
                Val+=0x0b;
                while(KeyPort!=0xfb);
                   Delayms(10);
                while(KeyPort!=0xfb);
                   return Val;
            }

            KeyPort=0xf7;
            if(KeyPort!=0xf7)
            {
                Val=KeyPort&0xf0;
                Val+=0x07;
                while(KeyPort!=0xf7);
                   Delayms(10);
                while(KeyPort!=0xf7);
                   return Val;
            }
        }
    }
    else return 0xff;
    return 0xff;
}

unsigned char KeyPro(void)
{
    switch(KeyScan())
    {   
        case 0xee:return 0;break;
        case 0xed:return 1;break;
        case 0xeb:return 2;break;
        case 0xe7:return 3;break;
        case 0xde:return 4;break;
        case 0xdd:return 5;break;
        case 0xdb:return 6;break;
        case 0xd7:return 7;break;
        case 0xbe:return 8;break;
        case 0xbd:return 9;break;
        case 0xbb:return 10;break; //.
        case 0xb7:return 11;break; //=
        case 0x7e:return 12;break; //+
        case 0x7d:return 13;break; //-
        case 0x7b:return 14;break; //*         
        case 0x77:return 15;break; ///

        default:return 0xff;break;
    }
}

unsigned int Get_ADC10bitResult(unsigned char channel)  //channel = 0~7 ��ȡADC�Ķ˿�ֵ
{
    Delayms(10);
    ADC_RES = 0;
    ADC_RESL = 0;

    ADC_CONTR = (ADC_CONTR & 0xe0) | 0x08 | channel;    //start the ADC
    _nop_();
    _nop_();
    _nop_();
    _nop_();

    while((ADC_CONTR & 0x10) == 0)  ;   //wait for ADC finish
    ADC_CONTR &= ~0x10;     //���ADC������־
    return  (((unsigned char)ADC_RES << 2) | (ADC_RESL & 3));
}

#define ADC_OFFSET  16
void CalculateAdcKey()  //ADC����ֵ���ж��ĸ�������
{
    unsigned char adc;
    unsigned int val;
    unsigned char   j;

    adc=Get_ADC10bitResult(4);
    if(adc>1023) return;//��������

    if(adc < (64-ADC_OFFSET))
    {
        ADC_KeyState = 0;   //��״̬��0
        ADC_KeyHoldCnt = 0;
    }
    j = 64;
    for(val=1; val<=16; val++)
    {
        if((adc >= (j - ADC_OFFSET)) && (adc <= (j + ADC_OFFSET)))  break;  //�ж��Ƿ���ƫ�Χ��
        j += 64;
    }
    ADC_KeyState3 = ADC_KeyState2;
    ADC_KeyState2 = ADC_KeyState1;
    if(val > 16)    ADC_KeyState1 = 0;  //����Ч
    else                        //����Ч
    {
        ADC_KeyState1 = val;
        if((ADC_KeyState3 == ADC_KeyState2) && (ADC_KeyState2 == ADC_KeyState1) &&
           (ADC_KeyState3 > 0) && (ADC_KeyState2 > 0) && (ADC_KeyState1 > 0))
        {
            if(ADC_KeyState == 0)   //��һ�μ�⵽
            {
                KeyCode  = val+15;  //�������
                ADC_KeyState = val; //�����״̬
                ADC_KeyHoldCnt = 0;
            }
            if(ADC_KeyState == val) //������⵽ͬһ������
            {
                if(++ADC_KeyHoldCnt >= 100) //����1���,��10��ÿ����ٶ�Repeat Key
                {
                    ADC_KeyHoldCnt = 90;
                    KeyCode  = val+15;  //�������
                }
            }
            else    ADC_KeyHoldCnt = 0; //����ʱ�������0
        }
    }
}