#pragma once
#include <windows.h>
#include <winternl.h>
#include <stdio.h>

void* getfunction(const char* ntdllfunction, PVOID dllbase);
void* viapeb(const wchar_t* dllname);

