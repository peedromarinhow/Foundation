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
#if !defined(FOUNDATION_HEADER)
#define FOUNDATION_HEADER

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
    #error "No operating system defined in this compiler."
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
    #error "No architecture defined in this compiler."
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
    #error "No operating system defined in this compiler."
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
    #error "No architecture defined in this compiler."
  #endif
#elif defined(_MSC_VER)
  #define COMPILER_CL 1
  #if defined(_WIN32)
    #define OS_WIN 1
  #else
    #error "No operating system defined in this compiler."
  #endif
  #if defined(_M_AMD64)
    #define ARCH_X64 1
  #elif defined(_M_I86)
    #define ARCH_X86 1
  #elif defined(_M_ARM)
    #define ARCH_ARM 1
  #else
    #error "No architecture defined in this compiler."
  #endif
#else
  #error "No context defined in this compiler."
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

#define AssertBreak() __debugbreak();
#if defined(ENABLE_ASSERT)
  #define Assert(Expr, ...) if (!(Expr)) AssertBreak();
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

#define R1AddComps(r1, r2) Todo()
#define R1SubComps(r1, r2) Todo()

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
// MEMORY
enum _mem_flags {
  mem_Unaccessible = Flag(0),
  mem_Readonly     = Flag(1),
  mem_Runnable     = Flag(2),
};

typedef struct _pool {
	size Cap;
	size Pos;
} pool;
function pool *PoolReserve  (size cap);
function void  PoolRelease  (pool *Pool);
function void *PoolPush     (pool *Pool, size Size);
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
#define  ArrayAdd(Array, x) (       \
  ((Array)->Cap < (Array)->Len + 1)? \
    _ArrayGrow(_ArrayExp(Array))     \
  :                                  \
    0,                               \
  (Array)->Mem[(Array)->Len] = (x),  \
  (Array)->Len++                     \
)

////////////////////////
// HASH TABLE
Todo();

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
// FILE PROPERTIES
typedef struct _file_properties {
  u32  Flags;
  b32  IsDir;
  size Size;
  dense_time Creation;
  dense_time Modified;
} file_properties;

////////////////////////
// SYSTEM INTERFACE
function b32  SysInit(i32 Argc, c8 **Argv);
function void SysEnd(void);
function void SysAbort(i32 Code);

function byte *SysMemReserve(size Size, u32 Flags);
function void  SysMemRelease(void *Ptr, size Size);

function u64  SysGetMicroseconds(void); //.note: Does not return 'dense_time'!
function void SysSleep(u32 Milliseconds);

function file_properties SysGetFileProps(str8 Path);
function str8 SysOpenFile  (pool *Pool,  str8 Path);
function b32  SysSaveFile  (str8  Data,  str8 Path);
function b32  SysRenameFile(str8  Old,   str8 New);
function b32  SysDeleteFile(str8  Path);
function b32  SysCreateDir (str8  Path);
function b32  SysDeleteDir (str8  Path);

////////////////////////
// OPENGL
#undef function
//.link: https://gist.github.com/mmozeiko/ed2ad27f75edf9c26053ce332a1f6647.
// Also download https://www.khronos.org/registry/EGL/api/KHR/khrplatform.h and put in "KHR" folder.
#include "download/glcorearb.h" //.link: https://www.khronos.org/registry/OpenGL/api/GL/glcorearb.h
#include "download/glext.h"     //.link: https://www.khronos.org/registry/OpenGL/api/GL/glext.h
#define function static

// These are the opengl functions to be loaded. All of them are supposed to be platform-agnostic.
#define SELECTED_OPENGL_FUNCTIONS(Macro)                           \
  Macro(PFNGLCLEARCOLORPROC,              ClearColor)              \
  Macro(PFNGLCLEARPROC,                   Clear)                   \
  Macro(PFNGLGENBUFFERSPROC,              GenBuffers)              \
  Macro(PFNGLBINDBUFFERPROC,              BindBuffer)              \
  Macro(PFNGLBUFFERDATAPROC,              BufferData)              \
  Macro(PFNGLCREATESHADERPROC,            CreateShader)            \
  Macro(PFNGLSHADERSOURCEPROC,            ShaderSource)            \
  Macro(PFNGLCOMPILESHADERPROC,           CompileShader)           \
  Macro(PFNGLGETSHADERIVPROC,             GetShaderiv)             \
  Macro(PFNGLGETSHADERINFOLOGPROC,        GetShaderInfoLog)        \
  Macro(PFNGLCREATEPROGRAMPROC,           CreateProgram)           \
  Macro(PFNGLATTACHSHADERPROC,            AttachShader)            \
  Macro(PFNGLLINKPROGRAMPROC,             LinkProgram)             \
  Macro(PFNGLGETPROGRAMIVPROC,            GetProgramiv)            \
  Macro(PFNGLGETPROGRAMINFOLOGPROC,       GetProgramInfoLog)       \
  Macro(PFNGLUSEPROGRAMPROC,              UseProgram)              \
  Macro(PFNGLGETUNIFORMLOCATIONPROC,      GetUniformLocation)      \
  Macro(PFNGLUNIFORM4FPROC,               Uniform4f)               \
  Macro(PFNGLDELETESHADERPROC,            DeleteShader)            \
  Macro(PFNGLVERTEXATTRIBPOINTERPROC,     VertexAttribPointer)     \
  Macro(PFNGLENABLEVERTEXATTRIBARRAYPROC, EnableVertexAttribArray) \
  Macro(PFNGLGENVERTEXARRAYSPROC,         GenVertexArrays)         \
  Macro(PFNGLBINDVERTEXARRAYPROC,         BindVertexArray)         \
  Macro(PFNGLDRAWELEMENTSPROC,            DrawElements)            \
  Macro(PFNGLDELETEVERTEXARRAYSPROC,      DeleteVertexArrays)      \
  Macro(PFNGLDELETEBUFFERSPROC,           DeleteBuffers)           \
  Macro(PFNGLDELETEPROGRAMPROC,           DeleteProgram)

typedef struct _opengl_api {
  #define Decl(type, Name) type Name;
    SELECTED_OPENGL_FUNCTIONS(Decl)
  #undef Decl
} opengl_api;

////////////////////////
// D3D11
typedef struct _d3d11_api {
  int Tmp;
} d3d11_api;

////////////////////////
// VULKAN
typedef struct _vulkan_api {
  int Tmp;
} vulkan_api;

////////////////////////
// WINDOW INTERFACE
#define WINDOW_BUTTON_COUNT 256
enum _buttons {
  button_None,

  button_Up,
  button_Down,
  button_Left,
  button_Right,

  button_F1,
  button_F2,
  button_F3,
  button_F4,
  button_F5,
  button_F6,
  button_F7,
  button_F8,
  button_F9,
  button_F10,
  button_F11,
  button_F12,
};
typedef struct _button {
  b8 Down     : 1;
  b8 Pressed  : 1;
  b8 Released : 1;
} button;
inline function void UpdateButton(button *Button, b32 IsDown);

typedef struct _sys_specific_window sys_specific_window;
typedef enum _gfx_api_kind {
  gfx_api_None,
  gfx_api_Opengl,
  gfx_api_D3d11,
  gfx_api_Vulkan,
} gfx_api_kind;
typedef struct _window {
  sys_specific_window *SysWnd;
  array(c8*)           Errors;
  b32                  Finish;
  void                *GfxApi;
  gfx_api_kind GfxApiKind;
  u64          FrameStart;
  r64          FrameDelta;
  r64          DesiredFps;
  b32          LostFrames;
  button       Buttons[WINDOW_BUTTON_COUNT];
} window;

//.note: For default width and height the default value is 0. For default x and y the default value is -1.
function window *WndInit(gfx_api_kind GfxApiKind, i32 w, i32 h, i32 x, i32 y);
function void    WndEnd(window *Window);

function void WndBeginFrame(window *Window);
function void WndEndFrame  (window *Window);

function void WndBeginRendering(window *Window);
function void WndEndRendering  (window *Window);


#endif//FOUNDATION_HEADER

/**************************************************************************************************
  PLTFORM INDEPENDENT IMPLEMENTATION
***************************************************************************************************/
#if defined(FOUNDATION_IMPLEMENTATION)

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

inline r32r1 R32r1Add(r32r1 r1, r32r1 r2) {
  r32r1 Res = {0};
  Todo();
  return Res;
}
inline u64r1 U64r1Add(u64r1 r1, u64r1 r2) {
  u64r1 Res = {0};
  Todo();
  return Res;
}

inline r32r1 R32r1Sub(r32r1 r1, r32r1 r2) {
  r32r1 Res = {0};
  Todo();
  return Res;
}
inline u64r1 U64r1Sub(u64r1 r1, u64r1 r2) {
  u64r1 Res = {0};
  Todo();
  return Res;
}

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

inline i32r2 I32r2Add(i32r2 r1, i32r2 r2) {
  i32r2 Res = {0};
  Todo();
  return Res;
}
inline r32r2 R32r2Add(r32r2 r1, r32r2 r2) {
  r32r2 Res = {0};
  Todo();
  return Res;
}

inline i32r2 I32r2Sub(i32r2 r1, i32r2 r2) {
  i32r2 Res = {0};
  Todo();
  return Res;
}
inline r32r2 R32r2Sub(r32r2 r1, r32r2 r2) {
  r32r2 Res = {0};
  Todo();
  return Res;
}

inline b32 I32r2Overlaps(i32r2 r1, i32r2 r2) { return R2Overlap(r1, r2); }
inline b32 R32r2Overlaps(r32r2 r1, r32r2 r2) { return R2Overlap(r1, r2); }

inline b32 I32r2Contains(i32r2 r, i32v2 v) { return R2Contains(r, v); }
inline b32 R32r2Contains(r32r2 r, r32v2 v) { return R2Contains(r, v); }

inline i32v2 I32r2Len(i32r2 r) { return I32v2(R2LenComps(r)); }
inline r32v2 R32r2Len(r32r2 r) { return R32v2(R2LenComps(r)); }

inline i32v2 I32r2Mid(i32r2 r) { return I32v2(R2MidComps(r)); }
inline r32v2 R32r2Mid(r32r2 r) { return R32v2(R2MidComps(r)); }

////////////////////////
// MEMORY
function pool *PoolReserve(size Cap) {
  if (Cap == 0)
    Cap = Gb(1);
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
// DYNAMIC ARRAYARRAY
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
Todo();

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
  Todo();
  return Res;
}
function str8 ConvertStr32ToStr8(pool *Pool, str32 Str) {
  str8 Res = {0};
  Todo();
  return Res;
}
function str16 ConvertStr8ToStr16(pool *Pool, str8 Str) {
  str16 Res = {0};

  c16 *Mem = PoolPush(Pool, sizeof(c16)*Str.Len*2 + 1);
  c16 *Dst = Mem;
  c8  *Ptr = Str.Ptr;
  c8  *End = Str.Ptr + Str.Len;
  while (Ptr < End){
    utf_char Decoded = DecodeUtf8(Ptr, cast(u32, End - Ptr));
    u32      EncSize = EncodeUtf16(Dst, Decoded.Code);
    Ptr += Decoded.Size;
    Dst += EncSize;
  }
 *Dst = 0;
  size Len = (size)(Dst - Mem);
  PoolPopAmount(Pool, (Str.Len*2 - Len)*sizeof(*Mem)); // Pop unused amount.
  Res.Len = Len;
  Res.Ptr = Mem;
	return Res;
}
function str8 ConvertStr16ToStr8(pool *Pool, str16 Str) {
  str8 Res = {0};
  Todo();
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

////////////////////////
// SYSTEM WINDOW
inline function void UpdateButton(button *Button, b32 IsDown) {
  Button->Released =  Button->Down && !IsDown;
  Button->Pressed  = !Button->Down &&  IsDown;
  Button->Down     =  IsDown;
}

/**************************************************************************************************
  PLATFORM DEPENDENT IMPLEMENTATIONS
***************************************************************************************************/

#if defined(OS_WIN)

#undef function
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include "timeapi.h"
#define function static

global pool *GlobalWin32Pool;
global str8 *GlobalWin32Argv;
global i32   GlobalWin32Argc;
global u64   GlobalWin32TicksPerSecond;
global u64   GlobalWin32TicksUponStart;

////////////////////////
// SETUP
function b32 SysInit(i32 Argc, c8 **Argv) {
  pool *Pool = PoolReserve(0);

  LARGE_INTEGER PerfFrequency = {0};
  if (!QueryPerformanceFrequency(&PerfFrequency)) {
    fprintf(stderr, "Could not retrieve performance frequency.");
    return false;
  }
  timeBeginPeriod(1);
  LARGE_INTEGER PerfCounter = {0};
  if (!QueryPerformanceCounter(&PerfCounter)) {
    fprintf(stderr, "Could not retrieve performance counter.");
    return false;
  }
  
  GlobalWin32Argc           = Argc;
  GlobalWin32Argv           = cast(str8*, PoolPush(Pool, Argc*sizeof(str8)));
  ItrNum (i, Argc)
    GlobalWin32Argv[i]      = Str8Cstr(Argv[i]);
  GlobalWin32TicksPerSecond = PerfFrequency.QuadPart;
  GlobalWin32TicksUponStart = PerfCounter.QuadPart;
  GlobalWin32Pool           = Pool;

  return true;
}
function void SysEnd(void) {
  PoolRelease(GlobalWin32Pool);
}
function void SysAbort(i32 Code) {
  ExitProcess(Code);
}

////////////////////////
// MEMORY
function byte *SysMemReserve(size Size, u32 Flags) {
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
function file_properties SysGetFileProps(str8 Path) {
  file_properties Res = {0};

  pool_snap Snap      = GetPoolSnapshot(GlobalWin32Pool);
  str16     PathUtf16 = ConvertStr8ToStr16(Snap.Pool, Path);

  WIN32_FILE_ATTRIBUTE_DATA Attributes = {0};
  if (GetFileAttributesExW(cast(WCHAR*, PathUtf16.Ptr), GetFileExInfoStandard, &Attributes)) {
    Todo();
  }
  return Res;
}

function str8 SysOpenFile(pool *Pool, str8 Path) {
  str8 Res = {};

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
    byte *OnePastLast = Buffer + TotalSize;
    b32   Success     = true;
    while (Ptr < OnePastLast) {
      DWORD ToRead = cast(DWORD, OnePastLast - Ptr);
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
  Todo();
  return false;
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

////////////////////////
// SYSTEM WINDOW
struct _sys_specific_window {
  void *MainFiber;
  void *MessageFiber;

  HINSTANCE Instance;
  HWND      WindowHandle;
  HDC       DeviceContext;

  WINDOWPLACEMENT WindowPosition;
  MONITORINFO     MonitorInfo;

  void *GfxCtx;
};

////////////////////////
// OPENGL
typedef HGLRC wgl_create_context_proc  (HDC);
typedef BOOL  wgl_delete_context_proc  (HGLRC);
typedef BOOL  wgl_make_current_proc    (HDC, HGLRC);
typedef PROC  wgl_get_proc_address_proc(LPCSTR);

typedef BOOL  wgl_choose_pixel_format_arb_proc   (HDC, const int*, const float*, UINT, int*, UINT*);
typedef HGLRC wgl_create_context_attribs_arb_proc(HDC, HGLRC, const int*);

#define WGL_NUMBER_PIXEL_FORMATS_ARB    0x2000
#define WGL_DRAW_TO_WINDOW_ARB          0x2001
#define WGL_DRAW_TO_BITMAP_ARB          0x2002
#define WGL_ACCELERATION_ARB            0x2003
#define WGL_NEED_PALETTE_ARB            0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB     0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB      0x2006
#define WGL_SWAP_METHOD_ARB             0x2007
#define WGL_NUMBER_OVERLAYS_ARB         0x2008
#define WGL_NUMBER_UNDERLAYS_ARB        0x2009
#define WGL_TRANSPARENT_ARB             0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB   0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB 0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB  0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB 0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB 0x203B
#define WGL_SHARE_DEPTH_ARB             0x200C
#define WGL_SHARE_STENCIL_ARB           0x200D
#define WGL_SHARE_ACCUM_ARB             0x200E
#define WGL_SUPPORT_GDI_ARB             0x200F
#define WGL_SUPPORT_OPENGL_ARB          0x2010
#define WGL_DOUBLE_BUFFER_ARB           0x2011
#define WGL_STEREO_ARB                  0x2012
#define WGL_PIXEL_TYPE_ARB              0x2013
#define WGL_COLOR_BITS_ARB              0x2014
#define WGL_RED_BITS_ARB                0x2015
#define WGL_RED_SHIFT_ARB               0x2016
#define WGL_GREEN_BITS_ARB              0x2017
#define WGL_GREEN_SHIFT_ARB             0x2018
#define WGL_BLUE_BITS_ARB               0x2019
#define WGL_BLUE_SHIFT_ARB              0x201A
#define WGL_ALPHA_BITS_ARB              0x201B
#define WGL_ALPHA_SHIFT_ARB             0x201C
#define WGL_ACCUM_BITS_ARB              0x201D
#define WGL_ACCUM_RED_BITS_ARB          0x201E
#define WGL_ACCUM_GREEN_BITS_ARB        0x201F
#define WGL_ACCUM_BLUE_BITS_ARB         0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB        0x2021
#define WGL_DEPTH_BITS_ARB              0x2022
#define WGL_STENCIL_BITS_ARB            0x2023
#define WGL_AUX_BUFFERS_ARB             0x2024
#define WGL_NO_ACCELERATION_ARB         0x2025
#define WGL_GENERIC_ACCELERATION_ARB    0x2026
#define WGL_FULL_ACCELERATION_ARB       0x2027
#define WGL_SWAP_EXCHANGE_ARB           0x2028
#define WGL_SWAP_COPY_ARB               0x2029
#define WGL_SWAP_UNDEFINED_ARB          0x202A
#define WGL_TYPE_RGBA_ARB               0x202B
#define WGL_TYPE_COLORINDEX_ARB         0x202C

#define WGL_CONTEXT_DEBUG_BIT_ARB              0x00000001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x00000002
#define WGL_CONTEXT_MAJOR_VERSION_ARB          0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB          0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB            0x2093
#define WGL_CONTEXT_FLAGS_ARB                  0x2094

#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

typedef struct _opengl_ctx {
  HGLRC                    Hglrc;
  wgl_make_current_proc   *MakeCurrent;
  wgl_delete_context_proc *DeleteContext;
} opengl_ctx;

//.link: https://www.khronos.org/opengl/wiki/Load_OpenGL_Functions
function void *_Win32OpenGLGetAnyFuncAddress(wgl_get_proc_address_proc *WglGetProcAddressProc, HMODULE OpenGLModule, const c8 *Name) {
  void *Proc = (void *)WglGetProcAddressProc(Name);
  if (Proc == 0 || (Proc == (void*)0x1) || (Proc == (void*)0x2) || (Proc == (void*)0x3) || (Proc == (void*)-1))
    Proc = (void *)GetProcAddress(OpenGLModule, Name);
  return Proc;
}

function void _Win32OpenGLWndInit(window *Window) {
  sys_specific_window *SysWnd = Window->SysWnd;

  // Load opengl32.dll and the functions we need.
  HMODULE OpenGLModule = null;
  if (!Window->Errors.Len)
    OpenGLModule = LoadLibraryA("opengl32.dll");
  if (OpenGLModule == null)
    ArrayAdd(&Window->Errors, "(opengl startup) could not load \"opengl32.dll\".");
  wgl_create_context_proc   *WglCreateContextProc  = null;
  wgl_delete_context_proc   *WglDeleteContextProc  = null;
  wgl_make_current_proc     *WglMakeCurrentProc    = null;
  wgl_get_proc_address_proc *WglGetProcAddressProc = null;
  if (!Window->Errors.Len) {
    WglCreateContextProc  = cast(wgl_create_context_proc*,   GetProcAddress(OpenGLModule, "wglCreateContext"));
    WglDeleteContextProc  = cast(wgl_delete_context_proc*,   GetProcAddress(OpenGLModule, "wglDeleteContext"));
    WglMakeCurrentProc    = cast(wgl_make_current_proc*,     GetProcAddress(OpenGLModule, "wglMakeCurrent"));
    WglGetProcAddressProc = cast(wgl_get_proc_address_proc*, GetProcAddress(OpenGLModule, "wglGetProcAddress"));
  }
  if (!WglCreateContextProc || !WglDeleteContextProc || !WglMakeCurrentProc || !WglGetProcAddressProc)
    ArrayAdd(&Window->Errors, "(opengl startup) could not load opengl module functions");

  // Create dummy window class.
  WNDCLASSW DummyClass = {0};
  if (!Window->Errors.Len) {
    DummyClass.lpfnWndProc   = DefWindowProcW;
    DummyClass.hInstance     = SysWnd->Instance;
    DummyClass.lpszClassName = L"dummy class";
  }
  if (!RegisterClassW(&DummyClass))
    ArrayAdd(&Window->Errors, "(opengl startup) could not create dummy window class");

  // Create dummy window.
  HWND DummyWindowHandle = null;
  if (!Window->Errors.Len)
    DummyWindowHandle = CreateWindowW(L"dummy class", L"dummy title", WS_TILEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, SysWnd->Instance, 0);
  if (!DummyWindowHandle)
    ArrayAdd(&Window->Errors, "(opengl startup) could not create dummy window for loading opengl");

  // Get dummy device context.
  HDC DummyDeviceContext = null;
  if (!Window->Errors.Len)
    DummyDeviceContext = GetDC(DummyWindowHandle);
  if (!DummyDeviceContext)
    ArrayAdd(&Window->Errors, "(opengl startup) could not create dummy device context for loading opengl");
  
  // Create dummy pixel format descriptor.
  PIXELFORMATDESCRIPTOR DummyPixelFormatDescriptor = {0};
  if (!Window->Errors.Len) {
    DummyPixelFormatDescriptor.nSize      = sizeof(DummyPixelFormatDescriptor);
    DummyPixelFormatDescriptor.nVersion   = 1;
    DummyPixelFormatDescriptor.dwFlags    = PFD_SUPPORT_OPENGL;
    DummyPixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
    DummyPixelFormatDescriptor.cColorBits = 24;
  }
  INT DummyPixelFormatIdx = ChoosePixelFormat(DummyDeviceContext, &DummyPixelFormatDescriptor);
  if (!DummyPixelFormatIdx)
    ArrayAdd(&Window->Errors, "(opengl startup) could not choose dummy pixel format");
  if (!Window->Errors.Len) {
    if (!SetPixelFormat(DummyDeviceContext, DummyPixelFormatIdx, &DummyPixelFormatDescriptor))
      ArrayAdd(&Window->Errors, "(opengl startup) could not set dummy pixel format");
  }

  // Get dummy opengl context.
  HGLRC DummyOpenglContext = null;
  if (!Window->Errors.Len)
    DummyOpenglContext = WglCreateContextProc(DummyDeviceContext);
  if (!DummyOpenglContext)
    ArrayAdd(&Window->Errors, "(opengl startup) could not create dummy opengl context");
  if (!Window->Errors.Len) {
    if (!WglMakeCurrentProc(DummyDeviceContext, DummyOpenglContext))
      ArrayAdd(&Window->Errors, "(opengl startup) calling 'WglMakeCurrent' for dummy context failed.");
  }
  
  // Load opengl functions from dummy context.
  wgl_choose_pixel_format_arb_proc    *WglChoosePixelFormatARBProc    = null;
  wgl_create_context_attribs_arb_proc *WglCreateContextAttribsARBProc = null;
  if (!Window->Errors.Len) {
    WglChoosePixelFormatARBProc    = cast(wgl_choose_pixel_format_arb_proc*,    WglGetProcAddressProc("wglChoosePixelFormatARB"));
    WglCreateContextAttribsARBProc = cast(wgl_create_context_attribs_arb_proc*, WglGetProcAddressProc("wglCreateContextAttribsARB"));
  }
  if (!WglChoosePixelFormatARBProc || !WglCreateContextAttribsARBProc)
    ArrayAdd(&Window->Errors, "(opengl startup) could not load opengl functions");

  if (!Window->Errors.Len) {
    if (!WglMakeCurrentProc(null, null))
      ArrayAdd(&Window->Errors, "(opengl startup) cleanup call to 'WglMakeCurrent' failed.");
  }

  // Cleanup dummy stuff.
  ReleaseDC(DummyWindowHandle, DummyDeviceContext);
  if (!Window->Errors.Len) {
    if (!WglDeleteContextProc(DummyOpenglContext))
      ArrayAdd(&Window->Errors, "(opengl startup) could not delete dummy opengl context");
    if (!DestroyWindow(DummyWindowHandle))
      ArrayAdd(&Window->Errors, "(opengl startup) could not destroy dummy window");
  }

  // Create pixel format.
  INT FormatAttribs[] = {
    WGL_DRAW_TO_WINDOW_ARB, true,
    WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
    WGL_SWAP_METHOD_ARB,    WGL_SWAP_EXCHANGE_ARB,
    WGL_SUPPORT_OPENGL_ARB, true,
    WGL_DOUBLE_BUFFER_ARB,  true,
    WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
    WGL_COLOR_BITS_ARB,     24,
    WGL_RED_BITS_ARB,       8,
    WGL_GREEN_BITS_ARB,     8,
    WGL_BLUE_BITS_ARB,      8,
    0,
  };
  INT  PixelFormatIdx  = 0;
  UINT NumberOfFormats = 0;
  if (!Window->Errors.Len) {
    if(!WglChoosePixelFormatARBProc(SysWnd->DeviceContext, FormatAttribs, null, 1, &PixelFormatIdx, &NumberOfFormats) || NumberOfFormats == 0)
      ArrayAdd(&Window->Errors, "(opengl startup) could not choose pixel format");
  }

  // Set pixel format.
  PIXELFORMATDESCRIPTOR PixelFormatDescriptor = {0};
  if (!Window->Errors.Len) {
    if (!SetPixelFormat(SysWnd->DeviceContext, PixelFormatIdx, &PixelFormatDescriptor))
      ArrayAdd(&Window->Errors, "(opengl startup) could not set pixel format");
  }

  // Create opengl context.
  opengl_ctx *Ctx = PoolPush(GlobalWin32Pool, sizeof(opengl_ctx));
  INT ContextAttribs[] = {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
    WGL_CONTEXT_MINOR_VERSION_ARB, 3,
    WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
    WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
    0
  };
  if (!Window->Errors.Len)
    Ctx->Hglrc = WglCreateContextAttribsARBProc(SysWnd->DeviceContext, 0, ContextAttribs);
  if (!Ctx->Hglrc)
    ArrayAdd(&Window->Errors, "(opengl startup) could not create opengl context");
  if (!Window->Errors.Len) {
    Ctx->MakeCurrent   = WglMakeCurrentProc;
    Ctx->DeleteContext = WglDeleteContextProc;
  }
  if (!Ctx->MakeCurrent(SysWnd->DeviceContext, Ctx->Hglrc))
    ArrayAdd(&Window->Errors, "(opengl) could not make current context");

  // Create opengl api.
  opengl_api *Api = PoolPush(GlobalWin32Pool, sizeof(opengl_api));
  #define Assign(type, Name)                                                                                         \
    Api->Name = cast(type, _Win32OpenGLGetAnyFuncAddress(WglGetProcAddressProc, OpenGLModule, Stringify(gl##Name))); \
    if (!Api->Name)                                                                                                  \
      ArrayAdd(&Window->Errors, "(opengl startup) could not load \"" Stringify(gl##Name) "\"");
    SELECTED_OPENGL_FUNCTIONS(Assign)
  #undef Assign

  if (!Window->Errors.Len) {
    SysWnd->GfxCtx = Ctx;
    Window->GfxApi = Api;
  }
}
function void _Win32OpenGLWndEnd(window *Window) {
  sys_specific_window *SysWnd = Window->SysWnd;
  opengl_ctx *Ctx = cast(opengl_ctx*, SysWnd->GfxCtx);
  if (!Window->Errors.Len)
    Ctx->DeleteContext(Ctx->Hglrc);
}

function void _Win32OpenGLBeginFrame(window *Window) {
  if (Window->Finish)
    return;

  sys_specific_window *SysWnd = Window->SysWnd;
  opengl_ctx          *Ctx    = cast(opengl_ctx*, SysWnd->GfxCtx);
  SysWnd->DeviceContext = GetDC(SysWnd->WindowHandle);
  if (!Ctx->MakeCurrent( SysWnd->DeviceContext, Ctx->Hglrc))
    ArrayAdd(&Window->Errors, "(opengl) could not make current context");
}
function void _Win32OpenGLEndFrame(window *Window) {
  if (Window->Finish)
    return;

  sys_specific_window *SysWnd = Window->SysWnd;
  if (!SwapBuffers(SysWnd->DeviceContext))
    ArrayAdd(&Window->Errors, "(opengl) could not swap buffers");
  if (Window->Errors.Len)
    Window->Finish = true;
  ReleaseDC(SysWnd->WindowHandle, SysWnd->DeviceContext);
}

////////////////////////
// D3D11
typedef struct _d3d11_ctx {
  int Tmp;
} d3d11_ctx;

function void _Win32D3d11WndInit(window *Window) {
  sys_specific_window *SysWnd = Window->SysWnd;

  // Load d3d11.dll and the functions we need.
  HMODULE D3d11Module = null;
  if (!Window->Errors.Len)
    D3d11Module = LoadLibraryA("d3d11.dll");
  if (D3d11Module == null)
    ArrayAdd(&Window->Errors, "(d3d11 startup) could not load \"d3d11.dll\".");
  if (!Window->Errors.Len) {
  }

  Todo();
}
function void _Win32D3d11WndEnd(window *Window) {
  sys_specific_window *SysWnd = Window->SysWnd;
  Todo();
}

function void _Win32D3d11BeginFrame(window *Window) {
  sys_specific_window *SysWnd = Window->SysWnd;
  Todo();
}
function void _Win32D3d11EndFrame(window *Window) {
  sys_specific_window *SysWnd = Window->SysWnd;
  Todo();
}

////////////////////////
// VULKAN
#undef function
// #define VK_USE_PLATFORM_WIN32_KHR
// #include "vulkan/vulkan.h"
#define function static

typedef struct _vulkan_ctx {
  int Tmp;
} vulkan_ctx;

function void _Win32VulkanWndInit(window *Window) {
  sys_specific_window *SysWnd = Window->SysWnd;
  Todo();
}
function void _Win32VulkanWndEnd(window *Window) {
  sys_specific_window *SysWnd = Window->SysWnd;
  Todo();
}

function void _Win32VulkanBeginFrame(window *Window) {
  sys_specific_window *SysWnd = Window->SysWnd;
  Todo();
}
function void _Win32VulkanEndFrame(window *Window) {
  sys_specific_window *SysWnd = Window->SysWnd;
  Todo();
}

////////////////////////
// SYSTEM WINDOW WRAPPING
function LRESULT CALLBACK _Win32WindowProc(HWND WindowHandle, UINT Message, WPARAM wParam, LPARAM lParam) {
  LRESULT Result = 0;
  window *Window = cast(window*, GetWindowLongPtr(WindowHandle, GWLP_USERDATA));
  switch (Message) {
    case WM_DESTROY:
      Window->Finish = true;
      break;
    case WM_TIMER:
      SwitchToFiber(Window->SysWnd->MainFiber);
      break;
    case WM_ENTERMENULOOP:
    case WM_ENTERSIZEMOVE:
      SetTimer(WindowHandle, 1, 1, 0);
      break;
    case WM_EXITMENULOOP:
    case WM_EXITSIZEMOVE:
      KillTimer(WindowHandle, 1);
      break;
    default:
      Result = DefWindowProcW(WindowHandle, Message, wParam, lParam);
  }
  return Result;
}
function void CALLBACK _Win32MessageFiberProc(void *MainFiber) {
  while (true) {
    MSG Message;
    while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE)) {
      TranslateMessage(&Message);
      DispatchMessage(&Message);
    }
    SwitchToFiber(MainFiber);
  }
}

function window *WndInit(gfx_api_kind GfxApiKind, i32 w, i32 h, i32 x, i32 y) {
  window              *Window = PoolPush(GlobalWin32Pool, sizeof(window));
  sys_specific_window *SysWnd = PoolPush(GlobalWin32Pool, sizeof(sys_specific_window));
  ZeroMemory(Window, sizeof(window));
  ZeroMemory(SysWnd, sizeof(sys_specific_window));
  
  Window->SysWnd     = SysWnd;
  Window->GfxApiKind = GfxApiKind;

  SysWnd->Instance     = GetModuleHandleW(null);
  SysWnd->MainFiber    = ConvertThreadToFiber(0);
  SysWnd->MessageFiber = CreateFiber(0, (PFIBER_START_ROUTINE)_Win32MessageFiberProc, SysWnd->MainFiber);
  if (!SysWnd->MainFiber || !SysWnd->MessageFiber)
    ArrayAdd(&Window->Errors, "could not create fibers");

  // Fullscreen support.
  SysWnd->WindowPosition.length = sizeof(SysWnd->WindowPosition);
  SysWnd->MonitorInfo.cbSize    = sizeof(SysWnd->MonitorInfo);

  // Create window class.
  WNDCLASSW Class = {0};
  if (!Window->Errors.Len) {
    Class.lpfnWndProc   = _Win32WindowProc;
    Class.hInstance     = SysWnd->Instance;
    Class.hCursor       = LoadCursor(0, IDC_ARROW);
    Class.lpszClassName = L"class";
  }
  if (!RegisterClassW(&Class))
    ArrayAdd(&Window->Errors, "could not create window class");

  // Create window.
  i32 WndX = (x==-1)? CW_USEDEFAULT : x;
  i32 WndY = (y==-1)? CW_USEDEFAULT : y;
  i32 WndW = (w==0)?  CW_USEDEFAULT : w;
  i32 WndH = (h==0)?  CW_USEDEFAULT : h;
  if (w != 0 && h != 0) {
    RECT WindowRect = {.right = WndW, .bottom = WndW};
    if (AdjustWindowRect(&WindowRect, WS_OVERLAPPEDWINDOW, 0)) {
      WndW = WindowRect.right  - WindowRect.left;
      WndH = WindowRect.bottom - WindowRect.top;
    }
  }
  if (!Window->Errors.Len)
    SysWnd->WindowHandle = CreateWindowW(L"class", L"title", WS_TILEDWINDOW, WndX, WndY, WndW, WndH, 0, 0, SysWnd->Instance, 0);
  if (!SysWnd->WindowHandle)
    ArrayAdd(&Window->Errors, "could not create window");
  if (!Window->Errors.Len)
    SetWindowLongPtr(SysWnd->WindowHandle, GWLP_USERDATA, (LONG_PTR)Window);

  // Get device context.
  if (!Window->Errors.Len)
    SysWnd->DeviceContext = GetDC(SysWnd->WindowHandle);
  if (!SysWnd->DeviceContext)
    ArrayAdd(&Window->Errors, "could not create dummy device context for loading opengl");

  if (Window->GfxApiKind == gfx_api_None)
    Todo();
  else
  if (Window->GfxApiKind == gfx_api_Opengl)
    _Win32OpenGLWndInit(Window);
  else
  if (Window->GfxApiKind == gfx_api_D3d11)
    _Win32D3d11WndInit(Window);
  else
  if (Window->GfxApiKind == gfx_api_Vulkan)
    _Win32VulkanWndInit(Window);

  // Show window.
  if (!Window->Errors.Len)
    ShowWindow(SysWnd->WindowHandle, SW_SHOWDEFAULT);
  
  if (Window->Errors.Len)
    Window->Finish = true;
  return Window;
}
function void WndEnd(window *Window) {
  sys_specific_window *SysWnd = Window->SysWnd;

  if (Window->GfxApiKind == gfx_api_None)
    Todo();
  else
  if (Window->GfxApiKind == gfx_api_Opengl)
    _Win32OpenGLWndEnd(Window);
  else
  if (Window->GfxApiKind == gfx_api_D3d11)
    _Win32D3d11WndEnd(Window);
  else
  if (Window->GfxApiKind == gfx_api_Vulkan)
    _Win32VulkanWndEnd(Window);
  
  ReleaseDC(SysWnd->WindowHandle, SysWnd->DeviceContext);

  if (Window->Errors.Len) {
    fprintf(stderr, "There were %d errors:\n", cast(int, Window->Errors.Len));
    ItrNum (i, Window->Errors.Len)
      fprintf(stderr, "  Error: %s.\n", Window->Errors.Mem[i]);
  }
  else
    fprintf(stdout, "The program ended without any errors.\n");
  
  ArrayFree(&Window->Errors);
}

global u8 GlobalWin32ButtonTable[WINDOW_BUTTON_COUNT] = {
  [VK_UP]    = button_Up,
  [VK_DOWN]  = button_Down,
  [VK_LEFT]  = button_Left,
  [VK_RIGHT] = button_Right,

  [VK_F1]  = button_F1,
  [VK_F2]  = button_F2,
  [VK_F3]  = button_F3,
  [VK_F4]  = button_F4,
  [VK_F5]  = button_F5,
  [VK_F6]  = button_F6,
  [VK_F7]  = button_F7,
  [VK_F8]  = button_F8,
  [VK_F9]  = button_F9,
  [VK_F10] = button_F10,
  [VK_F11] = button_F11,
  [VK_F12] = button_F12,
};
function void WndBeginFrame(window *Window) {
  if (Window->Errors.Len)
    return;

  Window->FrameStart = SysGetMicroseconds();
  Window->LostFrames = false;
  
  sys_specific_window *SysWnd = Window->SysWnd;

  SwitchToFiber(SysWnd->MessageFiber);

  // Update kyboard.
  BYTE KeyboardState[WINDOW_BUTTON_COUNT];
  GetKeyboardState(KeyboardState);
  ItrNum (Key, WINDOW_BUTTON_COUNT)
    UpdateButton(&Window->Buttons[GlobalWin32ButtonTable[Key]], KeyboardState[Key] >> 7);

  // Fullscreen support.
  //.copy: From https://devblogs.microsoft.com/oldnewthing/20100412-00/?p=14353.
  if (Window->Buttons[button_F11].Pressed) {
    DWORD Style = GetWindowLong(SysWnd->WindowHandle, GWL_STYLE);
    if (Style & WS_OVERLAPPEDWINDOW && GetWindowPlacement(SysWnd->WindowHandle, &SysWnd->WindowPosition) && GetMonitorInfo(MonitorFromWindow(SysWnd->WindowHandle, MONITOR_DEFAULTTOPRIMARY), &SysWnd->MonitorInfo)) {
      SetWindowLong(SysWnd->WindowHandle, GWL_STYLE, Style & ~WS_OVERLAPPEDWINDOW);
      SetWindowPos(
        SysWnd->WindowHandle, HWND_TOP,
        SysWnd->MonitorInfo.rcMonitor.left,    SysWnd->MonitorInfo.rcMonitor.top,
        SysWnd->MonitorInfo.rcMonitor.right  - SysWnd->MonitorInfo.rcMonitor.left,
        SysWnd->MonitorInfo.rcMonitor.bottom - SysWnd->MonitorInfo.rcMonitor.top,
        SWP_NOOWNERZORDER | SWP_FRAMECHANGED
      );
    }
    else {
      SetWindowLong(SysWnd->WindowHandle, GWL_STYLE, Style | WS_OVERLAPPEDWINDOW);
      SetWindowPlacement(SysWnd->WindowHandle, &SysWnd->WindowPosition);
      SetWindowPos(
        SysWnd->WindowHandle, null, 0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED
      );
    }
  }
}
function void WndEndFrame(window *Window) {
  // Frame timing and frame-rate capping.
  Window->FrameDelta = (cast(r64, SysGetMicroseconds()) - cast(r64, Window->FrameStart))/cast(r64, Million(1));
  r32 TimeToSleep = (1.0f/Window->DesiredFps - Window->FrameDelta);
  if (TimeToSleep > 0) 
    Sleep(TimeToSleep*Thousand(1));
  else
    Window->LostFrames = true;
}

function void WndBeginRendering(window *Window) {
  if (Window->Errors.Len)
    return;

  if (Window->GfxApiKind == gfx_api_None)
    Todo();
  else
  if (Window->GfxApiKind == gfx_api_Opengl)
    _Win32OpenGLBeginFrame(Window);
  else
  if (Window->GfxApiKind == gfx_api_D3d11)
    _Win32D3d11BeginFrame(Window);
  else
  if (Window->GfxApiKind == gfx_api_Vulkan)
    _Win32VulkanBeginFrame(Window);
}
function void WndEndRendering(window *Window) {
  if (Window->Errors.Len)
    return;

  if (Window->GfxApiKind == gfx_api_None)
    Todo();
  else
  if (Window->GfxApiKind == gfx_api_Opengl)
    _Win32OpenGLEndFrame(Window);
  else
  if (Window->GfxApiKind == gfx_api_D3d11)
    _Win32D3d11EndFrame(Window);
  else
  if (Window->GfxApiKind == gfx_api_Vulkan)
    _Win32VulkanEndFrame(Window);
}

#elif defined(OS_LNX)

////////////////////////
// SETUP
function b32  SysInit(i32 Argc, c8 **Argv);
function void SysEnd(void);
function void SysAbort(i32 Code);

////////////////////////
// MEMORY
function byte *SysMemReserve(size Size, u32 Flags);
function void  SysMemRelease(void *Ptr, size Size);

////////////////////////
// TIME
function u64  SysGetMicroseconds(void); //.note: Does not return 'dense_time'!
function void SysSleep(u32 Milliseconds);

////////////////////////
// FILES
function file_properties SysGetFileProps(str8 Path);
function str8 SysOpenFile  (pool *Pool,  str8 Path);
function b32  SysSaveFile  (str8  Data,  str8 Path);
function b32  SysRenameFile(str8  Old,   str8 New);
function b32  SysDeleteFile(str8  Path);
function b32  SysCreateDir (str8  Path);
function b32  SysDeleteDir (str8  Path);

////////////////////////
// SYSTEM WINDOW
struct _sys_specific_window {
  void *GfxCtx;
};

////////////////////////
// SYSTEM WINDOW WRAPPING
function window *WndInit(gfx_api_kind GfxApiKind, i32 w, i32 h, i32 x, i32 y);
function void    WndEnd(window *Window);

function void WndBeginRendering(window *Window);
function void WndEndRendering  (window *Window);

#elif defined(OS_MAC)

Todo();

#endif

#endif//FOUNDATION_IMPLEMENTATION