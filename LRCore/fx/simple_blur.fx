
technique simple_blur_ps11
{
 pass p0
 {
  ZENABLE=FALSE;
  FOGENABLE=FALSE;
  LIGHTING=FALSE;
  ALPHATESTENABLE=FALSE;
  ALPHABLENDENABLE=FALSE;

  //c0 - shift factors
  VertexShader =
  asm {
   vs_1_1
   dcl_position v0
   dcl_texcoord v1
   mov oPos,v0
   sub oT0.xy,v1,c0
   add oT1.xy,v1,c0
   mov r0.x,c0
   mov r0.y,-c0
   sub oT2.xy,v1,r0
   sub oT3.xy,v1,-r0
  };

  PixelShader =
  asm {
   ps_1_1
   def c0,0.5,0.5,0.5,0.5
   tex t0
   tex t1
   tex t2
   tex t3
   lrp r0,c0,t0,t1
   lrp r1,c0,t2,t3
   lrp r0,c0,r0,r1
  };
 }
}

//do full 3x3 (actually 2x3 :) ) transform of texture coordinates
technique simple_blur_m33_ps11
{
 pass p0
 {
  ZENABLE=FALSE;
  FOGENABLE=FALSE;
  LIGHTING=FALSE;
  ALPHATESTENABLE=FALSE;
  ALPHABLENDENABLE=FALSE;

  //c0-c1 first texture mtx
  //c2-c3 2nd texture mtx
  //c4-c5 3rd texture mtx
  //c6-c7 4th texture mtx
  VertexShader =
  asm {
   vs_1_1
   def c8,1.0,1.0,1.0,1.0
   dcl_position v0
   dcl_texcoord v1
   mov oPos,v0
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT0.xy,r0,c0
   m3x2 oT1.xy,r0,c2
   m3x2 oT2.xy,r0,c4
   m3x2 oT3.xy,r0,c6
  };

  PixelShader =
  asm {
   ps_1_1
   def c0,0.5,0.5,0.5,0.5
   tex t0
   tex t1
   tex t2
   tex t3
   lrp r0,c0,t0,t1
   lrp r1,c0,t2,t3
   lrp r0,c0,r0,r1
  };
 }
}

//glow effect uses this additional shader to perform move+add
technique glow_mad_ps11
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
   dcl_position v0
   dcl_texcoord v1
   mov oPos,v0
   mov oT0.xy,v1
   mov oT1.xy,v1
  };

  //c0 - glow intensity
  //t0 blurred, t1 original
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mad_sat r0,c0,t0,t1
  };
 }
}

//this one artifically amplifies glow strength
//that's all that can be done without true hdr
technique glow_mad_x2_ps11
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
   dcl_position v0
   dcl_texcoord v1
   mov oPos,v0
   mov oT0.xy,v1
   mov oT1.xy,v1
  };

  //c0 - glow intensity
  //t0 blurred, t1 original
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mul_x2_sat r0,c0,t0
   add_sat r0.rgb,r0,t1
   +mad_sat r0.a,c0,t0,t1
  };
 }
}

technique glow_mad_x4_ps11
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
   dcl_position v0
   dcl_texcoord v1
   mov oPos,v0
   mov oT0.xy,v1
   mov oT1.xy,v1
  };

  //c0 - glow intensity
  //t0 blurred, t1 original
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mul_x4_sat r0,c0,t0
   add_sat r0.rgb,r0,t1
   +mad_sat r0.a,c0,t0,t1
  };
 }
}

//TODO: should we cut off alpha channel as well??
technique glow_cutoff_x2_ps11
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
   dcl_position v0
   dcl_texcoord v1
   mov oPos,v0
   mov oT0.xy,v1
  };

  //c0 - glow intensity
  //t0 blurred, t1 original
  PixelShader =
  asm {
   ps_1_1
   def c0,0.5,0.5,0.5,0.0
   tex t0
   sub_x2_sat r0.rgb,t0,c0
   +mov r0.a,t0
  };
 }
}

technique glow_cutoff_x4_ps11
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
   dcl_position v0
   dcl_texcoord v1
   mov oPos,v0
   mov oT0.xy,v1
  };

  //c0 - glow intensity
  //t0 blurred, t1 original
  PixelShader =
  asm {
   ps_1_1
   def c0,0.75,0.75,0.75,0.0
   tex t0
   sub_x4_sat r0.rgb,t0,c0
   +mov r0.a,t0
  };
 }
}

technique blur_compose_color_mask_ps11
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
   dcl_position v0
   dcl_texcoord v1
   mov oPos,v0
   mov oT0.xy,v1
   mov oT1.xy,v1
  };

  //c0 - blur mask
  //t0 blurred, t1 original
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   lrp r0,c0,t0,t1
  };
 }
}


