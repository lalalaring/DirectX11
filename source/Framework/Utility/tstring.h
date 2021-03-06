#ifndef __TSTRING_H__ONE__
#define __TSTRING_H__ONE__


// tcharに対応したstringを定義している
#include<string>
#include<tchar.h>

typedef std::basic_string<_TCHAR, std::char_traits<_TCHAR>, std::allocator<_TCHAR> > tstring;

#endif