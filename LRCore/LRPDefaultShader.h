/*****************************************************************
 LiquidReality default shader plugin class
 (c) 2002 Maciej Mróz
******************************************************************/

#ifndef _LR_DEFAULT_SHADER_H_
#define _LR_DEFAULT_SHADER_H_

namespace LR
{

	struct TextureDesc
	{
		//0 - none
		//1 - cameraspace position
		//2 - cameraspace normal
		//3 - cameraspace reflection vector
		DWORD		dwTexgenMode;
		DWORD		dwCoordSetIndex;	//already offset by 1 if tangent is used
										//no need to do it again
		D3DXMATRIXA16	mtxTexture;
		bool			bIdentityTexMatrix;
	};

	struct PassDesc
	{
		DWORD dwTextureCount;
		DWORD dwTextureDescID[8];
		DWORD dwPixelShaderConstantCount;
		float constants[16];	//c0-c4 in single vector
		inline void reset()
		{
			dwTextureCount=0;
			dwPixelShaderConstantCount=0;
		}

		inline void addPixelShaderConstant(float *pfVec4)
		{
			float *pfTmp=&constants[4*dwPixelShaderConstantCount];
			*pfTmp++=*pfVec4++;
			*pfTmp++=*pfVec4++;
			*pfTmp++=*pfVec4++;
			*pfTmp++=*pfVec4++;
			dwPixelShaderConstantCount++;
		}
	};

	enum DEFAULT_SHADER_TEXTURES
	{
		TEX_COLOR,
		TEX_DIFFUSE,
		TEX_LUMINOSITY,
		TEX_GLOSS,
		TEX_SHININESS,
		TEX_REFLECTIVITY,
		TEX_ENVIRONMENT
	};

	enum DEFAULT_SHADER_TECHNIQUES
	{
		//gf3 class hw (ps 1.1)
		TECHNIQUE_NOTHING_PS11,
		TECHNIQUE_COLOR_PS11,
		TECHNIQUE_COLOR_DIFFUSE_PS11,
		TECHNIQUE_COLOR_LUMI_PS11,
		TECHNIQUE_COLOR_DIF_LUMI_PS11,
		TECHNIQUE_COLOR_SHINE_PS11,
		TECHNIQUE_COLOR_DIF_SHINE_PS11,
		TECHNIQUE_COLOR_LUMI_SHINE_PS11,
		TECHNIQUE_COLOR_DIF_LUMI_SHINE_PS11,
		TECHNIQUE_COLOR_GLOSS_SHINE_PS11,
		TECHNIQUE_COLOR_DIF_GLOSS_SHINE_PS11,
		TECHNIQUE_COLOR_LUMI_GLOSS_SHINE_PS11,
		TECHNIQUE_COLOR_DIF_LUMI_GLOSS_SHINE_PS11,
		TECHNIQUE_COLOR_ENV_PS11,
		TECHNIQUE_COLOR_DIF_ENV_PS11,
		TECHNIQUE_COLOR_LUMI_ENV_PS11,
		TECHNIQUE_COLOR_DIF_LUMI_ENV_PS11,
		TECHNIQUE_COLOR_SHINE_ENV_PS11,
		TECHNIQUE_COLOR_DIF_SHINE_ENV_PS11,
		TECHNIQUE_COLOR_LUMI_SHINE_ENV_PS11,
		TECHNIQUE_COLOR_DIF_LUMI_SHINE_ENV_PS11,
		TECHNIQUE_COLOR_GLOSS_SHINE_ENV_PS11,
		TECHNIQUE_COLOR_DIF_GLOSS_SHINE_ENV_PS11,
		TECHNIQUE_COLOR_LUMI_GLOSS_SHINE_ENV_PS11,
		TECHNIQUE_COLOR_DIF_LUMI_GLOSS_SHINE_ENV_PS11,
		TECHNIQUE_COLOR_REFL_ENV_PS11,
		TECHNIQUE_COLOR_DIF_REFL_ENV_PS11,
		TECHNIQUE_COLOR_LUMI_REFL_ENV_PS11,
		TECHNIQUE_COLOR_DIF_LUMI_REFL_ENV_PS11,
		TECHNIQUE_COLOR_SHINE_REFL_ENV_PS11,
		TECHNIQUE_COLOR_DIF_SHINE_REFL_ENV_PS11,
		TECHNIQUE_COLOR_LUMI_SHINE_REFL_ENV_PS11,
		TECHNIQUE_COLOR_DIF_LUMI_SHINE_REFL_ENV_PS11,
		TECHNIQUE_COLOR_GLOSS_SHINE_REFL_ENV_PS11,
		TECHNIQUE_COLOR_DIF_GLOSS_SHINE_REFL_ENV_PS11,
		TECHNIQUE_COLOR_LUMI_GLOSS_SHINE_REFL_ENV_PS11,
		TECHNIQUE_COLOR_DIF_LUMI_GLOSS_SHINE_REFL_ENV_PS11,
		//ps 1.4 optimizations (r8500 class)
		TECHNIQUE_COLOR_DIF_LUMI_GLOSS_SHINE_PS14,
		TECHNIQUE_COLOR_DIF_LUMI_SHINE_ENV_PS14,
		TECHNIQUE_COLOR_DIF_GLOSS_SHINE_ENV_PS14,
		TECHNIQUE_COLOR_LUMI_GLOSS_SHINE_ENV_PS14,
		TECHNIQUE_COLOR_DIF_LUMI_GLOSS_SHINE_ENV_PS14,
		TECHNIQUE_COLOR_DIF_LUMI_REFL_ENV_PS14,
		TECHNIQUE_COLOR_DIF_SHINE_REFL_ENV_PS14,
		TECHNIQUE_COLOR_LUMI_SHINE_REFL_ENV_PS14,
		TECHNIQUE_COLOR_DIF_LUMI_SHINE_REFL_ENV_PS14,
		TECHNIQUE_COLOR_GLOSS_SHINE_REFL_ENV_PS14,
		TECHNIQUE_COLOR_DIF_GLOSS_SHINE_REFL_ENV_PS14,
		TECHNIQUE_COLOR_LUMI_GLOSS_SHINE_REFL_ENV_PS14,
		TECHNIQUE_COLOR_DIF_LUMI_GLOSS_SHINE_REFL_ENV_PS14,
	};

	//quite a few of them :)
	#define TOTAL_DEFAULT_FX_TECHNIQUE_COUNT 50

	//shader must be reentrant (in sense of multiple ongoing scene renders at once
	//not in sense of being thread safe - but that more or less turns out to be
	//the same in terms of solution), in order to assure any "render local state"
	//is wrapped into RenderDesc structure that is stack allocated for every render()
	//call
	struct RenderDesc
	{
		bool			bUseEffect;
		TextureDesc		TextureDescTbl[16];
		//used to indentify codepath in render pass setup
		PassDesc		EffectDesc[8];		//up to 8 render passes for now
	};

	class LRPDefaultShader : public ShaderImpl
	{
		//internal stuff
		//creation
		bool			m_bCreated;
		bool			m_bInCreate;
		//rendering
		LPD3DXEFFECT	m_pDefaultEffect;
		//texture handles
		D3DXHANDLE		m_TexHandles[16];
		//technique handles - old TSS pipeline
		D3DXHANDLE		m_TechniqueHandles[TOTAL_DEFAULT_FX_TECHNIQUE_COUNT];
		//parameter handles
		D3DXHANDLE		m_ReflectivityHandle;

	public:
		LRPDefaultShader();
		virtual ~LRPDefaultShader();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT getNewInstance(BaseObject **ppOut);
		void clear();
		//shader specific
		void initialize(AutoPtr<fs> &pfs);
		void render(SHADER_RENDER_MODE eMode,ScenePtr pScene,RenderTblEntry *pRenderTbl,
			DWORD dwRenderTblSize,int iMirrorLevel);
	protected:
		void internalCreateSurface(SurfacePtr &pSurface);
	private:
		//vertex lighting profile, fixed function only, gf1/radeon class hardware
		//heavy pixel shader optimizations
		HRESULT setupTexture(DWORD dwStage,DWORD dwTexgenMode,DWORD dwCoordSetIndex,
								bool bIdentityMatrix,D3DXMATRIXA16 *pmtxTexture);
		void buildTextureDesc(float fTime,TextureDesc *pPassDesc,
								Material* pMaterial,MT_TEXTURE eTextureId,
								MeshPtr pMeshNode);
		HRESULT setEffectTexture(float fTime,TextureDesc *pPassDesc,
								Material* pMaterial,MT_TEXTURE eTextureId,
								MeshPtr pMeshNode,
								D3DXHANDLE hTexture);
		D3DXMATRIXA16* buildTextureMatrix(float fTime,Material *pMaterial,
								MT_TEXTURE eTextureId);
		void setupMaterial(Material *pTmpMaterial, float fTime,
								RenderTblEntry *pRenderTbl,bool *pbRenderKill,
								bool *pbRenderstatesChanged);
		HRESULT setupRenderingPS14(float fTime,MeshPtr pMeshNode,
								Material* pMaterial,
								UINT *puPasses,RenderDesc *pRenderDesc);
		HRESULT setupRendering(float fTime,MeshPtr pMeshNode,
								Material* pMaterial,
								UINT *puPasses,RenderDesc *pRenderDesc);
		HRESULT setupBlending(MeshPtr pMeshNode,Material *pMaterial);
		HRESULT setupRenderPassPS14(UINT uPass,RenderDesc *pRenderDesc);
		HRESULT setupRenderPass(UINT uPass,RenderDesc *pRenderDesc);
		HRESULT cleanupRenderSetup(RenderDesc *pRenderDesc);
		//ligthing setup
		HRESULT buildLightInfo(float fTime,D3DLIGHT9 *pLightInfo,ScenePtr pScene,DWORD dwLightIndex);
		//global scene renderstate setup
		HRESULT baseRenderSetup(float fTime,ScenePtr pScene,bool &bFogEnabled);
		//main function
		void renderVLFixedProfile(SHADER_RENDER_MODE eMode,ScenePtr pScene,
							RenderTblEntry *pRenderTbl,DWORD dwRenderTblSize,
							int iMirrorLevel);
		//generic utilities
		HRESULT pushGeometry(MeshPtr pMeshNode);
	};

	/*
	 *	Surface
	 */
	class MaterialImpl : public virtual Material,public SurfaceImpl
	{
		//properties
		LRString		m_TextureTbl[7];
		EnvelopePtr		m_TextureIntensities[7];
		DWORD			m_TexCoordControlParamTbl[7];
		DWORD			m_TexCoordMatrixTransforms[7];
		EnvelopePtr		m_TexCoord_S_TranslateParamEnv[7];
		EnvelopePtr		m_TexCoord_T_TranslateParamEnv[7];
		EnvelopePtr		m_TexCoord_S_Scale[7];
		EnvelopePtr		m_TexCoord_T_Scale[7];
		EnvelopePtr		m_pTransparency;
		DWORD			m_dwTransparency;
		ColorPtr		m_pDiffuseColor;
		ColorPtr		m_pSpecularColor;
		ColorPtr		m_pAmbientColor;
		//alpha testing support
		DWORD			m_dwAlphaTestEnable;
		DWORD			m_dwAlphaTestMode;
		EnvelopePtr		m_pAlphaTestCutOutValue;
		//unaffected by fog
		DWORD			m_dwUnaffectedByFog;
		//internals
		TexturePtr		m_pTextures[7];
		//material cache
		bool			m_bMaterialCacheInitialized;
		float			m_fLastCacheTime;
		D3DMATERIAL9	m_MaterialCache;
		//texture matrix cache
		bool			m_bTextureMtxCacheInitialized[7];
		float			m_fLastTextureMtxCacheTime[7];
		D3DXMATRIXA16	m_TextureMtxCache[16];
	public:
		MaterialImpl();
		virtual ~MaterialImpl();
		bool equals(BaseObject *pLRBaseObj);
		//shader param block ALWAYS uses the same class/superclass as main shader! (NOTE: why?!)
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);	//doesn't have real usage since
														//Material is never used with class factory
		void clear();
		void getTextureIntensity(DWORD dwTextureID,EnvelopePtr *ppEnv);
		void getAmbientColor(ColorPtr *ppCol /*OUT*/);
		void getDiffuseColor(ColorPtr *ppCol /*OUT*/);
		void getSpecularColor(ColorPtr *ppCol /*OUT*/);
		//modes:
		//0=none
		//1=blend
		//2=add
		//3=subract
		//4=rev_subract
		//5=minmax
		DWORD getTransparencyMode();
		void setTransparencyMode(DWORD dwMode);
		void getTransparency(EnvelopePtr *ppEnv);
		//alpha testing settings
		//0 - disabled
		//1 - enabled
		DWORD getAlphaTestEnable();
		void setAlphaTestEnable(DWORD dwEnable);
		//0 - cut out on alpha less than critical value
		//1 - cut out on greater than
		DWORD getAlphaCutOutMode();
		void setAlphaCutOutMode(DWORD dwMode);
		void getAlphaCutOutValue(EnvelopePtr *ppEnv /*OUT*/);
		//unaffected by fog setting
		//0 - disabled
		//1 - enabled
		DWORD getUnaffectedByFog();
		void setUnaffectedByFog(DWORD dwVal);
		//texturing stuff
		DWORD getTexturePresenceFlags();
		void attachTexture(MT_TEXTURE eTextureId,TexturePtr pTexture);
		void getTexture(MT_TEXTURE eTextureId,TexturePtr &ppTexture);
		DWORD getTextureMatrixMode(MT_TEXTURE eTextureId);
		void setTextureMatrixMode(MT_TEXTURE eTextureId,DWORD dwMode);
		DWORD getTextureCoordSetControl(MT_TEXTURE eTextureId);
		void setTextureCoordSetControl(MT_TEXTURE eTextureId,DWORD dwCoordSetControl);
		void getTexCoordScalingEnv(MT_TEXTURE eTextureId,MT_TEXTURE_COORDINATE eCoord,
									EnvelopePtr *ppEnv /*OUT*/);
		void getTexCoordTranslateEnv(MT_TEXTURE eTextureId,MT_TEXTURE_COORDINATE eCoord,
									EnvelopePtr *ppEnv /*OUT*/);
		D3DMATERIAL9* getD3DMaterial(float fTime);
		D3DXMATRIXA16* getTextureMatrix(float fTime,MT_TEXTURE eTextureId);
	};
}

#endif