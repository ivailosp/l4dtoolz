#ifndef _INCLUDE_L4D1_SIGNATURE_LINUX_
#define _INCLUDE_L4D1_SIGNATURE_LINUX_

const char* server_dll[] = {"server.so", 0};
const char* engine_dll[] = {"engine.so", 0};
const char* matchmaking_dll[] = {"matchmaking_ds.so", "matchmaking.so", 0};

//CBaseServer::GetMaxHumanPlayers(void)const
//fuction is in engine.so
const char* friends_lobby = "\x16\x56\x53\x83\xEC\xC3\xE8\xC3\xC3\xC3\xC3\x81\xC3\xC3\xC3\xC3\xC3\x8B\x74\xC3\xC3\x8B\x83";
unsigned char friends_lobby_new[] = {0x06, 0x00, 0xB8, 0x3C, 0x00, 0x00, 0x00, 0xC3};
void* friends_lobby_org = NULL;

//CBaseServer::ConnectClient(netadr_s &, int, int, int, char  const*, char  const*, char  const*, int, CUtlVector<CLC_SplitPlayerConnect, CUtlMemory<CLC_SplitPlayerConnect, int>> &, bool)
//fuction is in engine.so
const char* lobby_sux = "\x0A\x8B\x86\x78\x01\x00\x00\x85\xC0\x0F\x85";
const char* lobby_sux_new = "\x06\x00\xB8\x01\x00\x00\x00\x90";
void* lobby_sux_org = NULL;

//CBaseServer::ConnectClient(netadr_s &, int, int, int, char  const*, char  const*, char  const*, int, CUtlVector<CLC_SplitPlayerConnect, CUtlMemory<CLC_SplitPlayerConnect, int>> &, bool)
//fuction is in engine.so
const char* max_players = "\x0F\xFF\x50\xC3\x29\xC7\x01\xEF\x3B\xC3\xC3\xC3\x00\x00\x0F\x8E";
unsigned char max_players_new[]= {0x06, 0x07, 0x83, 0xFF, 0x3C, 0x90, 0x90, 0x90};
void* max_players_org = NULL;

//CServerGameClients::GetMaxHumanPlayers(void)
//fuction is in server.so
const char* server_bplayers = "\x1F\x53\x83\xEC\xC3\xE8\xC3\xC3\xC3\xC3\x81\xC3\xC3\xC3\xC3\xC3\x8B\x83\xC3\xC3\xC3\xC3\x8B\x00\x85\xC0\x74\x10\x8B\x10\x89\x04";
unsigned char server_bplayers_new[] = {0x06, 0x00, 0xB8, 0x3C, 0x00, 0x00, 0x00, 0xC3};
void* server_bplayers_org = NULL;

//CTerrorGameRules::ClientConnected(edict_t *, char  const*, char  const*, char *, int)
//fuction is in server.so
const char* human_limit = "\x12\x8B\x54\xC3\xC3\x8B\x02\x89\x14\xC3\xFF\x90\xC3\x02\x00\x00\x39\xF8\x7E";
const char* human_limit_new = "\x02\x11\x90\x90";
void* human_limit_org = NULL;

//_ZL10maxplayersRK8CCommand
//fuction is in engine.so
const char* players = "\x13\x83\xBB\xC3\xC3\xC3\xC3\x01\x7F\xC3\x8B\x80\x0C\xC3\xC3\x00\x89\xC3\xC3\xE8";
const char* players_new = "\x02\x07\x90\x90";
void* players_org = NULL;

const char* players2 = "\x13\x83\xBB\xC3\xC3\xC3\xC3\x01\x7F\xC3\x8B\x80\x0C\xC3\xC3\x00\x89\xC3\xC3\xE8";
const char* players_new2 = "\x03\x1D\x89\xC2\xEB";
void* players_org2 = NULL;

//CBaseServer::SetReservationCookie(unsigned long long, char  const*, ...)
//function in engine.so
const char* unreserved = "\x1F\x55\x57\x56\x53\x81\xEC\xC3\xC3\x00\x00\xE8\xC3\xC3\xC3\xC3\x81\xC3\xC3\xC3\xC3\xC3\x8B\xB4\x24\x60\x01\x00\x00\x8B\xAC\x24";
const char* unreserved_new = "\x01\x00\xC3";
void* unreserved_org = NULL;

const char* lobby_match = "\x06\xB8\x08\x00\x00\x00\xC3";
unsigned char lobby_match_new[] = {0x01, 0x02, 0xC3};
void* lobby_match_org = NULL;

#endif //_INCLUDE_L4D1_SIGNATURE_LINUX_

