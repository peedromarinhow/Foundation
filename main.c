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
function id_and_log _OpenGlLinkProgram(pool *Pool, u32 QuadVertShader, u32 QuadFragShader) {
  u32 Program = GlCreateProgram();

  GlAttachShader(Program, QuadVertShader);
  GlAttachShader(Program, QuadFragShader);
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

typedef union _quad {
  struct {
    r32v4 Pts;
    r32v4 Clr;
    r32   r,s;
  };
  r32 c[8];
} quad;

function i32 Main(str8 ArgStr) {
  pool *Pool = PoolReserve(0);
  wnd  *Wnd  = SysInitWnd();

  const c8 *QuadVertShaderSrc = 
    "#version 330 core\n"

    "uniform vec2  WndDim;\n"

    "layout (location = 0) in vec2  TriPatt;\n"
    "layout (location = 1) in vec4  QuadPts;\n"
    "layout (location = 2) in vec4  QuadClr;\n"
    "layout (location = 3) in float QuadR;\n"
    "layout (location = 4) in float QuadS;\n"

    "out vec2  Cent;\n"
    "out vec2  Size;\n"
    "out vec2  Pos;\n"
    "out vec4  Clr;\n"
    "out vec2  Uv;\n"
    "out float r;\n"
    "out float s;\n"

    "void main() {\n"
    "  Cent = (QuadPts.xy + QuadPts.zw)*0.5;\n"
    "  Size = (QuadPts.zw - QuadPts.xy)*0.5;\n"
    "  Pos  = (Cent + Size*TriPatt);\n"
    "  Clr  = QuadClr;\n"
    "  Uv   = (TriPatt + vec2(1.0, 1.0))*0.5;\n"
    "  r    = QuadR;\n"
    "  s    = QuadS;\n"
    "  gl_Position = vec4(2*Pos/WndDim - vec2(1, 1), 0.0, 1.0);\n"
    "}\n"
  ;
  const c8 *QuadFragShaderSrc = 
    "#version 330 core\n"

    "uniform sampler2D Texture;\n"

    "in  vec2  Cent;\n"
    "in  vec2  Size;\n" 
    "in  vec2  Pos;\n"
    "in  vec4  Clr;\n"
    "in  vec2  Uv;\n"
    "in  float r;\n"
    "in  float s;\n"

    "out vec4 FragColor;\n"

    "void main() {\n"
    "  vec2  p = abs(Pos - Cent) - Size + vec2(r, r) + vec2(s, s);\n"
    "  float d = length(max(p, 0)) + min(max(p.x, p.y), 0) - r;\n"
    "  float Sample = 1;\n"//texture(Texture, Uv).r;\n"
    "  FragColor = vec4(Clr.xyz, Clr.w*smoothstep(s, -s, d)*Sample);\n"
    "}\n"
  ;

  //.link: https://learnopengl.com/Getting-started/Hello-Triangle
  id_and_log QuadVertShader    = _OpenGlCompileShader(Pool, QuadVertShaderSrc, GL_VERTEX_SHADER);
  id_and_log QuadFragShader    = _OpenGlCompileShader(Pool, QuadFragShaderSrc, GL_FRAGMENT_SHADER);
  id_and_log QuadShaderProgram = _OpenGlLinkProgram(Pool, QuadVertShader.Id, QuadFragShader.Id);
  GlDeleteShader(QuadVertShader.Id);
  GlDeleteShader(QuadFragShader.Id);

  u32 Vao = 0;
  GlGenVertexArrays(1, &Vao);
  GlBindVertexArray(Vao);

  u32 Vbo = 0;
  GlGenBuffers(1, &Vbo);
  GlBindBuffer(GL_ARRAY_BUFFER, Vbo);

  byte *TexPixels = PoolPushZeros(Pool, 256*256);
  ItrNum (y, 256) {
    byte *Line = TexPixels + y*256;
    ItrNum (x, 256) {
      u32 v = ((x/40) + (y/40))%4;
      v *= 85;
      Line[x] = cast(u8, v);
    }
  }

  u32 Tex = 0;
  GlGenTextures(1, &Tex);
  GlBindTexture(GL_TEXTURE_2D, Tex);
  GlTexImage2D(GL_TEXTURE_2D, Tex, GL_RED, 256, 256, 0, GL_RED, GL_UNSIGNED_BYTE, TexPixels);
  GlTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  GlTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  GlTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  GlTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  GlBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  GlEnable(GL_BLEND);
  // GlEnable(GL_FRAMEBUFFER_SRGB);

  r32 TriangulationPatter[] = {
    -1.0f, +1.0f,
    +1.0f, +1.0f,
    -1.0f, -1.0f,
    +1.0f, +1.0f,
    -1.0f, -1.0f,
    +1.0f, -1.0f,
  };
  i32 TriangulationPatterSize = countof(TriangulationPatter);

  i32  BuffSize = TriangulationPatterSize*sizeof(r32) + 32*sizeof(quad);
  r32 *Buff     = PoolPushZeros(Pool, BuffSize);
  ItrNum (Idx, countof(TriangulationPatter))
    Buff[Idx] = TriangulationPatter[Idx];

  quad *Quads  = cast(quad*, Buff + TriangulationPatterSize);
  Quads[0].Pts = R32v4(50.0f, 50.0f, 100.0f, 200.0f);
  Quads[0].Clr = R32v4(0.0f, 1.0f, 0.6f, 1.0f);
  // Quads[0].r   = 10.0f;
  // Quads[0].s   = 1.0f;

  Quads[1].Pts = R32v4(100.0f, 200.0f, 200.0f, 400.0f);
  Quads[1].Clr = R32v4(0.6f, 0.0f, 1.0f, 1.0f);
  // Quads[1].r   = 20.0f;
  // Quads[1].s   = 10.0f;

  Quads[2].Pts = R32v4(200.0f, 400.0f, 250.0f, 450.0f);
  Quads[2].Clr = R32v4(0.0f, 0.6f, 1.0f, 1.0f);
  // Quads[2].r   = 10.0f;
  // Quads[2].s   = 5.0f;

  BuffSize = TriangulationPatterSize*sizeof(r32) + 3*sizeof(quad);

  const void *QuadsOffs = cast(void*, Quads) - cast(void*, Buff);

  while (!Wnd->Finish) {
    SysWndPull(Wnd);

    r32 WndW   = Wnd->w,      WndH   = Wnd->h;
    r32 MouseX = Wnd->MouseX, MouseY = Wnd->MouseY;

    GlViewport(0, 0, WndW, WndH);
    GlClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    GlClear(GL_COLOR_BUFFER_BIT);
    
    GlBufferData(GL_ARRAY_BUFFER, BuffSize, Buff, GL_STREAM_DRAW);

    GlUseProgram(QuadShaderProgram.Id);

    GlUniform2f(GlGetUniformLocation(QuadShaderProgram.Id, "WndDim"), WndW, WndH);

    GlUniform1i(GlGetUniformLocation(QuadShaderProgram.Id, "Texture"), 0);
    GlActiveTexture(GL_TEXTURE0);
    GlBindTexture(GL_TEXTURE_2D, Tex);

    GlEnableVertexAttribArray(0);
    GlVertexAttribDivisor(0, 0);
    GlVertexAttribPointer(0, 2, GL_FLOAT, false, 2*sizeof(r32), 0);

    GlEnableVertexAttribArray(1);
    GlVertexAttribDivisor(1, 1);
    GlVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(quad), QuadsOffs + MemberOffset(quad, Pts));

    GlEnableVertexAttribArray(2);
    GlVertexAttribDivisor(2, 1);
    GlVertexAttribPointer(2, 4, GL_FLOAT, false, sizeof(quad), QuadsOffs + MemberOffset(quad, Clr));

    GlEnableVertexAttribArray(3);
    GlVertexAttribDivisor(3, 1);
    GlVertexAttribPointer(3, 1, GL_FLOAT, false, sizeof(quad), QuadsOffs + MemberOffset(quad, r));

    GlEnableVertexAttribArray(4);
    GlVertexAttribDivisor(4, 1);
    GlVertexAttribPointer(4, 1, GL_FLOAT, false, sizeof(quad), QuadsOffs + MemberOffset(quad, s));

    GlDrawArraysInstanced(GL_TRIANGLES, 0, 6, 4);

    SysWndPush(Wnd);
  }

  GlDeleteProgram(QuadShaderProgram.Id);

  SysKillWnd(Wnd);

  return 0;
}