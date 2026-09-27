#ifndef OPENGL_RENDERER_API_H
#define OPENGL_RENDERER_API_H

internal void renderer_api_clear(int x, int y, int width, int height);

internal void renderer_api_draw_elements(const Vertex_Array* vertex_array, uint32_t idx_count);

internal void renderer_api_set_line_width(float line_width);
internal void renderer_api_draw_lines(const Vertex_Array* vertex_array, uint32_t idx_count);

internal void renderer_api_enable_depth_test();
internal void renderer_api_disable_depth_test();
internal void renderer_api_enable_scissor();
internal void renderer_api_disable_scissor();
internal void renderer_api_scissor(int x, int y, int width, int height);

#endif OPENGL_RENDERER_API_H
