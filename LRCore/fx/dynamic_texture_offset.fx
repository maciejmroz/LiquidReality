//create offset map from heightmap
//use offset map for normal texbem-style distort
//_much_ slower than simple texbem

//ps1.4+ version - single pass
technique dynamic_texture_offset_ps14
{
 pass p0
 {
  ZENABLE=FALSE;
  FOGENABLE=FALSE;
  LIGHTING=FALSE;
  ALPHATESTENABLE=FALSE;
  ALPHABLENDENABLE=FALSE;

  //c0 - shift factors
  //c1 - adjustment matrix
  VertexShader =
  asm {
   vs_1_1
   def c8,1.0,1.0,1.0,1.0
   dcl_position v0
   dcl_texcoord v1
   mov oPos,v0
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 r1.xy,r0,c1
   mov oT0.xy,r1
   mov oT1.x,r1
   sub oT1.y,r1,c0
   mov oT2.y,r1
   sub oT2.x,r1,c0
   mov oT3.xy,v1
  };

  //t0 - center sample
  //t1 - top sample
  //t2 - left sample
  //t3 - our texture to distort
  PixelShader =
  asm {
   ps_1_4
   def c1,0.3,0.59,0.11,0.0
   texld r0,t0
   texld r1,t1
   texld r2,t2
   texcrd r3.rgb,t3
   dp3 r0,r0,c1
   dp3 r1,r1,c1
   dp3 r2,r2,c1
   sub r0.r,r0,r2
   sub r0.g,r0,r1
   mad_sat r3.rg,r0,c0,r3
   phase
   texld r3,r3.rgb
   mov r0,r3
  };
 }
}

//ps1.4+ version - single pass
technique dynamic_texture_offset_reflect_ps14
{
 pass p0
 {
  ZENABLE=FALSE;
  FOGENABLE=FALSE;
  LIGHTING=FALSE;
  ALPHATESTENABLE=FALSE;
  ALPHABLENDENABLE=FALSE;

  //c0 - shift factors
  //c1 - adjustment matrix
  VertexShader =
  asm {
   vs_1_1
   def c8,1.0,1.0,1.0,1.0
   dcl_position v0
   dcl_texcoord v1
   mov oPos,v0
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 r1.xy,r0,c1
   mov oT0.xy,r1
   mov oT1.x,r1
   sub oT1.y,r1,c0
   mov oT2.y,r1
   sub oT2.x,r1,c0
   mov oT3.xy,v1
  };

  //t0 - center sample
  //t1 - top sample
  //t2 - left sample
  //t3 - our texture to distort
  //t4 - reflection map
  PixelShader =
  asm {
   ps_1_4
   def c1,0.3,0.59,0.11,0.0
   def c2,1.0,1.0,0.0,0.0
   texld r0,t0
   texld r1,t1
   texld r2,t2
   texcrd r3.rgb,t3
   dp3 r5,r0,c1
   dp3 r1,r1,c1
   dp3 r2,r2,c1
   sub r0.r,r5,r2
   sub r0.g,r5,r1
   mad_sat r2.rg,r0,c0,r3
   phase
   texld r3,r2.rgb
   texld r4,r2.rgb
   dp3 r5,r5,c1
   lrp r0,r5.a,r4,r3
  };
 }
}
