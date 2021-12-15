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

#include "LL_Font.h"
#include "LL_User.h"
#include "LL_Config.h"
//#include "LL_TTF.h"
#include "LL_Lattice.h"
//#include "LL_ZkBin.h"

#define FONT_WORD_BUFFER_SIZE(s)    (((s*s%8)?(s*s/8)+1:(s*s/8))*2)

static llFontLib activeFontInfo={0};

void llFontOpenLibrary(llFontLibType fontLibType,uint8_t* fontName, uint16_t fontSize)
{
    uint8_t i;
    uint8_t *path;
    bool isFontNameSame=false;

    isFontNameSame=llStringCompare(activeFontInfo.name,fontName);

    if((isFontNameSame!=true)||(activeFontInfo.libType!=fontLibType)||(activeFontInfo.fontSize!=fontSize))
    {
        for(i=0;i<userFontLibTotalNum;i++)
        {
            if((llStringCompare((uint8_t *)userFontLibList[i].name,fontName)==true)&&(userFontLibList[i].fontSize==fontSize))
            {
                switch(userFontLibList[i].libType)
                {
#if USE_TTF == 1
                    case typeTTF:
                    {
                        activeFontInfo.pointer=NULL;

                        if(isFontNameSame!=true)
                        {
                            strcpy(activeFontInfo.name,fontName);
                            activeFontInfo.fontSize=fontSize;
                            activeFontInfo.libType=fontLibType;
                            activeFontInfo.dataBits=userFontLibList[i].dataBits;

                            path=(uint8_t *)llMalloc((strlen(USER_FOLDER_PATH)+strlen(fontName)+5)*sizeof(uint8_t));
                            strcpy(path,USER_FOLDER_PATH);
                            strcat(path,fontName);
                            strcat(path,".ttf");
                            llTTF_fontOpenLibrary(path,fontSize);
                            llFree(path);
                        }
                        else
                        {
                            activeFontInfo.fontSize=fontSize;
                            llTTF_setSize(fontSize);
                        }
                    }
                        break;
#endif
                    case typeCFile:
                    {
#if USE_TTF == 1
                        llTTF_fontCloseLibrary();
#endif
                        strcpy((char*)activeFontInfo.name,(const char*)fontName);
                        activeFontInfo.fontSize=userFontLibList[i].fontSize;
                        activeFontInfo.libType=userFontLibList[i].libType;
                        activeFontInfo.pointer=userFontLibList[i].pointer;
                        llLattice_fontOpenLibrary(activeFontInfo.pointer);
                    }
                        break;
#if USE_ZK_BIN == 1
                    case typeDataFile:
                    {
#if USE_TTF == 1
                        llTTF_fontCloseLibrary();
#endif
                        if(isFontNameSame!=true)
                        {
                            strcpy((char*)activeFontInfo.name,(const char*)fontName);
                            activeFontInfo.fontSize=fontSize;
                            activeFontInfo.libType=fontLibType;

                            path=(uint8_t *)llMalloc((strlen(USER_FOLDER_PATH)+strlen("fonts/")+strlen((const char*)fontName)+5)*sizeof(uint8_t));
                            strcpy((char*)path,USER_FOLDER_PATH);
                            strcat((char*)path,"fonts/");
                            strcat((char*)path,(const char*)fontName);
                            strcat((char*)path,".zk");
                            llZkBin_fontOpenLibrary(path);
                            llFree(path);
                        }
                        else
                        {
                            activeFontInfo.fontSize=fontSize;
#if USE_TTF == 1
                            llTTF_setSize(fontSize);
#endif
                        }
                    }
                        break;
#endif
                    default:
                        break;
                }
            }
        }
    }
}

//void llFontCloseLibrary(void)
//{
//    memset(active_font,0,32);
//    font_px_size=0;
//    if(Font_Library_Type==LIBRARY_TYPE_TTF)
//    {
//        llTTF_fontCloseLibrary();
//    }
//    else
//    {
//        llLattice_fontCloseLibrary();
//    }
//}


//
// 调用该函数前，先打开字库文件
// in : word
// out: width height buffer_pointer
//  return buffer malloc pointer

uint8_t* llFontGetData(uint8_t *word,uint8_t *dataBits, uint16_t *width, uint16_t *height,bool *isMalloc)
{
    uint8_t *buffer=NULL;

    switch (activeFontInfo.libType)
    {
#if USE_TTF == 1
        case typeTTF:
        {
            buffer=(uint8_t*)llMalloc(FONT_WORD_BUFFER_SIZE(activeFontInfo.fontSize)*sizeof(uint8_t));
            llTTF_fontGetData(word,activeFontInfo.fontSize,buffer,width,height);
            *isMalloc=true;
        }
            break;
#endif
        case typeCFile:
        {
            llLattice_fontGetData(word,dataBits,(void **)&buffer,width,height);
            *isMalloc=false;
        }
            break;
        case typeDataFile:
        {
            *isMalloc=false;
        }
            break;
        default:
            break;

    }
    return buffer;
}


uint8_t llFontGetWidthSize(uint8_t * word)
{
    uint8_t width=0;

    switch (activeFontInfo.libType)
    {
#if USE_TTF == 1
        case typeTTF:
        {
            width=llTTF_fontGetWidthSize(word);
        }
            break;
#endif
        case typeCFile:
        {
            width=llLattice_fontGetWidthSize(word);
        }
            break;
        case typeDataFile:
        {

        }
            break;
        default:
            break;
    }

    return width;
}

uint8_t llFontGetHeightSize(void)
{
    uint8_t width=0;

    switch (activeFontInfo.libType)
    {
#if USE_TTF == 1
        case typeTTF:
        {
            width=llTTF_fontGetHeightSize();
        }
            break;
#endif
        case typeCFile:
        {
            width=llLattice_fontGetHeightSize();
        }
            break;
        case typeDataFile:
        {

        }
            break;
        default:
            break;
    }

    return width;
}

