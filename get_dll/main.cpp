#include "getdll.hpp"
const wchar_t* dllname = L"ntdll";
const char* functionname = "NtOpenProcess";

int main()
{


	PVOID dlladdress = viapeb(dllname);
	if (dlladdress) {
		printf("dll base address %ls : %x", dllname, dlladdress);
		Sleep(3000);
		system("cls");

	}
	else {
		printf("address not found");
		return 0;
	}

	PVOID functionaddress = getfunction(functionname, dlladdress);

	if (functionaddress) {
		printf("address of the function %s : %x", functionname, functionaddress);
	
	}
	else {
		printf("%s address not found", functionname);
		return 0;
	}

}