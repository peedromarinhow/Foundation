/*
 * WIN32 LAYER 
 */

#undef Push
#include <windows.h>
#include <userenv.h>
#define Push(Arena, Size) ArenaPush((Arena), (Size))



global DWORD Win32ThreadContextIndex = 0;
global u64   Win32TicksPerSecond = 1;

global arena *Win32PermArena = {0};
global str8 Win32ProgramPath = {0};
global str8 Win32UserPath = {0};
global str8 Win32TempPath = {0};

global str_list Win32CmdLine = {0};



//////////////////////////////////////
// SETUP
internal void OsMainInit(int Argc, char **Argv) {
  LARGE_INTEGER PerfFrequency = {0};
  if (QueryPerformanceFrequency(&PerfFrequency))
    Win32TicksPerSecond = ((u64)PerfFrequency.HighPart << 32) | PerfFrequency.LowPart;
  timeBeginPeriod(1);
  
  Win32PermArena = ArenaMake();
  
  for (i32 i = 0; i < Argc; i++) {
    str8 Arg = strCStr((u8*)Argv[i]);
    PushToStrList(Win32PermArena, &Win32CmdLine, Arg);
  }

  temp_mem Scratch = GetScratch(null, 0);
  {
    DWORD Cap = 2048;
    u16 *Buffer = null;
    DWORD Size = 0;
    for (u64 r = 0; r < 4; r++, Cap *= 4) {
      u16 *TryBuffer = PushArray(Scratch.Arena, u16, Cap);
      DWORD TrySize = GetModuleFileNameW(0, (WCHAR*)TryBuffer, Cap);
      if (TrySize == Cap && GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        TempMemEnd(Scratch);
      else {
        Buffer = TryBuffer;
        Size = TrySize;
        break;
      }
    }
    str8 FullPath = Str8FromStr16(Scratch.Arena, str16_(Buffer, Size));
    str8 ProgramPath = ChopStrAfterLastSlash(FullPath);
    Win32ProgramPath = PushStrCopy(Win32PermArena, ProgramPath);
  }

  {
    HANDLE Token = GetCurrentProcessToken();
    DWORD Cap = 2048;
    u16 *Buffer = PushArray(Scratch.Arena, u16, Cap);
    if (!GetUserProfileDirectoryW(Token, (WCHAR*)Buffer, &Cap)) {
      TempMemEnd(Scratch);
      Buffer = PushArray(Scratch.Arena, u16, Cap);
      if (!GetUserProfileDirectoryW(Token, (WCHAR*)Buffer, &Cap))
        Buffer = null;
    }
    if (Buffer != 0)
      Win32UserPath = Str8FromStr16(Win32PermArena, str16CStr(Buffer));
  }

  {
    DWORD Cap = 2048;
    u16 *Buffer = PushArray(Scratch.Arena, u16, Cap);
    DWORD Size = GetTempPathW(Cap, (WCHAR*)Buffer);
    if (Size > Cap) {
      TempMemEnd(Scratch);
      Buffer = PushArray(Scratch.Arena, u16, Size + 1);
      Size = GetTempPathW(Size + 1, (WCHAR*)Buffer);
    }
    Win32TempPath = Str8FromStr16(Win32PermArena, str16_(Buffer, Size - 1));
  }
  ReleaseScratch(Scratch);
}

internal void Win32MainInit(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPSTR     lpCmdLine,
  int       nShowCmd) {
  int    Argc = __argc;
  char **Argv = __argv;
  OsMainInit(Argc, Argv);
}

internal str_list OsCmdLineArgs(void) {
  str_list Res = Win32CmdLine;
  return Res;
}



//////////////////////////////////////
// MEMORY
internal void *OsMemRes(size Size) {
  return VirtualAlloc(0, Size, MEM_RESERVE, PAGE_READWRITE);
  
}
internal b32 OsMemCom(void *Ptr, size Size) {
  return (VirtualAlloc(Ptr, Size, MEM_COMMIT, PAGE_READWRITE) != null);
}
internal void OsMemDec(void *Ptr, size Size) {
  VirtualFree(Ptr, Size, MEM_DECOMMIT);
}
internal void OsMemRel(void *Ptr, size Size) {
  VirtualFree(Ptr, 0, MEM_RELEASE);
}



//////////////////////////////////////
// TIME HELPERS
internal date_time Win32DateTimeFromSystemTime(SYSTEMTIME *SystemTime) {
  date_time Res = {0};
  Res.Year = SystemTime->wYear;
  Res.Month = (u8)SystemTime->wMonth;
  Res.Day = SystemTime->wDay;
  Res.Hour = SystemTime->wHour;
  Res.Minute = SystemTime->wMinute;
  Res.Second = SystemTime->wSecond;
  Res.Millisecond = SystemTime->wMilliseconds;
  return Res;
}
internal SYSTEMTIME Win32SystemTimeFromDateTime(date_time *DateTime) {
  SYSTEMTIME Res = {0};
  Res.wYear = DateTime->Year;
  Res.wMonth = DateTime->Month;
  Res.wDay = DateTime->Day;
  Res.wHour = DateTime->Hour;
  Res.wMinute = DateTime->Minute;
  Res.wSecond = DateTime->Second;
  Res.wMilliseconds = DateTime->Millisecond;
  return Res;
}
internal dense_time Win32DenseTimeFromFiletime(FILETIME *FileTime) {
  SYSTEMTIME SystemTime = {0};
  FileTimeToSystemTime(FileTime, &SystemTime);
  date_time DateTime = Win32DateTimeFromSystemTime(&SystemTime);
  dense_time Res = DenseTimeFromDate(&DateTime);
  return Res;
}



//////////////////////////////////////
// FILES
internal file_propfl Win32FilePropertyFlagsFromAttributes(DWORD Attributes) {
  file_propfl Res = file_prop_None;
  if (Attributes & FILE_ATTRIBUTE_DIRECTORY)
    Res |= file_prop_IsFolder;
  return Res;
}
internal data_accessfl Win32DataAccessFlagsFromAttributes(DWORD Attributes) {
  data_accessfl Res = 0;
  if (!(Attributes & FILE_ATTRIBUTE_READONLY))
    Res |= data_accessfl_Write;
  return Res;
}

internal str8 OsReadFile(arena *Arena, str8 Filename) {
  temp_mem Scratch = GetScratch(&Arena, 1);
  str16 FilenameUtf16 = Str16FromStr8(Scratch.Arena, Filename);
  FilenameUtf16.Str[FilenameUtf16.Len] = null;
  HANDLE File = CreateFileW((WCHAR*)FilenameUtf16.Str, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  str8 Result = {0};
  if (File != INVALID_HANDLE_VALUE) {
    DWORD HiSize = 0;
    DWORD LoSize = GetFileSize(File, &HiSize);
    u64 TotalSize = (((u64)HiSize) << 32) | (u64)LoSize;

    temp_mem RestorePoint = TempMemBegin(Arena);
    byte *Buffer = PushArray(Arena, byte, TotalSize);

    byte *Ptr = Buffer;
    byte *OnePastLast = Buffer + TotalSize;
    b32 Success = true;
    while (Ptr < OnePastLast) {
      u64 TotalToRead = (u64)(OnePastLast - Ptr);
      DWORD ToRead = (DWORD)TotalToRead;
      if (TotalToRead > u32_Max)
        ToRead = u32_Max;
      DWORD ActuallyRead = 0;
      if (!ReadFile(File, Ptr, ToRead, &ActuallyRead, 0)) {
        Success = false;
        break;
      }
      Ptr += ActuallyRead;
    }
    
    if (Success) {
      Result.Str = Buffer;
      Result.Len = TotalSize;
    }
    else
      TempMemEnd(RestorePoint);
    CloseHandle(File);
  }
  ReleaseScratch(Scratch);
  return Result;
}
internal b32 OsWriteFileDataList(str8 Filename, str_list Data) {
  temp_mem Scratch = GetScratch(null, 0);
  str16 FilenameUtf16 = Str16FromStr8(Scratch.Arena, Filename);
  HANDLE File = CreateFileW((WCHAR*)FilenameUtf16.Str, GENERIC_WRITE, 0, 0,
    CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
  b32 Success = false;
  if (File != INVALID_HANDLE_VALUE) {
    Success = true;
    
    for (str_node *Node = Data.First; Node != null; Node = Node->Next) {
      byte *Ptr = Node->Str.Str;
      byte *OnePastLast = Ptr + Node->Str.Len;
      while (Ptr < OnePastLast) {
        u64 TotalToWrite = (u64)(OnePastLast - Ptr);
        DWORD ToWrite = TotalToWrite;
        if (TotalToWrite > u32_Max)
          ToWrite = u32_Max;
        DWORD ActuallyWritten = 0;
        if (!WriteFile(File, Ptr, ToWrite, &ActuallyWritten, 0)) {
          Success = false;
          goto DOUBLE_LOOP_BREAK;
        }
        Ptr += ActuallyWritten;
      }
    }
    DOUBLE_LOOP_BREAK:
      CloseHandle(File);
  }
  ReleaseScratch(Scratch);
  return Success;
}
internal b32 OsWriteFile(str8 Filename, str8 Data) {
  str_node Node = {0};
  str_list List = {0};
  PushNodeToStrList(&List, &Node, Data);
  b32 Success = OsWriteFileDataList(Filename, List);
  return Success;
}

// file properties
internal file_props OsGetFileProperties(str8 Filename) {
  temp_mem Scratch = GetScratch(null, 0);
  str16 FilenameUtf16 = Str16FromStr8(Scratch.Arena, Filename);
  file_props Res = {0};
  WIN32_FILE_ATTRIBUTE_DATA Attributes = {0};
  if (GetFileAttributesExW((WCHAR*)FilenameUtf16.Str, GetFileExInfoStandard, &Attributes)) {
    Res.Size = ((u64)Attributes.nFileSizeHigh << 32) | (u64)Attributes.nFileSizeLow;
    Res.Flags = Win32FilePropertyFlagsFromAttributes(Attributes.dwFileAttributes);
    Res.CreateTime = Win32DenseTimeFromFiletime(&Attributes.ftCreationTime);
    Res.ModifyTime = Win32DenseTimeFromFiletime(&Attributes.ftLastWriteTime);
    Res.Access = Win32DataAccessFlagsFromAttributes(Attributes.dwFileAttributes);
  }
  ReleaseScratch(Scratch);
  return Res;
}

// file manipulation
internal b32 OsDeleteFile(str8 Filename) {
  temp_mem Scratch = GetScratch(null, 0);
  str16 FilenameUtf16 = Str16FromStr8(Scratch.Arena, Filename);
  b32 Res = DeleteFileW((WCHAR*)FilenameUtf16.Str);
  ReleaseScratch(Scratch);
  return Res;
}
internal b32 OsRenameFile(arena *Arena, str8 OldName, str8 NewName) {
  temp_mem Scratch = GetScratch(null, 0);
  str16 OldNameUtf16 = Str16FromStr8(Scratch.Arena, OldName);
  str16 NewNameUtf16 = Str16FromStr8(Scratch.Arena, NewName);
  b32 Res = MoveFileW((WCHAR*)OldNameUtf16.Str, (WCHAR*)NewNameUtf16.Str);
  ReleaseScratch(Scratch);
  return Res;
}
internal b32 OsMakeDir(str8 Path) {
  temp_mem Scratch = GetScratch(null, 0);
  str16 PathUtf16 = Str16FromStr8(Scratch.Arena, Path);
  b32 Res = CreateDirectoryW((WCHAR*)PathUtf16.Str, 0);
  ReleaseScratch(Scratch);
  return Res;
}
internal b32 OsDeleteDir(str8 Path) {
  temp_mem Scratch = GetScratch(null, 0);
  str16 PathUtf16 = Str16FromStr8(Scratch.Arena, Path);
  b32 Res = RemoveDirectoryW((WCHAR*)PathUtf16.Str);
  ReleaseScratch(Scratch);
  return Res;
}

// file iterator
typedef struct _win32_file_iterator {
  HANDLE Handle;
  WIN32_FIND_DATAW FindData;
  b32 Done;
} win32_file_iterator;
StaticAssert(sizeof(win32_file_iterator) <= sizeof(os_file_iterator), os_file_iterator);
internal os_file_iterator OsFileIteratorInit(arena *Arena, str8 Path) {
  str_node Nodes[2];
  str_list List = {0};
  PushNodeToStrList(&List, Nodes + 0, Path);
  PushNodeToStrList(&List, Nodes + 1, strl("\\*"));
  temp_mem Scratch = GetScratch(null, 0);
  str8 PathStar = JoinStrList(Scratch.Arena, &List);
  str16 PathUtf16 = Str16FromStr8(Scratch.Arena, PathStar);
  os_file_iterator Res = {0};
  win32_file_iterator *Win32Iter = (win32_file_iterator *)&Res;
  Win32Iter->Handle = FindFirstFileW((WCHAR*)PathUtf16.Str, &Win32Iter->FindData);
  ReleaseScratch(Scratch);
  return Res;
}
internal b32 OsFileIteratorNext(arena *Arena, os_file_iterator *Iter, str8 *NameOut, file_props  *PropOut) {
  b32 Res = false;
  win32_file_iterator *Win32Iter = (win32_file_iterator *)Iter;
  if (Win32Iter->Handle != 0 && Win32Iter->Handle != INVALID_HANDLE_VALUE) {
    while (!Win32Iter->Done) {
      WCHAR *Filename = Win32Iter->FindData.cFileName;
      b32 IsDot = Filename[0] == '.' && Filename[1] == 0;
      b32 IsDotDot = Filename[0] == '.' && Filename[1] == '.' && Filename[2] == 0;
      b32 Emit = !IsDot && !IsDotDot;
      WIN32_FIND_DATAW Data = {0};
      if (Emit)
        CopyStruct(&Data, &Win32Iter->FindData);
      if (!FindNextFileW(Win32Iter->Handle, &Win32Iter->FindData))
        Win32Iter->Done = true;
      if (Emit) {
        *NameOut = Str8FromStr16(Arena, str16CStr((c16*)Data.cFileName));
        PropOut->Size = ((u64)Data.nFileSizeHigh << 32) | (u64)Data.nFileSizeLow;
        PropOut->Flags = Win32FilePropertyFlagsFromAttributes(Data.dwFileAttributes);
        PropOut->CreateTime = Win32DenseTimeFromFiletime(&Data.ftCreationTime);
        PropOut->ModifyTime = Win32DenseTimeFromFiletime(&Data.ftLastWriteTime);
        PropOut->Access = Win32DataAccessFlagsFromAttributes(Data.dwFileAttributes);
        Res = true;
        break;
      }
    }
  }
  return Res;
}
internal void OsFileIteratorEnd(os_file_iterator *Iter) {
  win32_file_iterator *Win32Iter = (win32_file_iterator *)Iter;
  if (Win32Iter->Handle != 0 && Win32Iter->Handle != INVALID_HANDLE_VALUE)
    FindClose(Win32Iter->Handle);
}

// paths
internal str8 OsFilePath(arena *Arena, os_path Path) {
  str8 Res = {0};
  if (Path == os_path_CurrentDir) {
    temp_mem Scratch = GetScratch(&Arena, 1);
    DWORD Cap = 2048;
    u16 *Buffer = PushArray(Scratch.Arena, u16, Cap);
    DWORD Size = GetCurrentDirectoryW(Cap, (WCHAR*)Buffer);
    if (Size >= Cap) {
      TempMemEnd(Scratch);
      Buffer = PushArray(Scratch.Arena, u16, Size + 1);
      GetCurrentDirectoryW(Size + 1, (WCHAR*)Buffer);
    }
    Res = Str8FromStr16(Arena, str16_(Buffer, Size));
    ReleaseScratch(Scratch);
  } 
  else
  if (Path == os_path_Program) Res = PushStrCopy(Arena, Win32ProgramPath);
  else
  if (Path == os_path_UserData) Res = PushStrCopy(Arena, Win32UserPath);
  else
  if (Path == os_path_TempData) Res = PushStrCopy(Arena, Win32TempPath);
  return Res;
}



//////////////////////////////////////
// TIME
internal date_time OsNowUniversalTime(void) {
  SYSTEMTIME SystemTime = {0};
  GetSystemTime(&SystemTime);
  date_time Res = Win32DateTimeFromSystemTime(&SystemTime);
  return Res;
}
internal date_time OsLocalTimeFromUniversal(date_time *DateTime) {
  SYSTEMTIME UniversalSystemTime = Win32SystemTimeFromDateTime(DateTime);
  FILETIME UniversalFileTime = {0};
  SystemTimeToFileTime(&UniversalSystemTime, &UniversalFileTime);
  FILETIME LocalFileTime = {0};
  FileTimeToLocalFileTime(&UniversalFileTime, &LocalFileTime);
  SYSTEMTIME LocalSystemTime = {0};
  FileTimeToSystemTime(&LocalFileTime, &LocalSystemTime);
  date_time Res = Win32DateTimeFromSystemTime(&LocalSystemTime);
  return Res;
}
internal date_time OsUniversalTimeFromLocal(date_time *DateTime) {
  SYSTEMTIME LocalSystemTime = Win32SystemTimeFromDateTime(DateTime);
  FILETIME LocalFileTime = {0};
  SystemTimeToFileTime(&LocalSystemTime, &LocalFileTime);
  FILETIME UniversalFileTime = {0};
  FileTimeToLocalFileTime(&LocalFileTime, &UniversalFileTime);
  SYSTEMTIME UniversalSystemTime = {0};
  FileTimeToSystemTime(&UniversalFileTime, &UniversalSystemTime);
  date_time Res = Win32DateTimeFromSystemTime(&UniversalSystemTime);
  return Res;
}

internal u64 OsNowMicroseconds(void) {
  u64 Res = 0;
  LARGE_INTEGER PerfCounter = {0};
  if (QueryPerformanceCounter(&PerfCounter)) {
    u64 Ticks = ((u64)PerfCounter.HighPart << 32) | PerfCounter.LowPart;
    Res = Ticks*Million(1)/Win32TicksPerSecond;
  }
  return Res;
}
internal void OsSleepMilliseconds(u32 t) { Sleep(t); }



//////////////////////////////////////
// LIBRARIES
internal os_lib OsLoadLib(str8 Path) {
  os_lib Res = {0};
  temp_mem Scratch = GetScratch(null, 0);
  str16 Path16 = Str16FromStr8(Scratch.Arena, Path);
  Res.v[0] = (u64)(LoadLibraryW((WCHAR*)Path16.Str));
  ReleaseScratch(Scratch);
  return Res;
}
internal voidf *OsGetProc(os_lib Lib, c8 *Name) {
  HMODULE Module = (HMODULE)(Lib.v[0]);
  voidf *Res = (voidf *)(GetProcAddress(Module, (LPCSTR)Name));
  return Res;
}
internal void OsFreeLib(os_lib Lib) {
  HMODULE Module = (HMODULE)(Lib.v[0]);
  FreeLibrary(Module);
}



//////////////////////////////////////
// MISC
internal void OsGetRandom(void *Data, size Size) {
  HCRYPTPROV Prov = 0;
  CryptAcquireContextW(&Prov, 0, 0, PROV_DSS, CRYPT_VERIFYCONTEXT);
  CryptGenRandom(Prov, Size, (BYTE*)Data);
  CryptReleaseContext(Prov, 0);
}
