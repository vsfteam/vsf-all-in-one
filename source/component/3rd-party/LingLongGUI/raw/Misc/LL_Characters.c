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

#include "LL_Characters.h"
#include "LL_Timer.h"
#include "LL_Handler.h"

//单字显示
void llCharShowChar(int16_t x, int16_t y,llGeometry showGeometry,uint8_t *str,llColor color)
{
    uint16_t i, j;
    llGeometry charGeometry;
    llPoint point;
    int All_Bit_Num=0;
    int Byte_Num=0;
    int Bit_Offset=0;
    uint8_t clolrLevel=0;
    llColor colorWhite;
    llColor colorPalette[4]= {0};
    uint8_t r,g,b;
    uint8_t wr,wg,wb;
    uint8_t offsetR,offsetG,offsetB;



    uint16_t width,height;
    uint8_t *buffer=NULL;
    bool isMalloc=false;
    uint8_t dataBits;

    buffer=llFontGetData(str,&dataBits,&width,&height,&isMalloc);  /* 取字模数据 */

    charGeometry.x=x;
    charGeometry.y=y;
    charGeometry.width=width;
    charGeometry.height=height;
    //计算文字是否超出显示区域

    if(llRectIsOverlap(showGeometry,charGeometry))//在区域内,需要显示
    {
        if(dataBits==2)
        {
            //计算色域
            colorWhite=(llColor)0xFFFFFFFF;//获得颜色最大值(白色)
            llGeometryGetR_G_B(color,&r,&g,&b);
            llGeometryGetR_G_B(colorWhite,&wr,&wg,&wb);
            offsetR=(wr-r)/3;
            offsetG=(wg-g)/3;
            offsetB=(wb-b)/3;

            colorPalette[3]=color;

            r=r+offsetR;
            g=g+offsetG;
            b=b+offsetB;

            colorPalette[2]=RGB_CONVERT(r,g,b);

            r=r+offsetR;
            g=g+offsetG;
            b=b+offsetB;

            colorPalette[1]=RGB_CONVERT(r,g,b);

            colorPalette[0]=colorWhite;
        }

        if(llRectIsFullIn(showGeometry,charGeometry))
        {
            for(i=0; i<height; i++)
            {
                for(j=0; j<width; j++)
                {
                    point.x=x + j;
                    point.y=y + i;
                    switch (dataBits)
                    {
                    case 1:
                    {
                        All_Bit_Num=i*width+j;
                        Byte_Num=All_Bit_Num/8;
                        Bit_Offset=All_Bit_Num%8;

                        //先取低位
                        if(GETBIT(buffer[Byte_Num],Bit_Offset))
                        {
                            llSetPoint( point.x, point.y, color );   /* 字符颜色 */
                        }
                        break;
                    }
                    case 2:
                    {
                        All_Bit_Num=i*width+j;
                        Byte_Num=All_Bit_Num/4;

                        Bit_Offset=All_Bit_Num%4;
                        Bit_Offset*=2;

                        clolrLevel=GETBIT(buffer[Byte_Num],Bit_Offset);
                        clolrLevel=clolrLevel<<1;
                        clolrLevel+=GETBIT(buffer[Byte_Num],Bit_Offset+1);

                        if(clolrLevel!=0)
                        {
                            llSetPoint( point.x, point.y, colorPalette[clolrLevel] );   /* 字符颜色 */
                        }
                        break;
                    }
                    default:
                        break;
                    }

                }
            }
        }
        else
        {
            for(i=0; i<height; i++)
            {
                for(j=0; j<width; j++)
                {
                    point.x=x + j;
                    point.y=y + i;
                    if(llPointInRect(point,showGeometry))
                    {
                        switch (dataBits)
                        {
                        case 1:
                        {
                            All_Bit_Num=i*width+j;
                            Byte_Num=All_Bit_Num/8;
                            Bit_Offset=All_Bit_Num%8;

                            //先取低位
                            if(GETBIT(buffer[Byte_Num],Bit_Offset))
                            {
                                llSetPoint( point.x, point.y, color );   /* 字符颜色 */
                            }
                            break;
                        }
                        case 2:
                        {
                            All_Bit_Num=i*width+j;
                            Byte_Num=All_Bit_Num/4;

                            Bit_Offset=All_Bit_Num%4;
                            Bit_Offset*=2;

                            clolrLevel=GETBIT(buffer[Byte_Num],Bit_Offset);
                            clolrLevel=clolrLevel<<1;
                            clolrLevel+=GETBIT(buffer[Byte_Num],Bit_Offset+1);

                            if(clolrLevel!=0)
                            {
                                llSetPoint( point.x, point.y, colorPalette[clolrLevel] );   /* 字符颜色 */
                            }
                            break;
                        }
                        default:
                            break;
                        }
                    }
                }
            }
        }
    }
    if(isMalloc)
    {
        llFree(buffer);
    }
}

//显示单行字符串
uint8_t *llCharShowLineChar(int16_t x, int16_t y,llGeometry showGeometry,uint8_t *str,uint16_t maxX,llColor color)
{
    uint16_t w;

    while(*str!=0)
    {
        w=llFontGetWidthSize(str);

        if((x+w)>maxX)
        {
            return str;
        }

        llCharShowChar(x, y,showGeometry,str,color);

        if(*str>=0x80)
        {
            //utf8 chinese use 3 bytes
            str++;
            str++;
        }
        str++;

        x=x+w;
        if(*str=='\n')
        {
            return str++;
        }
    }
    return 0;
}

llGeometry llCharGetCharPos(llChar* charInitStruct,uint8_t *str,uint16_t offsetStr)
{
    uint16_t w,h;
    uint16_t addWidth=0;
    llGeometry retGeometry={0};
    int16_t x, y;
    int16_t oldX;
    uint16_t offsetCount=0;
    x=charInitStruct->geometry.x;
    y=charInitStruct->geometry.y;
    oldX=x;
    while(*str!=0)
    {
        h= llFontGetHeightSize();
        w=llFontGetWidthSize(str);
        retGeometry.x=x;
        retGeometry.y=y;
        retGeometry.height=h;
        retGeometry.width=w;
        if(charInitStruct->isAutoLineBreak)
        {
            addWidth+=w;
            if(addWidth>charInitStruct->geometry.width)
            {
                addWidth=w;
                y=y+h;
                x=oldX;
            }
        }

        if(*str>=0x80)
        {
            //utf8 chinese use 3 bytes

            str++;
            str++;
            offsetCount++;
            offsetCount++;
        }
        str++;
        offsetCount++;

        x=x+w;
        if(*str=='\n')
        {
            y=y+h;
            x=oldX;
            str++;
        }
        if(offsetCount>offsetStr)
        {
            return retGeometry;
        }


    }
    return retGeometry;
}

void llCharSetText(int16_t x, int16_t y,llGeometry showGeometry, uint8_t *str,llColor Color,uint16_t maxWidth,bool isAutoLineBreak,bool isPassword)
{
    uint16_t w,h;
    int16_t oldX;
    uint8_t *buffer=NULL;
    uint8_t passwordStr='*';
    bool isMalloc=false;
    uint8_t dataBits;

    uint16_t addWidth=0;

    oldX=x;
    while(*str!=0)
    {
        buffer=llFontGetData(str,&dataBits,&w,&h,&isMalloc);  /* 取字模数据 */
        
        if(isAutoLineBreak)
        {
            addWidth+=w;
            if(addWidth>maxWidth)
            {
                addWidth=w;
                y=y+h;
                x=oldX;
            }
        }

        //英文 0x0-0x7F
        if(isPassword)
        {
            llCharShowChar(x, y,showGeometry,&passwordStr,Color);
        }
        else
        {
            llCharShowChar(x, y,showGeometry,str,Color);
        }

        if(isMalloc)
        {
            llFree(buffer);
        }
        
        if(*str>=0x80)
        {
            //utf8 chinese use 3 bytes

            str++;
            str++;
        }
        str++;

        x=x+w;
        if(*str=='\n')
        {
            y=y+h;
            x=oldX;
            str++;
        }

    }
}

//void llCharSetText(int16_t x, int16_t y,llGeometry showGeometry, uint8_t *str,llColor color,uint16_t maxX,bool isAutoLineBreak)
//{
//    uint16_t w,h;
//    int16_t oldX;
////    uint8_t *buffer=NULL;
//    bool isMalloc=false;
//    uint8_t dataBits;

//    uint16_t addWidth=0;

//    oldX=x;
//    while(*str!=0)
//    {
//        h= llFontGetHeightSize();
//        str=llCharShowLineChar(x,y,showGeometry,str,maxX,color);
//
//        if(isAutoLineBreak)
//        {
//            y=y+h;
//            x=oldX;
//        }
//        else
//        {
//            while((*str!=0)&&(*str=='\n'))
//            {
//                str++;
//            }
//        }
//    }
//}

int16_t llCharCalVerticalPos(llChar* charInitStruct,int16_t textHeight)
{
    int16_t y = 0;

    if(charInitStruct->vAlign==llAlignVCenter)
    {
        y=RECTANGLE_MID_Y_POS(charInitStruct->geometry.y,charInitStruct->geometry.height,textHeight);
    }
    else
    {
        if(charInitStruct->vAlign==llAlignVTop)
        {
            y=charInitStruct->geometry.y;
        }
        else
        {
            if(charInitStruct->vAlign==llAlignVBottom)
            {
                if(charInitStruct->fontLibInfo->fontSize<charInitStruct->geometry.height)//文字高度<显示高度
                {
                    y=RECTANGLE_BUTTOM_Y_POS(charInitStruct->geometry.y,charInitStruct->geometry.height,textHeight);
                }
                else
                {
                    y=charInitStruct->geometry.y;
                }
            }
        }
    }
    return y;
}

int16_t llCharCalHorizontalPos(llChar* charInitStruct,int16_t textWidth)
{
    int16_t x = 0;

    switch (charInitStruct->hAlign)
    {
    case llAlignHCenter:
    {
        //文字长度 text_width
        //显示长度 charInitStruct->width
        if(textWidth<charInitStruct->geometry.width)//文字长度<显示长度
        {
            x=RECTANGLE_MID_X_POS(charInitStruct->geometry.x,charInitStruct->geometry.width,textWidth);
        }
        else
        {
            x=charInitStruct->geometry.x;
        }
        break;
    }
    case llAlignHLeft:
    {
        x=charInitStruct->geometry.x;
        break;
    }
    case llAlignHRight:
    {
        //文字长度 text_width
        //显示长度 charInitStruct->width
        if(textWidth<charInitStruct->geometry.width)//文字长度<显示长度
        {
            x=RECTANGLE_RIGHT_X_POS(charInitStruct->geometry.x,charInitStruct->geometry.width,textWidth);
        }
        else
        {
            x=charInitStruct->geometry.x;
        }
        break;
    }
    default:
        break;
    }
    return x;
}

llGeometry llCharDisplay(llChar* charInitStruct)
{
    uint16_t charLength=0;
    int16_t x,y;
    int16_t textWidth,textHeight,textWidthTemp;
    int16_t charWidth;
    llGeometry realGeometry;

    if(!charInitStruct->isTransparent)//不透明
    {
        //画背景颜色
        llFillSingleColor(charInitStruct->showGeometry.x,
                          charInitStruct->showGeometry.y,
                          charInitStruct->showGeometry.x+charInitStruct->showGeometry.width-1,
                          charInitStruct->showGeometry.y+charInitStruct->showGeometry.height-1,
                          charInitStruct->backgroundColor);
    }

    //获取文字书写长度
    charLength=0;
    textWidth=0;
    textWidthTemp=0;
    textHeight=llFontGetHeightSize();

    //计算显示高度宽度
    while(charInitStruct->text[charLength]!=0)
    {
        charWidth=llFontGetWidthSize(&charInitStruct->text[charLength]);

        //处理换行,计算显示高度
        if((charInitStruct->text[charLength]=='\n')||(((textWidthTemp+charWidth)>charInitStruct->geometry.width)&&(charInitStruct->isAutoLineBreak==true)))
        {
            textHeight+=llFontGetHeightSize();
            textWidthTemp=0;
        }

        //计算宽度
        textWidthTemp+=charWidth;

        if(textWidth<textWidthTemp)
        {
            textWidth=textWidthTemp;
        }

        //英文 0x0-0x7F
        if(charInitStruct->text[charLength]>=0x80)
        {
            //utf8 chinese use 3 bytes
            charLength++;
            charLength++;
        }
        charLength++;
    }

    x=llCharCalHorizontalPos(charInitStruct,textWidth);

    y=llCharCalVerticalPos(charInitStruct,textHeight);

    realGeometry.x=x;
    realGeometry.y=y;
    realGeometry.width=textWidth;
    realGeometry.height=textHeight;

    llCharSetText(x,y,charInitStruct->showGeometry,charInitStruct->text,charInitStruct->charColor,charInitStruct->geometry.width,charInitStruct->isAutoLineBreak,charInitStruct->isPassword);

    return realGeometry;
}


void llCharDisplayCursor(llPoint pos,uint16_t height,llColor backgroundColor,bool *isShow)
{
    if(*isShow==true)
    {
        *isShow=false;
        llFillSingleColor(pos.x+2,pos.y+1,pos.x+2,pos.y+height-2,0);
    }
    else
    {
        *isShow=true;
        llFillSingleColor(pos.x+2,pos.y+1,pos.x+2,pos.y+height-2,backgroundColor);
    }
}



#define CURSOR_RUN   1
#define CURSOR_STOP   0

static uint8_t cursorBlinkState=CURSOR_STOP;
static uint32_t cursorBlinkCounter;
static llPoint cursorPos;
static uint16_t cursorHeight;
static llColor cursorBackgroundColor;
static bool cursorShow=false;
static void *cursorBlinkWidget=NULL;
static bool *cursorBlinkFlag=NULL;
void llCharSetCursor(void* blinkWidget,bool *enableFlag,int16_t x,int16_t y,uint16_t cursorBlinkHeight,llColor backgroundColor)
{
    cursorBlinkFlag=enableFlag;
    cursorBlinkWidget=blinkWidget;

    cursorPos.x=x;
    cursorPos.y=y;
    cursorHeight=cursorBlinkHeight;
    cursorBackgroundColor=backgroundColor;

    cursorBlinkState=CURSOR_RUN;
    llTimer_start(&cursorBlinkCounter);
    cursorShow=true;
    llCharDisplayCursor(cursorPos,cursorHeight,cursorBackgroundColor,&cursorShow);
}

void llCharAutoStopCursorBlink(void* clickWidget)
{
    if(cursorBlinkWidget!=NULL)
    {
        if(cursorBlinkWidget!=clickWidget)
        {
            llCharStopCursorBlink();
        }
    }
}

void llCharStopCursorBlink(void)
{
    if(cursorBlinkWidget!=NULL)
    {
        llEmitSignal(cursorBlinkWidget,SIGNAL_EIDTING_FINISHED);
        cursorBlinkWidget=NULL;
        cursorBlinkState=CURSOR_STOP;
        cursorBlinkCounter=0;
        cursorShow=false;
        *cursorBlinkFlag=false;
        llCharDisplayCursor(cursorPos,cursorHeight,cursorBackgroundColor,&cursorShow);
    }
}

void llCharCursorBlinkLoop(void)
{
    if(cursorBlinkState==CURSOR_RUN)
    {
        if(llTimer_timeOut(&cursorBlinkCounter,500,true))
        {
            //光标闪烁
            llCharDisplayCursor(cursorPos,cursorHeight,cursorBackgroundColor,&cursorShow);
        }
    }
}

//llPoint llCharGetCursorOffset(llChar* charInitStruct)
//{
//    uint16_t temp=0;
//    uint16_t text_width;
//    llPoint retPos;

//    //获取文字书写长度
//    temp=0;
//    text_width=0;

//    while(charInitStruct->text[temp]!=0)
//    {
//        //英文 0x0-0x7F
//        if(charInitStruct->text[temp]<0x80)
//        {

//            text_width+=llFontGetWidthSize(&charInitStruct->text[temp]);
//        }
//        else
//        {
//            //utf8 chinese use 3 bytes
//            text_width+=llFontGetWidthSize(&charInitStruct->text[temp]);
//            temp++;
//            temp++;
//        }
//        temp++;
//    }

//    if(charInitStruct->vAlign==llAlignVCenter)
//    {
//        retPos.y=text_mid_y_pos(charInitStruct->geometry.y,charInitStruct->geometry.height,charInitStruct->fontSize);
//    }
//    else
//    {
//        if(charInitStruct->vAlign==llAlignVTop)
//        {
//            retPos.y=charInitStruct->geometry.y;
//        }
//        else
//        {
//            if(charInitStruct->vAlign==llAlignVBottom)
//            {
//                if(charInitStruct->fontSize<charInitStruct->geometry.height)//文字高度<显示高度
//                {
//                    retPos.y=text_bottom_y_pos(charInitStruct->geometry.y,charInitStruct->geometry.height,charInitStruct->fontSize);
//                }
//                else
//                {
//                    retPos.y=charInitStruct->geometry.y;
//                }
//            }
//        }
//    }
//    retPos.x=charInitStruct->geometry.x+2;



//    return retPos;
//}







