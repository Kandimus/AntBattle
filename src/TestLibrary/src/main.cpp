#include "export.h"
#include <windows.h>

#include "PlayerInfo.h"
#include "AntInfo.h"

#ifdef __cplusplus
extern "C"
{
#endif

BOOL WINAPI DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}

TESTLIBRARY_PUBLIC_EXPORT
void antInit(AntBattle::PlayerInfo* pi)
{
	if (!pi) {
		return;
	}

	pi->apiVersion = AntBattle::ANTBATTLE_API_VERSION;
	pi->teamName = "Test black team";
	pi->type = AntBattle::AntType::Black;
	pi->version = "0.1";
}


TESTLIBRARY_PUBLIC_EXPORT
void antFinalize(void)
{

}

TESTLIBRARY_PUBLIC_EXPORT
void workerProcess(AntBattle::AntInfo* ai, AntBattle::Command* cmd)
{
	cmd->count = 1;
	cmd->type = AntBattle::CommandType::Explore;
}

TESTLIBRARY_PUBLIC_EXPORT
void solderProcess(AntBattle::AntInfo* ai, AntBattle::Command* cmd)
{
	cmd->count = 1;
	cmd->type = AntBattle::CommandType::Explore;
}

TESTLIBRARY_PUBLIC_EXPORT
void queenProcess(AntBattle::AntInfo* ai, AntBattle::Command* cmd)
{
	cmd->count = 1;
	cmd->type = AntBattle::CommandType::Idle;
}

#ifdef __cplusplus
} // __cplusplus defined.
#endif
