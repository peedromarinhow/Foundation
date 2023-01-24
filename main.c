#define FOUNDATION_IMPL
#include "foundation.h"

typedef struct _id_log {
  u32  Id;
  str8 Log;
} id_and_log;

function id_and_log _OpenGlCompileShader(pool *Pool, const c8 *Src, u32 Type) {
  u32 Shader = GlCreateShader(Type);
  GlShaderSource(Shader, 1, &Src, NULL);
  GlCompileShader(Shader);

  c8 *LogStr = null;
  i32 LogLen = 0;
  GlGetShaderiv(Shader, GL_INFO_LOG_LENGTH, &LogLen);
  if (LogLen) {
    LogStr = PoolPush(Pool, sizeof(c8)*LogLen);
    GlGetShaderInfoLog(Shader, LogLen, NULL, LogStr);
  }

  i32 Stat = 0;
  GlGetShaderiv(Shader, GL_COMPILE_STATUS, &Stat);
  if (Stat == 0) {
    GlDeleteShader(Shader);
    Shader = 0;
  }

  return (id_and_log){Shader, {LogStr, LogLen}};
}
function id_and_log _OpenGlLinkProgram(pool *Pool, u32 VertShader, u32 FragShader) {
  u32 Program = GlCreateProgram();

  GlAttachShader(Program, VertShader);
  GlAttachShader(Program, FragShader);
  GlLinkProgram(Program);

  c8 *LogStr = null;
  i32 LogLen = 0;
  GlGetProgramiv(Program, GL_INFO_LOG_LENGTH, &LogLen);
  if (LogLen) {
    LogStr = PoolPush(Pool, sizeof(c8)*LogLen);
    GlGetProgramInfoLog(Program, LogLen, NULL, LogStr);
  }

  i32 Stat = 0;
  GlGetProgramiv(Program, GL_LINK_STATUS, &Stat);
  if (Stat == 0) {
    GlDeleteProgram(Program);
    Program = 0;
  }

  return (id_and_log){Program, {LogStr, LogLen}};
}

function i32 Main(str8 ArgStr) {
  pool *Pool = PoolReserve(0);
  wnd  *Wnd  = SysInitWnd();

  const c8 *VertShaderSrc = 
    "#version 330 core\n"
    "uniform vec2 ViewXform;\n"
    "layout (location = 0) in vec2 TriPatt;\n"
    "layout (location = 1) in vec4 RectPts;\n"
    "out vec2 RectCenter;\n"
    "out vec2 RectRadius;\n"
    "out vec2 RectPos;\n"
    "void main() {\n"
    "  vec2 Center = (RectPts.xy + RectPts.zw)*0.5;\n"
    "  vec2 Radius = (RectPts.zw - RectPts.xy)*0.5;\n"
    "  vec2 Pos    = Center + Radius*TriPatt;\n"
    "  gl_Position = vec4(Pos*ViewXform, 0.0, 1.0);\n"
    "  RectCenter  = Center;\n"
    "  RectRadius  = Radius;\n"
    "  RectPos     = Pos;\n"
    "}\n"
  ;
  const c8 *FragShaderSrc = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in  vec2 RectCenter;\n"
    "in  vec2 RectRadius;\n"
    "in  vec2 RectPos;\n"
    "void main() {\n"
    "  FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
    "}\n"
  ;

  //.link: https://learnopengl.com/Getting-started/Hello-Triangle
  id_and_log VertShader    = _OpenGlCompileShader(Pool, VertShaderSrc, GL_VERTEX_SHADER);
  id_and_log FragShader    = _OpenGlCompileShader(Pool, FragShaderSrc, GL_FRAGMENT_SHADER);
  id_and_log ShaderProgram = _OpenGlLinkProgram(Pool, VertShader.Id, FragShader.Id);
  GlDeleteShader(VertShader.Id);
  GlDeleteShader(FragShader.Id);

  u32 Vao = 0;
  GlGenVertexArrays(1, &Vao);
  GlBindVertexArray(Vao);

  u32 Vbo = 0;
  GlGenBuffers(1, &Vbo);
  GlBindBuffer(GL_ARRAY_BUFFER, Vbo);

  r32 PtsData[] = {
    -1.0f, +1.0f,
    +1.0f, +1.0f,
    -1.0f, -1.0f,
    +1.0f, +1.0f,
    -1.0f, -1.0f,
    +1.0f, -1.0f,

    200.f, 200.f, 300.f, 300.f,
    100.f, 400.f, 400.f, 500.f,
    600.f, 100.f, 700.f, 300.f,
    600.f, 400.f, 800.f, 700.f,
  };

  while (!Wnd->Finish) {
    SysWndPull(Wnd);

    GlViewport(0, 0, Wnd->w, Wnd->h);
    GlClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    GlClear(GL_COLOR_BUFFER_BIT);

    GlBufferData(GL_ARRAY_BUFFER, sizeof(PtsData), PtsData, GL_STREAM_DRAW);

    GlUseProgram(ShaderProgram.Id);

    u32 ViewXform = GlGetUniformLocation(ShaderProgram.Id, "ViewXform");
    GlUniform2f(ViewXform, 1.f/Wnd->w, -1.f/Wnd->h);

    GlEnableVertexAttribArray(0);
    GlVertexAttribDivisor(0, 0);
    GlVertexAttribPointer(0, 2, GL_FLOAT, false, 2*sizeof(r32), 0);

    GlEnableVertexAttribArray(1);
    GlVertexAttribDivisor(1, 1);
    GlVertexAttribPointer(1, 4, GL_FLOAT, false, 4*sizeof(r32), PtrFromInt(12*sizeof(r32)));

    GlDrawArraysInstanced(GL_TRIANGLES, 0, 6, 4);

    SysWndPush(Wnd);
  }

  GlDeleteProgram(ShaderProgram.Id);

  SysKillWnd(Wnd);

  return 0;
}