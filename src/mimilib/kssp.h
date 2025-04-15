/*Benjamin DELPY `gentilkiwi`
https://blog.gentilkiwi.com
benjamin@gentilkiwi.com
Licence : https://creativecommons.org/licenses/by/4.0/
*/
#pragma once
#include "utils.h"
#include <sspi.h>
#include <ntsecpkg.h>

NTSTATUS NTAPI kssp_SpLsaModeInitialize(ULONG LsaVersion, PULONG PackageVersion, PSECPKG_FUNCTION_TABLE *ppTables, PULONG pcTables);
NTSTATUS NTAPI kssp_SpInitialize(ULONG_PTR PackageId, PSECPKG_PARAMETERS Parameters, PLSA_SECPKG_FUNCTION_TABLE FunctionTable);
NTSTATUS NTAPI kssp_SpGetInfo(PSecPkgInfoW PackageInfo);
NTSTATUS NTAPI kssp_SpShutDown(void);
NTSTATUS NTAPI kssp_LogonUserExW(ClientRequest,LogonType,AuthenticationInformation,ClientAuthenticationBase,AuthenticationInformationLength,ProfileBuffer,ProfileBufferLength,LogonId,SubStatus,TokenInformationType,TokenInformation,AccountName,AuthenticatingAuthority,MachineName);
NTSTATUS NTAPI kssp_LogonUserEx2(ClientRequest,
LogonType,
ProtocolSubmitBuffer,
ClientBufferBase,
SubmitBufferSize,
ProfileBuffer,
ProfileBufferSize,
LogonId,
SubStatus,
TokenInformationType,
TokenInformation,
AccountName,
AuthenticatingAuthority,
MachineName,
PrimaryCredentials,
SupplementalCredentials);

NTSTATUS NTAPI kssp_LogonUser(
	ClientRequest,
	LogonType,
	AuthenticationInformation,
	ClientAuthenticationBase,
	AuthenticationInformationLength,
	ProfileBuffer,
	ProfileBufferLength,
	LogonId,
	SubStatus,
	TokenInformationType,
	TokenInformation,
	AccountName,
	AuthenticatingAuthority);



	NTSTATUS NTAPI kssp_LsaApCallPakage(
		IN PLSA_CLIENT_REQUEST ClientRequest,
		IN PVOID ProtocolSubmitBuffer,
		IN PVOID ClientBufferBase,
		IN ULONG SubmitBufferLength,
		OUT PVOID * ProtocolReturnBuffer,
		OUT PULONG ReturnBufferLength,
		OUT PNTSTATUS ProtocolStatus
	);

NTSTATUS NTAPI kssp_SpAcceptCredentials(SECURITY_LOGON_TYPE LogonType, PUNICODE_STRING AccountName, PSECPKG_PRIMARY_CRED PrimaryCredentials, PSECPKG_SUPPLEMENTAL_CRED SupplementalCredentials);