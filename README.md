octave-gl-print
===============

Printing routine using OSMesa for offscreen rendering.
For printing to vector image formats EPS, PS, PDF, SVG, PGF and TEX
gl2ps (http://www.geuz.org/gl2ps/) is used.

## Dependencies
The minimum required version of OSMesa is 9.0.

OSMesa versions < 9.0 show a strange bug where the return values of
```
glGetIntegerv (GL_DEPTH_BITS, &z);
glGetIntegerv (GL_STENCIL_BITS, &s);
glGetIntegerv (GL_ACCUM_RED_BITS, &a);
```
contain arbitrary values and the rendered buffer is garbage.

### Install OSMesa via apt-get / aptitude
tested with
- Debian GNU/Linux 8.0 (jessie)
- Ubuntu 14.04 (trusty)

```
sudo apt-get update
sudo apt-get install libosmesa6-dev
```

### Build OSMesa from source
Needed for Debian wheezy which ships OSMesa 8.0.5.

```
wget -nc ftp://ftp.freedesktop.org/pub/mesa/current/MesaLib-10.2.2.tar.bz2
tar xjf MesaLib-10.2.2.tar.bz2
cd Mesa-10.2.2/

autoreconf -fi

./configure \
    CXXFLAGS="-O2 -g -DDEFAULT_SOFTWARE_DEPTH_BITS=31" \
    CFLAGS="-O2 -g -DDEFAULT_SOFTWARE_DEPTH_BITS=31" \
    --disable-xvmc \
    --disable-glx \
    --disable-dri \
    --with-dri-drivers="" \
    --with-gallium-drivers="" \
    --enable-texture-float \
    --disable-shared-glapi \
    --disable-egl \
    --with-egl-platforms="" \
    --enable-osmesa \
    --enable-gallium-llvm=no \
    --prefix=/usr/local/mesa/10.2.2/classic

make -j4
#optional
make check
sudo make install
```

## Build gl-print and run check:
```
./bootstrap
./configure
make check
```

## Use gl_print

Have a look at
```
octave:1> help gl_print
```

### Example
```
plot (sin (0:0.1:2*pi));
gl_print ("sin.png");
gl_print ("sin.pdf");
```

## Mesa Demos
I found the Mesa demos (especially the OSMesa demos in ./src/osdemos/) helpful:

```
git clone git://anongit.freedesktop.org/mesa/demos mesa_demos
cd mesa_demos
./autogen.sh

[if you've built from source perhaps
"export PKG_CONFIG_PATH=/usr/local/mesa/10.2.2/classic/lib/pkgconfig/"]

./configure --enable-osmesa
```

## Links
- Build instruction for OSMesa and benchmarks: http://www.paraview.org/Wiki/ParaView/ParaView_And_Mesa_3D
