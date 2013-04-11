The shader is based on Prof. Jerry Tessendorf's implementation for ocean waves, 
that found at HOT (Houdini Ocean Toolkit).

The code depends on external libraries that can be downloaded first,
those needs to be placed in 'src/3rdparty'
those libraries are: blitz, fftw3 and loki

Compile:
$ g++ -o bs_Ocean.os -c -fPIC -D_LINUX -I. -I$ARNOLDTREE/include -I/usr/include/ -I3rdparty/include/ -I3rdparty/src/blitz-0.9/ -m64 -O3 -pthread -pipe -fPIC bs_Ocean.cpp

Link:
$g++ -o bs_Ocean.so -shared bs_Ocean.os -L$ARNOLDTREE/bin -L/usr/lib -L/usr/lib64 -L/usr/local/arnold/bin -L/usr/autodesk/maya2012-x64/lib -lfftw3f -lm -lai

Install:
copy bs_Ocean.so and bs_Ocean.mtd to ArnoldShaderPath (i.e, /usr/local/mtoa/shaders/)

*Mods by John C.  (redpawFX) 
	Modified to put both shaders into one compile, added  fairly generic linux makeFile and fixed some  naming conflicts.

