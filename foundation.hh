#pragma once

#include <stdio.h>
#include <stdlib.h>

// *Very* *heavily* inspired by "github.com/Mr-4th-Programming/mr4th".

/*
 * FOUNDATION
 */

//////////////////////////////////////
// COMPILER CONTEXT
// Clang.
#if defined(__clang__)
  #define COMPILER_CLANG 1
  // Os.
  #if defined(_WIN32)
    #define OS_WINDOWS 1
  #elif defined(__gnu_linux__)
    #define OS_LINUX 1
  #elif defined(__APPLE__) && defined(__MACH__)
    #define OS_MAC 1
  #else
    #error No operating system defined in this compiler.
  #endif
  // Architecture.
  #if defined(__amd64__)
    #define ARCH_X64 1
  #elif defined(__i386__)
    #define ARCH_X86 1
  #elif defined(__arm__)
    #define ARCH_ARM 1
  #elif defined(__aarch64__)
    #define ARCH_ARM64 1
  #else
    #error No architecture defined in this compiler.
  #endif

// Gcc.
#elif defined(__GNUC__)
  #define COMPILER_GCC 1
  // Os.
  #if defined(_WIN32)
    #define OS_WINDOWS 1
  #elif defined(__gnu_linux__)
    #define OS_LINUX 1
  #elif defined(__APPLE__) && defined(__MACH__)
    #define OS_MAC 1
  #else
    #error No operating system defined in this compiler.
  #endif
  // Architecture.
  #if defined(__amd64__)
    #define ARCH_X64 1
  #elif defined(__i386__)
    #define ARCH_X86 1
  #elif defined(__arm__)
    #define ARCH_ARM 1
  #elif defined(__aarch64__)
    #define ARCH_ARM64 1
  #else
    #error No architecture defined in this compiler.
  #endif

// Cl.
#elif defined(_MSC_VER)
  #define COMPILER_CL 1
  // Os.
  #if defined(_WIN32)
    #define OS_WINDOWS 1
  #else
    #error No operating system defined in this compiler.
  #endif
  // Architecture.
  #if defined(_M_AMD64)
    #define ARCH_X64 1
  #elif defined(_M_I86)
    #define ARCH_X86 1
  #elif defined(_M_ARM)
    #define ARCH_ARM 1
  #else
    #error No architecture defined in this compiler.
  #endif

#else
  #error No context defined in this compiler.
#endif

//.note: fill missings with 0
#ifndef COMPILER_CL
  #define COMPILER_CL 0
#endif
#ifndef COMPILER_CLANG
  #define COMPILER_CLANG 0
#endif
#ifndef COMPILER_GCC
  #define COMPILER_GCC 0
#endif

#ifndef OS_WINDOWS
  #define OS_WINDOWS 0
#endif
#ifndef OS_LINUX
  #define OS_LINUX 0
#endif
#ifndef OS_MAC
  #define OS_MAC 0
#endif

#ifndef ARCH_X64
  #define ARCH_X64 0
#endif
#ifndef ARCH_X86
  #define ARCH_X86 0
#endif
#ifndef ARCH_ARM
  #define ARCH_ARM 0
#endif
#ifndef ARCH_ARM64
  #define ARCH_ARM64 0
#endif



///////////////////////////////////////
// HELPER MACROS
#define null 0

#define internal static
#define global   static
#define local    static
#define persist  static

#if COMPILER_CL
  #define threadlocal __declspec(thread)
#elif COMPILER_CLANG
  #define threadlocal __thread
#else
  #error 'threadlocal' not defined in this compiler.
#endif

// Math.
#define Min(x, y) ((x) <= (y) ? (x) : (y))
#define Max(x, y) ((x) >= (y) ? (x) : (y))
#define Clamp(x, c, y) ((c) < (x)? (x) : (y) < (c)? (y) : (c))

#define IsPow2(x) (((x) != 0) && ((x) & ((x)-1)) == 0)
#define AlignUpPow2(x, p) (((x) + (p) - 1)&~((p) - 1))
#define AlignDownPow2(x, p) ((x)&~((p) - 1))
#define AlignUpPtr(p, a) ((void *)AlignUpPow2((uintptr_t)(p), (a)))
#define AlignDownPtr(p, a) ((void *)AlignDownPow2((uintptr_t)(p), (a)))

#define Kb(x) ((x) << 10)
#define Mb(x) ((x) << 20)
#define Gb(x) ((x) << 30)
#define Tb(x) ((x) << 40)

#define Thousand(x) (x * 1000)
#define Million(x)  (x * 1000000)
#define Billion(x)  (x * 1000000000llu)
#define Trillion(x) (x * 1000000000000llu)

// Memory operations.
#include <string.h>
#define ZeroMem(Mem, n) memset((Mem), null, (n))
#define ZeroStruct(Struct) ZeroMem((Struct), sizeof(*(Struct)))
#define ZeroArray(Array)   ZeroMem((Array), sizeof(Array))

#define MatchMem(a, b, n) (memcmp((a), (b), (n)) == false)

#define CopyMem(Dest, Src, n) memmove((Dest), (Src), (n))
#define CopyStruct(Dest, Src) CopyMem((Dest), (Src), Min(sizeof(*(Dest)), sizeof(*(Src))))
#define CopyArray(Dest, Src)  CopyMem((Dest), (Src), Min(sizeof(Dest), sizeof(Src)))

#define countof(x) (sizeof(x)/sizeof(*(x)))

// Assert.
#define _Halt() exit(1)
#ifdef ENABLE_ASSERT
  #include <stdio.h>
  #define Assert(e)         if (!(e)) { printf("Assertion failed in %s[%d].\n", __FILE__, __LINE__); _Halt(); }
  #define AssertMsg(e, Msg) if (!(e)) { printf("Assertion failed in %s[%d]. \"%s\"\n", __FILE__, __LINE__, Msg); _Halt(); }
#else
	#define Assert()
#endif

#define StaticAssert(c, l) typedef u8 l##__LINE__ [(c)?1:-1]

// Debug.
#ifdef DEBUG
  #define Debug(Stmt) do { Stmt; } while (0);
#else
	#define Debug()
#endif

// Loops.
#define itrn(i, n) for (size (i) = 0; (i) < (n); (i)++)

// Rainbows.
#define TermBrightWhite(s)   "\033[0;97m" s "\033[0;37m"
#define TermWhite(s)         "\033[0;37m" s "\033[0;37m"
#define TermGray(s)          "\033[0;90m" s "\033[0;37m"
#define TermBlack(s)         "\033[0;30m" s "\033[0;37m"
#define TermRed(s)           "\033[0;31m" s "\033[0;37m"
#define TermYellow(s)        "\033[0;33m" s "\033[0;37m"
#define TermBrightYellow(s)  "\033[0;93m" s "\033[0;37m"
#define TermBrightGreen(s)   "\033[0;92m" s "\033[0;37m"
#define TermGreen(s)         "\033[0;32m" s "\033[0;37m"
#define TermBrightCyan(s)    "\033[0;96m" s "\033[0;37m"
#define TermCyan(s)          "\033[0;36m" s "\033[0;37m"
#define TermBrightBlue(s)    "\033[0;94m" s "\033[0;37m"
#define TermBlue(s)          "\033[0;34m" s "\033[0;37m"
#define TermMagenta(s)       "\033[0;35m" s "\033[0;37m"
#define TermBrightMagenta(s) "\033[0;95m" s "\033[0;37m"
#define TermBrightRed(s)     "\033[0;91m" s "\033[0;37m"



///////////////////////////////////////
// TYPES
#include <stdint.h>
typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef u8  c8;
typedef u16 c16;
typedef u32 c32;
typedef u64 c64;
typedef i8  b8;
typedef i16 b16;
typedef i32 b32;
typedef i64 b64;
typedef float  f32;
typedef double f64;
typedef size_t size;
typedef u8 byte;
typedef void  voidf(void);

// Constants.
global i8  i8_Min  = (i8 )0x80;
global i16 i16_Min = (i16)0x8000;
global i32 i32_Min = (i32)0x80000000;
global i64 i64_Min = (i64)0x8000000000000000llu;

global i8  i8_Max  = (i8 )0x7F;
global i16 i16_Max = (i16)0x7FFF;
global i32 i32_Max = (i32)0x7FFFFFFF;
global i64 i64_Max = (i64)0x7FFFFFFFFFFFFFFFllu;

global u8  u8_Max  = 0xFF;
global u16 u16_Max = 0xFFFF;
global u32 u32_Max = 0xFFFFFFFF;
global u64 u64_Max = 0xFFFFFFFFFFFFFFFFllu;

global f32 r32_Pi  = 3.14159265359f;
global f32 r32_Tau = 6.28318530718f;
global f32 r32_e   = 2.71828182846f;

global f64 r64_Pi  = 3.14159265359;
global f64 r64_Tau = 6.28318530718;
global f64 r64_e   = 2.71828182846;

typedef u64 dense_time;
struct date_time {
  u16 Millisecond; // [0, 999]
  u8 Second;       // [0, 59]
  u8 Minute;       // [0, 59]
  u8 Hour;         // [0, 23]
  u8 Day;          // [0, 30]
  u8 Month;        // [0, 11]
  i16 Year;        // 1 = 1 CE, -1 = -1 BCE
};
internal dense_time DenseTimeFromDate(date_time *Date) {
  u32 EncodedYear = (u32)((i32)Date->Year + 0x8000);
  dense_time Res = 0;
  Res += EncodedYear;
  Res *= 12;
  Res += (Date->Month - 1);
  Res *= 31;
  Res += Date->Day;
  Res *= 24;
  Res += Date->Hour;
  Res *= 60;
  Res += Date->Minute;
  Res *= 61;
  Res += Date->Second;
  Res *= 1000;
  Res += Date->Millisecond;
  return Res;
}
internal date_time DateTimeFromDense(dense_time Dense) {
  date_time Res = {0};
  Res.Millisecond = Dense%1000;
  Dense /= 1000;
  Res.Second = Dense%61;
  Dense /= 61;
  Res.Minute = Dense%60;
  Dense /= 60;
  Res.Hour = Dense%24;
  Dense /= 24;
  Res.Day = Dense%31;
  Dense /= 31;
  Res.Month = (Dense%12) + 1;
  Dense /= 12;
  Res.Year = ((i32)(Dense) - 0x8000);
  return Res;
}

// Data access flags.
typedef u32 data_accessfl;
enum _data_accessfl {
  data_accessfl_None    = 0,
  data_accessfl_Read    = (1 << 0),
  data_accessfl_Write   = (1 << 1),
  data_accessfl_Execute = (1 << 2),
};

// File properties.
typedef u32 file_propfl;
enum _file_propfl {
  file_prop_None     = 0,
  file_prop_IsFolder = (1 << 0)
};

struct file_props {
  u64 Size;
  file_propfl Flags;
  dense_time CreateTime;
  dense_time ModifyTime;
  data_accessfl Access;
};


///////////////////////////////////////
// MEMORY
internal void *OsMemRes(size Size); // If only C supported out of order decalrations...
internal b32   OsMemCom(void *Ptr, size Size);
internal void  OsMemDec(void *Ptr, size Size);
internal void  OsMemRel(void *Ptr, size Size);

#ifndef MemRes
  #define MemRes OsMemRes
#endif
#ifndef MemCom
  #define MemCom OsMemCom
#endif
#ifndef MemDec
  #define MemDec OsMemDec
#endif
#ifndef MemRel
  #define MemRel OsMemRel
#endif

#ifndef MemRes
  #error Did not define 'MemRes' of type '(u64)->^void'.
#endif
#ifndef MemCom
  #error Did not define 'MemCom' of type '(^void, u64)->b32'.
#endif
#ifndef MemDec
  #error Did not define 'MemDec' of type '(^void, u64)->void'.
#endif
#ifndef MemRel
  #error Did not define 'MemRel' of type '(^void, u64)->void'.
#endif

// Arena.
#define ARENA_DEFAULT_RES_SIZE Gb(1)
#define ARENA_DEFAULT_COM_SIZE Mb(64)
struct arena {
	size Cap;
	size Pos;
	size Com;
};

#define MEM_FIRST_COM Kb(4)
StaticAssert(sizeof(arena) <= MEM_FIRST_COM, CheckArenaSize);

internal arena *ArenaReserve(size ReserveSize) {
  arena *Res = null;
  if (ReserveSize >= MEM_FIRST_COM) {
    void *Mem = MemRes(ReserveSize);
    if (MemCom(Mem, MEM_FIRST_COM)) {
      Res = (arena*)Mem;
      Res->Cap = ReserveSize;
      Res->Pos = AlignUpPow2(sizeof(arena), 64);
      Res->Com = MEM_FIRST_COM;
    }
  }
  AssertMsg(Res != null, "Failed to create arena.");
  return Res;
}
internal arena *ArenaMake() {
  return ArenaReserve(ARENA_DEFAULT_RES_SIZE);
}
internal void ArenaRelease(arena *Arena) {
  MemRel(Arena, Arena->Cap);
}
internal void *ArenaPush(arena *Arena, size Size) {
  void *Res = null;
	if (Arena->Pos + Size <= Arena->Cap) {
    void *ResSuccess = ((byte*)Arena) + Arena->Pos;
		size Pos = Arena->Pos + Size;
    size Com = Arena->Com;
    if (Pos > Com) {
      size Aligned = AlignUpPow2(Pos, ARENA_DEFAULT_COM_SIZE);
      size NextCom = Min(Aligned, Arena->Cap);
      size ComSize = NextCom - Com;
      if (MemCom((byte*)Arena + Com, ComSize)) {
        Com = NextCom;
        Arena->Com = Com;
      }
    }
    if (Pos <= Com) {
      Res = ResSuccess;
      Arena->Pos = Pos;
    }
	}	
  AssertMsg(Res != null, "Failed to commit to arena.");
  return Res;
}
internal void ArenaPopTo(arena *Arena, size Pos) {
  if (Pos < Arena->Pos) {
    Arena->Pos   = Pos;
    size Pos     = Arena->Pos;
    size Aligned = AlignUpPow2(Pos, ARENA_DEFAULT_COM_SIZE);
    size NextCom = Min(Aligned, Arena->Cap);
    size Com     = Arena->Com;
    if (NextCom < Com) {
      size Dec = Com - NextCom;
      MemDec(((byte*)Arena) + NextCom, Dec);
      Arena->Com = NextCom;
    }
  }
}
internal void *ArenaPushZeros(arena *Arena, size Size) {
  void *Res = ArenaPush(Arena, Size);
  ZeroMem(Res, Size);
  return Res;
}
internal void ArenaAlign(arena *Arena, size Align) {
  size Pos = Arena->Pos;
  size Aligned = AlignUpPow2(Pos, Align);
  size Delta = Aligned - Pos;
  if (Delta > 0)
    ArenaPush(Arena, Delta);
}
internal void ArenaAlignZeros(arena *Arena, size Align) {
  size Pos = Arena->Pos;
  size Aligned = AlignUpPow2(Pos, Align);
  size Delta = Aligned - Pos;
  if (Delta > 0)
    ArenaPushZeros(Arena, Delta);
}
internal void ArenaPopAmount(arena *Arena, size Amount) {
  if (Amount <= Arena->Pos)
    ArenaPopTo(Arena, Arena->Pos - Amount);
}

#define Push(Arena, Size) ArenaPush((Arena), (Size))
#define PushZero(Arena, Size) ArenaPushZeros((Arena), (Size))
#define PushStruct(Arena, t) (t*)ArenaPush((Arena), sizeof(t))
#define PushZeroStruct(Arena, t) (t*)ArenaPushZeros((Arena), sizeof(t))
#define PushArray(Arena, t, Count) (t*)ArenaPush((Arena), sizeof(t)*(Count))
#define PushZerosArray(Arena, t, Count) (t*)ArenaPushZeros((Arena), sizeof(t)*(Count))
#define Pop(Arena, n) ArenaPopAmount((Arena), n)

// Temprary memory.
struct temp_mem {
  arena *Arena;
  size   Pos;
};
internal temp_mem TempMemBegin(arena *Arena) {
  temp_mem Res = {Arena, Arena->Pos};
  return Res;
}
internal void TempMemEnd(temp_mem TempMem) {
  ArenaPopTo(TempMem.Arena, TempMem.Pos);
}

// Scratch.
#if !defined(SCRATCH_POOL_NO)
  #define SCRATCH_POOL_NO 4
#endif
threadlocal arena *_ScratchPool[SCRATCH_POOL_NO];

internal temp_mem GetScratch(arena **ConflictArray, size Count) {
  if (_ScratchPool[0] == null) {
    arena **ScratchSlot = _ScratchPool;
    for (size i = 0; i < SCRATCH_POOL_NO; i++, ScratchSlot++)
      *ScratchSlot = ArenaMake();
  }
  temp_mem Res = {};
  arena **ScratchSlot = _ScratchPool;
  itrn (i, SCRATCH_POOL_NO) {
    b32 IsNotConflict = true;
    arena **ConflictPtr = ConflictArray;
    for (size j = 0; j < Count; j++, ConflictPtr++) {
      if (*ScratchSlot == *ConflictPtr) {
        IsNotConflict = false;
        break;
      }
    }
    if (IsNotConflict) {
      Res = TempMemBegin(*ScratchSlot);
      break;
    }
  }
  return Res;
}
#define ReleaseScratch(Temp) TempMemEnd(Temp)

// Arrays.
template <typename type> struct array {
	type *Data;
  size Len;
  size Cap;

	type &operator[](size Idx) { return Data[Idx]; }
};

template <typename type> void PreallocArray(array<type> *Array, arena *Arena, size Cap) {
  type *Data = PushArray(Arena, type, Cap);
  Array->Data = Data;
  Array->Cap  = Cap;
  Array->Len  = 0;
}
template <typename type> int ArrayGrow(array<type> *Array) {
  if (Array->Len + 1 > Array->Cap) {
    size NewCap  = (Array->Cap == 0)? 8 : Array->Cap * 2;
    type *Data = (type*)MemRes(NewCap * sizeof(type));
    if (MemCom(Data, NewCap * sizeof(type))) {
      CopyMem(Data, Array->Data, Array->Len * sizeof(type));
      if (Data == 0)
        return -1;
      Array->Data = Data;
      Array->Cap  = NewCap;
    }
  }
  return 0;
}
template <typename type> void ArrayPut(array<type> *Array, type x) {
	if (Array->Cap < Array->Len+1) {
		ArrayGrow(Array);
	}
	Array->Data[Array->Len] = x;
	Array->Len++;
}

// List.
template <typename type> struct list {
	type *Data;
  size  Len;

	type &operator[](size Idx) { return Data[Idx]; }
};
template <typename type> list<type> ListFromArray(array<type> *Array) {
  return {Array->Data, Array->Len};
}
template <typename type> list<type> ListCopyFromArray(array<type> *Array) {
  type *Data = (type*)MemRes(Array->Len * sizeof(type));
  CopyMem(Data, Array->Data, Array->Len * sizeof(type));
  return {Data, Array->Len};
}

// Map.
internal u64 u64Hash(u64 x) {
  x *= 0xff51afd7ed558ccdull;
  x ^= x >> 32;
  return x;
}
internal u64 PtrHash(void *Ptr) {
  return u64Hash((u64)Ptr);
}
internal u64 MixHash(u64 x, u64 y) {
  x ^= y;
  x *= 0xff51afd7ed558ccdull;
  x ^= x >> 32;
  return x;
}
internal u64 BytesHash(void *Ptr, size Len) {
  u64 x = 0xcbf29ce484222325ull;
  c8 *Buff = (c8 *)Ptr;
  itrn (i, Len) {
    x ^= Buff[i];
    x *= 0x100000001b3;
    x ^= x >> 32;
  }
  return x;
}
internal u64 StrHashRange(c8 *Start, c8 *End) {
  u64 x = 0xcbf29ce484222325ull;
  while (Start != End) {
    x ^= *Start++;
    x *= 1099511628211ull;
    x ^= x >> 32;
  }
  return x;
}

// Hash map.
template <typename type> struct map_entry {
  void *Key;
  type  Val;
};
template <typename type> struct map {
  map_entry<type> *Entries;
  size Len;
  size Cap;
};

template <typename type> void MapPut(map<type> *Map, u64 Key, type *Val);
template <typename type> void MapGrow(map<type> *Map, u64 NewCap) {
  NewCap = Max(16, NewCap);
  map<type> NewMap = {};
  NewMap.Cap       = NewCap;
  NewMap.Entries   = (map_entry<type>*)MemRes(NewCap*sizeof(map_entry<type>));
  if (MemCom(NewMap.Entries, NewCap*sizeof(map_entry<type>))) {
    itrn (i, Map->Cap) {
      map_entry<type> *Entry = Map->Entries + i;
      if (Entry->Key)
        MapPut(&NewMap, Entry->Key, Entry->Val);
    }
    MemRel(Map->Entries, Map->Cap);
  }
  *Map = NewMap;
}
template <typename type> type MapGet(map<type> *Map, void *Key) {
  if (Map->Len == 0)
    return null;
  Assert(IsPow2(Map->Cap));
  Assert(Map->Len < Map->Cap);
  size i = (size)PtrHash(Key);
  while (true) {
    i &= Map->Cap - 1;
    map_entry<type> *Entry = Map->Entries + i;
    if (Entry->Key == Key)
      return Entry->Val;
    else
    if (!Entry->Key)
      return null;
    i++;
  }
  return null;
}
template <typename type> void MapPut(map<type> *Map, void *Key, type Val) {
  Assert(Key);
  if (2*Map->Len >= Map->Cap)
    MapGrow(Map, 2*Map->Cap);
  Assert(2*Map->Len < Map->Cap);
  size i = (size)PtrHash(Key);
  while (true) {
    i &= Map->Cap - 1;
    map_entry<type> *Entry = Map->Entries + i;
    if (!Entry->Key) {
      Map->Len++;
      Entry->Key = Key;
      Entry->Val = Val;
      return;
    }
    else
    if (Entry->Key == Key) {
      Entry->Val = Val;
      return;
    }
    i++;
  }
}


///////////////////////////////////////
// STRINGS

// Chars.
inline c32 ToUpper(c32 Char) {
  if ('a' <= Char && Char <= 'z')
    Char += 'A' - 'a';
  return Char;
}
inline c32 ToLower(c32 Char) {
  if ('A' <= Char && Char <= 'Z')
    Char += 'a' - 'A';
  return Char;
}

inline b32 IsNum(c32 Char) {
  return Char >= '0' && Char <= '9';
}
inline b32 IsAl(c32 Char) {
  return Char >= 'a' && Char <= 'z' ||
         Char >= 'A' && Char <= 'Z';
}
inline b32 IsNumOrUnderscore(c32 Char) { return IsNum(Char) || Char == '_'; }

inline b32 IsAlOrUnderscore(c32 Char)    { return IsAl(Char) || Char == '_'; }
inline b32 IsAlnumOrUnderscore(c32 Char) { return IsAl(Char) || IsNum(Char) || Char == '_'; }
inline b32 IsSlash(c32 Char)             { return Char == '\\' || Char == '/'; }

// Strings.
typedef struct _str8 {
	c8  *Str;
	size Len;
} str8;
typedef struct _str16 {
	u16 *Str;
	size Len;
} str16;

// Common string (utf8).
typedef str8 str;
#define stre(_Str) (i32)(_Str).Len, (_Str).Str //.note: For use with printf.
#define strl(Str) str_((c8 *)(Str), sizeof(Str) - 1)
inline str str_(c8 *Str, size Len) {
	str Res = {Str, Len};
	return Res;
}
inline str strRange(c8 *Begin, c8 *End) {
	str Res = {Begin, (size)(End - Begin)};
	return Res;
}
internal str strCStr(c8 *CStr) {
  c8 *Ptr = CStr;
  while (*Ptr != 0)
     Ptr++;
	str Res = {CStr, (size)(Ptr - CStr)};
	return Res;
}

// Wide string (utf16).
internal str16 str16_(c16 *Str, size Len) {
	str16 Res = {Str, Len};
	return Res;
}
internal str16 str16Range(c16 *Begin, c16 *End) {
	str16 Res = {Begin, (size)(End - Begin)};
	return Res;
}
internal str16 str16CStr(c16 *CStr) {
  c16 *Ptr = CStr;
  while (*Ptr != 0)
    Ptr += 1;
	str16 Res = {CStr, (size)(Ptr - CStr)};
	return Res;
}

// String lists.
typedef struct _str_node {
	struct _str_node *Next;
	str Str;
} str_node;
typedef struct _str_list {
	str_node *First;
	str_node *Last;
	size NoNodes;
	size TotalSize;
} str_list;
internal void PushNodeToStrList(str_list *List, str_node *Node, str Str) {
	Node->Str = Str;
  if (List->First == 0) {
    List->First = List->Last = Node;
    Node->Next  = 0;
  }
  else {
    List->Last->Next = Node;
    List->Last = Node;
    Node->Next = 0;
  }
	List->NoNodes++;
	List->TotalSize += Str.Len;
}
internal void PushToStrList(arena *Arena, str_list *List, str Str) {
	str_node *Node = PushStruct(Arena, str_node);
	PushNodeToStrList(List, Node, Str);
}
internal str JoinStrList(arena *Arena, str_list *List) {
  size Len = List->TotalSize;
	c8 *Str = PushArray(Arena, c8, Len);
	c8 *Ptr = Str;
	b32 IsMid = false;
	for (str_node *Node = List->First; Node != null; Node = Node->Next) {
		CopyMem(Ptr, Node->Str.Str, Node->Str.Len);
		Ptr += Node->Str.Len;
		if (Node->Next != null)
			IsMid = true;
	}
	return str_(Str, Len);
}
internal str_list SplitStr(arena *Arena, str Str, str Splitters) {
	str_list Res = {0};
	c8 *Ptr  = Str.Str;
	c8 *Word = Ptr;
	c8 *Opl  = Str.Str + Str.Len;
	while (Ptr < Opl) {
		byte Byte = *Ptr;
		b32 IsSplitterByte = false;
		itrn (i, Splitters.Len) {
			// Is this byte a splitter?.
			if (Byte == Splitters.Str[i]) {
				IsSplitterByte = true;
				break;
			}
		}
		// If so, try emitting word.
		if (IsSplitterByte) {
			if (Word < Ptr)
				PushToStrList(Arena, &Res, strRange(Word, Ptr));
			Word = Ptr + 1;
		}
		Ptr++;
	}
	// Try emitting last word.
	if (Word < Ptr)
		PushToStrList(Arena, &Res, strRange(Word, Ptr));
	return Res;
}

// Extras.
internal str PushStrCopy(arena *Arena, str Str) {
  str Res = {0};
  Res.Str = PushArray(Arena, u8, Str.Len + 1);
  Res.Len = Str.Len;
  CopyMem(Res.Str, Str.Str, Str.Len);
  Res.Str[Res.Len] = 0;
  return Res;
}
internal b32 MatchStr(str a, str b, b32 MatchCase) {
  b32 Res = false;
  if (a.Len = b.Len) {
    Res = true;
    for (size i = 0; i < a.Len; i++) {
      c8 CharA = a.Str[i];
      c8 CharB = b.Str[i];
      if (!MatchCase) {
        CharA = ToUpper(CharA);
        CharB = ToUpper(CharB);
      }
      if (CharA != CharB) {
        Res = false;
        break;
      }
    }
  }
  return Res;
}
internal str ChopStrBeforeLastSlash(str Str) {
  str Res = Str;
  if (Str.Len > 0) {
    u64 Pos = Str.Len;
    for (u64 i = Str.Len - 1; i >= 0; i--) {
      if (IsSlash(Str.Str[i])) {
        Pos = i;
        break;
      }
    }
    Res.Str = Str.Str + Pos + 1;
    Res.Len = Str.Len - Pos;
  }
  return Res;
}
internal str ChopStrAfterLastSlash(str Str) {
  str Res = Str;
  if (Str.Len > 0) {
    u64 Pos = Str.Len;
    for (u64 i = Str.Len - 1; i >= 0; i--) {
      if (IsSlash(Str.Str[i])) {
        Pos = i;
        break;
      }
    }
    Res.Len = Pos;
  }
  return Res;
}



///////////////////////////////////////
// UNICODE

typedef struct _utf_char {
	u32 Codepoint;
	size Size;
} utf_char;

// Encoding & decoding.
internal utf_char DecodeUtf8(c8 *Str, size Len) {
	local u8 CodepointLen[] = {
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 1,
		0, 0, 0, 0,
		0, 0, 0, 0,
		2, 2, 2, 2,
		3, 3,
		4,
		0
	};
	local u8 FirstByteMask[] = {0, 0x7F, 0x1F, 0x0F, 0x07};
	local u8 FinalShift[] = {0, 18, 12, 6, 0};
	utf_char Res = {0};
	if (Len > 0) {
		Res.Codepoint = '#';
		Res.Size = 1;
		byte Byte = Str[0];
		u8 Size = CodepointLen[Byte >> 3];
		if (0 < Size && Size <= Len) {
			size Codepoint = (Byte & FirstByteMask[Size]) << 18;
			switch (Size) {
				case 4: Codepoint |= ((Str[3] & 0x3F) << 0);
				case 3: Codepoint |= ((Str[2] & 0x3F) << 6);
				case 2: Codepoint |= ((Str[1] & 0x3F) << 12);
				default: break;
			}
      Codepoint >>= FinalShift[Size];

			Res.Codepoint = Codepoint;
			Res.Size = Size;
		}
	}
	return Res;
}
internal size EncodeUtf8(c8 *Dest, size Codepoint) {
	size Size = 0;
	if (Codepoint < (1 << 8)) {
		Dest[0] = Codepoint;
		Size = 1;
	}
	else
	if (Codepoint < (1 << 11)) {
		Dest[0] = 0xC0 | (Codepoint >> 6);
		Dest[1] = 0x80 | (Codepoint & 0x3F);
		Size = 2;
	}
	else
	if (Codepoint < (1 << 16)) {
		Dest[0] = 0xE0 |  (Codepoint >> 12);
		Dest[1] = 0x80 | ((Codepoint >> 6) & 0x3F);
		Dest[2] = 0x80 |  (Codepoint       & 0x3F);
		Size = 3;
	}
	else
	if (Codepoint < (1 << 21)) {
		Dest[0] = 0xF0 |  (Codepoint >> 18);
		Dest[1] = 0x80 | ((Codepoint >> 12) & 0x3F);
		Dest[2] = 0x80 | ((Codepoint >> 6)  & 0x3F);
		Dest[3] = 0x80 |  (Codepoint        & 0x3F);
		Size = 4;
	}
	else {
		Dest[0] = '#';
		Size = 1;
	}

	return Size;
}
internal utf_char DecodeUtf16(u16 *Str, size Len) {
	utf_char Res = {'#', 1};
	u16 c1 = Str[0];
	if (c1 < 0xD800 || 0xDFFF < c1)
		Res.Codepoint = c1;
	else {
		u16 c2 = Str[1];
		if (0xD800 <= c1 && c1 < 0xDC00 &&
		    0xDC00 <= c2 && c2 < 0xE000) {
			Res.Codepoint = (((c1 - 0xDC00) << 10) | (c2 - 0xDC00)) + 0x10000,
			Res.Size = 2;
		}
	}
	return Res;
}
internal size EncodeUtf16(u16 *Dest, size Codepoint) {
	size Len = 0;
	if (Codepoint < 0x10000) {
		Dest[0] = Codepoint;
		Len = 1;
	}
	else {
		size c = Codepoint - 0x10000;
		Dest[0] = (c >> 10)   + 0xD800;
		Dest[1] = (c & 0x3FF) + 0xDC00;
		Len = 2;
	}
	return Len;
}

// Conversions.
internal str16 Str16FromStr8(arena *Arena, str8 Str) {
	u16 *Mem = PushArray(Arena, u16, Str.Len*2 + 1);
	u16 *DestPtr = Mem;
	c8  *Ptr = Str.Str;
	c8  *OnePastLast = Str.Str + Str.Len;
	while (Ptr < OnePastLast) {
		utf_char Decoded = DecodeUtf8(Ptr, (u32)(OnePastLast - Ptr));
		size EncodedLen = EncodeUtf16(DestPtr, Decoded.Codepoint);
		Ptr += Decoded.Size;
		DestPtr += EncodedLen;
	}
 *DestPtr = 0;
	size AllocationSize = Str.Len*2 + 1;
	size StrLen = (u32)(DestPtr - Mem);
	size Unused = AllocationSize - StrLen - 1;
	Pop(Arena, Unused * sizeof(*Mem));
	str16 Res = {Mem, StrLen};
	return Res;
}
internal str8 Str8FromStr16(arena *Arena, str16 Str) {
	c8 *Mem = PushArray(Arena, c8, Str.Len*3 + 1);
	c8  *DestPtr = Mem;
	u16 *Ptr = Str.Str;
	u16 *OnePastLast = Str.Str + Str.Len;
	while (Ptr < OnePastLast) {
		utf_char Decoded = DecodeUtf16(Ptr, (size)(OnePastLast - Ptr));
		u16 EncodedSize = EncodeUtf8(DestPtr, Decoded.Codepoint);
		Ptr += Decoded.Size;
		DestPtr += EncodedSize;
	}
 *DestPtr = 0;
	size AllocationSize = Str.Len*3 + 1;
	size StrLen = (size)(DestPtr - Mem);
	size Unused = AllocationSize - StrLen - 1;
	Pop(Arena, Unused*sizeof(*Mem));
	str8 Res = {Mem, StrLen};
	return Res;
}