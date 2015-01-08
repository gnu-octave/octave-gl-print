octave-gl-print
===============

Offscreen rendering print using OSMesa.
gl2ps is used for vector formats EPS, PS, PDF, SVG, PGF and TEX.

## Dependencies

The minimum required version for OSMesa is 9.0.

OSMesa < 9.0 shows a strage bug where the return values of
```
glGetIntegerv (GL_DEPTH_BITS, &z);
glGetIntegerv (GL_STENCIL_BITS, &s);
glGetIntegerv (GL_ACCUM_RED_BITS, &a);
```
return arbitrary values.

### Install OSMesa via apt-get / aptitude

tested with
- Debian GNU/Linux 8.0 (jessie)
- Ubuntu 14.04 (trusty)

```
sudo apt-get update
sudo apt-get install libosmesa6-dev
```

### Build OSMesa from source

Needed for Debian wheezy which shipps OSMesa 8.0.5.

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
```

```
make check    #optional
sudo make install
```

## Build gl-print and run:

```
./bootstrap
./configure
make check
```

### Mesa Demos

I found the Mesa demos (especially the OSMesa demos in ./src/osdemos/) helpful:

```
git clone git://anongit.freedesktop.org/mesa/demos mesa_demos
cd mesa_demos
./autogen.sh

[if you've built from source perhaps
"export PKG_CONFIG_PATH=/usr/local/mesa/10.2.2/classic/lib/pkgconfig/"]

./configure --enable-osmesa

### Links
- Build instruction for OSMesa and benchmarks: http://www.paraview.org/Wiki/ParaView/ParaView_And_Mesa_3D
