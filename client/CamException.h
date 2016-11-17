#pragma once

#include "stdafx.h"
class CamException
{
public:
    CamException(void);
    ~CamException(void);
private:
   static  char* str2char(CString str);
    static char* CString2char(CString &str,DWORD *outCharLength);//CString转char，因为VS2005中的字符问题比较纠结
    static CString QueryExePath();//获得当前EXE程序的路径
     
public:
    static void WriteToFile(CString exMsg);//采用MFC的方法获取系统时间　并　写入文件
};