#include "LL_IconSlider.h"
#include "LL_Linked_List.h"
#include "LL_Handler.h"
#include "LL_Timer.h"
void llIconSliderAction(void *widget, uint8_t touchSignal);
void nIconSliderDelete(uint16_t nameId);
void pIconSliderRefresh(llIconSlider *widget);
void nIconSliderRefresh(uint16_t nameId);

bool slotIconSliderPressed(llConnectInfo info)
{
    llIconSlider * widget;
    int16_t x,y;
    widget=(llIconSlider*)info.receiver;

    if(widget->isEnable)
    {
        if(llClickGetPoint(&x,&y)==true)
        {
            llTimer_stop(&widget->clickTimer);
            widget->isClickItem=true;
            widget->clickPoint.x=x;
            widget->clickPoint.y=y;
            widget->moveOffset=0;
            return true;
        }
    }
    return false;
}

bool slotIconSliderReleased(llConnectInfo info)
{
    llIconSlider * widget;
    uint8_t itemNumMod,minShowItemCount;

    widget=(llIconSlider*)info.receiver;
    if(widget->isEnable)
    {
        if((widget->moveOffset>30)||(widget->moveOffset<-30))
        {
            if(widget->isHorizontalScroll)//水平
            {
                if(widget->moveOffset<0)//向左拖动
                {
                    widget->itemNum=(-widget->moveX)/(widget->itemWidth*widget->columnCount+widget->pageSpacing)*widget->columnCount;
                    widget->itemNum+=widget->rowCount*widget->columnCount;

                    //计算能显示几个图片
                    minShowItemCount=(widget->geometry.width+widget->pageSpacing)/(widget->itemWidth*widget->columnCount+widget->pageSpacing)*(widget->rowCount*widget->columnCount);

                    if(widget->imageCount>minShowItemCount)
                    {
                        if(widget->itemNum>=(widget->imageCount-minShowItemCount))
                        {
                            widget->itemNum=widget->imageCount-minShowItemCount;
                        }
                        widget->moveX=-(widget->itemNum/(widget->columnCount*widget->rowCount))*(widget->itemWidth*widget->columnCount+widget->pageSpacing);
                    }
                    else
                    {
                        widget->moveX=0;
                        widget->itemNum=0;
                    }
                }
                else//向右拖动
                {
                    if(widget->moveX>0)
                    {
                        widget->moveX=0;
                        widget->itemNum=0;
                    }
                    else
                    {
                        widget->itemNum=(-widget->moveX)/(widget->itemWidth*widget->columnCount+widget->pageSpacing)*widget->columnCount;

                        if(widget->itemNum>=widget->imageCount)
                        {
                            widget->itemNum=widget->imageCount-1;
                        }

                        widget->moveX=-(widget->itemNum/(widget->columnCount*widget->rowCount))*(widget->itemWidth*widget->columnCount+widget->pageSpacing);
                    }
                }
            }
            else
            {
                if(widget->moveOffset<0)//向上拖动
                {
                    widget->itemNum=(-widget->moveY)/(widget->itemHeight*widget->rowCount+widget->pageSpacing)*widget->rowCount;
                    widget->itemNum+=widget->rowCount*widget->columnCount;

                    //计算能显示几个图片
                    minShowItemCount=(widget->geometry.height+widget->pageSpacing)/(widget->itemHeight*widget->rowCount+widget->pageSpacing)*(widget->rowCount*widget->columnCount);

                    if(widget->imageCount>minShowItemCount)
                    {
                        if(widget->itemNum>=(widget->imageCount-minShowItemCount))
                        {
                            widget->itemNum=widget->imageCount-minShowItemCount;
                        }
                        widget->moveY=-(widget->itemNum/(widget->columnCount*widget->rowCount))*(widget->itemHeight*widget->rowCount+widget->pageSpacing);
                    }
                    else
                    {
                        widget->moveY=0;
                        widget->itemNum=0;
                    }
                }
                else//向下拖动
                {
                    if(widget->moveY>0)
                    {
                        widget->moveY=0;
                        widget->itemNum=0;
                    }
                    else
                    {
                        widget->itemNum=(-widget->moveY)/(widget->itemHeight*widget->rowCount+widget->pageSpacing)*widget->rowCount;

                        if(widget->itemNum>=widget->imageCount)
                        {
                            widget->itemNum=widget->imageCount-1;
                        }

                        widget->moveY=-(widget->itemNum/(widget->columnCount*widget->rowCount))*(widget->itemHeight*widget->rowCount+widget->pageSpacing);
                    }
                }
            }
        }
        else
        {
            //还原
            if(widget->isHorizontalScroll)//水平
            {
                widget->moveX=-widget->itemNum*(widget->itemWidth+widget->pageSpacing);
            }
            else
            {
                widget->moveY=-widget->itemNum*(widget->itemHeight+widget->pageSpacing);
            }
        }

        if(widget->isClickItem==true)
        {
            llTimer_start(&widget->clickTimer);
        }

        pIconSliderRefresh(widget);
    }
    return false;
}

bool slotIconSliderMove(llConnectInfo info)
{
    llIconSlider * widget;
    int16_t nowX,nowY;
    bool clickState;

    widget=(llIconSlider*)info.receiver;

    if(widget->isEnable)
    {
        clickState=llClickGetPoint(&nowX,&nowY);
        if(clickState==true)
        {
            if(widget->isClickItem==true)//判断是移动还是点击
            {
                if((abs(nowX-widget->clickPoint.x)>5)||(abs(nowY-widget->clickPoint.y)>5))
                {
                    widget->isClickItem=false;
                }
            }

            if(widget->isHorizontalScroll)//水平
            {
                widget->moveOffset=nowX-widget->clickPoint.x;
                //拉到尽头偏移减半
                if((widget->moveOffset>0)||((widget->moveOffset+widget->columnCount*widget->itemWidth)<widget->geometry.width))
                {
                    widget->moveOffset/=2;
                }
                widget->moveX=widget->moveOffset-((widget->itemNum/(widget->columnCount*widget->rowCount))*(widget->itemWidth*widget->columnCount+widget->pageSpacing));
                pIconSliderRefresh(widget);
            }
            else
            {
                widget->moveOffset=nowY-widget->clickPoint.y;
                //拉到尽头偏移减半
                if((widget->moveOffset>0)||((widget->moveOffset+widget->rowCount*widget->itemHeight)<widget->geometry.height))
                {
                    widget->moveOffset/=2;
                }
                widget->moveY=widget->moveOffset-((widget->itemNum/(widget->columnCount*widget->rowCount))*(widget->itemHeight*widget->rowCount+widget->pageSpacing));
                pIconSliderRefresh(widget);
            }
        }
    }
    return false;
}

llIconSlider *llIconSliderQuickCreate(uint16_t nameId, uint16_t parentNameId,
                                      int16_t x, int16_t y, int16_t width, int16_t height,
                                      uint8_t rowCount,uint8_t columnCount,
                                      uint8_t itemWidth,uint8_t itemHeight,
                                      uint8_t pageMax,uint8_t pageSpacing,
                                      bool isHorizontalScroll,bool isHidden)
{
    llIconSlider * pNewWidget = NULL;
    llListWidgetInfo *parentInfo;
    uint32_t *imageAddrBuf;

    //检查父链表存在
    if(llList_GetInfoByName(&parentInfo,parentNameId)==true)
    {
        pNewWidget = LL_MALLOC_WIDGET_INFO(llIconSlider);
        imageAddrBuf = llMalloc(sizeof (uint32_t)*rowCount*columnCount*pageMax);
        if((pNewWidget!=NULL)&&(imageAddrBuf!=NULL))
        {
            pNewWidget->nameId=nameId;

            pNewWidget->parentType=((llGeneral*)(parentInfo->widget))->widgetType;
            pNewWidget->parentWidget=parentInfo->widget;

            pNewWidget->widgetType=widgetTypeIconSlider;
            pNewWidget->geometry.x=x;
            pNewWidget->geometry.y=y;
            pNewWidget->geometry.width=width;
            pNewWidget->geometry.height=height;

            pNewWidget->moveX=0;
            pNewWidget->moveY=0;
            pNewWidget->itemNum=0;

            pNewWidget->rowCount=rowCount;
            pNewWidget->columnCount=columnCount;
            pNewWidget->itemWidth=itemWidth;
            pNewWidget->itemHeight=itemHeight;

            pNewWidget->deleteFunc=nIconSliderDelete;
            pNewWidget->actionFunc=llIconSliderAction;
            pNewWidget->refreshFunc=nIconSliderRefresh;
            pNewWidget->isHidden=isHidden;
            pNewWidget->isEnable=true;
            pNewWidget->pageMax=pageMax;
            pNewWidget->imageCount=0;
            pNewWidget->isHorizontalScroll=isHorizontalScroll;
            pNewWidget->page=0;
            pNewWidget->imageAddr=imageAddrBuf;
            pNewWidget->pageSpacing=pageSpacing;
            pNewWidget->isClickItem=true;
            pNewWidget->isWaitRefresh=false;
            //add linked list
            llListWidgetAdd(&(parentInfo->child_link),pNewWidget);

            //动作
            llConnectSignal(nameId,SIGNAL_CLICK_PRESSED,nameId,slotIconSliderPressed);
            llConnectSignal(nameId,SIGNAL_CLICK_HOLD_MOVE,nameId,slotIconSliderMove);
            llConnectSignal(nameId,SIGNAL_CLICK_RELEASED,nameId,slotIconSliderReleased);
        }
        else
        {
            llFree(pNewWidget);
            llFree(imageAddrBuf);
        }
    }

    return pNewWidget;
}

llIconSlider *llIconSliderCreate(uint16_t nameId, uint16_t parentNameId,
                                 int16_t x, int16_t y, int16_t width, int16_t height,
                                 uint8_t rowCount,uint8_t columnCount,
                                 uint8_t pageMax,uint8_t pageSpacing,
                                 bool isHorizontalScroll,bool isHidden)
{
    return llIconSliderQuickCreate(nameId,parentNameId,x,y,width,height,rowCount,columnCount,
                                   (uint8_t)(width/rowCount),(uint8_t)(height/columnCount),
                                   pageMax,pageSpacing,
                                   isHorizontalScroll,isHidden);
}


void llIconSliderAction(void *widget,uint8_t touchSignal)
{
    if(((llIconSlider*)widget)->isEnable)
    {
        switch(touchSignal)
        {
        case SIGNAL_CLICK_PRESSED:
        case SIGNAL_CLICK_RELEASED:
        case SIGNAL_CLICK_HOLD_MOVE:
        {
            llEmitSignal(widget,touchSignal);
            break;
        }
        default:
            break;
        }
    }
}

void pIconSliderLoop(llIconSlider *widget)
{
    int16_t calX,calY;
    uint8_t row,column;

    int16_t nowX,nowY;
    bool clickState;

    uint8_t groupNum;
    uint16_t tempPixLen;
    if(widget->isWaitRefresh)
    {
        widget->isWaitRefresh=false;
        pIconSliderRefresh((llIconSlider*)widget);
    }
    if(llTimer_timeOut(&widget->clickTimer,200,false))
    {
        clickState=llClickGetPoint(&nowX,&nowY);
        if(clickState==false)
        {
            //计算点击item序号
            calX=widget->clickPoint.x-widget->geometry.x;
            calY=widget->clickPoint.y-widget->geometry.y;
            if(widget->isHorizontalScroll)//水平
            {
                groupNum=(-widget->moveX+calX)/(widget->itemWidth*widget->columnCount+widget->pageSpacing);
                column=calX%(widget->itemWidth*widget->columnCount+widget->pageSpacing)/widget->itemWidth;
                row=calY/widget->itemHeight;
            }
            else
            {
                groupNum=(-widget->moveY+calY)/(widget->itemHeight*widget->rowCount+widget->pageSpacing);
                row=calY%(widget->itemHeight*widget->rowCount+widget->pageSpacing)/widget->itemHeight;
                column=calX/widget->itemWidth;
            }
            if((column>=widget->columnCount)||(row>=widget->rowCount))//空白处
            {
                return;
            }
            widget->clickItemNum=groupNum*widget->columnCount*widget->rowCount;
            widget->clickItemNum+=row*widget->columnCount+column;

            llEmitSignal(widget,SIGNAL_WIDGET_ACTIVE);
        }
    }
}

void nIconSliderLoop(uint16_t nameId)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeIconSlider);
    if(widget!=NULL)
    {
        pIconSliderLoop((llIconSlider*)widget);
    }
}

void pIconSliderFree(llIconSlider *widget)
{
    llFree(widget->imageAddr);
    llFree(widget);
}

void nIconSliderDelete(uint16_t nameId)
{
    llListHead *tempPos,*safePos;
    llListWidgetInfo *linkInfo;
    llListWidgetInfo *parentInfo;
    llListWidgetInfo *tempInfo;
    llIconSlider *widget;

    if(llList_GetInfoByName(&linkInfo,nameId)==true)
    {
        widget=linkInfo->widget;
        //查找父链表
        llList_GetInfoByName(&parentInfo,((llGeneral*)widget->parentWidget)->nameId);
        //消除自身在父链表中的位置数据
        list_for_each_prev_safe(tempPos, safePos,&parentInfo->child_link)
        {
            tempInfo = list_entry(tempPos, llListWidgetInfo, parent_link_pos);
            if(tempInfo->widget==widget)
            {
                llLinkedListDelete(tempPos);
                pIconSliderFree(widget);
                llFree(tempInfo);
            }
        }
    }
}

void pIconSliderRefresh(llIconSlider *widget)
{
    llPoint globalPos;
    int16_t x,y;
    uint8_t page;
    uint8_t i;
    llGeometry imgGeometry,showGeometry;
    llSize imgSize;

    if(widget->isHidden==false)
    {
        globalPos=llListGetGlobalPos((llGeneral*)widget);

        //空余区域填充
        if((widget->moveX>0)||(widget->moveY>0))//左侧，上方
        {
            imgGeometry.x=globalPos.x;
            imgGeometry.y=globalPos.y;
            if(widget->moveX>0)
            {
                imgGeometry.width=widget->moveX;
                imgGeometry.height=widget->geometry.height;

                if( llRectIntersect(widget->geometry, imgGeometry,&showGeometry))
                {
                    llGeneralWidgetParentRecover((llGeneral*)widget,showGeometry);
                }
            }
            else
            {
                imgGeometry.width=widget->geometry.width;
                imgGeometry.height=widget->moveY;
                if( llRectIntersect(widget->geometry, imgGeometry,&showGeometry))
                {
                    llGeneralWidgetParentRecover((llGeneral*)widget,showGeometry);
                }
            }
        }

        else//右侧，下方
        {
            if(widget->isHorizontalScroll)//水平
            {
                imgGeometry.x=globalPos.x+widget->moveX+widget->itemWidth*widget->columnCount*widget->pageMax+widget->pageSpacing*(widget->pageMax-1);
                if(imgGeometry.x<(widget->geometry.x+widget->geometry.width))
                {
                    imgGeometry.y=globalPos.y;
                    imgGeometry.width=widget->geometry.x+widget->geometry.width-imgGeometry.x;
                    imgGeometry.height=widget->geometry.height;
                    if( llRectIntersect(widget->geometry, imgGeometry,&showGeometry))
                    {
                        llGeneralWidgetParentRecover((llGeneral*)widget,showGeometry);
                    }
                }
            }
            else
            {
                imgGeometry.y=globalPos.y+widget->moveY+widget->itemHeight*widget->rowCount*widget->pageMax+widget->pageSpacing*(widget->pageMax-1);
                if(imgGeometry.y<(widget->geometry.y+widget->geometry.height))
                {
                    imgGeometry.x=globalPos.x;
                    imgGeometry.width=widget->geometry.width;
                    imgGeometry.height=widget->geometry.y+widget->geometry.height-imgGeometry.y;
                    if( llRectIntersect(widget->geometry, imgGeometry,&showGeometry))
                    {
                        llGeneralWidgetParentRecover((llGeneral*)widget,showGeometry);
                    }
                }
            }
        }

        //填充中间空余区域
        for(i=0; i<(widget->pageMax-1); i++)
        {
            if(widget->isHorizontalScroll)//水平
            {
                imgGeometry.x=globalPos.x+widget->moveX+widget->itemWidth*widget->columnCount+(i*(widget->itemWidth*widget->columnCount+widget->pageSpacing));
                imgGeometry.y=globalPos.y;
                imgGeometry.width=widget->pageSpacing;
                imgGeometry.height=widget->geometry.height;

                if( llRectIntersect(widget->geometry, imgGeometry,&showGeometry))
                {
                    llGeneralWidgetParentRecover((llGeneral*)widget,showGeometry);
                }
            }
            else
            {
                imgGeometry.x=globalPos.x;
                imgGeometry.y=globalPos.y+widget->moveY+widget->itemHeight*widget->rowCount+(i*(widget->itemHeight*widget->rowCount+widget->pageSpacing));
                imgGeometry.width=widget->geometry.width;
                imgGeometry.height=widget->pageSpacing;

                if( llRectIntersect(widget->geometry, imgGeometry,&showGeometry))
                {
                    llGeneralWidgetParentRecover((llGeneral*)widget,showGeometry);
                }
            }
        }

        for(i=0; i<(widget->rowCount*widget->columnCount*widget->pageMax); i++)
        {
            //计算坐标
            //页
            page=i/(widget->rowCount*widget->columnCount);

            //窗口内坐标
            x=(i%widget->columnCount)*widget->itemWidth;
            y=(i/widget->columnCount)*widget->itemHeight;

            //换页坐标
            if(widget->isHorizontalScroll)
            {
                x+=page*(widget->itemWidth*widget->columnCount+widget->pageSpacing);
                y-=page*widget->itemHeight*widget->rowCount;
            }
            else
            {
                y+=page*widget->pageSpacing;
            }

            //全局坐标
            x+=globalPos.x+widget->moveX;
            y+=globalPos.y+widget->moveY;

            if(i<widget->imageCount)
            {
                imgGeometry.x=x;
                imgGeometry.y=y;

                imgSize=llGeneralGetImageSize(widget->imageAddr[i]);
                imgGeometry.width=imgSize.width;
                imgGeometry.height=imgSize.height;

                if( llRectIntersect(widget->geometry, imgGeometry,&showGeometry))
                {
                    llGeneralImageSpecificAreaShow((llGeneral*)widget,imgGeometry, showGeometry,widget->imageAddr[i]);
                }
            }
            else
            {
                imgGeometry.x=x;
                imgGeometry.y=y;
                imgGeometry.width=widget->itemWidth;
                imgGeometry.height=widget->itemHeight;
                if( llRectIntersect(widget->geometry, imgGeometry,&showGeometry))
                {
                    llGeneralWidgetParentRecover((llGeneral*)widget,showGeometry);
                }
            }
        }

    }
}

void nIconSliderRefresh(uint16_t nameId)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeIconSlider);

    if(widget!=NULL)
    {
        pIconSliderRefresh((llIconSlider*)widget);
    }
}

void pIconSliderAddImage(llIconSlider *widget,uint32_t imageAddr)
{
    if(widget->imageCount<(widget->rowCount*widget->columnCount*widget->pageMax))
    {
        widget->imageAddr[widget->imageCount++]=imageAddr;
    }
}

void nIconSliderAddImage(uint16_t nameId,uint32_t imageAddr)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeIconSlider);
    if(widget!=NULL)
    {
        pIconSliderAddImage((llIconSlider*)widget,imageAddr);
        ((llIconSlider*)widget)->isWaitRefresh=true;
    }
}

uint16_t pIconSliderGetClickItemNum(llIconSlider *widget)
{
    return widget->clickItemNum;
}

uint16_t nIconSliderGetClickItemNum(uint16_t nameId)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeIconSlider);
    if(widget!=NULL)
    {
        return pIconSliderGetClickItemNum((llIconSlider*)widget);
    }
    return 0;
}
