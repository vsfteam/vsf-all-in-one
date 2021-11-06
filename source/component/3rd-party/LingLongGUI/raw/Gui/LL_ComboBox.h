#ifndef _LL_COMBOBOX_H_
#define _LL_COMBOBOX_H_

#ifdef __cplusplus
extern "C" {
#endif



#include "LL_General.h"
#include "LL_Handler.h"
#include "LL_Characters.h"

typedef struct
{
    LLGENERAL_ATTRIBUTES;
    bool isExpand:1;
    bool isImageStyle:1;
    llFontLib *fontLibInfo;
    llColor textColor;
    llColor bgColor;
    llColor listBgColor;
    llColor selectBgColor;
    int16_t itemHeight;
    uint8_t *textList[LL_COMBOBOX_ITEM_MAX];
    uint8_t itemLenMax;
    uint8_t selectNum;
    uint8_t itemCount;
}llComboBox;

llComboBox* llComboBoxQuickCreate(uint16_t nameId, uint16_t parentNameId,int16_t x, int16_t y, int16_t width, int16_t height,
                                  llFontLib *fontLib,llColor textColor,llColor bgColor,llColor listBgColor,llColor selectBgColor,
                                  uint8_t defaultSelectNum,bool isHidden);
llComboBox* llComboBoxCreate(uint16_t nameId, uint16_t parentNameId, 
                          int16_t x, int16_t y, int16_t width, int16_t height,
                          llFontLib *fontLib,bool isHidden);
void pComboBoxAddItem(llComboBox *widget,uint8_t *itemText);
void nComboBoxAddItem(uint16_t nameId,uint8_t *text);
void pComboBoxSetEnabled(llComboBox *widget, bool state);
void nComboBoxSetEnabled(uint16_t nameId, bool state);

#ifdef __cplusplus
}
#endif

#endif //_LL_COMBOBOX_H_
