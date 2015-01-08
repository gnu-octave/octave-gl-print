## Copyright (C) 2015 Andreas Weber <andy.weber.aw@gmail.com>
##
## This program is free software; you can redistribute it and/or modify it under
## the terms of the GNU General Public License as published by the Free Software
## Foundation; either version 3 of the License, or (at your option) any later
## version.
##
## This program is distributed in the hope that it will be useful, but WITHOUT
## ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
## FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
## details.
##
## You should have received a copy of the GNU General Public License along with
## this program; if not, see <http:##www.gnu.org/licenses/>.

## -*- texinfo -*-
## @deftypefn  {Function File} {} gl_print (@var{filename})
## @deftypefnx {Function File} {} gl_print (@var{h}, @var{filename})
## @deftypefnx {Function File} {} gl_print (@var{h}, @var{filename}, @var{simple_sort})
## Save a plot to a file using OSMesa.
##
## Vector formats like SVG, PDF, PS and so on are created with gl2ps.
## Bitmap formats like PNG, JPG, BMP ... do not use gl2ps.
##
## If @var{simple_sort} is true, GL2PS_SIMPLE_SORT is used instead of
## GL2PS_BSP_SORT as Z-depth sorting algorithm.
##
## @var{filename} defines the name of the output file.  If the
## file name has no suffix, one is inferred from the specified
## device and appended to the file name.  If no filename is
## specified, the output is sent to the printer.
##
## @var{h} specifies the handle of the figure to print.  If no handle is
## specified the current figure is used.
##
## @seealso{print, saveas, hgsave, orient, figure}
## @end deftypefn

function gl_print (varargin)

  h = get (0, "currentfigure");
  simple_sort = false;
  if (nargin == 1)
    filename = varargin{1};
  elseif (nargin == 2)
    h = varargin{1};
    filename = varargin{2};
  elseif (nargin == 3)
    h = varargin{1};
    filename = varargin{2};
    simple_sort = varargin{3};
  else
    print_usage ();
  endif

  if (! isfigure (h) || isempty (h))
    error ("no valid figure");
  endif

  [~, ~, EXT] = fileparts (filename);

  gl2ps_list = {".eps", ".pdf", ".ps", ".svg", ".pgf", ".tex"};
  if (any (strcmpi (gl2ps_list, EXT)))
    term = EXT(2:end);
    if (simple_sort)
      term = strcat (term, "is2D");
    endif
    __gl_print__ (h, filename, term);
  else
    img = __gl_print__ (h);
    imwrite (img, filename);
  endif

endfunction
