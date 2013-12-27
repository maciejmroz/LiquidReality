/********************************************************************
Basic texture classes
(c) 2002 Maciej Mróz
********************************************************************/

#ifndef _LRP_BASE_CAMERA_FX_H_INCLUDED_
#define _LRP_BASE_CAMERA_FX_H_INCLUDED_

namespace LR
{

	/*
 	 *	GrayscaleShift
	 */

	//blend between colored and grayscale version of rendertarget
	//out=factor*gray(in)+(1-factor)*in
	//gray.rgb=dp3(<0.3;0.59;0.11>,<R;G;B>)
	//other options (possible future filters):
	//desaturate (HSV's luminance): L=(min(R,G,B)+max(R,G,B))/2
	//HSV's value: V=max(R,G,B)
	class LRPGrayscaleShiftFx : public CameraEffectImpl
	{
		EnvelopePtr		m_ShiftFactorEnv;				//clamped to <0;1> range in apply()
		LPD3DXEFFECT	m_pFx;
		D3DXHANDLE		m_hTechnique;					//only one technique, one effect
	public:
		LRPGrayscaleShiftFx();
		virtual ~LRPGrayscaleShiftFx();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,DWORD dwDesiredResultRT=0xffffffff);
	};

	/*
	*	NegativeShift
	*/

	//shift to negative - simple blend between color and 1-color
	class LRPNegativeShiftFx : public CameraEffectImpl
	{
		EnvelopePtr		m_ShiftFactorEnv;				//clamped to <0;1> range in apply()
		LPD3DXEFFECT	m_pFx;
		D3DXHANDLE		m_hTechnique;					//only one technique, one effect
	public:
		LRPNegativeShiftFx();
		virtual ~LRPNegativeShiftFx();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,DWORD dwDesiredResultRT=0xffffffff);
	};

	/*
	 *	DirectionalLight
	 */

	//shift to negative - simple blend between color and 1-color
	class LRPDirectionalLightFx : public CameraEffectImpl
	{
		EnvelopePtr		m_LightDirectionEnv;
		ColorPtr		m_LightColor;
		EnvelopePtr		m_Height;
		LPD3DXEFFECT	m_pFx;
		D3DXHANDLE		m_hTechnique;					//only one technique, one effect
	public:
		LRPDirectionalLightFx();
		virtual ~LRPDirectionalLightFx();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,DWORD dwDesiredResultRT=0xffffffff);
	};

	/*
	 *	Overbright
	 */

	//shift to whiteness
	//x2,x4,x8 versions; ps1.4 offers better IQ
	//ps2.0 used as compatibility path for gf fx
	class LRPOverbrightFx : public CameraEffectImpl
	{
		EnvelopePtr		m_ShiftFactorEnv;				//clamped to <0;1> range in apply()
		LPD3DXEFFECT	m_pFx;
		D3DXHANDLE		m_hPS20Technique;
		DWORD			m_dwMode;
	public:
		LRPOverbrightFx();
		virtual ~LRPOverbrightFx();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,DWORD dwDesiredResultRT=0xffffffff);
	};

	/*
	 *	SimpleGlow
	 */

	//use simple blur as basis, then add to original scene using specified factor
	class LRPSimpleGlowFx : public CameraEffectImpl
	{
		//properties
		EnvelopePtr		m_GlowSizeEnv;
		EnvelopePtr		m_GlowIntensityEnv;
		DWORD			m_dwAmplification;
		//internals
		LPD3DXEFFECT	m_pFx;
		D3DXHANDLE		m_hBlurTechnique;
		D3DXHANDLE		m_hGlowMadTechnique;
		D3DXHANDLE		m_hGlowMadX2Technique;
		D3DXHANDLE		m_hGlowMadX4Technique;
	public:
		LRPSimpleGlowFx();
		virtual ~LRPSimpleGlowFx();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,DWORD dwDesiredResultRT=0xffffffff);
	};

	/*
	 *	CutoffGlow
	 */

	//use simple blur as basis, then add to original scene using specified factor
	class LRPCutoffGlowFx : public CameraEffectImpl
	{
		//properties
		EnvelopePtr		m_GlowSizeEnv;
		EnvelopePtr		m_GlowIntensityEnv;
		DWORD			m_dwAmplification;
		DWORD			m_dwCutoff;
		//internals
		LPD3DXEFFECT	m_pFx;
		D3DXHANDLE		m_hBlurTechnique;
		D3DXHANDLE		m_hGlowMadTechnique;
		D3DXHANDLE		m_hGlowMadX2Technique;
		D3DXHANDLE		m_hGlowMadX4Technique;
		D3DXHANDLE		m_hCutoffX2Technique;
		D3DXHANDLE		m_hCutoffX4Technique;
	public:
		LRPCutoffGlowFx();
		virtual ~LRPCutoffGlowFx();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,DWORD dwDesiredResultRT=0xffffffff);
	};

	/*
	 *	DirectionalGlow
	 */

	//use directional blur as basis, then add to original scene using specified factor
	class LRPDirectionalGlowFx : public CameraEffectImpl
	{
		//properties
		EnvelopePtr		m_GlowSizeEnv;
		EnvelopePtr		m_GlowIntensityEnv;
		EnvelopePtr		m_GlowDirectionEnv;
		DWORD			m_dwAmplification;
		//internals
		LPD3DXEFFECT	m_pFx;
		D3DXHANDLE		m_hBlurTechnique;
		D3DXHANDLE		m_hGlowMadTechnique;
		D3DXHANDLE		m_hGlowMadX2Technique;
		D3DXHANDLE		m_hGlowMadX4Technique;
		//it is exact match to generalized m3x2 blur, however as we are not going to generalize
		//glow effects that way it would be senseless to introduce 3x2 base for just one class
		//future refactorings should however do some grouping by rt usage pattern combined with
		//'algorithm injection', not really sure how right now
		unsigned int getNumIterations(float fTime,DWORD dwRTSizeX,DWORD dwRTSizeY);
		void buildTransformMatrices(unsigned int uiIteration,float fTime,
			DWORD dwRTSizeX,DWORD dwRTSizeY,float ConstantTbl[32]);
	public:
		LRPDirectionalGlowFx();
		virtual ~LRPDirectionalGlowFx();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,DWORD dwDesiredResultRT=0xffffffff);
	};

	/*
	 *	DirectionalCutoffGlow
	 */

	class LRPDirectionalCutoffGlowFx : public CameraEffectImpl
	{
		//properties
		EnvelopePtr		m_GlowSizeEnv;
		EnvelopePtr		m_GlowIntensityEnv;
		EnvelopePtr		m_GlowDirectionEnv;
		DWORD			m_dwAmplification;
		DWORD			m_dwCutoff;
		//internals
		LPD3DXEFFECT	m_pFx;
		D3DXHANDLE		m_hBlurTechnique;
		D3DXHANDLE		m_hGlowMadTechnique;
		D3DXHANDLE		m_hGlowMadX2Technique;
		D3DXHANDLE		m_hGlowMadX4Technique;
		D3DXHANDLE		m_hCutoffX2Technique;
		D3DXHANDLE		m_hCutoffX4Technique;
		unsigned int getNumIterations(float fTime,DWORD dwRTSizeX,DWORD dwRTSizeY);
		void buildTransformMatrices(unsigned int uiIteration,float fTime,
			DWORD dwRTSizeX,DWORD dwRTSizeY,float ConstantTbl[32]);
	public:
		LRPDirectionalCutoffGlowFx();
		virtual ~LRPDirectionalCutoffGlowFx();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,DWORD dwDesiredResultRT=0xffffffff);
	};

	//use simple blur as basis, then add to original scene using specified factor
	class LRPChannelBlurFx : public CameraEffectImpl
	{
		//properties
		EnvelopePtr		m_BlurSizeEnv;
		DWORD			m_dwBlurR;
		DWORD			m_dwBlurG;
		DWORD			m_dwBlurB;
		DWORD			m_dwBlurA;
		//internals
		LPD3DXEFFECT	m_pFx;
		D3DXHANDLE		m_hBlurTechnique;
		D3DXHANDLE		m_hBlurComposeTechnique;
	public:
		LRPChannelBlurFx();
		virtual ~LRPChannelBlurFx();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,DWORD dwDesiredResultRT=0xffffffff);
	};

	/*
	 *	AlphaTextureCompose
	 */

	//compose texture using alpha channels of source screen and/or texture
	class LRPAlphaTextureComposeFx : public CameraEffectImpl
	{
		//properties
		LRString		m_strTexture;
		DWORD			m_dwRGBMode;			//source/dest(texture) alpha
		DWORD			m_dwAlphaMode;			//source/dest(texture) alpha
		//internals
		TexturePtr		m_pTexture;
		LPD3DXEFFECT	m_pFx;
		D3DXHANDLE		m_hSrcAlphaRGBSrcAlphaA;
		D3DXHANDLE		m_hSrcAlphaRGBDstAlphaA;
		D3DXHANDLE		m_hDstAlphaRGBSrcAlphaA;
		D3DXHANDLE		m_hDstAlphaRGBDstAlphaA;
	public:
		LRPAlphaTextureComposeFx();
		virtual ~LRPAlphaTextureComposeFx();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,DWORD dwDesiredResultRT=0xffffffff);
	};

	/*
	 *	PremultipliedAlphaTextureCompose
	 */

	//compose texture using alpha channels of source screen and/or texture
	class LRPPremultipliedAlphaTextureComposeFx : public CameraEffectImpl
	{
		//properties
		LRString		m_strTexture;
		DWORD			m_dwRGBMode;			//source/dest(texture) alpha
		DWORD			m_dwAlphaMode;			//source/dest(texture) alpha
		//internals
		TexturePtr		m_pTexture;
		LPD3DXEFFECT	m_pFx;
		D3DXHANDLE		m_hSrcAlphaRGBSrcAlphaA;
		D3DXHANDLE		m_hSrcAlphaRGBDstAlphaA;
		D3DXHANDLE		m_hDstAlphaRGBSrcAlphaA;
		D3DXHANDLE		m_hDstAlphaRGBDstAlphaA;
	public:
		LRPPremultipliedAlphaTextureComposeFx();
		virtual ~LRPPremultipliedAlphaTextureComposeFx();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,DWORD dwDesiredResultRT=0xffffffff);
	};

	/*
	 *	MaskTextureCompose
	 */

	//mask screen with texture using texture's RGB or alpha
	//out=in*texture.rgb (alpha unchanged)
	//OR out=in*texture.a (alpha is also masked)
	class LRPMaskTextureComposeFx : public CameraEffectImpl
	{
		//properties
		LRString		m_strTexture;
		DWORD			m_dwComposeMode;		//RGB mask/alpha mask
		//internals
		TexturePtr		m_pTexture;
		LPD3DXEFFECT	m_pFx;
		D3DXHANDLE		m_hRGBCompose;
		D3DXHANDLE		m_hAlphaCompose;
	public:
		LRPMaskTextureComposeFx();
		virtual ~LRPMaskTextureComposeFx();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,DWORD dwDesiredResultRT=0xffffffff);
	};

	/*
	 *	LayerTextureCompose - uber composer that mimics most
	 *  Photoshop's layer blending modes
	 */

	enum LayerTextureComposeMode
	{
		LTCM_NORMAL,
		LTCM_DARKEN,
		LTCM_MULTIPLY,
		LTCM_LIGHTEN,
		LTCM_SCREEN,
		LTCM_OVERLAY,
		LTCM_HARD_LIGHT,
		LTCM_DIFFERENCE,
		LTCM_EXCLUSION,
		LTCM_OVERLAY_PS14,
		LTCM_HARD_LIGHT_PS14,
		LTCM_EXCLUSION_PS14
	};

	/*
	All the techniques we use here:
	Normal
	Darken
	Multiply
	Lighten
	Screen
	Overlay
	Hard Light
	Difference
	Exclusion

	Techniques that are undocumented/have too big hw implementation tradeoffs (today)
	at least some of them CAN be guessed and others will be implemented on ps2.0:
	Dissolve
	Color Burn
	Linear Burn
	Linear Dodge
	Soft Light
	Vivid Light
	Linear Light
	Pin Light
	Hue
	Saturation
	Color
	Luminosity
	*/

	#define NUM_LAYER_FX		36			//num_alpha_modes*num_color_modes

	class LRPLayerTextureComposeFx : public CameraEffectImpl
	{
		//properties
		LRString		m_strTexture;
		DWORD			m_dwComposeMode;		//RGB mask/alpha mask
		DWORD			m_dwAlphaMode;			//add/keep/replace
		EnvelopePtr		m_OpacityEnv;
		//internals
		TexturePtr		m_pTexture;
		LPD3DXEFFECT	m_pFx;
		//thare are quite a few effects here so it's better to keep them in an array
		D3DXHANDLE		m_ComposeTechniques[NUM_LAYER_FX];
	public:
		LRPLayerTextureComposeFx();
		virtual ~LRPLayerTextureComposeFx();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,DWORD dwDesiredResultRT=0xffffffff);
	};

	//this one differs from previous only by additional reverse mode
	//(it is a superset, but separate source as previous one will get
	//doomed in 0.3.x)
	class LRPLayerTextureCompose2Fx : public CameraEffectImpl
	{
		//properties
		LRString		m_strTexture;
		DWORD			m_dwComposeMode;		//RGB mask/alpha mask
		DWORD			m_dwAlphaMode;			//add/keep/replace
		EnvelopePtr		m_OpacityEnv;
		DWORD			m_dwReverse;
		//internals
		TexturePtr		m_pTexture;
		LPD3DXEFFECT	m_pFx;
		//thare are quite a few effects here so it's better to keep them in an array
		D3DXHANDLE		m_ComposeTechniques[NUM_LAYER_FX];
	public:
		LRPLayerTextureCompose2Fx();
		virtual ~LRPLayerTextureCompose2Fx();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,DWORD dwDesiredResultRT=0xffffffff);
	};

	/*
	 *	SpriteDraw
	 */

	//draw sprite on current render target
	//uses only default ADD and BLEND sprite modes
	//TODO: fp rt compatibility will require RT switching and
	//completely different codepath
	class LRPSpriteDrawFx : public CameraEffectImpl
	{
		//properties
		LRString		m_strTexture;
		DWORD			m_dwComposeMode;		//blend/add
		EnvelopePtr		m_Intensity;
		EnvelopePtr		m_PositionX;
		EnvelopePtr		m_PositionY;
		EnvelopePtr		m_SizeX;
		EnvelopePtr		m_SizeY;
		EnvelopePtr		m_TexturePositionX;
		EnvelopePtr		m_TexturePositionY;
		EnvelopePtr		m_TextureSizeX;
		EnvelopePtr		m_TextureSizeY;
		//internals
		TexturePtr			m_pTexture;
		LPD3DXEFFECT		m_pFx;
		D3DXHANDLE			m_hBlend;
		D3DXHANDLE			m_hAdd;
		D3DXHANDLE			m_hConstantBlend;
	public:
		LRPSpriteDrawFx();
		virtual ~LRPSpriteDrawFx();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,DWORD dwDesiredResultRT=0xffffffff);
	};

	/*
	 *	Base class for blur effects based on 4 sample averaging
	 *  with texture coordinates defined by generic m3x2 transform
	 *  of default sprite texcoords
	 */

	class LRGenericM3x2BlurBase : public CameraEffectImpl
	{
		LPD3DXEFFECT			m_pFx;
		D3DXHANDLE				m_hTechnique;					//only one technique, one effect
	public:
		LRGenericM3x2BlurBase();
		virtual ~LRGenericM3x2BlurBase();
		void initialize(AutoPtr<fs> &pfs);
		void apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,DWORD dwDesiredResultRT=0xffffffff);
	protected:
		//derived class has to say how many iterations it is going to need
		virtual unsigned int getNumIterations(float fTime,DWORD dwRTSizeX,DWORD dwRTSizeY)=0;
		//output 8 vertex shader constants
		virtual void buildTransformMatrices(unsigned int uiIteration,float fTime,
											DWORD dwRTSizeX,DWORD dwRTSizeY,float ConstantTbl[32])=0;
	};

	/*
	 *	DirectionalBlur class - derived from generic m3x2 blur class
	 */

	class LRPDirectionalBlurFx : public LRGenericM3x2BlurBase
	{
		EnvelopePtr		m_BlurSizeEnv;
		EnvelopePtr		m_BlurDirectionEnv;
	public:
		LRPDirectionalBlurFx();
		virtual ~LRPDirectionalBlurFx();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
	protected:
		unsigned int getNumIterations(float fTime,DWORD dwRTSizeX,DWORD dwRTSizeY);
		void buildTransformMatrices(unsigned int uiIteration,float fTime,
										DWORD dwRTSizeX,DWORD dwRTSizeY,float ConstantTbl[32]);
	};

	/*
	 *	RadialBlur class - derived from generic m3x2 blur class
	 *  not exactly the same as multiple layers but classic sw radial blur
	 *  wasn't exactly the same neither; looks cool and it's all that matters :)
	 */

	//beware that this effect IS NOT ASPECT RATIO INVARIANT, perhaps not even
	//RT resolution invariant!!!!
	class LRPRadialBlurFx : public LRGenericM3x2BlurBase
	{
		EnvelopePtr		m_BlurSizeEnv;
	public:
		LRPRadialBlurFx();
		virtual ~LRPRadialBlurFx();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
	protected:
		unsigned int getNumIterations(float fTime,DWORD dwRTSizeX,DWORD dwRTSizeY);
		void buildTransformMatrices(unsigned int uiIteration,float fTime,
										DWORD dwRTSizeX,DWORD dwRTSizeY,float ConstantTbl[32]);
	};

	/*
	 *	SimpleBlur
	 */

	//recursive averaging, 4 samples/pass
	class LRPSimpleBlurFx : public LRGenericM3x2BlurBase
	{
		EnvelopePtr	m_BlurSizeEnv;
	public:
		LRPSimpleBlurFx();
		virtual ~LRPSimpleBlurFx();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
	protected:
		unsigned int getNumIterations(float fTime,DWORD dwRTSizeX,DWORD dwRTSizeY);
		void buildTransformMatrices(unsigned int uiIteration,float fTime,
										DWORD dwRTSizeX,DWORD dwRTSizeY,float ConstantTbl[32]);
	};

	/*
	 *	Base class for transform effects doing 3x2 matrix transforms of input
	 */

	class LRGenericM3x2TransformBase : public CameraEffectImpl
	{
		LPD3DXEFFECT			m_pFx;
		D3DXHANDLE				m_hTechnique;					//only one technique, one effect
	public:
		LRGenericM3x2TransformBase();
		virtual ~LRGenericM3x2TransformBase();
		void initialize(AutoPtr<fs> &pfs);
		void apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,DWORD dwDesiredResultRT=0xffffffff);
	protected:
		//output 2 vertex shader constants
		virtual void buildTransformMatrix(float fTime,DWORD dwRTSizeX,DWORD dwRTSizeY,float ConstantTbl[8])=0;
	};

	/*
	 *	Tiler effect - scale and then rotate
	 */

	class LRPTilerFx : public LRGenericM3x2TransformBase
	{
		EnvelopePtr	m_ScaleX;
		EnvelopePtr	m_ScaleY;
		EnvelopePtr	m_ScalePivotX;
		EnvelopePtr	m_ScalePivotY;
		EnvelopePtr	m_Angle;
		EnvelopePtr	m_RotatePivotX;
		EnvelopePtr	m_RotatePivotY;
	public:
		LRPTilerFx();
		~LRPTilerFx();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
	protected:
		virtual void buildTransformMatrix(float fTime,DWORD dwRTSizeX,DWORD dwRTSizeY,float ConstantTbl[8]);
	};

	/*
	 *	TiledMaskTextureCompose
	 */

	//mask screen with texture using texture's RGB or alpha
	//out=in*texture.rgb (alpha unchanged)
	//OR out=in*texture.a (alpha is also masked)
	//perform generic m3x2 on mask texture
	class LRPTiledMaskTextureComposeFx : public CameraEffectImpl
	{
		//properties
		LRString		m_strTexture;
		DWORD			m_dwComposeMode;		//RGB mask/alpha mask
		EnvelopePtr		m_ScaleX;
		EnvelopePtr		m_ScaleY;
		EnvelopePtr		m_ScalePivotX;
		EnvelopePtr		m_ScalePivotY;
		EnvelopePtr		m_Angle;
		EnvelopePtr		m_RotatePivotX;
		EnvelopePtr		m_RotatePivotY;	
		//internals
		TexturePtr		m_pTexture;
		LPD3DXEFFECT	m_pFx;
		D3DXHANDLE		m_hRGBCompose;
		D3DXHANDLE		m_hAlphaCompose;
	protected:
		void buildTransformMatrix(float fTime,DWORD dwRTSizeX,DWORD dwRTSizeY,float ConstantTbl[8]);
	public:
		LRPTiledMaskTextureComposeFx();
		virtual ~LRPTiledMaskTextureComposeFx();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,DWORD dwDesiredResultRT=0xffffffff);
	};

	/*
	 *	ColorRemap
	 */

	//compose texture using alpha channels of source screen and/or texture
	class LRPColorRemapFx : public CameraEffectImpl
	{
		//properties
		EnvelopePtr		m_ShiftFactorEnv;				//clamped to <0;1> range in apply()
		LRString		m_strTexture;
		//internals
		TexturePtr		m_pTexture;
		LPD3DXEFFECT	m_pFx;
		D3DXHANDLE		m_hPhase1Technique;
	public:
		LRPColorRemapFx();
		virtual ~LRPColorRemapFx();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,DWORD dwDesiredResultRT=0xffffffff);
	};

	/*
	 *	NegativeShiftTextureFactor
	 */

	//compose texture using alpha channels of source screen and/or texture
	class LRPNegativeShiftTextureFactorFx : public CameraEffectImpl
	{
		//properties
		EnvelopePtr		m_ShiftFactorEnv;				//clamped to <0;1> range in apply()
		LRString		m_strTexture;
		//internals
		TexturePtr		m_pTexture;
		LPD3DXEFFECT	m_pFx;
		D3DXHANDLE		m_hTechnique;
	public:
		LRPNegativeShiftTextureFactorFx();
		virtual ~LRPNegativeShiftTextureFactorFx();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,DWORD dwDesiredResultRT=0xffffffff);
	};

	/*
	 *	GrayscaleShiftTextureFactor
	 */

	//compose texture using alpha channels of source screen and/or texture
	class LRPGrayscaleShiftTextureFactorFx : public CameraEffectImpl
	{
		//properties
		EnvelopePtr		m_ShiftFactorEnv;				//clamped to <0;1> range in apply()
		LRString		m_strTexture;
		//internals
		TexturePtr		m_pTexture;
		LPD3DXEFFECT	m_pFx;
		D3DXHANDLE		m_hTechnique;
	public:
		LRPGrayscaleShiftTextureFactorFx();
		virtual ~LRPGrayscaleShiftTextureFactorFx();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,DWORD dwDesiredResultRT=0xffffffff);
	};


	/*
	 *	StaticOffset
	 */

	//compose texture using alpha channels of source screen and/or texture
	class LRPStaticOffsetFx : public CameraEffectImpl
	{
		//properties
		EnvelopePtr		m_StrengthXEnv;				//clamped to <0;1> range in apply()
		EnvelopePtr		m_StrengthYEnv;				//clamped to <0;1> range in apply()
		LRString		m_strTexture;
		//internals
		TexturePtr		m_pTexture;
		LPD3DXEFFECT	m_pFx;
		D3DXHANDLE		m_hTechnique;
	public:
		LRPStaticOffsetFx();
		virtual ~LRPStaticOffsetFx();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,DWORD dwDesiredResultRT=0xffffffff);
	};

	/*
	 *	DynamicOffset
	 */

	class LRPDynamicOffsetFx : public CameraEffectImpl
	{
		//properties
		EnvelopePtr		m_Height;					//1.0 by default - means 1/256 of texture size
		EnvelopePtr		m_StrengthXEnv;				//clamped to <0;1> range in apply()
		EnvelopePtr		m_StrengthYEnv;				//clamped to <0;1> range in apply()
		LRString		m_strTexture;
		//internals
		TexturePtr		m_pTexture;
		LPD3DXEFFECT	m_pFx;
		D3DXHANDLE		m_hPhase1Technique;
	public:
		LRPDynamicOffsetFx();
		virtual ~LRPDynamicOffsetFx();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,DWORD dwDesiredResultRT=0xffffffff);
	};

	/*
	 *	DynamicOffsetReflect - based on previous, includes fake reflection term
	 */

	//THIS CLASS IS NOT FULLY WIDESCREEN MODE COMPATIBLE
	class LRPDynamicOffsetReflectFx : public CameraEffectImpl
	{
		//properties
		EnvelopePtr		m_Height;					//1.0 by default - means 1/256 of texture size
		EnvelopePtr		m_StrengthXEnv;				//clamped to <0;1> range in apply()
		EnvelopePtr		m_StrengthYEnv;				//clamped to <0;1> range in apply()
		LRString		m_strTexture;
		LRString		m_strEnvironment;
		//internals
		TexturePtr		m_pTexture;
		TexturePtr		m_pEnvTexture;
		LPD3DXEFFECT	m_pFx;
		D3DXHANDLE		m_hPhase1Technique;
	public:
		LRPDynamicOffsetReflectFx();
		virtual ~LRPDynamicOffsetReflectFx();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,DWORD dwDesiredResultRT=0xffffffff);
	};

	/*
	 *	SelfOffset - very similar to DynamicOffset but uses input rt as offset texture
	 *  BEWARE: if any fix is done to DynamicOffset it has to manually be propagated to this class
	 *  TODO: fix somehow this issue
	 */

	class LRPSelfOffsetFx : public CameraEffectImpl
	{
		//properties
		EnvelopePtr		m_Height;					//1.0 by default - means 1/256 of texture size
		EnvelopePtr		m_StrengthXEnv;				//clamped to <0;1> range in apply()
		EnvelopePtr		m_StrengthYEnv;				//clamped to <0;1> range in apply()
		//internals
		LPD3DXEFFECT	m_pFx;
		D3DXHANDLE		m_hPhase1Technique;
	public:
		LRPSelfOffsetFx();
		virtual ~LRPSelfOffsetFx();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,DWORD dwDesiredResultRT=0xffffffff);
	};

	/*
	 *	Emboss
	 */

	class LRPEmbossFx : public CameraEffectImpl
	{
		EnvelopePtr		m_Height;						//1.0 by default - means 1/256 of texture size
		ColorPtr		m_Color;
		LPD3DXEFFECT	m_pFx;
		D3DXHANDLE		m_hTechnique;					//only one technique, one effect
	public:
		LRPEmbossFx();
		virtual ~LRPEmbossFx();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,DWORD dwDesiredResultRT=0xffffffff);
	};

	/*
	 *	EdgeOutliner
	 */

	class LRPEdgeOutlinerFx : public CameraEffectImpl
	{
		//properties
		EnvelopePtr		m_Height;					//1.0 by default - means 1/256 of texture size
		ColorPtr		m_OutlineColor;				//white by default
		EnvelopePtr		m_Treshold;
		//internals
		LPD3DXEFFECT	m_pFx;
		D3DXHANDLE		m_hPhase1Technique;
		D3DXHANDLE		m_hPhase2Technique;
	public:
		LRPEdgeOutlinerFx();
		virtual ~LRPEdgeOutlinerFx();
		const char* getObjectSuperClass();
		const char* getObjectClass();
		void registerProperties();
		HRESULT	getNewInstance(BaseObject **ppOut);
		void initialize(AutoPtr<fs> &pfs);
		void apply(float fTime,DWORD dwSourceRT,DWORD &dwResultRT,DWORD dwDesiredResultRT=0xffffffff);
	};

}

#endif