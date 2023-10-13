#include "memory.h"
#include <iostream>
#include "skStr.h"
i_memory& i_memory::reeq()
{
	static i_memory ins;
	return ins;
}


void i_memory::initialize()
{
	//using namespace KeyAuth;
	//std::string name = skCrypt("fivem").decrypt(); // application name. right above the blurred text aka the secret on the licenses tab among other tabs
	//std::string ownerid = skCrypt("MReGlTDoYM").decrypt(); // ownerid, found in account settings. click your profile picture on top right of dashboard and then account settings.
	//std::string secret = skCrypt("d6bdf96cde9bb74076cd929713dbd332204106092e3bb17af5e392f9cfc8530e").decrypt(); // app secret, the blurred text on licenses tab and other tabs
	//std::string version = skCrypt("1.3").decrypt(); // leave alone unless you've changed version on website
	//std::string url = skCrypt("http://smartmember.ir/api/1.2/").decrypt(); // change if you're self-hosting
	//api KeyAuthApp(name, ownerid, secret, version, url);

	//KeyAuthApp.init();
	//if (!KeyAuthApp.data.success)
	//{
	//	std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
	//	Sleep(1500);
	//	exit(0);
	//}

	//std::ifstream file(skCrypt("C:\\check.txt"));
	//std::string license;
	//std::getline(file, license);

	//KeyAuthApp.license(license);

	//if (!KeyAuthApp.data.success)
	//{
	//	std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
	//	Sleep(1500);
	//	exit(0);
	//}

	//bool ok = false;
	//for (std::string subs : KeyAuthApp.data.subscriptions)
	//{
	//	if (subs == "FiveM")
	//	{
	//		ok = true;
	//	}
	//}

	//if (!ok)
	//{
	//	std::cout << skCrypt("\n invalid sub");
	//	Sleep(1500);
	//	exit(0);
	//}

	//KeyAuthApp.check();

	//if (!KeyAuthApp.data.success)
	//{
	//	std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
	//	Sleep(1500);
	//	exit(0);
	//}

	window = FindWindowA("grcWindow", 0);
	if (!window)
	{
		std::cout << "Fivem not found (start fivem before load)";
	}
	GetWindowThreadProcessId(window, &pid);
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);


}



bool i_memory::find_build(const wchar_t* processName) {

	bool exists = false;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry))
		while (Process32Next(snapshot, &entry))
			if (!_wcsicmp(entry.szExeFile, processName))
				exists = true;

	CloseHandle(snapshot);
	return exists;
}



uintptr_t i_memory::get_base_address(std::string a1)
{
	std::wstring pname = std::wstring(a1.begin(), a1.end());
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	if (hSnap == INVALID_HANDLE_VALUE)
		return 0;
	MODULEENTRY32 ent;
	ent.dwSize = sizeof(ent);
	if (Module32First(hSnap, &ent))
	{
		do
		{
			if (!_wcsicmp(ent.szModule, pname.c_str()))
			{
				std::wcout << "Build : " << ent.szModule << std::endl;
				CloseHandle(hSnap);
				return (uintptr_t)ent.modBaseAddr;
			}
		} while (Module32Next(hSnap, &ent));
	}
	CloseHandle(hSnap);
	return 0;
}


HWND i_memory::get_process_wnd(uint32_t pid)
{
	std::pair<HWND, uint32_t> params = { 0, pid };
	BOOL bResult = EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
		auto pParams = (std::pair<HWND, uint32_t>*)(lParam);
	uint32_t processId = 0;

	if (GetWindowThreadProcessId(hwnd, reinterpret_cast<LPDWORD>(&processId)) && processId == pParams->second) {
		SetLastError((uint32_t)-1);
		pParams->first = hwnd;
		return FALSE;
	}

	return TRUE;

		}, (LPARAM)&params);

	if (!bResult && GetLastError() == -1 && params.first)
		return params.first;

	return NULL;
}
