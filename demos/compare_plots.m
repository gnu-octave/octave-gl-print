## Make some plots with native graphics_toolkit fltk,
## graphics_toolkit gnuplot and gl_print using OSMesa

close all
addpath ( canonicalize_file_name ("../"));
graphics_toolkit fltk
sombrero
tic
print ("-S800,600", "sombrero_fltk.png")
print ("-S800,600", "sombrero_fltk.svg")
toc

close all
graphics_toolkit gnuplot
sombrero
tic
print ("-S800,600", "sombrero_gnuplot.png")
print ("-S800,600", "sombrero_gnuplot.svg")
toc

close all
figure ("visible", "off", "position", [0 0 800 600])
sombrero
tic
gl_print ("sombrero_osmesa.png")
gl_print ("sombrero_osmesa_gl2ps.svg")
toc





