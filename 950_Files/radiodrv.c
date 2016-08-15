//**************************************************************************
// Copyright (c) 2007, Cheertek Inc . All rights reserved.
// J000, all right reserved.
// Product :  Firmware
//
// Date : 2007.02.26
// Author : Cheertek (J000 Winnie)
// Purpose : Provide all Raiod tuner driver
// Sources : Radiodrv.h/ Radiodrv.c
//***************************************************************************
// Modification use :
//**************************************************************************
// Update
// Date : 2007.02.26
// Name : Cheertek (J000 Winnie)
// Description :
//
// Item Source
// ---------------------------------------- -------------------
//**************************************************************************
#include "winav.h"
#include "chips.h"
#include "hio.h"
#include "RADIODRV.h"
#ifdef SUPPORT_RADIO_TUNER
//*********************************************************************************************************
//  @summary    This is use delay time for radio function
//  @param  _bCount be used to delay time
//  @return None
//  @retval None
//  @description
//      This function be used to delay some time for radio driver function
//  @bug    None
//  @history    
//*********************************************************************************************************
void _Delay_RADIODRV(BYTE _bCount)
{   
   for(;_bCount > 0; _bCount--)
   {
   }
}

//*********************************************************************************************************
//  @summary    Control the IIC SCL as High
//  @param  None
//  @return None
//  @retval None
//  @description
//      This function be used to control SCL pin of IIC as High state
//  @bug    None
//  @history    
//*********************************************************************************************************
void _RADIODRV_IIC_SCLHIGH(void)
{
     HAL_WriteGPIO(GRP_RADIOSCK, PIN_RADIOSCK, 1); 
      _Delay_RADIODRV(40);
}
//*********************************************************************************************************
//  @summary    Control the IIC SCL as Low
//  @param  None
//  @return None
//  @retval None
//  @description
//      This function be used to control SCL pin of IIC as low state
//  @bug    None
//  @history    
//*********************************************************************************************************
void _RADIODRV_IIC_SCLLOW(void)
{
     HAL_WriteGPIO(GRP_RADIOSCK, PIN_RADIOSCK, 0); 
     _Delay_RADIODRV(100);
}
//*********************************************************************************************************
//  @summary    Control the IIC SDA as HIGH
//  @param  None
//  @return None
//  @retval None
//  @description
//      This function be used to control SCL pin of IIC as HIGH state
//  @bug    None
//  @history    
//*********************************************************************************************************
void _RADIODRV_IIC_SDAHIGH(void)
{
    
    HAL_WriteGPIO(GRP_RADIOSDA, PIN_RADIOSDA, 1); 
    _Delay_RADIODRV(40);
}
//*********************************************************************************************************
//  @summary    Control the IIC SDA as LOW
//  @param  None
//  @return None
//  @retval None
//  @description
//      This function be used to control SCL pin of IIC as LOW state
//  @bug    None
//  @history    
//*********************************************************************************************************
void _RADIODRV_IIC_SDALOW(void)
{   
    HAL_WriteGPIO(GRP_RADIOSDA, PIN_RADIOSDA, 0); 
     _Delay_RADIODRV(40);
}
//*********************************************************************************************************
//  @summary    Send the start signal of IIC at SCL/SDA pin
//  @param  None
//  @return None
//  @retval None
//  @description
//      This function be used to send start signal at SCL/SDA pin 
//  @bug    None
//  @history    
//*********************************************************************************************************
void _RADIODRV_IIC_START(void)
{
    _RADIODRV_IIC_SCLHIGH();
    _RADIODRV_IIC_SDAHIGH();   
    _RADIODRV_IIC_SDALOW();
    _RADIODRV_IIC_SCLLOW();    
}
//*********************************************************************************************************
//  @summary    Send the stop signal of IIC at SCL/SDA pin
//  @param  None
//  @return None
//  @retval None
//  @description
//      This function be used to send stop signal at SCL/SDA pin 
//  @bug    None
//  @history    
//*********************************************************************************************************
void _RADIODRV_IIC_STOP(void)
{
    _RADIODRV_IIC_SCLLOW();
    _RADIODRV_IIC_SDALOW();
    _RADIODRV_IIC_SCLHIGH();
    _RADIODRV_IIC_SDAHIGH();    
}
//*********************************************************************************************************
//  @summary    Get the ACK signal from other device via SCL/SDA
//  @param  None
//  @return Make sure the ack signal
//  @retval TRUE    get the ack signal successfully
//  @retval FALSE   get the ack signal fail
//  @description
//      This function be used to get the ack signal from other device via SCL/SDA 
//  @bug    None
//  @history    
//*********************************************************************************************************
BOOL _RADIODRV_IIC_GetACK(void)
{
    BYTE _bRADIODRVData;   
    _Delay_RADIODRV(10);
    HAL_ReadGPIO(GRP_RADIOSDA,PIN_RADIOSDA); //setting input mode  
    _Delay_RADIODRV(10);
    _RADIODRV_IIC_SCLHIGH();     
    _bRADIODRVData = HAL_ReadGPIO(GRP_RADIOSDA, PIN_RADIOSDA); //ack as low
    _RADIODRV_IIC_SCLLOW(); 
    if(_bRADIODRVData==0) //ack    
        return RADIODRV_IIC_ACK;        
    return RADIODRV_IIC_NONACK;   
}
//*********************************************************************************************************
//  @summary    To set the ACK signal to other device via SCL/SDA
//  @param  bACK    Send ack signal. if bACK as TRUE, the low signal of sda pin. otherwise, the high signal of sda pin
//  @return None
//  @retval None
//  @description
//      This function be used to set the ack signal to other device via SCL/SDA 
//  @bug    None
//  @history    
//*********************************************************************************************************
void _RADIODRV_IIC_SETACK(BOOL bACK)  
{        
    if(bACK)      
        _RADIODRV_IIC_SDALOW();      
    else          
        _RADIODRV_IIC_SDAHIGH(); //set sda as non-ack before stop  
     
        _RADIODRV_IIC_SCLHIGH();
        _RADIODRV_IIC_SCLLOW();   
}
//*********************************************************************************************************
//  @summary    Write byte data to radio device via SCL/SDA pin
//  @param  bData   Desired write byte
//  @return Make sure the write byte status
//  @retval TRUE    Write byte successfully
//  @retval FALSE   Write byte fail
//  @description
//      This function be used to write registers of radio tuner to control radio device by IIC interface 
//  @bug    None
//  @history    
//*********************************************************************************************************
BOOL _RADIODRV_WriteBYTE(BYTE bData)
{
     BYTE bIndex;
     for(bIndex = 0;bIndex < 8; bIndex++)
    {           
	if( bData & 0x80)
	   _RADIODRV_IIC_SDAHIGH();   
	else
	   _RADIODRV_IIC_SDALOW(); 
       _RADIODRV_IIC_SCLHIGH();	 
       bData  = bData<<1 ;
       _RADIODRV_IIC_SCLLOW();	   
         
    }
     return _RADIODRV_IIC_GetACK();
}
//*********************************************************************************************************
//  @summary    Read byte data to radio device via SCL/SDA pin
//  @param  bData   Desired read byte
//  @return Make sure the read byte status
//  @retval TRUE    read byte successfully
//  @retval FALSE   read byte fail
//  @description
//      This function be used to read registers of radio tuner to control radio device by IIC interface 
//  @bug    None
//  @history    
//*********************************************************************************************************
BYTE _RADIODRV_ReadBYTE(BYTE bACK)
{
    BYTE bData;
    BYTE bIndex;
    bData=0x00;
    for ( bIndex = 0; bIndex< 8; bIndex++)
    {	   
        bData =bData<< 1;
	    if ( HAL_ReadGPIO(GRP_RADIOSDA, PIN_RADIOSDA))
	    bData |= 0x01;        
        _RADIODRV_IIC_SCLHIGH();
        _RADIODRV_IIC_SCLLOW(); 
    }
    _RADIODRV_IIC_SETACK(bACK) ; 
    return bData;
}
//*********************************************************************************************************
//  @summary    Write register of si4703 radio tuner which size of word to control si4703 radio tuner
//  @param  bLastReg   desired writes last register address
//  @param  wRegArray  desired writes register data
//  @return Make sure that write registers status
//  @retval TRUE    write Si4703 radio tuner successfully
//  @retval FALSE   write Si4703 radio tuner fail
//  @description
//      This function be used to write registers of radio tuner which size of word to control si4703 radio tuner 
//  @bug    None
//  @history    
//*********************************************************************************************************
BYTE _RADIODRV_WriteReg(BYTE bLastReg,WORD* wRegArray)    
{
     //RADIODRV tuner start write reg from 0x02     
    BYTE bRADIODRV_Reg=0x02;     
   _RADIODRV_IIC_START();
   if(_RADIODRV_WriteBYTE(RADIODRV_ADDRESS)==FALSE)
    {
        goto ERROR_W;
    }         
    //write the word which pairs of byte
    do
    {
        _RADIODRV_WriteBYTE(wRegArray[bRADIODRV_Reg]>>8); //upper-byte
        _RADIODRV_WriteBYTE(wRegArray[bRADIODRV_Reg]&0xff);//lower-byte         
        bRADIODRV_Reg = (bRADIODRV_Reg + 1) & 0x0f;
    }while(bRADIODRV_Reg!=(bLastReg+1)&0x0f);
    _RADIODRV_IIC_STOP(); //stop will reset internal counter of si4703  
    return RADIODRV_NOERR;
ERROR_W:   
    _RADIODRV_IIC_STOP();
    printf("\n@ err_w");
    return RADIODRV_ERR;        

}
//*********************************************************************************************************
//  @summary    Read register of si4703 radio tuner which size of word to control si4703 radio tuner
//  @param  bLastReg   desired read last register address
//  @param  wRegArray  desired read register data
//  @return Make sure that read registers status
//  @retval TRUE    read Si4703 radio tuner successfully
//  @retval FALSE   read Si4703 radio tuner fail
//  @description
//      This function be used to read registers of radio tuner which size of word to control si4703 radio tuner 
//  @bug    None
//  @history    
//*********************************************************************************************************
BYTE _RADIODRV_ReadReg(BYTE bLastReg, WORD* wRegArray)
{
    BYTE bRADIODRV_Temp;
    WORD wRADIODRV_Temp;  
    //start read register from 0x0a
    BYTE bRADIODRV_Reg=0x0A; 
    _RADIODRV_IIC_START();
    if(_RADIODRV_WriteBYTE(RADIODRV_ADDRESS|0x01)==FALSE)  
    {
        goto ERROR_R;
    }      
    
     do
    {
        wRADIODRV_Temp=_RADIODRV_ReadBYTE(1);//high-byte
        bRADIODRV_Temp=_RADIODRV_ReadBYTE(bLastReg-bRADIODRV_Reg);//lower-byte
        wRegArray[bRADIODRV_Reg]=wRADIODRV_Temp*256+bRADIODRV_Temp; //get data of reg.        
        bRADIODRV_Reg = (bRADIODRV_Reg + 1)&0x0f;        
    }while(bRADIODRV_Reg!=((bLastReg+1)&0x0f));
    _RADIODRV_IIC_STOP();    //stop will reset internal counter of si4703
    return RADIODRV_NOERR;
ERROR_R:
    _RADIODRV_IIC_STOP(); 
    printf("\n @@ err_r");
   return RADIODRV_ERR;
}


//*********************************************************************************************************
//  @summary    Reset the radio tuner
//  @param  None
//  @return None
//  @retval None
//  @description
//      This function be used to reset the radio tuner for si4703 
//  @bug    None
//  @history    
//*********************************************************************************************************
void RADIODRV_Reset(void)
{
   // printf("\n@@ Reset!!") ;
    //first time needs to select 2-wire mode when reset pin rising edge  
    HAL_WriteGPIO(GRP_RADIOGPIO3, PIN_RADIOGPIO3, 1);
    HAL_WriteGPIO(GRP_RADIOSDA, PIN_RADIOSDA, 1);
    HAL_WriteGPIO(GRP_RADIOSCK, PIN_RADIOSCK, 1); //sck as 1
    HAL_WriteGPIO(GRP_RADIOSEN, PIN_RADIOSEN, 1); //sen as 1
    HAL_WriteGPIO(GRP_RADIOGPIO3, PIN_RADIOGPIO3, 0); //gpio3 as 1
    HAL_WriteGPIO(GRP_RADIORST, PIN_RADIORST, 0);
    Delay_mS(1); 
    HAL_WriteGPIO(GRP_RADIOSDA, PIN_RADIOSDA, 0);//sda as 0    
    //reset pin is active high, so needs to control reset pin
    Delay_mS(1);    
    HAL_WriteGPIO(GRP_RADIORST, PIN_RADIORST, 1);
    Delay_mS(1); 
    //return the default condition
    HAL_WriteGPIO(GRP_RADIOSDA, PIN_RADIOSDA, 1);
    HAL_WriteGPIO(GRP_RADIOGPIO3, PIN_RADIOGPIO3, 1); //gpio3 as 1
    HAL_WriteGPIO(GRP_RADIOGPIO3, PIN_RADIOGPIO2, 1); //gpio3 as 1
    HAL_WriteGPIO(GRP_RADIOSEN, PIN_RADIOSEN, 1); //sen as 1       
    HAL_WriteGPIO(GRP_RADIOSCK, PIN_RADIOSCK, 1); //sck as 1
    Delay_mS(50);
  //  printf("reset exit");
}
//*********************************************************************************************************
//  @summary    This function be used to power up radio tuner of si4703
//  @param  None
//  @return None
//  @retval None
//  @description
//      This function be used to power-up for radio tuner of si4703 
//  @bug    None
//  @history    
//*********************************************************************************************************
void RADIODRV_PowerUp(void)
{
    //printf("\n @@ power up!!");
    //recover volume setting
    bRADIODRVTmp=RADIODRV_GetVol();       
    bRADIODRVTmp&=0x0F; //radio volume max step as 0x0F
    //power up need set 0x02
    wRADIODRVTunerReg[2]=0x4801; //dis-able mute and powe up si4703
    wRADIODRVTunerReg[3]=0x0000;
    wRADIODRVTunerReg[4]=0x4004;
    wRADIODRVTunerReg[5]=0x1910|bRADIODRVTmp;    
    wRADIODRVTunerReg[6]=0x0200;
    _RADIODRV_WriteReg(0x06,wRADIODRVTunerReg);
     Delay_mS(85); //si4703 needs power up time
    //-------------------------------------  
  
}

//*********************************************************************************************************
//  @summary    This function be used to power down radio tuner of si4703
//  @param  None
//  @return None
//  @retval None
//  @description
//      This function be used to power-down for radio tuner of si4703 
//  @bug    None
//  @history    
//*********************************************************************************************************
void RADIODRV_PowerDown(void)
{
    wRADIODRVTunerReg[2]=0x0041; //enable mute and power down mode and enable mute
    _RADIODRV_WriteReg(0x02,wRADIODRVTunerReg);
    HAL_WriteGPIO(GRP_RADIOSDA, PIN_RADIOSDA, 1);
    HAL_WriteGPIO(GRP_RADIOGPIO3, PIN_RADIOGPIO3, 1); //gpio3 as 1
    HAL_WriteGPIO(GRP_RADIOGPIO3, PIN_RADIOGPIO2, 1); //gpio3 as 1
    HAL_WriteGPIO(GRP_RADIOSEN, PIN_RADIOSEN, 1); //sen as 1       
    HAL_WriteGPIO(GRP_RADIOSCK, PIN_RADIOSCK, 1); //sck as 1     
}
//*********************************************************************************************************
//  @summary    This function be used to convert frequency to channel
//  @param  wFreq   Desired frequency which needs to convert to channel
//  @return number of channel 
//  @retval number of channel which user desires to convert frequency
//  @description
//      This function be used to convert frequency to channel 
//  @bug    None
//  @history    
//*********************************************************************************************************
WORD _RADIODRV_FreqToChan(WORD wFreq)
{
    WORD wChannelSpace;
    WORD wBottomBand;
    WORD wChannel;	
//step 1: Setting BottmBand from band select Reg[0x05].bit[7:6]
    if((wRADIODRVTunerReg[0x05]&0x00C0)==0x0000)
        wBottomBand=8750;//87.5Mhz
    else
        wBottomBand=7600;//76Mhz

    if(wFreq<wBottomBand)
        wFreq=wBottomBand;
//step2:Setting Channel space from channel space

    if((wRADIODRVTunerReg[0x05]&0x0030)==0x0000)
    {
        wChannelSpace=20;//200khz
        
    }
    else if((wRADIODRVTunerReg[0x05]&0x0030)==0x0010)
    {
        wChannelSpace=10;//100Khz
      
    }
    else
    {
       wChannelSpace=5;//50Khz 
      
    }
//step3 :get channel value
       wChannel=(wFreq-wBottomBand)/wChannelSpace;
//only for debug

//printf("\n @wFreq=%x",wFreq);
return (wChannel);	
}

//*********************************************************************************************************
//  @summary    This function be used to convert channel to frequency
//  @param  wChan   Desired channel which needs to convert to frequency
//  @return value of frequency 
//  @retval value of frequency which user desires to convert channel
//  @description
//      This function be used to convert channel to frequency 
//  @bug    None
//  @history    
//*********************************************************************************************************
WORD _RADIODRV_ChanToFreq(WORD wChan)
{
    WORD wChannelSpace;
    WORD wBottomBand;
    WORD wFreq;	

//step 1: Setting BottmBand from band select Reg[0x05].bit[7:6]
    if((wRADIODRVTunerReg[0x05]&0x00C0)==0x0000)
        wBottomBand=8750;//87.5Mhz
    else
        wBottomBand=7600;//76Mhz
 
//step2:Setting Channel space from channel space
    if((wRADIODRVTunerReg[0x05]&0x0030)==0x0000)
        wChannelSpace=20;//200khz
    else if((wRADIODRVTunerReg[0x05]&0x0030)==0x0010)
        wChannelSpace=10;//100Khz
    else
       wChannelSpace=5;//50Khz 
//step3: get the frequency       
       wFreq=wBottomBand+wChannelSpace*wChan;   

	return (wFreq);
}
//*********************************************************************************************************
//  @summary    This function be used to tune radio station
//  @param  wRADIODRVfreq   desired tune frequency
//  @return status which be tuned frequency 
//  @retval TRUE    Tune frequency is sucessful
//  @retval FALSE   Tune frequency is un-sucessful
//  @description
//      This function be used to tune frequency 
//  @bug    None
//  @history    
//*********************************************************************************************************
BYTE RADIODRV_tune(WORD wRADIODRVfreq) 
{
    WORD wRADIODRVTemp;
    
    //[step 0 ]: check the boundary value for band select
    if(RADIODRV_GetBand()==RADIODRV_BS_USA)
    {  
        if(wRADIODRVfreq>=10800) wRADIODRVTemp=10800;
        if(wRADIODRVfreq<=8750)wRADIODRVTemp=8750;
     }// japan wide band
    else if(RADIODRV_GetBand()==RADIODRV_BS_JapWide)
    {
        if(wRADIODRVfreq>=10800) wRADIODRVTemp=10800;
        if(wRADIODRVfreq<=7600)wRADIODRVTemp=7600;
    }
    else // japan
    {
        if(wRADIODRVfreq>=9000) wRADIODRVTemp=9000;
        if(wRADIODRVfreq<=7600)wRADIODRVTemp=7600;
    }
    
    //[step 1 ]: get channel number from current frequency and enable tune bit      
       wRADIODRVTunerReg[3]=_RADIODRV_FreqToChan(wRADIODRVTemp)|0x8000;
    
    _RADIODRV_WriteReg(0x03,wRADIODRVTunerReg);
    //[step2]: wait the tune complete bit set  
     //added time-out method to avoid radio system hang
    dwRadioDrvTime=OS_GetSysTimer();  
    do{
		_RADIODRV_ReadReg(0x0A, wRADIODRVTunerReg);              
            if((OS_GetSysTimer()-dwRadioDrvTime)>COUNT_2_SEC)
            {
                //printf("\n @@ Tuner_F1");
                return FALSE;
            }
    } while ((wRADIODRVTunerReg[10] & 0x4000) == 0x00); 		  
    //[step3]: clean tune bit at register 0x03
    wRADIODRVTunerReg[ 3] &= ~0x8000;
    _RADIODRV_WriteReg(0x03,wRADIODRVTunerReg);
    //[step4]: wait STC bit be cleared
     //added time-out method to avoid radio system hang
    dwRadioDrvTime=OS_GetSysTimer();  
    do {
		_RADIODRV_ReadReg(0x0A, wRADIODRVTunerReg);
          if((OS_GetSysTimer()-dwRadioDrvTime)>COUNT_2_SEC)
            {
              //  printf("\n @@ Tuner_F2");
                return FALSE;
            }
    } while ((wRADIODRVTunerReg[10] & 0x4000) != 0x00); 		  
    //read the channel and store channel number	
    _RADIODRV_ReadReg(0x0B, wRADIODRVTunerReg);
    return TRUE;
   // printf("\n @ tune end");
}


//*********************************************************************************************************
//  @summary    This function be used to control volume of radio tuner 
//  @param  bVol   Desire to setting volume steps
//  @return None
//  @retval None
//  @description
//      This function be used to set volume of radio tuner 
//  @bug    None
//  @history    
//*********************************************************************************************************
void RADIODRV_SetVol (BYTE bVol)
{ 
    wRADIODRVTunerReg[5] &= (~0x000F);
	wRADIODRVTunerReg[5] |= bVol & 0x000F;	
	_RADIODRV_WriteReg(0x05,wRADIODRVTunerReg);    
   // printf("@@se volume %x",wRADIODRVTunerReg[5] );         
}

//*********************************************************************************************************
//  @summary    This function be used to set mute for volume
//  @param  bmute   bmute as TRUE which disable mute,otherwise enalbe mute
//  @return None
//  @retval None
//  @description
//      This function be used to mute volume of radio  
//  @bug    None
//  @history    
//*********************************************************************************************************
void RADIODRV_SetMute (BYTE bMute)
{    
    //Si4703 mute control must use hardware mute
    if (bMute)
    {
	wRADIODRVTunerReg[2] |= 0x4000; //disable mute	
    }
   else
   {
	wRADIODRVTunerReg[2]&= (~0x4000); //enable mute	
   }      	
	_RADIODRV_WriteReg(0x02,wRADIODRVTunerReg);       
  //  printf("@@volume %x",wRADIODRVTunerReg[2] );
}
//*********************************************************************************************************
//  @summary    This function be used to initiate radio tuner 
//  @param  None
//  @return Initial status of radio tuner
//  @retval None
//  @description
//      This function be used to initiate radio tuner
//  @bug    None
//  @history    
//*********************************************************************************************************
BYTE RADIODRV_Init(void)
{      
    //BYTE bIndex;  
   
    // printf("@@\n RADIODRV tuner int!!"); 
    //[step0]: reset Radio ic
     RADIODRV_Reset();
    //[step1]: initial array value
      _RADIODRV_ReadReg(0x09, wRADIODRVTunerReg);  //read register from 0x0A~0x0F, 0x00~0x09
    //  for(bIndex=0;bIndex<16;bIndex++)
    //  printf("\n @1_%hx=%x",bIndex,wRADIODRVTunerReg[bIndex]);
     
     //[step2]: initial default register value and power up        
    //wRADIODRVTunerReg[2]=0x4001; //dis-able mute and powe up si4703
    wRADIODRVTunerReg[2]=0x4801; //dis-able mute and powe up si4703, enable Verbose mode
    wRADIODRVTunerReg[3]=0x0000;
    #ifdef SUPPORT_RADIO_RDS
    wRADIODRVTunerReg[4]=0xD004;
    #else
    wRADIODRVTunerReg[4]=0x4004;
    #endif
    //wRADIODRVTunerReg[5]=0x1910;  
    wRADIODRVTunerReg[5]=0x0C10;  
  // wRADIODRVTunerReg[6]|=0x0048;  
    wRADIODRVTunerReg[6]=0x0200;
    _RADIODRV_WriteReg(0x06,wRADIODRVTunerReg);
    Delay_mS(85);
    
    _RADIODRV_ReadReg(0x9, wRADIODRVTunerReg);  //read register from 0x0A~0x0F, 0x00~0x09  
  //   for(bIndex=0;bIndex<16;bIndex++)
   //  printf("\n @%hx=%x",bIndex,wRADIODRVTunerReg[bIndex]);
     //---------------------------------------------------- 
     //[Step3]: read ID to check Si47xx has been active
     return _RADIODRV_GetChipID();     
}

//*********************************************************************************************************
//  @summary    This function be used to get chip id of si4703
//  @param  None
//  @return Chip ID for si4703
//  @retval 0x04    SI4700
//  @retval 0x01    SI4701
//  @retval 0x02    SI4702
//  @retval 0x03    SI4703
//  @description
//      This function be used to get chip-id of the si-4703
//  @bug    None
//  @history    
//*********************************************************************************************************
BYTE _RADIODRV_GetChipID(void)
{
   //get current chip ID for si4703
    _RADIODRV_ReadReg(0x01, wRADIODRVTunerReg);   
     if((wRADIODRVTunerReg[1] & 0x03C0)==0x0000)
        return RADIODRV_SI4700;
     else if  ((wRADIODRVTunerReg[1] & 0x03C0)==0x0040)
        return RADIODRV_SI4702;
     else if ((wRADIODRVTunerReg[1] & 0x03C0)==0x0200)
        return RADIODRV_SI4701;
      else if ((wRADIODRVTunerReg[1] & 0x03C0)==0x0240)
        return RADIODRV_SI4703;
      else
        return RADIODRV_SIFAIL;
}
//*********************************************************************************************************
//  @summary    This function be used to get frequency
//  @param  None
//  @return Frequency of current station
//  @retval frequency value
//  @description
//      This function be used to get frequency of current station
//  @bug    None
//  @history    
//*********************************************************************************************************
WORD RADIODRV_GetFreq(void)
{
   //get current tuned channel
  // printf("\n @ Get Freq :%x:",(_RADIODRV_ChanToFreq(wRADIODRVTunerReg[0x0B]&0x3FF)));
    return (_RADIODRV_ChanToFreq(wRADIODRVTunerReg[0x0B]&0x3FF)) ;      
}
//*********************************************************************************************************
//  @summary    This function be used to get RSSI value
//  @param  None
//  @return Get RSSI of current station
//  @retval RSSI value
//  @description
//      This function be used to get RSSI(Received Signal Strength Indicator) value of current station
//  @bug    None
//  @history    
//*********************************************************************************************************
BYTE RADIODRV_GetRSSI(void)
{
  _RADIODRV_ReadReg(0x0A, wRADIODRVTunerReg);   
  printf("\n @ Get RSSI :%x:",wRADIODRVTunerReg[0x0A]);	
  return ((BYTE)wRADIODRVTunerReg[0x0A]&0xFF);;
}
//*********************************************************************************************************
//  @summary    This function be used to get volume value
//  @param  None
//  @return Get volume of current setting value 
//  @retval Volume value
//  @description
//      This function be used to get volume of current setting value
//  @bug    None
//  @history    
//*********************************************************************************************************
BYTE RADIODRV_GetVol(void)
{
    _RADIODRV_ReadReg(0x05, wRADIODRVTunerReg);   
    printf("\n @@ Vol = %x",((BYTE)(wRADIODRVTunerReg[5] & 0x000F)));
    return ((BYTE)(wRADIODRVTunerReg[5] & 0x000F));   
}
//*********************************************************************************************************
//  @summary    This function be used to get mute status of volume
//  @param  None
//  @return Get mute status of volume
//  @retval TRUE    Enalbe Mute function
//  @retval FALSE   DISABLE Mute function
//  @description
//      This function be used to get mute status of volume
//  @bug    None
//  @history    
//*********************************************************************************************************
BYTE RADIODRV_GetMute(void)
{
    _RADIODRV_ReadReg(0x02, wRADIODRVTunerReg);      
    if((wRADIODRVTunerReg[2] & 0x4000))
    {
       // printf("\n @@ dis mute %lx",wRADIODRVTunerReg[2]);
        return FALSE; //disable mute
    }
     else 
     {
        //printf("\n @@ mute %lx",wRADIODRVTunerReg[2]);
        return TRUE; //enable mute  
     }
}
//*********************************************************************************************************
//  @summary    This function be used to get band value
//  @param  None
//  @return Get band value which current setting radio system
//  @retval 0x00    Band range from 87.5~108Mhz
//  @retval 0x01    Band range from 76~108Mhz
//  @retval 0x02    Band range from 76~90Mhz
//  @description
//      This function be used to get band value for current radio system
//  @bug    None
//  @history    
//*********************************************************************************************************
BYTE RADIODRV_GetBand(void)
{
    _RADIODRV_ReadReg(0x05, wRADIODRVTunerReg);      
    if((wRADIODRVTunerReg[5] & 0x00C0)==0x0000)
        return RADIODRV_BS_USA;       //87.5  -108
    else if ((wRADIODRVTunerReg[5] & 0x00C0)==0x0040)
        return RADIODRV_BS_JapWide; //76-108 
    else
        return RADIODRV_BS_Jap;    //76-90
}
//*********************************************************************************************************
//  @summary    This function be used to get channel space
//  @param  None
//  @return Get current space for current radio system
//  @retval 0x00    space for 200khz
//  @retval 0x01    space for 100khz
//  @retval 0x02    space for 50khz
//  @description
//      This function be used to get current channle space setting
//  @bug    None
//  @history    
//*********************************************************************************************************
BYTE RADIODRV_GetChanSpace(void)
{
    _RADIODRV_ReadReg(0x05, wRADIODRVTunerReg);      
    if((wRADIODRVTunerReg[5] & 0x0030)==0x0000)
        return RADIODRV_CS_USA;        //200k hz
    else if ((wRADIODRVTunerReg[5] & 0x0030)==0x0010)
        return RADIODRV_CS_JAP;  // 100 khz
    else
        return RADIODRV_CS_Non; //   50khz
}
//*********************************************************************************************************
//  @summary    This function be used to set band
//  @param  bRADIODRVband   Desire to set band value. 0x00 for 87.5~108Mhz, 0x01 for 76~108Mhz and 0x02 for 76~90Mhz
//  @return None
//  @retval None
//  @description
//      This function be used to set band
//  @bug    None
//  @history    
//*********************************************************************************************************
void RADIODRV_SETBand(BYTE bRADIODRVband) 
{       
    wRADIODRVTunerReg[5]|=(bRADIODRVband<<6)&0x00C0;  //setting band
  //  printf("\n @Band_1: %x",wRADIODRVTunerReg[5]);
    _RADIODRV_WriteReg(0x05,wRADIODRVTunerReg);   
}
//*********************************************************************************************************
//  @summary    This function be used to set channel space
//  @param  bChanSpace   Desire to set channel space. 0x00 for 200KHZ, 0x01 for 100KHZ and 0x02 for 50KHZ
//  @return None
//  @retval None
//  @description
//      This function be used to set channel space
//  @bug    None
//  @history    
//*********************************************************************************************************
void RADIODRV_SETChanSpace(BYTE bChanSpace)
{
   
    wRADIODRVTunerReg[5]|=(bChanSpace<<4)&0x0030; 
  //  printf("\n @space_1:%x",wRADIODRVTunerReg[5]);
    _RADIODRV_WriteReg(0x05,wRADIODRVTunerReg);  
}

//*********************************************************************************************************
//  @summary    This function be used to seek station for radio tuner control
//  @param  _bSeekDir   if _bSeekDir as TRUN, the seek up direction, else _bSeekDir as FALSE, the seek down direction
//  @return Get the seek status
//  @retval TRUE    Seek station successfully
//  @retval FALSE   Seek station fail
//  @description
//      This function be used to seek station for radio tuner control
//  @bug    None
//  @history    
//*********************************************************************************************************
BYTE _RADIODRV_SEEKAction(BYTE _bSeekDir)
{
     //[step 1] :select the seek direction
    if(_bSeekDir)
        wRADIODRVTunerReg[0x02]|=0x0200;// seek up set 1
    else
        wRADIODRVTunerReg[0x02]&=~(0x0200);// seek down set 0
    //[step 2]: Start to seek
        wRADIODRVTunerReg[0x02]|=0x0100;
       _RADIODRV_WriteReg(0x02,wRADIODRVTunerReg); 
    // [step3] :wait for the stc bit set (seek complete)
        //added time-out method to avoid radio system hang
         dwRadioDrvTime=OS_GetSysTimer();  
        do{              
		_RADIODRV_ReadReg(0x0A, wRADIODRVTunerReg);            
             if((OS_GetSysTimer()-dwRadioDrvTime)>COUNT_2_SEC )
             {
                //printf("\n @@SF1");
                return RADIODRV_SEEK_ERR;
             }
        } while ((wRADIODRVTunerReg[0x0A] & 0x4000) == 0x0000); 
          
    //[step4 ]:clear the stc bit  and wait for the STC bit be cleared  
   	  wRADIODRVTunerReg[ 0x02] &= ~(0x0100);
         _RADIODRV_WriteReg(0x02,wRADIODRVTunerReg); 
        //added time-out method to avoid radio system hang
         dwRadioDrvTime=OS_GetSysTimer();  
	 do{        
		_RADIODRV_ReadReg(0x0A, wRADIODRVTunerReg);
             if((OS_GetSysTimer()-dwRadioDrvTime)>COUNT_2_SEC )
             {
               // printf("\n @@SF2");
                return RADIODRV_SEEK_ERR;
             }
        } while ((wRADIODRVTunerReg[0x0A] & 0x4000) != 0x0000); 
     //[step5]: Read the channel number
     _RADIODRV_ReadReg(0x0B, wRADIODRVTunerReg);
     //[step6 ]: return the seek status 
     //return 0 is successful and 1 is fail      
     if((wRADIODRVTunerReg[0x0A] & 0x2000))
     {
        printf("\n @seek error");
        return RADIODRV_SEEK_ERR;
     }
     else
     {
       // printf("\n @seek ok");
        return RADIODRV_SEEK_NOERR;
     }     
}
//*********************************************************************************************************
//  @summary    This function be used to seek station
//  @param  bSeekDIR   if _bSeekDir as TRUN, the seek up direction, else _bSeekDir as FALSE, the seek down direction
//  @return Get the seek status
//  @retval TRUE    Seek station successfully
//  @retval FALSE   Seek station fail
//  @description
//      This function be used to seek station
//  @bug    None
//  @history    
//*********************************************************************************************************
BYTE RADIODRV_Seek(BYTE bSeekDIR) //seek up as 1 / seek down 0
{  
    if(!_RADIODRV_SEEKAction(bSeekDIR)) //seek ok
    {
       
       if((RADIODRV_GetBand()==RADIODRV_BS_USA)|(RADIODRV_GetBand()==RADIODRV_BS_JapWide))
       {  
            if(RADIODRV_GetFreq()==10800) //usa max frequency 108Mhz
            {
            //    printf("\n @@ SEEK MAX");
                if(!_RADIODRV_SEEKAction(bSeekDIR))
                return RADIODRV_SEEK_NOERR; 
                else
                return RADIODRV_SEEK_ERR;    
            }
        
        }
       else // japan
       {
            if(RADIODRV_GetFreq()==9000) //japan max frequency 90Mhz
            {
                if(!_RADIODRV_SEEKAction(bSeekDIR))
                return RADIODRV_SEEK_NOERR; 
                else
                return RADIODRV_SEEK_ERR;  
            }
        }
        return RADIODRV_SEEK_NOERR;
    }
    else
    {
          return RADIODRV_SEEK_ERR;
    }  
	
}

//*********************************************************************************************************
//  @summary    This function be used to auto scan 
//  @param  wFreq   Desired frequency which start auto scan
//  @param  wPreSetArray    Be used to record the preset stations.
//  @param  bCount          Desired total auto scan stations
//  @param  bFlag           Force to stop auto scan
//  @return Get the auto scan status
//  @retval TRUE    Get the auto scan successfully
//  @retval FALSE   Get the auto scan fail
//  @description
//      This function be used to auto scan stations
//  @bug    None
//  @history    
//*********************************************************************************************************
BYTE RADIODRV_AutoScan(WORD wFreq,WORD* wPreSetArray, BYTE bCount, BYTE bFLAG)
{
     BYTE bIndex;
     BYTE bRADIODRVStauts; //record seek status  
     WORD wRADIODRV_CurrentFreq;     

     wRADIODRV_CurrentFreq=0;  
     bRADIODRVStauts=0;
 //    printf("\n@@ start auto scan");
     RADIODRV_SetMute(1); //dis-able mute
    //Step 1: initial preset array    
    for (bIndex=0; bIndex<bCount; bIndex++)
    {       
	wPreSetArray[bIndex] = 0xFFFF;
    }
    //step 2: auto scan from setting of frequency
    RADIODRV_tune(wFreq);          
    //step3 : loop to do auto scan from setting frequency to couter station
    for(bIndex=0; bIndex<bCount;bIndex++)
    {  //[step4.1] : check error condition or force break 
         if(bFLAG) break; 
        
         //[step3.2]: store frequency for current station        
         //[step3.3]: To do next station
         bRADIODRVStauts=RADIODRV_Seek(1);
         wRADIODRV_CurrentFreq=RADIODRV_GetFreq();
         if(!bRADIODRVStauts)
         {
            wPreSetArray[bIndex]=wRADIODRV_CurrentFreq; 
         }
         else
         {
            break;
         }
         if(wRADIODRV_CurrentFreq>=wFreq) break; //don't loop to seek
    } 
 
   if(bRADIODRVStauts) 
   {
    return RADIODRV_ERR;
   }
   else
   {  
    return RADIODRV_NOERR;
   }
  
 RADIODRV_SetMute(0); //enable mute
}

#ifdef SUPPORT_RADIO_RDS
//*********************************************************************************************************
//  @summary    This function be used to initiate RDS functions 
//  @param  None
//  @return None
//  @retval None
//  @description
//      This function be used to initiate RDS functions
//  @bug    None
//  @history    
//*********************************************************************************************************
void RADIODRV_RDSInit(void)
{
    BYTE bIndex;
     //initial RDS variable 
     wRADIODRV_PI=0x00;
     bRADIODRV_CountPI=0x00;      
     bRADIODRV_CountPTY=0x00;
     bRADIODRV_CountPS=0x00;     
     bRADIODRV_CountECC=0x00;
     bRADIODRV_CountPTYN=0x00;
     bRADIODRV_RDS_RTLen=0x00;
     bRADIODRV_TP=0x00; //traffic programme id
     bRADIODRV_TA=0x00;//traffic announcement
     //initial array
     for(bIndex=0x00;bIndex<sizeof(bRADIODRV_RDS_PTY);bIndex++ )
        bRADIODRV_RDS_PTY[bIndex]=0x20;
     for(bIndex=0x00;bIndex<sizeof(bRADIODRV_RDS_PS);bIndex++ )
        bRADIODRV_RDS_PS[bIndex]=0x20;
     for(bIndex=0x00;bIndex<sizeof(bRADIODRV_RDS_PTYN);bIndex++ )
        bRADIODRV_RDS_PTYN[bIndex]=0x20;
     for(bIndex=0x00;bIndex<sizeof(bRADIODRV_RDS_RT);bIndex++ )
     {  
        bRADIODRV_RDS_RT[bIndex]=0x20;
       // bRADIODRV_RDS_RTmp[bIndex]=0x20;
     }
     //Read RDS register setting
     _RADIODRV_ReadReg(0x0f, wRADIODRVTunerReg);
     bRADIODRV_RDS_flag=(BYTE)((wRADIODRVTunerReg[0xD] & 0x0010) >> 4);
     bRADIODRV_RDS_RTFirst=0x00; //the flag use the first segment address about radio text (RT)
     bRADIODRV_RDS_RTEnd=0x00;    
     wRADIODRV_RDS_RTmp=0x00;
     wRADIODRV_RDS_RT=0x00;
    // printf("\n @0x0A:%x",wRADIODRVTunerReg[0x0A]);
}
//*********************************************************************************************************
//  @summary    This function be used to check RDS type from on-air 
//  @param  bRDS_TYPE   User needs to get RDS type form on-air
//  @return Get the status of the RDS type 
//  @retval TRUE    Support RDS tpye of the on-air
//  @retval FALSE   Don't support RDS type of the on-air
//  @description
//      This function be used to check RDS type from on-air
//  @bug    None
//  @history    
//*********************************************************************************************************
BYTE RADIODRV_RDSTYPE(BYTE bRDS_TYPE)
{   
    BYTE bRDS_Group=0; //record RDS group type
    //step[1]:read the RDS group type  
    _RADIODRV_ReadReg(0x0D, wRADIODRVTunerReg);    
     //printf("\n @0x0A=%x",wRADIODRVTunerReg[0x0A]);
    //step[2]: wait RDS data ready, then read group
    if((wRADIODRVTunerReg[0x0A]&0x8000)==0x8000)
    {
        bRDS_Group=(BYTE)(wRADIODRVTunerReg[0x0D]>>11);   
    
    //step[3]: compare desired bRDS_TYPE to from bRDS_Group
        switch(bRDS_TYPE)
        {
            case RDS_PI: //program identification which support all group from RDS spec.
            case RDS_PTY:  //program type which suppport all group from RDS spec.
            case RDS_TP:    //Traffic programme identification code which support all group frim RDS spec.
                if((bRDS_Group>=RDS_TYPE_0A)||(bRDS_Group<=RDS_TYPE_15B))                          
                    return RADIODRV_NOERR;                
                else                 
                    return RADIODRV_ERR;                
                break;            

          case RDS_ECC: //support the Externd country code
                if(bRDS_Group==RDS_TYPE_1A)                          
                    return RADIODRV_NOERR;                
                else                 
                    return RADIODRV_ERR;                
                break; 
                
          case RDS_PS: //program service name only in 0x0A and 0x0B group
                if((bRDS_Group==RDS_TYPE_0A)||(bRDS_Group==RDS_TYPE_0B))
                    return RADIODRV_NOERR;
                else
                    return RADIODRV_ERR;
                break;
            
          case RDS_AF: //alternative frequency only in 0x0A and 0x0B group
                if(bRDS_Group==RDS_TYPE_0A)
                    return RADIODRV_NOERR;
                else
                    return RADIODRV_ERR;            
                break;
            
          case RDS_TA://Traffic announcement only in 0x0A, 0x0B, 0x14B, 0x15B
                if((bRDS_Group==RDS_TYPE_0A)||(bRDS_Group==RDS_TYPE_0B)||(bRDS_Group==RDS_TYPE_14B)||(bRDS_Group==RDS_TYPE_15B))
                    return RADIODRV_NOERR;
                else
                    return RADIODRV_ERR;
                break;
            
            case RDS_DI://decoder identification code only in 0x0A, 0x0B, 0x15B
            case RDS_MS://music speed only in 0x0A, 0x0B, 0x15B
                if((bRDS_Group==RDS_TYPE_0A)||(bRDS_Group==RDS_TYPE_0B)||(bRDS_Group==RDS_TYPE_15B))
                    return RADIODRV_NOERR;
                else
                    return RADIODRV_ERR;
                break;
          
            case RDS_RT: //radio text only in 0x2A, 0x2B
                 if((bRDS_Group==RDS_TYPE_2A)||(bRDS_Group==RDS_TYPE_2B))
                    return RADIODRV_NOERR;
                else
                    return RADIODRV_ERR;
                break;     
                
            case RDS_PTYN:
                 if((bRDS_Group==RDS_TYPE_10A))
                    return RADIODRV_NOERR;
                else
                    return RADIODRV_ERR;
                break;      
                
           case RDS_EON://Enhanced other networks inforamtion only in 0x14A
                if(bRDS_Group==RDS_TYPE_14A)
                    return RADIODRV_NOERR;
                else
                    return RADIODRV_ERR;
                break;

           case RDS_CT:  //support the clock-time and date
                if(bRDS_Group==RDS_TYPE_4A)
                    return RADIODRV_NOERR;
                else
                    return RADIODRV_ERR;
                break;
        default:           
                return RADIODRV_ERR;
            break;
        }
    }
     return FALSE; //non support this type
      
}
//*********************************************************************************************************
//  @summary    This function be used to update RDS information
//  @param  bRDS_TYPE   User needs to update RDS type information
//  @param  pbRDString  User desires to display message
//  @return Get the status of updated RDS information
//  @retval TRUE    Updated the RDS information is ready
//  @retval FALSE   Updated the RDS information is un-ready
//  @description
//      This function be used to update RDS information
//  @bug    None
//  @history    
//*********************************************************************************************************
BYTE RADIODRV_UpdateRDS(BYTE bRDS_Type, BYTE* pbRDString)
{
    //step[0]: read the RDS block data
    _RADIODRV_ReadReg(0x0f, wRADIODRVTunerReg);
    //step[1]: wait RDS data ready, then read group
    if((wRADIODRVTunerReg[0x0A]&0x8000)==0x8000)
    {
        if((wRADIODRVTunerReg[0x0B]&0xC000) || (wRADIODRVTunerReg[0x0B]&0x3000) || (wRADIODRVTunerReg[0x0B]&0x0C00))
        {
            return FALSE;
        }
       // printf("\n @@ U_RDS!!");
        switch(bRDS_Type)
        {
            //
            case RDS_PI: //program identification
                if(RADIODRV_RDS_PI(pbRDString))
                    return RADIODRV_NOERR;
                else    
                    return RADIODRV_ERR;
                break;
            //
            case RDS_PTY://program type
                if(RADIODRV_RDS_PTY(pbRDString))
                    return RADIODRV_NOERR;
                else    
                    return RADIODRV_ERR;
                break;
            //
            case  RDS_TP: //traffic programme
                if(RADIODRV_RDS_TP())
                    return RADIODRV_NOERR;
                else    
                    return RADIODRV_ERR;
                break; 
             //
             case  RDS_ECC: //traffic programme
                if(RADIODRV_RDS_ECC(pbRDString))
                    return RADIODRV_NOERR;
                else    
                    return RADIODRV_ERR;
                break; 
              //
             case  RDS_TA:
                if(RADIODRV_RDS_TA())
                    return RADIODRV_NOERR;
                else    
                    return RADIODRV_ERR;
                break;      
            //
            case RDS_DI:
                if(RADIODRV_RDS_DI())
                    return RADIODRV_RDS_DI();
                else
                    return RADIODRV_ERR;
                break;
            //
             case RDS_PS://program service name
                if(RADIODRV_RDS_PS(pbRDString))
                    return RADIODRV_NOERR;
                else
                    return RADIODRV_ERR;
                break;
            //
             case RDS_RT://radio text
                if(RADIODRV_RDS_RT(pbRDString))
                    return RADIODRV_NOERR;                
                else    
                    return RADIODRV_ERR;
                break;
            //
            case RDS_CT://clock time
                if(RADIODRV_RDS_CT(pbRDString))
                    return RADIODRV_NOERR;                
                else    
                    return RADIODRV_ERR;
                break;
            //
            case RDS_PTYN:
                if(RADIODRV_RDS_PTYN(pbRDString))
                      return RADIODRV_NOERR;                
                else    
                    return RADIODRV_ERR;
                break;
            //
            default:
                    return RADIODRV_ERR;
                break;                
        }
        return FALSE;
    }
    return FALSE;
}
//*********************************************************************************************************
//  @summary    This function be used to access PI code form the broadcasting
//  @param  pbRDS_PI   User needs to get PI code information
//  @return Get the status of the PI code
//  @retval TRUE    Get the PI code successfully
//  @retval FALSE   Get the PI code fail
//  @description
//      This function be used to access PI code from the broadcasting
//  @bug    None
//  @history    
//*********************************************************************************************************
BYTE RADIODRV_RDS_PI(BYTE* pbRDS_PI )
{   
    BYTE bRDS_GRP;      
    //step[1]: check group 0x0A only block 1 support PI code,but group 0x0B block1/3 support PI code
    bRDS_GRP=(BYTE)(wRADIODRVTunerReg[0x0D]>>11); 
    //step[2]: needs more time to check actual PI  code
    if(bRDS_GRP&0x01)//group 0x0B : PI code exist two block 
    {    
        if((wRADIODRV_PI==wRADIODRVTunerReg[0x0C])&&(wRADIODRVTunerReg[0x0C]==wRADIODRVTunerReg[0x0E]))
        {
            wRADIODRV_PI=wRADIODRVTunerReg[0x0C];    
            bRADIODRV_CountPI+=1;
        }
        else
        {
             wRADIODRV_PI=wRADIODRVTunerReg[0x0C];
        }
    }
    else//group 0x0A
    {        
        if(wRADIODRV_PI==wRADIODRVTunerReg[0x0C])
        {
            wRADIODRV_PI=wRADIODRVTunerReg[0x0C];
            bRADIODRV_CountPI+=1;
         //   printf("\n @@PI--%x",wRADIODRVTunerReg[0x0C]);     
        }
        else
        {
            wRADIODRV_PI=wRADIODRVTunerReg[0x0C];
        }
    }    
    //step[3]: return PI code
    if(bRADIODRV_CountPI>=RDS_VALIDATE_LIMIT)
    {        
        pbRDS_PI[0]=0x02; // PI code length
        pbRDS_PI[1]=(BYTE)(wRADIODRVTunerReg[0x0C]&0x00FF); //low BYTE for programme reference number
        pbRDS_PI[2]=(BYTE)(wRADIODRVTunerReg[0x0C]>>8);      //high BYTE for  high-nibble for programme type 
        wRADIODRV_PI=0x00;                                                         //low-nibble for country identification
        bRADIODRV_CountPI=0x00;
     //   printf("@@_PI_1 %hx, %hx, %hx",bRADIODRV_CountPI,pbRDS_PI[2],pbRDS_PI[1]);
        return RADIODRV_NOERR; //true
    }    
   return RADIODRV_ERR;//false
        
}
//*********************************************************************************************************
//  @summary    This function be used to access ECC info. from the broadcasting
//  @param  pbRDS_ECC   User needs to access ECC info. from the broadcasting
//  @return Get the status of the ECC code
//  @retval TRUE    Get the ECC code successfully
//  @retval FALSE   Get the ECC code fail
//  @description
//      This function be used to access ECC code from the broadcasting
//  @bug    None
//  @history    
//*********************************************************************************************************
BYTE RADIODRV_RDS_ECC(BYTE* pbRDS_ECC )
{   
    BYTE bRDS_GRP;      
    //step[1]: check group 0x0A only block 1 support PI code,but group 0x0B block1/3 support PI code
    bRDS_GRP=(BYTE)(wRADIODRVTunerReg[0x0D]>>11); 
    //step[2]: needs more time to check actual ECC code
    if(bRDS_GRP==RDS_TYPE_1A)
    {
          if((wRADIODRVTunerReg[0x0E]&0x7000)==0x0000) //check the variant code
          {
                if(bRADIODRV_ECC==(wRADIODRVTunerReg[0x0E]&0x000F))
                {
                    bRADIODRV_ECC=(wRADIODRVTunerReg[0x0E]&0x000F);
                    bRADIODRV_CountECC=bRADIODRV_CountECC+1;
                }
                else
                {
                    bRADIODRV_ECC=(wRADIODRVTunerReg[0x0E]&0x000F);
                }
                //if get the ECC code count more than RDS_VALIDATE_LIMIT, then get the ECC value
                if(bRADIODRV_CountECC>=RDS_VALIDATE_LIMIT)
                {
                    pbRDS_ECC[0]=0x01;
                    pbRDS_ECC[1]=(wRADIODRVTunerReg[0x0E]&0x000F);
                    bRADIODRV_CountECC=0x00;
                    return RADIODRV_NOERR; //True
                }
                return RADIODRV_ERR;//false
          }
          return RADIODRV_ERR;//false
    }        
     return RADIODRV_ERR;//false
}
//*********************************************************************************************************
//  @summary    This function be used to access PTY info. from the broadcasting
//  @param  pbRDS_ECC   User needs to access PTY info. from the broadcasting
//  @return Get the status of the PTY info.
//  @retval TRUE    Get the PTY code successfully
//  @retval FALSE   Get the PTY code fail
//  @description
//      This function be used to access PTY code from the broadcasting
//  @bug    None
//  @history    
//*********************************************************************************************************
BYTE RADIODRV_RDS_PTY(BYTE* pbRDS_PTY)
{
    BYTE bIndex;   
    BYTE bRDS_PTY;
    //Needs more time to get the PTY code, The purpose is that get correct PTY value
    if(bRADIODRV_PTY==(BYTE)((wRADIODRVTunerReg[0xD]>>5) & 0x1f))
    {
        bRADIODRV_PTY=(BYTE)((wRADIODRVTunerReg[0xD]>>5) & 0x1f); //pty code
        bRADIODRV_CountPTY+=1;            
    }
    else
    {
         bRADIODRV_PTY=(BYTE)((wRADIODRVTunerReg[0xD]>>5) & 0x1f);        
        return FALSE;
    }
    if(bRADIODRV_CountPTY>=RDS_VALIDATE_LIMIT)       
    {        
          //   printf("\n @@ PTY_1");
        if(strcmp(pbRDS_PTY,bRADIODRV_RDS_PTY)) //pty code string
        {       
            bRDS_PTY=(BYTE)((wRADIODRVTunerReg[0xD]>>5) & 0x1f);//get the PTY
            for(bIndex=0;bIndex<9;bIndex++)
            pbRDS_PTY[bIndex]= 0x20;    //clear the array
         //   printf("\n @@ PTY_2");   
            switch(bRDS_PTY)
            {          
                case RDS_PTY_NONE:
                    strcpy(pbRDS_PTY+1,"None\0");                    
                    break;
                case RDS_PTY_NEWS:    
                    strcpy(pbRDS_PTY+1,"News\0");                     
                    break;
                case RDS_PTY_AFFAIRS:
                    strcpy(pbRDS_PTY,"Affairs\0");                     
                    break;
                case RDS_PTY_INFO:
                    strcpy(pbRDS_PTY+1,"Info\0");                     
                    break;
                case RDS_PTY_SPORT:
                    strcpy(pbRDS_PTY+1,"Sport\0");                      
                    break;    
                case RDS_PTY_EDUCATE:
                    strcpy(pbRDS_PTY+1,"Educate\0");                     
                    break;        
                case RDS_PTY_DRAMA:
                    strcpy(pbRDS_PTY+1,"Drama\0");                     
                    break;    
                case RDS_PTY_CULTURE:
                    strcpy(pbRDS_PTY+1,"Culture\0");                     
                    break;    
                case RDS_PTY_SCIENCE:                       
                    strcpy(pbRDS_PTY+1,"Science\0");                   
                    break;
                case RDS_PTY_VARIED:
                    strcpy(pbRDS_PTY+1,"Varied\0");                     
                    break;
                case RDS_PTY_POPM:
                    strcpy(pbRDS_PTY+1,"Pop M\0");                     
                    break;
                case RDS_PTY_ROCKM:
                    strcpy(pbRDS_PTY+1,"ROCK M\0");
                    break;
                case RDS_PTY_EASYM:
                    strcpy(pbRDS_PTY+1,"Easy M\0");
                    break;    
                case RDS_PTY_LIGHTM:
                    strcpy(pbRDS_PTY+1,"Light M\0");
                    break;    
                case RDS_PTY_CLASSICS:
                    strcpy(pbRDS_PTY+1,"Classics\0");
                    break; 
                case RDS_PTY_OTHERM:
                    strcpy(pbRDS_PTY+1,"Other M\0");
                    break;       
                case RDS_PTY_Weather:
                    strcpy(pbRDS_PTY+1,"Weather\0");
                    break;      
                case RDS_PTY_FINANCE:
                    strcpy(pbRDS_PTY+1,"Finance\0");
                    break;   
                case RDS_PTY_CHIDREN:
                    strcpy(pbRDS_PTY+1,"Children\0");
                    break;     
                case RDS_PTY_SOCIAL:
                    strcpy(pbRDS_PTY+1,"Social\0");
                    break;      
                case RDS_PTY_RELIGION:
                    strcpy(pbRDS_PTY+1,"Religion\0");
                    break;    
                case RDS_PTY_PHONE:
                    strcpy(pbRDS_PTY+1,"Phone In\0");
                    break;    
                case RDS_PTY_TRAVEL:
                    strcpy(pbRDS_PTY+1,"Travel\0");                     
                    break;
                case RDS_PTY_LEISURE:
                    strcpy(pbRDS_PTY+1,"Leisure\0");                     
                    break;     
                case RDS_PTY_JAZZ:
                    strcpy(pbRDS_PTY+1,"JAZZ\0");                     
                    break;   
                 case RDS_PTY_COUNTRY:
                    strcpy(pbRDS_PTY+1,"Country\0");                     
                    break;      
                case RDS_PTY_NATION:
                    strcpy(pbRDS_PTY+1,"Nation M\0");                     
                    break;  
                case RDS_PTY_OLDIES:
                    strcpy(pbRDS_PTY+1,"Oldies\0");                     
                    break; 
                case RDS_PTY_FOLKM:
                    strcpy(pbRDS_PTY+1,"Folk M\0");                     
                    break;  
                case RDS_PTY_DOC:
                    strcpy(pbRDS_PTY+1,"Document\0");                     
                    break;    
               case RDS_PTY_TEST:
                    strcpy(pbRDS_PTY+1,"TEST\0");                     
                    break;       
                case RDS_PTY_ALARM:
                    strcpy(pbRDS_PTY+1,"Alarm\0");                     
                    break;
                    
                default:
                    printf("\n @@ No PTY");       
                    return RADIODRV_ERR;
                    break;
            } 
            //store the radio program type
         strcpy(bRADIODRV_RDS_PTY,pbRDS_PTY);         
          //Get the string length
         bIndex=1;
         while(pbRDS_PTY[bIndex]!='\0')          
            bIndex++;       
         pbRDS_PTY[0]=bIndex-1;
         //printf("\n @@PTY_L %hx",pbRDS_PTY[0]);
         bRADIODRV_CountPTY=0x00;
         if(bRDS_PTY==RDS_PTY_NONE)
         {
             return (!bRDS_PTY); //return 0xFF
         }
         else
         {
            return bRDS_PTY; //return the PTY
         }
         }
        else
        {
           // printf("\n @@ PTY NS~~");
            return RADIODRV_ERR;
        }
    }
    return RADIODRV_ERR;
}
//*********************************************************************************************************
//  @summary    This function be used to access PS info. from the broadcasting
//  @param  pbRDS_PS    User needs to access PS info. from the broadcasting
//  @return Get the status of the PS info.
//  @retval TRUE    Get the PS code successfully
//  @retval FALSE   Get the PS code fail
//  @description
//      This function be used to access PS code from the broadcasting
//  @bug    None
//  @history    
//*********************************************************************************************************
BYTE RADIODRV_RDS_PS(BYTE* pbRDS_PS)
{
   BYTE bPSIndex=0;    
//   BYTE bIndex;
  //step[1]: read program service name index which using for character index
  bPSIndex=(wRADIODRVTunerReg[0x0d]&0x03)*2;
  //step[2]: put the character to program service array
  bRADIODRV_RDS_PS[bPSIndex+1]=(wRADIODRVTunerReg[0x0f]>>0x08);//high word
  bRADIODRV_RDS_PS[bPSIndex+2]=(wRADIODRVTunerReg[0x0f]&0xFF);//low word    
  //Step[3]:To confirm the completed program service name.
  //The total program service name has 8 characters to fill the PS array.So, needs to confirm tatal PS filled the array!!
  if((bRADIODRV_RDS_PS[1]==(wRADIODRVTunerReg[0x0f]>>0x08))&&(bRADIODRV_RDS_PS[2]==(wRADIODRVTunerReg[0x0f]&0xFF)))
  bRADIODRV_CountPS|=0x01;
  if((bRADIODRV_RDS_PS[3]==(wRADIODRVTunerReg[0x0f]>>0x08))&&(bRADIODRV_RDS_PS[4]==(wRADIODRVTunerReg[0x0f]&0xFF)))
  bRADIODRV_CountPS|=0x02;
  if((bRADIODRV_RDS_PS[5]==(wRADIODRVTunerReg[0x0f]>>0x08))&&(bRADIODRV_RDS_PS[6]==(wRADIODRVTunerReg[0x0f]&0xFF)))
  bRADIODRV_CountPS|=0x04;
  if((bRADIODRV_RDS_PS[7]==(wRADIODRVTunerReg[0x0f]>>0x08))&&(bRADIODRV_RDS_PS[8]==(wRADIODRVTunerReg[0x0f]&0xFF)))
  bRADIODRV_CountPS|=0x08;  
  //step[3]: check the the program service name. if the PS is different, program must copy new PS strings
  //The bRADIODRV_CountPS=0x0F mean the Program service name is completed
  if(((bRADIODRV_CountPS&0x0F)==0x0F)&&(strcmp(pbRDS_PS,bRADIODRV_RDS_PS))) 
  {       
        //the length of program service name 
         bRADIODRV_RDS_PS[0]=0x08;        
        strcpy(pbRDS_PS,bRADIODRV_RDS_PS);   
        bRADIODRV_CountPS=0x00;//reset the counter        
        return RADIODRV_NOERR; //Needs to return the character table. Support three Character table in the RDS spec
          // for(bIndex=0;bIndex<9;bIndex++)        
          //printf("\n@@_1 %hx,%hx",bRADIODRV_RDS_PS[bIndex],bRADIODRV_RDS_PS[bIndex]);
  }
  return RADIODRV_ERR;   
}
//*********************************************************************************************************
//  @summary    This function be used to access TP info. from the broadcasting
//  @param  None
//  @return Get the status of the TP info.
//  @retval TRUE    Get the TP code successfully
//  @retval FALSE   Get the TP code fail
//  @description
//      This function be used to access TP code from the broadcasting
//  @bug    None
//  @history    
//*********************************************************************************************************
BYTE RADIODRV_RDS_TP(void)
{ 
    //Traffic programme identification code in all group
    //read the TP bit
    bRADIODRV_TP=(BYTE)(wRADIODRVTunerReg[0x0D]>>10); 
    if(bRADIODRV_TP)
    {
       //printf("\n @@TP");
        return RADIODRV_NOERR;
    }
    else
    {     
       //printf("\n @@NON_TP");
        return RADIODRV_ERR;
    }
    
}
//*********************************************************************************************************
//  @summary    This function be used to access TA info. from the broadcasting
//  @param  None
//  @return Get the status of the TA info.
//  @retval TRUE    Get the TA code successfully
//  @retval FALSE   Get the TA code fail
//  @description
//      This function be used to access TA code from the broadcasting
//  @bug    None
//  @history    
//*********************************************************************************************************
BYTE RADIODRV_RDS_TA(void)
{
    //Traffic announcement code
    bRADIODRV_TA=(BYTE)(wRADIODRVTunerReg[0x0D]>>4); 
     if(bRADIODRV_TA)
    {          
        return RADIODRV_NOERR;
    }
    else
    {    
        return RADIODRV_ERR;
    }   
}
//*********************************************************************************************************
//  @summary    This function be used to access DI info. from the broadcasting
//  @param  None
//  @return Get the status of the DI info.
//  @retval TRUE    Get the DI code successfully
//  @retval FALSE   Get the DI code fail
//  @description
//      This function be used to access DI(decoder identification) code from the broadcasting.
//      There are 4-bit used to indicate different operating modes to switch individual decoders on/off
//      and to indicate if PTY codes in the transmission are dynamically switched
//  @bug    None
//  @history    
//*********************************************************************************************************
BYTE RADIODRV_RDS_DI(void)
{
    BYTE bDIIndex; 
    BYTE bDItmp;
    bDIIndex=(wRADIODRVTunerReg[0x0d]&0x03);//get the DI index
    bDItmp=(BYTE)((wRADIODRVTunerReg[0x0D]>>2)&0x01); //get the DI value
    bRADIODRV_DI|=(bDItmp<<(3-bDIIndex));
   // pbRDS_DI[0]=(bRADIODRV_DI&0x0F);
   // printf("\n @@ DI %hx, %hx, %x",bDIIndex,bRADIODRV_DI,(wRADIODRVTunerReg[0x0d]));
    return (bRADIODRV_DI&0x0F);
}
//*********************************************************************************************************
//  @summary    This function be used to access RT info. from the broadcasting
//  @param  pbRDS_RT    Desired display radio text
//  @return Get the status of the RT info.
//  @retval TRUE    Get the RT info. successfully
//  @retval FALSE   Get the RT info. fail
//  @description
//      This function be used to access radio text from the broadcasting.
//  @bug    None
//  @history    
//*********************************************************************************************************
BYTE RADIODRV_RDS_RT(BYTE* pbRDS_RT)
{
     BYTE bRTIndex;    
     BYTE bIndex;
     BYTE bRDS_GRP;
     BYTE bRDS_FLAG;
     //[step 1]: read the radio text index and change flag     
     bRDS_GRP=(BYTE)(wRADIODRVTunerReg[0x0D]>>11);   
     bRDS_FLAG=(BYTE)((wRADIODRVTunerReg[0xD] & 0x0010) >> 4); //read the flag  
     //[step 2]: if A/B flag has been change, user needs to clean text array and variables
     if(bRADIODRV_RDS_flag!=bRDS_FLAG)
     {
     //   printf("@@_0");
        bRADIODRV_RDS_RTFirst=0x00;
        bRADIODRV_RDS_RTLen=0x00;    
        bRADIODRV_RDS_RTEnd=0x00;
        wRADIODRV_RDS_RTmp=0x00;
        wRADIODRV_RDS_RT=0x00;
        for(bIndex=0;bIndex<sizeof(bRADIODRV_RDS_RT);bIndex++)    
        {
           bRADIODRV_RDS_RT[bIndex]=0x20; //clean null character       
         //  bRADIODRV_RDS_RTmp[bIndex]=0x20;
         }        
     }
     //[step 3]: user needs to updated the radio text by text segment for 0x2A group
     if(bRDS_GRP==RDS_TYPE_2A)
     {    
        bRTIndex=(wRADIODRVTunerReg[0x0d]&0x000F)*4; //text segment be used to index the position of text
        if(bRTIndex==0x00)//get the first text segment, user start to store array
        {
            bRADIODRV_RDS_RTFirst=0x01;
            bRADIODRV_RDS_flag=bRDS_FLAG;
         }     
        if(bRADIODRV_RDS_RTFirst)//read the radio text
        {
            bRADIODRV_RDS_RT[bRTIndex+1]=(wRADIODRVTunerReg[0x0E]>>0x08); // character 1     
            bRADIODRV_RDS_RT[bRTIndex+2]=(wRADIODRVTunerReg[0x0E]&0xFF);// character 2
            bRADIODRV_RDS_RT[bRTIndex+3]=(wRADIODRVTunerReg[0x0F]>>0x08);// character 3
            bRADIODRV_RDS_RT[bRTIndex+4]=(wRADIODRVTunerReg[0x0F]&0xFF);// character 4  
            //[step 4]:To make sure to get total characters by text segment
            if(!((wRADIODRV_RDS_RTmp>>(bRTIndex/4))&0x0001)) //check this text segment has been stored
            wRADIODRV_RDS_RTmp|=(0x0001<<(bRTIndex/4));//if this bit has not been stored, user needs to tag this text segment
            //printf("\n @@ %hx,_%hx,_%hx,_%hx,_%hx",bRTIndex,bRADIODRV_RDS_RT[bRTIndex+1],bRADIODRV_RDS_RT[bRTIndex+2],bRADIODRV_RDS_RT[bRTIndex+3],bRADIODRV_RDS_RT[bRTIndex+4]);
            //[step5]: if text [radio ==0x0D],the ending of this radio text
            if((bRADIODRV_RDS_RT[bRTIndex+1]==0x0D)||(bRADIODRV_RDS_RT[bRTIndex+2]==0x0D)||(bRADIODRV_RDS_RT[bRTIndex+3]==0x0D)||(bRADIODRV_RDS_RT[bRTIndex+4]==0x0D))
            {     
                //[step6]: check total radio text has been read via compare with radio text flag wRADIODRV_RDS_RTmp
                for(bIndex=0;bIndex<=(bRTIndex/4);bIndex++)
                {
                    wRADIODRV_RDS_RT|=(0x0001<<bIndex);
                }
                if(wRADIODRV_RDS_RTmp==wRADIODRV_RDS_RT)
                {
                    bRADIODRV_RDS_RTEnd=0x01;
                }              
            }   
            else
            {
                 return RADIODRV_ERR;           
            }
        }
     }
     else//0x2B only support the 32 character in RDS block 4
     { 
        //[step 3_2B]: user needs to updated the radio text by text segment for 0x2B group
        bRTIndex=(wRADIODRVTunerReg[0x0d]&0x000F)*2;//text segment be used to index the position of text
        if(bRTIndex==0x00)//get the first text segment, user start to store array
        {
            bRADIODRV_RDS_RTFirst=0x01;
            bRADIODRV_RDS_flag=bRDS_FLAG;
         }     
         if(bRADIODRV_RDS_RTFirst)//read the radio text
         {
             bRADIODRV_RDS_RT[bRTIndex+1]=(wRADIODRVTunerReg[0x0F]>>0x08);// character 3
             bRADIODRV_RDS_RT[bRTIndex+2]=(wRADIODRVTunerReg[0x0F]&0xFF);// character 4 
               //[step 4_2B]:To make sure to get total characters by text segment
             if(!((wRADIODRV_RDS_RTmp>>(bRTIndex/4))&0x0001))
             wRADIODRV_RDS_RTmp|=(0x0001<<(bRTIndex/4));
              //[step5_2B]: if text [radio ==0x0D],the ending of this radio text
             if((bRADIODRV_RDS_RT[bRTIndex+1]==0x0D)||(bRADIODRV_RDS_RT[bRTIndex+2]==0x0D))
             {
                //[step6_2B]: check total radio text has been read via compare with radio text flag wRADIODRV_RDS_RTmp
                 for(bIndex=0;bIndex<=(bRTIndex/4);bIndex++)
                {
                    wRADIODRV_RDS_RT|=(0x0001<<bIndex);
                }
                if(wRADIODRV_RDS_RTmp==wRADIODRV_RDS_RT)
                {
                    bRADIODRV_RDS_RTEnd=0x01;
                }           
             }
             else
             {
                 return RADIODRV_ERR;           
             }
         }
     }
     //[step 7]: checked the ending of radio text
      if(((bRTIndex==0x0F*4)&&(bRDS_GRP==0x2A))||((bRTIndex==0x0F*2)&&(bRDS_GRP==0x2B))||(bRADIODRV_RDS_RTEnd))//||(bRADIODRV_RDS_RT[bRTIndex+1]==0x0D)||(bRADIODRV_RDS_RT[bRTIndex+2]==0x0D)||(bRADIODRV_RDS_RT[bRTIndex+3]==0x0D)||(bRADIODRV_RDS_RT[bRTIndex+4]==0x0D))   
      {       
                //Get the length of total radio text
                 for(bIndex=0;bIndex<sizeof(bRADIODRV_RDS_RT);bIndex++)
                 {
                    if(bRADIODRV_RDS_RT[bIndex]==0x0D) 
                        break;
                    else
                        bRADIODRV_RDS_RTLen++;
                 }
                  bRADIODRV_RDS_RT[0]=bRADIODRV_RDS_RTLen-1;   
                 for(bIndex=bRADIODRV_RDS_RTLen;bIndex<sizeof(bRADIODRV_RDS_RT);bIndex++)    
                 {
                    bRADIODRV_RDS_RT[bIndex]=0x20; //clean null character  
                 }
                    
           //     if(strcmp(pbRDS_RT,bRADIODRV_RDS_RT))
            //    {     
                   strcpy(pbRDS_RT,bRADIODRV_RDS_RT);                                     
                    for(bIndex=0;bIndex<sizeof(bRADIODRV_RDS_RT);bIndex++)    
                    {
                        bRADIODRV_RDS_RT[bIndex]=0x20; //clean null character       
                    //    bRADIODRV_RDS_RTmp[bIndex]=0x20;
                    }
                    bRADIODRV_RDS_RTLen=0x00;                 
                    bRADIODRV_RDS_RTEnd=0x00;
                    wRADIODRV_RDS_RTmp=0x00;
                    wRADIODRV_RDS_RT=0x00;
                    return RADIODRV_NOERR;
            //    }                      
             //    return RADIODRV_ERR;                  
      }      
       return RADIODRV_ERR;      
  
}
//*********************************************************************************************************
//  @summary    This function be used to access CT info. from the broadcasting
//  @param  pbRDS_CT    Desired display date and time
//  @return Get the status of the CT info.
//  @retval TRUE    Get the CT info. successfully
//  @retval FALSE   Get the CT info. fail
//  @description
//      This function be used to access clock and time from the broadcasting.
//  @bug    None
//  @history    
//*********************************************************************************************************
BYTE RADIODRV_RDS_CT(BYTE* pbRDS_CT)
{
   
    DWORD dwRDS_MJD;
    RDS_CTime  RDS_CTmp;
    BYTE bRDS_CTOFFSET;
    //[step1]: parser the MJD(Day code) and UTC
    RDS_CTmp.bRDS_CTHour=(BYTE)((wRADIODRVTunerReg[0x0F]>>12)&0x000F);
    RDS_CTmp.bRDS_CTHour=RDS_CTmp.bRDS_CTHour+(BYTE)(((wRADIODRVTunerReg[0x0E]&0x0001)<<4));
    RDS_CTmp.bRDS_CTMin=(BYTE)((wRADIODRVTunerReg[0x0F]>>6)&0x003F);
    bRDS_CTOFFSET=(BYTE)(wRADIODRVTunerReg[0x0F]&0x003F);
    dwRDS_MJD=((wRADIODRVTunerReg[0x0E]>>1)&0x7FFF);
    dwRDS_MJD=((wRADIODRVTunerReg[0x0D]&0x0003)<<15)+dwRDS_MJD;
  
    if(dwRDS_MJD==0x0000)
    {
        return RADIODRV_ERR;
    }
    //[step2]: get the MJD value, then do the conversion between time and date
    //Get the Date from the MJD
    RDS_CTmp.wRDS_CTYear=((100L*dwRDS_MJD-1507820)/36525L);
    RDS_CTmp.bRDS_CTMonth= (BYTE)((10000L*dwRDS_MJD-149561000L-10000L*((RDS_CTmp.wRDS_CTYear*36525L)/100L))/306001L);
    RDS_CTmp.bRDS_CTDate=(BYTE)(dwRDS_MJD-14956L-((RDS_CTmp.wRDS_CTYear*36525L)/100L)-((RDS_CTmp.bRDS_CTMonth*306001L)/10000L));
    if((RDS_CTmp.bRDS_CTMonth==14)||(RDS_CTmp.bRDS_CTMonth==15) )
    {
        RDS_CTmp.wRDS_CTYear=RDS_CTmp.wRDS_CTYear+1;
        RDS_CTmp.bRDS_CTMonth=RDS_CTmp.bRDS_CTMonth-1+1*12;
    }
    else
    {
        RDS_CTmp.wRDS_CTYear=RDS_CTmp.wRDS_CTYear;
       RDS_CTmp.bRDS_CTMonth=RDS_CTmp.bRDS_CTMonth-1;
    }
    RDS_CTmp.wRDS_CTYear=RDS_CTmp.wRDS_CTYear+1900;
    RDS_CTmp.bRDS_WEEK=(BYTE)(((dwRDS_MJD+2)%7)+1);
    //Get the Time from the MJD
    if(bRDS_CTOFFSET&0x20) //negative offset
    {      
        RDS_CTmp.bRDS_CTHour=RDS_CTmp.bRDS_CTHour-(bRDS_CTOFFSET/2);
    }
    else //positive offset
    {        
        RDS_CTmp.bRDS_CTHour=RDS_CTmp.bRDS_CTHour+(bRDS_CTOFFSET/2);
    }
    //protect date and clock
    if((RDS_CTmp.bRDS_CTMonth>12)||(RDS_CTmp.bRDS_CTMonth<=0)||((RDS_CTmp.bRDS_CTDate)>31)||((RDS_CTmp.bRDS_WEEK)>7)||((RDS_CTmp.bRDS_CTHour)>24)||((RDS_CTmp.bRDS_CTMin)>60))
        return RADIODRV_ERR;
    //
    pbRDS_CT[0]=7;
    pbRDS_CT[1]=(BYTE)((RDS_CTmp.wRDS_CTYear&0xFF00)>>8);//year
    pbRDS_CT[2]=(BYTE)(RDS_CTmp.wRDS_CTYear&0x00FF);
    pbRDS_CT[3]=RDS_CTmp.bRDS_CTMonth; //month
    pbRDS_CT[4]=RDS_CTmp.bRDS_CTDate; //date
    pbRDS_CT[5]=RDS_CTmp.bRDS_WEEK;//week
    pbRDS_CT[6]=RDS_CTmp.bRDS_CTHour;//hour
    pbRDS_CT[7]=RDS_CTmp.bRDS_CTMin;//min.     
   
 //   printf("\n@@hour: %hx",RDS_CTmp.bRDS_CTHour);
 //   printf("\n@@min: %hx",RDS_CTmp.bRDS_CTMin);
 //   printf("\n@@offset %hx",bRDS_CTOFFSET); 
  // printf("\n@@bRDS_CTYear: %x",pbRDS_CT[1]);
 //   printf("\n@@bRDS_CTMonth: %hx",RDS_CTmp.bRDS_CTMonth);
  //  printf("\n@@bRDS_CTDate: %hx",RDS_CTmp.bRDS_CTDate);
  // printf("\n@@bRDS_WEEK: %hx",RDS_CTmp.bRDS_WEEK);
    return RADIODRV_NOERR;
}
//*********************************************************************************************************
//  @summary    This function be used to access PTYN info. from the broadcasting
//  @param  pbRDS_PTYN    Desired display PTYN
//  @return Get the status of the PTYN info.
//  @retval TRUE    Get the PTYN info. successfully
//  @retval FALSE   Get the PTYN info. fail
//  @description
//      This function be used to access programme type information from the broadcasting.
//  @bug    None
//  @history    
//*********************************************************************************************************
BYTE RADIODRV_RDS_PTYN(BYTE* pbRDS_PTYN)
{
     BYTE bRDS_GRP;
     BYTE bRDS_PTYNIndex;
     bRDS_GRP=(BYTE)(wRADIODRVTunerReg[0x0D]>>11); 
     //[step1]: read the radio group (0x10A) for support the PTYN(Programme type Name)
     if(bRDS_GRP==RDS_TYPE_10A)
     {
        //step[2]: read the PTYN segment Address, 0 as Text character number from 1~4 and 1 as Text characger number from 5~8
        bRDS_PTYNIndex=(BYTE)((wRADIODRVTunerReg[0x0d]&0x0001)*4);
        bRADIODRV_RDS_PTYN[bRDS_PTYNIndex+1]=(wRADIODRVTunerReg[0x0E]>>0x08);//character 1     
        bRADIODRV_RDS_PTYN[bRDS_PTYNIndex+2]=(wRADIODRVTunerReg[0x0E]&0xFF);//character 2
        bRADIODRV_RDS_PTYN[bRDS_PTYNIndex+3]=(wRADIODRVTunerReg[0x0F]>>0x08);//character 3
        bRADIODRV_RDS_PTYN[bRDS_PTYNIndex+4]=(wRADIODRVTunerReg[0x0F]&0xFF);// character 4 
        //Step[3]:To confirm the completed Program type name.
        //The total Program type name has 8 characters to fill the PTYN array.So, needs to confirm tatal PTYN filled the array!!
        if((bRADIODRV_RDS_PTYN[1]==(wRADIODRVTunerReg[0x0E]>>0x08))&&(bRADIODRV_RDS_PTYN[2]==(wRADIODRVTunerReg[0x0E]&0xFF)))
        bRADIODRV_CountPTYN|=0x01; //ensure the char.1 and char.2
        if((bRADIODRV_RDS_PTYN[3]==(wRADIODRVTunerReg[0x0F]>>0x08))&&(bRADIODRV_RDS_PTYN[4]==(wRADIODRVTunerReg[0x0F]&0xFF)))
        bRADIODRV_CountPTYN|=0x02;//ensure the char.3 and char.4
        if((bRADIODRV_RDS_PTYN[5]==(wRADIODRVTunerReg[0x0E]>>0x08))&&(bRADIODRV_RDS_PTYN[6]==(wRADIODRVTunerReg[0x0E]&0xFF)))
        bRADIODRV_CountPTYN|=0x04;//ensure the char.5 and char.6
       if((bRADIODRV_RDS_PTYN[7]==(wRADIODRVTunerReg[0x0F]>>0x08))&&(bRADIODRV_RDS_PTYN[8]==(wRADIODRVTunerReg[0x0F]&0xFF)))
        bRADIODRV_CountPTYN|=0x08;//ensure the char.5 and char.6
       //step[4] To check the completed programme type names
       if(((bRADIODRV_CountPS&0x0F)==0x0F)&&(strcmp(pbRDS_PTYN,bRADIODRV_RDS_PTYN))) 
       {
         //strore the programme type name
         bRADIODRV_RDS_PTYN[0]=0x08;
         strcpy(pbRDS_PTYN,bRADIODRV_RDS_PTYN); 
         bRADIODRV_CountPTYN=0x00;
         return RADIODRV_NOERR;
       }
        return RADIODRV_ERR;
     }
     return RADIODRV_ERR;
}
//*********************************************************************************************************
//  @summary    This function be used to get RDS blocks from the broadcasting
//  @param  pwRDSBlock    Get the blocks of RDS data from the broadcastings
//  @return Get the status of the blocks of RDS info.
//  @retval TRUE    Get the block of RDS info. successfully
//  @retval FALSE   Get the block of RDS info. fail
//  @description
//      This function be used to access blocks of RDS infor. from the broadcasting.
//  @bug    None
//  @history    
//*********************************************************************************************************
BYTE RADIODRV_GetRDSBlock(WORD* pwRDSBlock)
{
     //step[0]: read the RDS block data
    _RADIODRV_ReadReg(0x0f, wRADIODRVTunerReg);
    //step[1]: wait RDS data ready, then read group
    if((wRADIODRVTunerReg[0x0A]&0x8000)==0x8000)
    {
        pwRDSBlock[0]=wRADIODRVTunerReg[0x0C];
        pwRDSBlock[1]=wRADIODRVTunerReg[0x0D];
        pwRDSBlock[2]=wRADIODRVTunerReg[0x0E];
        pwRDSBlock[3]=wRADIODRVTunerReg[0x0F];
        //printf("\n @@ RDS %x, %x, %x, %x",pwRDSBlock[0],pwRDSBlock[1],pwRDSBlock[2],pwRDSBlock[3]);
        return RADIODRV_NOERR;
    }
    return RADIODRV_ERR;
}
#endif //end SUPPORT_RADIO_RDS
#endif


