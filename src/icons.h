
#ifndef ICONS_h
#define ICONS_h

/*
get PNG icons:
https://iconarchive.com/search?q=wifi&items=max&page=1&res=16&license=commercialfree

open it with XnView
  - convert to monochrome 1bit (nicht gerastert)
  - save as XBM
  - copy the hex code into here
*/



#include <pgmspace.h>  // PROGMEM support header

#define ICON_WIDTH 16
#define ICON_HEIGHT 16

// 'ETH_icon', 16x16px
const unsigned char ETH_icon [] PROGMEM = {
0xfe, 0x7f, 
0x03, 0xc0, 
0x01, 0x80, 
0x01, 0x80, 
0xc1, 0x83, 
0xc1, 0x83, 
0xf9, 0x9f, 
0xf9, 0x9f, 
0xf9, 0x9f, 
0xf9, 0x9f, 
0xd9, 0x9b, 
0xd9, 0x9b, 
0x01, 0x80, 
0x01, 0x80, 
0x03, 0xc0, 
0xfe, 0x7f, 
};

// 'WiFi_icon', 16x16px
const unsigned char WiFi_icon [] PROGMEM = {
0x00, 0x00, 
0x00, 0x00, 
0xf0, 0x0f, 
0xfc, 0x3f, 
0x0e, 0x70, 
0xe6, 0x67, 
0xf8, 0x1f, 
0x18, 0x18, 
0xc8, 0x13, 
0xe0, 0x07, 
0x20, 0x04, 
0x80, 0x01, 
0x80, 0x01, 
0x00, 0x00, 
0x00, 0x00, 
0x00, 0x00,
};

// 'AP_icon', 16x16px
const unsigned char AP_icon [] PROGMEM = {
0x00, 0x00, 
0x00, 0x00, 
0x04, 0x20, 
0x02, 0x40, 
0x0a, 0x50, 
0x25, 0xb4, 
0x95, 0xa9, 
0x55, 0xaa, 
0x55, 0xaa, 
0xd5, 0xab, 
0x25, 0xb4, 
0x0a, 0x50, 
0x02, 0x40, 
0x04, 0x20, 
0x00, 0x00, 
0x00, 0x00,  
};

// 'Warning_icon', 16x16px
const unsigned char Warning_icon [] PROGMEM = {
0x00, 0x00, 
0x80, 0x01, 
0xc0, 0x03, 
0xc0, 0x03, 
0xe0, 0x07, 
0x70, 0x0e, 
0x70, 0x0e, 
0x78, 0x1e, 
0x78, 0x1e, 
0xfc, 0x3f, 
0xfc, 0x3f, 
0x7e, 0x7e, 
0x7f, 0xfe, 
0xff, 0xff, 
0xfe, 0x7f, 
0x00, 0x00, 
};

// 'Error_icon', 16x16px
const unsigned char Error_icon [] PROGMEM = {
0x00, 0x00, 
0x00, 0x00, 
0xe0, 0x07, 
0xf0, 0x0f, 
0xf8, 0x1f, 
0xfc, 0x3f, 
0xfc, 0x3f, 
0x0c, 0x30, 
0x0c, 0x30, 
0x0c, 0x30, 
0xfc, 0x3f, 
0xfc, 0x3f, 
0xf8, 0x1f, 
0xf0, 0x0f, 
0xe0, 0x07, 
0x00, 0x00, 
};

// 'Info_icon', 16x16px
const unsigned char Info_icon [] PROGMEM = {
0x00, 0x00, 
0xe0, 0x07, 
0xf8, 0x1f, 
0xfc, 0x3f, 
0xfc, 0x3c, 
0xfe, 0x7e, 
0xfe, 0x7f, 
0x7e, 0x7e, 
0x7e, 0x7e, 
0x7e, 0x7e, 
0x7e, 0x7f, 
0x3c, 0x3e, 
0xfc, 0x3f, 
0xf8, 0x1f, 
0xe0, 0x07, 
0x00, 0x00,  
};

#endif // ICONS_h