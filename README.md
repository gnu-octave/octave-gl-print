octave-gl-print
===============

Offscreen rendering print using OSMesa and gl2ps

I build this project on Debian GNU/Linux 8.0 (jessie) thus the instructions below may differ for your platform.

### Install dependencies with apt-get / aptitude

```
sudo apt-get update
sudo apt-get install libosmesa6-dev
```

### Build gl-print and run:

```
./bootstrap
./configure
make check
```

have a look at check1.svg!

### Build OSMesa from source

usefull link with speed comparison:
http://www.paraview.org/Wiki/ParaView/ParaView_And_Mesa_3D

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

perhaps "make check" if you want

    sudo make install

### Mesa Demos

I found the Mesa demos (especially the OSMesa demos in ./src/osdemos/) helpful:

```
git clone git://anongit.freedesktop.org/mesa/demos mesa_demos
cd mesa_demos
./autogen.sh

[if you've built from source perhaps
"export PKG_CONFIG_PATH=/usr/local/mesa/10.2.2/classic/lib/pkgconfig/"]

./configure --enable-osmesa
