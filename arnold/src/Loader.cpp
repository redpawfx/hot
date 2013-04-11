#include <ai.h>
#include <stdio.h>

extern AtNodeMethods* bs_OceanMtd;
extern AtNodeMethods* bs_oceanFoamMtd;


enum SHADERS
{
	SHADER_OCEAN,
	SHADER_OCEAN_FOAM,
};

node_loader
{
   switch (i)
   {
	case SHADER_OCEAN:
		node->methods 	  = (AtNodeMethods*) bs_OceanMtd;
		node->output_type = AI_TYPE_VECTOR;
		node->name		  = "bs_Ocean";
		node->node_type   = AI_NODE_SHADER;
		break;
	case SHADER_OCEAN_FOAM:
		node->methods     = (AtNodeMethods*) bs_oceanFoamMtd;
		node->output_type = AI_TYPE_FLOAT;
		node->name        = "bs_oceanFoam";
		node->node_type   = AI_NODE_SHADER;
		break;
	default:
		return FALSE;
   }

   sprintf(node->version, AI_VERSION);

   return TRUE;
}
