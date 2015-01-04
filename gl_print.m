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
## Save a plot to a file using OSMesa, gl2ps and ghostscript for bitmap output.
##
## If @var{simple_sort} is true, GL2PS_SIMPLE_SORT is used instead of
## GL2PS_BSP_SORT as Z-depth sorting algorithm.
##
## Both output formatted for printing (PDF and PostScript), and many bitmapped
## and vector image formats are supported.
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
  endif

  if (! isfigure (h) || isempty (h))
    error ("no valid figure");
  endif

  [~, ~, EXT] = fileparts (filename);

  direct_list = {".eps", ".pdf", ".ps", ".svg", ".pgf", ".tex"};
  if (any (strcmpi (direct_list, EXT)))
    cmd = strcat ("cat > ", filename);
    term = EXT(2:end);
  elseif (strcmpi (".png", EXT))
    cmd = sprintf ("gs -dQUIET -dNOPAUSE -dBATCH -dSAFER -sDEVICE=png16m -dTextAlphaBits=4 -dGraphicsAlphaBits=4 -r150x150 -dEPSCrop -sOutputFile=\"%s\" -", filename);
    term = "eps";
  elseif (strcmpi (".jpg", EXT))
    cmd = sprintf ("gs -dQUIET -dNOPAUSE -dBATCH -dSAFER -sDEVICE=jpeg -dTextAlphaBits=4 -dGraphicsAlphaBits=4 -r150x150 -dEPSCrop -sOutputFile=\"%s\" -", filename);
    term = "eps";
  else
    error ("Unknown extension \"%s\"", EXT);
  endif

  if (simple_sort)
    term = strcat (term, "is2D");
  endif

  __gl_print__ (h, cmd, term);

endfunction
