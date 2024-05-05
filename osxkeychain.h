#include <Security/Security.h>

const char *keychain_get(char *name);
int keychain_store(char *name, char *value);
