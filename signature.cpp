#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "signature.h"

#ifdef WIN32
#include <windows.h>
#include <TlHelp32.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#include <link.h>
#endif

#define SIGN_HEADER_LEN		2
#define SIGN_LEN_BYTE		0
#define SIGN_OFFSET_BYTE	1


static int lock_region(const void *addr, unsigned int sign_len, int lock)
{
#ifndef WIN32
	unsigned int all_adr;
	unsigned int all_size;
	unsigned int p_size;
	unsigned int u_addr;

	p_size = sysconf(_SC_PAGESIZE);
	u_addr = (unsigned int)addr;

	all_adr = (u_addr + sign_len) & ~(p_size-1);
	all_size = u_addr - all_adr + sign_len;

	if(lock)
		mlock((void *)all_adr, all_size);
	else
		munlock((void *)all_adr, all_size);
#endif
	return 0;
}

void *find_signature(const char* mask, struct base_addr_t *base_addr, int pure)
{
	char *pBasePtr = (char *)base_addr->addr;
	char *pEndPtr = pBasePtr+base_addr->len-(int)mask[SIGN_LEN_BYTE];
	int i;
	char* tmp;
	if(base_addr == NULL)
		return NULL;

	lock_region(pBasePtr, pEndPtr-pBasePtr, 1);

	while(pBasePtr < pEndPtr)
	{
		tmp = pBasePtr;
		
		for(i = 1; i <= mask[SIGN_LEN_BYTE]; ++i) {
			if(pure && mask[i] == '\xC3'){
				tmp++;
				continue;
			}
			if(mask[i] != *tmp ) {
				break;
			}
			tmp++;
		}
		if(i-1 == mask[0]) {
			lock_region(pBasePtr, pEndPtr-pBasePtr, 0);
			return pBasePtr;
		}

		pBasePtr++;
	}

	lock_region(pBasePtr, pEndPtr-pBasePtr, 0);
	return NULL;
}

#ifndef WIN32
struct v_data{
	const char *fname;
	void *baddr;
	unsigned int blen;
};

static int callback(struct dl_phdr_info *info, size_t size, void *data)
{
	int i;
	const char* filename;

	if (!info->dlpi_name || !info->dlpi_name[0])
		return 0;

	filename = ((struct v_data *)data)->fname;

	if(strstr(info->dlpi_name, filename)) {
		if(strstr( info->dlpi_name, "metamod") == NULL) {
			((struct v_data *)data)->baddr = (void*)info->dlpi_addr;
			((struct v_data *)data)->blen = 0;
			for(i = 0; i < info->dlpi_phnum; ++i) {
				((struct v_data *)data)->blen+=info->dlpi_phdr[i].p_filesz;
				break;
			}
			return 1;
		}
	}
	return 0;
}
#endif

int find_base(const char* name, struct base_addr_t *base_addr)
{
#ifdef WIN32
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 modent;
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
	if(hModuleSnap == INVALID_HANDLE_VALUE) {
		return 0;
	}

	modent.dwSize = sizeof(MODULEENTRY32);

	if(!Module32Next(hModuleSnap, &modent)){
		CloseHandle(hModuleSnap);
		return 0;
	}
	do {
		if(strstr( modent.szExePath, name)) {
			if(strstr( modent.szExePath, "metamod"))
					continue;
			base_addr->addr = modent.modBaseAddr;
			base_addr->len = modent.modBaseSize;
			CloseHandle(hModuleSnap);
			return 1;
		}
	} while(Module32Next(hModuleSnap, &modent));
	CloseHandle(hModuleSnap);
#else
	struct v_data vdata;
	vdata.fname = name;
	if(dl_iterate_phdr(callback, &vdata)){
		base_addr->addr = vdata.baddr;
		base_addr->len = vdata.blen;
		return 1;
	}
#endif
	base_addr->addr = NULL;
	base_addr->len = 0;
	return 0;

}

int write_signature(const void* addr, const void* signature)
{
	if(!addr)
		return 0;

	unsigned int u_addr_sign;
	unsigned int sign_len;
	unsigned int sign_off;
	unsigned int u_addr;

	sign_len = ((unsigned char *)signature)[SIGN_LEN_BYTE];
	sign_off = ((unsigned char *)signature)[SIGN_OFFSET_BYTE];
	u_addr = (unsigned int)addr;
	u_addr_sign = (unsigned int)signature;

#ifdef WIN32
	HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());
	WriteProcessMemory(hProcess, (void *)(u_addr+sign_off), (void *)(u_addr_sign+SIGN_HEADER_LEN), sign_len, NULL);
	CloseHandle(h_process);
#else

	lock_region(addr, sign_len, 1);
	memcpy((void *)(u_addr+sign_off), (void *)(u_addr_sign+SIGN_HEADER_LEN), sign_len);
	lock_region(addr, sign_len, 0);

#endif
	return 1;
}

int read_signature(const void *addr, void *signature)
{
	unsigned int u_addr_sign;
	unsigned int sign_len;
	unsigned int u_addr;

	sign_len = ((unsigned char *)signature)[SIGN_LEN_BYTE];
	u_addr = (unsigned int)addr;
	u_addr_sign = (unsigned int)signature;

#ifdef WIN32
	HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());
	ReadProcessMemory(h_process, (void *)(u_addr+sign_len), (void *)(u_addr_sign+SIGN_HEADER_LEN), sign_len, NULL);
	CloseHandle(h_process);
#else
	lock_region(addr, sign_len, 1);
	memcpy((void *)(u_addr_sign+SIGN_HEADER_LEN), (void *)(u_addr+sign_len), sign_len);
	lock_region(addr, sign_len, 0);
#endif
	return 0;
}

int get_original_signature(const void *offset, const void *new_sig, void **org_sig)
{
	unsigned int sign_len;

	if(!offset)
		return 0;

	sign_len = ((unsigned char *)new_sig)[SIGN_LEN_BYTE];
	*org_sig = malloc(sign_len + SIGN_HEADER_LEN);
	memcpy(org_sig, new_sig, SIGN_HEADER_LEN);
	return read_signature(offset, org_sig);
}

