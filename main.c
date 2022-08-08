#define FOUNDATION_IMPLEMENTATION
#include "foundation.h"

function b32 CompileShader(opengl_api *Gl, str8 Src, u32 Type, u32 *Shader, c8 *Error, size ErrorLen) {
  if (Src.Ptr == null) {
   *Error = "Shader is null.";
    return false;
  }
  *Shader = Gl->CreateShader(Type);
  Gl->ShaderSource(*Shader, 1, &Src.Ptr, NULL);
  Gl->CompileShader(*Shader);
  int Ok;
  Gl->GetShaderiv(*Shader, GL_COMPILE_STATUS, &Ok);
  if (!Ok) {
    Gl->GetShaderInfoLog(*Shader, ErrorLen, NULL, Error);
    return false;
  }
  return true;
}

int main(int Argc, char **Argv) {
  SysInit(Argc, Argv);

  pool *Pool = PoolReserve(0);

  window     *Wnd = WndInit(gfx_api_Opengl, 0, 0, -1, -1);
  opengl_api *Gl  = cast(opengl_api*, Wnd->GfxApi);

  //.link: https://learnopengl.com/Getting-started/Hello-Triangle
  // Create vertex shader.
  #define MAX_ERROR 512
  c8 ErrorStr[MAX_ERROR];
  u32 VertShader;
  if (!CompileShader(Gl, SysOpenFile(Pool, Str8Cstr("shaders/main.vert")), GL_VERTEX_SHADER, &VertShader, ErrorStr, MAX_ERROR)) {
    fprintf(stderr, "Error: vertex shader compilation failed. Error: %s.\n", ErrorStr);
    return 1;
  }
  // Create fragment shader.
  u32 FragShader;
  if (!CompileShader(Gl, SysOpenFile(Pool, Str8Cstr("shaders/main.frag")), GL_FRAGMENT_SHADER, &FragShader, ErrorStr, MAX_ERROR)) {
    fprintf(stderr, "Error: fragment shader compilation failed. Error: %s.\n", ErrorStr);
    return 1;
  }

  // Create program and delete shaders.
  i32 Ok;
  u32 ShaderProgram = Gl->CreateProgram();
  Gl->AttachShader(ShaderProgram, VertShader);
  Gl->AttachShader(ShaderProgram, FragShader);
  Gl->LinkProgram(ShaderProgram);
  Gl->GetProgramiv(ShaderProgram, GL_LINK_STATUS, &Ok);
  if (!Ok) {
    Gl->GetProgramInfoLog(ShaderProgram, MAX_ERROR, NULL, ErrorStr);
    fprintf(stderr, "Error: program shader linking failed. Error: %s.\n", ErrorStr);
  }
  Gl->DeleteShader(VertShader);
  Gl->DeleteShader(FragShader);

  // Create buffers.
  r32 Verts[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
  };
  u32 Idxs[] = {  // note that we start from 0!
    0, 1, 3,      // first Triangle
    1, 2, 3       // second Triangle
  };
  u32 VBO, VAO, EBO;
  Gl->GenVertexArrays(1, &VAO);
  Gl->GenBuffers     (1, &VBO);
  Gl->GenBuffers     (1, &EBO);

  Gl->BindVertexArray(VAO);
  
  Gl->BindBuffer(GL_ARRAY_BUFFER, VBO);
  Gl->BufferData(GL_ARRAY_BUFFER, sizeof(Verts), Verts, GL_STATIC_DRAW);

  Gl->VertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(r32), (void*)0);
  Gl->EnableVertexAttribArray(0);

  Gl->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  Gl->BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Idxs), Idxs, GL_STATIC_DRAW);

  r32 t = 0;

  while (!Wnd->Finish) {
    WndBeginFrame(Wnd);

    WndBeginRendering(Wnd);
      // Clear.
      Gl->ClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      Gl->Clear(GL_COLOR_BUFFER_BIT);

      // Draw the rectangle.
      Gl->UseProgram(ShaderProgram);
      Gl->Uniform4f(Gl->GetUniformLocation(ShaderProgram, "Color"), R32Sin(2*t)/2.0f + 0.5f, 0.5f, 0.2f, 1.0f);
      Gl->BindVertexArray(VAO);
      Gl->DrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    WndEndRendering(Wnd);

    t += Wnd->FrameDelta;

    WndEndFrame(Wnd);
  }

  // Arrays and buffers.
  Gl->DeleteVertexArrays(1, &VAO);
  Gl->DeleteBuffers     (1, &VBO);
  Gl->DeleteBuffers     (1, &EBO);

  // Delete program.
  Gl->DeleteProgram(ShaderProgram);

  WndEnd(Wnd);
  SysEnd();
}
