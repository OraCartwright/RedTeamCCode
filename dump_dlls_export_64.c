#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

VOID DumpListOfExport(VOID *lib);

VOID DumpListOfExport(VOID *lib) {
    DWORD dwIter = 0;
    CHAR* base = lib;
    CHAR* PE = base + (unsigned char)*(base + 0x3c);
    DWORD ExportDirectoryOffset = *((DWORD*)PE + (0x8a / 4));
    CHAR* ExportDirectory = base + ExportDirectoryOffset;
    DWORD dwFunctionsCount = *((DWORD*)ExportDirectory + (0x14 / 4));
    DWORD OffsetNamesTableOffset = *((DWORD*)ExportDirectory + (0x20 / 4));
    DWORD* OffsetNamesTable = base + OffsetNamesTableOffset;

    printf("------------------------------------------\nBASE\t\t\t0x%p\t%s\nPE\t\t\t0x%p\t%s\nExportTableOffset\t0x%p\nOffsetNameTable\t\t0x%p\nFunctions Count\t\t0x%x (%d)\n------------------------------------------\n",
    base, base, PE, PE, ExportDirectory, OffsetNamesTable, dwFunctionsCount, dwFunctionsCount);

    for(dwIter; dwIter < dwFunctionsCount - 1; dwIter++) {
        DWORD64 offset = *(OffsetNamesTable + dwIter);
        CHAR* current = base + offset;
        printf("%s\n", current);
    }
}

int main (int argc, char **argv) {
    CHAR *dll = argv[1];
    HANDLE hDll = LoadLibrary(dll);
    printf("Loading %s\n", dll);
    if(hDll == NULL) {
        ExitProcess(0);
    }

    DumpListOfExport(hDll);
    CloseHandle(hDll);
    printf("------------------------------------------\nCompleted\n");
    return 0;
}
