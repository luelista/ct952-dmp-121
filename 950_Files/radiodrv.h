#ifndef __RADIODRV_H__
#define __RADIODRV_H__

#ifdef __cplusplus
   extern "C"{
#endif
//---------------------------------------
//  @summary Define radio tuner ID
//  @description
//      Be used to define radio tuner ID
//  @note
//---------------------------------------
#define RADIO_TUNER_SI4703
//---------------------------------------
//  @summary Define address of radio tuner
//  @description
//      Be used to define address of radio tuner
//  @note
//---------------------------------------
#define RADIODRV_ADDRESS 0x20
//---------------------------------------
//  @summary Define ACK condition of IIC
//  @description
//      Be used to define ACK condiction of IIC
//  @note
//---------------------------------------
#define RADIODRV_IIC_ACK TRUE
//---------------------------------------
//  @summary Define NON-ACK condition of IIC
//  @description
//      Be used to define NON-ACK condiction of IIC
//  @note
//---------------------------------------
#define RADIODRV_IIC_NONACK FALSE
//---------------------------------------
//  @summary Define successful condition 
//  @description
//      Be used to return successful condition
//  @note
//---------------------------------------
#define RADIODRV_NOERR TRUE
//---------------------------------------
//  @summary Define error condition 
//  @description
//      Be used to return error condition
//  @note
//---------------------------------------
#define RADIODRV_ERR      FALSE
//---------------------------------------
//  @summary Define successful condition of seek station
//  @description
//      Be used to return successful condition of seek station
//  @note
//---------------------------------------
#define RADIODRV_SEEK_NOERR FALSE
//---------------------------------------
//  @summary Define fail condition of seek station
//  @description
//      Be used to return fail condition of seek station
//  @note
//---------------------------------------
#define RADIODRV_SEEK_ERR TRUE
//---------------------------------------
//  @summary Define band range of radio for USA
//  @description
//      Be used to define band range of radio for USA with 87.5~108MHZ
//  @note
//---------------------------------------
#define RADIODRV_BS_USA  0x00  
//---------------------------------------
//  @summary Define band range of radio for JapWide
//  @description
//      Be used to define band range of radio for JapWide with 76~108MHZ
//  @note
//---------------------------------------
#define RADIODRV_BS_JapWide 0x01 
//---------------------------------------
//  @summary Define band range of radio for Jap
//  @description
//      Be used to define band range of radio for JapWide with 76~90MHZ
//  @note
//---------------------------------------
#define RADIODRV_BS_Jap 0x02 
//---------------------------------------
//  @summary Define channel space of radio for USA
//  @description
//      Be used to define channel space of radio for USA with 200kHz
//  @note
//---------------------------------------
#define RADIODRV_CS_USA  0x00  
//---------------------------------------
//  @summary Define channel space of radio for JAP
//  @description
//      Be used to define channel space of radio for JAP with 100kHz
//  @note
//---------------------------------------
#define RADIODRV_CS_JAP   0x01 
//---------------------------------------
//  @summary Define channel space of radio for none
//  @description
//      Be used to define channel space of radio for JAP with 50kHz
//  @note
//---------------------------------------
#define RADIODRV_CS_Non  0x02 
//---------------------------------------
//  @summary Define serial number of radio tuner for SI4701
//  @description
//      Be used to define serial number of radio tuner for SI4701
//  @note
//---------------------------------------
#define RADIODRV_SI4701  0x01
//---------------------------------------
//  @summary Define serial number of radio tuner for SI4702
//  @description
//      Be used to define serial number of radio tuner for SI4702
//  @note
//---------------------------------------
#define RADIODRV_SI4702  0x02
//---------------------------------------
//  @summary Define serial number of radio tuner for SI4703
//  @description
//      Be used to define serial number of radio tuner for SI4703
//  @note
//---------------------------------------
#define RADIODRV_SI4703  0x03
//---------------------------------------
//  @summary Define serial number of radio tuner for SI4700
//  @description
//      Be used to define serial number of radio tuner for SI4700
//  @note
//---------------------------------------
#define RADIODRV_SI4700  0x04
//---------------------------------------
//  @summary Define serial number of radio tuner for fail condition
//  @description
//      Be used to define serial number of radio tuner for fail condition
//  @note
//---------------------------------------
#define RADIODRV_SIFAIL   0x00

#ifdef SUPPORT_RADIO_RDS
//---------------------------------------
//  @summary Define 0x0A group of RDS feature 
//  @description
//      Be used to define 0x0A group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_0A     ( 0 * 2 + 0)
//---------------------------------------
//  @summary Define 0x0B group of RDS feature 
//  @description
//      Be used to define 0x0B group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_0B     ( 0 * 2 + 1)
//---------------------------------------
//  @summary Define 0x1A group of RDS feature 
//  @description
//      Be used to define 0x1A group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_1A     ( 1 * 2 + 0)
//---------------------------------------
//  @summary Define 0x1B group of RDS feature 
//  @description
//      Be used to define 0x1B group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_1B     ( 1 * 2 + 1)
//---------------------------------------
//  @summary Define 0x2A group of RDS feature 
//  @description
//      Be used to define 0x2A group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_2A     ( 2 * 2 + 0)
//---------------------------------------
//  @summary Define 0x2B group of RDS feature 
//  @description
//      Be used to define 0x2B group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_2B     ( 2 * 2 + 1)
//---------------------------------------
//  @summary Define 0x3A group of RDS feature 
//  @description
//      Be used to define 0x3A group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_3A     ( 3 * 2 + 0)
//---------------------------------------
//  @summary Define 0x3B group of RDS feature 
//  @description
//      Be used to define 0x3B group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_3B     ( 3 * 2 + 1)
//---------------------------------------
//  @summary Define 0x4A group of RDS feature 
//  @description
//      Be used to define 0x4A group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_4A     ( 4 * 2 + 0)
//---------------------------------------
//  @summary Define 0x4B group of RDS feature 
//  @description
//      Be used to define 0x4B group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_4B     ( 4 * 2 + 1)
//---------------------------------------
//  @summary Define 0x5A group of RDS feature 
//  @description
//      Be used to define 0x5A group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_5A     ( 5 * 2 + 0)
//---------------------------------------
//  @summary Define 0x5B group of RDS feature 
//  @description
//      Be used to define 00x5B group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_5B     ( 5 * 2 + 1)
//---------------------------------------
//  @summary Define 0x6A group of RDS feature 
//  @description
//      Be used to define 0x6A group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_6A     ( 6 * 2 + 0)
//---------------------------------------
//  @summary Define 0x6B group of RDS feature 
//  @description
//      Be used to define 0x6B group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_6B     ( 6 * 2 + 1)
//---------------------------------------
//  @summary Define 0x7A group of RDS feature 
//  @description
//      Be used to define 0x7A group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_7A     ( 7 * 2 + 0)
//---------------------------------------
//  @summary Define 0x7B group of RDS feature 
//  @description
//      Be used to define 0x7B group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_7B     ( 7 * 2 + 1)
//---------------------------------------
//  @summary Define 0x8A group of RDS feature 
//  @description
//      Be used to define 0x8A group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_8A     ( 8 * 2 + 0)
//---------------------------------------
//  @summary Define 0x8B group of RDS feature 
//  @description
//      Be used to define 0x8B group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_8B     ( 8 * 2 + 1)
//---------------------------------------
//  @summary Define 0x9A group of RDS feature 
//  @description
//      Be used to define 0x9A group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_9A     ( 9 * 2 + 0)
//---------------------------------------
//  @summary Define 0x9B group of RDS feature 
//  @description
//      Be used to define 0x9B group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_9B     ( 9 * 2 + 1)
//---------------------------------------
//  @summary Define 0x10A group of RDS feature 
//  @description
//      Be used to define 0x10A group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_10A    (10 * 2 + 0)
//---------------------------------------
//  @summary Define 0x10B group of RDS feature 
//  @description
//      Be used to define 0x10B group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_10B    (10 * 2 + 1)
//---------------------------------------
//  @summary Define 0x11A group of RDS feature 
//  @description
//      Be used to define 0x11A group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_11A    (11 * 2 + 0)
//---------------------------------------
//  @summary Define 0x11B group of RDS feature 
//  @description
//      Be used to define 0x11B group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_11B    (11 * 2 + 1)
//---------------------------------------
//  @summary Define 0x12A group of RDS feature 
//  @description
//      Be used to define 0x12A group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_12A    (12 * 2 + 0)
//---------------------------------------
//  @summary Define 0x12B group of RDS feature 
//  @description
//      Be used to define 0x12B group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_12B    (12 * 2 + 1)
//---------------------------------------
//  @summary Define 0x13A group of RDS feature 
//  @description
//      Be used to define 0x13A group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_13A    (13 * 2 + 0)
//---------------------------------------
//  @summary Define 0x13B group of RDS feature 
//  @description
//      Be used to define 0x13B group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_13B    (13 * 2 + 1)
//---------------------------------------
//  @summary Define 0x14A group of RDS feature 
//  @description
//      Be used to define 0x14A group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_14A    (14 * 2 + 0)
//---------------------------------------
//  @summary Define 0x14B group of RDS feature 
//  @description
//      Be used to define 0x14B group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_14B    (14 * 2 + 1)
//---------------------------------------
//  @summary Define 0x15A group of RDS feature 
//  @description
//      Be used to define 0x15A group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_15A    (15 * 2 + 0)
//---------------------------------------
//  @summary Define 0x15B group of RDS feature 
//  @description
//      Be used to define 0x15B group of RDS feature
//  @note
//---------------------------------------
#define RDS_TYPE_15B    (15 * 2 + 1)
//---------------------------------------
//  @summary Define PI of RDS feature 
//  @description
//      Be used to define programme identification(PI) of RDS feature 
//  @note
//---------------------------------------
#define RDS_PI        0x00
//---------------------------------------
//  @summary Define PTY of RDS feature 
//  @description
//      Be used to define programme type code(PTY) of RDS feature 
//  @note
//---------------------------------------
#define RDS_PTY     0x01
//---------------------------------------
//  @summary Define TP of RDS feature 
//  @description
//      Be used to define traffic programme (TP) of RDS feature 
//  @note
//---------------------------------------
#define RDS_TP       0x02
//---------------------------------------
//  @summary Define PS of RDS feature 
//  @description
//      Be used to define programme service name (PS) of RDS feature 
//  @note
//---------------------------------------
#define RDS_PS       0x03
//---------------------------------------
//  @summary Define AF of RDS feature 
//  @description
//      Be used to define alternative frequency code (AF) of RDS feature 
//  @note
//---------------------------------------
#define RDS_AF       0x04
//---------------------------------------
//  @summary Define TA of RDS feature 
//  @description
//      Be used to define traffic announcement code (TA) of RDS feature 
//  @note
//---------------------------------------
#define RDS_TA       0x05
//---------------------------------------
//  @summary Define DI of RDS feature 
//  @description
//      Be used to define decoder identification (DI) of RDS feature 
//  @note
//---------------------------------------
#define RDS_DI        0x06
//---------------------------------------
//  @summary Define MS of RDS feature 
//  @description
//      Be used to define music speech of RDS feature 
//  @note
//---------------------------------------
#define RDS_MS       0x07
//---------------------------------------
//  @summary Define RT of RDS feature 
//  @description
//      Be used to define radio text of RDS feature 
//  @note
//---------------------------------------
#define RDS_RT        0x08
//---------------------------------------
//  @summary Define EON of RDS feature 
//  @description
//      Be used to define enhance other network information of RDS feature 
//  @note
//---------------------------------------
#define RDS_EON     0x09
//---------------------------------------
//  @summary Define CT of RDS feature 
//  @description
//      Be used to define date and clock of RDS feature 
//  @note
//---------------------------------------
#define RDS_CT        0x0A
//---------------------------------------
//  @summary Define ECC of RDS feature 
//  @description
//      Be used to define Extended country code (ECC) of RDS feature 
//  @note
//---------------------------------------
#define RDS_ECC      0x0B
//---------------------------------------
//  @summary Define PTYN of RDS feature 
//  @description
//      Be used to define programme type name (PTYN) of RDS feature 
//  @note
//---------------------------------------
#define RDS_PTYN    0x0C
//---------------------------------------
//  @summary define a new type for record program type
//  @description
//      This enum be used to record the program type
//  @note
//---------------------------------------
enum RDS_PTYCode{
        RDS_PTY_NONE,
        RDS_PTY_NEWS,
        RDS_PTY_AFFAIRS,
        RDS_PTY_INFO,
        RDS_PTY_SPORT,
        RDS_PTY_EDUCATE,
        RDS_PTY_DRAMA,
        RDS_PTY_CULTURE,
        RDS_PTY_SCIENCE,
        RDS_PTY_VARIED,
        RDS_PTY_POPM,
        RDS_PTY_ROCKM,
        RDS_PTY_EASYM,
        RDS_PTY_LIGHTM,
        RDS_PTY_CLASSICS,
        RDS_PTY_OTHERM,
        RDS_PTY_Weather,
        RDS_PTY_FINANCE,
        RDS_PTY_CHIDREN,
        RDS_PTY_SOCIAL,
        RDS_PTY_RELIGION,
        RDS_PTY_PHONE,
        RDS_PTY_TRAVEL,
        RDS_PTY_LEISURE,
        RDS_PTY_JAZZ,
        RDS_PTY_COUNTRY,
        RDS_PTY_NATION,
        RDS_PTY_OLDIES,
        RDS_PTY_FOLKM,
        RDS_PTY_DOC,
        RDS_PTY_TEST,
        RDS_PTY_ALARM,        
};
//---------------------------------------
//  @summary Define count for check valid times
//  @description
//      Be used to check PTY, PI, ECC valid value
//  @note
//---------------------------------------
#define RDS_VALIDATE_LIMIT 4 
#endif //#ifdef SUPPORT_RADIO_RDS
//*********************************************************************************************************
//  @summary    Be used to delay time for iic interface 
//  @param  bMilliSec   Be used to delay time
//  @return None
//  @retval None
//  @description
//      This external function be used to delay time for iic interface
//  @bug    None
//  @history 2007.04.18 first version   
//  @note     
//*********************************************************************************************************
extern void Delay_mS(BYTE bMilliSec);
WORD wRADIODRVTunerReg[16]; //Be used to store registers of si4703
BYTE bRADIODRVTmp;//Be used to temporary variable
DWORD dwRadioDrvTime; //Be used to store time to avoid system hang
 
#ifdef SUPPORT_RADIO_RDS
WORD wRADIODRV_PI;//Be used to store PI code for RDS feature
BYTE   bRADIODRV_PTY;//Be used to store PTY code for RDS feature
BYTE   bRADIODRV_CountPI;//Be used to count PI code to confirm valid value
BYTE   bRADIODRV_CountPTY;//Be used to count PTY code to confirm valid value
BYTE   bRADIODRV_CountPS;//Be used to count PS code to confirm valid value
BYTE   bRADIODRV_CountECC;//Be used to count ECC code to confirm valid value
BYTE   bRADIODRV_CountPTYN;//Be used to count PTYN code to confirm valid value
BYTE   bRADIODRV_TP;//Be used to store traffic programme (TP) code for RDS feature 
BYTE   bRADIODRV_TA;//Be used to store  traffic announcenent (TA) code for RDS feature
BYTE   bRADIODRV_DI;//Be used to store  decoder identification(DI) code for RDS feature
BYTE   bRADIODRV_ECC;//Be used to store  Extended country code(ECC) code for RDS feature
//
BYTE   bRADIODRV_RDS_PS[9];//Be used to save program service name which has 8 characters from RDS spec.
BYTE   bRADIODRV_RDS_PTY[10]; //Be used to save program type code which has 8 characters from RDS spec. And bRADIODRV_RDS_PTY[0] be used to recored length of PTY
BYTE   bRADIODRV_RDS_RT[65];//Be used to save RADIO TEXT which has 64 characters from RDS spec and bRADIODRV_RDS_RT[0] be used to record the length of RT
BYTE   bRADIODRV_RDS_RTLen;//Be used to count length of radio text
BYTE   bRADIODRV_RDS_PTYN[9];//Be used to save Programme Type Name and bRADIODRV_RDS_PTYN[0] be used to record the length of PTYN
BYTE   bRADIODRV_RDS_flag;//Be used to A/B flag of  radio text which change new radio text 
BYTE   bRADIODRV_RDS_RTFirst;//Be used to save flag which first segment of radio text has been received  
BYTE   bRADIODRV_RDS_RTEnd;//Be used to save flag which total radio text has been received
WORD wRADIODRV_RDS_RT;// Be used to check valid radio text of value
WORD wRADIODRV_RDS_RTmp;//Be used to check valid radio text of value

//---------------------------------------
//  @summary define a new datatype for date and clock
//  @description
//      This structure be used to define date and clock
//  @note
//---------------------------------------
typedef struct tagRDS_CT
{
    WORD wRDS_CTYear;
    BYTE bRDS_CTMonth;
    BYTE bRDS_CTDate;
    BYTE bRDS_WEEK;
    BYTE bRDS_CTHour;
    BYTE bRDS_CTMin;   
    
} RDS_CTime, *PRDS_CTime;

#endif //#ifdef SUPPORT_RADIO_RDS
//*********************************************************************************************************
//  @summary    Control the IIC SCL as High
//  @param  None
//  @return None
//  @retval None
//  @description
//      This function be used to control SCL pin of IIC as High state
//  @bug    None
//  @history 2007.04.18 first version   
//  @note   
//*********************************************************************************************************
void _RADIODRV_IIC_SCLHIGH(void);
//*********************************************************************************************************
//  @summary    Control the IIC SCL as Low
//  @param  None
//  @return None
//  @retval None
//  @description
//      This function be used to control SCL pin of IIC as low state
//  @bug    None
//  @history 2007.04.18 first version   
//  @note     
//*********************************************************************************************************
void _RADIODRV_IIC_SCLLOW(void);
//*********************************************************************************************************
//  @summary    Control the IIC SDA as HIGH
//  @param  None
//  @return None
//  @retval None
//  @description
//      This function be used to control SCL pin of IIC as HIGH state
//  @bug    None
//  @history 2007.04.18 first version   
//  @note     
//*********************************************************************************************************
void _RADIODRV_IIC_SDAHIGH(void);
//*********************************************************************************************************
//  @summary    Control the IIC SDA as LOW
//  @param  None
//  @return None
//  @retval None
//  @description
//      This function be used to control SCL pin of IIC as LOW state
//  @bug    None
//  @history 2007.04.18 first version   
//  @note      
//*********************************************************************************************************
void _RADIODRV_IIC_SDALOW(void);
//*********************************************************************************************************
//  @summary    Send the start signal of IIC at SCL/SDA pin
//  @param  None
//  @return None
//  @retval None
//  @description
//      This function be used to send start signal at SCL/SDA pin 
//  @bug    None
//  @history 2007.04.18 first version   
//  @note      
//*********************************************************************************************************
void _RADIODRV_IIC_START(void);
//*********************************************************************************************************
//  @summary    Send the stop signal of IIC at SCL/SDA pin
//  @param  None
//  @return None
//  @retval None
//  @description
//      This function be used to send stop signal at SCL/SDA pin 
//  @bug    None
//  @history 2007.04.18 first version   
//  @note     
//*********************************************************************************************************
void _RADIODRV_IIC_STOP(void);
//*********************************************************************************************************
//  @summary    Get the ACK signal from other device via SCL/SDA
//  @param  None
//  @return Make sure the ack signal
//  @retval TRUE    get the ack signal successfully
//  @retval FALSE   get the ack signal fail
//  @description
//      This function be used to get the ack signal from other device via SCL/SDA 
//  @bug    None
//  @history 2007.04.18 first version   
//  @note  
//*********************************************************************************************************
BOOL _RADIODRV_IIC_GetACK(void);
//*********************************************************************************************************
//  @summary    To set the ACK signal to other device via SCL/SDA
//  @param  bACK    Send ack signal. if bACK as TRUE, the low signal of sda pin. otherwise, the high signal of sda pin
//  @return None
//  @retval None
//  @description
//      This function be used to set the ack signal to other device via SCL/SDA 
//  @bug    None
//  @history 2007.04.18 first version   
//  @note    
//*********************************************************************************************************
void _RADIODRV_IIC_SETACK(BOOL bACK) ;
//*********************************************************************************************************
//  @summary    Write byte data to radio device via SCL/SDA pin
//  @param  bData   Desired write byte
//  @return Make sure the write byte status
//  @retval TRUE    Write byte successfully
//  @retval FALSE   Write byte fail
//  @description
//      This function be used to write registers of radio tuner to control radio device by IIC interface 
//  @bug    None
//  @history 2007.04.18 first version   
//  @note  
//*********************************************************************************************************
BOOL _RADIODRV_WriteBYTE(BYTE bData);
//*********************************************************************************************************
//  @summary    Read byte data to radio device via SCL/SDA pin
//  @param  bData   Desired read byte
//  @return Make sure the read byte status
//  @retval TRUE    read byte successfully
//  @retval FALSE   read byte fail
//  @description
//      This function be used to read registers of radio tuner to control radio device by IIC interface 
//  @bug    None
//  @history 2007.04.18 first version   
//  @note    
//*********************************************************************************************************
BYTE _RADIODRV_ReadBYTE(BYTE bACK);
//*********************************************************************************************************
//  @summary    Write register of si4703 radio tuner which size of word to control si4703 radio tuner
//  @param  bLastReg   desired writes last register address
//  @param  wRegArray  desired writes register data
//  @return Make sure that write registers status
//  @retval TRUE    write Si4703 radio tuner successfully
//  @retval FALSE   write Si4703 radio tuner fail
//  @description
//      This function be used to write registers of radio tuner which word of size to control si4703 radio tuner 
//  @bug    None
//  @history 2007.04.18 first version   
//  @note    
//*********************************************************************************************************
BYTE _RADIODRV_WriteReg(BYTE bLastReg,WORD* wRegArray) ;
//*********************************************************************************************************
//  @summary    Read register of si4703 radio tuner which size of word to control si4703 radio tuner
//  @param  bLastReg   desired read last register address
//  @param  wRegArray  desired read register data
//  @return Make sure that read registers status
//  @retval TRUE    read Si4703 radio tuner successfully
//  @retval FALSE   read Si4703 radio tuner fail
//  @description
//      This function be used to read registers of radio tuner which word of size to control si4703 radio tuner 
//  @bug    None
//  @history 2007.04.18 first version   
//  @note    
//*********************************************************************************************************
BYTE _RADIODRV_ReadReg(BYTE bLastReg, WORD* wRegArray);
//*********************************************************************************************************
//  @summary    Reset the radio tuner
//  @param  None
//  @return None
//  @retval None
//  @description
//      This function be used to reset the radio tuner for si4703 
//  @bug    None
//  @history 2007.04.18 first version   
//  @note   
//*********************************************************************************************************
void RADIODRV_Reset(void); 
//*********************************************************************************************************
//  @summary    This function be used to power up radio tuner of si4703
//  @param  None
//  @return None
//  @retval None
//  @description
//      This function be used to power-up for radio tuner of si4703 
//  @bug    None
//  @history 2007.04.18 first version   
//  @note  
//*********************************************************************************************************
void RADIODRV_PowerUp(void); 
//*********************************************************************************************************
//  @summary    This function be used to power down radio tuner of si4703
//  @param  None
//  @return None
//  @retval None
//  @description
//      This function be used to power-down for radio tuner of si4703 
//  @bug    None
//  @history 2007.04.18 first version   
//  @note    
//*********************************************************************************************************
void RADIODRV_PowerDown(void); 
//*********************************************************************************************************
//  @summary    This function be used to convert frequency to channel
//  @param  wFreq   Desired frequency which needs to convert to channel
//  @return Number of channel 
//  @retval Number of channel which user desires to convert frequency
//  @description
//      This function be used to convert frequency to channel 
//  @bug    None
//  @history 2007.04.18 first version   
//  @note     
//*********************************************************************************************************
WORD _RADIODRV_FreqToChan(WORD wFreq);
//*********************************************************************************************************
//  @summary    This function be used to convert channel to frequency
//  @param  wChan   Desired channel which needs to convert to frequency
//  @return Frequency 
//  @retval Frequency which user desires to convert channel
//  @description
//      This function be used to convert channel to frequency 
//  @bug    None
//  @history 2007.04.18 first version   
//  @note    
//*********************************************************************************************************
WORD _RADIODRV_ChanToFreq(WORD wChan);
//*********************************************************************************************************
//  @summary    This function be used to tune radio station
//  @param  wRADIODRVfreq   desired tune frequency
//  @return status which be tuned frequency 
//  @retval TRUE    Tune frequency is sucessful
//  @retval FALSE   Tune frequency is un-sucessful
//  @description
//      This function be used to tune frequency 
//  @bug    None
//  @history 2007.04.18 first version   
//  @note     
//*********************************************************************************************************
BYTE RADIODRV_tune(WORD wFMfreq);
//*********************************************************************************************************
//  @summary    This function be used to control volume of radio tuner 
//  @param  bVol   Desire to setting volume steps
//  @return None
//  @retval None
//  @description
//      This function be used to set volume of radio tuner 
//  @bug    None
//  @history 2007.04.18 first version   
//  @note   
//*********************************************************************************************************
void RADIODRV_SetVol (BYTE bVol);
//*********************************************************************************************************
//  @summary    This function be used to set mute for volume
//  @param  bmute   bmute as TRUE which disable mute,otherwise enalbe mute
//  @return None
//  @retval None
//  @description
//      This function be used to mute volume of radio  
//  @bug    None
//  @history 2007.04.18 first version   
//  @note     
//*********************************************************************************************************
void RADIODRV_SetMute (BYTE bMute);
//*********************************************************************************************************
//  @summary    This function be used to initiate radio tuner 
//  @param  None
//  @return Initial status of radio tuner
//  @retval None
//  @description
//      This function be used to initiate radio tuner
//  @bug    None
//  @history 2007.04.18 first version   
//  @note      
//*********************************************************************************************************
BYTE RADIODRV_Init(void); 
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
//  @history 2007.04.18 first version   
//  @note     
//*********************************************************************************************************
BYTE _RADIODRV_GetChipID(void);
//*********************************************************************************************************
//  @summary    This function be used to get frequency
//  @param  None
//  @return Frequency of current station
//  @retval frequency value
//  @description
//      This function be used to get frequency of current station
//  @bug    None
//  @history 2007.04.18 first version   
//  @note      
//*********************************************************************************************************
WORD RADIODRV_GetFreq(void);
//*********************************************************************************************************
//  @summary    This function be used to get RSSI value
//  @param  None
//  @return Get RSSI of current station
//  @retval RSSI value
//  @description
//      This function be used to get RSSI(Received Signal Strength Indicator) value of current station
//  @bug    None
//  @history 2007.04.18 first version   
//  @note   
//*********************************************************************************************************
BYTE RADIODRV_GetRSSI(void);
//*********************************************************************************************************
//  @summary    This function be used to get volume value
//  @param  None
//  @return Get volume of current setting value 
//  @retval Volume value
//  @description
//      This function be used to get volume of current setting value
//  @bug    None
//  @history 2007.04.18 first version   
//  @note   
//*********************************************************************************************************
BYTE RADIODRV_GetVol(void);
//*********************************************************************************************************
//  @summary    This function be used to get mute status of volume
//  @param  None
//  @return Get mute status of volume
//  @retval TRUE    Enable Mute function
//  @retval FALSE   DISABLE Mute function
//  @description
//      This function be used to get mute status of volume
//  @bug    None
//  @history 2007.04.18 first version   
//  @note    
//*********************************************************************************************************
BYTE RADIODRV_GetMute(void);
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
//  @history 2007.04.18 first version   
//  @note   
//*********************************************************************************************************
BYTE RADIODRV_GetBand(void);
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
//  @history 2007.04.18 first version   
//  @note     
//*********************************************************************************************************
BYTE RADIODRV_GetChanSpace(void);
//*********************************************************************************************************
//  @summary    This function be used to set channel space
//  @param  bChanSpace   Desire to set channel space. 0x00 for 200KHZ, 0x01 for 100KHZ and 0x02 for 50KHZ
//  @return None
//  @retval None
//  @description
//      This function be used to set channel space
//  @bug    None
//  @history 2007.04.18 first version   
//  @note      
//*********************************************************************************************************
void RADIODRV_SETChanSpace(BYTE bChanSpace);
//*********************************************************************************************************
//  @summary    This function be used to set band
//  @param  bRADIODRVband   Desire to set band value. 0x00 for 87.5~108Mhz, 0x01 for 76~108Mhz and 0x02 for 76~90Mhz
//  @return None
//  @retval None
//  @description
//      This function be used to set band
//  @bug    None
//  @history 2007.04.18 first version   
//  @note     
//*********************************************************************************************************
void RADIODRV_SETBand(BYTE bFMband); 
//*********************************************************************************************************
//  @summary    This function be used to seek station for radio tuner control
//  @param  _bSeekDir   if _bSeekDir as TRUN, the seek up direction, else _bSeekDir as FALSE, the seek down direction
//  @return Get the seek status
//  @retval TRUE    Seek station successfully
//  @retval FALSE   Seek station fail
//  @description
//      This function be used to seek station for radio tuner control
//  @bug    None
//  @history 2007.04.18 first version   
//  @note     
//*********************************************************************************************************
BYTE _RADIODRV_SEEKAction(BYTE _bSeekDir);
//*********************************************************************************************************
//  @summary    This function be used to seek station
//  @param  bSeekDIR   if _bSeekDir as TRUN, the seek up direction, else _bSeekDir as FALSE, the seek down direction
//  @return Get the seek status
//  @retval TRUE    Seek station successfully
//  @retval FALSE   Seek station fail
//  @description
//      This function be used to seek station
//  @bug    None
//  @history 2007.04.18 first version   
//  @note  
//*********************************************************************************************************
BYTE RADIODRV_Seek(BYTE bSeekDIR); 
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
//  @history 2007.04.18 first version   
//  @note  
//*********************************************************************************************************
BYTE RADIODRV_AutoScan(WORD wFreq,WORD* wPreSetArray, BYTE bCount, BYTE bFLAG);
//*********************************************************************************************************
//  @summary    This function be used to initiate RDS functions 
//  @param  None
//  @return None
//  @retval None
//  @description
//      This function be used to initiate RDS functions
//  @bug    None
//  @history 2007.04.18 first version   
//  @note  
//*********************************************************************************************************
void RADIODRV_RDSInit(void);
//*********************************************************************************************************
//  @summary    This function be used to check RDS type from on-air 
//  @param  bRDS_TYPE   User needs to get RDS type form on-air
//  @return Get the status of the RDS type 
//  @retval TRUE    Support RDS tpye of the on-air
//  @retval FALSE   Don't support RDS type of the on-air
//  @description
//      This function be used to check RDS type from on-air
//  @bug    None
//  @history 2007.04.18 first version   
//  @note     
//*********************************************************************************************************
BYTE RADIODRV_RDSTYPE(BYTE bRDS_TYPE);
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
//  @history 2007.04.18 first version   
//  @note  
//*********************************************************************************************************
BYTE RADIODRV_UpdateRDS(BYTE bRDS_Type, BYTE* pbRDString);
//*********************************************************************************************************
//  @summary    This function be used to access PI code form the broadcasting
//  @param  pbRDS_PI   User needs to get PI code information
//  @return Get the status of the PI code
//  @retval TRUE    Get the PI code successfully
//  @retval FALSE   Get the PI code fail
//  @description
//      This function be used to access PI code from the broadcasting
//  @bug    None
//  @history 2007.04.18 first version   
//  @note    
//*********************************************************************************************************
BYTE RADIODRV_RDS_PI(BYTE* pbRDS_PI );
//*********************************************************************************************************
//  @summary    This function be used to access ECC info. from the broadcasting
//  @param  pbRDS_ECC   User needs to access ECC info. from the broadcasting
//  @return Get the status of the ECC code
//  @retval TRUE    Get the ECC code successfully
//  @retval FALSE   Get the ECC code fail
//  @description
//      This function be used to access ECC code from the broadcasting
//  @bug    None
//  @history 2007.04.18 first version   
//  @note  
//*********************************************************************************************************
BYTE RADIODRV_RDS_ECC(BYTE* pbRDS_ECC );
//*********************************************************************************************************
//  @summary    This function be used to access PTY info. from the broadcasting
//  @param  pbRDS_ECC   User needs to access PTY info. from the broadcasting
//  @return Get the status of the PTY info.
//  @retval TRUE    Get the PTY code successfully
//  @retval FALSE   Get the PTY code fail
//  @description
//      This function be used to access PTY code from the broadcasting
//  @bug    None
//  @history 2007.04.18 first version   
//  @note     
//*********************************************************************************************************
BYTE RADIODRV_RDS_PTY(BYTE* pbRDS_PTY);
//*********************************************************************************************************
//  @summary    This function be used to access PS info. from the broadcasting
//  @param  pbRDS_PS    User needs to access PS info. from the broadcasting
//  @return Get the status of the PS info.
//  @retval TRUE    Get the PS code successfully
//  @retval FALSE   Get the PS code fail
//  @description
//      This function be used to access PS code from the broadcasting
//  @bug    None
//  @history 2007.04.18 first version   
//  @note  
//*********************************************************************************************************
BYTE RADIODRV_RDS_PS(BYTE* pbRDS_PS);
//*********************************************************************************************************
//  @summary    This function be used to access TP info. from the broadcasting
//  @param  None
//  @return Get the status of the TP info.
//  @retval TRUE    Get the TP code successfully
//  @retval FALSE   Get the TP code fail
//  @description
//      This function be used to access TP code from the broadcasting
//  @bug    None
//  @history 2007.04.18 first version   
//  @note   
//*********************************************************************************************************
BYTE RADIODRV_RDS_TP(void);
//*********************************************************************************************************
//  @summary    This function be used to access TA info. from the broadcasting
//  @param  None
//  @return Get the status of the TA info.
//  @retval TRUE    Get the TA code successfully
//  @retval FALSE   Get the TA code fail
//  @description
//      This function be used to access TA code from the broadcasting
//  @bug    None
//  @history 2007.04.18 first version   
//  @note   
//*********************************************************************************************************
BYTE RADIODRV_RDS_TA(void);
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
//  @history 2007.04.18 first version   
//  @note  
//*********************************************************************************************************
BYTE RADIODRV_RDS_DI(void);
//*********************************************************************************************************
//  @summary    This function be used to access RT info. from the broadcasting
//  @param  pbRDS_RT    Desired display radio text
//  @return Get the status of the RT info.
//  @retval TRUE    Get the RT info. successfully
//  @retval FALSE   Get the RT info. fail
//  @description
//      This function be used to access radio text from the broadcasting.
//  @bug    None
//  @history 2007.04.18 first version   
//  @note     
//*********************************************************************************************************
BYTE RADIODRV_RDS_RT(BYTE* pbRDS_RT);
//*********************************************************************************************************
//  @summary    This function be used to access CT info. from the broadcasting
//  @param  pbRDS_CT    Desired display date and time
//  @return Get the status of the CT info.
//  @retval TRUE    Get the CT info. successfully
//  @retval FALSE   Get the CT info. fail
//  @description
//      This function be used to access clock and time from the broadcasting.
//  @bug    None
//  @history 2007.04.18 first version   
//  @note  
//*********************************************************************************************************
BYTE RADIODRV_RDS_CT(BYTE* pbRDS_CT);
//*********************************************************************************************************
//  @summary    This function be used to access PTYN info. from the broadcasting
//  @param  pbRDS_PTYN    Desired display PTYN
//  @return Get the status of the PTYN info.
//  @retval TRUE    Get the PTYN info. successfully
//  @retval FALSE   Get the PTYN info. fail
//  @description
//      This function be used to access programme type information from the broadcasting.
//  @bug    None
//  @history 2007.04.18 first version   
//  @note     
//*********************************************************************************************************
BYTE RADIODRV_RDS_PTYN(BYTE* pbRDS_PTYN);
//*********************************************************************************************************
//  @summary    This function be used to get RDS blocks from the broadcasting
//  @param  pwRDSBlock    Get the blocks of RDS data from the broadcastings
//  @return Get the status of the blocks of RDS info.
//  @retval TRUE    Get the block of RDS info. successfully
//  @retval FALSE   Get the block of RDS info. fail
//  @description
//      This function be used to access blocks of RDS infor. from the broadcasting.
//  @bug    None
//  @history 2007.04.18 first version   
//  @note    
//*********************************************************************************************************
BYTE RADIODRV_GetRDSBlock(WORD* pwRDSBlock);
//BYTE RADIODRV_RDS_RT(BYTE* pbRDS_RT);

#ifdef __cplusplus
   }
#endif

#endif  // __HDEVICE_H__
