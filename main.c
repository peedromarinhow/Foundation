#define ENABLE_ASSERT
#define DEBUG

#define FOUNDATION_IMPL
#include "foundation.h"
#undef  FOUNDATION_IMPL

struct id_and_log {
  u32  Id;
  str8 Log;
};

function struct id_and_log _OpenGlCompileShader(pool *Pool, const c8 *Src, u32 Type) {
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
  return (struct id_and_log){Shader, {LogStr, LogLen}};
}
function struct id_and_log _OpenGlLinkProgram(pool *Pool, u32 VertShader, u32 FragShader) {
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
  return (struct id_and_log){Program, {LogStr, LogLen}};
}

typedef union _vert {
  r32 Cmp[8];
  struct {
    r32v2 xy;
    r32v2 uv;
    r32v4 rgba;
  };
} vert;

typedef struct _gl_ren {
  vert *Buff;
  u32   Used;
  u32   Vao, Vbo, Prog;
} gl_ren;
function void GlInitRenderer(pool *Pool, gl_ren *Ren, u32 Vertn, const c8 *VertShaderSrc, const c8 *FragShaderSrc) {
  Ren->Used = 0;
  Ren->Buff = PoolPushZeros(Pool, Vertn*sizeof(vert));

  GlGenVertexArrays(1, &Ren->Vao);
  GlBindVertexArray(Ren->Vao);

  GlGenBuffers(1, &Ren->Vbo);
  GlBindBuffer(GL_ARRAY_BUFFER, Ren->Vbo);
  GlBufferData(GL_ARRAY_BUFFER, Vertn, Ren->Buff, GL_DYNAMIC_DRAW);

  GlEnableVertexAttribArray(0);
  GlVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vert), &Member(vert, xy));

  GlEnableVertexAttribArray(1);
  GlVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vert), &Member(vert, uv));

  GlEnableVertexAttribArray(2);
  GlVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(vert), &Member(vert, rgba));

  //.link: https://learnopengl.com/Getting-started/Hello-Triangle
  struct id_and_log VertShader    = _OpenGlCompileShader(Pool, VertShaderSrc, GL_VERTEX_SHADER);
  struct id_and_log FragShader    = _OpenGlCompileShader(Pool, FragShaderSrc, GL_FRAGMENT_SHADER);
  struct id_and_log ShaderProgram = _OpenGlLinkProgram(Pool, VertShader.Id, FragShader.Id);
  GlDeleteShader(VertShader.Id);
  GlDeleteShader(FragShader.Id);

  Ren->Prog = ShaderProgram.Id;

  GlBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  GlEnable(GL_BLEND);
  GlEnable(GL_FRAMEBUFFER_SRGB);
}
inline void GlPushVert(gl_ren *Ren, r32 x, r32 y, r32 u, r32 v, r32 r, r32 g, r32 b, r32 a) {
  vert *Buff = Ren->Buff;
  u32   Used = Ren->Used;
  Buff[Used].Cmp[0] = x;
  Buff[Used].Cmp[1] = y;
  Buff[Used].Cmp[2] = u;
  Buff[Used].Cmp[3] = v;
  Buff[Used].Cmp[4] = r;
  Buff[Used].Cmp[5] = g;
  Buff[Used].Cmp[6] = b;
  Buff[Used].Cmp[7] = a;
  Ren->Used += 1;
}
function void GlUpdate(gl_ren *Ren, r32 WndW, r32 WndH) {
    GlViewport(0, 0, WndW, WndH);
    GlClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    GlClear(GL_COLOR_BUFFER_BIT);

    u32 Prog = Ren->Prog;
    GlUseProgram(Prog);
    GlUniform2f(GlGetUniformLocation(Prog, "WndDim"), WndW, WndH);
    // GlUniform1i(GlGetUniformLocation(Prog, "Texture"), 0);

    u32 Used = Ren->Used;
    GlBufferSubData(GL_ARRAY_BUFFER, 0, Used*sizeof(vert), Ren->Buff);
    GlDrawArrays(GL_TRIANGLES, 0, Used);
    Ren->Used = 0;
}

function i32 Main(str8 ArgStr) {
  pool *Pool = PoolReserve(0);
  wnd  *Wnd  = SysInitWnd();

  const c8 *VertShaderSrc =
    "#version 330 core\n"
    "uniform vec2 WndDim;\n"
    "layout(location = 0) in vec2 xy;\n"
    "layout(location = 1) in vec2 uv;\n"
    "layout(location = 2) in vec4 rgba;\n"

    "out vec2 Frag_xy;\n"
    "out vec2 Frag_uv;\n"
    "out vec4 Frag_rgba;\n"

    "void main() {\n"
    "  Frag_xy  = xy;\n"
    "  Frag_uv = uv;\n"
    "  Frag_rgba = rgba;\n"
    "  gl_Position = vec4(2*xy/WndDim - vec2(1, 1), 0.0, 1.0);\n"
    "}\n"
  ;
  const c8 *FragShaderSrc =
    "#version 330 core\n"
    "uniform sampler2D Texture;"

    "in  vec2 Frag_xy;\n"
    "in  vec2 Frag_uv;\n"
    "in  vec4 Frag_rgba;\n"

    "out vec4 OutColor;\n"

    "void main() {\n"
    // "  OutColor = Frag_rgba*texture(Texture, Frag_uv.st);\n"
    "  OutColor = Frag_rgba;\n"
    "}\n"
  ;

  gl_ren Ren;
  GlInitRenderer(Pool, &Ren, 1204, VertShaderSrc, FragShaderSrc);

  while (!Wnd->Finish) {
    SysWndPull(Wnd);

    r32 MouseX = Wnd->MouseX,
        MouseY = Wnd->h - Wnd->MouseY;

    GlPushVert(&Ren,    100,    100, 0, 0, 1, 0, 0, 1);
    GlPushVert(&Ren,    200,    200, 0, 0, 0, 1, 0, 1);
    GlPushVert(&Ren, MouseX, MouseY, 0, 0, 0, 0, 1, 1);

    GlUpdate(&Ren, Wnd->w, Wnd->h);

    SysWndPush(Wnd);
  }

  SysKillWnd(Wnd);

  return 0;
}
