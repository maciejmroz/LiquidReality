/*
 *	Lightwave scene filer importer
 *  outputs .lrsc2 (+all mesh associated files)
 */

#ifndef _LWS_IMPORT_H_INCLUDED_
#define _LWS_IMPORT_H_INCLUDED_

//very smilar to LWOImportDesc but slightly extended and altered
struct LWSImportDescStruct {
	//input file name (full path)
	char szFileName[256];
	char szOutputRootDir[256];
	//output options
	BOOL bOutputGeometry;
	BOOL bOutputMesh;
	BOOL bOutputMaterial;
	BOOL bOutputTextures;
	BOOL bOutputScene;
	//name of shader to use for this mesh
//	char szShaderName[256];
	//overwrite/update options
	//what to do if output file already exists
	BOOL bOverwriteGeometry;
	BOOL bOverwriteMesh;
	BOOL bOverwriteMaterial;
	BOOL bOverwriteTextures;
	BOOL bOverwriteImageFiles;
	BOOL bOverwriteSceneFile;
	//mesh output options (standard LR mesh options)
	BOOL bDeformers;
	BOOL bSpaceWarps;
	BOOL bRenderable;
	DWORD dwLockabilityMask;
	BOOL bNPatches;
	//importer opions
	BOOL bMakeTangentSpace;
};

//returns TRUE for success
extern BOOL importLWSFile(LWSImportDescStruct& rfImportDesc);

#endif