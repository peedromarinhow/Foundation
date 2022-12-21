#include <math.h>
#include <stdio.h>
#include <string.h>

/**************************************************************************************************

  LAYER ZERO

  FOUNDATION

.doc:
  Todo().

***************************************************************************************************/



/**************************************************************************************************
  HEADER
***************************************************************************************************/
#if !defined(FOUNDATION_HEAD)
#define FOUNDATION_HEAD

////////////////////////
// CONTEXT CRACKING
#if defined(__clang__)
  #define COMPILER_CLANG 1
  #if defined(_WIN32)
    #define OS_WIN 1
  #elif defined(__gnu_linux__)
    #define OS_LNX 1
  #elif defined(__APPLE__) && defined(__MACH__)
    #define OS_MAC 1
  #else
    #error "No operating system defined Time this compiler."
  #endif
  #if defined(__amd64__)
    #define ARCH_X64 1
  #elif defined(__i386__)
    #define ARCH_X86 1
  #elif defined(__arm__)
    #define ARCH_ARM 1
  #elif defined(__aarch64__)
    #define ARCH_ARM64 1
  #else
    #error "No architecture defined Time this compiler."
  #endif
#elif defined(__GNUC__)
  #define COMPILER_GCC 1
  #if defined(_WIN32)
    #define OS_WIN 1
  #elif defined(__gnu_linux__)
    #define OS_LNX 1
  #elif defined(__APPLE__) && defined(__MACH__)
    #define OS_MAC 1
  #else
    #error "No operating system defined Time this compiler."
  #endif
  #if defined(__amd64__)
    #define ARCH_X64 1
  #elif defined(__i386__)
    #define ARCH_X86 1
  #elif defined(__arm__)
    #define ARCH_ARM 1
  #elif defined(__aarch64__)
    #define ARCH_ARM64 1
  #else
    #error "No architecture defined Time this compiler."
  #endif
#elif defined(_MSC_VER)
  #define COMPILER_CL 1
  #if defined(_WIN32)
    #define OS_WIN 1
  #else
    #error "No operating system defined Time this compiler."
  #endif
  #if defined(_M_AMD64)
    #define ARCH_X64 1
  #elif defined(_M_I86)
    #define ARCH_X86 1
  #elif defined(_M_ARM)
    #define ARCH_ARM 1
  #else
    #error "No architecture defined Time this compiler."
  #endif
#else
  #error "No context defined Time this compiler."
#endif

////////////////////////
// SYNTAX SUGAR
#define null  0
#define true  1
#define false 0

#define local    static
#define global   static
#define persist  static
#define function static

#define unused(a)  ((void)a)
#define countof(a) ((int)(sizeof(a) / sizeof(*(a))))
#define cast(t, x) ((t)(x))
//.note: These were Written with a lowecase first letter because they were supposed to be more like C keywords.

#define _Glue(x, y) x##y
#define Glue(x, y) _Glue(x, y)
#define Stringify(s) #s
#define UniqueId(Name) Glue(Name, __LINE__)
#define Todo(s)

#define AssertBreak() __debugbreak(); SysAbort(1);
#if defined(ENABLE_ASSERT)
  #define Assert(Expr, ...) if (!(Expr)) { printf("(" #Expr ") Assert failed: " __VA_ARGS__); AssertBreak(); }
#else
	#define Assert()
#endif
#define StaticAssert(Expr) typedef char UniqueId(_compile_assert_afray)[(Expr) ? 1 : -1]

#define Member(t,m) (((t*)0)->m)
#define MemberOffset(t,m) IntFromPtr(&Member(t,m))

#define IntFromPtr(p) (u64)((u8*)p - (u8*)0)
#define PtrFromInt(x) (void*)((u8*)0 + (x))

#define Min(x, y)      ((x) <= (y)? (x) : (y))
#define Max(x, y)      ((x) >= (y)? (x) : (y))
#define Clamp(x, v, y) ((v) <  (x)? (x) : (y) < (v)? (y) : (v))
#define ClampNorm(x, v, y) Clamp(0, v, 1)

#define IsPowerOf2(x)     (((x) != 0) && ((x) &  ((x) - 1)) == 0)
#define AlignUpPow2(x, p) (((x) +   (p) - 1)  & ~((p) - 1))
#define AlignDoPow2(x, p)  ((x) & ~((p) - 1))
#define AlignUpPtr(p, a)   ((void *)AlignUpPow2((uintptr_t)(p), (a)))
#define AlignDoPtr(p, a)   ((void *)AlignDoPow2((uintptr_t)(p), (a)))

#define Kb(x) ((x) << 10)
#define Mb(x) ((x) << 20)
#define Gb(x) ((x) << 30)
#define Tb(x) ((x) << 40)

#define Thousand(x) (x * 1000)
#define Million(x)  (x * 1000000)
#define Billion(x)  (x * 1000000000llu)
#define Trillion(x) (x * 1000000000000llu)

#define Kilo(x) Thousand(x)
#define Mega(x) Million(x)
#define Giga(x) Billion(x)
#define Tera(x) Trillion(x)

#define Flag(n) ((1u) << n)

#define ItrNum(i, n)    for (size (i) = 0; (i) < (n); (i) += 1)
#define ItrPtr(i, s, e) for (size (i) = 0; (s) < (e); (i) += 1, (s)++)

#define V2AddComps(v1, v2) (v1).x + (v1).x, (v1).y + (v2).y
#define V2SubComps(v1, v2) (v1).x - (v1).x, (v1).y - (v2).y

#define R1AddComps(r1, r2) Min(r1.Min, r2.Min), Max(r1.Max, r2.Max)
#define R1SubComps(r1, r2) Max(r1.Min, r2.Min), Min(r1.Max, r2.Max)

#define R1Overlap(r1, r2) ((r1).Min < (r2).Max && (r2).Min < (r1).Max)
#define R2Overlap(r1, r2) ((r1.x0 < r2.x1 && r2.x0 < r1.x1) && (r1.y0 < r2.y1 && r2.y0 < r1.y1))

#define R1Contains(r, x) ((r).Min <= (x) && (x) < (r).Max)
#define R2Contains(r, v) ((r.x0 <= v.x && v.x < r.x1) && (r.y0 <= v.x && v.x < r.y1))

#define R1Len(r) ((r).Max - (r).Min)
#define R2LenComps(r) r.x1 - r.x0, r.y1 - r.y0

#define R1Mid(r) (((r).Min + (r).Max)/2)
#define R2MidComps(r) r.x0 + (r.x1 - r.x0)/2, r.y0 + (r.y1 - r.y0)/2

////////////////////////
// BASIC TYPES
typedef unsigned long long size;
typedef unsigned char      byte;

typedef signed char   i8;
typedef short         i16;
typedef int           i32;
typedef long long int i64;
StaticAssert(sizeof(i8)  == 1);
StaticAssert(sizeof(i16) == 2);
StaticAssert(sizeof(i32) == 4);
StaticAssert(sizeof(i64) == 8);
#define i8Min  (i8 )0x80
#define i16Min (i16)0x8000
#define i32Min (i32)0x80000000
#define i64Min (i64)0x8000000000000000llu
#define i8Max  (i8 )0x7F
#define i16Max (i16)0x7FFF
#define i32Max (i32)0x7FFFFFFF
#define i64Max (i64)0x7FFFFFFFFFFFFFFFllu

typedef unsigned char          u8;
typedef unsigned short         u16;
typedef unsigned int           u32;
typedef unsigned long long int u64;
StaticAssert(sizeof(u8)  == 1);
StaticAssert(sizeof(u16) == 2);
StaticAssert(sizeof(u32) == 4);
StaticAssert(sizeof(u64) == 8);
#define u8Max  0xFF
#define u16Max 0xFFFF
#define u32Max 0xFFFFFFFF
#define u64Max 0xFFFFFFFFFFFFFFFFllu

typedef char           c8;
typedef unsigned short c16;
typedef unsigned int   c32;
StaticAssert(sizeof(c8)  == 1);
StaticAssert(sizeof(c16) == 2);
StaticAssert(sizeof(c32) == 4);
#define IsSpace(c) ((c) == ' ' || (c) == '\t' || (c) == '\r')
#define IsSlash(c) ((c) == '\\' || (c) == '/')
#define IsDigit(c) ((c) >= '0' && (c) <= '9')
#define IsUpper(c) ((c) >= 'A' && (c) <= 'Z')
#define IsLower(c) ((c) >= 'a' && (c) <= 'z')
#define IsAlpha(c) (IsLower(c) || IsUpper(c))
#define ToUpper(c) (IsLower(c)? (c) + 'A'-'a' : c)
#define ToLower(c) (IsUpper(c)? (c) + 'a'-'A' : c)

typedef float  r32;
typedef double r64;
StaticAssert(sizeof(r32) == 4);
StaticAssert(sizeof(r64) == 8);
#define r32Pi    3.14159265359f
#define r32Tau   6.28318530718f
#define r32Euler 2.71828182846f
#define r64Pi    3.14159265359
#define r64Tau   6.28318530718
#define r64Euler 2.71828182846

typedef i8  b8;
typedef i16 b16;
typedef i32 b32;
typedef i64 b64;

typedef void void_proc(void);

////////////////////////
// COMPUND TYPES
typedef union _i32v2 {
  struct {
    i32 x, y;
  };
  struct {
    i32 w, h;
  };
	i32 c[2];
} i32v2;
typedef union _r32v2 {
  struct {
    r32 x, y;
  };
  struct {
    r32 w, h;
  };
	r32 c[2];
} r32v2;

typedef union _u64r1 {
  struct  {
    u64 Min, Max;
  };
  struct  {
    u64 First, AfterLast;
  };
  u64 c[2];
} u64r1;
typedef union _r32r1 {
  struct  {
    r32 Min, Max;
  };
  r32 c[2];
} r32r1;
typedef union _i32r2 {
  struct {
    i32v2 Min, Max;
  };
  struct {
    i32v2 p1, p2;
  };
  struct {
    r32 x0, y0, x1, y1;
  };
  i32v2 v[2];
  i32   c[4];
} i32r2;
typedef union _r32r2 {
  struct {
    r32v2 Min, Max;
  };
  struct {
    r32v2 p1, p2;
  };
  struct {
    r32 x0, y0, x1, y1;
  };
  r32v2 v[2];
  r32   c[4];
} r32r2;

////////////////////////
// MATH FUNCTIONS
function r32 R32Trunc(r32 x);
function r64 R64Trunc(r64 x);

function r32 R32Floor(r32 x);
function r64 R64Floor(r64 x);

function r32 R32Ceil(r32 x);
function r64 R64Ceil(r64 x);

function r32 R32Abs(r32 x);
function r64 R64Abs(r64 x);

function r32 R32Sign(r32 x);
function r64 R64Sign(r64 x);

function r32 R32Sqrt(r32 x);
function r64 R64Sqrt(r64 x);

function r32 R32Log(r32 x);
function r64 R64Log(r64 x);

function r32 R32Sin(r32 x);
function r64 R64Sin(r64 x);

function r32 R32Cos(r32 x);
function r64 R64Cos(r64 x);

function r32 R32Tan(r32 x);
function r64 R64Tan(r64 x);

function r32 R32Atan(r32 x);
function r64 R64Atan(r64 x);

function r32 R32Pow(r32 x, r32 p);
function r64 R64Pow(r64 x, r64 p);

inline i32v2 I32v2(i32 x, i32 y);
inline r32v2 R32v2(r32 x, r32 y);

inline i32v2 I32v2Add(i32v2 v1, i32v2 v2);
inline r32v2 R32v2Add(r32v2 v1, r32v2 v2);

inline i32v2 I32v2Sub(i32v2 v1, i32v2 v2);
inline r32v2 R32v2Sub(r32v2 v1, r32v2 v2);

inline r32r1 R32r1(r32 Min, r32 Max);
inline u64r1 U64r1(u64 Min, u64 Max);

inline r32r1 R32r1Add(r32r1 r1, r32r1 r2);
inline u64r1 U64r1Add(u64r1 r1, u64r1 r2);

inline r32r1 R32r1Sub(r32r1 r1, r32r1 r2);
inline u64r1 U64r1Sub(u64r1 r1, u64r1 r2);

inline b32 R32r1Overlaps(r32r1 r1, r32r1 r2);
inline b32 U64r1Overlaps(u64r1 r1, u64r1 r2);

inline b32 R32r1Contains(r32r1 r, r32 x);
inline b32 U64r1Contains(u64r1 r, u64 x);

inline r32 R32r1Len(r32r1 r);
inline u64 U64r1Len(u64r1 r);

inline r32 R32r1Mid(r32r1 r);
inline u64 U64r1Mid(u64r1 r);

inline i32r2 I32r2(i32v2 Min, i32v2 Max);
inline r32r2 R32r2(r32v2 Min, r32v2 Max);

inline i32r2 I32r2Add(i32r2 r1, i32r2 r2);
inline r32r2 R32r2Add(r32r2 r1, r32r2 r2);

inline i32r2 I32r2Sub(i32r2 r1, i32r2 r2);
inline r32r2 R32r2Sub(r32r2 r1, r32r2 r2);

inline b32 I32r2Overlaps(i32r2 r1, i32r2 r2);
inline b32 R32r2Overlaps(r32r2 r1, r32r2 r2);

inline b32 I32r2Contains(i32r2 r, i32v2 v);
inline b32 R32r2Contains(r32r2 r, r32v2 v);

inline i32v2 I32r2Len(i32r2 r);
inline r32v2 R32r2Len(r32r2 r);

inline i32v2 I32r2Mid(i32r2 r);
inline r32v2 R32r2Mid(r32r2 r);

////////////////////////
// HASH FUNCTIONS
function u64 StrRangeHash(c8 *Start, c8 *End);
function u64 U64Hash(u64 x);
function u64 U64HashMix(u64 x, u64 y);

////////////////////////
// MEMORY
#define POOL_INIT_CAP Gb(1)
typedef struct _pool {
	size Cap;
	size Pos;
} pool;
function pool *PoolReserve  (size cap);
function void  PoolRelease  (pool *Pool);
function void *PoolPush     (pool *Pool, size Size);
function void *PoolPushZeros(pool *Pool, size Size);
function void  PoolPopTo    (pool *Pool, size Pos);
function void  PoolPopAmount(pool *Pool, size Amount);

typedef struct _pool_snap {
  pool *Pool;
  size  Pos;
} pool_snap;
function pool_snap GetPoolSnapshot(pool *Pool);
function void      EndPoolSnapshot(pool_snap Snap);

////////////////////////
// DYNAMIC ARRAY
#define ARRAY_INIT_CAP 16
#define array(type) struct { size Cap, Len; type *Mem; }

function b32 _ArrayMake(size *Cap, size *Len, size Itm, void **Mem, size InitCap);
function b32 _ArrayGrow(size *Cap, size *Len, size Itm, void **Mem);

#define _ArrayExp(Array) &(Array)->Cap, &(Array)->Len, sizeof(*(Array)->Mem), cast(void**, &(Array)->Mem) 
#define  ArrayMake(Array, Len) _ArrayMake(_ArrayExp(Array), (Len))
#define  ArrayFree(Array) SysMemRelease((Array)->Mem, (Array)->Cap*sizeof(*(Array)->Mem)), (Array)->Cap = 0, (Array)->Len = 0, (Array)->Mem = null
#define  ArrayAdd(Array, x) (        \
  ((Array)->Cap < (Array)->Len + 1)? \
    _ArrayGrow(_ArrayExp(Array))     \
  :                                  \
    0,                               \
  (Array)->Mem[(Array)->Len] = (x),  \
  (Array)->Len++                     \
)

////////////////////////
// HASH TABLE
//.link: https://youtu.be/k9MBMvR2IvI;
//.link: https://github.com/pervognsen/bitwise.
//.link: https://github.com/rxi/map.
#define TABLE_MIN_CAP  16
#define TABLE_MAX_LOAD 70
#define table(type) struct { size Cap, Len; u64 *Keys; byte *Vals; type Tmp; type *Ptr; }

function b32   _TableMake(size *Cap, size *Len, u64 **Keys, byte **Vals, size Itm, size InitCap);
function void  _TableFree(size *Cap, size *Len, u64 **Keys, byte **Vals, size Itm);
function b32   _TableGrow(size *Cap, size *Len, u64 **Keys, byte **Vals, size Itm, size NewCap);
function void *_TableAdd (size *Cap, size *Len, u64 **Keys, byte **Vals, size Itm, u64 Key, void *Val);
function void *_TableGet (size *Cap, size *Len, u64 **Keys, byte **Vals, size Itm, u64 Key);

#define _TableExp(Table) &(Table)->Cap, &(Table)->Len, &(Table)->Keys, &(Table)->Vals, sizeof((Table)->Tmp)
#define  TableMake(Table, Len)                           _TableMake(_TableExp(Table), (Len))
#define  TableFree(Table)                                _TableFree(_TableExp(Table))
#define  TableAdd(Table, Key, Val) (Table)->Tmp = (Val), _TableAdd (_TableExp(Table), cast(u64, Key),   &(Table)->Tmp)
#define  TableGet(Table, Key)     ((Table)->Ptr =        _TableGet (_TableExp(Table), cast(u64, Key)))? *(Table)->Ptr : (Table)->Tmp

////////////////////////
// STRINGS
typedef struct _str8 {
	c8  *Ptr;
	size Len;
} str8;
typedef struct _str16 {
	c16 *Ptr;
	size Len;
} str16;
typedef struct _str32 {
	c32 *Ptr;
	size Len;
} str32;
inline   str8  Str8      (c8  *Ptr,   size Len);
inline   str8  Str8Range (c8  *Start, c8  *End);
function str8  Str8Cstr  (c8  *Ptr);
inline   str16 Str16     (c16 *Ptr,   size Len);
inline   str16 Str16Range(c16 *Start, c16 *End);
function str16 Str16Cstr (c16 *Ptr);
inline   str32 Str32     (c32 *Ptr,   size Len);
inline   str32 Str32Range(c32 *Start, c32 *End);
function str32 Str32Cstr (c32 *Ptr);

#define StrExp(Str) cast(int, Str.Len), Str.Ptr

typedef array(str8)  str8_array;
typedef array(str16) str16_array;
typedef array(str32) str32_array;

typedef struct _utf_char {
	u32  Code;
	size Size;
} utf_char;
function utf_char DecodeUtf8 (c8  *Ptr, u32 Size);
function c32      EncodeUtf8 (c8  *Dst, u32 Codepoint);
function utf_char DecodeUtf16(c16 *Ptr, u32 Size);
function c32      EncodeUtf16(c16 *Dst, u32 Codepoint);

function str32 ConvertStr8ToStr32(pool *Pool, str8  Str);
function str8  ConvertStr32ToStr8(pool *Pool, str32 Str);
function str16 ConvertStr8ToStr16(pool *Pool, str8  Str);
function str8  ConvertStr16ToStr8(pool *Pool, str16 Str);

////////////////////////
// TIME
typedef enum _month {
  month_Jan,
  month_reb,
  month_Mar,
  month_Apr,
  month_May,
  month_Jun,
  month_Jul,
  month_Aug,
  month_Sep,
  month_Oct,
  month_Nov,
  month_Dec
} month;
typedef enum _week_day {
  week_day_Sun,
  week_day_Mon,
  week_day_Tue,
  week_day_Wed,
  week_day_Thu,
  week_day_Fri,
  week_day_Sat
} week_day;
typedef u64 dense_time;
typedef struct _date_time {
  u16 Msec;  // [0, 999]
  u8  Sec;   // [0, 59]
  u8  Min;   // [0, 59]
  u8  Hour;  // [0, 23]
  u8  Day;   // [1, 31]
  u8  Month; // [1, 12]
  i16 Year;  // 1 = 1CE, 2020 = 2020CE, 0 = 1BCE, -100 = 101BCE, etc.
} date_time;
function dense_time DenseTimeFromDate(date_time *Date);
function date_time  DateTimeFromDense(dense_time Dense);

////////////////////////
// SYSTEM INTERFACE
function i32 Main(str8 ArgStr);

function void SysAbort(i32 Code);

enum _mem_flags {
  mem_Unaccessible = Flag(0),
  mem_Readonly     = Flag(1),
  mem_Runnable     = Flag(2),
};
function void *SysMemReserve(size Size, u32 Flags);
function void  SysMemRelease(void *Ptr, size Size);

function date_time SysUniversalTime(void);
function u64  SysGetTicks(void);
function u64  SysGetMicroseconds(void); //.note: Does not return 'dense_time'!
function void SysSleep(u32 Milliseconds);

function str8 SysOpenFile  (pool *Pool,  str8 Path);
function b32  SysSaveFile  (str8  Data,  str8 Path);
function b32  SysRenameFile(str8  Old,   str8 New);
function b32  SysDeleteFile(str8  Path);
function b32 SysCreateDir  (str8 Path);
function b32 SysDeleteDir  (str8 Path);

typedef struct _file_props {
  u8 Readonly;
  b8 IsDir;
  size Size;
  dense_time Creation;
  dense_time Modified;
} file_props;
function file_props SysGetFileProps(str8 Path);

typedef struct _file_iter {
  byte Data[640];
} file_iter;
function file_iter  SysInitFileIter(str8 Path);
function b32        SysNextFileIter(pool *Pool, file_iter *Iter, str8 *NameOut, file_props *PropsOut);
function void       SysKillFileIter(file_iter *Iter);

typedef struct _dyn_lib {
  u64 Data;
} dyn_lib;
function dyn_lib    SysLoadLib(str8 Path);
function void_proc *SysGetLibProc(dyn_lib Lib, c8 *Name);
function void       SysReleaseLib(dyn_lib Lib);

typedef struct _wnd {
  b32 Finish;
} wnd;
function wnd *SysInitWnd(void);
function void SysKillWnd(wnd *Wnd);
function void SysWndPull(wnd *Wnd);
function void SysWndPush(wnd *Wnd);

#endif//FOUNDATION_HEAD

/**************************************************************************************************
  PLTFORM INDEPENDENT IMPLEMENTATION
***************************************************************************************************/
#if defined(FOUNDATION_IMPL)

////////////////////////
// MATH FUNCTIONS
function r32 R32Trunc(r32 x) {
  return truncf(x);
}
function r64 R64Trunc(r64 x) {
  return trunc(x);
}

function r32 R32Floor(r32 x) {
  return floorf(x);
}
function r64 R64Floor(r64 x) {
  return floor(x);
}

function r32 R32Ceil(r32 x) {
  return ceilf(x);
}
function r64 R64Ceil(r64 x) {
  return ceil(x);
}

function r32 R32Abs(r32 x) {
  union { r32 f; u32 u; } r = {x};
  r.u &= 0x7fffffff;
  return r.f;
} 
function r64 R64Abs(r64 x) {
  union { r64 f; u64 u; } r = {x};
  r.u &= 0x7fffffffffffffff;
  return(r.f);
}

function r32 R32Sign(r32 x) {
  union { r32 f; u32 u; } r = {x};
  return (r.u&0x80000000)? -1.f : 1.f;
}
function r64 R64Sign(r64 x) {
  union { r64 f; u64 u; } r = {x};
  return (r.u&0x8000000000000000)? -1. : 1.;
}

function r32 R32Sqrt(r32 x) {
  return sqrtf(x);
}
function r64 R64Sqrt(r64 x) {
  return sqrt(x);
}

function r32 R32Log(r32 x) {
  return logf(x);
}
function r64 R64Log(r64 x) {
  return log(x);
}

function r32 R32Sin(r32 x) {
  return sinf(x);
}
function r64 R64Sin(r64 x) {
  return sin(x);
}

function r32 R32Cos(r32 x) {
  return cosf(x);
}
function r64 R64Cos(r64 x) {
  return cos(x);
}

function r32 R32Tan(r32 x) {
  return tanf(x);
}
function r64 R64Tan(r64 x) {
  return tan(x);
}

function r32 R32InvSin(r32 x) {
  return asinf(x);
}
function r64 R64InvSin(r64 x) {
  return asin(x);
}

function r32 R32InvCos(r32 x) {
  return acosf(x);
}
function r64 R64InvCos(r64 x) {
  return acos(x);
}

function r32 R32InvTan(r32 x) {
  return atanf(x);
}
function r64 R64InvTan(r64 x) {
  return atan(x);
}

function r32 R32Atan(r32 x) {
  return atanf(x);
}
function r64 R64Atan(r64 x) {
  return atan(x);
}

function r32 R32Pow(r32 x, r32 p) {
  return powf(x, p);
}
function r64 R64Pow(r64 x, r64 p) {
  return pow(x, p);
}

inline i32v2 I32v2(i32 x, i32 y) {
  i32v2 Res = {x, y};
  return Res;
}
inline r32v2 R32v2(r32 x, r32 y) {
  r32v2 Res = {x, y};
  return Res;
}

inline i32v2 I32v2Add(i32v2 v1, i32v2 v2) { return I32v2(V2AddComps(v1, v2)); }
inline r32v2 R32v2Add(r32v2 v1, r32v2 v2) { return R32v2(V2AddComps(v1, v2)); }

inline i32v2 I32v2Sub(i32v2 v1, i32v2 v2) { return I32v2(V2SubComps(v1, v2)); }
inline r32v2 R32v2Sub(r32v2 v1, r32v2 v2) { return R32v2(V2SubComps(v1, v2)); }

inline r32r1 R32r1(r32 Min, r32 Max) {
  r32r1 Res = {Min, Max};
  return Res;
}
inline u64r1 U64r1(u64 Min, u64 Max) {
  u64r1 Res = {Min, Max};
  return Res;
}

inline r32r1 R32r1Add(r32r1 r1, r32r1 r2) { return R32r1(R1AddComps(r1, r2)); }
inline u64r1 U64r1Add(u64r1 r1, u64r1 r2) { return U64r1(R1AddComps(r1, r2)); }

inline r32r1 R32r1Sub(r32r1 r1, r32r1 r2) { return R32r1(R1SubComps(r1, r2)); }
inline u64r1 U64r1Sub(u64r1 r1, u64r1 r2) { return U64r1(R1SubComps(r1, r2)); }

inline b32 R32r1Overlaps(r32r1 r1, r32r1 r2) { return R1Overlap(r1, r2); }
inline b32 U64r1Overlaps(u64r1 r1, u64r1 r2) { return R1Overlap(r1, r2); }

inline b32 R32r1Contains(r32r1 r, r32 x) { return R1Contains(r, x); }
inline b32 U64r1Contains(u64r1 r, u64 x) { return R1Contains(r, x); }

inline r32 R32r1Len(r32r1 r) { return R1Len(r); }
inline u64 U64r1Len(u64r1 r) { return R1Len(r); }

inline r32 R32r1Mid(r32r1 r) { return R1Mid(r); }
inline u64 U64r1Mid(u64r1 r) { return R1Mid(r); }

inline i32r2 I32r2(i32v2 Min, i32v2 Max) {
  i32r2 Res = {Min, Max};
  return Res;
}
inline r32r2 R32r2(r32v2 Min, r32v2 Max) {
  r32r2 Res = {Min, Max};
  return Res;
}

Todo();
// inline i32r2 I32r2Add(i32r2 r1, i32r2 r2) { return I32r2(I32v2(Min(r1.Min.x, r1.Min.y), Min(r1.Min.y)), I32v2(Max(r1.Max.x), Max(r1.Max.y))); }
// inline r32r2 R32r2Add(r32r2 r1, r32r2 r2) { return R32r2(R32v2(Min(r1.Min.x, r1.Min.y), Min(r1.Min.y)), R32v2(Max(r1.Max.x), Max(r1.Max.y))); }

Todo();
// inline i32r2 I32r2Sub(i32r2 r1, i32r2 r2) { return I32r2(I32v2(Max(r1.Min.x), Max(r1.Min.y)), I32v2(Min(r1.Max.x), Min(r1.Max.y))); }
// inline r32r2 R32r2Sub(r32r2 r1, r32r2 r2) { return R32r2(R32v2(Max(r1.Min.x), Max(r1.Min.y)), R32v2(Min(r1.Max.x), Min(r1.Max.y))); }

inline b32 I32r2Overlaps(i32r2 r1, i32r2 r2) { return R2Overlap(r1, r2); }
inline b32 R32r2Overlaps(r32r2 r1, r32r2 r2) { return R2Overlap(r1, r2); }

inline b32 I32r2Contains(i32r2 r, i32v2 v) { return R2Contains(r, v); }
inline b32 R32r2Contains(r32r2 r, r32v2 v) { return R2Contains(r, v); }

inline i32v2 I32r2Len(i32r2 r) { return I32v2(R2LenComps(r)); }
inline r32v2 R32r2Len(r32r2 r) { return R32v2(R2LenComps(r)); }

inline i32v2 I32r2Mid(i32r2 r) { return I32v2(R2MidComps(r)); }
inline r32v2 R32r2Mid(r32r2 r) { return R32v2(R2MidComps(r)); }

////////////////////////
// HASH FUNCTIONS
function u64 StrRangeHash(c8 *Start, c8 *End) {
  u64 x = 0xcbf29ce484222325ull;
  while (Start != End) {
    x ^= *Start++;
    x *= 1099511628211ull;
    x ^= x >> 32;
  }
  return x;
}
function u64 U64Hash(u64 x) {
  x *= 0xff51afd7ed558ccdull;
  x ^= x >> 32;
  return x;
}
function u64 U64HashMix(u64 x, u64 y) {
  x ^= y;
  x *= 0xff51afd7ed558ccdull;
  x ^= x >> 32;
  return x;
}

////////////////////////
// MEMORY
function pool *PoolReserve(size Cap) {
  if (Cap == 0)
    Cap = POOL_INIT_CAP;
  byte *Mem = SysMemReserve(Cap, 0);
  pool *Res = cast(pool*, Mem);
  Res->Cap = Cap;
  Res->Pos = AlignUpPow2(sizeof(pool), 64);
  Assert(Res != null, "Failed to create pool.");
  return Res;
}
function void PoolRelease(pool *Pool) {
  SysMemRelease(cast(byte*, Pool), Pool->Cap);
}
function void *PoolPush(pool *Pool, size Size) {
  void *Res = null;
	if (Pool->Pos + Size <= Pool->Cap) {
		Res = cast(byte*, Pool) + Pool->Pos;
    Pool->Pos += Size;
	}
  Assert(Res != null, "Failed to push to pool.");
  return Res;
}
function void *PoolPushZeros(pool *Pool, size Size) {
  void *Res = PoolPush(Pool, Size);
  memset(Res, 0, Size);
  return Res;
}
function void PoolPopTo(pool *Pool, size Pos) {
  if (Pos < Pool->Pos)
    Pool->Pos = Pos;
}
function void PoolPopAmount(pool *Pool, size Amount) {
  if (Amount < Pool->Pos)
    PoolPopTo(Pool, Pool->Pos - Amount);
}

function pool_snap GetPoolSnapshot(pool *Pool) {
  pool_snap Res = {Pool, Pool->Pos};
  return Res;
}
function void EndPoolSnapshot(pool_snap Snap) {
  PoolPopTo(Snap.Pool, Snap.Pos);
}

////////////////////////
// DYNAMIC ARRAY
function b32 _ArrayMake(size *Cap, size *Len, size Itm, void **Mem, size InitCap) {
  *Mem = SysMemReserve(Itm*InitCap, 0);
  *Cap = InitCap;
  *Len = 0;
  if (*Mem == null)
    return false;
  return true;
}
function b32 _ArrayGrow(size *Cap, size *Len, size Itm, void **Mem) {
  if ((*Mem) == null) {
    *Mem = SysMemReserve(Itm*ARRAY_INIT_CAP, 0);
    if ((*Mem) == null)
      return false;
  }
  else {
    size  NewCap = ((*Cap) == 0)? 8 : (*Cap) * 2;
    void *NewMem = SysMemReserve(NewCap*Itm, 0);
    if (NewMem != null)
      memcpy(NewMem, *Mem, (*Len)*Itm);
    SysMemRelease(*Mem, (*Len)*Itm);
    if (NewMem == null)
      return false;
    *Mem = NewMem;
    *Cap = NewCap;
  }
  return true;
}

////////////////////////
// HASH TABLE
//.link: https://github.com/rxi/map/tree/master/src
//.link: https://github.com/pervognsen/bitwise/
function b32 _TableMake(size *Cap, size *Len, u64 **Keys, byte **Vals, size Itm, size InitCap) {
  InitCap = AlignUpPow2(Max(InitCap, TABLE_MIN_CAP), 16);
  *Keys = cast(u64*,  SysMemReserve(InitCap*sizeof(u64), 0));
  *Vals = cast(byte*, SysMemReserve(InitCap*Itm, 0));
  *Cap  = InitCap;
  *Len  = 0;
  if (*Keys == null || *Vals == null)
    return false;
  return true;
}
function void _TableFree(size *Cap, size *Len, u64 **Keys, byte **Vals, size Itm) {
  SysMemRelease(*Keys, (*Cap)*sizeof(u64));
  SysMemRelease(*Vals, (*Cap)*Itm);

  *Cap  = 0;
  *Len  = 0;
  *Keys = null;
  *Vals = null;
}
function b32 _TableGrow(size *Cap, size *Len, u64 **Keys, byte **Vals, size Itm, size NewCap) {
  size  OldCap  = *Cap;
  u64  *OldKeys = *Keys;
  byte *OldVals = *Vals;

  *Cap  = AlignUpPow2(Max(NewCap, TABLE_MIN_CAP), 16);
  *Len  = 0;
  *Keys = cast(u64*,  SysMemReserve((*Cap)*sizeof(u64), 0));
  *Vals = cast(byte*, SysMemReserve((*Cap)*Itm, 0));
  
  ItrNum (Index, OldCap) {
    u64 Key = OldKeys[Index];
    if (Key) {
      if (!_TableAdd(Cap, Len, Keys, Vals, Itm, Key, OldVals + Index*Itm))
        return false;
    }
  }

  SysMemRelease(OldKeys, OldCap*sizeof(u64));
  SysMemRelease(OldVals, OldCap*Itm);

  return true;
}
function void *_TableAdd(size *Cap, size *Len, u64 **Keys, byte **Vals, size Itm, u64 Key, void *Val) {
  Assert(Key);
  if (((*Len) + 1)*100 >= (*Cap)*TABLE_MAX_LOAD) {
    if (!_TableGrow(Cap, Len, Keys, Vals, Itm, 2*(*Cap)))
      return false;
  }

  u64 Index = U64Hash(Key) & *Cap - 1;
  u32 Probe = 1;
  while ((*Keys)[Index]) {
    Index += Probe;
    Probe += 1;
    while (Index >= *Cap)
      Index -= *Cap;
  }

  *Len += 1;
  (*Keys)[Index] = Key;
  memcpy(*Vals + Index*Itm, Val, Itm);
  memset(Val, 0, Itm);

  return *Vals + Index*Itm;
}
function void *_TableGet(size *Cap, size *Len, u64 **Keys, byte **Vals, size Itm, u64 Key) {
  if (*Len == 0)
    return null;

  u64 Index = U64Hash(Key) & *Cap - 1;
  u32 Probe = 1;
  while ((*Keys)[Index]) {
    if ((*Keys)[Index] == Key)
      return cast(void*, *Vals + Index*Itm);
    Index += Probe;
    Probe += 1;
    while (Index >= *Cap)
      Index -= *Cap;
  }

  return null;
}

////////////////////////
// STRINGS
//.link: (adapted from) https://github.com/Mr-4th-Programming/mr4th/blob/main/src/base/base_string.cpp.
inline str8 Str8(c8 *Ptr, size Len) {
	str8 Res = {Ptr, Len};
	return Res;
}
inline str8 Str8Range(c8 *Start, c8 *End) {
	str8 Res = {Start, (size)(End - Start)};
	return Res;
}
function str8 Str8Cstr(c8 *Str) {
  c8 *Ptr = Str;
  while (*Ptr != '\0')
    Ptr++;
	str8 Res = {Str, (size)(Ptr - Str)};
	return Res;
}
inline str16 Str16(c16 *Ptr, size Len) {
	str16 Res = {Ptr, Len};
	return Res;
}
inline str16 Str16Range(c16 *Start, c16 *End) {
	str16 Res = {Start, (size)(End - Start)};
	return Res;
}
function str16 Str16Cstr(c16 *Str) {
  c16 *Ptr = Str;
  while (*Ptr != 0)
    Ptr++;
	str16 Res = {Str, (size)(Ptr - Str)};
	return Res;
}
inline str32 Str32(c32 *Ptr, size Len) {
	str32 Res = {Ptr, Len};
	return Res;
}
inline str32 Str32Range(c32 *Start, c32 *End) {
	str32 Res = {Start, (size)(End - Start)};
	return Res;
}
function str32 Str32Cstr(c32 *Str) {
  c32 *Ptr = Str;
  while (*Ptr != 0)
    Ptr++;
	str32 Res = {Str, (size)(Ptr - Str)};
	return Res;
}

function utf_char DecodeUtf8(c8 *Ptr, u32 Len) {
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
		Res.Code = '#';
		Res.Size = 1;
		byte Byte = Ptr[0];
		u8 Size = CodepointLen[Byte >> 3];
		if (0 < Size && Size <= Len) {
			size Codepoint = (Byte & FirstByteMask[Size]) << 18;
			switch (Size) {
				case 4: Codepoint |= ((Ptr[3] & 0x3F) << 0);
				case 3: Codepoint |= ((Ptr[2] & 0x3F) << 6);
				case 2: Codepoint |= ((Ptr[1] & 0x3F) << 12);
				default: break;
			}
      Codepoint >>= FinalShift[Size];
			Res.Code    = Codepoint;
			Res.Size = Size;
		}
	}
	return Res;
}
function c32 EncodeUtf8(c8 *Dst, u32 Codepoint) {
	size Size = 0;
	if (Codepoint < (1 << 8)) {
		Dst[0] = Codepoint;
		Size = 1;
	}
	else
	if (Codepoint < (1 << 11)) {
		Dst[0] = 0xC0 | (Codepoint >> 6);
		Dst[1] = 0x80 | (Codepoint & 0x3F);
		Size = 2;
	}
	else
	if (Codepoint < (1 << 16)) {
		Dst[0] = 0xE0 |  (Codepoint >> 12);
		Dst[1] = 0x80 | ((Codepoint >> 6) & 0x3F);
		Dst[2] = 0x80 |  (Codepoint       & 0x3F);
		Size = 3;
	}
	else
	if (Codepoint < (1 << 21)) {
		Dst[0] = 0xF0 |  (Codepoint >> 18);
		Dst[1] = 0x80 | ((Codepoint >> 12) & 0x3F);
		Dst[2] = 0x80 | ((Codepoint >> 6)  & 0x3F);
		Dst[3] = 0x80 |  (Codepoint        & 0x3F);
		Size = 4;
	}
	else {
		Dst[0] = '#';
		Size = 1;
	}
	return Size;
}
function utf_char DecodeUtf16(c16 *Ptr, u32 Size) {
	utf_char Res = {'#', 1};
	u16 c1 = Ptr[0];
	if (c1 < 0xD800 || 0xDFFF < c1)
		Res.Code = c1;
	else {
		u16 c2 = Ptr[1];
		if (0xD800 <= c1 && c1 < 0xDC00 &&
		    0xDC00 <= c2 && c2 < 0xE000) {
			Res.Code = (((c1 - 0xDC00) << 10) | (c2 - 0xDC00)) + 0x10000,
			Res.Size = 2;
		}
	}
	return Res;
}
function c32 EncodeUtf16(c16 *Dst, u32 Codepoint) {
	size Len = 0;
	if (Codepoint < 0x10000) {
		Dst[0] = Codepoint;
		Len = 1;
	}
	else {
		size c = Codepoint - 0x10000;
		Dst[0] = (c >> 10)   + 0xD800;
		Dst[1] = (c & 0x3FF) + 0xDC00;
		Len = 2;
	}
	return Len;
}

function str32 ConvertStr8ToStr32(pool *Pool, str8 Str) {
  str32 Res = {0};
  c32  *Mem = PoolPush(Pool, sizeof(c32)*(Str.Len + 1));
	c32  *Dst = Mem;
	c8   *Ptr = Str.Ptr;
	c8   *Opl = Str.Ptr + Str.Len;
	while (Ptr < Opl) {
		utf_char Decoded = DecodeUtf8(Ptr, (size)(Opl - Ptr));
	 *Dst = Decoded.Code;
		Ptr += Decoded.Size;
		Dst++;
	}
 *Dst = 0;
	size Len = (size)(Dst - Mem);
	PoolPopAmount(Pool, sizeof(*Mem)*(Str.Len - Len));
  Res.Ptr = Mem;
  Res.Len = Len;
	return Res;
}
function str8 ConvertStr32ToStr8(pool *Pool, str32 Str) {
  str8 Res = {0};
  c8  *Mem = PoolPush(Pool, sizeof(u8)*(Str.Len*4 + 1));
	c8  *Dst = Mem;
	u32 *Ptr = Str.Ptr;
	u32 *Opl = Str.Ptr + Str.Len;
	while (Ptr < Opl) {
		size Size = EncodeUtf8(Dst, *Ptr);
		Ptr++;
		Dst += Size;
	}
 *Dst = '\0';
	size Len = (size)(Dst - Mem);
	PoolPopAmount(Pool, sizeof(*Mem)*(Str.Len - Len));
  Res.Ptr = Mem;
  Res.Len = Len;
  return Res;
}
function str16 ConvertStr8ToStr16(pool *Pool, str8 Str) {
  str16 Res = {0};
  c16  *Mem = PoolPush(Pool, sizeof(c16)*(Str.Len*2 + 1));
  c16  *Dst = Mem;
  c8   *Ptr = Str.Ptr;
  c8   *End = Str.Ptr + Str.Len;
  while (Ptr < End){
    utf_char Decoded = DecodeUtf8(Ptr, cast(u32, End - Ptr));
    u32      EncSize = EncodeUtf16(Dst, Decoded.Code);
    Ptr += Decoded.Size;
    Dst += EncSize;
  }
 *Dst = 0;
  size Len = (size)(Dst - Mem);
  PoolPopAmount(Pool, sizeof(*Mem)*(Str.Len*2 - Len));
  Res.Ptr = Mem;
  Res.Len = Len;
	return Res;
}
function str8 ConvertStr16ToStr8(pool *Pool, str16 Str) {
  str8 Res = {0};
  c8  *Mem = PoolPush(Pool, sizeof(c8)*(Str.Len*3 + 1));
	c8  *Dst = Mem;
	u16 *Ptr = Str.Ptr;
	u16 *Opl = Str.Ptr + Str.Len;
	while (Ptr < Opl) {
		utf_char Decoded = DecodeUtf16(Ptr, (size)(Opl - Ptr));
		u16      EncSize = EncodeUtf8(Dst, Decoded.Code);
		Ptr += Decoded.Size;
		Dst += EncSize;
	}
 *Dst = 0;
	size Len = (size)(Dst - Mem);
  PoolPopAmount(Pool, sizeof(*Mem)*(Str.Len*3 - Len));
  Res.Ptr = Mem;
  Res.Len = Len;
  return Res;
}

////////////////////////
// TIME
function dense_time DenseTimeFromDate(date_time *Date) {
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
  Res += Date->Min;
  Res *= 61;
  Res += Date->Sec;
  Res *= 1000;
  Res += Date->Msec;
  return Res;
}
function date_time DateTimeFromDense(dense_time Dense) {
  date_time Res = {0};
  Res.Msec = Dense%1000;
  Dense /= 1000;
  Res.Sec = Dense%61;
  Dense /= 61;
  Res.Min = Dense%60;
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

/**************************************************************************************************
  PLATFORM DEPENDENT IMPLEMENTATIONS
***************************************************************************************************/

#if defined(OS_WIN)

#undef function
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include "windows.h"
#include "shellapi.h"
#include "timeapi.h"
#define function static

global pool *GlobalWin32Pool;
global u64   GlobalWin32TicksPerSecond;
global u64   GlobalWin32TicksUponStart;

int main(void) {
  pool *Pool = PoolReserve(0);

  LARGE_INTEGER PerfFrequency = {0};
  if (!QueryPerformanceFrequency(&PerfFrequency))
    fprintf(stderr, "Could not retrieve performance frequency.");
  timeBeginPeriod(1);
  LARGE_INTEGER PerfCounter = {0};
  if (!QueryPerformanceCounter(&PerfCounter))
    fprintf(stderr, "Could not retrieve performance counter.");

  GlobalWin32TicksPerSecond = PerfFrequency.QuadPart;
  GlobalWin32TicksUponStart = PerfCounter.QuadPart;
  GlobalWin32Pool           = Pool;

  str16 ArgStrUtf16 = Str16Cstr(GetCommandLineW());
  str8  ArgStr      = ConvertStr16ToStr8(Pool, ArgStrUtf16);

  i32 Res = Main(ArgStr);

  PoolRelease(GlobalWin32Pool);

  return Res;
}

function void SysAbort(i32 Code) {
  ExitProcess(Code);
}

////////////////////////
// MEMORY
function void *SysMemReserve(size Size, u32 Flags) {
  u32 ProtectionFlags = 0;
  if (Flags & mem_Unaccessible)
    ProtectionFlags |= PAGE_NOACCESS;
  if (Flags & mem_Readonly)
    ProtectionFlags |= PAGE_READONLY;
  if (Flags & mem_Runnable)
    ProtectionFlags |= PAGE_EXECUTE;
  if (!(ProtectionFlags & (PAGE_READONLY|PAGE_NOACCESS)))
    ProtectionFlags |= PAGE_READWRITE;
  return cast(byte*, VirtualAlloc(null, Size, MEM_RESERVE | MEM_COMMIT, ProtectionFlags));
}
function void SysMemRelease(void *Ptr, size Size) {
  VirtualFree(Ptr, Size, MEM_RELEASE);
}

////////////////////////
// TIME
function date_time _Win32ConvertSysTimeToDateTime(SYSTEMTIME *Time){
    date_time Res = {0};
    Res.Year  = Time->wYear;
    Res.Month = (u8)Time->wMonth;
    Res.Day   = Time->wDay;
    Res.Hour  = Time->wHour;
    Res.Min   = Time->wMinute;
    Res.Sec   = Time->wSecond;
    Res.Msec  = Time->wMilliseconds;
    return(Res);
}
function SYSTEMTIME _Win32ConvertDateTimeToSysTime(date_time *Time){
    SYSTEMTIME Res = {0};
    Res.wYear         = Time->Year;
    Res.wMonth        = Time->Month;
    Res.wDay          = Time->Day;
    Res.wHour         = Time->Hour;
    Res.wMinute       = Time->Min;
    Res.wSecond       = Time->Sec;
    Res.wMilliseconds = Time->Msec;
    return(Res);
}
function dense_time _Win32ConvertFileTimeToDenseTime(FILETIME *FileTime){
    SYSTEMTIME SysTime = {0};
    FileTimeToSystemTime(FileTime, &SysTime);
    date_time  DateTime = _Win32ConvertSysTimeToDateTime(&SysTime);
    dense_time Res      = DenseTimeFromDate(&DateTime);
    return(Res);
}

function date_time SysUniversalTime(void) {
  SYSTEMTIME SysTime = {0};
  GetSystemTime(&SysTime);
  date_time Res = _Win32ConvertSysTimeToDateTime(&SysTime);
  return Res;
}
function u64 SysGetTicks(void) {
  LARGE_INTEGER PerfCounter = {0};
  if (QueryPerformanceCounter(&PerfCounter))
    return PerfCounter.QuadPart;
  return -1;
}
function u64 SysGetMicroseconds(void) {
  LARGE_INTEGER PerfCounter = {0};
  if (QueryPerformanceCounter(&PerfCounter))
    return PerfCounter.QuadPart*Million(1)/GlobalWin32TicksPerSecond;
  return -1;
}
function void SysSleep(u32 Milliseconds) {
  Sleep(Milliseconds);
}

////////////////////////
// FILES
function str8 SysOpenFile(pool *Pool, str8 Path) {
  str8 Res = {0};

  pool_snap Snap      = GetPoolSnapshot(GlobalWin32Pool);
  str16     PathUtf16 = ConvertStr8ToStr16(Snap.Pool, Path);

  HANDLE File = CreateFileW(cast(WCHAR*, PathUtf16.Ptr), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  if (File != INVALID_HANDLE_VALUE) {
    DWORD HiSize    = 0;
    DWORD LoSize    = GetFileSize(File, &HiSize);
    u64   TotalSize = ((cast(u64, HiSize)) << 32) | cast(u64, LoSize);

    pool_snap RestorePoint = GetPoolSnapshot(Pool);
    byte     *Buffer       = cast(byte*, PoolPush(Pool, sizeof(byte)*TotalSize));

    byte *Ptr         = Buffer;
    byte *Opl = Buffer + TotalSize;
    b32   Success     = true;
    while (Ptr < Opl) {
      DWORD ToRead = cast(DWORD, Opl - Ptr);
      if (ToRead > u32Max)
        ToRead = u32Max;

      DWORD ActuallyRead = 0;
      if (!ReadFile(File, Ptr, ToRead, &ActuallyRead, 0)) {
        Success = false;
        break;
      }
      Ptr += ActuallyRead;
    }
  
    if (Success) {
      Res.Ptr = cast(c8*, Buffer);
      Res.Len = TotalSize;
    }
    else
      EndPoolSnapshot(RestorePoint);
    CloseHandle(File);
  }

  EndPoolSnapshot(Snap);
  return Res;
}
function b32 SysSaveFile(str8 Data, str8 Path) {
  b32 Success = false;

  pool_snap Snap      = GetPoolSnapshot(GlobalWin32Pool);
  str16     PathUtf16 = ConvertStr8ToStr16(Snap.Pool, Path);

  HANDLE File = CreateFileW(cast(WCHAR*, PathUtf16.Ptr), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
  if (File != INVALID_HANDLE_VALUE) {
    byte *Ptr = Data.Ptr;
    byte *Opl = Ptr + Data.Len;
    while (Ptr < Opl) {
      u64 TotalToWrite = (u64)(Opl - Ptr);
      DWORD ToWrite = TotalToWrite;
      if (TotalToWrite > u32Max)
        ToWrite = u32Max;
      DWORD ActuallyWritten = 0;
      if (!WriteFile(File, Ptr, ToWrite, &ActuallyWritten, 0)) {
        Success = false;
        break;
      }
      Ptr += ActuallyWritten;
    }
  }

  CloseHandle(File);

  return Success;
}
function b32 SysRenameFile(str8 Old, str8 New) {
  pool_snap Snap     = GetPoolSnapshot(GlobalWin32Pool);
  str16     OldUtf16 = ConvertStr8ToStr16(Snap.Pool, Old);
  str16     NewUtf16 = ConvertStr8ToStr16(Snap.Pool, New);
  b32       Success  = MoveFileW(cast(WCHAR*, OldUtf16.Ptr), cast(WCHAR*, NewUtf16.Ptr));
  EndPoolSnapshot(Snap);
  return Success;
}
function b32 SysDeleteFile(str8 Path) {
  pool_snap Snap      = GetPoolSnapshot(GlobalWin32Pool);
  str16     PathUtf16 = ConvertStr8ToStr16(Snap.Pool, Path);
  b32       Success   = DeleteFileW(cast(WCHAR*, PathUtf16.Ptr));
  EndPoolSnapshot(Snap);
  return Success;
}
function b32 SysCreateDir(str8 Path) {
  pool_snap Snap      = GetPoolSnapshot(GlobalWin32Pool);
  str16     PathUtf16 = ConvertStr8ToStr16(Snap.Pool, Path);
  b32       Success   = CreateDirectoryW(cast(WCHAR*, PathUtf16.Ptr), 0);
  EndPoolSnapshot(Snap);
  return Success;
}
function b32 SysDeleteDir(str8 Path) {
  pool_snap Snap      = GetPoolSnapshot(GlobalWin32Pool);
  str16     PathUtf16 = ConvertStr8ToStr16(Snap.Pool, Path);
  b32       Success   = RemoveDirectoryW(cast(WCHAR*, PathUtf16.Ptr));
  EndPoolSnapshot(Snap);
  return Success;
}

function file_props SysGetFileProps(str8 Path) {
  file_props Res = {0};

  pool_snap Snap      = GetPoolSnapshot(GlobalWin32Pool);
  str16     PathUtf16 = ConvertStr8ToStr16(Snap.Pool, Path);

  WIN32_FILE_ATTRIBUTE_DATA Attributes = {0};
  if (GetFileAttributesExW(cast(WCHAR*, PathUtf16.Ptr), GetFileExInfoStandard, &Attributes)) {
    Res.Readonly = Attributes.dwFileAttributes & FILE_ATTRIBUTE_READONLY;
    Res.IsDir    = Attributes.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
    Res.Size     = ((u64)Attributes.nFileSizeHigh << 32) | (u64)Attributes.nFileSizeLow;;
    Res.Creation = _Win32ConvertFileTimeToDenseTime(&Attributes.ftCreationTime);
    Res.Modified = _Win32ConvertFileTimeToDenseTime(&Attributes.ftLastWriteTime);
  }

  EndPoolSnapshot(Snap);

  return Res;
}

struct _win32_file_iter {
  HANDLE           DirHandle;
  WIN32_FIND_DATAW FindData;
  b32              Finished;
};
function file_iter SysInitFileIter(str8 Path) {
  file_iter Res = {0};

  struct _win32_file_iter *Win32Iter = cast(struct _win32_file_iter*, &Res);
  pool_snap  Snap      = GetPoolSnapshot(GlobalWin32Pool);
  str16      PathUtf16 = ConvertStr8ToStr16(Snap.Pool, Path);
  Win32Iter->DirHandle = FindFirstFileW(cast(WCHAR*, PathUtf16.Ptr), &Win32Iter->FindData);

  EndPoolSnapshot(Snap);

  return Res;
}
function b32 SysNextFileIter(pool *Pool, file_iter *Iter, str8 *NameOut, file_props *PropsOut) {
  b32 Res = false;

  struct _win32_file_iter *Win32Iter = cast(struct _win32_file_iter*, &Iter);
  if (Win32Iter->DirHandle != 0 && Win32Iter->DirHandle != INVALID_HANDLE_VALUE) {
    while (!Win32Iter->Finished) {
      WCHAR *Filename = Win32Iter->FindData.cFileName;
      b32 ShouldEmit  = !(Filename[0] == '.' && Filename[1] == 0) && !(Filename[0] == '.' && Filename[1] == '.' && Filename[2] == 0);
      WIN32_FIND_DATAW Data = {0};
      if (ShouldEmit)
        memcpy(&Data, &Win32Iter->FindData, sizeof(WIN32_FIND_DATAW));
      if (!FindNextFileW(Win32Iter->DirHandle, &Win32Iter->FindData))
        Win32Iter->Finished = true;
      if (ShouldEmit) {
        *NameOut = ConvertStr16ToStr8(Pool, Str16Cstr(cast(c16*, Data.cFileName)));
        PropsOut->Readonly = Data.dwFileAttributes & FILE_ATTRIBUTE_READONLY;
        PropsOut->IsDir    = Data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
        PropsOut->Size     = ((u64)Data.nFileSizeHigh << 32) | (u64)Data.nFileSizeLow;;
        PropsOut->Creation = _Win32ConvertFileTimeToDenseTime(&Data.ftCreationTime);
        PropsOut->Modified = _Win32ConvertFileTimeToDenseTime(&Data.ftLastWriteTime);
        Res = true;
        break;
      }
    }
  }

  return Res;
}
function void SysKillFileIter(file_iter *Iter) {
  struct _win32_file_iter *Win32Iter = cast(struct _win32_file_iter*, &Iter);
  if (Win32Iter->DirHandle != 0 && Win32Iter->DirHandle != INVALID_HANDLE_VALUE)
    FindClose(Win32Iter->DirHandle);
}

function dyn_lib SysLoadLib(str8 Path) {
  dyn_lib   Res       = {0};
  pool_snap Snap      = GetPoolSnapshot(GlobalWin32Pool);
  str16     PathUtf16 = ConvertStr8ToStr16(Snap.Pool, Path);
  Res.Data = cast(u64, LoadLibraryW(cast(WCHAR*, PathUtf16.Ptr)));
  EndPoolSnapshot(Snap);
  return Res;
}
function void_proc *SysGetLibProc(dyn_lib Lib, c8 *Name) {
  HMODULE Module = cast(HMODULE, Lib.Data);
  void_proc *Res = cast(void_proc*, GetProcAddress(Module, Name));
  return Res;
}
function void SysReleaseLib(dyn_lib Lib) {
  HMODULE Module = cast(HMODULE, Lib.Data);
  FreeLibrary(Module);
}

////////////////////////
// WINDOW
struct _win32_wnd {
  void *MainFiber;
  void *MsgFiber;
  HINSTANCE Instance;
  HWND      Handle;
  HDC       DeviceCtx;
  wnd Wnd;
  b32 Used;
};

#if !defined(NO_GFX)
global struct _win32_wnd _Win32Wnd;
#endif

function LRESULT CALLBACK _Win32WindowProc(HWND Handle, UINT Msg, WPARAM wParam, LPARAM lParam) {
  LRESULT Result = 0;
  switch (Msg) {
    case WM_DESTROY:
      _Win32Wnd.Wnd.Finish = true;
      break;
    case WM_TIMER:
      SwitchToFiber(_Win32Wnd.MainFiber);
      break;
    case WM_ENTERMENULOOP:
    case WM_ENTERSIZEMOVE:
      SetTimer(Handle, 1, 1, 0);
      break;
    case WM_EXITMENULOOP:
    case WM_EXITSIZEMOVE:
      KillTimer(Handle, 1);
      break;
    default:
      Result = DefWindowProcW(Handle, Msg, wParam, lParam);
  }
  return Result;
}
function void CALLBACK _Win32MessageFiberProc(void *MainFiber) {
  while (true) {
    MSG Msg;
    while (PeekMessage(&Msg, 0, 0, 0, PM_REMOVE)) {
      TranslateMessage(&Msg);
      DispatchMessage(&Msg);
    }
    SwitchToFiber(MainFiber);
  }
}

function wnd *SysInitWnd(void) {
  if (_Win32Wnd.Used)
    return &_Win32Wnd.Wnd;

  _Win32Wnd.Instance  = GetModuleHandleW(null);
  _Win32Wnd.MainFiber = ConvertThreadToFiber(0);
  _Win32Wnd.MsgFiber  = CreateFiber(0, (PFIBER_START_ROUTINE)_Win32MessageFiberProc, _Win32Wnd.MainFiber);

  WNDCLASSW Class = {0};
  Class.lpfnWndProc   = _Win32WindowProc;
  Class.hInstance     = _Win32Wnd.Instance;
  Class.hCursor       = LoadCursor(0, IDC_ARROW);
  Class.lpszClassName = L"class";
  RegisterClassW(&Class);

  _Win32Wnd.Handle    = CreateWindowW(L"class", L"title", WS_TILEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, _Win32Wnd.Instance, 0);
  _Win32Wnd.DeviceCtx = GetDC(_Win32Wnd.Handle);
  _Win32Wnd.Used      = true;

  ShowWindow(_Win32Wnd.Handle, SW_SHOWDEFAULT);

  return &_Win32Wnd.Wnd;
}
function void SysKillWnd(wnd *Wnd) {
  ShowWindow(_Win32Wnd.Handle, SW_HIDE);
  ReleaseDC(_Win32Wnd.Handle, _Win32Wnd.DeviceCtx);
  _Win32Wnd.Used = false;
}
function void SysWndPull(wnd *Wnd) {
  SwitchToFiber(_Win32Wnd.MsgFiber);
  SwapBuffers(_Win32Wnd.DeviceCtx);
}
function void SysWndPush(wnd *Wnd) {

}

#elif defined(OS_LNX)

function void SysAbort(i32 Code);

////////////////////////
// MEMORY
function void *SysMemReserve(size Size, u32 Flags);
function void  SysMemRelease(void *Ptr, size Size);

////////////////////////
// TIME
function u64  SysGetMicroseconds(void); //.note: Does not return 'dense_time'!
function void SysSleep(u32 Milliseconds);

////////////////////////
// FILES
function file_props SysGetFileProps(str8 Path);
function str8 SysOpenFile  (pool *Pool,  str8 Path);
function b32  SysSaveFile  (str8  Data,  str8 Path);
function b32  SysRenameFile(str8  Old,   str8 New);
function b32  SysDeleteFile(str8  Path);
function b32  SysCreateDir (str8  Path);
function b32  SysDeleteDir (str8  Path);

#elif defined(OS_MAC)

function void SysAbort(i32 Code);

////////////////////////
// MEMORY
function void *SysMemReserve(size Size, u32 Flags);
function void  SysMemRelease(void *Ptr, size Size);

////////////////////////
// TIME
function u64  SysGetMicroseconds(void); //.note: Does not return 'dense_time'!
function void SysSleep(u32 Milliseconds);

////////////////////////
// FILES
function file_props SysGetFileProps(str8 Path);
function str8 SysOpenFile  (pool *Pool,  str8 Path);
function b32  SysSaveFile  (str8  Data,  str8 Path);
function b32  SysRenameFile(str8  Old,   str8 New);
function b32  SysDeleteFile(str8  Path);
function b32  SysCreateDir (str8  Path);
function b32  SysDeleteDir (str8  Path);

#endif

#endif//FOUNDATION_IMPL