#ifndef _SC_GUI_H
#define _SC_GUI_H

#include "vsf_cfg.h"
#include "./lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LCD_SCREEN_WIDTH
#   define  LCD_SCREEN_WIDTH   1024
#endif

#ifndef LCD_SCREEN_HEIGHT
#   define  LCD_SCREEN_HEIGHT  768
#endif

#ifndef SC_PFB_BUF_SIZE
#   define  SC_PFB_BUF_SIZE    (20 * LCD_SCREEN_WIDTH * 2)
#endif

#define sc_abs(a) ((a<0)?(-a):(a))
#define SC_MAX(a,b)(a>b?a:b)
#define SC_MIN(a,b)(a<b?a:b)

/**fun: pfb画点*/
#define set_pixel_value(dest, offs, src,  c)   dest->buf[offs]=alphaBlend(c,dest->buf[offs],src)

typedef struct
{
    int         xs;
    int         ys;
    uint16_t    w;
    uint16_t    h;
    uint16_t    *buf;
    uint16_t    num;        //
    uint16_t    stup;
} SC_tile;

typedef struct
{
    int         xs;
    int         ys;
    int         xe;
    int         ye;
} SC_ARER;

typedef struct
{
    const uint8_t *map;
    uint16_t w;
    uint16_t h;
    uint16_t pitch;
    enum {
        SC_IMAGE_RGB565,
        SC_IMAGE_ARGB8888,
    } format;
} SC_img_t;

typedef struct
{
   // uint16_t    state;
    uint16_t    alpha;      //全局透明度
    uint16_t    bkc;         //主题底色
    uint16_t    bc;          //背影色
    uint16_t    fc;          //前影色

    uint16_t    dma_i;
    uint16_t    *dma_prt;
    uint16_t    *pfb_buf;

    SC_ARER      lcd_area;
} SC_GUI;

typedef enum {
    SC_ALIGN_CENTER         = 0,
    SC_ALIGN_LEFT           = 1 << 0,
    SC_ALIGN_RIGHT          = 1 << 1,
} SC_ALIGN;

extern SC_GUI *gui;

uint16_t alphaBlend( uint16_t fc, uint16_t bc,uint8_t alpha);
void SC_GUI_Init(uint16_t bkc,uint16_t bc,uint16_t fc);
void SC_pfb_clip(SC_tile *clip, int xs,int ys,int xe,int ye,uint16_t colour);
void SC_pfb_DrawFill(SC_tile *dest,int xs,int ys,int xe,int ye,uint16_t fc);
void SC_pfb_DrawFrame(SC_tile *dest, int xs, int ys, int xe, int ye, int width, uint16_t fc);
void SC_pfb_Image(SC_tile *dest,int xs,int ys,uint8_t alpha,SC_img_t *src);

void SC_pfb_RoundFrame(SC_tile *dest,int xs,int ys,int xe,int ye, int r,int ir, uint16_t ac,uint16_t bc);
void SC_pfb_RectFrame(SC_tile *dest,int xs,int ys,int xe,int ye, int width, uint16_t ac,uint16_t bc);
int SC_pfb_printf(SC_tile *dest, int x,int y,const char* txt,uint16_t fc,uint16_t bc, lv_font_t* font, int width, int line_space, SC_ALIGN align);

///-----------------------------------------------------
#define C_MAROON                       0x8000
#define C_DARK_RED                     0x8800
#define C_BROWN                        0xA145
#define C_FIREBRICK                    0xB104
#define C_CRIMSON                      0xD8A7
#define C_RED                          0xF800
#define C_TOMATO                       0xFB09
#define C_CORAL                        0xFBEA
#define C_INDIAN_RED                   0xCAEB
#define C_LIGHT_CORAL                  0xEC10
#define C_DARK_SALMON                  0xE4AF
#define C_SALMON                       0xF40E
#define C_LIGHT_SALMON                 0xFD0F
#define C_ORANGE_RED                   0xFA20
#define C_DARK_ORANGE                  0xFC60
#define C_ORANGE                       0xFD20
#define C_GOLD                         0xFEA0
#define C_DARK_GOLDEN_ROD              0xB421
#define C_GOLDEN_ROD                   0xDD24
#define C_PALE_GOLDEN_ROD              0xEF35
#define C_DARK_KHAKI                   0xBDAD
#define C_KHAKI                        0xEF31
#define C_OLIVE                        0x8400
#define C_YELLOW                       0xFFE0
#define C_YELLOW_GREEN                 0x9E66
#define C_DARK_OLIVE_GREEN             0x5346
#define C_OLIVE_DRAB                   0x6C64
#define C_LAWN_GREEN                   0x7FC0
#define C_CHART_REUSE                  0x7FE0
#define C_GREEN_YELLOW                 0xAFE6
#define C_DARK_GREEN                   0x0320
#define C_GREEN                        0x07E0
#define C_FOREST_GREEN                 0x2444


#define C_LIME                         0x07E0
#define C_LIME_GREEN                   0x3666
#define C_LIGHT_GREEN                  0x9772
#define C_PALE_GREEN                   0x97D2
#define C_DARK_SEA_GREEN               0x8DD1
#define C_MEDIUM_SPRING_GREEN          0x07D3
#define C_SPRING_GREEN                 0x07EF
#define C_SEA_GREEN                    0x344B
#define C_MEDIUM_AQUA_MARINE           0x6675
#define C_MEDIUM_SEA_GREEN             0x3D8E
#define C_LIGHT_SEA_GREEN              0x2595
#define C_DARK_SLATE_GRAY              0x328A
#define C_TEAL                         0x0410
#define C_DARK_CYAN                    0x0451
#define C_AQUA                         0x07FF
#define C_CYAN                         0x07FF
#define C_LIGHT_CYAN                   0xDFFF
#define C_DARK_TURQUOISE               0x0679
#define C_TURQUOISE                    0x46F9
#define C_MEDIUM_TURQUOISE             0x4E99
#define C_PALE_TURQUOISE               0xAF7D
#define C_AQUA_MARINE                  0x7FFA

#define C_POWDER_BLUE                  0xAEFC
#define C_CADET_BLUE                   0x64F3
#define C_STEEL_BLUE                   0x4C16
#define C_CORN_FLOWER_BLUE             0x64BD
#define C_DEEP_SKY_BLUE                0x05FF
#define C_DODGER_BLUE                  0x249F
#define C_LIGHT_BLUE                   0xAEBC
#define C_SKY_BLUE                     0x867D
#define C_LIGHT_SKY_BLUE               0x867E
#define C_MIDNIGHT_BLUE                0x18CE
#define C_NAVY                         0x0010
#define C_DARK_BLUE                    0x0011
#define C_MEDIUM_BLUE                  0x0019
#define C_BLUE                         0x001F
#define C_ROYAL_BLUE                   0x435B
#define C_BLUE_VIOLET                  0x897B
#define C_INDIGO                       0x4810
#define C_DARK_SLATE_BLUE              0x49F1
#define C_SLATE_BLUE                   0x6AD9
#define C_MEDIUM_SLATE_BLUE            0x7B5D
#define C_MEDIUM_PURPLE                0x939B
#define C_DARK_MAGENTA                 0x8811
#define C_DARK_VIOLET                  0x901A
#define C_DARK_ORCHID                  0x9999
#define C_MEDIUM_ORCHID                0xBABA
#define C_PURPLE                       0x8010
#define C_THISTLE                      0xD5FA
#define C_PLUM                         0xDD1B
#define C_VIOLET                       0xEC1D
#define C_MAGENTA                      0xF81F
#define C_ORCHID                       0xDB9A
#define C_MEDIUM_VIOLET_RED            0xC0B0
#define C_PALE_VIOLET_RED              0xDB92
#define C_DEEP_PINK                    0xF8B2
#define C_HOT_PINK                     0xFB56
#define C_LIGHT_PINK                   0xFDB7
#define C_PINK                         0xFDF9
#define C_ANTIQUE_WHITE                0xF75A
#define C_BEIGE                        0xF7BB
#define C_BISQUE                       0xFF18
#define C_BLANCHED_ALMOND              0xFF59
#define C_WHEAT                        0xF6F6
#define C_CORN_SILK                    0xFFBB
#define C_LEMON_CHIFFON                0xFFD9
#define C_LIGHT_GOLDEN_ROD_YELLOW      0xF7DA
#define C_LIGHT_YELLOW                 0xFFFB
#define C_SADDLE_BROWN                 0x8A22
#define C_SIENNA                       0x9A85
#define C_CHOCOLATE                    0xD344
#define C_PERU                         0xCC28
#define C_SANDY_BROWN                  0xF52C
#define C_BURLY_WOOD                   0xDDB0
#define C_TAN                          0xD591
#define C_ROSY_BROWN                   0xBC71
#define C_MOCCASIN                     0xFF16
#define C_NAVAJO_WHITE                 0xFEF5
#define C_PEACH_PUFF                   0xFED6
#define C_MISTY_ROSE                   0xFF1B
#define C_LAVENDER_BLUSH               0xFF7E
#define C_LINEN                        0xF77C
#define C_OLD_LACE                     0xFFBC
#define C_PAPAYA_WHIP                  0xFF7A
#define C_SEA_SHELL                    0xFFBD
#define C_MINT_CREAM                   0xF7FE
#define C_SLATE_GRAY                   0x7412
#define C_LIGHT_SLATE_GRAY             0x7453
#define C_LIGHT_STEEL_BLUE             0xAE1B
#define C_LAVENDER                     0xE73E
#define C_FLORAL_WHITE                 0xFFDD
#define C_ALICE_BLUE                   0xEFBF
#define C_GHOST_WHITE                  0xF7BF
#define C_HONEYDEW                     0xEFFD
#define C_IVORY                        0xFFFD
#define C_AZURE                        0xEFFF
#define C_SNOW                         0xFFDE
#define C_DIM_GRAY                     0x6B4D
#define C_GRAY                         0x8410
#define C_DARK_GRAY                    0xAD55
#define C_SILVER                       0xBDF7
#define C_LIGHT_GRAY                   0xD69A
#define C_GAINSBORO                    0xDEDB
#define C_WHITE_SMOKE                  0xF7BE
#define C_WHITE                        0xFFFF
#define C_BLACK                        0x0000

#ifdef __cplusplus
}
#endif
#endif
