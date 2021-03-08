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
	strcpy(r, str);
	strcpy(&r[l], _str);
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
	strcpy(r, str);
	strcpy(&r[l], _str.str);
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
		strcpy(s, str);
		delete str;
		str = s;
		strLength = len + 1;
	}
	strcpy(&str[l], szText);
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
	strcpy(str, szText);
}

