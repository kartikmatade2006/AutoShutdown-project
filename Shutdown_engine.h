#ifndef SHUTDOWN_ENGINE_H
#define SHUTDOWN_ENGINE_H

#include <windows.h>

// Handles low-level OS operations to force a safe system shutdown
inline bool ForceSystemShutdown() {
    HANDLE hToken; 
    TOKEN_PRIVILEGES tkp; 

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
        return false;

    // Fixed: Added [0] index to access the first element of the privileges array
    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid); 
    tkp.PrivilegeCount = 1;  
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0); 
    if (GetLastError() != ERROR_SUCCESS) return false;

    return ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCEIFHUNG, SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER);
}

#endif // SHUTDOWN_ENGINE_H
