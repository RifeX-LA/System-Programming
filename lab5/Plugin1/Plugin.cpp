#include "dll.h"
#include <stdio.h>

BOOLEAN GetAuthor(LPSTR buffer, DWORD dwBufferSize, DWORD *pdwBytesWritten) {
    int result = _snprintf_s(buffer, dwBufferSize, 20, "Timur Borisenko");
    *pdwBytesWritten = result;
    return result;
}

BOOLEAN GetDescription(LPSTR buffer, DWORD dwBufferSize, DWORD *pdwBytesWritten) {
    int result = _snprintf_s(buffer, dwBufferSize, 17, "Shows system info!");
    *pdwBytesWritten = result;
    return result;
}

VOID Execute() {
    SYSTEM_INFO siSysInfo;
    GetSystemInfo(&siSysInfo);

    printf("Hardware information: \n");
    printf("  OEM ID: %u\n", siSysInfo.dwOemId);
    printf("  Number of processors: %u\n", siSysInfo.dwNumberOfProcessors);
    printf("  Page size: %u\n", siSysInfo.dwPageSize);
    printf("  Processor type: %u\n", siSysInfo.dwProcessorType);
    printf("  Minimum application address: %lx\n", siSysInfo.lpMinimumApplicationAddress);
    printf("  Maximum application address: %lx\n", siSysInfo.lpMaximumApplicationAddress);
    printf("  Active processor mask: %u\n", siSysInfo.dwActiveProcessorMask);
}