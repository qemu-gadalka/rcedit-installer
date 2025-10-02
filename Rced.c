#include <stdio.h>
#include <windows.h>

int IsRunningAsAdmin() {
    BOOL isAdmin = FALSE;
    HANDLE hToken = NULL;

    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION elevation;
        DWORD dwSize;

        if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize)) {
            isAdmin = elevation.TokenIsElevated;
        }
        CloseHandle(hToken);
    }

    return isAdmin;
}

int DownloadRcedit() {
    const char* powershell_cmd =
        "powershell -Command \"$ProgressPreference='SilentlyContinue'; "
        "Invoke-WebRequest -Uri 'https://github.com/electron/rcedit/releases/download/v2.0.0/rcedit-x64.exe' "
        "-OutFile 'C:\\Windows\\System32\\rcedit.exe'; "
        "$ProgressPreference='Continue'\"";

    return system(powershell_cmd);
}

int main() {
    if (!IsRunningAsAdmin()) {
        char exePath[MAX_PATH];
        GetModuleFileNameA(NULL, exePath, MAX_PATH);

        SHELLEXECUTEINFOA shExInfo = {0};
        shExInfo.cbSize = sizeof(SHELLEXECUTEINFOA);
        shExInfo.fMask = SEE_MASK_DEFAULT;
        shExInfo.hwnd = NULL;
        shExInfo.lpVerb = "runas";
        shExInfo.lpFile = exePath;
        shExInfo.lpParameters = "";
        shExInfo.lpDirectory = NULL;
        shExInfo.nShow = SW_SHOWNORMAL;

        if (!ShellExecuteExA(&shExInfo)) {

        }
        return 0;
    }

    int downloadResult = DownloadRcedit();

    if (downloadResult == 0) {
        if (GetFileAttributesA("C:\\Windows\\System32\\rcedit.exe") != INVALID_FILE_ATTRIBUTES) {

        } else {
            
            return 1;
        }
    } else {
        
        return 1;
    }

    return 0;
}
