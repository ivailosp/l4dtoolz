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

	static void* max_players_friend_lobby;
	static void* max_players_connect;
	static void* max_players_server_browser;
	static void* lobby_sux_ptr;
	static void* chuman_limit;
	static void* tmp_player;
	static void* tmp_player2;
	static void* unreserved_ptr;
	static void* lobby_match_ptr;
private:

#if !defined METAMOD_PLAPI_VERSION
	SourceHook::CallClass<IVEngineServer> *m_EngineCC;
#endif
};


extern l4dtoolz g_l4dtoolz;

PLUGIN_GLOBALVARS();

#endif //_INCLUDE_METAMOD_SOURCE_STUB_PLUGIN_H_
