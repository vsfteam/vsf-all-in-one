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

#include "LL_Lattice.h"
#include "LL_General.h"

static llLatticeLibraryInfo * latticeActiveFont;

//static uint8_t utf8ToUnicode(uint8_t* unicode, uint8_t *utf8)
//{
//    uint8_t nBytes = 0;

//    if (0 == (*utf8 & 0x80))
//    {
//        /*
//         * single-byte char
//         */
//        nBytes = 1;
//        unicode[0] = *utf8;
//    }
//    else
//    {
//        /*
//         * 3-byte char (chinese char)
//         */
//        if ( (*utf8 & 0xf0) == 0xe0 )
//        {
//            nBytes  = 3;
//            unicode[0] = ((utf8[0] & 0x0f) <<4) + ((utf8[1] & 0x3c) >>2);
//            unicode[1] = ((utf8[1] & 0x03) <<6) + (utf8[2] & 0x3f);
//        }
//        else
//        {
//            //            debug("ERROR: utf-8 to unicode, nBytes !=3\n");
//            nBytes = 0;
//            unicode[0] = '?';
//        }
//    }
//    return nBytes;
//}

void llLattice_fontOpenLibrary(llLatticeLibraryInfo * libPointer)
{
    latticeActiveFont=libPointer;
}

void llLattice_fontCloseLibrary(void)
{
    if(latticeActiveFont!=NULL)
    {
        latticeActiveFont=NULL;
    }
}

void llLattice_fontGetData(uint8_t * utf8In,uint8_t *dataBits,void **retBitmap, uint16_t *retWidth, uint16_t *retHeight)
{
    uint8_t unicode[2]={0};
    uint32_t fontCount,i;
    uint8_t ret;

    ret=llUtf8ToUnicode(unicode,utf8In);

    if(ret!=0)
    {
        //查找unicode

        fontCount=latticeActiveFont->totalCharNum;
        *dataBits=latticeActiveFont->dataBits;

        for(i=0;i<fontCount;i++)
        {
            if(ret==1)
            {
                if((latticeActiveFont->dataInfo[i].unicodeL==unicode[0])&&(latticeActiveFont->dataInfo[i].unicodeH==0))
                {
                    *retHeight=latticeActiveFont->dataInfo[i].height;
                    *retWidth=latticeActiveFont->dataInfo[i].width;
                    *retBitmap=latticeActiveFont->dataInfo[i].dataPointer;
                    break;
                }
            }
            else
            {
                if((latticeActiveFont->dataInfo[i].unicodeL==unicode[1])&&(latticeActiveFont->dataInfo[i].unicodeH==unicode[0]))
                {
                    *retHeight=latticeActiveFont->dataInfo[i].height;
                    *retWidth=latticeActiveFont->dataInfo[i].width;
                    *retBitmap=latticeActiveFont->dataInfo[i].dataPointer;
                    break;
                }
            }


        }
    }
}

uint8_t llLattice_fontGetWidthSize(uint8_t * utf8In)
{
    unsigned char unicode[2]={0};
    uint32_t fontCount,i;
    uint8_t ret;

    ret=llUtf8ToUnicode(unicode,utf8In);

    if(ret!=0)
    {
        //查找unicode

        fontCount=latticeActiveFont->totalCharNum;

        for(i=0;i<fontCount;i++)
        {
            if(ret==1)
            {
                if((latticeActiveFont->dataInfo[i].unicodeL==unicode[0])&&(latticeActiveFont->dataInfo[i].unicodeH==0))
                {
                    return latticeActiveFont->dataInfo[i].width;
                }
            }
            else
            {
                if((latticeActiveFont->dataInfo[i].unicodeL==unicode[1])&&(latticeActiveFont->dataInfo[i].unicodeH==unicode[0]))
                {
                    return latticeActiveFont->dataInfo[i].width;
                }
            }

        }
    }
    return 0;
}

uint8_t llLattice_fontGetHeightSize(void)
{
    return (uint8_t)latticeActiveFont->dataInfo[0].height;
}






