#include "opengl_renderer_api.h"
// opengl_renderer_api

internal void renderer_api_clear(int x, int y, int width, int height)
{
    glViewport(x, y, width, height);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

internal void renderer_api_draw_elements(const Vertex_Array* vertex_array, uint32_t idx_count)
{
    vertex_array_bind(*vertex_array);

    glCheckError(glDrawElements(GL_TRIANGLES, idx_count, GL_UNSIGNED_INT, NULL));
}

internal void renderer_api_set_line_width(float line_width)
{
    glCheckError(glLineWidth(line_width));
}

internal void renderer_api_draw_lines(const Vertex_Array* vertex_array, uint32_t idx_count)
{
    vertex_array_bind(*vertex_array);

    // NOTE: temp work around to draw lines on top
    glDisable(GL_DEPTH_TEST);
    glCheckError(glDrawArrays(GL_LINES, 0, idx_count));
    glEnable(GL_DEPTH_TEST);
}

internal void renderer_api_enable_scissor()
{
    glEnable(GL_SCISSOR_TEST);
}

internal void renderer_api_disable_scissor()
{
    glDisable(GL_SCISSOR_TEST);
}

internal void renderer_api_scissor(int x, int y, int width, int height)
{
    glScissor(x, y, width, height);
}
