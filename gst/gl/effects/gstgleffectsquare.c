/*
 * GStreamer
 * Copyright (C) 2008 Filippo Argiolas <filippo.argiolas@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include <gstgleffects.h>

static void
gst_gl_effects_square_callback (gint width, gint height, guint texture,
    gpointer data)
{
  GstGLShader *shader;
  GstGLEffects *effects = GST_GL_EFFECTS (data);
  GstGLDisplay *display = GST_GL_FILTER (effects)->display;
  GstGLFuncs *gl = display->gl_vtable;

  shader = g_hash_table_lookup (effects->shaderstable, "square0");

  if (!shader) {
    shader = gst_gl_shader_new (display);
    g_hash_table_insert (effects->shaderstable, "square0", shader);
  }

  if (!gst_gl_shader_compile_and_check (shader,
          square_fragment_source, GST_GL_SHADER_FRAGMENT_SOURCE)) {
    gst_gl_display_set_error (display, "Failed to initialize square shader");
    GST_ELEMENT_ERROR (effects, RESOURCE, NOT_FOUND,
        GST_GL_DISPLAY_ERR_MSG (display), (NULL));
    return;
  }

  gl->MatrixMode (GL_PROJECTION);
  gl->LoadIdentity ();

  gst_gl_shader_use (shader);

  gl->ActiveTexture (GL_TEXTURE0);
  gl->Enable (GL_TEXTURE_RECTANGLE_ARB);
  gl->BindTexture (GL_TEXTURE_RECTANGLE_ARB, texture);

  gst_gl_shader_set_uniform_1i (shader, "tex", 0);

  gst_gl_shader_set_uniform_1f (shader, "width", (gfloat) width / 2.0f);
  gst_gl_shader_set_uniform_1f (shader, "height", (gfloat) height / 2.0f);

  gst_gl_effects_draw_texture (effects, texture, width, height);
}

void
gst_gl_effects_square (GstGLEffects * effects)
{
  GstGLFilter *filter = GST_GL_FILTER (effects);

  gst_gl_filter_render_to_target (filter, TRUE, effects->intexture,
      effects->outtexture, gst_gl_effects_square_callback, effects);
}
