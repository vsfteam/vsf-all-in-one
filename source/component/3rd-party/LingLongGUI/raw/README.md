# LingLongGUI

#### 介绍
玲珑GUI是高效的界面开发解决方案。  
代替串口屏、组态，降低产品成本，产品软硬件自主可控。  
配套界面开发软件，图形化编辑界面，生成C代码，直接和用户产品代码结合。  
配套下载升级软件和bootloader，解决产品升级功能和图片下载问题。  

#### 目的
提高开发效率，降低开发难度，降低产品成本

#### 教程
详细教程请查看 https://www.yuque.com/books/share/3317aaa7-f47f-4bfd-a4c2-7e64e7f1c4be?# 《玲珑GUI教程》  
本gui配套上位机，请移步至qq群下载  

10分钟完成玲珑GUI移植  
以下教程仅最小化移植，如需要完整功能，需要把config.c内的函数补全即可。  
1、安装keil、LingLongGuiBuiilder  
2、准备开发板TFT项目文件，确保路径不能包含中文，屏幕能够显示  
3、keil Options for Target -- C/C++ -- 勾选C99  
4、keil Options for Target -- C/C++ -- Misc Controls  
&ensp;&ensp;&ensp;填入--no-multibyte-chars --locale=english  
&ensp;&ensp;&ensp;AC5必须填入--no-multibyte-chars，AC6则不需要  
5、启动gui builder，keil -- tools --LingLongGuiBuilder  
6、新建gui界面，放置控件，生成文件（为了快速测试，请勿设置任何图片），必须保存项目后，再点击生成文件  
7、返回keil，自动刷新项目树目录，编译项目  
8、打开LL_Config.c(在llGui\User中)  
9、函数llCfgSetPoint添加TFT打点函数  
&ensp;&ensp;&ensp;函数llCfgGetPoint添加TFT取色函数  
&ensp;&ensp;&ensp;函数llCfgFillSingleColor添加矩形单色填充函数  
10、main函数加入初始化函数和循环函数  
#include "LL_Handler.h"  
int main(void)  
{  
&ensp;&ensp;llInit();  
&ensp;&ensp;while(1)  
&ensp;&ensp;{  
&ensp;&ensp;&ensp;&ensp;llHandler();  
&ensp;&ensp;}  
}  
11、编译，下载

#### 技术讨论
QQ群：1004783094

#### 版权声明
本软件协议使用Apache License, Version 2.0  
附加协议：  
如果使用本软件编写二次开发类产品，包括且不限于串口屏、组态屏、二次开发屏幕模组，  
二次开发类产品则需要使用符合GPL兼容自由软件许可证或获得商业授权  

简而言之，允许开发终端类产品，禁止未授权的闭源二次开发类产品  
  
GPL兼容自由软件许可证种类参考  
http://www.gnu.org/licenses/license-list.en.html





