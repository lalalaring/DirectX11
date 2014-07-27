#ifndef __WINDOW_DEFINE_H__ONE__
#define __WINDOW_DEFINE_H__ONE__

#include<tchar.h>

// Brambling Framework.
#define CLASS_NAME	_T("Bb_Framework")	// �E�B���h�E�N���X�̖��O

//----- �萔��` -----//
enum WINDOW_TYPE
{
	WINDOW_TYPE_WINDOW,
	WINDOW_TYPE_FULLSCREEN	// �t���X�N���[�����[�h
};

// �K���Ƀf�B�X�v���C���[�h��
enum DISPLAY_MODE
{
	DISPLAY_MODE_VGA_640x480,
	DISPLAY_MODE_SVGA_800x600,
	DISPLAY_MODE_XGA_1024x768,
	DISPLAY_MODE_SXGA_1280x1024,
	DISPLAY_MODE_FHD_1920x1080,
};

// TODO:�萔�ł͂Ȃ��Q�[�����ɐ؂�ւ�����悤����
//const WINDOW_TYPE g_wndMode = WINDOW_MODE_WINDOW_640x480;

#endif