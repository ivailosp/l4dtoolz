#ifndef _INCLUDE_L4D2_SIGNATURE_LINUX_
#define _INCLUDE_L4D2_SIGNATURE_LINUX_

const char* server_dll[] = {"server_srv.so", "server.so", 0};
const char* engine_dll[] = {"engine_srv.so", "engine.so", 0};
const char* matchmaking_dll[] = {"matchmaking_ds_srv.so", "matchmaking_srv.so", "matchmaking_ds.so", "matchmaking.so", 0};

const char* lobby_sux = "\x13\xFF\x50\xC3\x84\xC0\x0F\x84\xC3\xC3\xC3\xC3\x8B\xB3\xC3\x01\x00\x00\x85\xF6";
const char* lobby_sux_new = "\x06\x0B\xBE\x01\x00\x00\x00\x90";
void *lobby_sux_org = NULL;

const char* max_players = "\x16\x89\xC6\x8B\x03\x89\x1C\x24\xFF\x50\xC3\x29\xC6\x03\x75\xC3\x3B\xB3\xC3\x01\x00\x00\x0F";
unsigned char max_players_new[]= {0x06, 0x0F, 0x83, 0xFE, 0x3C, 0x90, 0x90, 0x90};
void *max_players_org = NULL;

const char* server_bplayers = "\x16\x55\x89\xE5\x83\xEC\x08\xE8\xC3\xC3\xC3\xC3\xC9\x3C\x01\x19\xC0\x83\xE0\xFC\x83\xC0\x08";
unsigned char server_bplayers_new[] = {0x06, 0x00, 0xB8, 0x3C, 0x00, 0x00, 0x00, 0xC3};
void *server_bplayers_org = NULL;

const char* players = "\x13\x83\x3D\xC3\xC3\xC3\xC3\x01\x7F\xC3\x8B\x80\xC3\xC3\x00\x00\x89\x04\x24\xE8";
const char* players_new = "\x02\x07\x90\x90";
void *players_org = NULL;

const char* players2 = "\x23\x55\x89\xE5\x53\x83\xEC\xC3\x8B\x5D\x08\x8B\x55\x0C\x8B\x83\xC3\xC3\x00\x00\x39\xD0\x7C\x0B\x8B\x83\xC3\xC3\x00\x00\x39\xC2\x0F\x4D\xC2\x8B";
const char* players_new2 = "\x01\x13\x89\xD0\xEB";
void *players_org2 = NULL;

const char* unreserved = "\x1D\x55\x89\xE5\x81\xEC\xC3\xC3\x00\x00\x89\x5D\xF4\x8B\x5D\x08\x89\x75\xF8\x8B\x55\x10\x89\x7D\xFC\x8B\x75\xC3\x8B\x8B";
const char* unreserved_new = "\x01\x00\xC3";
void *unreserved_org = NULL;

const char* lobby_match = "\x0A\x55\xB8\x08\x00\x00\x00\x89\xE5\x5D\xC3";
unsigned char lobby_match_new[] = {0x01, 0x02, 0xC3};
void *lobby_match_org = NULL;

#endif //_INCLUDE_L4D2_SIGNATURE_LINUX_

