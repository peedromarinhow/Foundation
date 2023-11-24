#define  FOUNDATION_IMPL
#include "foundation.h"

#include "glcorearb.h"
#include "glext.h"
#include "wglext.h"
#define SELECTED_OPENGL_FUNCTIONS(Macro)                              \
  Macro(PFNGLENABLEPROC,                   glEnable)                  \
  Macro(PFNGLVIEWPORTPROC,                 glViewport)                \
  Macro(PFNGLCLEARCOLORPROC,               glClearColor)              \
  Macro(PFNGLCLEARPROC,                    glClear)                   \
  Macro(PFNGLBLENDFUNCPROC,                glBlendFunc)               \
  Macro(PFNGLGENTEXTURESPROC,              glGenTextures)             \
  Macro(PFNGLBINDTEXTUREPROC,              glBindTexture)             \
  Macro(PFNGLDELETETEXTURESPROC,           glDeleteTextures)          \
  Macro(PFNGLTEXIMAGE1DPROC,               glTexImage1D)              \
  Macro(PFNGLTEXIMAGE2DPROC,               glTexImage2D)              \
  Macro(PFNGLACTIVETEXTUREPROC,            glActiveTexture)           \
  Macro(PFNGLTEXTUREPARAMETERIPROC,        glTextureParameteri)       \
  Macro(PFNGLGENBUFFERSPROC,               glGenBuffers)              \
  Macro(PFNGLBUFFERSUBDATAPROC,            glBufferSubData)           \
  Macro(PFNGLDELETEBUFFERSPROC,            glDeleteBuffers)           \
  Macro(PFNGLBINDBUFFERPROC,               glBindBuffer)              \
  Macro(PFNGLBUFFERDATAPROC,               glBufferData)              \
  Macro(PFNGLVERTEXATTRIBPOINTERPROC,      glVertexAttribPointer)     \
  Macro(PFNGLVERTEXATTRIBDIVISORPROC,      glVertexAttribDivisor)     \
  Macro(PFNGLENABLEVERTEXATTRIBARRAYPROC,  glEnableVertexAttribArray) \
  Macro(PFNGLGENVERTEXARRAYSPROC,          glGenVertexArrays)         \
  Macro(PFNGLBINDVERTEXARRAYPROC,          glBindVertexArray)         \
  Macro(PFNGLDRAWARRAYSPROC,               glDrawArrays)              \
  Macro(PFNGLDRAWARRAYSINSTANCEDPROC,      glDrawArraysInstanced)     \
  Macro(PFNGLDELETEVERTEXARRAYSPROC,       glDeleteVertexArrays)      \
  Macro(PFNGLTEXPARAMETERIPROC,            glTexParameteri)           \
  Macro(PFNGLTEXPARAMETERIPROC,            glTexParameteri)           \
  Macro(PFNGLCREATESHADERPROC,             glCreateShader)            \
  Macro(PFNGLDELETESHADERPROC,             glDeleteShader)            \
  Macro(PFNGLSHADERSOURCEPROC,             glShaderSource)            \
  Macro(PFNGLCOMPILESHADERPROC,            glCompileShader)           \
  Macro(PFNGLGETSHADERIVPROC,              glGetShaderiv)             \
  Macro(PFNGLGETSHADERINFOLOGPROC,         glGetShaderInfoLog)        \
  Macro(PFNGLATTACHSHADERPROC,             glAttachShader)            \
  Macro(PFNGLCREATEPROGRAMPROC,            glCreateProgram)           \
  Macro(PFNGLDELETEPROGRAMPROC,            glDeleteProgram)           \
  Macro(PFNGLLINKPROGRAMPROC,              glLinkProgram)             \
  Macro(PFNGLGETPROGRAMIVPROC,             glGetProgramiv)            \
  Macro(PFNGLGETPROGRAMINFOLOGPROC,        glGetProgramInfoLog)       \
  Macro(PFNGLUSEPROGRAMPROC,               glUseProgram)              \
  Macro(PFNGLGETUNIFORMLOCATIONPROC,       glGetUniformLocation)      \
  Macro(PFNGLGETERRORPROC,                 glGetError)                \
  Macro(PFNGLUNIFORM1IPROC,                glUniform1i)               \
  Macro(PFNGLUNIFORM1FPROC,                glUniform1f)               \
  Macro(PFNGLUNIFORM2FPROC,                glUniform2f)               \
  Macro(PFNGLUNIFORM4FPROC,                glUniform4f)               \
  Macro(PFNGLGENFRAMEBUFFERSPROC,          glGenFramebuffers)         \
  Macro(PFNGLBINDFRAMEBUFFERPROC,          glBindFramebuffer)         \
  Macro(PFNGLDELETEFRAMEBUFFERSPROC,       glDeleteFramebuffers)      \
  Macro(PFNGLFRAMEBUFFERTEXTURE2DPROC,     glFramebufferTexture2D)    \
  Macro(PFNGLSTENCILMASKPROC,              glStencilMask)             \
  Macro(PFNGLSTENCILFUNCPROC,              glStencilFunc)             \
  Macro(PFNGLBLENDFUNCSEPARATEPROC,        glBlendFuncSeparate)       \
  Macro(PFNGLBINDATTRIBLOCATIONPROC,       glBindAttribLocation)      \
  Macro(PFNGLUNIFORMBLOCKBINDINGPROC,      glUniformBlockBinding)     \
  Macro(PFNGLGETINTEGERVPROC,              glGetIntegerv)             \
  Macro(PFNGLFINISHPROC,                   glFinish)                  \
  Macro(PFNGLPIXELSTOREIPROC,              glPixelStorei)             \
  Macro(PFNGLGENERATEMIPMAPPROC,           glGenerateMipmap)          \
  Macro(PFNGLPIXELSTOREIPROC,              glPixelStorei)             \
  Macro(PFNGLTEXSUBIMAGE2DPROC,            glTexSubImage2D)           \
  Macro(PFNGLBINDBUFFERRANGEPROC,          glBindBufferRange)         \
  Macro(PFNGLCOLORMASKPROC,                glColorMask)               \
  Macro(PFNGLSTENCILOPSEPARATEPROC,        glStencilOpSeparate)       \
  Macro(PFNGLDISABLEPROC,                  glDisable)                 \
  Macro(PFNGLSTENCILOPPROC,                glStencilOp)               \
  Macro(PFNGLGETUNIFORMBLOCKINDEXPROC,     glGetUniformBlockIndex)    \
  Macro(PFNGLCULLFACEPROC,                 glCullFace)                \
  Macro(PFNGLFRONTFACEPROC,                glFrontFace)               \
  Macro(PFNGLUNIFORM2FVPROC,               glUniform2fv)              \
  Macro(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray)
 
#define Decl(Type, Name) Type Name;
  SELECTED_OPENGL_FUNCTIONS(Decl)
#undef Decl

//.link: https://www.khronos.org/opengl/wiki/Load_OpenGl_Functions
function void *GetAnyGlProc(HMODULE GlDll, PROC (*wglGetProcAddressProc)(LPCSTR), const c8 *Name) {
  void *Proc = (void *)wglGetProcAddressProc(Name);
  if (Proc == 0 || (Proc == (void*)0x1) || (Proc == (void*)0x2) || (Proc == (void*)0x3) || (Proc == (void*)-1))
    Proc = (void *)GetProcAddress(GlDll, Name);
  return Proc;
}
function void ReadShaders(str Source, c8 **VertSrc, c8 **FragSrc) {
  c8 *Cur = Source.Str;
  u32 i   = 0;
  while (*Cur != '$' && i++ < Source.Len)
    Cur += 1;
  *Cur++ = '\0';
  *VertSrc = Source.Str;
  *FragSrc = Cur;
}
function u32 CompileShader(GLchar *Src, GLenum Type) {
  u32 Shader = glCreateShader(Type);
  glShaderSource(Shader, 1, &Src, NULL);
  glCompileShader(Shader);
  int Success = 0;
  glGetShaderiv(Shader, GL_COMPILE_STATUS, &Success);
  if (!Success) {
    c8  LogStr[1024];
    u32 LogLen = 0;
    glGetShaderInfoLog(Shader, sizeof(LogStr), &LogLen, LogStr);
    fprintf(stderr, "%.*s\n", LogLen, LogStr);
    return 0;
  }
  return Shader;
}
function u32 CompileProgram(c8 *Path) {
  c8 *VertSrc;
  c8 *FragSrc;
  ReadShaders(FileOpen(Path), &VertSrc, &FragSrc);
  u32 VertShader = CompileShader(VertSrc, GL_VERTEX_SHADER);
  u32 FragShader = CompileShader(FragSrc, GL_FRAGMENT_SHADER);
  u32 Program    = glCreateProgram();
  glAttachShader(Program, VertShader);
  glAttachShader(Program, FragShader);
  glLinkProgram(Program);
  c8  LogStr[1024];
  u32 LogLen = 0;
  glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &LogLen);
  if (LogLen) {
    glGetProgramInfoLog(Program, LogLen, NULL, LogStr);
    fprintf(stderr, "%.*s\n", LogLen, LogStr);
    return 0;
  }
  i32 Stat = 0;
  glGetProgramiv(Program, GL_LINK_STATUS, &Stat);
  if (Stat == 0) {
    glDeleteProgram(Program);
    Program = 0;
    return 0;
  }
  glDeleteShader(VertShader);
  glDeleteShader(FragShader);
  return Program;
}

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

b32   IsRunning = true;
void *MainFiber = null;
static LRESULT CALLBACK WindProc(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam) {
  LRESULT Result = 0;
  switch (Message) {
    case WM_DESTROY:
      IsRunning = false;
      break;
    case WM_TIMER:
      SwitchToFiber(MainFiber);
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
      Result = DefWindowProcW(Handle, Message, wParam, lParam);
  }
  return Result;
}
static void CALLBACK FiberProc(void *MainFiber) {
  while (true) {
    MSG Message;
    while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE)) {
      TranslateMessage(&Message);
      DispatchMessage(&Message);
    }
    SwitchToFiber(MainFiber);
  }
}
typedef HGLRC wgl_create_context_proc  (HDC);
typedef BOOL  wgl_delete_context_proc  (HGLRC);
typedef BOOL  wgl_make_current_proc    (HDC, HGLRC);
typedef PROC  wgl_get_proc_address_proc(LPCSTR);
wgl_create_context_proc   *wglCreateContextProc  = null;
wgl_delete_context_proc   *wglDeleteContextProc  = null;
wgl_make_current_proc     *wglMakeCurrentProc    = null;
wgl_get_proc_address_proc *wglGetProcAddressProc = null;
function void InitializeOpenGL(HINSTANCE Instance, HDC *DevCtx, HGLRC *GlCtx) {
  HMODULE OpenGlDll = LoadLibraryA("opengl32.dll");

  wglCreateContextProc  = (wgl_create_context_proc*)GetProcAddress(OpenGlDll, "wglCreateContext");
  wglDeleteContextProc  = (wgl_delete_context_proc*)GetProcAddress(OpenGlDll, "wglDeleteContext");
  wglMakeCurrentProc    = (wgl_make_current_proc*)GetProcAddress(OpenGlDll, "wglMakeCurrent");
  wglGetProcAddressProc = (wgl_get_proc_address_proc*)GetProcAddress(OpenGlDll, "wglGetProcAddress");

  WNDCLASSW DumbClass = {
    .lpfnWndProc   = DefWindowProcW,
    .hInstance     = Instance,
    .lpszClassName = L"dummy class"
  };
  RegisterClassW(&DumbClass);
  HWND DumbWindow = CreateWindowW(L"dummy class", L"dummy title", WS_TILEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, Instance, 0);
  HDC  DumbDevCtx = GetDC(DumbWindow);
  PIXELFORMATDESCRIPTOR DumbPixDesc = {
    .nSize      = sizeof(DumbPixDesc),
    .nVersion   = 1,
    .dwFlags    = PFD_SUPPORT_OPENGL,
    .iPixelType = PFD_TYPE_RGBA,
    .cColorBits = 24
  };
  INT DumbPixIdx = ChoosePixelFormat(DumbDevCtx, &DumbPixDesc);
  SetPixelFormat(DumbDevCtx, DumbPixIdx, &DumbPixDesc);

  HGLRC DumbGlCtx = wglCreateContextProc(DumbDevCtx);
  wglMakeCurrentProc(DumbDevCtx, DumbGlCtx);

  
  PFNWGLCHOOSEPIXELFORMATARBPROC    wglChoosePixelFormatARBProc    = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddressProc("wglChoosePixelFormatARB");
  PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARBProc = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddressProc("wglCreateContextAttribsARB");
  PFNWGLSWAPINTERVALEXTPROC         wglSwapIntervalEXT             = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddressProc("wglSwapIntervalEXT");

  wglMakeCurrentProc(null, null);
  ReleaseDC(DumbWindow, DumbDevCtx);
  wglDeleteContextProc(DumbGlCtx);
  DestroyWindow(DumbWindow);

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
  wglChoosePixelFormatARBProc(*DevCtx, FormatAttribs, null, 1, &PixelFormatIdx, &NumberOfFormats);
  PIXELFORMATDESCRIPTOR PixelFormatDescriptor = {0};
  SetPixelFormat(*DevCtx, PixelFormatIdx, &PixelFormatDescriptor);
  INT ContextAttribs[] = {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
    WGL_CONTEXT_MINOR_VERSION_ARB, 5,
    WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
    WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
    0
  };
  *GlCtx = wglCreateContextAttribsARBProc(*DevCtx, 0, ContextAttribs);
  wglMakeCurrentProc(*DevCtx, *GlCtx);

  #define Assign(Type, Name) Name = (Type)(GetAnyGlProc(OpenGlDll, wglGetProcAddressProc, strify(Name))); Assert(Name, "Could not load "strify(Name));
  SELECTED_OPENGL_FUNCTIONS(Assign)
  #undef Assign

  wglSwapIntervalEXT(1);

  glEnable(GL_BLEND);
  glEnable(GL_MULTISAMPLE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
i32 main(void) {
  HINSTANCE Instance = GetModuleHandleW(null);
  MainFiber = ConvertThreadToFiber(0);
  void *MsgFiber = CreateFiber(0, (PFIBER_START_ROUTINE)FiberProc, MainFiber);

  WNDCLASSW Class = {
    .style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
    .lpfnWndProc   = WindProc,
    .hInstance     = Instance,
    .hCursor       = LoadCursor(0, IDC_ARROW),
    .lpszClassName = L"class"
  };
  RegisterClassW(&Class);
  HWND Window = CreateWindowW(L"class", L"title", WS_TILEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, Instance, 0);

  HDC   DevCtx = GetDC(Window);
  HGLRC GlCtx  = null;
  InitializeOpenGL(Instance, &DevCtx, &GlCtx);

  stbtt_fontinfo FontInfo;
  stbtt_InitFont(&FontInfo, FileOpen("download/pala.ttf").Str, 0);
  stbtt_vertex *GlyphVertices = MemRes(1024*sizeof(stbtt_vertex));
  u32 NumTris = stbtt_GetCodepointShape(&FontInfo, 's', &GlyphVertices);

  u32 Program;
  u32 Vbo;
  u32 Vao;

  struct tri {
    f32 x0, y0;
    f32 x1, y1;
    f32 cx, cy;
  } *Tris = MemRes(sizeof(struct tri)*(NumTris-1));

  u32 u = 0;

  f32 x = 0, y = 0;
  fornum (i, NumTris) {
    stbtt_vertex *g = &GlyphVertices[i];
    switch (g->type) {
      case STBTT_vmove:
      case STBTT_vline:
      case STBTT_vcubic:
        x = g->x/2;
        y = g->y/2;
        break;
      case STBTT_vcurve:
        Tris[u].x0 = g->x/2;
        Tris[u].y0 = g->y/2;

        Tris[u].x1 = x;
        Tris[u].y1 = y;

        Tris[u].cx = g->cx/2;
        Tris[u].cy = g->cy/2;

        x = g->x/2;
        y = g->y/2;
        break;
    }
    u += 1;
  }

  // Tris[0].x0 = 0;
  // Tris[0].y0 = 0;
  // Tris[0].x1 = 300;
  // Tris[0].y1 = 0;
  // Tris[0].cx = 1090/2;
  // Tris[0].cy = 600;
  glGenVertexArrays(1, &Vao);
  glBindVertexArray(Vao);
  
  glGenBuffers(1, &Vbo);
  glBindBuffer(GL_ARRAY_BUFFER, Vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(struct tri)*(NumTris-1), Tris, GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(f32), (void*)0);
  glEnableVertexAttribArray(0);

  Program = CompileProgram("bez.hlsl");
  u32 UniformWndDim = glGetUniformLocation(Program, "WndDim");

  ShowWindow(Window, SW_SHOWDEFAULT);

  while (IsRunning) {
    SwitchToFiber(MsgFiber);

    RECT ClientRect;
    GetClientRect(Window, &ClientRect);
    i32 WndW = ClientRect.right  - ClientRect.left;
    i32 WndH = ClientRect.bottom - ClientRect.top;

    POINT WindowPos = {ClientRect.left, ClientRect.top};
    ClientToScreen(Window, &WindowPos);

    POINT MousePos;
    GetCursorPos(&MousePos);
    i32 MouseX = MousePos.x - WindowPos.x;
    i32 MouseY = MousePos.y - WindowPos.y;

    DevCtx = GetDC(Window);
    wglMakeCurrentProc(DevCtx, GlCtx);

    // Tris[0].cx = MouseX;
    // Tris[0].cy = WndH - MouseY;

		glViewport(0, 0, WndW, WndH);
    glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(Program);
    glUniform2f(UniformWndDim, WndW, WndH);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 6*sizeof(f32), Tris);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(struct tri)*(NumTris-1));

    SwapBuffers(DevCtx);
    ReleaseDC(Window, DevCtx);
  }

  glDeleteVertexArrays(1, &Vao);
  glDeleteBuffers(1, &Vbo);
  glDeleteProgram(Program);

  wglDeleteContextProc(GlCtx);
  wglMakeCurrentProc(DevCtx, null);
  ShowWindow(Window, SW_HIDE);
  ReleaseDC(Window, DevCtx);

  return 0;
}