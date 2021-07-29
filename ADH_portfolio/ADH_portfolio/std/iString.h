#pragma once

class iString
{
public:
	iString();
	iString(const char* str);
	iString(const iString& str);

	virtual ~iString();

	const char* operator + (const char* str);
	const char* operator + (const iString& str);

	iString& operator += (const char* str);
	iString& operator += (const iString& str);

	void add(const char* str);
	void set(const char* str);

	static char** getStringLine(const char* str, int& lineNum, int d = '\n');
	static void freeStringLine(char** line, int& lineNum);

public:
	char* str;
	int strLength;
};