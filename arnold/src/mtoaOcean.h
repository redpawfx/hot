#include "Ocean.h"

class mtoaOcean
{
    public: 
	mtoaOcean();

    virtual ~mtoaOcean();

	 // This is where all the wave action takes place
    drw::Ocean        *_ocean;
    drw::OceanContext *_ocean_context;
    float              _normalize_factor;
    
};

class mtoaOceanFoam
{
    public: 
	mtoaOceanFoam();

    virtual ~mtoaOceanFoam();

	 // This is where all the wave action takes place
    drw::Ocean        *_ocean;
    drw::OceanContext *_ocean_context;
    float              _normalize_factor;
    
};
