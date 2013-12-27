//pixel shaders for alpha compositing (version for premultiplied alpha)
//camera effect
//src==input rt
//dst==effect param texture

//both rgb and a are calculated with source alpha
technique premul_alpha_compose_src_src_ps11
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

  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mad r0.rgb,1-t0.a,t1,t0
   +mov r0.a,t0.a
  };
 }
}

technique premul_alpha_compose_src_dst_ps11
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

  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mad r0.rgb,1-t0.a,t1,t0
   +mov r0.a,t1.a
  };
 }
}

technique premul_alpha_compose_dst_src_ps11
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

  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mad r0.rgb,1-t1.a,t0,t1
   +mov r0.a,t0.a
  };
 }
}

technique premul_alpha_compose_dst_dst_ps11
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

  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mad r0.rgb,1-t1.a,t0,t1
   +mov r0.a,t1.a
  };
 }
}
