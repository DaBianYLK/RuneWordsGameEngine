/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��
	AUTH :	���һ���																			   DATE : 2016-04-25
	DESC :	�ṩ����string��wstring��tstring����
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <string>

namespace Rwge
{
#ifdef _UNICODE
	typedef std::wstring	tstring;
#else
	typedef std::string		tstring;
#endif
}