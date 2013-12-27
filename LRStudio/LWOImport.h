/*
 *	LWO2 Importer
 *  outputs .flm+.lrmh+.lrmt+.lrtx files
 */

#ifndef _LWO_IMPORT_H_INCLUDED_
#define _LWO_IMPORT_H_INCLUDED_

struct LWO2ImportDescStruct {
	//input file name (full path)
	char szFileName[256];
	char szOutputRootDir[256];
	//_base_ name of output .flm (default is the same as LWO filename)
	char szOutputFLMBaseName[256];
	//output options
	BOOL bOutputGeometry;
	BOOL bOutputMesh;
	BOOL bOutputMaterial;
	BOOL bOutputTextures;
	//name of shader to use for this mesh
//	char szShaderName[256];
	//overwrite/update options
	//what to do if output file already exists
	BOOL bOverwriteGeometry;
	BOOL bOverwriteMesh;
	BOOL bOverwriteMaterial;
	BOOL bOverwriteTextures;
	BOOL bOverwriteImageFiles;
	//mesh output options (standard LR mesh options)
	BOOL bDeformers;
	BOOL bSpaceWarps;
	BOOL bRenderable;
	DWORD dwLockabilityMask;
	BOOL bNPatches;
	//importer opions
	BOOL bMakeTangentSpace;
};

#define MAX_MESH_SURFACES 128

struct LWO2ImportResult {
	UINT uiMeshCount;
	char MeshNameTbl[MAX_MESH_SURFACES][256];
};

//returns TRUE for success
extern BOOL importLWO2File(LWO2ImportDescStruct& rfImportDesc,LWO2ImportResult *pResult);

#endif