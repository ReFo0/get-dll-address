#pragma once
#include "getdll.hpp"

void* getfunction(const char* ntdllfunction, PVOID dllbase) {

	DWORD j;
	uintptr_t RVA = 0;

	const LPVOID basedlladdress = (LPVOID)dllbase;
	PIMAGE_DOS_HEADER pImgDOSHead = (PIMAGE_DOS_HEADER)basedlladdress;
	PIMAGE_NT_HEADERS pImgNTHead = (PIMAGE_NT_HEADERS)((DWORD_PTR)basedlladdress + pImgDOSHead->e_lfanew);
	PIMAGE_EXPORT_DIRECTORY pImgExpDir = (PIMAGE_EXPORT_DIRECTORY)((LPBYTE)basedlladdress + pImgNTHead->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

	PDWORD Address = (PDWORD)((LPBYTE)basedlladdress + pImgExpDir->AddressOfFunctions);

	PDWORD Name = (PDWORD)((LPBYTE)basedlladdress + pImgExpDir->AddressOfNames);

	PWORD Ordinal = (PWORD)((LPBYTE)basedlladdress + pImgExpDir->AddressOfNameOrdinals);

	for (j = 0; j < pImgExpDir->NumberOfNames; j++) {
		if (!strcmp(ntdllfunction, (char*)basedlladdress + Name[j])) {
			RVA = (uintptr_t)((LPBYTE)Address[Ordinal[j]]);
			break;
		}
	}

	if (RVA) {
		uintptr_t moduleBase = (uintptr_t)basedlladdress;
		uintptr_t* TrueAddress = (uintptr_t*)(moduleBase + RVA);
		return (PVOID)TrueAddress;
	}
	else {
		return (PVOID)RVA;
	}
}


void* viapeb(const wchar_t* dllname) {

	PPEB pPeb = 0;
	PLDR_DATA_TABLE_ENTRY pDataTableEntry = 0;
	PVOID dlladdress = 0;

#ifdef _M_X64
	PPEB pPEB = (PPEB)__readgsqword(0x60);
#else
	PPEB pPEB = (PPEB)__readfsdword(0x30);
#endif

	PPEB_LDR_DATA pLdr = pPEB->Ldr;
	PLIST_ENTRY AddressFirstPLIST = &pLdr->InMemoryOrderModuleList;
	PLIST_ENTRY AddressFirstNode = AddressFirstPLIST->Flink;

	for (PLIST_ENTRY Node = AddressFirstNode; Node != AddressFirstPLIST; Node = Node->Flink)
	{

		Node = Node - 1;

		pDataTableEntry = (PLDR_DATA_TABLE_ENTRY)Node;

		wchar_t* FullDLLName = (wchar_t*)pDataTableEntry->FullDllName.Buffer;

		for (int size = wcslen(FullDLLName), cpt = 0; cpt < size; cpt++) {
			FullDLLName[cpt] = tolower(FullDLLName[cpt]);
		}

		if (wcsstr(FullDLLName, dllname) != NULL) {
			dlladdress = (PVOID)pDataTableEntry->DllBase;
			return dlladdress;
		}

		Node = Node + 1;
	}

	return dlladdress;
}
