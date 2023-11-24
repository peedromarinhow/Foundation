// vertex shader
#version 450 core
layout (location = 0) in vec2 Pos;
uniform vec2 WndDim;
out vec2 uv;
void main() {
  uv = vec2(((gl_VertexID+1)%3) == 0, ((gl_VertexID+2)%3) == 0);
  gl_Position = vec4((Pos*2)/WndDim - 1, 0.0, 1.0);
}

$
// fragment shader
#version 450 core
in vec2  uv;
out vec4 FragColor;
void main() {
  vec2  TexCoord = uv.x * vec2(0.5, 0.0) + uv.y * vec2(1.0, 1.0);
  float IsInside = float(sign(TexCoord.x * TexCoord.x - TexCoord.y) < 0.0);
  FragColor      = vec4(1.0f, 0.5f, 0.2f, IsInside);
}
