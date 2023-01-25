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
function struct id_and_log _OpenGlLinkProgram(pool *Pool, u32 VertShader, u32 GeomShader, u32 FragShader) {
  u32 Program = GlCreateProgram();
  GlAttachShader(Program, VertShader);
  GlAttachShader(Program, GeomShader);
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
    r32v2 Pos;
    r32v2 Tex;
    r32v4 Clr;
  };
} vert;

const u32 Vertn = 1024;

inline void PushVert(vert *Buff, i32 *Used, r32 x, r32 y, r32 u, r32 v, r32 r, r32 g, r32 b, r32 a) {
  Buff[*Used].Cmp[0] = x;
  Buff[*Used].Cmp[1] = y;
  Buff[*Used].Cmp[2] = u;
  Buff[*Used].Cmp[3] = v;
  Buff[*Used].Cmp[4] = r;
  Buff[*Used].Cmp[5] = g;
  Buff[*Used].Cmp[6] = b;
  Buff[*Used].Cmp[7] = a;
  *Used += 1;
}

function i32 Main(str8 ArgStr) {
  pool *Pool = PoolReserve(0);
  wnd  *Wnd  = SysInitWnd();

  i32   Used = 0;
  vert *Verts = PoolPushZeros(Pool, Vertn*sizeof(vert));

  u32 Vao, Vbo;
  GlGenVertexArrays(1, &Vao);
  GlBindVertexArray(Vao);

  GlGenBuffers(1, &Vbo);
  GlBindBuffer(GL_ARRAY_BUFFER, Vbo);
  GlBufferData(GL_ARRAY_BUFFER, Vertn, Verts, GL_DYNAMIC_DRAW);

  GlEnableVertexAttribArray(0);
  GlVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vert), &Member(vert, Pos));

  GlEnableVertexAttribArray(1);
  GlVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vert), &Member(vert, Tex));

  GlEnableVertexAttribArray(2);
  GlVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(vert), &Member(vert, Clr));

  const c8 *VertShaderSrc =
    "#version 330 core\n"
    "uniform vec2 WndDim;\n"
    "layout(location = 0) in vec2 Pos;\n"
    "layout(location = 1) in vec2 Tex;\n"
    "layout(location = 2) in vec4 Clr;\n"
    // "out vec2 FragPos;\n"
    // "out vec2 FragTex;\n"
    // "out vec4 FragClr;\n"
    "void main() {\n"
    // "  FragPos = Pos;\n"
    // "  FragTex = Tex;\n"
    // "  FragClr = Clr;\n"
    "  gl_Position = vec4(2*Pos/WndDim - vec2(1, 1), 0.0, 1.0);\n"
    "}\n"
  ;
  const c8 *GeomShaderSrc =
    "#version 330 core\n"
    "layout (lines) in;\n"
    "layout (triangle_strip, max_vertices = 4) out;\n"
    "void main() {\n"
    "  vec4 p = gl_in[0].gl_Position;\n"
    "  vec4 s = gl_in[1].gl_Position - gl_in[0].gl_Position;\n"
    "  gl_Position = p;\n"
    "  EmitVertex();\n"
    "  gl_Position = p + vec4(s.x, 0, 0, 0);\n"
    "  EmitVertex();\n"
    "  gl_Position = p + vec4(0, s.y, 0, 0);\n"
    "  EmitVertex();\n"
    "  gl_Position = p + s;\n"
    "  EmitVertex();\n"
    "  EndPrimitive();\n"
    "}\n"
  ;
  const c8 *FragShaderSrc =
    "#version 330 core\n"
    "out vec4 OutClr;\n"
    "void main() {\n"
    "  OutClr = vec4(1, 0.2, 1, 1);\n"
    "}\n"
  ;

  //.link: https://learnopengl.com/Getting-started/Hello-Triangle
  struct id_and_log VertShader    = _OpenGlCompileShader(Pool, VertShaderSrc, GL_VERTEX_SHADER);
  struct id_and_log GeomShader    = _OpenGlCompileShader(Pool, GeomShaderSrc, GL_GEOMETRY_SHADER);
  struct id_and_log FragShader    = _OpenGlCompileShader(Pool, FragShaderSrc, GL_FRAGMENT_SHADER);
  struct id_and_log ShaderProgram = _OpenGlLinkProgram(Pool, VertShader.Id, GeomShader.Id, FragShader.Id);
  GlDeleteShader(VertShader.Id);
  GlDeleteShader(GeomShader.Id);
  GlDeleteShader(FragShader.Id);

  u32 Error = GlGetError();

  GlBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  GlEnable(GL_BLEND);
  GlEnable(GL_FRAMEBUFFER_SRGB);

  while (!Wnd->Finish) {
    SysWndPull(Wnd);

    r32 WndW   = Wnd->w,      WndH   = Wnd->h;
    r32 MouseX = Wnd->MouseX, MouseY = WndH-Wnd->MouseY;

    GlViewport(0, 0, WndW, WndH);
    GlClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    GlClear(GL_COLOR_BUFFER_BIT);

    GlUseProgram(ShaderProgram.Id);
    GlUniform2f(GlGetUniformLocation(ShaderProgram.Id, "WndDim"), WndW, WndH);

    PushVert(Verts, &Used, 100, 100, 0, 0, 1, 0, 0, 1);
    PushVert(Verts, &Used, 200, 200, 0, 0, 1, 0, 0, 1);
    PushVert(Verts, &Used, MouseX,    MouseY,    0, 0, 0, 1, 0, 1);
    PushVert(Verts, &Used, MouseX+10, MouseY+10, 0, 0, 0, 1, 0, 1);

    GlBufferSubData(GL_ARRAY_BUFFER, 0, Used*sizeof(vert), Verts);
    GlDrawArrays(GL_LINES, 0, Used);
    Used = 0;

    SysWndPush(Wnd);
  }

  SysKillWnd(Wnd);

  return 0;
}
