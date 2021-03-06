/*--------------------------------文*件*信*息-----------------------------
* 文件名：  Sensor.c
* 版　本：  v1.0
* 描  述：  100T双轴 整合后的版本
* 创建人：  
* 日  期：  
--------------------------------------------------------------------------
*当前版本:	v1.0
*修 改 人:
*日　  期:
----------------------------------修改履历--------------------------------
1、
2、
3、
4、
------------------------------------------------------------------------*/
#define __SENSOR_C__
#include "_Include.h"

//-------------define-----------------

static int8u    xdata  filtercnt; //滤波系数
//***********************************************************************/
// *功    能：数据初始化
// *入    口：p_sn,sensor结构体指针(X,Y);p_tempcal,temp_calib结构体指针(温补);
//            p_angcal,ang_calib结构体指针(角度标定)
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void Sn_DataInit(struct sensor *p_sn, struct temp_calib *p_tempcal, struct ang_calib *p_angcal)
{
        p_sn->val=0;     //原始值
        p_sn->ftval=0;   //滤波之后的值
        p_sn->ftval_f=0;
        p_sn->oftval=0;  //温补之后的值
        p_sn->ang=0;    //
        p_sn->tempcal=p_tempcal; //指向结构体temp_xcal /Temp_Ycal
        p_sn->angcal=p_angcal;    //指向ang_xcal Ang_Ycal
}

//***********************************************************************/
// *功    能：传感器值滤波
// *入    口：p_sn,sensor结构体指针(X,Y);val,新值;ftcnt，一阶滤波系数
// *出    口：
// *备    注：ADC滤波后的值
// *函数性质：
//***********************************************************************/
static void SnFilter(struct sensor *p_sn, int16u val, int8u ftcnt)
{
    if(ValInRange(p_sn->ftval,val,(int16u)FILTER_SUBB))    //限幅滤波
    {
        p_sn->ftval_f=Filter_1st(p_sn->ftval_f,val,ftcnt);     //一阶滤波
        
        p_sn->ftval=(int16u)(p_sn->ftval_f+0.5);
        
    }
    else
        p_sn->ftval  =val;    //变化幅度较大，直接写入新值
        p_sn->ftval_f=val;
}

//***********************************************************************/
// *功    能：获取温补的角度值
// *入    口：p_sn,sensor结构体指针(X,Y);temp,温度值
// *出    口：温补值
// *备    注：offset=k*temp+a;
// *函数性质：
//***********************************************************************/
static int16s GetOffset(struct sensor *p_sn, int16u temp)
{
    int8u i;
    float offset;

    for(i=0;i<(p_sn->tempcal->calcnt-2);i++)
    {
        if(temp<(p_sn->tempcal->ptempval[i+1]))    //查找温度在温补模块中温度数组中的位置
            break;
    }
    offset = p_sn->tempcal->prm_k[i]*temp+p_sn->tempcal->prm_a[i];    //获取相应的K,A值，并计算

    return (int16s)offset;
}

//***********************************************************************/
// *功    能：获取标定后的角度值
// *入    口：p_sn,sensor结构体指针(X,Y);val,角度的ADC值
// *出    口：角度正弦值
// *备    注：ang=k*ang_adc+a;
// *函数性质：
//***********************************************************************/
static float GetAngCalcVal(struct sensor *p_sn, int16u val)
{
    int8u i;
    float tmp;

    for(i=0;i<(p_sn->angcal->calcnt-2);i++)
    {
        if(val<(p_sn->angcal->adcarr[i+1]))    //查找当前角度ADC值在角度标定模块中ADC值数组中的位置
            break;
    }
    tmp = p_sn->angcal->prm_k[i]*val + p_sn->angcal->prm_a[i];    //获取相应的K,A值，并计算
    return tmp;
}

//***********************************************************************/
// *功    能：角度计算
// *入    口：p_sn,sensor结构体指针(X,Y);
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
static void Ang_Calc(struct sensor *p_sn)
{
    float ang;
    ang=GetAngCalcVal(p_sn,p_sn->oftval);       //角度标定后的角度正弦值
    if(ang>=1)
        p_sn->ang=90000;
    else if(ang<=-1)
        p_sn->ang=-90000;
    else
    {
        ang=asin(ang);
        ang=ang/PI*180000;
        p_sn->ang=ang;
    }
}

//***********************************************************************/
// *功    能：加速度传感器X数据初始化
// *备    注：temp_xcal，温补模块；ang_xcal，角度标定模块
//***********************************************************************/
void SnXDataInit(void)
{
    Sn_DataInit(&accel_x,&Temp_Xcal,&Ang_Xcal);
}
void SnYDataInit(void)
{
    Sn_DataInit(&accel_y,&Temp_Ycal,&Ang_Ycal);    //加速度传感器Y数据初始化
}
void SnXFilter(int16u val, int16u ftcnt)
{
    SnFilter(&accel_x,val,ftcnt);    //X滤波
}
void SnYFilter(int16u val, int16u ftcnt)
{
    SnFilter(&accel_y,val,ftcnt);    //y滤波
}
int16s GetXOffset(int16u temp)
{
    return(GetOffset(&accel_x,temp));    //X温补
}
int16s GetYOffset(int16u temp)
{
    return(GetOffset(&accel_y,temp));    //Y温补
}
void AngXCalc(void)
{
    Ang_Calc(&accel_x);    //X角度计算
}
void AngYCalc(void)
{
    Ang_Calc(&accel_y);    //Y角度计算
}
//***********************************************************************/
// *功    能：传感器结构体初始化
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void SnInit(void)
{
    CalibInit();            //校准模块初始化
    SetFilterCnt(4);        //
    accel_x.DataInit=SnXDataInit;
    accel_x.Filter=SnXFilter;
    accel_x.GetTempOffset=GetXOffset;
    accel_x.AngCalc=AngXCalc;
    accel_x.DataInit();

    accel_y.DataInit=SnYDataInit;
    accel_y.Filter=SnYFilter;
    accel_y.GetTempOffset=GetYOffset;
    accel_y.AngCalc=AngYCalc;
    accel_y.DataInit();
}

//***********************************************************************/
// *功    能：获取角度的AD值（窗口滤波后），进行一阶滤波和温度补偿 ，算出角度值
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void SnProc(void)
{
    {
    if(AdcEndTag==0)
        return;
    AdcEndTag=0;

    switch(AdcEndId)
    {
        case 0:
             accel_x.val=Adc_GetXVal();                                             //读取X轴的角度AD值 （窗口滤波后）
             accel_x.Filter((accel_x.val),filtercnt);                               //一阶滤波
             accel_x.oftval=(int16u)((int16s)accel_x.ftval+
                                            accel_x.GetTempOffset(Get_McuTemp()));  //温度补尝
            accel_x.AngCalc();                                                      //计算角度（温补后）
                break;

        case 1:
            accel_y.val=Adc_GetYVal();
            accel_y.Filter((accel_y.val),filtercnt);                                //一阶滤波
            accel_y.oftval=(int16u)((int16s)accel_y.ftval+
                                    accel_y.GetTempOffset(Get_McuTemp()));          //温度补尝
            accel_y.AngCalc();
        default:
            break;
    }
    ADC0CN |= 0x10;
 }
}

//***********************************************************************/
// *功    能：获取一阶滤波系数
// *入    口：
// *出    口：一阶滤波系数
// *备    注：
// *函数性质：
//***********************************************************************/
int8u GetFilterCnt(void)
{
    return filtercnt;
}
//***********************************************************************/
// *功    能：设置一阶滤波系数
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
void SetFilterCnt(int8u cnt)
{
    filtercnt=cnt;
}

//***********************************************************************/
// *功    能：获取当前温度
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
int16u Get_McuTemp(void)
{
//  if(TEMP_S == 0)
//      return(SensorTemp);     //传感器温度
//  else
        return(Adc_GetTemp());      //MCU所测的温度值
}

//***********************************************************************/
// *功    能：获取X轴一阶滤波后的值
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
int16u GetXFtVal(void)
{
    return (accel_x.ftval);
}

//***********************************************************************/
// *功    能：获取Y轴一阶滤波后的值
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
int16u GetYFtVal(void)
{
    return (accel_y.ftval);
}

//***********************************************************************/
// *功    能：获取X轴温补后的值
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
int16s GetXOftVal(void)
{
    return (accel_x.oftval);
}

//***********************************************************************/
// *功    能：获取Y轴温补后的值
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
int16s GetYOftVal(void)
{
    return (accel_y.oftval);
}

//***********************************************************************/
// *功    能：获取X轴角度值
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
int32s GetXAng(void)
{
    return (accel_x.ang);
}

//***********************************************************************/
// *功    能：获取Y轴角度值
// *入    口：
// *出    口：
// *备    注：
// *函数性质：
//***********************************************************************/
int32s GetYAng(void)
{
    return (accel_y.ang);
}

int32s Get_McuTempInCelsius(void)
{
//  if(TEMP_S == 0)
//      return(SensorTemp);     //传感器温度
//  else
	
       // return(Adc_GetTemp()); //MCU所测的温度值
	
	return((((float)Adc_GetTemp()- (float)McuTAdcValWhileSet)*0.305/13.2+ SetMcuTincelsius)*1000);
}
