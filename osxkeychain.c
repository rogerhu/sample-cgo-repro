#include <CoreFoundation/CoreFoundation.h>
#include <Security/Security.h>
#include <stdio.h>
#include <string.h>

CFStringRef kEngFlowServiceName = CFSTR("com.example.secretstore");

const char *keychain_get(char *name) {
  // MacOS takes care of its own memory allocation, we just need to free it
  // later
  CFDataRef data = NULL;

  // Need to convert this C string to one used for Core Foundation
  CFStringRef secret_name = CFStringCreateWithBytesNoCopy(
      kCFAllocatorDefault, (unsigned char *)name, strlen(name),
      kCFStringEncodingUTF8, false, kCFAllocatorNull);

  CFMutableDictionaryRef query = CFDictionaryCreateMutable(NULL, 4, NULL, NULL);

  // Based on our implementation //services/secretstore/keychain.cc
  // Somewhat equivalent to running security find-generic-password -a
  // "com.engflow.secretstore"

  CFDictionaryAddValue(query, kSecClass, kSecClassGenericPassword);
  CFDictionaryAddValue(query, kSecAttrService, kEngFlowServiceName);
  CFDictionaryAddValue(query, kSecAttrAccount, secret_name);
  CFDictionaryAddValue(query, kSecReturnData, kCFBooleanTrue);

  OSStatus result = SecItemCopyMatching(query, (CFTypeRef *)&data);

  CFRelease(query);

  if (result != errSecSuccess) {
    return NULL;
  }

  /* We need to copy the contents from the CoreFoundation pointer. If we try to
   * use CFRelease(data), it will also deallocate the memory associated with the
   * returned pointer of CFDataGetBytePtr()..
   */
  CFIndex length = CFDataGetLength(data);
  char *secret = malloc(length);
  CFDataGetBytes(data, CFRangeMake(0, length), secret);

  CFRelease(data);

  return secret;
}
