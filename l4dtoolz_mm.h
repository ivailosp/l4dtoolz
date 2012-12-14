#ifndef _INCLUDE_METAMOD_SOURCE_STUB_PLUGIN_H_
#define _INCLUDE_METAMOD_SOURCE_STUB_PLUGIN_H_

#include <ISmmPlugin.h>


class l4dtoolz : public ISmmPlugin, public IMetamodListener
{
public:
	bool Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late);
	bool Unload(char *error, size_t maxlen);
	bool Pause(char *error, size_t maxlen);
	bool Unpause(char *error, size_t maxlen);

	void OnVSPListening(IServerPluginCallbacks *iface);
public:
	const char *GetAuthor();
	const char *GetName();
	const char *GetDescription();
	const char *GetURL();
	const char *GetLicense();
	const char *GetVersion();
	const char *GetDate();
	const char *GetLogTag();
public:

	static void OnChangeMaxplayers ( IConVar *var, const char *pOldValue, float flOldValue );
	static void OnChangeRemovehumanlimit ( IConVar *var, const char *pOldValue, float flOldValue );
	static void OnChangeUnreserved ( IConVar *var, const char *pOldValue, float flOldValue );
	static void OnChangeIvailosp ( IConVar *var, const char *pOldValue, float flOldValue );

	static char* max_players_friend_lobby;
	static char* max_players_connect;
	static char* max_players_server_browser;
	static char* lobby_sux_ptr;
	static char* chuman_limit;
	static char* tmp_player;
	static char* tmp_player2;
	static char* unreserved_ptr;
	static char* lobby_match_ptr;

public:
	static bool find_base(const char* name, char*& base_addr, size_t& base_len);
	static void* FindSignature(const char* mask, const char* base_addr, size_t base_len, bool pure_sign = false);
	static void get_org_sig(const char* offset, const char* new_sig, char*& org_sig);
	static void WriteSignature(char* addr, const char* signature);
	static void ReadSignature(const char* addr, char* signature);
private:

#if !defined METAMOD_PLAPI_VERSION
	SourceHook::CallClass<IVEngineServer> *m_EngineCC;
#endif

#ifdef WIN32
private:
	static HANDLE hProcess;
#endif
};


extern l4dtoolz g_l4dtoolz;

PLUGIN_GLOBALVARS();

#endif //_INCLUDE_METAMOD_SOURCE_STUB_PLUGIN_H_
