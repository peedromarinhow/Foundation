#include "foundation.h"

/**************************************************************************************************

  LAYER ONE

  RENDER

.doc:
  Todo().

***************************************************************************************************/



/**************************************************************************************************
  HEADER
***************************************************************************************************/
#if !defined(RENDER_HEAD)
#define RENDER_HEAD

typedef struct _renderer {
  window *Wnd;
  size    VertsLen;
  size    VertsCap;
  r32v2  *Verts;

  void *Back;
} renderer;

function renderer *RenInit(pool *Pool, window *Window);
function void      RenEnd (renderer *Renderer);
function void      RenDraw(renderer *Renderer);

////////////////////////
// OPENGL
typedef struct _opengl_renderer {
  u32 ShaderProgram;
  u32 VertsArray;
  u32 VertsBuff;
} opengl_renderer;
function u32 _OpenGLCompileShader(pool *Pool, const c8 *Src, u32 Type);
function u32 _OpenGLLinkProgram  (pool *Pool, u32 VertShader, u32 FragShader);

function void _OpenGLRenInit(pool *Pool, renderer *Renderer);
function void _OpenGLRenEnd (renderer *Renderer);
function void _OpenGLRenDraw(renderer *Renderer);

////////////////////////
// D3D11
typedef struct _d3d11_renderer {
  int Tmp;
} d3d11_renderer;

function void _D3d11RenInit(pool *Pool, renderer *Renderer);
function void _D3d11RenEnd (renderer *Renderer);
function void _D3d11RenDraw(renderer *Renderer);

#endif//RENDER_HEAD

/**************************************************************************************************
  IMPLEMENTATION
***************************************************************************************************/
#if defined(RENDER_IMPL)

////////////////////////
// OPENGL
function u32 _OpenGLCompileShader(pool *Pool,  const c8 *Src, u32 Type) {
  u32 Res = GlCreateShader(Type);
  GlShaderSource(Res, 1, &Src, NULL);
  GlCompileShader(Res);

  c8 *LogStr = null;
  i32 LogLen = 0;
  GlGetShaderiv(Res, GL_INFO_LOG_LENGTH, &LogLen);
  if (LogLen) {
    pool_snap Snap = GetPoolSnapshot(Pool);
    LogStr = PoolPush(Snap.Pool, sizeof(c8)*LogLen);
    GlGetShaderInfoLog(Res, LogLen, NULL, LogStr);
    fprintf(stderr, "Error: %s shader compilation failed. Error: %s\n", (Type==GL_VERTEX_SHADER)? "vertex":"fragment", LogStr);
    EndPoolSnapshot(Snap);
    Res = 0;
  }
  return Res;
}
function u32 _OpenGLLinkProgram(pool *Pool, u32 VertShader, u32 FragShader) {
  u32 Res = GlCreateProgram();
  GlAttachShader(Res, VertShader);
  GlAttachShader(Res, FragShader);
  GlLinkProgram(Res);

  c8 *LogStr = null;
  i32 LogLen = 0;
  GlGetProgramiv(Res, GL_INFO_LOG_LENGTH, &LogLen);
  if (LogLen) {
    pool_snap Snap = GetPoolSnapshot(Pool);
    LogStr = PoolPush(Snap.Pool, sizeof(c8)*LogLen);
    GlGetProgramInfoLog(Res, LogLen, NULL, LogStr);
    fprintf(stderr, "Error: program compilation failed. Error: %s\n", LogStr);
    EndPoolSnapshot(Snap);
    Res = 0;
  }

  return Res;
}

function void _OpenGLRenInit(pool *Pool, renderer *Renderer) {
  const c8 *VertShaderSrc = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\n"
  ;
  const c8 *FragShaderSrc = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 Color;\n"
    "void main() {\n"
    "  FragColor = Color;\n"
    "}\n"
  ;

  //.link: https://learnopengl.com/Getting-started/Hello-Triangle
  // Create vertex shader.
  u32 VertShader = _OpenGLCompileShader(Pool, VertShaderSrc, GL_VERTEX_SHADER);
  // Create fragment shader.
  u32 FragShader = _OpenGLCompileShader(Pool, FragShaderSrc, GL_FRAGMENT_SHADER);

  // Create program and delete shaders.
  opengl_renderer *Back = PoolPushZeros(Pool, sizeof(opengl_renderer));
  Back->ShaderProgram = _OpenGLLinkProgram(Pool, VertShader, FragShader);
  GlDeleteShader(VertShader);
  GlDeleteShader(FragShader);

  // Create vertex array.
  GlGenVertexArrays(1, &Back->VertsArray);
  GlBindVertexArray(Back->VertsArray);

  // Create vertex buffer.
  GlGenBuffers(1, &Back->VertsBuff);
  GlBindBuffer(GL_ARRAY_BUFFER, Back->VertsBuff);
  GlBufferData(GL_ARRAY_BUFFER, Renderer->VertsCap*sizeof(r32), Renderer->Verts, GL_DYNAMIC_DRAW);

  GlEnableVertexAttribArray(0);
  GlVertexAttribPointer(0, 2, GL_FLOAT, false, 0, null);

  Renderer->Back = Back;
}
function void _OpenGLRenEnd(renderer *Renderer) {
  opengl_renderer *Back = cast(opengl_renderer*, Renderer->Back);

  GlDeleteVertexArrays(1, &Back->VertsArray);
  GlDeleteBuffers     (1, &Back->VertsBuff);
  GlDeleteProgram     (Back->ShaderProgram);
}

function void _OpenGLRenDraw(renderer *Renderer) {
  opengl_renderer *Back = cast(opengl_renderer*, Renderer->Back);

  GlViewport(0, 0, Renderer->Wnd->Dim.w, Renderer->Wnd->Dim.h);
  GlClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  GlClear(GL_COLOR_BUFFER_BIT);

  GlUseProgram(Back->ShaderProgram);
  GlUniform4f(GlGetUniformLocation(Back->ShaderProgram, "Color"), 1.0f, 0.5f, 0.2f, 1.0f);
  GlBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(r32v2)*Renderer->VertsLen, Renderer->Verts);
  GlDrawArraysInstanced(GL_TRIANGLES, 0, cast(u32, Renderer->VertsLen), 1);
}

////////////////////////
// D3D11
function void _D3d11RenInit(pool *Pool, renderer *Renderer) {
  d3d11_renderer *Back = PoolPushZeros(Pool, sizeof(opengl_renderer));

  Todo();

  Renderer->Back = Back;
}
function void _D3d11RenEnd (renderer *Renderer) {
  d3d11_renderer *Back = cast(d3d11_renderer*, Renderer->Back);

  Todo();
}
function void _D3d11RenDraw(renderer *Renderer) {
  d3d11_renderer *Back = cast(d3d11_renderer*, Renderer->Back);

  Todo();
}

////////////////////////
// RENDERER WRAPPING
function void RenPushVert(renderer *Renderer, r32v2 v) {
  Assert(Renderer->VertsLen < Renderer->VertsCap);
  Renderer->Verts[Renderer->VertsLen++] = v;
}

function renderer *RenInit(pool *Pool, window *Window) {
  renderer *Renderer  = PoolPushZeros(Pool, sizeof(renderer));
  Renderer->Wnd       = Window;
  Renderer->VertsCap  = 3*32;
  Renderer->Verts     = PoolPush(Pool, Renderer->VertsCap*sizeof(r32v2));

  if (Window->GfxApiKind == gfx_api_None)
    Todo();
  else
  if (Window->GfxApiKind == gfx_api_Opengl)
    _OpenGLRenInit(Pool, Renderer);
  else
  if (Window->GfxApiKind == gfx_api_D3d11)
    _D3d11RenInit(Pool, Renderer);

  return Renderer;
}
function void RenEnd(renderer *Renderer) {
  if (Renderer->Wnd->GfxApiKind == gfx_api_None)
    Todo();
  else
  if (Renderer->Wnd->GfxApiKind == gfx_api_Opengl)
    _OpenGLRenEnd(Renderer);
  else
  if (Renderer->Wnd->GfxApiKind == gfx_api_D3d11)
    _D3d11RenEnd(Renderer);
}

function void RenDraw(renderer *Renderer) {
  if (Renderer->Wnd->GfxApiKind == gfx_api_None)
    Todo();
  else
  if (Renderer->Wnd->GfxApiKind == gfx_api_Opengl)
    _OpenGLRenDraw(Renderer);
  else
  if (Renderer->Wnd->GfxApiKind == gfx_api_D3d11)
    _D3d11RenDraw(Renderer);
  
  Renderer->VertsLen = 0;
}

#endif//RENDER_IMPL