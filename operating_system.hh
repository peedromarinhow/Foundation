#pragma once

#include "foundation.hh"

/*
 * OS LAYER
 */

//////////////////////////////////////
//.doc: SETUP
internal void OsMainInit(int Argc, char **Argv);
internal str_list OsCmdLineArgs(void);



//////////////////////////////////////
//.doc: MEMORY
internal void *OsMemRes(size Size);
internal b32   OsMemCom(void *Ptr, size Size);
internal void  OsMemDec(void *Ptr, size Size);
internal void  OsMemRel(void *Ptr, size Size);



//////////////////////////////////////
//.doc: FILES
internal str OsReadFile(arena *Arena, str Filename);
internal b32 OsWriteFileDataList(str Filename, str_list Data);
internal b32 OsWriteFile(str Filename, str Data);

internal file_props OsGetFileProperties(str Filename);

internal b32 OsDeleteFile(str Filename);
internal b32 IsRenameFile(arena *Arena, str Filename, str NewName);
internal b32 OsMakeDir(str Path);
internal b32 OsDeleteDir(str Path);

// file iterator
struct os_file_iterator {
  byte v[640];
};
internal os_file_iterator OsFileIteratorInit(arena *Arena, str Path);
internal b32  OsFileIteratorNext(os_file_iterator *Iter, str *NameOut, file_props *PropOut);
internal void OsFileIteratorEnd(os_file_iterator *Iter);

// paths
enum os_path {
  os_path_CurrentDir,
  os_path_Program,
  os_path_UserData,
  os_path_TempData,
  os_path_Count,
};
internal str OsFilePath(arena *Arena, os_path Path);



//////////////////////////////////////
//.doc: TIME
internal date_time OsNowUniversalTime(void);
internal date_time OsLocalTimeFromUniversal(date_time *DateTime);
internal date_time OsUniversalTimeFromLocal(date_time *DateTime);

internal u64 OsNowMicroSecods(void);
internal void OsSleepMilliSeconds(u64 t);



//////////////////////////////////////
//.doc: LIBRARIES
struct os_lib {
  u64 v[1];
};
internal os_lib OsLoadLib(str Path);
internal voidf *OsGetProc(os_lib Lib, c8 *Name);
internal void   OsFreeLib(os_lib Lib);



//////////////////////////////////////
//.doc: MISC
internal void OsGetRandom(void *Data, size Size);



#if OS_WINDOWS
  #include "win32.cc"
#else
  #error "no backend for this os was implemented"
#endif
