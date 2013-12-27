//pixel shaders for texture masking
//camera effect
//src==input rt
//dst==effect param texture

//mask using texture's rgb, alpha unchanged
technique mask_rgb_ps11
{
 pass p0
 {
  ZENABLE=FALSE;
  FOGENABLE=FALSE;
  LIGHTING=FALSE;
  ALPHATESTENABLE=FALSE;
  ALPHABLENDENABLE=FALSE;

  VertexShader =
  asm {
   vs_1_1
   def c8,1.0,1.0,1.0,1.0
   dcl_position v0
   dcl_texcoord v1
   mov oPos,v0
   mov oT0.xy,v1
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT1.xy,r0,c0
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mul r0.rgb,t0,t1
   +mov r0.a,t0.a
  };
 }
}

//mask using texture's alpha, alpha multiplied
technique mask_alpha_ps11
{
 pass p0
 {
  ZENABLE=FALSE;
  FOGENABLE=FALSE;
  LIGHTING=FALSE;
  ALPHATESTENABLE=FALSE;
  ALPHABLENDENABLE=FALSE;

  VertexShader =
  asm {
   vs_1_1
   def c8,1.0,1.0,1.0,1.0
   dcl_position v0
   dcl_texcoord v1
   mov oPos,v0
   mov oT0.xy,v1
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT1.xy,r0,c0
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mul r0,t0,t1.a
  };
 }
}

