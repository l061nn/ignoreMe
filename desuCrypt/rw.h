#pragma once
#include "stdafx.h"
class file {
private:
	const std::wstring encExt = L".desucrpt";
public:
	file();
	~file();
	void startProcedureOnFile(std::wstring file, crypt *cryptObj);
	void decryptProcedure(std::wstring file, crypt *cryptObj);

};
