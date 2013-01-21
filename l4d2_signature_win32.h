#ifndef _INCLUDE_L4D2_SIGNATURE_WIN32_
#define _INCLUDE_L4D2_SIGNATURE_WIN32_

const char* server_dll = "server.dll";
const char* engine_dll = "engine.dll";
const char* matchmaking_dll = "matchmaking_ds.dll";
const char* matchmaking_dll_alt = "matchmaking.dll";

const char* friends_lobby = "\x12\x56\x8B\xF1\x8B\x0D\xC3\xC3\xC3\xC3\x85\xC9\x74\xC3\x8B\x01\x8B\x50\x48";
unsigned char friends_lobby_new[] = {0x06, 0x00, 0xB8, 0x3C, 0x00, 0x00, 0x00, 0xC3};
char* friends_lobby_org = NULL;

const char* max_players = "\x28\x83\xBE\xC3\xC3\x00\x00\x00\x74\xC3\x8B\x54\xC3\xC3\x8B\x06\x8B\x7A\xC3\x8B\x50\x10\x8B\xCE\xFF\xD2\x2B\xF8\x8B\x06\x8B\x50\x08\x8B\xCE\xFF\xD2\x03\xC7\x3B\x86";
unsigned char max_players_new[]= {0x06, 0x26, 0x83, 0xF8, 0x3C, 0x90, 0x90, 0x90};
char* max_players_org = NULL;

const char* lobby_sux_new = "\x02\x07\x90\x90";
char* lobby_sux_org = NULL;

const char* server_bplayers = "\x1D\x56\x8B\xF1\xE8\xC3\xC3\xC3\xC3\x8B\x4C\x24\x08\x89\x01\x8B\x0D\xC3\xC3\xC3\xC3\x85\xC9\x74\xC3\x8B\x11\x8B\x42\x48";
unsigned char server_bplayers_new[] = {0x05, 0x1A, 0xB8, 0x3C, 0x00, 0x00, 0x00};
char* server_bplayers_org = NULL;

const char* human_limit = "\x15\x8B\x13\x8B\x82\xC3\xC3\x00\x00\x8B\xCB\xFF\xD0\x3B\xF8\x7C\xC3\x8B\xC3\xC3\xC3\x8B";
const char* human_limit_new = "\x01\x0E\xEB";
char* human_limit_org = NULL;

const char* players = "\x15\x83\x3D\xC3\xC3\xC3\xC3\x02\x7C\xC3\x83\xC4\x08\xC7\x44\xC3\xC3\xC3\xC3\xC3\xC3\xFF";
const char* players_new = "\x01\x07\xEB";
char* players_org = NULL;

const char* players2 = "\x10\x56\x8B\xF1\x8B\x86\xC3\x02\x00\x00\x8B\x4C\x24\x08\x3B\xC8\x7F";
const char* players_new2 = "\x02\x0F\x90\x90";
char* players_org2 = NULL;


const char* unreserved = "\x1E\x81\xEC\xC3\xC3\x00\x00\x55\x8B\xAC\xC3\xC3\xC3\x00\x00\x56\x8B\xB4\xC3\xC3\xC3\x00\x00\x57\x8B\xBC\xC3\xC3\xC3\x00\x00";
const char* unreserved_new = "\x01\x00\xC3";
char* unreserved_org = NULL;

const char* lobby_match = "\x06\xB8\x08\x00\x00\x00\xC3";
unsigned char lobby_match_new[] = {0x01, 0x01, 0xC3};
char* lobby_match_org = NULL;

#endif //_INCLUDE_L4D2_SIGNATURE_WIN32_

