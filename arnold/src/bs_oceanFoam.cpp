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

#define _foamThreshold   (params[0].FLT)
#define _foamOffset      (params[1].FLT)
#define _globalScale     (params[2].FLT)
#define _resolution      (params[3].INT) 
#define _size            (params[4].FLT)
#define _windSpeed       (params[5].FLT)
#define _waveHeight      (params[6].FLT)
#define _shortestWave    (params[7].FLT)
#define _choppiness      (params[8].FLT)
#define _windDirection   (params[9].FLT)
#define _dampReflections (params[10].FLT)
#define _windAlign       (params[11].FLT)
#define _oceanDepth      (params[12].FLT)
#define _time            (params[13].FLT)
#define _seed            (params[14].INT)


AI_SHADER_NODE_EXPORT_METHODS(bs_oceanFoamMtd);

node_parameters
{
    AiParameterFLT ("foamThreshold"  , 1.5);
    AiParameterFLT ("foamOffset"     , 0.0);
    AiParameterFLT ("globalScale"    , 1.0);
    AiParameterInt ("resolution"     , 8);	// grid aufloesung
    AiParameterFLT ("size"           , 200);     // The grid mentiond above is computed for and applied to the input geometry in tiles of this size.
    AiParameterFLT ("windSpeed"      , 30);      // Wind Speed - Affects the shape of the waves, "Windspeed (m/s)"
    AiParameterFLT ("waveHeight"     , 1);
    AiParameterFLT ("shortestWave"   , 0.02);    // Shortest Wavelength(m)
    AiParameterFLT ("choppiness"     , 0.8);
    AiParameterFLT ("windDirection"  , 0);       // Wind direction in degrees
    AiParameterFLT ("dampReflections", 0.5);    // Damp reflections - In a fully developed ocean you will have waves travelling in both the forward and backwards directions. This parameter damps out the negative direcion waves.
    AiParameterFLT ("windAlign"      , 2.0);     // Wind Alignment - Controls how closely the waves travel in the direction of the wind.
    AiParameterFLT ("oceanDepth"     , 200);     // Ocean Depth - Affects the spectrum of waves generated. Visually in doesn't seem to have that great an influence.
    AiParameterFLT ("time"           , 0);
    AiParameterInt ("seed"           , 0);       // Seed - Seeds the random number generator.

	AiMetaDataSetStr(mds, NULL, "maya.classification", "shader/surface");
    AiMetaDataSetStr(mds, NULL, "maya.name", "BS_OceanFoamMask");
    AiMetaDataSetInt(mds, NULL, "maya.id", 0x001139999);
	AiMetaDataSetStr(mds, NULL, "maya.output_name", "foamMask");
	AiMetaDataSetStr(mds, NULL, "maya.output_shortname", "foam");

}

mtoaOceanFoam *ocf;

mtoaOceanFoam::mtoaOceanFoam() 
{ 
    _ocean = 0;
    _ocean_context = 0;
    _normalize_factor = 1.0f;
}

mtoaOceanFoam::~mtoaOceanFoam()
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

shader_evaluate
{
    AtParamValue *params = AiNodeGetParams(node);
        
        
    AtPoint Pdisp, Porig;
    
    float foamMask;
    AtVector eplus, eminus;
   
    
    Porig = sg->P;

    drw::EvalData evaldata;
    ocf->_ocean_context->eval2_xz((1.0/_globalScale)*Porig.x, (1.0/_globalScale)*Porig.z, evaldata);
    
    Pdisp.x = Porig.x - evaldata.disp[0];
    Pdisp.y = Porig.y - evaldata.disp[1];
    Pdisp.z = Porig.z - evaldata.disp[2];
    evaldata.Jminus = 0;
    
    ocf->_ocean_context->eval2_xz((1.0/_globalScale)*Pdisp.x, (1.0/_globalScale)*Pdisp.z, evaldata);
    
    foamMask = 0;
    if ( evaldata.Jminus < ((_foamThreshold -1)))
        foamMask = (-1 * evaldata.Jminus) + ((_foamThreshold - 1));
    
    //foamMask = (1 - evaldata.Jminus/(2 * _foamThreshold));
    // foamMask += 0.25 *(float( (evaldata.Eplus - evaldata.Eminus) - (1 - (_foamOffset + 0.5 )) ));
    
    sg->out.FLT = foamMask;
}

node_initialize
{

    ocf = new mtoaOceanFoam();
    
    bool do_chop = false;
    if (_choppiness > 0.0001)
        do_chop = true;
    
    bool do_jacobian = true;
    bool do_normal = true;
    
    int do_buffer = 0;
    if (do_chop)
        do_buffer = 1;

    int res = _resolution;
    res = (int)pow (2.0, res);
    
    ocf->_ocean = new drw::Ocean(res, res, _size/float(res), _size/float(res),
                                _windSpeed, _shortestWave, 0.00001f, _windDirection/180.0f * M_PI,
                                1.0f -_dampReflections, _windAlign, _oceanDepth, _seed);
    ocf->_normalize_factor = ocf->_ocean->get_height_normalize_factor();

    ocf->_ocean_context = ocf->_ocean->new_context(true, do_chop, do_normal, do_jacobian);
    
    // sum up the waves at this timestep
    ocf->_ocean->update( _time, *ocf->_ocean_context, true, do_chop, do_normal, do_jacobian, ocf->_normalize_factor * _waveHeight, _choppiness);

}

node_update
{
   
}


node_finish
{

    if (ocf)
        delete ocf;
}


