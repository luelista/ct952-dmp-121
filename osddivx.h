
#ifndef __OSDDIVX_H__
#define __OSDDIVX_H__ 

#ifndef NO_DNR

typedef enum tagDialogType
{
    DLG_REG_CODE,
    DLG_AUTHOR_ERROR,
    DLG_RENTAL_EXPIRED,
    DLG_RENTAL_CONFIRM
} DIVX_DLG_TYPE;

extern    BOOL    OSDDIVX_DrawDialog(DIVX_DLG_TYPE bDlgType);
extern    BYTE    OSDDIVX_ProcessKey(void);
extern    void    OSDDIVX_SelectOKButton(void);
extern    void    OSDDIVX_SelectCancelButton(void);

#endif

#endif
