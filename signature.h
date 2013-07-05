#ifndef _INCLUDE_SIGNATURE_
#define _INCLUDE_SIGNATURE_

struct base_addr_t{
	void *addr;
	unsigned int len;
};

void *find_signature(const char *mask, struct base_addr_t *base_addr, int pure);
int find_base(const char *name, struct base_addr_t *base_addr);
int find_base_from_list(const char *name[], struct base_addr_t *base_addr);
int write_signature(const void *addr, const void *signature);
int read_signature(const void *addr, void *signature);
int get_original_signature(const void *offset, const void *new_sig, void *&org_sig);

#endif //_INCLUDE_SIGNATURE_
