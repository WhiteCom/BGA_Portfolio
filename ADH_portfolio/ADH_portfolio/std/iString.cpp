#include "iString.h"
#include "iStd.h"

#define MAX_STRLEN 16

iString::iString()
{
	str = new char[MAX_STRLEN];
	strLength = MAX_STRLEN;
	str[0] = 0;
}

iString::iString(const char* _str)
{
	str = new char[MAX_STRLEN];
	strLength = MAX_STRLEN;
	str[0] = 0;

	set(_str);
}

iString::iString(const iString& _str)
{
	str = new char[MAX_STRLEN];
	strLength = MAX_STRLEN;
	str[0] = 0;

	set(_str.str);
}

iString::~iString()
{
	delete str;
}

static char* tmp_str = NULL;
static int tmp_strLen = -1;

const char* iString::operator + (const char* _str)
{
	int l = strlen(str);
	int len = l + strlen(_str);
	char* r = NULL;
	if (len > tmp_strLen)
	{
		if (tmp_str)
			delete tmp_str;
		tmp_str = new char[tmp_strLen = len + 1];
	}
	r = tmp_str;
#if 0
	strcpy(r, str);
	strcpy(&r[l], _str);
#else
	memcpy(r, str, l);
	r[l] = 0;
	memcpy(&r[l], _str, len - l);
	//r[len] = 0;
#endif
	r[len] = 0;
	return r;
}

const char* iString::operator + (const iString& _str)
{
	int l = strlen(str);
	int len = l +strlen(_str.str);

	char* r = NULL;
	if (len > tmp_strLen)
	{
		if (tmp_str)
			delete tmp_str;
		tmp_str = new char[tmp_strLen = len + 1];
	}
	r = tmp_str;
#if 0
	strcpy(r, str);
	strcpy(&r[l], _str);
#else
	memcpy(r, str, l);
	r[l] = 0;
	memcpy(&r[l], _str.str, len - l);
	//r[len] = 0;
#endif
	r[len] = 0;
	return r;
}

iString& iString::operator += (const char* _str)
{
	add(_str);
	return *this;
}
iString& iString::operator += (const iString& _str)
{
	add(_str.str);
	return *this;
}

void iString::add(const char* _str)
{
	char szText[256];
	va_start_end(szText, _str);

	int l = strlen(str);
	int len = l + strlen(szText);
	if (len > l)
	{
		char* s = new char[len + 1];
#if 0
		strcpy(s, str);
#else
		memcpy(s, str, l);
		s[l] = 0;
#endif
		delete str;
		str = s;
		strLength = len + 1;
	}
#if 0
	strcpy(&str[l], szText);
#else
	memcpy(&str[l], szText, len - l);
	str[len] = 0;
#endif
}

void iString::set(const char* _str)
{
	char szText[256];
	va_start_end(szText, _str);

	int l = strlen(str);
	int len = strlen(szText);
	if (len > l)
	{
		delete str;
		str = new char[strLength = len + 1];
	}
#if 0
	strcpy(str, szText);
#else
	memcpy(str, szText, len);
	str[len] = 0;
#endif
}

char** iString::getStringLine(const char* _str, int& lineNum, int d)
{
	int num = 1;

	int i, l, off, len = strlen(_str);
	for (i = 0; i < len; i++)
	{
		if (_str[i] == d)
			num++;
	}

	char** line = new char* [num];
	for (i = 0, l = 0, off = 0; i < len; i++)
	{
		if (_str[i] == d)
		{
			int n = i - off;
			char* s = new char[n + 1];
			memcpy(s, &_str[off], n);
			s[n] = 0;
			line[l] = s;
			l++;
			off = i + 1;
		}
	}
	int n = i - off;
	char* s = new char[n + 1];
	memcpy(s, &_str[off], n);
	s[n] = 0;
	line[l] = s;

	lineNum = num;
	return line;
}
void iString::freeStringLine(char** line, int& lineNum)
{
	for (int i = 0; i < lineNum; i++)
	{
		delete line[i];
	}
	delete line;
}

