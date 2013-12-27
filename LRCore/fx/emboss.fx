//simple emboss filter
//with post multiply by color
technique emboss_ps11
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
   mov oT0.xy,v1
   mov oT1.x,v1
   sub oT1.y,v1,c0
   mov oT2.y,v1
   sub oT2.x,v1,c0
  };

  //t0 - center sample
  //t1 - top sample
  //t2 - left sample
  //difference is in -1..1 range
  PixelShader =
  asm {
   ps_1_1
   def c1,0.25,0.25,0.25,0.25
   def c2,0.25,0.25,0.25,0.25
   def c3,0.3,0.59,0.11,0.0
   tex t0
   tex t1
   tex t2
   sub r0,t0,t2
   dp3 r0,r0,c3
   sub r1,t0,t1
   dp3 r1,r1,c3
   mad r0,r0,c1,c2
   mad r1,r1,c1,c2
   add r0,r0,r1
   mul r0,r0,c0		//externally supplied modulate factor
  };
 }
}

technique edge_outliner_ps14
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
   mov oT0.xy,v1
   mov oT1.x,v1
   sub oT1.y,v1,c0
   mov oT2.y,v1
   sub oT2.x,v1,c0
  };

  //t0 - center sample
  //t1 - top sample
  //t2 - left sample
  //c0 - treshold value
  //c1 - outline color
  PixelShader =
  asm {
   ps_1_4
   def c5,0.5,0.5,0.5,0.5
   def c6,0.25,0.25,0.25,0.25
   def c7,0.3,0.59,0.11,0.0
   texld r0,t0
   texld r1,t1
   texld r2,t2
   sub r5,r0,r2
   dp3 r5,r5,c7
   sub r4,r0,r1
   dp3 r4,r4,c7
   mad r4,r4,c6,c6
   mad r5,r5,c6,c6
   add r5,r4,r5
   phase
   texld r0,t0			//reload rexture to get alpha back, hack to overcome hw limitations
   mov r5.a,r5.r		//hack  again ... must do something about it
   sub r4,r5,c5
   cnd r4,r5,r4,-r4
   sub r4,r4,c0
   add r4,r4,c5
   cnd r0,r4,c1,r0
  };
 }
}