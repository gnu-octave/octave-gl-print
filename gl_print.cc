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

DEFUN_DLD(gl_print, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {Loadable Function} gl_print (@var{h}, @var{filename}, @var{term})\n\
Print figure @var{h} using OSMesa and gl2ps.\n\
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
  int nargin = args.length ();

  if (nargin != 3)
    {
      print_usage();
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

      std::string filename = args(1).string_value ();
      if (error_state)
        {
          error ("Filename has to be a string");
          return retval;
        }

      std::string term = args(2).string_value ();
      if (error_state)
        {
          error ("Term has to be a string");
          return retval;
        }

      octave_stdout << "Width=" << Width
                    << " Height=" << Height
                    << " Filename=" << filename
                    << " Term=" << term << std::endl;

      OSMesaContext ctx;
      void *buffer;

      /* Create an RGBA-mode context */
      #if OSMESA_MAJOR_VERSION * 100 + OSMESA_MINOR_VERSION >= 305
      /* specify Z, stencil, accum sizes */
      ctx = OSMesaCreateContextExt( OSMESA_RGBA, 16, 0, 0, NULL );
      #else
      ctx = OSMesaCreateContext( OSMESA_RGBA, NULL );
      #endif
      if (! ctx)
        {
          printf("OSMesaCreateContext failed!\n");
          return retval;
        }

      /* Allocate the image buffer */
      buffer = malloc( Width * Height * 4 * sizeof(GLubyte) );
      if (! buffer)
        {
          printf("Alloc image buffer failed!\n");
          return retval;
        }

      /* Bind the buffer to the context and make it current */
      if (! OSMesaMakeCurrent( ctx, buffer, GL_UNSIGNED_BYTE, Width, Height ))
        {
          printf("OSMesaMakeCurrent failed!\n");
          return retval;
        }

      int z, s, a;
      glGetIntegerv(GL_DEPTH_BITS, &z);
      glGetIntegerv(GL_STENCIL_BITS, &s);
      glGetIntegerv(GL_ACCUM_RED_BITS, &a);
      printf("Depth=%d Stencil=%d Accum=%d\n", z, s, a);

      FILE *filep;
      filep = fopen (filename.c_str () ,"w");
      if (filep)
        {
          glps_renderer rend (filep, term);
          rend.draw (fobj, "");
          fclose (filep);
        }
      else
        error ("Couldn't create file \"%s\"", filename.c_str ());

      /* free the image buffer */
      free( buffer );

      /* destroy the context */
      OSMesaDestroyContext( ctx );
    }
  else
    error ("H has to be a valid figure handle");

  return retval;
}
