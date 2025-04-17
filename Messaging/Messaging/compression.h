#pragma once
#include "miniz.h"
#include <vector>
#include <iostream>

class zInt
{
public:
	zInt();
	~zInt();
	std::vector<unsigned char> myCompress(std::vector<unsigned char> _msg);
	std::vector<unsigned char> myDecompress(std::vector<unsigned char> _msg);

private:
	mz_ulong bufferLen;
	unsigned char* pBuf;
};