texture color_tex;
texture diffuse_tex;
texture luminosity_tex;  //"diffuse lightmap"
texture gloss_tex;
texture shininess_tex;   //"specular lightmap"

// reflectivity factor texture
// generally alpha channel of this texture is used
// as per-pixel reflection factor
// pixel sdader std surface implementation uses separate
// reflection factors for R,G,B and A so it is generally
// good idea to replicate reflection into all color channels
// for compatility reasons (unless ps hw is minimum required)
texture reflectivity_tex;
texture environment_tex;
float4 reflectivity_col;  //reflectivity - should be replicated to rgba

//
//
//
//
// Pixel shader based techniques
//
//
//
//

technique nothing_ps11
{
 pass p0
 {
  PixelShader =
  asm {
   ps_1_1
   mov r0,v0
  };
 }
}

technique color_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  PixelShader =
  asm {
   ps_1_1
   tex t0
   mul r0,t0,v0
  };
 }
}


technique color_diffuse_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <diffuse_tex>;
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mul r0,t0,t1
   mul r0,r0,v0
  };
 }
}

technique color_lumi_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <luminosity_tex>;
  //c0 - intensity of luminosity texture
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mad r0,t1,c0,v0
   mul r0.rgb,r0,t0
   +mul r0.a,t0,v0
  };
 }
}


technique color_dif_lumi_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <diffuse_tex>;
  Texture[2] = <luminosity_tex>;
  //c0 - intensity of luminosity texture
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   mad r0,t2,c0,v0
   mul r0.rgb,r0,t0
   +mul r0.a,t0,v0
   mul r0,r0,t1
  };
 }
}

technique color_shine_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <shininess_tex>;
  //c0 - gloss/shine compound intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mul r0,t0,v0
   mad r0.rgb,c0,t1,r0
  };
 }
}

technique color_dif_shine_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <diffuse_tex>;
  Texture[2] = <shininess_tex>;
  //c0 - gloss/shine compound intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   mul r0,t0,v0
   mul r0,r0,t1
   mad r0.rgb,c0,t2,r0
  };
 }
}

technique color_lumi_shine_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <luminosity_tex>;
  Texture[2] = <shininess_tex>;
  //c0 - intensity of luminosity texture
  //c1 - gloss/shine compound intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   mad r0,t1,c0,v0
   mul r0.rgb,r0,t0
   +mul r0.a,t0,v0
   mad r0.rgb,c1,t2,r0
  };
 }
}

technique color_dif_lumi_shine_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <diffuse_tex>;
  Texture[2] = <luminosity_tex>;
  Texture[3] = <shininess_tex>;
  //c0 - intensity of luminosity texture
  //c1 - gloss/shine compound intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   tex t3
   mad r0,t2,c0,v0
   mul r0.rgb,r0,t0
   +mul r0.a,t0,v0
   mul r0,r0,t1
   mad r0.rgb,c1,t3,r0
  };
 }
}

technique color_gloss_shine_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <gloss_tex>;
  Texture[2] = <shininess_tex>;
  //c0 - gloss/shine compound intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   mul r0,t0,v0
   mul t2,t2,c0
   mad r0.rgb,t1,t2,r0
  };
 }
}

technique color_dif_gloss_shine_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <diffuse_tex>;
  Texture[2] = <gloss_tex>;
  Texture[3] = <shininess_tex>;
  //c0 - gloss/shine compound intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   tex t3
   mul r0,t0,v0
   mul r0,r0,t1
   mul t3,t3,c0
   mad r0.rgb,t2,t3,r0
  };
 }
}

technique color_lumi_gloss_shine_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <luminosity_tex>;
  Texture[2] = <gloss_tex>;
  Texture[3] = <shininess_tex>;
  //c0 - intensity of luminosity texture
  //c1 - gloss/shine compound intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   tex t3
   mad r0,t1,c0,v0
   mul r0.rgb,r0,t0
   +mul r0.a,t0,v0
   mul t3,t3,c1
   mad r0.rgb,t2,t3,r0
  };
 }
}

technique color_dif_lumi_gloss_shine_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <diffuse_tex>;
  Texture[2] = <luminosity_tex>;
  //c0 - intensity of luminosity texture
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   mad r0,t2,c0,v0
   mul r0.rgb,r0,t0
   +mul r0.a,t0,v0
   mul r0,r0,t1
  };
 }
 pass p1
 {
  AlphaBlendEnable = True;
  BlendOp = Add;
  SrcBlend = One;
  DestBlend = One;

  Texture[0] = <gloss_tex>;
  Texture[1] = <shininess_tex>;
  //c0 - gloss/shine compound intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mul r0,t0,t1
   mul r0,r0,v0
   mul r0,r0,c0
  };
 }
}

technique color_env_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <environment_tex>;

  //c0 - reflectivity factor
  //c1 - environonment intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mul r0,t0,v0
   mul t1,t1,c1
   lrp r0.rgb,c0,t1,r0
  };
 }
}

technique color_dif_env_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <diffuse_tex>;
  Texture[2] = <environment_tex>;

  //c0 - reflectivity factor
  //c1 - environonment intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   mul r0,t0,v0
   mul r0,r0,t1
   mul t2,t2,c1
   lrp r0.rgb,c0,t2,r0
  };
 }
}

technique color_lumi_env_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <luminosity_tex>;
  Texture[2] = <environment_tex>;

  //c0 - luminosity
  //c1 - reflectivity factor
  //c2 - environonment intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   mad r0,t1,c0,v0
   mul r0.rgb,r0,t0
   +mul r0.a,t0,v0
   mul t2,t2,c2
   lrp r0.rgb,c1,t2,r0
  };
 }
}

technique color_dif_lumi_env_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <diffuse_tex>;
  Texture[2] = <luminosity_tex>;
  Texture[3] = <environment_tex>;

  //c0 - luminosity
  //c1 - reflectivity factor
  //c2 - environonment intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   tex t3
   mad r0,t2,c0,v0
   mul r0.rgb,r0,t0
   +mul r0.a,t0,v0
   mul r0,r0,t1
   mul t3,t3,c2
   lrp r0.rgb,c1,t3,r0
  };
 }
}

technique color_shine_env_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <shininess_tex>;
  Texture[2] = <environment_tex>;

  //c0 - gloss/shine compound intensity
  //c1 - reflectivity factor
  //c2 - environonment intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   mul r0,t0,v0
   mad r0.rgb,c0,t1,r0
   mul t2,t2,c2
   lrp r0.rgb,c1,t2,r0
  };
 }
}

technique color_dif_shine_env_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <diffuse_tex>;
  Texture[2] = <shininess_tex>;
  Texture[3] = <environment_tex>;

  //c0 - gloss/shine compound intensity
  //c1 - reflectivity factor
  //c2 - environonment intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   tex t3
   mul r0,t0,v0
   mul r0,r0,t1
   mad r0.rgb,c0,t2,r0
   mul t3,t3,c2
   lrp r0.rgb,c1,t3,r0
  };
 }
}

technique color_lumi_shine_env_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <luminosity_tex>;
  Texture[2] = <shininess_tex>;
  Texture[3] = <environment_tex>;

  //c0 - luminosity
  //c1 - gloss/shine compound intensity
  //c2 - reflectivity factor
  //c3 - environonment intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   tex t3
   mad r0,t1,c0,v0
   mul r0.rgb,r0,t0
   +mul r0.a,t0,v0
   mad r0.rgb,c1,t2,r0
   mul t3,t3,c3
   lrp r0.rgb,c2,t3,r0
  };
 }
}

technique color_dif_lumi_shine_env_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <diffuse_tex>;
  Texture[2] = <luminosity_tex>;
  Texture[3] = <shininess_tex>;
  //c0 - intensity of luminosity texture
  //c1 - gloss/shine compound intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   tex t3
   mad r0,t2,c0,v0
   mul r0.rgb,r0,t0
   +mul r0.a,t0,v0
   mul r0,r0,t1
   mad r0.rgb,c1,t3,r0
  };
 }
 pass p1
 {
  AlphaBlendEnable = True;
  BlendOp = Add;
  SrcBlend = SrcAlpha;
  DestBlend = InvSrcAlpha;

  Texture[0] = <environment_tex>;

  //c0 - reflectivity factor
  //c1 - environonment intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   mul r0.rgb,t0,c1
   +mov r0.a,c0
  };
 }
}

technique color_gloss_shine_env_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <gloss_tex>;
  Texture[2] = <shininess_tex>;
  Texture[3] = <environment_tex>;
  //c0 - gloss/shine compound intensity
  //c1 - reflectivity factor
  //c2 - environonment intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   tex t3
   mul r0,t0,v0
   mul t2,t2,c0
   mad r0.rgb,t1,t2,r0
   mul t3,t3,c2
   lrp r0.rgb,c1,t3,r0
  };
 }
}

technique color_dif_gloss_shine_env_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <diffuse_tex>;
  Texture[2] = <gloss_tex>;
  Texture[3] = <shininess_tex>;
  //c0 - gloss/shine compound intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   tex t3
   mul r0,t0,v0
   mul r0,r0,t1
   mul t3,t3,c0
   mad r0.rgb,t2,t3,r0
  };
 }
 pass p1
 {
  AlphaBlendEnable = True;
  BlendOp = Add;
  SrcBlend = SrcAlpha;
  DestBlend = InvSrcAlpha;

  Texture[0] = <environment_tex>;

  //c0 - reflectivity factor
  //c1 - environonment intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   mul r0.rgb,t0,c1
   +mov r0.a,c0
  };
 }
}

technique color_lumi_gloss_shine_env_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <luminosity_tex>;
  Texture[2] = <gloss_tex>;
  Texture[3] = <shininess_tex>;
  //c0 - intensity of luminosity texture
  //c1 - gloss/shine compound intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   tex t3
   mad r0,t1,c0,v0
   mul r0.rgb,r0,t0
   +mul r0.a,t0,v0
   mul t3,t3,c1
   mad r0.rgb,t2,t3,r0
  };
 }
 pass p1
 {
  AlphaBlendEnable = True;
  BlendOp = Add;
  SrcBlend = SrcAlpha;
  DestBlend = InvSrcAlpha;

  Texture[0] = <environment_tex>;

  //c0 - reflectivity factor
  //c1 - environonment intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   mul r0.rgb,t0,c1
   +mov r0.a,c0
  };
 }
}

technique color_dif_lumi_gloss_shine_env_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <diffuse_tex>;
  Texture[2] = <luminosity_tex>;
  //c0 - intensity of luminosity texture
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   mad r0,t2,c0,v0
   mul r0.rgb,r0,t0
   +mul r0.a,t0,v0
   mul r0,r0,t1
  };
 }
 pass p1
 {
  AlphaBlendEnable = True;
  BlendOp = Add;
  SrcBlend = Zero;
  DestBlend = One;

  Texture[0] = <gloss_tex>;
  Texture[1] = <shininess_tex>;
  //c0 - gloss/shine compound intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mul r0,t0,t1
   mul r0,r0,c0
  };
 }
 pass p2
 {
  AlphaBlendEnable = True;
  BlendOp = Add;
  SrcBlend = SrcAlpha;
  DestBlend = InvSrcAlpha;

  Texture[0] = <environment_tex>;

  //c0 - reflectivity factor
  //c1 - environonment intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   mul r0.rgb,t0,c1
   +mov r0.a,c0
  };
 }
}


technique color_refl_env_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <reflectivity_tex>;
  Texture[2] = <environment_tex>;
  //c0 - reflectivity factor
  //c1 - environonment intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   mul r0,t0,v0
   mul t1,t1,c0
   mul t2,t2,c1
   lrp r0.rgb,t1,t2,r0
  };
 }
}


technique color_dif_refl_env_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <diffuse_tex>;
  Texture[2] = <reflectivity_tex>;
  Texture[3] = <environment_tex>;
  //c0 - reflectivity factor
  //c1 - environonment intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   tex t3
   mul r0,t0,v0
   mul r0,r0,t1
   mul t2,t2,c0
   mul t3,t3,c1
   lrp r0.rgb,t2,t3,r0
  };
 }
}

technique color_lumi_refl_env_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <luminosity_tex>;
  Texture[2] = <reflectivity_tex>;
  Texture[3] = <environment_tex>;
  //c0 - luminosity
  //c1 - reflectivity factor
  //c2 - environonment intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   tex t3
   mad r0,c0,t1,v0
   mul r0.rgb,r0,t0
   +mul r0.a,t0,v0
   mul t2,t2,c1
   mul t3,t3,c2
   lrp r0.rgb,t2,t3,r0
  };
 }
}

technique color_dif_lumi_refl_env_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <diffuse_tex>;
  Texture[2] = <luminosity_tex>;
  //c0 - intensity of luminosity texture
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   mad r0,t2,c0,v0
   mul r0.rgb,r0,t0
   +mul r0.a,t0,v0
   mul r0,r0,t1
  };
 }
 pass p1
 {
  AlphaBlendEnable = True;
  BlendOp = Add;
  SrcBlend = SrcAlpha;
  DestBlend = InvSrcAlpha;

  Texture[0] = <reflectivity_tex>;
  Texture[1] = <environment_tex>;
  //c0 - reflectivity factor
  //c1 - environonment intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mul r0.rgb,t1,c1
   +mul r0.a,t0,c0
  };
 }
}

technique color_shine_refl_env_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <shininess_tex>;
  Texture[2] = <reflectivity_tex>;
  Texture[3] = <environment_tex>;
  //c0 - gloss/shine compound intensity
  //c1 - reflectivity factor
  //c2 - environonment intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   tex t3
   mul r0,t0,v0
   mad r0.rgb,c0,t1,r0
   mul t2,t2,c1
   mul t3,t3,c2
   lrp r0.rgb,t2,t3,r0
  };
 }
}

technique color_dif_shine_refl_env_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <diffuse_tex>;
  Texture[2] = <shininess_tex>;
  //c0 - gloss/shine compound intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   mul r0,t0,v0
   mul r0,r0,t1
   mad r0.rgb,c0,t2,r0
  };
 }
 pass p1
 {
  AlphaBlendEnable = True;
  BlendOp = Add;
  SrcBlend = SrcAlpha;
  DestBlend = InvSrcAlpha;

  Texture[0] = <reflectivity_tex>;
  Texture[1] = <environment_tex>;
  //c0 - reflectivity factor
  //c1 - environonment intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mul r0.rgb,t1,c1
   +mul r0.a,t0,c0
  };
 }
}

technique color_lumi_shine_refl_env_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <luminosity_tex>;
  Texture[2] = <shininess_tex>;
  //c0 - intensity of luminosity texture
  //c1 - gloss/shine compound intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   mad r0,t1,c0,v0
   mul r0.rgb,r0,t0
   +mul r0.a,t0,v0
   mad r0.rgb,c1,t2,r0
  };
 }
 pass p1
 {
  AlphaBlendEnable = True;
  BlendOp = Add;
  SrcBlend = SrcAlpha;
  DestBlend = InvSrcAlpha;

  Texture[0] = <reflectivity_tex>;
  Texture[1] = <environment_tex>;
  //c0 - reflectivity factor
  //c1 - environonment intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mul r0.rgb,t1,c1
   +mul r0.a,t0,c0
  };
 }
}

technique color_dif_lumi_shine_refl_env_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <diffuse_tex>;
  Texture[2] = <luminosity_tex>;
  Texture[3] = <shininess_tex>;
  //c0 - intensity of luminosity texture
  //c1 - gloss/shine compound intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   tex t3
   mad r0,t2,c0,v0
   mul r0.rgb,r0,t0
   +mul r0.a,t0,v0
   mul r0,r0,t1
   mad r0.rgb,c1,t3,r0
  };
 }
 pass p1
 {
  AlphaBlendEnable = True;
  BlendOp = Add;
  SrcBlend = SrcAlpha;
  DestBlend = InvSrcAlpha;

  Texture[0] = <reflectivity_tex>;
  Texture[1] = <environment_tex>;
  //c0 - reflectivity factor
  //c1 - environonment intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mul r0.rgb,t1,c1
   +mul r0.a,t0,c0
  };
 }
}

technique color_gloss_shine_refl_env_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <gloss_tex>;
  Texture[2] = <shininess_tex>;
  //c0 - gloss/shine compound intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   mul r0,t0,v0
   mul t2,t2,c0
   mad r0.rgb,t1,t2,r0
  };
 }
 pass p1
 {
  AlphaBlendEnable = True;
  BlendOp = Add;
  SrcBlend = SrcAlpha;
  DestBlend = InvSrcAlpha;

  Texture[0] = <reflectivity_tex>;
  Texture[1] = <environment_tex>;
  //c0 - reflectivity factor
  //c1 - environonment intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mul r0.rgb,t1,c1
   +mul r0.a,t0,c0
  };
 }
}

technique color_dif_gloss_shine_refl_env_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <diffuse_tex>;
  Texture[2] = <gloss_tex>;
  Texture[3] = <shininess_tex>;
  //c0 - gloss/shine compound intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   tex t3
   mul r0,t0,v0
   mul r0,r0,t1
   mul t3,t3,c0
   mad r0.rgb,t2,t3,r0
  };
 }
 pass p1
 {
  AlphaBlendEnable = True;
  BlendOp = Add;
  SrcBlend = SrcAlpha;
  DestBlend = InvSrcAlpha;

  Texture[0] = <reflectivity_tex>;
  Texture[1] = <environment_tex>;
  //c0 - reflectivity factor
  //c1 - environonment intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mul r0.rgb,t1,c1
   +mul r0.a,t0,c0
  };
 }
}

technique color_lumi_gloss_shine_refl_env_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <luminosity_tex>;
  Texture[2] = <gloss_tex>;
  Texture[3] = <shininess_tex>;
  //c0 - intensity of luminosity texture
  //c1 - gloss/shine compound intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   tex t3
   mad r0,t1,c0,v0
   mul r0.rgb,r0,t0
   +mul r0.a,t0,v0
   mul t3,t3,c1
   mad r0.rgb,t2,t3,r0
  };
 }
 pass p1
 {
  AlphaBlendEnable = True;
  BlendOp = Add;
  SrcBlend = SrcAlpha;
  DestBlend = InvSrcAlpha;

  Texture[0] = <reflectivity_tex>;
  Texture[1] = <environment_tex>;
  //c0 - reflectivity factor
  //c1 - environonment intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mul r0.rgb,t1,c1
   +mul r0.a,t0,c0
  };
 }
}

technique color_dif_lumi_gloss_shine_refl_env_ps11
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <diffuse_tex>;
  Texture[2] = <luminosity_tex>;
  //c0 - intensity of luminosity texture
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   tex t2
   mad r0,t2,c0,v0
   mul r0.rgb,r0,t0
   +mul r0.a,t0,v0
   mul r0,r0,t1
  };
 }
 pass p1
 {
  AlphaBlendEnable = True;
  BlendOp = Add;
  SrcBlend = One;
  DestBlend = One;

  Texture[0] = <gloss_tex>;
  Texture[1] = <shininess_tex>;
  //c0 - gloss/shine compound intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mul r0,t0,t1
   mul r0,r0,v0
   mul r0,r0,c0
  };
 }
 pass p2
 {
  AlphaBlendEnable = True;
  BlendOp = Add;
  SrcBlend = SrcAlpha;
  DestBlend = InvSrcAlpha;

  Texture[0] = <reflectivity_tex>;
  Texture[1] = <environment_tex>;
  //c0 - reflectivity factor
  //c1 - environonment intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mul r0.rgb,t1,c1
   +mul r0.a,t0,c0
  };
 }
}

//
//
//
// Pixel shader 1.4 optimized techniques
//
//
//

technique color_dif_lumi_gloss_shine_ps14
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <diffuse_tex>;
  Texture[2] = <luminosity_tex>;
  Texture[3] = <gloss_tex>;
  Texture[4] = <shininess_tex>;

  //c0 - intensity of luminosity texture
  //c1 - gloss/shine compound intensity
  PixelShader =
  asm {
   ps_1_4
   texld r0,t0
   texld r1,t1
   texld r2,t2
   texld r3,t3
   texld r4,t4
   mad r2,r2,c0,v0
   mul r0.rgb,r0,r2
   +mul r0.a,r0,v0
   mul r0,r0,r1
   mul r3,r3,c1
   mad r0.rgb,r3,r4,r0
  };
 }
}

technique color_dif_lumi_shine_env_ps14
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <diffuse_tex>;
  Texture[2] = <luminosity_tex>;
  Texture[3] = <shininess_tex>;
  Texture[4] = <environment_tex>;

  //c0 - intensity of luminosity texture
  //c1 - gloss/shine compound intensity
  //c2 - reflectivity factor
  //c3 - environonment intensity
  PixelShader =
  asm {
   ps_1_4
   texld r0,t0
   texld r1,t1
   texld r2,t2
   texld r3,t3
   texld r4,t4
   mad r2,r2,c0,v0
   mul r0.rgb,r0,r2
   +mul r0.a,r0,v0
   mul r3,r3,c1
   mad r0.rgb,r0,r1,r3
   +mul r0.a,r0,r1
   mul r4,r4,c3
   lrp r0.rgb,c2,r4,r0
  };
 }
}

technique color_dif_gloss_shine_env_ps14
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <diffuse_tex>;
  Texture[2] = <gloss_tex>;
  Texture[3] = <shininess_tex>;
  Texture[4] = <environment_tex>;

  //c0 - gloss/shine compound intensity
  //c1 - reflectivity factor
  //c2 - environonment intensity
  PixelShader =
  asm {
   ps_1_4
   texld r0,t0
   texld r1,t1
   texld r2,t2
   texld r3,t3
   texld r4,t4
   mul r0,r0,v0
   mul r0,r0,r1
   mul r2,r2,c0
   mad r0.rgb,r2,r3,r0
   mul r4,r4,c2
   lrp r0.rgb,c1,r4,r0
  };
 }
}


technique color_lumi_gloss_shine_env_ps14
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <luminosity_tex>;
  Texture[2] = <gloss_tex>;
  Texture[3] = <shininess_tex>;
  Texture[4] = <environment_tex>;

  //c0 - intensity of luminosity texture
  //c1 - gloss/shine compound intensity
  //c2 - reflectivity factor
  //c3 - environonment intensity
  PixelShader =
  asm {
   ps_1_4
   texld r0,t0
   texld r1,t1
   texld r2,t2
   texld r3,t3
   texld r4,t4
   mad r1,r1,c0,v0
   mul r0.rgb,r0,r2
   +mul r0.a,r0,v0
   mul r2,r2,c1
   mad r0.rgb,r2,r3,r0
   mul r4,r4,c3
   lrp r0.rgb,c2,r4,r0
  };
 }
}

technique color_dif_lumi_gloss_shine_env_ps14
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <diffuse_tex>;
  Texture[2] = <luminosity_tex>;
  Texture[3] = <gloss_tex>;
  Texture[4] = <shininess_tex>;
  Texture[5] = <environment_tex>;

  //c0 - intensity of luminosity texture
  //c1 - gloss/shine compound intensity
  //c2 - reflectivity factor
  //c3 - environonment intensity
  PixelShader =
  asm {
   ps_1_4
   texld r0,t0
   texld r1,t1
   texld r2,t2
   texld r3,t3
   texld r4,t4
   texld r5,t5
   mad r2,r2,c0,v0
   mul r0.rgb,r0,r2
   +mul r0.a,r0,v0
   mul r0,r0,r1
   mul r3,r3,c1
   mad r0.rgb,r3,r4,r0
   mul r5,r5,c3
   lrp r0.rgb,c2,r5,r0
  };
 }
}

technique color_dif_lumi_refl_env_ps14
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <diffuse_tex>;
  Texture[2] = <luminosity_tex>;
  Texture[3] = <reflectivity_tex>;
  Texture[4] = <environment_tex>;

  //c0 - intensity of luminosity texture
  //c1 - reflectivity factor
  //c2 - environonment intensity
  PixelShader =
  asm {
   ps_1_4
   texld r0,t0
   texld r1,t1
   texld r2,t2
   texld r3,t3
   texld r4,t4
   mad r2,r2,c0,v0
   mul r0.rgb,r0,r2
   +mul r0.a,r0,v0
   mul r0,r0,r1
   mul r3,r3,c1
   mul r4,r4,c2
   lrp r0.rgb,r3,r4,r0
  };
 }
}

technique color_dif_shine_refl_env_ps14
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <diffuse_tex>;
  Texture[2] = <shininess_tex>;
  Texture[3] = <reflectivity_tex>;
  Texture[4] = <environment_tex>;

  //c0 - gloss/shine compound intensity
  //c1 - reflectivity factor
  //c2 - environonment intensity
  PixelShader =
  asm {
   ps_1_4
   texld r0,t0
   texld r1,t1
   texld r2,t2
   texld r3,t3
   texld r4,t4
   mul r0,r0,v0
   mul r2,r2,c0
   mad r0.rgb,r0,r1,r2
   +mul r0.a,r0,r1
   mul r3,r3,c1
   mul r4,r4,c2
   lrp r0.rgb,r3,r4,r0
  };
 }
}

technique color_lumi_shine_refl_env_ps14
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <luminosity_tex>;
  Texture[2] = <shininess_tex>;
  Texture[3] = <reflectivity_tex>;
  Texture[4] = <environment_tex>;

  //c0 - intensity of luminosity texture
  //c1 - gloss/shine compound intensity
  //c2 - reflectivity factor
  //c3 - environonment intensity
  PixelShader =
  asm {
   ps_1_4
   texld r0,t0
   texld r1,t1
   texld r2,t2
   texld r3,t3
   texld r4,t4
   mad r1,r1,c0,v0
   mul r2,r2,c1
   mad r0.rgb,r0,r1,r2
   +mul r0.a,r0,v0
   mul r3,r3,c2
   mul r4,r4,c3
   lrp r0.rgb,r3,r4,r0
  };
 }
}

technique color_dif_lumi_shine_refl_env_ps14
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <diffuse_tex>;
  Texture[2] = <luminosity_tex>;
  Texture[3] = <shininess_tex>;
  Texture[4] = <reflectivity_tex>;
  Texture[5] = <environment_tex>;

  //c0 - intensity of luminosity texture
  //c1 - gloss/shine compound intensity
  //c2 - reflectivity factor
  //c3 - environonment intensity
  PixelShader =
  asm {
   ps_1_4
   texld r0,t0
   texld r1,t1
   texld r2,t2
   texld r3,t3
   texld r4,t4
   texld r5,t5
   mad r2,r2,c0,v0
   mul r0.rgb,r0,r2
   +mul r0.a,r0,v0
   mul r3,r3,c1
   mad r0.rgb,r0,r1,r3
   +mul r0.a,r0,r1
   mul r4,r4,c2
   mul r5,r5,c3
   lrp r0.rgb,r4,r5,r0
  };
 }
}

technique color_gloss_shine_refl_env_ps14
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <gloss_tex>;
  Texture[2] = <shininess_tex>;
  Texture[3] = <reflectivity_tex>;
  Texture[4] = <environment_tex>;

  //c0 - gloss/shine compound intensity
  //c1 - reflectivity factor
  //c2 - environonment intensity
  PixelShader =
  asm {
   ps_1_4
   texld r0,t0
   texld r1,t1
   texld r2,t2
   texld r3,t3
   texld r4,t4
   mul r0,r0,v0
   mul r1,r1,c0
   mad r0.rgb,r1,r2,r0
   mul r3,r3,c1
   mul r4,r4,c2
   lrp r0.rgb,r3,r4,r0
  };
 }
}

technique color_dif_gloss_shine_refl_env_ps14
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <diffuse_tex>;
  Texture[2] = <gloss_tex>;
  Texture[3] = <shininess_tex>;
  Texture[4] = <reflectivity_tex>;
  Texture[5] = <environment_tex>;

  //c0 - gloss/shine compound intensity
  //c1 - reflectivity factor
  //c2 - environonment intensity
  PixelShader =
  asm {
   ps_1_4
   texld r0,t0
   texld r1,t1
   texld r2,t2
   texld r3,t3
   texld r4,t4
   texld r5,t5
   mul r0,r0,v0
   mul r0,r0,r1
   mul r2,r2,c0
   mad r0.rgb,r2,r3,r0
   mul r4,r4,c1
   mul r5,r5,c2
   lrp r0.rgb,r4,r5,r0
  };
 }
}

technique color_lumi_gloss_shine_refl_env_ps14
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <luminosity_tex>;
  Texture[2] = <gloss_tex>;
  Texture[3] = <shininess_tex>;
  Texture[4] = <reflectivity_tex>;
  Texture[5] = <environment_tex>;

  //c0 - intensity of luminosity texture
  //c1 - gloss/shine compound intensity
  //c2 - reflectivity factor
  //c3 - environonment intensity
  PixelShader =
  asm {
   ps_1_4
   texld r0,t0
   texld r1,t1
   texld r2,t2
   texld r3,t3
   texld r4,t4
   texld r5,t5
   mad r1,r1,c0,v0
   mul r0.rgb,r0,r1
   +mul r0.a,r0,v0
   mul r2,r2,c1
   mad r0.rgb,r2,r3,r0
   mul r4,r4,c2
   mul r5,r5,c3
   lrp r0.rgb,r4,r5,r0
  };
 }
}

//the only two pass ps 1.4 technique
technique color_dif_lumi_gloss_shine_refl_env_ps14
{
 pass p0
 {
  Texture[0] = <color_tex>;
  Texture[1] = <diffuse_tex>;
  Texture[2] = <luminosity_tex>;
  Texture[3] = <gloss_tex>;
  Texture[4] = <shininess_tex>;

  //c0 - intensity of luminosity texture
  //c1 - gloss/shine compound intensity
  PixelShader =
  asm {
   ps_1_4
   texld r0,t0
   texld r1,t1
   texld r2,t2
   texld r3,t3
   texld r4,t4
   mad r2,r2,c0,v0
   mul r0.rgb,r0,r2
   +mul r0.a,r0,v0
   mul r0,r0,r1
   mul r3,r3,c1
   mad r0.rgb,r3,r4,r0
  };
 }
 pass p1
 {
  AlphaBlendEnable = True;
  BlendOp = Add;
  SrcBlend = SrcAlpha;
  DestBlend = InvSrcAlpha;

  Texture[0] = <reflectivity_tex>;
  Texture[1] = <environment_tex>;
  //c0 - reflectivity factor
  //c1 - environonment intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mul r0.rgb,t1,c1
   +mul r0.a,t0,c0
  };
 }
}
