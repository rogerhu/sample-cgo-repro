package main

/*
 const int a=1;
 #include "osxkeychain.h"
*/
import "C"

import "fmt"

func main() {
	fmt.Println("hello")
	result := C.keychain_get(C.CString("engflow"))
	fmt.Printf(C.GoString(result))
}
