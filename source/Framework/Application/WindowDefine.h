#ifndef __WINDOW_DEFINE_H__ONE__
#define __WINDOW_DEFINE_H__ONE__

#include<tchar.h>

// Brambling Framework.
#define CLASS_NAME	_T("Bb_Framework")	// ウィンドウクラスの名前

//----- 定数定義 -----//
enum WINDOW_TYPE
{
	WINDOW_TYPE_WINDOW,
	WINDOW_TYPE_FULLSCREEN	// フルスクリーンモード
};

// 適当にディスプレイモード列挙
enum DISPLAY_MODE
{
	DISPLAY_MODE_VGA_640x480,
	DISPLAY_MODE_SVGA_800x600,
	DISPLAY_MODE_XGA_1024x768,
	DISPLAY_MODE_SXGA_1280x1024,
	DISPLAY_MODE_FHD_1920x1080,
};

// TODO:定数ではなくゲーム中に切り替えられるようする
//const WINDOW_TYPE g_wndMode = WINDOW_MODE_WINDOW_640x480;

#endif