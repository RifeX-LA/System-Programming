#include "dll.h"
#include <stdio.h>

BOOLEAN GetAuthor(LPSTR buffer, DWORD dwBufferSize, DWORD *pdwBytesWritten) {
    int result = _snprintf_s(buffer, dwBufferSize, 20, "Timur Borisenko");
    *pdwBytesWritten = result;
    return result;
}

BOOLEAN GetDescription(LPSTR buffer, DWORD dwBufferSize, DWORD *pdwBytesWritten) {
    int result = _snprintf_s(buffer, dwBufferSize, 17, "Shows system info");
    *pdwBytesWritten = result;
    return result;
}

VOID Execute() {
    system("msinfo32");
    printf("\n");
}