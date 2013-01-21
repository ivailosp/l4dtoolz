#include <stdio.h>
#include "l4dtoolz_mm.h"

#ifdef WIN32
#include <windows.h>
#include <TlHelp32.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#include <link.h>
#endif

#ifdef WIN32
#ifdef L4D1
#include "l4d1_signature_win32.h"
#else
#include "l4d2_signature_win32.h"
#endif
#else
#ifdef L4D1
#include "l4d1_signature_linux.h"
#else
#include "l4d2_signature_linux.h"
#endif
#endif

l4dtoolz g_l4dtoolz;
IVEngineServer* engine = NULL;
IServerPluginCallbacks* vsp_callbacks = NULL;
ICvar* icvar = NULL;

#ifdef WIN32
HANDLE l4dtoolz::hProcess;
#endif

char* l4dtoolz::max_players_friend_lobby = NULL;
char* l4dtoolz::max_players_connect = NULL;
char* l4dtoolz::max_players_server_browser = NULL;
char* l4dtoolz::lobby_sux_ptr = NULL;
char* l4dtoolz::chuman_limit = NULL;
char* l4dtoolz::tmp_player = NULL;
char* l4dtoolz::tmp_player2 = NULL;
char* l4dtoolz::unreserved_ptr = NULL;
char* l4dtoolz::lobby_match_ptr = NULL;

ConVar sv_maxplayers("sv_maxplayers", "-1", 0, "Max Human Players", true, -1, true, 32, l4dtoolz::OnChangeMaxplayers);
ConVar sv_removehumanlimit("sv_removehumanlimit", "0", 0, "Remove Human limit reached kick", true, 0, true, 1, l4dtoolz::OnChangeRemovehumanlimit);
ConVar L4DToolZ("L4DToolZ", "",0,"L4DToolZ Author",l4dtoolz::OnChangeIvailosp);
ConVar sv_force_unreserved("sv_force_unreserved", "0", 0, "Disallow lobby reservation cookie", true, 0, true, 1, l4dtoolz::OnChangeUnreserved);

void* l4dtoolz::FindSignature(const char* mask, const char* base_addr, size_t base_len, bool pure_sign)
{
	char *pBasePtr = (char *)base_addr;
	char *pEndPtr = pBasePtr+base_len-(int)mask[0];
	int i;
	char* tmp;
	if(base_addr == NULL)
		return NULL;
#ifndef WIN32
	unsigned int p_size = sysconf(_SC_PAGESIZE);
	char* all_adr = (char*)((unsigned int)pBasePtr & ~(p_size-1));
	unsigned int size = pEndPtr - all_adr;
	mlock(all_adr, size);
#endif
	while(pBasePtr < pEndPtr)
	{
		tmp = pBasePtr;
		
		for(i = 1; i <= mask[0]; ++i){
			if(mask[i] == '\xC3' && pure_sign == false){
				tmp++;
				continue;
			}
			if(mask[i] != *tmp ){
				break;
			}
			tmp++;
		}
		if(i-1 == mask[0]){
#ifndef WIN32
			munlock(all_adr, size);
#endif
			return pBasePtr;
		}

		pBasePtr++;
	}
#ifndef WIN32
	munlock(all_adr, size);
#endif	
	return NULL;
}
#ifndef WIN32
struct v_data{
	char* fname;
	char* baddr;
	size_t blen;
};
static int callback(struct dl_phdr_info *info, size_t size, void *data)
{
	if (!info->dlpi_name || !info->dlpi_name[0]){
		return 0;
	}
	char* filename = ((v_data*)data)->fname;
	if(strstr(info->dlpi_name, filename)){
		if(strstr( info->dlpi_name, "metamod") == NULL){
			((v_data*)data)->baddr = (char*)info->dlpi_addr;
			((v_data*)data)->blen = 0;
			for(int j = 0; j < info->dlpi_phnum; ++j){
				((v_data*)data)->blen+=info->dlpi_phdr[j].p_filesz;
				break;
			}
			return 1;
		}
	}
	return 0;
}
#endif
bool l4dtoolz::find_base(const char* name, char*& base_addr, size_t& base_len){
	base_addr = NULL;
	base_len = 0;
#ifdef WIN32
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 modent;
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
	if(hModuleSnap == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	modent.dwSize = sizeof(MODULEENTRY32);

	if(!Module32Next(hModuleSnap, &modent))
	{
		CloseHandle(hModuleSnap);
		return false;
	}
	do
	{
		if(strstr( modent.szExePath, name))
		{
			if(strstr( modent.szExePath, "metamod"))
					continue;
			base_addr = (char*)modent.modBaseAddr;
			base_len = modent.modBaseSize;
			CloseHandle(hModuleSnap);
			return true;
		}
	} while(Module32Next(hModuleSnap, &modent));
	CloseHandle(hModuleSnap);
	return false;
#else
	v_data vdata;
	vdata.fname = (char*)name;
	if(dl_iterate_phdr(callback, &vdata)){
		base_addr = vdata.baddr;
		base_len = vdata.blen;
		return true;
	}else{
		base_addr = 0;
		base_len = 0;
		return false;
	}
#endif
}

void l4dtoolz::WriteSignature(char* addr, const char* signature){
#ifdef WIN32
	WriteProcessMemory(hProcess,addr+signature[1], signature+2, signature[0], NULL);
#else
	unsigned int p_size = sysconf(_SC_PAGESIZE);
	char* all_adr = (char*)(((unsigned int)addr + signature[1]) & ~(p_size-1));
	unsigned int size = addr - all_adr + signature[0];
	mlock(all_adr, size);
	mprotect(all_adr, size, PROT_READ|PROT_WRITE|PROT_EXEC);
	memcpy(addr+signature[1], signature+2, signature[0]);
	munlock(all_adr, size);
#endif
}

void l4dtoolz::ReadSignature(const char* addr, char* signature){
#ifdef WIN32
	ReadProcessMemory(hProcess, addr+signature[1], signature+2, signature[0], NULL);
#else
	unsigned int p_size = sysconf(_SC_PAGESIZE);
	char* all_adr = (char*)(((unsigned int)addr + signature[1]) & ~(p_size-1));
	unsigned int size = addr - all_adr + signature[0];
	mlock(all_adr, size);
	memcpy(signature+2, addr+signature[1], signature[0]);
	munlock(all_adr, size);
#endif
}

void l4dtoolz::OnChangeMaxplayers ( IConVar *var, const char *pOldValue, float flOldValue )
{
	if (max_players_friend_lobby == NULL || max_players_connect == NULL || max_players_server_browser == NULL || lobby_sux_ptr == NULL){
		Msg("sv_maxplayers init error\n");
		return;
	}
	if(((ConVar*)var)->GetInt() >= 0){
		max_players_new[4] = friends_lobby_new[3] = server_bplayers_new[3] = ((ConVar*)var)->GetInt();
		if(lobby_match_ptr != NULL){
			lobby_match_new[2] = ((ConVar*)var)->GetInt();
			WriteSignature(lobby_match_ptr, (const char*)lobby_match_new);
		}else{
			Msg("sv_maxplayers MS init error\n");
		}
		WriteSignature(max_players_friend_lobby, (const char*)friends_lobby_new);
		WriteSignature(max_players_connect, (const char*)max_players_new);
		WriteSignature(lobby_sux_ptr, lobby_sux_new);
		WriteSignature(max_players_server_browser, (const char*)server_bplayers_new);
	}else{
		WriteSignature(max_players_friend_lobby, friends_lobby_org);
		WriteSignature(max_players_connect, max_players_org);
		WriteSignature(lobby_sux_ptr, lobby_sux_org);
		WriteSignature(max_players_server_browser, server_bplayers_org);
		
		if(lobby_match_ptr != NULL)
			WriteSignature(lobby_match_ptr, lobby_match_org);
	}
}
void l4dtoolz::OnChangeRemovehumanlimit ( IConVar *var, const char *pOldValue, float flOldValue )
{
	if(chuman_limit == NULL){
		Msg( "sv_removehumanlimit init error\n");
		return;
	}
	if( ((ConVar*)var)->GetInt() == 1){
		WriteSignature(chuman_limit, human_limit_new);
	}else{
		WriteSignature(chuman_limit, human_limit_org);
	}
}


void l4dtoolz::OnChangeIvailosp ( IConVar *var, const char *pOldValue, float flOldValue )
{
	if(tmp_player == NULL || tmp_player2 == NULL){
		Msg("L4DToolZ init error\n");
		return;
	}
	WriteSignature(tmp_player, players_org);
	WriteSignature(tmp_player2, players_org2);
}
void l4dtoolz::OnChangeUnreserved ( IConVar *var, const char *pOldValue, float flOldValue )
{
	if(unreserved_ptr == NULL ){
		Msg("unreserved_ptr init error\n");
		return;
	}
	if( ((ConVar*)var)->GetInt() == 1){
		WriteSignature(unreserved_ptr, unreserved_new);
		engine->ServerCommand("sv_allow_lobby_connect_only 0\n");
	}else{
		WriteSignature(unreserved_ptr, unreserved_org);
	}
	
}

class BaseAccessor : public IConCommandBaseAccessor
{
public:
	bool RegisterConCommandBase(ConCommandBase *pCommandBase)
	{
		return META_REGCVAR(pCommandBase);
	}
} s_BaseAccessor;

PLUGIN_EXPOSE(l4dtoolz, g_l4dtoolz);

void l4dtoolz::get_org_sig(const char* offset, const char* new_sig, char*& org_sig){
	if(!offset)
		return;
	if(org_sig)
		delete org_sig;
	org_sig = new char[new_sig[0] + 2];
	org_sig[0] = new_sig[0];
	org_sig[1] = new_sig[1];
	ReadSignature(offset, org_sig);
}
bool l4dtoolz::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
#ifdef WIN32
	hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE, GetCurrentProcessId());
#endif
	PLUGIN_SAVEVARS();

	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer, INTERFACEVERSION_VENGINESERVER);
	GET_V_IFACE_CURRENT(GetEngineFactory, icvar, ICvar, CVAR_INTERFACE_VERSION);


#if defined METAMOD_PLAPI_VERSION
	if ((vsp_callbacks = ismm->GetVSPInfo(NULL)) == NULL)
#endif
	{
		ismm->AddListener(this, this);
		ismm->EnableVSPListener();
	}


#if !defined METAMOD_PLAPI_VERSION
	m_EngineCC = SH_GET_CALLCLASS(engine);
#endif

#if SOURCE_ENGINE >= SE_ORANGEBOX
	g_pCVar = icvar;
	ConVar_Register(0, &s_BaseAccessor);
#else
	ConCommandBaseMgr::OneTimeInit(&s_BaseAccessor);
#endif

	char* base_addr = NULL;
	size_t base_len = 0;

	find_base(matchmaking_dll, base_addr, base_len);

#ifdef WIN32
	if(base_addr == NULL)
		find_base("matchmaking.dll", base_addr, base_len);
#endif

	if(lobby_match_ptr == NULL){
		lobby_match_ptr = (char*)FindSignature(lobby_match, base_addr, base_len, true);
		get_org_sig(lobby_match, (const char*)lobby_match_new, lobby_match_org);
	}

	find_base(engine_dll, base_addr, base_len);
	if(max_players_friend_lobby == NULL){
		max_players_friend_lobby = (char*)FindSignature(friends_lobby, base_addr, base_len);
		get_org_sig(max_players_friend_lobby, (const char*)friends_lobby_new, friends_lobby_org);
	}
	if(max_players_connect == NULL){
		max_players_connect = (char*)FindSignature(max_players, base_addr, base_len);
		if(max_players_connect != NULL){
			get_org_sig(max_players_connect, (const char*)max_players_new, max_players_org);
		}
	}
	if(lobby_sux_ptr==NULL){

#ifdef WIN32
		lobby_sux_ptr = max_players_connect;
#else
		lobby_sux_ptr = (char*)FindSignature(lobby_sux, base_addr, base_len);
#endif
		if(lobby_sux_ptr!=NULL){
			get_org_sig(lobby_sux_ptr, lobby_sux_new, lobby_sux_org);
		}
	}
#ifdef WIN32
	if(max_players_server_browser == NULL){
		max_players_server_browser = (char*)FindSignature(server_bplayers, base_addr, base_len);
		get_org_sig(max_players_server_browser, (const char*)server_bplayers_new, server_bplayers_org);
	}
#endif
	if(tmp_player == NULL){
		tmp_player = (char*)FindSignature(players, base_addr, base_len);
		if(tmp_player != NULL){
#ifdef WIN32
			tmp_player2 = (char*)FindSignature(players2, base_addr, base_len);
#else
			tmp_player2 = tmp_player;
#endif
			if(tmp_player2 != NULL){
				get_org_sig(tmp_player, players_new, players_org);
				WriteSignature(tmp_player, players_new);
				get_org_sig(tmp_player2, players_new2, players_org2);
				WriteSignature(tmp_player2, players_new2);
				engine->ServerCommand("maxplayers 32\n");
				engine->ServerCommand("L4DToolZ ivailosp@abv.bg\n");
			}
		}
	}
	if(unreserved_ptr==NULL){
		unreserved_ptr = (char*)FindSignature(unreserved, base_addr, base_len);
		get_org_sig(unreserved_ptr, unreserved_new, unreserved_org);
	}

	find_base(server_dll, base_addr, base_len);

	if(chuman_limit == NULL){
		chuman_limit = (char*)FindSignature(human_limit, base_addr, base_len);
		get_org_sig(chuman_limit, human_limit_new, human_limit_org);
	}

#ifndef WIN32
	if(max_players_server_browser == NULL){
		max_players_server_browser = (char*)FindSignature(server_bplayers, base_addr, base_len);
		get_org_sig(max_players_server_browser, (const char*)server_bplayers_new, server_bplayers_org);
	}
#endif

	return true;
}

bool l4dtoolz::Unload(char *error, size_t maxlen)
{

#if !defined METAMOD_PLAPI_VERSION
	SH_RELEASE_CALLCLASS(m_EngineCC);
#endif

	if(max_players_friend_lobby != NULL)
		WriteSignature(max_players_friend_lobby, friends_lobby_org);
	if(max_players_connect != NULL)
		WriteSignature(max_players_connect, max_players_org);
	if(lobby_sux_ptr != NULL)
		WriteSignature(lobby_sux_ptr, lobby_sux_org);
	if(max_players_server_browser != NULL)
		WriteSignature(max_players_server_browser, server_bplayers_org);
	if(chuman_limit != NULL)
		WriteSignature(chuman_limit, human_limit_org);

	if(tmp_player != NULL)
		WriteSignature(tmp_player, players_org);
	if(tmp_player2 != NULL)
		WriteSignature(tmp_player2, players_org2);
	if(unreserved_ptr != NULL)
		WriteSignature(unreserved_ptr, unreserved_org);
	if(lobby_match_ptr != NULL)
		WriteSignature(lobby_match_ptr, lobby_match_org);

#ifdef WIN32
	CloseHandle(hProcess);
#endif

	delete[] friends_lobby_org;
	delete[] max_players_org;
	delete[] lobby_sux_org;
	delete[] server_bplayers_org;
	delete[] human_limit_org;
	delete[] players_org;
	delete[] players_org2;
	delete[] unreserved_org;
	return true;
}

void l4dtoolz::OnVSPListening(IServerPluginCallbacks *iface)
{
	vsp_callbacks = iface;
}


bool l4dtoolz::Pause(char *error, size_t maxlen)
{
	return true;
}

bool l4dtoolz::Unpause(char *error, size_t maxlen)
{
	return true;
}

const char *l4dtoolz::GetLicense()
{
	return "";
}

const char *l4dtoolz::GetVersion()
{
	return "1.0.0.9f";
}

const char *l4dtoolz::GetDate()
{
	return __DATE__;
}

const char *l4dtoolz::GetLogTag()
{
	return "L4DToolZ";
}

const char *l4dtoolz::GetAuthor()
{
	return "Ivailosp";
}

const char *l4dtoolz::GetDescription()
{
	return "Ivailosp plugin";
}

const char *l4dtoolz::GetName()
{
	return "L4DToolZ";
}

const char *l4dtoolz::GetURL()
{
	return "n/a";
}
