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
  r32    *Verts;

  void *Back;
} renderer;

function renderer *RenInit(pool *Pool, window *Window);

////////////////////////
// OPENGL
typedef struct _opengl_renderer {
  u32 ShaderProgram;
  u32 VertsArray;
  u32 VertsBuff;
} opengl_renderer;
function u32 OpenGLCompileShader(pool *Pool, opengl_api *Gl, const c8 *Src, u32 Type);
function u32 OpenGLLinkProgram  (pool *Pool, opengl_api *Gl, u32 VertShader, u32 FragShader);

#endif//RENDER_HEAD

/**************************************************************************************************
  IMPLEMENTATION
***************************************************************************************************/
#if defined(RENDER_IMPL)

function renderer *RenInit(pool *Pool, window *Window) {
  renderer *Ren  = PoolPushZeros(Pool, sizeof(renderer));
  Ren->Wnd       = Window;
  Ren->VertsCap  = 3*32;
  Ren->Verts     = PoolPush(Pool, Ren->VertsCap*sizeof(r32));
  Ren->VertsLen = 0;
  r32 Verts[] = {
    0.0f,  0.5f,  0.0f,
    0.5f, -0.5f,  0.0f,
   -0.5f, -0.5f,  0.0f,
   -1.0f,  0.5f,  0.0f
  };
  ItrNum (i, countof(Verts))
    Ren->Verts[Ren->VertsLen++] = Verts[i];

  opengl_api *Gl = cast(opengl_api*, Window->GfxApi);

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
  u32 VertShader = OpenGLCompileShader(Pool, Gl, VertShaderSrc, GL_VERTEX_SHADER);
  // Create fragment shader.
  u32 FragShader = OpenGLCompileShader(Pool, Gl, FragShaderSrc, GL_FRAGMENT_SHADER);

  // Create program and delete shaders.
  opengl_renderer *Back = PoolPushZeros(Pool, sizeof(opengl_renderer));
  Back->ShaderProgram = OpenGLLinkProgram(Pool, Gl, VertShader, FragShader);
  Gl->DeleteShader(VertShader);
  Gl->DeleteShader(FragShader);

  // Create vertex buffer.
  Gl->GenBuffers(1, &Back->VertsBuff);
  Gl->BindBuffer(GL_ARRAY_BUFFER, Back->VertsBuff);
  Gl->BufferData(GL_ARRAY_BUFFER, Ren->VertsCap*sizeof(r32), Ren->Verts, GL_STATIC_DRAW);

  // Create vertex array.
  Gl->GenVertexArrays(1, &Back->VertsArray);
  Gl->BindVertexArray(Back->VertsArray);
  Gl->EnableVertexAttribArray(0);
  Gl->BindBuffer(GL_ARRAY_BUFFER, Back->VertsBuff);
  Gl->VertexAttribPointer(0, 3, GL_FLOAT, false, 0, null);

  Ren->Back = Back;

  return Ren;
}
function void RenDraw(renderer *Ren) {
  opengl_api      *Gl   = cast(opengl_api*, Ren->Wnd->GfxApi);
  opengl_renderer *Back = cast(opengl_renderer*, Ren->Back);

  // Clear.
  Gl->ClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  Gl->Clear(GL_COLOR_BUFFER_BIT);

  // Draw the rectangle.
  Gl->UseProgram(Back->ShaderProgram);
  Gl->Uniform4f(Gl->GetUniformLocation(Back->ShaderProgram, "Color"), 1.0f, 0.5f, 0.2f, 1.0f);
  Gl->BindVertexArray(Back->VertsArray);
  Gl->DrawArrays(GL_LINES, 0, Ren->VertsLen/3);
}
function void RenEnd(renderer *Ren) {
  opengl_api      *Gl   = cast(opengl_api*, Ren->Wnd->GfxApi);
  opengl_renderer *Back = cast(opengl_renderer*, Ren->Back);

  // Arrays and buffers.
  Gl->DeleteVertexArrays(1, &Back->VertsArray);
  Gl->DeleteBuffers     (1, &Back->VertsBuff);

  // Delete program.
  Gl->DeleteProgram(Back->ShaderProgram);
}

function u32 OpenGLCompileShader(pool *Pool, opengl_api *Gl, const c8 *Src, u32 Type) {
  u32 Res = Gl->CreateShader(Type);
  Gl->ShaderSource(Res, 1, &Src, NULL);
  Gl->CompileShader(Res);

  c8 *LogStr = null;
  i32 LogLen = 0;
  Gl->GetShaderiv(Res, GL_INFO_LOG_LENGTH, &LogLen);
  if (LogLen) {
    pool_snap Snap = GetPoolSnapshot(Pool);
    LogStr = PoolPush(Snap.Pool, sizeof(c8)*LogLen);
    Gl->GetShaderInfoLog(Res, LogLen, NULL, LogStr);
    fprintf(stderr, "Error: %s shader compilation failed. Error: %s\n", (Type==GL_VERTEX_SHADER)? "vertex":"fragment", LogStr);
    EndPoolSnapshot(Snap);
    Res = 0;
  }
  return Res;
}

function u32 OpenGLLinkProgram(pool *Pool, opengl_api *Gl, u32 VertShader, u32 FragShader) {
  u32 Res = Gl->CreateProgram();
  Gl->AttachShader(Res, VertShader);
  Gl->AttachShader(Res, FragShader);
  Gl->LinkProgram(Res);

  c8 *LogStr = null;
  i32 LogLen = 0;
  Gl->GetProgramiv(Res, GL_INFO_LOG_LENGTH, &LogLen);
  if (LogLen) {
    pool_snap Snap = GetPoolSnapshot(Pool);
    LogStr = PoolPush(Snap.Pool, sizeof(c8)*LogLen);
    Gl->GetProgramInfoLog(Res, LogLen, NULL, LogStr);
    fprintf(stderr, "Error: program compilation failed. Error: %s\n", LogStr);
    EndPoolSnapshot(Snap);
    Res = 0;
  }

  return Res;
}

#endif//RENDER_IMPL