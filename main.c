#include "foundation.h"

int main(int Argc, char **Argv) {
  SysInit(Argc, Argv);

  window *Window = SysCreateWindowWithOpenGL();

  r32 Red = 0;

  while (!Window->Quit) {
    SysGetInput(Window);
    SysBeginRenderingWithOpengl(Window);
    Window->GlClearColor(Red, 0, 0, 1);
    Window->GlClear(GL_COLOR_BUFFER_BIT);
    SysEndRenderingWithOpengl(Window);
    Red += 0.05f;
    if (Red >= 0.9f)
      Red = 0;
  }

  SysEnd();
}
