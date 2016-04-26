/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】
	AUTH :	大便一箩筐																			   DATE : 2016-04-25
	DESC :	提供兼容string与wstring的tstring类型
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