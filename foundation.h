#include <windows.h>
#include <stdio.h>

////////////////////////
////////////////////////
////////////////////////
// FOUNDATION
#ifndef FOUNDATION_H
#define FOUNDATION_H

#undef min
#undef max

////////////////////////
// SYNTAX SUGAR
#define null  0
#define true  1
#define false 0

#define strify(s) #s
#define lineid(Name) Name##__LINE__

#define countof(a) ((int)(sizeof(a) / sizeof(*(a))))
#define min(x, y)  ((x) <= (y)? (x) : (y))
#define max(x, y)  ((x) >= (y)? (x) : (y))
#define fornum(i, n)    for (u64 (i) = 0;   (i) < (n); (i) += 1)
#define forran(i, s, e) for (u64 (i) = (s); (i) < (e); (i) += 1)

#ifdef __cplusplus
  #define function static
#endif
#define function

#if defined(DEBUG)
  #define Assert(Expr, ...)                                   \
    if (!(Expr))                                              \
      printf("Assert failed in %s:%d:1", __FILE__, __LINE__), \
      printf(" " __VA_ARGS__),                                \
      __debugbreak();
#else
	#define Assert()
#endif
#define StaticAssert(Expr) typedef char lineid(static_assert_on_line_)[(Expr) ? 1 : -1]

#define AlignUpPow2(x, p) (((x) +   (p) - 1)  & ~((p) - 1))
#define AlignDoPow2(x, p)  ((x) & ~((p) - 1))

////////////////////////
// TYPES
typedef unsigned char byte;

typedef signed char   i8;
typedef short         i16;
typedef int           i32;
typedef long long int i64;
StaticAssert(sizeof(i8)  == 1);
StaticAssert(sizeof(i16) == 2);
StaticAssert(sizeof(i32) == 4);
StaticAssert(sizeof(i64) == 8);
typedef i8  b8;
typedef i16 b16;
typedef i32 b32;
typedef i64 b64;

typedef unsigned char          u8;
typedef unsigned short         u16;
typedef unsigned int           u32;
typedef unsigned long long int u64;
StaticAssert(sizeof(u8)  == 1);
StaticAssert(sizeof(u16) == 2);
StaticAssert(sizeof(u32) == 4);
StaticAssert(sizeof(u64) == 8);

typedef char           c8;
typedef unsigned short c16;
typedef unsigned int   c32;
StaticAssert(sizeof(c8)  == 1);
StaticAssert(sizeof(c16) == 2);
StaticAssert(sizeof(c32) == 4);
#define IsSpace(c) ((c) == ' ' || (c) == '\r' || (c) == '\t' || (c) == '\v') // Could these be accelerated with tables?
#define IsSlash(c) ((c) == '\\' || (c) == '/')
#define IsDigit(c) ((c) >= '0' && (c) <= '9')
#define IsUpper(c) ((c) >= 'A' && (c) <= 'Z')
#define IsLower(c) ((c) >= 'a' && (c) <= 'z')
#define IsAlpha(c) (IsLower(c) || IsUpper(c))
#define ToUpper(c) (IsLower(c)? (c) + 'A'-'a' : c)
#define ToLower(c) (IsUpper(c)? (c) + 'a'-'A' : c)

typedef float  f32;
typedef double f64;

typedef void proc(void);

#include <emmintrin.h>
typedef __m128  m128;
typedef __m128i u128;
typedef __m128i i128;

function u64 HashU64(u64 x);
function u64 HashStr(c8 *Str, u32 Len);
function u64 HashCStr(c8 *Str);

////////////////////////
// MEMORY
function void *MemRes(u64 Size);
function void  MemRel(void *Ptr, u64 Size);

function b32  MemCmp(const byte *Mm1, const byte *Mm2, u64 Len);
function void MemSet(byte *Mem, u8 Val, u64 Len);
function void MemCpy(byte *Dst, const byte *Src, u64 Len);

#define POOL_INIT_CAP 1024*1024*1024
typedef struct _pool {
	u64 Cap;
	u64 Pos;

#ifdef __cplusplus
  inline void  Release(void);
  inline void *Put(u64 Size);
  inline void  Pop(u64 Size);
#endif
} pool;
function pool *ReservePool(void);
inline void    ReleasePool(pool *p);
inline void   *PoolPut(pool *p, u64 Size);
inline void    PoolPop(pool *p, u64 Size);

typedef struct _str {
  c8 *Str;
  u64 Len;
} str;
#define StrLit(Lit) (str){Lit, sizeof(Lit)-1}

#define ARRAY_INIT_CAP 16
#define array(type) struct { u64 Cap, Len; type *Mem; }

function b32 _ArrayMake(u64 *Cap, u64 *Len, u64 Itm, void **Mem, u64 InitCap);
function b32 _ArrayGrow(u64 *Cap, u64 *Len, u64 Itm, void **Mem);

#define _ArrayExp(Array) &(Array)->Cap, &(Array)->Len, sizeof(*(Array)->Mem), (void**)(&(Array)->Mem) 
#define  ArrayMake(Array, Len) _ArrayMake(_ArrayExp(Array), (Len))
#define  ArrayFree(Array) MemRel((Array)->Mem, (Array)->Cap*sizeof(*(Array)->Mem)), (Array)->Cap = 0, (Array)->Len = 0, (Array)->Mem = null
#define  ArrayAdd(Array, x) (        \
  ((Array)->Cap < (Array)->Len + 1)? \
    _ArrayGrow(_ArrayExp(Array))     \
  :                                  \
    0,                               \
  (Array)->Mem[(Array)->Len] = (x),  \
  (Array)->Len++                     \
)

//.link: https://youtu.be/k9MBMvR2IvI;
//.link: https://github.com/pervognsen/bitwise.
//.link: https://github.com/rxi/map.
#define TABLE_MIN_CAP  16
#define TABLE_MAX_LOAD 70
#define table(type) struct { u64 Cap, Len; u64 *Keys; byte *Vals; type Tmp; type *Ptr; }

function b32   _TableMake(u64 *Cap, u64 *Len, u64 **Keys, byte **Vals, u64 Itm, u64 InitCap);
function void  _TableFree(u64 *Cap, u64 *Len, u64 **Keys, byte **Vals, u64 Itm);
function b32   _TableGrow(u64 *Cap, u64 *Len, u64 **Keys, byte **Vals, u64 Itm, u64 NewCap);
function void *_TableAdd (u64 *Cap, u64 *Len, u64 **Keys, byte **Vals, u64 Itm, u64 Key, void *Val);
function void *_TableGet (u64 *Cap, u64 *Len, u64 **Keys, byte **Vals, u64 Itm, u64 Key);

#define _TableExp(Table) &(Table)->Cap, &(Table)->Len, &(Table)->Keys, &(Table)->Vals, sizeof((Table)->Tmp)
#define  TableMake(Table, Len)                           _TableMake(_TableExp(Table), (Len))
#define  TableFree(Table)                                _TableFree(_TableExp(Table))
#define  TableAdd(Table, Key, Val) (Table)->Tmp = (Val), _TableAdd (_TableExp(Table), (u64)(Key), &(Table)->Tmp)
#define  TableGet(Table, Key)     ((Table)->Ptr =        _TableGet (_TableExp(Table), (u64)(Key)))? *(Table)->Ptr : (Table)->Tmp


////////////////////////
// TIME
inline u64 GetTicks(void);
inline u64 GetUsecs(void);

////////////////////////
// FILES
function str FileOpen(c8 *Path);
function b32 FileSave(c8 *Path, byte *Data, u32 Size);
function b32 CreateDir(c8 *Path);
function b32 DeleteDir(c8 *Path);

typedef u64 dll;
function dll   LoadDll(c8 *Path);
function proc *GetProc(dll Dll, c8 *Name);
function void  FreeDll(dll Dll);

#endif

////////////////////////
////////////////////////
////////////////////////
// FOUNDATION IMPLEMENTATION
#ifdef FOUNDATION_IMPL

function u64 HashU64(u64 x) {
  x *= 0xff51afd7ed558ccdull;
  x ^= x >> 32;
  return x;
}
function u64 HashStr(c8 *Str, u32 Len) {
  u64 Hash = 0xCBF29CE484222325ull;
  fornum (i, Len) {
    Hash ^= Str[i];
    Hash *= 0x100000001B3ull;
  }
  return Hash;
}
function u64 HashCStr(c8 *Str) {
  u64 Hash = 0xCBF29CE484222325ull;
  while (*Str) {
    Hash ^= *Str;
    Hash *= 0x100000001B3ull;
    Str  += 1;
  }
  return Hash;
}

function void *MemRes(u64 Size) {
  return (byte*)VirtualAlloc(0, Size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
}
function void MemRel(void *Ptr, u64 Size) {
  VirtualFree(Ptr, Size, MEM_RELEASE);
}

// The SIMD spirit is here, at least.
//.todo: Use the actual SIMD.
function b32 MemCmp(const byte *mm1, const byte *mm2, u64 Len) {
  fornum (i, Len>>3) {
    if (*(u64*)mm1 != *(u64*)mm2)
      return false;
    mm1 += 8;
    mm2 += 8;
  }
  fornum (i, Len&7) {
    if (*mm1 != *mm2)
      return false;
    mm1 += 1;
    mm2 += 1;
  }
  return true;
}
function void MemSet(byte *Mem, u8 Val, u64 Len) {
  fornum (i, Len>>3) {
    *(u64*)Mem = Val; // Doesn't broadcast the value to the whole u64 slot.
    Mem += 8;
  }
  fornum (i, Len&7) {
    *Mem = Val;
    Mem += 1;
  }
}
function void MemCpy(byte *Dst, const byte *Src, u64 Len) {
  fornum (i, Len>>3) {
    *(u64*)Dst = *(u64*)Src;
    Dst += 8;
    Src += 8;
  }
  fornum (i, Len&7) {
    *Dst = *Src;
    Dst += 1;
    Src += 1;
  }
}

function pool *ReservePool(void) {
  byte *Mem = MemRes(POOL_INIT_CAP);
  pool *Res = (pool*)Mem;
  Res->Cap = POOL_INIT_CAP;
  Res->Pos = AlignUpPow2(sizeof(pool), 64);
  Assert(Res != null, "Failed to create pool.");
  return Res;
}
inline void ReleasePool(pool *p) {
  MemRel(p, p->Cap);
}
inline void *PoolPut(pool *p, u64 Size) {
  byte *Res = null;
	if (p->Pos + Size <= p->Cap) {
		Res = (byte*)p + p->Pos;
    p->Pos += Size;
	}
  Assert(Res != null, "Failed to push to pool.");
  return Res;
}
inline void PoolPop(pool *p, u64 Size) {
  if (Size < p->Pos)
    p->Pos -= Size;
}
#ifdef __cplusplus
  inline void  pool::Release(void) {ReleasePool(this);}
  inline void *pool::Put(u64 Size) {return PoolPut(this, Size);}
  inline void  pool::Pop(u64 Size) {PoolPop(this, Size);}
#endif

function str StrFmt(c8 *Fmt, ...) {
  va_list Args;
  va_start(Args, Fmt);
  c8 Buff[1024];
  u32 Len = vsnprintf_s(Buff, 1024, 1024, Fmt, Args);
  va_end(Args);
  return (str){Buff, Len};
}

function b32 _ArrayMake(u64 *Cap, u64 *Len, u64 Itm, void **Mem, u64 InitCap) {
  *Mem = MemRes(Itm*InitCap);
  *Cap = InitCap;
  *Len = 0;
  if (*Mem == null)
    return false;
  return true;
}
function b32 _ArrayGrow(u64 *Cap, u64 *Len, u64 Itm, void **Mem) {
  if ((*Mem) == null) {
    *Mem = MemRes(Itm*ARRAY_INIT_CAP);
    if ((*Mem) == null)
      return false;
  }
  else {
    u64   NewCap = ((*Cap) == 0)? 8 : (*Cap) * 2;
    void *NewMem = MemRes(NewCap*Itm);
    if (NewMem != null)
      MemCpy(NewMem, *Mem, (*Len)*Itm);
    MemRel(*Mem, (*Len)*Itm);
    if (NewMem == null)
      return false;
    *Mem = NewMem;
    *Cap = NewCap;
  }
  return true;
}

function b32 _TableMake(u64 *Cap, u64 *Len, u64 **Keys, byte **Vals, u64 Itm, u64 InitCap) {
  InitCap = AlignUpPow2(max(InitCap, TABLE_MIN_CAP), 16);
  *Keys = (u64*)MemRes(InitCap*sizeof(u64));
  *Vals = (byte*)MemRes(InitCap*Itm);
  *Cap  = InitCap;
  *Len  = 0;
  if (*Keys == null || *Vals == null)
    return false;
  return true;
}
function void _TableFree(u64 *Cap, u64 *Len, u64 **Keys, byte **Vals, u64 Itm) {
  MemRel(*Keys, (*Cap)*sizeof(u64));
  MemRel(*Vals, (*Cap)*Itm);
  *Cap  = 0;
  *Len  = 0;
  *Keys = null;
  *Vals = null;
}
function b32 _TableGrow(u64 *Cap, u64 *Len, u64 **Keys, byte **Vals, u64 Itm, u64 NewCap) {
  u64  OldCap  = *Cap;
  u64  *OldKeys = *Keys;
  byte *OldVals = *Vals;
  *Cap  = AlignUpPow2(max(NewCap, TABLE_MIN_CAP), 16);
  *Len  = 0;
  *Keys = (u64*)MemRes((*Cap)*sizeof(u64));
  *Vals = (byte*)MemRes((*Cap)*Itm);
  fornum (Index, OldCap) {
    u64 Key = OldKeys[Index];
    if (Key) {
      if (!_TableAdd(Cap, Len, Keys, Vals, Itm, Key, OldVals + Index*Itm))
        return false;
    }
  }
  MemRel(OldKeys, OldCap*sizeof(u64));
  MemRel(OldVals, OldCap*Itm);
  return true;
}
function void *_TableAdd(u64 *Cap, u64 *Len, u64 **Keys, byte **Vals, u64 Itm, u64 Key, void *Val) {
  Assert(Key);
  if (((*Len) + 1)*100 >= (*Cap)*TABLE_MAX_LOAD) {
    if (!_TableGrow(Cap, Len, Keys, Vals, Itm, 2*(*Cap)))
      return false;
  }
  u64 Index = HashU64(Key) & *Cap - 1;
  u32 Probe = 1;
  while ((*Keys)[Index]) {
    Index += Probe;
    Probe += 1;
    while (Index >= *Cap)
      Index -= *Cap;
  }
  *Len += 1;
  (*Keys)[Index] = Key;
  MemCpy(*Vals + Index*Itm, Val, Itm);
  MemSet(Val, 0, Itm);
  return *Vals + Index*Itm;
}
function void *_TableGet(u64 *Cap, u64 *Len, u64 **Keys, byte **Vals, u64 Itm, u64 Key) {
  if (*Len == 0)
    return null;
  u64 Index = HashU64(Key) & *Cap - 1;
  u32 Probe = 1;
  while ((*Keys)[Index]) {
    if ((*Keys)[Index] == Key)
      return (void*)Vals + Index*Itm;
    Index += Probe;
    Probe += 1;
    while (Index >= *Cap)
      Index -= *Cap;
  }
  return null;
}

inline u64 GetTicks(void) {
  LARGE_INTEGER Count = {0};
  if (QueryPerformanceCounter(&Count))
    return Count.QuadPart;
  return -1;
}
inline u64 GetUsecs(void) {
  LARGE_INTEGER Count = {0};
  LARGE_INTEGER Freq  = {0};
  if (QueryPerformanceCounter(&Count) && QueryPerformanceFrequency(&Freq))
    return Count.QuadPart*1000000/Freq.QuadPart;
  return -1;
}

function str FileOpen(c8 *Path) {
  HANDLE File = CreateFileA(Path, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  u32    Len  = GetFileSize(File, 0);
  c8    *Ptr  = (c8*)MemRes(Len+1);
  ReadFile(File, Ptr, Len, 0, 0);
  CloseHandle(File);
  return (str){Ptr, Len};
}
function b32 FileSave(c8 *Path, byte *Data, u32 Size) {
  b32 Success = true;
  HANDLE File = CreateFileA(Path, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
  if (!WriteFile(File, Data, Size, 0, 0))
    Success = false;
  CloseHandle(File);
  return Success;
}
function b32 CreateDir(c8 *Path) {
  return CreateDirectoryA(Path, 0);
}
function b32 DeleteDir(c8 *Path) {
  return RemoveDirectoryA(Path);
}

function dll LoadDll(c8 *Path) {
  return (dll)LoadLibraryA(Path);
}
function proc *GetProc(dll Lib, c8 *Name) {
  return (proc*)GetProcAddress((HMODULE)(Lib), Name);
}
function void FreeDll(dll Lib) {
  FreeLibrary((HMODULE)(Lib));
}

#endif
