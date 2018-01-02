#pragma once
#include "stdafx.h"
class crypt {
private:
	HCRYPTKEY keyHandle;
	std::wstring keyContainerPath;
	HCRYPTKEY xchgKeyHandle;
	DWORD keyBlobLen;
	HCRYPTPROV provPtr;
public:
	crypt();
	void initializeContainer();
	~crypt();
	void destroyKeys();
	void encryptBuffer(byte *buf, DWORD bufLen, BOOL isLastBlock);
	void decryptBuffer(byte *buf, DWORD bufLen, BOOL isLastBlock);
	BOOL writeKeyToFile();
	BOOL importKeyFromFile();
};