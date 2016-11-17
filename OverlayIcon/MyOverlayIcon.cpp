// MyOverlayIcon.cpp : CMyOverlayIcon µÄÊµÏÖ

#include "stdafx.h"
#include "MyOverlayIcon.h"

#include <iostream>
#include <string>
#include <locale.h> 
#include <fstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <time.h>
using namespace std;

vector<string> *file_list = NULL;

time_t last = 0;

void read_file()
{
	 time_t now = time(NULL);
	 if(now-last< 1*60)
	 {
		 return;
	 }
	last = now;
	if(file_list == NULL)
		file_list = new vector<string>;
	if(file_list == NULL)
		return;
	file_list->clear();
    ifstream fin("c:/list.txt");
	if( fin.fail() )
		return;
    string str;
    char Arr[1024];

    while (!fin.eof())
    {
        fin.getline(Arr,1024);
        
		str = string(Arr);
		transform(str.begin(),str.end(),str.begin(),::tolower);
		if( str!="<file>" && str!="<dir>" )
			file_list->push_back(str);
    }
    fin.close();
}

string WstringToString(const wstring& str)
{
    unsigned len = str.size() * 4;
    setlocale(LC_CTYPE, "");
    char *p = new char[len];
    wcstombs(p,str.c_str(),len);
    std::string str1(p);
    delete[] p;
    return str1;
}

bool find_file(wchar_t *s)
{
	read_file();
	wstring file = wstring(s);
	string str = WstringToString(file);
	transform(str.begin(),str.end(),str.begin(),::tolower);
	for(int i=0; i<file_list->size(); i++)
	{
		if( str.find(file_list->at(i)) != string::npos )
			return true;
	}
	return false;
}

// CMyOverlayIcon
// IShellIconOverlayIdentifier::GetOverlayInfo
// returns The Overlay Icon Location to the system
STDMETHODIMP CMyOverlayIcon::GetOverlayInfo(
             LPWSTR pwszIconFile,
             int cchMax,
             int* pIndex,
             DWORD* pdwFlags)
{
  // Get our module's full path
  GetModuleFileNameW(_AtlBaseModule.GetModuleInstance(), pwszIconFile, cchMax);

  // Use first icon in the resource
  *pIndex=0; 

  *pdwFlags = ISIOI_ICONFILE | ISIOI_ICONINDEX;
  return S_OK;
}

// IShellIconOverlayIdentifier::GetPriority
// returns the priority of this overlay 0 being the highest. 
STDMETHODIMP CMyOverlayIcon::GetPriority(int* pPriority)
{
  // we want highest priority 
  *pPriority=0;
  return S_OK;
}

// IShellIconOverlayIdentifier::IsMemberOf
// Returns whether the object should have this overlay or not 
STDMETHODIMP CMyOverlayIcon::IsMemberOf(LPCWSTR pwszPath, DWORD dwAttrib)
{
  wchar_t *s = _wcsdup(pwszPath);
  HRESULT r = S_FALSE;
  
  _wcslwr(s);

  // Criteria
 if ( find_file(s) )
    r = S_OK;

  free(s);

  return r;
}

