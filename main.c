#define RENDER_IMPL
#include "render.h"

#define FOUNDATION_IMPL
#include "foundation.h"

int main(int Argc, char **Argv) {
  SysInit(Argc, Argv);

  table(i32) Ints = {0};
  i32 N = 1000;
  ItrNum (i, N)
    TableAdd(&Ints, _U64Hash(256*(i+1)), 42+i);
  ItrNum (i, N)
    printf("%d\n", TableGet(&Ints, _U64Hash(256*(i+1))));
  
  printf("finished\n");

  SysEnd();
}
