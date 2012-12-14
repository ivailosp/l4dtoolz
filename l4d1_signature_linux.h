const char* server_dll = "server_i486.so";
const char* engine_dll = "engine_i486.so";
const char* matchmaking_dll = "matchmaking_ds_i486.so";

//CBaseServer::GetMaxHumanPlayers(void)const
//fuction is in engine_i486.so
const char* friends_lobby = "\x1B\x53\xE8\x00\x00\x00\x00\x5B\x81\xC3\xC3\xC3\xC3\xC3\x83\xEC\x08\x8B\x83\xC3\xC3\xC3\xC3\x8B\x10\x85\xD2\x74";
unsigned char friends_lobby_new[] = {0x06, 0x00, 0xB8, 0x3C, 0x00, 0x00, 0x00, 0xC3};
char* friends_lobby_org = NULL;

//CBaseServer::ConnectClient(netadr_s &, int, int, int, char  const*, char  const*, char  const*, int, CUtlVector<CLC_SplitPlayerConnect, CUtlMemory<CLC_SplitPlayerConnect, int>> &, bool)
//fuction is in engine_i486.so
const char* lobby_sux = "\x0B\x8B\x85\x78\x01\x00\x00\x85\xC0\x74\xC3\x8B";
const char* lobby_sux_new = "\x02\x09\x90\x90";
char* lobby_sux_org = NULL;

//CBaseServer::ConnectClient(netadr_s &, int, int, int, char  const*, char  const*, char  const*, int, CUtlVector<CLC_SplitPlayerConnect, CUtlMemory<CLC_SplitPlayerConnect, int>> &, bool)
//fuction is in engine_i486.so
const char* max_players = "\x13\x89\x2C\xC3\xFF\xC3\xC3\x8D\x14\x37\x29\xC2\x3B\x95\x78\x01\x00\x00\x0F\x8F";
char max_players_new[]= {0x06, 0x11, 0x83, 0xFA, 0x3C, 0x90, 0x90, 0x90};
char* max_players_org = NULL;

//CServerGameClients::GetMaxHumanPlayers(void)
//fuction is in server_i486.so
const char* server_bplayers = "\x1C\x53\xE8\x00\x00\x00\x00\x5B\x81\xC3\xC3\xC3\xC3\xC3\x83\xEC\x08\xE8\xC3\xC3\xC3\xC3\x59\x3C\x01\x5A\x19\xC0\x5B";
unsigned char server_bplayers_new[] = {0x06, 0x00, 0xB8, 0x3C, 0x00, 0x00, 0x00, 0xC3};
char* server_bplayers_org = NULL;


//CTerrorGameRules::ClientConnected(edict_t *, char  const*, char  const*, char *, int)
//fuction is in server_i486.so
const char* human_limit = "\x15\x8B\x6C\xC3\xC3\x8B\x75\x00\x89\x2C\x24\xFF\x96\xC3\xC3\xC3\xC3\x39\xF8\x7E\xC3\x8B";
const char* human_limit_new = "\x02\x12\x90\x90";
char* human_limit_org = NULL;

//_ZL10maxplayersRK8CCommand
//fuction is in engine_i486.so
const char* players = "\x13\x8B\xB3\xC3\xC3\xC3\xC3\x83\x7E\xC3\xC3\x7F\xC3\x8B\x90\xC3\xC3\x00\x00\x89";
const char* players_new = "\x02\x0A\x90\x90";
char* players_org = NULL;

//CGameServer::SetMaxClients(int)
//fuction is in engine_i486.so
const char* players2 = "\x19\x8B\x54\xC3\xC3\x8B\x86\xC3\xC3\xC3\xC3\x39\xD0\x7C\xC3\x8B\x86\xC3\xC3\xC3\xC3\x39\xC2\x0F\x4D\xC2";
const char* players_new2 = "\x02\x0C\x90\x90";
char* players_org2 = NULL;

//CBaseServer::SetReservationCookie(unsigned long long, char  const*, ...)
//function in engine_i486.so
const char* unreserved =  "\x31\x55\x57\x56\x53\x81\xEC\x3C\x01\x00\x00\xE8\x00\x00\x00\x00\x5B\x81\xC3\xC3\xC3\xC3\xC3\x8B\x84\x24\xC3\xC3\x00\x00\x8B\xB4\x24\xC3\xC3\x00\x00\x89\xC3\xC3\xC3\x8B\xAC\x24\xC3\xC3\x00\x00\x89\xC2";
const char* unreserved_new = "\x01\x00\xC3";
char* unreserved_org = NULL;

const char* lobby_match = "\x06\xB8\x08\x00\x00\x00\xC3";
unsigned char lobby_match_new[] = {0x01, 0x01, 0xC3};
char* lobby_match_org = NULL;
