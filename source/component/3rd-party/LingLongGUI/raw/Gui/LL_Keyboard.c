/*
 * Copyright 2021 Ou Jianbo 59935554@qq.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * additional license
 * If you use this software to write secondary development type products,
 * must be released under GPL compatible free software license or commercial license.
*/

#include "LL_Keyboard.h"
#include "LL_Handler.h"
#include "LL_Linked_List.h"
#include "LL_LineEdit.h"
#include "LL_Background.h"
#include "LL_Window.h"
#include "LL_Button.h"
#include "asciiDefine.h"

//#define KEY_SPACE                         0x20  //空格
//#define KEY_EXCLAMATION_MARK              0x21	 // !
//#define KEY_QUOTATION_MARK                0x22     // "
//#define KEY_NUMBER_SIGN                   0x23   // #
//#define KEY_DOLLAR_SIGN                   0x24  // $
//#define KEY_PERCENT_SIGN                  0x25  // %
//#define KEY_AMPERSAND                     0x26  // &
//#define KEY_APOSTROPHE                    0x27  // '
//#define KEY_LEFT_ROUND_BRACKET            0x28  // (
//#define KEY_RIGHT_ROUND_BRACKET           0x29  // )
//#define KEY_ASTERISK                  0x2A  // *
//#define KEY_ADD                               0x2B  // +
//#define KEY_COMMA                               0x2C  // ,
//#define KEY_SUB                               0x2D  // -
//#define KEY_DOT                               0x2E  // .
//#define KEY_FORWARD_SLASH                  0x2F  // /
//#define KEY_0                               0x30  // 0
//#define KEY_1                               0x31  // 1
//#define KEY_2                               0x32  // 2
//#define KEY_3                               0x33  // 3
//#define KEY_4                               0x34  // 4
//#define KEY_5                               0x35  // 5
//#define KEY_6                               0x36  // 6
//#define KEY_7                               0x37  // 7
//#define KEY_8                               0x38  // 8
//#define KEY_9                               0x39  // 9
//#define KEY_COLON                               0x3A  // :
//#define KEY_SEMICOLON                  0x3B  // ;
//#define KEY_LESS_THAN_SIGN                  0x3C  // <
//#define KEY_EQUALS_SIGN                  0x3D  // =
//#define KEY_GREATER_THAN_SIGN                   0x3E  // >
//#define KEY_QUESTION_MARK                  0x3F  // ?
//#define KEY_AT_SIGN                  0x40  // @
//#define KEY_A                  0x41  //
//#define KEY_B                  0x42  //
//#define KEY_C                  0x43  //
//#define KEY_D                  0x44  //
//#define KEY_E                  0x45  //
//#define KEY_F                  0x46  //
//#define KEY_G                  0x47  //
//#define KEY_H                  0x48  //
//#define KEY_I                  0x49  //
//#define KEY_J                  0x4A  //
//#define KEY_K                  0x4B  //
//#define KEY_L                  0x4C  //
//#define KEY_M                  0x4D  //
//#define KEY_N                  0x4E  //
//#define KEY_O                  0x4F  //
//#define KEY_P                  0x50  //
//#define KEY_Q                  0x51  //
//#define KEY_R                  0x52  //
//#define KEY_S                  0x53  //
//#define KEY_T                  0x54  //
//#define KEY_U                  0x55  //
//#define KEY_V                  0x56  //
//#define KEY_W                  0x57  //
//#define KEY_X                  0x58  //
//#define KEY_Y                  0x59  //
//#define KEY_Z                  0x5A  //
//#define KEY_LEFT_SQUARE_BRACKET     0x5B  // [
//#define KEY_BACKSLASH     0x5C  // \ 反斜杠
//#define KEY_RIGHT_SQUARE_BRACKET     0x5D  // ]
//#define KEY_CARET     0x5E  // ^
//#define KEY_UNDERSCORE     0x5F  // _
//#define KEY_SINGLE_QUOTATION_MARK    0x60  // `
//#define KEY_a     0x61  //
//#define KEY_b     0x62  //
//#define KEY_c     0x63  //
//#define KEY_d     0x64  //
//#define KEY_e     0x65  //
//#define KEY_f     0x66  //
//#define KEY_g     0x67  //
//#define KEY_h     0x68  //
//#define KEY_i     0x69  //
//#define KEY_j     0x6A  //
//#define KEY_k     0x6B  //
//#define KEY_l     0x6C  //
//#define KEY_m     0x6D  //
//#define KEY_n     0x6E  //
//#define KEY_o     0x6F  //
//#define KEY_p     0x70  //
//#define KEY_q     0x71  //
//#define KEY_r     0x72  //
//#define KEY_s     0x73  //
//#define KEY_t     0x74  //
//#define KEY_u     0x75  //
//#define KEY_v     0x76  //
//#define KEY_w     0x77  //
//#define KEY_x     0x78  //
//#define KEY_y     0x79  //
//#define KEY_z     0x7A  //
//#define KEY_LEFT_FRENCH_BRACKET     0x7B  //{
//#define KEY_VERTICAL_BAR     0x7C  //|
//#define KEY_RIGHT_FRENCH_BRACKET     0x7D  //}
//#define KEY_TILDE     0x7E  //~

#define KEY_CLEAN          0
#define KEY_ALL_CLEAN          1
#define KEY_ADD_SUB     2

//16位id  页数占高6位,0-63页，id占低10位,0-1023个
//60-63 为内置默认使用
//60 0xF000
//61 0xF400
//62 0xF800
//63 0xFC00

#define DEF_ID_BTN_0          0xFC00
#define DEF_ID_BTN_1          0xFC01
#define DEF_ID_BTN_2          0xFC02
#define DEF_ID_BTN_3          0xFC03
#define DEF_ID_BTN_4          0xFC04
#define DEF_ID_BTN_5          0xFC05
#define DEF_ID_BTN_6          0xFC06
#define DEF_ID_BTN_7          0xFC07
#define DEF_ID_BTN_8          0xFC08
#define DEF_ID_BTN_9          0xFC09
#define DEF_ID_BTN_A          0xFC0A
#define DEF_ID_BTN_B          0xFC0B
#define DEF_ID_BTN_C          0xFC0C
#define DEF_ID_BTN_D          0xFC0D
#define DEF_ID_BTN_E          0xFC0E
#define DEF_ID_BTN_F          0xFC0F
#define DEF_ID_BTN_G          0xFC10
#define DEF_ID_BTN_H          0xFC11
#define DEF_ID_BTN_I          0xFC12
#define DEF_ID_BTN_J          0xFC13
#define DEF_ID_BTN_K          0xFC14
#define DEF_ID_BTN_L          0xFC15
#define DEF_ID_BTN_M          0xFC16
#define DEF_ID_BTN_N          0xFC17
#define DEF_ID_BTN_O          0xFC18
#define DEF_ID_BTN_P          0xFC19
#define DEF_ID_BTN_Q          0xFC1A
#define DEF_ID_BTN_R          0xFC1B
#define DEF_ID_BTN_S          0xFC1C
#define DEF_ID_BTN_T          0xFC1D
#define DEF_ID_BTN_U          0xFC1E
#define DEF_ID_BTN_V          0xFC1F
#define DEF_ID_BTN_W          0xFC20
#define DEF_ID_BTN_X          0xFC21
#define DEF_ID_BTN_Y          0xFC22
#define DEF_ID_BTN_Z          0xFC23

#define DEF_ID_BTN_CLOSE      0xFC24
#define DEF_ID_BTN_CLEAR      0xFC25
#define DEF_ID_BTN_AC         0xFC26
#define DEF_ID_BTN_ADD_SUB    0xFC27
#define DEF_ID_BTN_DOT        0xFC28

#define DEF_ID_BTN_SHIFT      0xFC29
#define DEF_ID_BTN_SPACE      0xFC2A

#define DEF_ID_BTN_ENTER      0xFC2B

#define DEF_ID_KEYBOARD        0xFF29

typedef struct
{
    void* targetWidget;
    bool hasDecimalPoint;
} keyboardInfo;

bool slotKeyboardNumClose(llConnectInfo info)
{
    llWindow *win;
    win=(llWindow*)info.receiver;
    llGeneral * rootWidget;

    rootWidget=llListGetRootWidget();

    nWindowDelete(DEF_ID_KEYBOARD);

    if(rootWidget->geometry.y!=0)
    {
        pBackgroundMove((llBackground*)rootWidget,0,0);
    }

    llCharStopCursorBlink();
    return true;
}

bool slotKeyboardNumEntry(llConnectInfo info)
{
    llGeneral *targetWidget;
    llWindow *win;
    
    win=(llWindow*)info.receiver;
    targetWidget=(llGeneral*)(((keyboardInfo*)(win->userInfo))->targetWidget);
    llEmitSignal((llLineEdit*)(targetWidget),SIGNAL_EIDTING_FINISHED);
    return slotKeyboardNumClose(info);
}

bool slotKeyboardNumHide(llConnectInfo info)
{
    llWindow *win;
    win=(llWindow*)info.receiver;
    llGeneral * rootWidget;

    rootWidget=llListGetRootWidget();

    if(rootWidget->geometry.y!=0)
    {
        pBackgroundMove((llBackground*)rootWidget,0,0);
    }

    pWindowSetHiden(win,true);
    llCharStopCursorBlink();
    return false;
}

bool slotKeyboardNomalInput(llConnectInfo info)
{
    llWindow *win;
    llButton *btn;
    llGeneral *targetWidget;
    uint32_t textLength;

    win=(llWindow*)info.receiver;
    btn=(llButton*)info.sender;

    targetWidget=(llGeneral*)(((keyboardInfo*)(win->userInfo))->targetWidget);
    if(targetWidget->widgetType==widgetTypeLineEdit)
    {
        textLength=strlen((const char*)((llLineEdit*)targetWidget)->textInfo.text);
        if(textLength<(((llLineEdit*)targetWidget)->textMaxLength-1))
        {
            ((llLineEdit*)targetWidget)->textInfo.text[textLength]=btn->keyValue;
            ((llLineEdit*)targetWidget)->textInfo.text[textLength+1]=0;

            ((llLineEdit*)targetWidget)->refreshFunc(targetWidget->nameId);
            llEmitSignal((llLineEdit*)(targetWidget),SIGNAL_VALUE_CHANGED);
        }
    }
    return false;
}

bool slotKeyboardInputSP(llConnectInfo info)
{
    llWindow *win;
    llButton *btn;
    llGeneral *targetWidget;
    uint32_t textLength;
    uint16_t i;

    win=(llWindow*)info.receiver;
    btn=(llButton*)info.sender;

    targetWidget=(llGeneral*)(((keyboardInfo*)(win->userInfo))->targetWidget);
    if(targetWidget->widgetType==widgetTypeLineEdit)
    {
        textLength=strlen((const char*)((llLineEdit*)targetWidget)->textInfo.text);
        switch(btn->keyValue)
        {
        case KEY_CLEAN:
        {

            if(textLength>0)
            {

                if(((keyboardInfo*)(win->userInfo))->hasDecimalPoint==true)
                {
                    if(((llLineEdit*)targetWidget)->textInfo.text[textLength-1]==ASCII_PERIOD)
                    {
                        ((keyboardInfo*)(win->userInfo))->hasDecimalPoint=false;
                    }
                }

                //英文 0x0-0x7F
                if(((llLineEdit*)targetWidget)->textInfo.text[textLength-1]<0x80)
                {
                    ((llLineEdit*)targetWidget)->textInfo.text[textLength-1]=0;
                }
                else
                {
                    ((llLineEdit*)targetWidget)->textInfo.text[textLength-1]=0;
                    ((llLineEdit*)targetWidget)->textInfo.text[textLength-2]=0;
                    ((llLineEdit*)targetWidget)->textInfo.text[textLength-3]=0;
                }

//                ((llLineEdit*)targetWidget)->textInfo.text[textLength-1]=0;
            }
        }
        break;
        case KEY_ALL_CLEAN:
        {
            memset(((llLineEdit*)targetWidget)->textInfo.text,0,textLength);
            ((keyboardInfo*)(win->userInfo))->hasDecimalPoint=false;
        }
        break;
        case KEY_ADD_SUB:
        {

            if(((llLineEdit*)targetWidget)->textInfo.text[0]==ASCII_SUB_SIGN)//负数
            {
                if(textLength>0)
                {
                    for (i=0; i<textLength; i++)
                    {
                        ((llLineEdit*)targetWidget)->textInfo.text[i]=((llLineEdit*)targetWidget)->textInfo.text[i+1];
                    }
                    ((llLineEdit*)targetWidget)->textInfo.text[textLength]=0;
                }
            }
            else
            {
                if((textLength>0)&&(textLength<(((llLineEdit*)targetWidget)->textMaxLength-2)))//添加负号
                {
                    for (i=textLength; i>0; i--)
                    {
                        ((llLineEdit*)targetWidget)->textInfo.text[i]=((llLineEdit*)targetWidget)->textInfo.text[i-1];
                    }
                    ((llLineEdit*)targetWidget)->textInfo.text[0]=ASCII_SUB_SIGN;
                }
            }


        }
        break;

        case ASCII_PERIOD:
        {
            if(((keyboardInfo*)(win->userInfo))->hasDecimalPoint!=true)
            {
                if(textLength<(((llLineEdit*)targetWidget)->textMaxLength-2))
                {
                    ((llLineEdit*)targetWidget)->textInfo.text[textLength]=btn->keyValue;
                    ((llLineEdit*)targetWidget)->textInfo.text[textLength+1]=0;
                    ((keyboardInfo*)(win->userInfo))->hasDecimalPoint=true;
                }
            }
        }
        break;
        default:
            break;
        }
        ((llLineEdit*)targetWidget)->refreshFunc(targetWidget->nameId);
        llEmitSignal((llLineEdit*)(targetWidget),SIGNAL_VALUE_CHANGED);
    }
    return false;
}



#define X_POS(x)   (int16_t)(widthStart+widthOneSize*8*x)
#define Y_POS(y)   (int16_t)(heightStart+heightOneSize*6*y)
#define W_SIZE     (int16_t)(widthOneSize*7)
#define H_SIZE     (int16_t)(heightOneSize*5)

void llKeyboardNumCreate(void* targetWidget, int16_t x, int16_t y, int16_t width, int16_t height,llFontLib *fontLib)
{
    float widthStart,widthOneSize,heightStart,heightOneSize;
    llWindow * window;
    void * userInfo;
    llButton * btn;
    llGeneral * rootWidget;
    int16_t keyboardX,keyboardY;
    llPoint targetPos;

    rootWidget=llListGetRootWidget();
    //需要获取全局坐标
    targetPos=llListGetGlobalPos(targetWidget);

    //判断输入位置坐标在下屏
    if(targetPos.y>(cfgMonitorHeight/2))
    {
        //上移显示
        pBackgroundMove((llBackground*)rootWidget,0,-(cfgMonitorHeight/2));

        keyboardX=x;
        keyboardY=y+rootWidget->geometry.y+cfgMonitorHeight;
    }
    else
    {
        keyboardX=x;
        keyboardY=y;
    }

    window=llWindowCreate(DEF_ID_KEYBOARD,rootWidget->nameId,keyboardX,keyboardY,width,height,false);
    userInfo=(void*)llMalloc(sizeof (keyboardInfo));
    if((window!=NULL)&&(userInfo!=NULL))
    {
        window->userInfo=userInfo;

        ((keyboardInfo*)(window->userInfo))->targetWidget=targetWidget;
        ((keyboardInfo*)(window->userInfo))->hasDecimalPoint=false;

        pWindowSetColor(window,RGB_CONVERT(215,216,220));

        // 4*5布局
        // 横向 6 42 6 42 6 42 6 42 6 -> 1:7:1:7:1:7:1:7:1 = 33
        // 纵向 6 29 6 29 6 29 6 29 6 29 6 -> 1:5:1:5:1:5:1:5:1:5:1 = 31

        widthOneSize=width/33.0;
        heightOneSize=height/31.0;
        widthStart=widthOneSize;
        heightStart=heightOneSize;

        btn=llButtonCreate(DEF_ID_BTN_CLOSE,DEF_ID_KEYBOARD,X_POS(3),Y_POS(0),W_SIZE,H_SIZE,(uint8_t *)"x",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));

        btn=llButtonCreate(DEF_ID_BTN_1,DEF_ID_KEYBOARD,X_POS(0),Y_POS(1),W_SIZE,H_SIZE,(uint8_t *)"1",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_1);
        btn=llButtonCreate(DEF_ID_BTN_2,DEF_ID_KEYBOARD,X_POS(1),Y_POS(1),W_SIZE,H_SIZE,(uint8_t *)"2",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_2);
        btn=llButtonCreate(DEF_ID_BTN_3,DEF_ID_KEYBOARD,X_POS(2),Y_POS(1),W_SIZE,H_SIZE,(uint8_t *)"3",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_3);
        btn=llButtonCreate(DEF_ID_BTN_CLEAR,DEF_ID_KEYBOARD,X_POS(3),Y_POS(1),W_SIZE,H_SIZE,(uint8_t *)"<-",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(170,173,178),RGB_CONVERT(188,190,197));
        pButtonSetKeyValue(btn,KEY_CLEAN);


        btn=llButtonCreate(DEF_ID_BTN_4,DEF_ID_KEYBOARD,X_POS(0),Y_POS(2),W_SIZE,H_SIZE,(uint8_t *)"4",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_4);
        btn=llButtonCreate(DEF_ID_BTN_5,DEF_ID_KEYBOARD,X_POS(1),Y_POS(2),W_SIZE,H_SIZE,(uint8_t *)"5",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_5);
        btn=llButtonCreate(DEF_ID_BTN_6,DEF_ID_KEYBOARD,X_POS(2),Y_POS(2),W_SIZE,H_SIZE,(uint8_t *)"6",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_6);
        btn=llButtonCreate(DEF_ID_BTN_AC,DEF_ID_KEYBOARD,X_POS(3),Y_POS(2),W_SIZE,H_SIZE,(uint8_t *)"AC",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(170,173,178),RGB_CONVERT(188,190,197));
        pButtonSetKeyValue(btn,KEY_ALL_CLEAN);


        btn=llButtonCreate(DEF_ID_BTN_7,DEF_ID_KEYBOARD,X_POS(0),Y_POS(3),W_SIZE,H_SIZE,(uint8_t *)"7",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_7);
        btn=llButtonCreate(DEF_ID_BTN_8,DEF_ID_KEYBOARD,X_POS(1),Y_POS(3),W_SIZE,H_SIZE,(uint8_t *)"8",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_8);
        btn=llButtonCreate(DEF_ID_BTN_9,DEF_ID_KEYBOARD,X_POS(2),Y_POS(3),W_SIZE,H_SIZE,(uint8_t *)"9",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_9);


        btn=llButtonCreate(DEF_ID_BTN_ADD_SUB,DEF_ID_KEYBOARD,X_POS(0),Y_POS(4),W_SIZE,H_SIZE,(uint8_t *)"+/-",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,KEY_ADD_SUB);
        btn=llButtonCreate(DEF_ID_BTN_0,DEF_ID_KEYBOARD,X_POS(1),Y_POS(4),W_SIZE,H_SIZE,(uint8_t *)"0",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_0);
        btn=llButtonCreate(DEF_ID_BTN_DOT,DEF_ID_KEYBOARD,X_POS(2),Y_POS(4),W_SIZE,H_SIZE,(uint8_t *)".",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_PERIOD);
        btn=llButtonCreate(DEF_ID_BTN_ENTER,DEF_ID_KEYBOARD,X_POS(3),Y_POS(4),W_SIZE,H_SIZE,(uint8_t *)"Enter",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_CR);


        //动作


        llConnectSignal(DEF_ID_BTN_0,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_1,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_2,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_3,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_4,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_5,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_6,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_7,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_8,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_9,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);

        llConnectSignal(DEF_ID_BTN_DOT,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardInputSP);
        llConnectSignal(DEF_ID_BTN_CLEAR,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardInputSP);
        llConnectSignal(DEF_ID_BTN_AC,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardInputSP);
        llConnectSignal(DEF_ID_BTN_ADD_SUB,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardInputSP);
        llConnectSignal(DEF_ID_BTN_CLOSE,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNumClose);
        llConnectSignal(DEF_ID_BTN_ENTER,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNumEntry);
    }
    else
    {
        llFree(window);
        llFree(userInfo);
    }
}

#define X_POS_QWERTY(x)   (int16_t)(widthStart+widthOneSize*6*x)
#define Y_POS_QWERTY(y)   (int16_t)(heightStart+heightOneSize*9*y)
#define W_SIZE_QWERTY     (int16_t)(widthOneSize*10)
#define H_SIZE_QWERTY     (int16_t)(heightOneSize*7)

void llKeyboardQwertyCreate(void* targetWidget, int16_t x, int16_t y, int16_t width, int16_t height,llFontLib *fontLib)
{
    float widthStart,widthOneSize,heightStart,heightOneSize;
    llWindow * window;
    void * userInfo;
    llButton * btn;
    llGeneral * rootWidget;
    int16_t keyboardX,keyboardY;
    llPoint targetPos;

    rootWidget=llListGetRootWidget();
    //需要获取全局坐标
    targetPos=llListGetGlobalPos(targetWidget);

    //判断输入位置坐标在下屏
    if(targetPos.y>(cfgMonitorHeight/2))
    {
        //上移显示
        pBackgroundMove((llBackground*)rootWidget,0,-(cfgMonitorHeight/2));

        keyboardX=x;
        keyboardY=y+rootWidget->geometry.y+cfgMonitorHeight;
    }
    else
    {
        keyboardX=x;
        keyboardY=y;
    }
    window=llWindowCreate(DEF_ID_KEYBOARD,rootWidget->nameId,keyboardX,keyboardY,width,height,false);
    userInfo=(void*)llMalloc(sizeof (keyboardInfo));
    if((window!=NULL)&&(userInfo!=NULL))
    {
        window->userInfo=userInfo;

        ((keyboardInfo*)(window->userInfo))->targetWidget=targetWidget;
        ((keyboardInfo*)(window->userInfo))->hasDecimalPoint=false;



        pWindowSetColor(window,RGB_CONVERT(215,216,220));

        // 10*6布局
        // 横向 1:10:2:10:2:10:2:10:2:10:2:10:2:10:2:10:2:10:2:10:1 = 120
        // 纵向 1:7:2:7:2:7:2:7:2:7:2:7:1 = 54

        widthOneSize=width/120.0;
        heightOneSize=height/54.0;
        widthStart=widthOneSize;
        heightStart=heightOneSize;

        btn=llButtonCreate(DEF_ID_BTN_CLOSE,DEF_ID_KEYBOARD,X_POS_QWERTY(18),Y_POS_QWERTY(0),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"x",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        
        btn=llButtonCreate(DEF_ID_BTN_1 ,DEF_ID_KEYBOARD,X_POS_QWERTY(0),Y_POS_QWERTY(1),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"1",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_1);
        btn=llButtonCreate(DEF_ID_BTN_2 ,DEF_ID_KEYBOARD,X_POS_QWERTY(2),Y_POS_QWERTY(1),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"2",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_2);
        btn=llButtonCreate(DEF_ID_BTN_3 ,DEF_ID_KEYBOARD,X_POS_QWERTY(4),Y_POS_QWERTY(1),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"3",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_3);
        btn=llButtonCreate(DEF_ID_BTN_4 ,DEF_ID_KEYBOARD,X_POS_QWERTY(6),Y_POS_QWERTY(1),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"4",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_4);
        btn=llButtonCreate(DEF_ID_BTN_5 ,DEF_ID_KEYBOARD,X_POS_QWERTY(8),Y_POS_QWERTY(1),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"5",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_5);
        btn=llButtonCreate(DEF_ID_BTN_6 ,DEF_ID_KEYBOARD,X_POS_QWERTY(10),Y_POS_QWERTY(1),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"6",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_6);
        btn=llButtonCreate(DEF_ID_BTN_7 ,DEF_ID_KEYBOARD,X_POS_QWERTY(12),Y_POS_QWERTY(1),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"7",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_7);
        btn=llButtonCreate(DEF_ID_BTN_8 ,DEF_ID_KEYBOARD,X_POS_QWERTY(14),Y_POS_QWERTY(1),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"8",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_8);
        btn=llButtonCreate(DEF_ID_BTN_9 ,DEF_ID_KEYBOARD,X_POS_QWERTY(16),Y_POS_QWERTY(1),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"9",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_9);
        btn=llButtonCreate(DEF_ID_BTN_0 ,DEF_ID_KEYBOARD,X_POS_QWERTY(18),Y_POS_QWERTY(1),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"0",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_0);
        
        btn=llButtonCreate(DEF_ID_BTN_Q ,DEF_ID_KEYBOARD,X_POS_QWERTY(0),Y_POS_QWERTY(2),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"Q",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_Q);
        btn=llButtonCreate(DEF_ID_BTN_W ,DEF_ID_KEYBOARD,X_POS_QWERTY(2),Y_POS_QWERTY(2),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"W",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_W);
        btn=llButtonCreate(DEF_ID_BTN_E ,DEF_ID_KEYBOARD,X_POS_QWERTY(4),Y_POS_QWERTY(2),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"E",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_E);
        btn=llButtonCreate(DEF_ID_BTN_R ,DEF_ID_KEYBOARD,X_POS_QWERTY(6),Y_POS_QWERTY(2),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"R",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_R);
        btn=llButtonCreate(DEF_ID_BTN_T ,DEF_ID_KEYBOARD,X_POS_QWERTY(8),Y_POS_QWERTY(2),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"T",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_T);
        btn=llButtonCreate(DEF_ID_BTN_Y ,DEF_ID_KEYBOARD,X_POS_QWERTY(10),Y_POS_QWERTY(2),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"Y",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_Y);
        btn=llButtonCreate(DEF_ID_BTN_U ,DEF_ID_KEYBOARD,X_POS_QWERTY(12),Y_POS_QWERTY(2),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"U",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_U);
        btn=llButtonCreate(DEF_ID_BTN_I ,DEF_ID_KEYBOARD,X_POS_QWERTY(14),Y_POS_QWERTY(2),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"I",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_I);
        btn=llButtonCreate(DEF_ID_BTN_O ,DEF_ID_KEYBOARD,X_POS_QWERTY(16),Y_POS_QWERTY(2),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"O",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_O);
        btn=llButtonCreate(DEF_ID_BTN_P ,DEF_ID_KEYBOARD,X_POS_QWERTY(18),Y_POS_QWERTY(2),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"P",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_P);
        
        btn=llButtonCreate(DEF_ID_BTN_A ,DEF_ID_KEYBOARD,X_POS_QWERTY(1),Y_POS_QWERTY(3),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"A",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_A);
        btn=llButtonCreate(DEF_ID_BTN_S ,DEF_ID_KEYBOARD,X_POS_QWERTY(3),Y_POS_QWERTY(3),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"S",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_S);
        btn=llButtonCreate(DEF_ID_BTN_D ,DEF_ID_KEYBOARD,X_POS_QWERTY(5),Y_POS_QWERTY(3),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"D",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_D);
        btn=llButtonCreate(DEF_ID_BTN_F ,DEF_ID_KEYBOARD,X_POS_QWERTY(7),Y_POS_QWERTY(3),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"F",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_F);
        btn=llButtonCreate(DEF_ID_BTN_G ,DEF_ID_KEYBOARD,X_POS_QWERTY(9),Y_POS_QWERTY(3),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"G",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_G);
        btn=llButtonCreate(DEF_ID_BTN_H ,DEF_ID_KEYBOARD,X_POS_QWERTY(11),Y_POS_QWERTY(3),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"H",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_H);
        btn=llButtonCreate(DEF_ID_BTN_J ,DEF_ID_KEYBOARD,X_POS_QWERTY(13),Y_POS_QWERTY(3),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"J",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_J);
        btn=llButtonCreate(DEF_ID_BTN_K ,DEF_ID_KEYBOARD,X_POS_QWERTY(15),Y_POS_QWERTY(3),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"K",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_K);
        btn=llButtonCreate(DEF_ID_BTN_L ,DEF_ID_KEYBOARD,X_POS_QWERTY(17),Y_POS_QWERTY(3),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"L",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_L);
        
        btn=llButtonCreate(DEF_ID_BTN_SHIFT ,DEF_ID_KEYBOARD,X_POS_QWERTY(0),Y_POS_QWERTY(4),W_SIZE_QWERTY*1.3,H_SIZE_QWERTY,(uint8_t *)"↑",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        
        btn=llButtonCreate(DEF_ID_BTN_Z ,DEF_ID_KEYBOARD,X_POS_QWERTY(3),Y_POS_QWERTY(4),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"Z",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_Z);
        btn=llButtonCreate(DEF_ID_BTN_X ,DEF_ID_KEYBOARD,X_POS_QWERTY(5),Y_POS_QWERTY(4),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"X",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_X);
        btn=llButtonCreate(DEF_ID_BTN_C ,DEF_ID_KEYBOARD,X_POS_QWERTY(7),Y_POS_QWERTY(4),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"C",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_C);
        btn=llButtonCreate(DEF_ID_BTN_V ,DEF_ID_KEYBOARD,X_POS_QWERTY(9),Y_POS_QWERTY(4),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"V",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_V);
        btn=llButtonCreate(DEF_ID_BTN_B ,DEF_ID_KEYBOARD,X_POS_QWERTY(11),Y_POS_QWERTY(4),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"B",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_B);
        btn=llButtonCreate(DEF_ID_BTN_N ,DEF_ID_KEYBOARD,X_POS_QWERTY(13),Y_POS_QWERTY(4),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"N",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_N);
        btn=llButtonCreate(DEF_ID_BTN_M ,DEF_ID_KEYBOARD,X_POS_QWERTY(15),Y_POS_QWERTY(4),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)"M",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,ASCII_M);
        
        btn=llButtonCreate(DEF_ID_BTN_CLEAR ,DEF_ID_KEYBOARD,X_POS_QWERTY(17)+W_SIZE_QWERTY*0.3,Y_POS_QWERTY(4),W_SIZE_QWERTY*1.3,H_SIZE_QWERTY,(uint8_t *)"<-",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        pButtonSetKeyValue(btn,KEY_CLEAN);
        
//        btn=llButtonCreate(DEF_ID_BTN_DOT ,DEF_ID_KEYBOARD,X_POS_QWERTY(5),Y_POS_QWERTY(5),W_SIZE_QWERTY,H_SIZE_QWERTY,(uint8_t *)".",fontLib,false);
//        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
//        pButtonSetKeyValue(btn,ASCII_PERIOD);
        btn=llButtonCreate(DEF_ID_BTN_SPACE ,DEF_ID_KEYBOARD,X_POS_QWERTY(7),Y_POS_QWERTY(5),widthOneSize*34,H_SIZE_QWERTY,(uint8_t *)"space",fontLib,false);
        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
//        btn=llButtonCreate(DEF_ID_BTN_DOT ,DEF_ID_KEYBOARD,X_POS_QWERTY(15),Y_POS_QWERTY(5),widthOneSize*28,H_SIZE_QWERTY,(uint8_t *)"Enter",fontLib,false);
//        pButtonSetBackgroundColor(btn,RGB_CONVERT(190,193,197),RGB_CONVERT(255,255,255));
        
        
        
        
        llConnectSignal(DEF_ID_BTN_0,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_1,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_2,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_3,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_4,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_5,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_6,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_7,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_8,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_9,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        
        llConnectSignal(DEF_ID_BTN_A,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_B,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_C,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_D,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_E,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_F,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_G,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_H,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_I,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_J,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_K,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_L,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_M,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_N,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_O,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_P,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_Q,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_R,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_S,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_T,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_U,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_V,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_W,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_X,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_Y,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        llConnectSignal(DEF_ID_BTN_Z,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNomalInput);
        
        llConnectSignal(DEF_ID_BTN_CLEAR,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardInputSP);
        llConnectSignal(DEF_ID_BTN_CLOSE,SIGNAL_CLICK_RELEASED,DEF_ID_KEYBOARD,slotKeyboardNumClose);
}
    else
    {
        llFree(window);
        llFree(userInfo);
    }
}
