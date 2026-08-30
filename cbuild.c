#define CBUILD_IMPLEMENTATION
#include "cbuild.h"

#define IncludeDirs "/I\"vendor\\GLFW\\include\" /I\"vendor\\glad\\include\" /I\"vendor\\cglm\\include\" /I\"vendor\\stb_image\""
#define CommonCompilerFlags "/DGLFW_STATIC /W3 /wd4996 /Zi /MT /nologo"
#define Files "\"src\\windows_buoyantui.c\" \"vendor\\glad\\src\\gl.c\""
#define LibPaths "/LIBPATH:\"vendor\\GLFW\\lib-vc2022\""
#define Libs "glfw3_mt.lib user32.lib gdi32.lib shell32.lib"

int main(int argc, char** argv)
{
    CBUILD_REBUILD_SELF(argc, argv);

    CBuild_Cmd cmd = {0};

    cbuild_cmd_begin(&cmd, CBUILD_COMPILER_CL);

    cbuild_cmd_append(&cmd, IncludeDirs);
    cbuild_cmd_append(&cmd, CommonCompilerFlags);
    cbuild_cmd_append(&cmd, Files);
    cbuild_cmd_append(&cmd, "/Fo:build\\");
    cbuild_cmd_append(&cmd, "/Fe:build\\windows_buoyantui.exe");
    cbuild_cmd_append(&cmd, "/link");
    cbuild_cmd_append(&cmd, "/DEBUG");
    cbuild_cmd_append(&cmd, LibPaths);
    cbuild_cmd_append(&cmd, Libs);

    cbuild_cmd_end(&cmd);
}
