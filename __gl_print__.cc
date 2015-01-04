// Copyright (C) 2015 Andreas Weber <andy.weber.aw@gmail.com>
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 3 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, see <http://www.gnu.org/licenses/>.

#include <octave/oct.h>
#include <sys/types.h>
#include "sysdep.h"
#include <string>
#include "GL/osmesa.h"

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#endif

#include "gl-render.h"
#include "gl2ps-renderer.h"
#include "graphics.h"

DEFUN_DLD(__gl_print__, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {Loadable Function} __gl_print__ (@var{h}, @var{cmd}, @var{term})\n\
Print figure @var{h} using OSMesa and gl2ps.\n\
\n\
@var{cmd} might be a pipe to ghostscript or simply @qcode{cat > filename}\n\
\n\
Valid options for @var{term}, which can be concatenated in one string, are:\n\
\n\
@table @asis\n\
@item @qcode{eps}, @qcode{pdf}, @qcode{ps}, @qcode{svg}, @qcode{pgf}, @qcode{tex}\n\
Select output format\n\
@item @qcode{is2D}\n\
Use GL2PS_SIMPLE_SORT instead of GL2PS_BSP_SORT as Z-depth sorting algorithm\n\
@item @qcode{notext}\n\
Don't render text\n\
@end table\n\
@end deftypefn")
{
  octave_value_list retval;
#ifdef HAVE_GL2PS_H
  int nargin = args.length ();

  if (nargin != 3)
    {
      print_usage ();
      return retval;
    }

  if (! (args(1).is_string () && args(2).is_string ()))
    {
      error ("CMD and TERM has to be strings");
      return retval;
    }

  int h = args(0).double_value ();
  graphics_object fobj = gh_manager::get_object (h);
  if (fobj &&  fobj.isa ("figure"))
    {


      figure::properties& fp =
        dynamic_cast<figure::properties&> (fobj.get_properties ());

      bool internal = true;
      Matrix bb = fp.get_boundingbox (internal);

      int Width = bb(2);
      int Height = bb(3);

      std::string cmd  = args(1).string_value ();
      std::string term = args(2).string_value ();

      octave_stdout << "Width=" << Width
                    << " Height=" << Height
                    << " Cmd=" << cmd
                    << " Term=" << term << std::endl;

      OSMesaContext ctx;
      void *buffer;

      // Create an RGBA-mode context
      #if OSMESA_MAJOR_VERSION * 100 + OSMESA_MINOR_VERSION >= 305
      // specify Z, stencil, accum sizes
      ctx = OSMesaCreateContextExt( OSMESA_RGBA, 16, 0, 0, NULL );
      #else
      ctx = OSMesaCreateContext( OSMESA_RGBA, NULL );
      #endif
      if (! ctx)
        {
          printf("OSMesaCreateContext failed!\n");
          return retval;
        }

      // Allocate the image buffer
      buffer = malloc (Width * Height * 4 * sizeof (GLubyte));
      if (! buffer)
        {
          printf ("Alloc image buffer failed!\n");
          return retval;
        }

      // Bind the buffer to the context and make it current
      if (! OSMesaMakeCurrent (ctx, buffer, GL_UNSIGNED_BYTE, Width, Height))
        {
          printf ("OSMesaMakeCurrent failed!\n");
          return retval;
        }

      int z, s, a;
      glGetIntegerv (GL_DEPTH_BITS, &z);
      glGetIntegerv (GL_STENCIL_BITS, &s);
      glGetIntegerv (GL_ACCUM_RED_BITS, &a);
      printf ("Depth=%d Stencil=%d Accum=%d\n", z, s, a);

      FILE *filep;
      filep = octave_popen (cmd.c_str (), "w");
      if (filep)
        {
          // check if the figure is visible
          bool v = fp.is_visible ();

          //if (v)
            //fp.set_visible ("off");

          glps_renderer rend (filep, term);

          //glMatrixMode (GL_PROJECTION);
          //glLoadIdentity ();
          //rend.set_viewport (Width, Height);

          rend.draw (fobj, cmd);
          octave_pclose (filep);

          // HACK for FLTK:
          // toggle figure visibility
          // If the figure previously was shown with FLTK, this causes
          // an update with ID = base_properties::ID_VISIBLE
          // which calls
          // -> do_toggle_window_visibility
          // -> show_canvas
          // -> canvas.make_current ()
          // which selects the FLTK OpenGL context again.
          // This is important because OSMesaMakeCurrent above changed the current context.
          if (v)
            {
              fp.set_visible ("off");
              fp.set_visible ("on");
              //rend.set_viewport (Width, Height);
            }
        }
      else
        error ("Couldn't create pipe to \"%s\"", cmd.c_str ());

      // free the image buffer
      free (buffer);

      // destroy the context
      OSMesaDestroyContext ( ctx );
    }
  else
    error ("H has to be a valid figure handle");

#else
  error ("Octave has been compiled without gl2ps. gl_print won't work without gl2ps.");
#endif
  return retval;
}
