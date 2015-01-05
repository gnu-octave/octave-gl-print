figure ("visible", "off");
sombrero;
gl_print ("sombrero.svg")'
gl_print ("sombrero.png")'

plot (sin (0:0.1:2*pi));
gl_print (gcf, "sin_simple.svg", true)
gl_print (gcf, "sin_simple.png", true)

## check size of generated files
assert (stat ("sombrero.svg").size, 2692270, 10)
assert (stat ("sombrero.png").size, 58686, 10)
assert (stat ("sin_simple.svg").size, 7438, 10)
assert (stat ("sin_simple.png").size, 9378, 10)
