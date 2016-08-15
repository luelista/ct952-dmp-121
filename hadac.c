#include "winav.h"
#include "ctkav.h"
#include "hal.h"   // LLY2.17, since using HAL_WriteGPIO()
#include "hadac.h"
void    _GetSampleFreq(BYTE bDACType, BYTE bSampleFreq, BYTE bBitResolution);
WORD    _wSampleRate;    // keep sample rate for specify Audio DAC
WORD    _wBitResolution; // keep bit resolution for specify Audio DAC
BYTE    __bHALDeEmphasis=FALSE; //rename __bHAL1_Deemphasis as __bHALDeEmphasis

// Big modification history
// LLY2.15, using general macro to control Audio DAC related pin


// LLY2.15, add macro to control Audio related pin action.
// [1] ADAC_CS6CH pin High/ Low action
#define ADAC_CS6CH_HI()     {HAL_WriteGPIO(ADAC_CS6CH_GPIO_GRP, PIN_ADAC_CS6CH, 1);}
#define ADAC_CS6CH_LO()     {HAL_WriteGPIO(ADAC_CS6CH_GPIO_GRP, PIN_ADAC_CS6CH, 0);}

// [2] ADAC_CS2CH pin High/ Low action
#define ADAC_CS2CH_HI()     {HAL_WriteGPIO(GPIO_A, PIN_ADAC_CS2CH, 1);}
#define ADAC_CS2CH_LO()     {HAL_WriteGPIO(GPIO_A, PIN_ADAC_CS2CH, 0);}

// [3] ADAC Clock pin High/ Low action
#define ADAC_SCK_HI()       {HAL_WriteGPIO(GPIO_A, PIN_ADAC_SCK, 1);}
#define ADAC_SCK_LO()       {HAL_WriteGPIO(GPIO_A, PIN_ADAC_SCK, 0);}

// [4] ADAC Data pin High/ Low action
#define ADAC_SDA_HI()       {HAL_WriteGPIO(GPIO_A, PIN_ADAC_SDA, 1);}
#define ADAC_SDA_LO()       {HAL_WriteGPIO(GPIO_A, PIN_ADAC_SDA, 0);}




//****************************************************************
// Description  :  Program Audio CLKs, resolution, and alignment.
// Reference    :  D100 setup_bitclk()
// Arguments    :  bSampleFreq: sampling freq
// Return       :  None
// Side Effect  :  None
//****************************************************************
void    _ProgramACLK(BYTE bSampleFreq) //test OK
{
    DWORD   dwSaveInt;

    // Define the table for getting N, M, P, K value.
    // LLY2.05, don't use the new table for CT909S
    // Since, E400 says CT909S and CT909 are same now.
    
    // LLY2.51, porting Chuan's code for CT909P IC ACLK setting.
    // LLY2.56, CT909G ACLK programming is same as CT909P
//#ifdef CT909P_IC_SYSTEM
#if defined(CT909P_IC_SYSTEM) || defined(CT909G_IC_SYSTEM)

#if ACLK_FSMF==ACLK_384FS
    WORD NMPKarr1[12][4] =  {   {63,8,0,125},  {97,24,0,50},   {31,4,0,75},   // 8, 11, 12
                                {127,24,0,45}, {97,24,0,25},   {63,14,0,25},   //16, 22, 24
                                {255,44,0,25}, {195,24,0,25}, {127,24,0,15},  //32, 44, 48
                                {511,74,0,15}, {391,124,0,5},  {255,74,0,5} }; //64, 88, 96
#else   // #if ACLK_FSMF==ACLK_384FS
    WORD NMPKarr1[12][4] =  {   {127,24,0,135},  {97,14,0,125},   {63,8,0,125},   // 8, 11, 12
                                {255,44,0,75},   {195,24,0,75},  {127,24,0,45},   //16, 22, 24
                                {511,74,0,45},  {391,74,0,25},  {255,44,0,25},   //32, 44, 48
                                {1023,124,0,27}, {783,124,0,15}, {511,74,0,15} }; //64, 88, 96
#endif  // #if ACLK_FSMF==ACLK_384FS

#else   // #if defined(CT909P_IC_SYSTEM) || defined(CT909G_IC_SYSTEM)

#if ACLK_FSMF==ACLK_384FS
    WORD NMPKarr1[12][4] =  {   {62,23,0,45},  {47,23,0,25},   {30,23,0,15},   // 8, 11, 12
                                {126,43,0,25}, {96,23,0,25},   {62,13,0,25},   //16, 22, 24
                                {254,73,0,15}, {390,123,0,10}, {126,23,0,15},  //32, 44, 48
                                {510,123,0,9}, {390,123,0,5},  {254,73,0,5} }; //64, 88, 96
#else   // #if ACLK_FSMF==ACLK_384FS
    WORD NMPKarr1[12][4] =  {   {126,23,0,135},  {96,23,0,75},   {62,23,0,45},   // 8, 11, 12
                                {254,73,0,45},   {194,73,0,25},  {126,23,0,45},   //16, 22, 24
                                {510,123,0,27},  {390,73,0,25},  {254,73,0,15},   //32, 44, 48
                                {1022,123,0,27}, {782,123,0,15}, {510,123,0,9} }; //64, 88, 96
#endif  // #if ACLK_FSMF==ACLK_384FS

#endif  // #if defined(CT909P_IC_SYSTEM) || defined(CT909G_IC_SYSTEM)

//          128     176     192
//	256     no      no      yes
//	384	    yes     no      no

    WORD NF;
    BYTE MF, PF, bclk_dac_df, aclk_dac_df, bclk_adc_df, aclk_adc_df;
    BYTE bIdx;

    switch (bSampleFreq)
    {
        case AUDIO_FREQ_8K:
            bIdx = 0;
            break;
        case AUDIO_FREQ_11K:
            bIdx = 1;
            break;
        case AUDIO_FREQ_12K:
            bIdx = 2;
            break;
        case AUDIO_FREQ_16K:
            bIdx = 3;
            break;
        case AUDIO_FREQ_22K:
            bIdx = 4;
            break;
        case AUDIO_FREQ_24K:
            bIdx = 5;
            break;
        case AUDIO_FREQ_32K:
            bIdx = 6;
            break;
        default:
        case AUDIO_FREQ_44K:
            bIdx = 7;
            break;
        case AUDIO_FREQ_48K:
            bIdx = 8;
            break;
        case AUDIO_FREQ_64K:
            bIdx = 9;
            break;
        case AUDIO_FREQ_88K:
            bIdx = 10;
            break;
        case AUDIO_FREQ_96K:
            bIdx = 11;
            break;
    }

    //aclk param
    NF = NMPKarr1[bIdx][0];
    MF = NMPKarr1[bIdx][1];
    PF = NMPKarr1[bIdx][2];
    aclk_dac_df = NMPKarr1[bIdx][3]-1;


#if ACLK_FSMF==ACLK_384FS
    bclk_adc_df = bclk_dac_df = 5; //0:64fs, 3:256fs, 5:384fs,
#else //ACLK_256FS
    bclk_adc_df = bclk_dac_df = 3; //0:64fs, 3:256fs, 5:384fs,
#endif

	aclk_adc_df = 0;  //bypass aclk to adc-aclk


    // Set aclk_dac3_df for internal audio dac
    {
    BYTE aclk_dac3_df;
#if  ACLK_FSMF==ACLK_384FS
    aclk_dac3_df = 2;
#else   // #if  ACLK_FSMF==ACLK_384FS
    aclk_dac3_df = 1;
#endif  // #if  ACLK_FSMF==ACLK_384FS

    // Set audio clock related bit[14:0] & [21:16] & [29:27].
    // So, read-back value and & 0xC7C08000 to clear those bit first
    REG_PLAT_CLK_FREQ_CONTROL1 = (REG_PLAT_CLK_FREQ_CONTROL1 & 0xC7C08000) |
                                    ((DWORD)bclk_dac_df<<16) |
                                    ((DWORD)bclk_adc_df<<19) |
                                    ((DWORD)aclk_dac_df) |
                                    ((DWORD)aclk_adc_df<<8) |
                                    ((DWORD)aclk_dac3_df<<27);

    // LLY2.51, porting Chuan's code for CT909P IC
    // LLY2.56, Porting Chuan's code since CT909G/ CT909P/ CT909R ACLK programming all different.
#ifdef CT909G_IC_SYSTEM
    REG_PLAT_APLL_CONTROL = (REG_PLAT_APLL_CONTROL & 0xfe200000) | (DWORD)NF | ((DWORD)MF<<11) | ((DWORD)PF<<19);
#endif  // #ifdef CT909G_IC_SYSTEM
#ifdef CT909P_IC_SYSTEM
    REG_PLAT_APLL_CONTROL = (REG_PLAT_APLL_CONTROL & 0xffa00000) | (DWORD)NF | ((DWORD)MF<<11) | ((DWORD)PF<<19);
#endif  // #ifdef CT909P_IC_SYSTEM

#ifdef CT909R_IC_SYSTEM
    // Set audio clock related bit[19:0] and bit[20] also set as 0 for internal ACLK
    REG_PLAT_APLL_CONTROL = (REG_PLAT_APLL_CONTROL & 0xffd00000) | (DWORD)NF | ((DWORD)MF<<11) | ((DWORD)PF<<18);
#endif  // #ifdef CT909R_IC_SYSTEM

    //enable Audio related clock, bit[22:18], value 0 means enable.
    OS_DISABLE_INTERRUPTS( dwSaveInt );
    REG_PLAT_CLK_GENERATOR_CONTROL &= 0xFF83FFFF;
    OS_RESTORE_INTERRUPTS( dwSaveInt) ;
    }

    //kevin0.69, not reset AIU 'cause AIU related registers will also be reset
}


//****************************************************************
// Description  :   Get sampling freq/bit resolution register settings for the specified ADAC
// Arguments    :   bDACType: ADAC type
//                  bSampleFreq: sampling freq
//                  bBitResolution: bit resoultion
// Return       :   _wSampleRate: sampling freq/bit resolution register setting
//                  _wBitResolution: bit resolution register setting
// Side Effect  :   None
//****************************************************************
void    _GetSampleFreq(BYTE bDACType, BYTE bSampleFreq, BYTE bBitResolution)
{
    switch(bDACType)
    {
    default:
    case    HAL_DAC_PCM1723:
    case    HAL_DAC_PCM1720:
        switch(bSampleFreq)
        {
        case AUDIO_FREQ_16K:
            _wSampleRate = 0x06A0;   // bit[7:4]=1010;
            break;
        case AUDIO_FREQ_32K:
            _wSampleRate = 0x0680;   // bit[7:4]=1000;
            break;
        case AUDIO_FREQ_64K:
            _wSampleRate = 0x0690;   // bit[7:4]=1001;
            break;
        case AUDIO_FREQ_24K:
            _wSampleRate = 0x0660;   // bit[7:4]=0110;
            break;
        default:
        case AUDIO_FREQ_48K:
            _wSampleRate = 0x0640;   // bit[7:4]=0100;  PCM1723 Register 3
            break;
        case AUDIO_FREQ_96K:
            _wSampleRate = 0x0650;   // bit[7:4]=0101;
            break;
        case AUDIO_FREQ_22K:
            _wSampleRate = 0x0620;   // bit[7:4]=0010;
            break;
        case AUDIO_FREQ_44K:
            _wSampleRate = 0x0600;   // bit[7:4]=0000;
            break ;
        case AUDIO_FREQ_88K:
            _wSampleRate = 0x0610;   // bit[7:4]=0001;
            break;
        }

        switch(bBitResolution)
        {
        default:
        case 16:
            _wBitResolution = 0x0520;   // bit[4:3]=00;
            break;
        case 20:
            _wBitResolution = 0x0528;   // bit[4:3]=01;
            break;
        case 24:
            _wBitResolution = 0x0530;   // bit[4:3]=10;
            break;
        }

#if ACLK_FSMF==ACLK_256FS
        _wSampleRate |= 0x0008;   // bit[3]=1 for 256fs; 0 for 384fs
#endif

#if (AUDIO_ALIGN_FORMAT == I2S_ALIGN)
        _wSampleRate |= 0x0001;   // bit[0]=0 for Right-aligned; 1 for I2S-aligned
#endif

#ifdef SUPPORT_AUDIO_DEEMPHASYS
        if (__bHALDeEmphasis)
        {
            __bHALDeEmphasis = FALSE;
            _wBitResolution |= 0x2;
        }
#endif

        break;
    case   HAL_DAC_WM8746:
        switch(bBitResolution)
        {
        default:
        case 16:
            _wBitResolution = 0x0600;
            break;
        case 20:
            _wBitResolution = 0x0610;
            break;
        case 24:
            _wBitResolution = 0x0620;
            break;
        }

#if (AUDIO_ALIGN_FORMAT == I2S_ALIGN)
            _wBitResolution |= 0x0002;   // bit[1:0]=0 for Right-aligned; 1 for Left-aligned; 2 for I2S-aligned
#elif (AUDIO_ALIGN_FORMAT == LEFT_ALIGN)
            _wBitResolution |= 0x0001;
#endif

#ifdef SUPPORT_AUDIO_DEEMPHASYS
        if (__bHALDeEmphasis)
        {
            _wSampleRate = 0x0522;   //use _wSampleRate as reg value to be written
        }
        else
#endif
        {
            _wSampleRate = 0x0520;
        }

        break ;

        case   HAL_DAC_W571C161:

		    switch(bBitResolution)
				{
			default:
			case 16:
			_wBitResolution = 0x0600;
			break;
			case 20:
			_wBitResolution = 0x0610;
			break;
			case 24:
			_wBitResolution = 0x0620;
			break;
				}

#if (AUDIO_ALIGN_FORMAT == I2S_ALIGN)
		_wBitResolution |= 0x0002;	 // bit[1:0]=0 for Right-aligned; 1 for Left-aligned; 2 for I2S-aligned
#elif (AUDIO_ALIGN_FORMAT == LEFT_ALIGN)
		_wBitResolution |= 0x0001;
#endif

	     _wSampleRate = 0x14C0;
	     break ;

    }
}



//****************************************************************
// Description  :   Program Audio DAC
// Arguments    :   bSampleFreq: sampling freq
//                  bBitResolution: bit resoultion
// Return       :   None
// Side Effect  :   None
//****************************************************************
// LLY2.15, remove ADAC pin define to "pio.h"
#if 0
#define PIN_ADAC_SCK        10
#define PIN_ADAC_SDA        13
#define PIN_ADAC_CS6CH      11 //5.1
#define PIN_ADAC_CS2CH      14 //2CH
#endif  // #if 0

// LLY1.50, add for CT909S internal audio DAC
// LLY2.17, used for internal ADAC programming.
#ifdef  ENABLE_INTERNAL_ADAC
void    _Program_Internal_ADAC(BYTE bSampleFreq, BYTE bBitResolution)
{
    // Nothing need to do since default setting is 24bit, I2S format.
}
#endif  // #ifdef  ENABLE_INTERNAL_ADAC



#if AUDIO_DAC == DAC_WM8746
void    _ProgramAudioDAC(BYTE bSampleFreq, BYTE bBitResolution) //SCK/SDA/CS shares w/ UART1/UART2/PD
{
    BYTE    bTemp;

    ///// WM8746 /////
    // Step 1: get sample rate
    _GetSampleFreq(HAL_DAC_WM8746, bSampleFreq, bBitResolution);

    // Step 3: set sample rate
    // Init Audio Dac
    ADAC_SCK_HI();
    ADAC_SDA_HI();
    ADAC_CS6CH_HI();
    ADAC_SCK_LO();
    ADAC_SCK_HI();

    // write Reg0000011
    for (bTemp=0; bTemp<16; bTemp++)
    {
        if ((_wBitResolution >> (15 - bTemp)) & 0x01)
        {
            ADAC_SDA_HI();
        }
        else
        {
            ADAC_SDA_LO();
        }
        ADAC_SCK_LO();
        ADAC_SCK_HI();
    }

    // Activate 8746
    ADAC_CS6CH_LO();
    ADAC_CS6CH_HI();

    // write Reg0000010
    for (bTemp=0; bTemp<16; bTemp++)
    {
        if ((_wSampleRate >> (15 - bTemp)) & 0x01)
        {
            ADAC_SDA_HI();
        }
        else
        {
            ADAC_SDA_LO();
        }
        ADAC_SCK_LO();
        ADAC_SCK_HI();
    }

    // Activate 8746
    ADAC_CS6CH_LO();
    ADAC_CS6CH_HI();
}
#endif //#if AUDIO_DAC == DAC_WM8746


#if AUDIO_DAC == DAC_GENERAL
void    _ProgramAudioDAC(BYTE bSampleFreq, BYTE bBitResolution)
{
    //no need to program
}
#endif //#if AUDIO_DAC == DAC_GENERAL


#if AUDIO_DAC == DAC_CE2752
void	_ProgramAudioDAC(BYTE bSampleFreq, BYTE bBitResolution)
{
	//no need to program
}
#endif //#if AUDIO_DAC == DAC_GENERAL


#if AUDIO_DAC == DAC_PCM1723 //kevin1.01
void    _ProgramAudioDAC(BYTE bSampleFreq, BYTE bBitResolution)
{
    BYTE    bTemp;

    ///// PCM1723 /////
    // Step 1: get sample rate
    _GetSampleFreq(HAL_DAC_PCM1723, bSampleFreq, bBitResolution);

    // Step 3: set sample rate
    // Init Audio Dac
    ADAC_SCK_HI();
    ADAC_SDA_HI();
    ADAC_CS2CH_HI();
    ADAC_SCK_LO();
    ADAC_SCK_HI();

    // write Reg2
    for (bTemp=0; bTemp<16; bTemp++)
    {
        if ((_wSampleRate >> (15 - bTemp)) & 0x01)
        {
            ADAC_SDA_HI();
        }
        else
        {
            ADAC_SDA_LO();
        }
        ADAC_SCK_LO();
        ADAC_SCK_HI();
    }

    // Activate 1723
    ADAC_CS2CH_LO();
    ADAC_CS2CH_HI();

    // write Reg3
    for (bTemp=0; bTemp<16; bTemp++)
    {
        if ((_wBitResolution >> (15 - bTemp)) & 0x01)
        {
            ADAC_SDA_HI();
        }
        else
        {
            ADAC_SDA_LO();
        }
        ADAC_SCK_LO();
        ADAC_SCK_HI();
    }

    // Activate 1723
    ADAC_CS2CH_LO();
    ADAC_CS2CH_HI();
}
#endif //#if AUDIO_DAC == DAC_PCM1723



#if AUDIO_DAC == DAC_PCM1723_WM8746
void    _ProgramAudioDAC(BYTE bSampleFreq, BYTE bBitResolution) //SCK/SDA/CS shares w/ UART1/UART2/PD
{
    BYTE    bTemp;

    ///// PCM1723 /////
    // Step 1: get sample rate
    _GetSampleFreq(HAL_DAC_PCM1723, bSampleFreq, bBitResolution);

    // Step 3: set sample rate
    // Init Audio Dac
    ADAC_SCK_HI();
    ADAC_SDA_HI();
    ADAC_CS2CH_HI();
    ADAC_SCK_LO();
    ADAC_SCK_HI();

    // write Reg2
    for (bTemp=0; bTemp<16; bTemp++)
    {
        if ((_wSampleRate >> (15 - bTemp)) & 0x01)
        {
            ADAC_SDA_HI();
        }
        else
        {
            ADAC_SDA_LO();
        }
        ADAC_SCK_LO();
        ADAC_SCK_HI();
    }

    // Activate 1723
    ADAC_CS2CH_LO();
    ADAC_CS2CH_HI();

    // write Reg3
    for (bTemp=0; bTemp<16; bTemp++)
    {
        if ((_wBitResolution >> (15 - bTemp)) & 0x01)
        {
            ADAC_SDA_HI();
        }
        else
        {
            ADAC_SDA_LO();
        }
        ADAC_SCK_LO();
        ADAC_SCK_HI();
    }

    // Activate 1723
    ADAC_CS2CH_LO();
    ADAC_CS2CH_HI();


    ///// WM8746 /////
    // Step 1: get sample rate
    _GetSampleFreq(HAL_DAC_WM8746, bSampleFreq, bBitResolution);

    // Step 3: set sample rate
    // Init Audio Dac
    ADAC_SCK_HI();
    ADAC_SDA_HI();
    ADAC_CS6CH_HI();
    ADAC_SCK_LO();
    ADAC_SCK_HI();

    // write Reg0000011
    for (bTemp=0; bTemp<16; bTemp++)
    {
        if ((_wBitResolution >> (15 - bTemp)) & 0x01)
        {
            ADAC_SDA_HI();
        }
        else
        {
            ADAC_SDA_LO();
        }
        ADAC_SCK_LO();
        ADAC_SCK_HI();
    }

    // Activate 8746
    ADAC_CS6CH_LO();
    ADAC_CS6CH_HI();

    // write Reg0000010
    for (bTemp=0; bTemp<16; bTemp++)
    {
        if ((_wSampleRate >> (15 - bTemp)) & 0x01)
        {
            ADAC_SDA_HI();
        }
        else
        {
            ADAC_SDA_LO();
        }
        ADAC_SCK_LO();
        ADAC_SCK_HI();
    }

    // Activate 8746
    ADAC_CS6CH_LO();
    ADAC_CS6CH_HI();
}
#endif //#if AUDIO_DAC == DAC_PCM1723_WM8746

#if AUDIO_DAC == DAC_W571C161 //Dennis Liu
void	_ProgramAudioDAC(BYTE bSampleFreq, BYTE bBitResolution)
{
	BYTE	bTemp;

	// Step 1: get bBitResolution
	_GetSampleFreq(HAL_DAC_W571C161, bSampleFreq, bBitResolution);

    ADAC_SCK_HI();
    ADAC_SDA_HI();
    ADAC_CS6CH_HI();
    ADAC_SCK_LO();
    ADAC_SCK_HI();

    ADAC_CS6CH_LO(); // PL CS

	// write R3
	for (bTemp=0; bTemp<16; bTemp++)
	{
		if ((_wBitResolution << bTemp) & 0x8000)
		{
            ADAC_SDA_HI();
		}
		else
		{
            ADAC_SDA_LO();
		}
        ADAC_SCK_LO();
        ADAC_SCK_HI();
	}

    ADAC_CS6CH_HI();

	// write R10
	ADAC_CS6CH_LO();
	for (bTemp=0; bTemp<16; bTemp++)
	{
		if ((_wSampleRate << bTemp) & 0x8000)
		{
            ADAC_SDA_HI();
		}
		else
		{
            ADAC_SDA_LO();
		}
        ADAC_SCK_LO();
        ADAC_SCK_HI();
	}
    ADAC_CS6CH_HI();

}
#endif //#if AUDIO_DAC == DAC_PCM1723
void	HAL_ADAC_Set(BYTE bSampleFreq, BYTE bBitResolution)
{
    // Micky2.16, don't call HAL_SetClock, it will destroy the module independency
    _ProgramACLK(bSampleFreq);

    // LLY2.17, program internal ADAC if use it.
#ifdef  ENABLE_INTERNAL_ADAC
    _Program_Internal_ADAC(bSampleFreq, SUPPORT_PCM_WORD_LENGTH);
#endif  // #ifdef  ENABLE_INTERNAL_ADAC

    // Program External ADAC
#ifndef USE_INTERNAL_ADAC_ONLY
    _ProgramAudioDAC(bSampleFreq, SUPPORT_PCM_WORD_LENGTH);
#endif  // #ifndef USE_INTERNAL_ADAC_ONLY

}



