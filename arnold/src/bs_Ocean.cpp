/*
 * Belal Salem Ocean Shader
 * belal@nothing-real.com
 * www.nothing-real.com
 * 
 * Compile: g++ -o arnold_Ocean.os -c -fPIC -D_LINUX -I. -I$ARNOLDTREE/include -I/usr/include/ \
 *              -I3rdparty/include/ -I3rdparty/src/blitz-0.9/  arnold_Ocean.cpp
 *              
 * Link:   g++ -o arnold_Ocean.so -shared arnold_Ocean.os -L$ARNOLDTREE/bin -L/usr/lib -L/usr/lib64 -L/usr/local/arnold/bin -lfftw3f -lm -lai
 */

#include <ai.h>
#include <string.h>
#include "math.h"
#include <iostream>

#include "mtoaOcean.h"

#define _globalScale     (params[0].FLT)
#define _resolution      (params[1].INT) 
#define _size            (params[2].FLT)
#define _windSpeed       (params[3].FLT)
#define _waveHeight      (params[4].FLT)
#define _shortestWave    (params[5].FLT)
#define _choppiness      (params[6].FLT)
#define _windDirection   (params[7].FLT)
#define _dampReflections (params[8].FLT)
#define _windAlign       (params[9].FLT)
#define _oceanDepth      (params[10].FLT)
#define _time            (params[11].FLT)
#define _seed            (params[12].INT)
#define _inDisplacement  (params[13].VEC)

AI_SHADER_NODE_EXPORT_METHODS(bs_OceanMtd);


node_parameters
{
    AiParameterFLT ("globalScale"   , 1.0);
    AiParameterInt ("resolution"    , 8);	// grid aufloesung
    AiParameterFLT ("size"          , 200);     // The grid mentiond above is computed for and applied to the input geometry in tiles of this size.
    AiParameterFLT ("windSpeed"     , 30);      // Wind Speed - Affects the shape of the waves, "Windspeed (m/s)"
    AiParameterFLT ("waveHeight"    , 1);
    AiParameterFLT ("shortestWave"  , 0.02);    // Shortest Wavelength(m)
    AiParameterFLT ("choppiness"    , 0.8);
    AiParameterFLT ("windDirection" , 0);       // Wind direction in degrees
    AiParameterFLT ("dampReflections", 0.5);    // Damp reflections - In a fully developed ocean you will have waves travelling in both the forward and backwards directions. This parameter damps out the negative direcion waves.
    AiParameterFLT ("windAlign"     , 2.0);     // Wind Alignment - Controls how closely the waves travel in the direction of the wind.
    AiParameterFLT ("oceanDepth"    , 200);     // Ocean Depth - Affects the spectrum of waves generated. Visually in doesn't seem to have that great an influence.
    AiParameterFLT ("time"          , 0);
    AiParameterInt ("seed"          , 0);       // Seed - Seeds the random number generator.
    AiParameterVec ("inDisplacement", 0, 0, 0); // an input for layering multiple displacements

	AiMetaDataSetStr(mds, NULL, "maya.classification", "shader/surface");
    AiMetaDataSetStr(mds, NULL, "maya.name", "BS_Ocean");
    AiMetaDataSetInt(mds, NULL, "maya.id", 0x001139998);
	AiMetaDataSetStr(mds, NULL, "maya.output_name", "displacement");
	AiMetaDataSetStr(mds, NULL, "maya.output_shortname", "disp");

}

mtoaOcean *oc;

mtoaOcean::mtoaOcean() 
{ 
    _ocean = 0;
    _ocean_context = 0;
    _normalize_factor = 1.0f;
}

mtoaOcean::~mtoaOcean()
{
    if (_ocean)
    {
        delete _ocean;
    }
    if (_ocean_context)
    {
        delete _ocean_context;
    }
}

bool SetRefererencePoints(AtShaderGlobals *sg, AtPoint &tmpPts)
{
   AtPoint Pref;
   bool usePref = AiUDataGetPnt("Pref",&Pref);
   if (usePref)
   {
      tmpPts = sg->P;
      sg->P = Pref;
      return true;
   }
   return false;
}

void RestorePoints(AtShaderGlobals *sg, AtPoint tmpPts)
{
   sg->P = tmpPts;
}

shader_evaluate
{
    AtParamValue *params = AiNodeGetParams(node);
         
    AtPoint P;

	AtPoint tmpPts;
   	bool usePref = SetRefererencePoints(sg, tmpPts);
    
    AtVector disp;
    float jminus;
    float jplus;
    AtVector eminus;
    AtVector eplus;
    
    P = sg->P;

    drw::EvalData evaldata;
    oc->_ocean_context->eval2_xz((1.0/_globalScale)*P.x, (1.0/_globalScale)*P.z, evaldata);
    
    disp.x = evaldata.disp[0] * _globalScale + _inDisplacement.x;
    disp.y = evaldata.disp[1] * _globalScale + _inDisplacement.y;
    disp.z = evaldata.disp[2] * _globalScale + _inDisplacement.z;
    
    sg->out.VEC = disp;
	if (usePref) RestorePoints(sg, tmpPts);
}

node_initialize
{


    oc = new mtoaOcean();
    
    bool do_chop = false;
    if ( _choppiness > 0.0001)
        do_chop = true;
    
    bool do_jacobian = true;
    bool do_normal = true;
    
    int do_buffer = 0;
    if (do_chop)
        do_buffer = 1;

    int res = _resolution;
    res = (int)pow (2.0, res);
    
    oc->_ocean = new drw::Ocean(res, res, _size/float(res), _size/float(res),
                                _windSpeed, _shortestWave, 0.00001f, _windDirection/180.0f * M_PI,
                                1.0f -_dampReflections, _windAlign, _oceanDepth, _seed);
    oc->_normalize_factor = oc->_ocean->get_height_normalize_factor();

    oc->_ocean_context = oc->_ocean->new_context(true, do_chop, do_normal, do_jacobian);
    
    // sum up the waves at this timestep
    oc->_ocean->update( _time, *oc->_ocean_context, true, do_chop, do_normal, do_jacobian, oc->_normalize_factor * _waveHeight, _choppiness);

}

node_update
{
   
}


node_finish
{

    if (oc)
        delete oc;
}


