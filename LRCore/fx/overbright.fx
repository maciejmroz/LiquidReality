
//lrp(c,tx,8*(tx+0.125*c))
technique overbright_x8_ps14
{
 pass p0
 {
  ZENABLE=FALSE;
  FOGENABLE=FALSE;
  LIGHTING=FALSE;
  ALPHATESTENABLE=FALSE;
  ALPHABLENDENABLE=FALSE;

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_4
   texld r0,t0
   mov_d8 r1,c0.a
   add_sat r0,r0,r1
   mov_x8 r1,r0
   lrp_sat r0,c0.a,r1,r0
  };
 }
}

technique overbright_x4_ps14
{
 pass p0
 {
  ZENABLE=FALSE;
  FOGENABLE=FALSE;
  LIGHTING=FALSE;
  ALPHATESTENABLE=FALSE;
  ALPHABLENDENABLE=FALSE;

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_4
   texld r0,t0
   mov_d4 r1,c0.a
   add_sat r0,r0,r1
   mov_x4 r1,r0
   lrp_sat r0,c0.a,r1,r0
  };
 }
}

technique overbright_x2_ps14
{
 pass p0
 {
  ZENABLE=FALSE;
  FOGENABLE=FALSE;
  LIGHTING=FALSE;
  ALPHATESTENABLE=FALSE;
  ALPHABLENDENABLE=FALSE;

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_4
   texld r0,t0
   mov_d2 r1,c0.a
   add_sat r0,r0,r1
   mov_x2 r1,r0
   lrp_sat r0,c0.a,r1,r0
  };
 }
}

//we don't have enough range on ps_1_1 - need to remap
//8*lrp(c,tx/8,tx+0.125*c))
technique overbright_x8_ps11
{
 pass p0
 {
  ZENABLE=FALSE;
  FOGENABLE=FALSE;
  LIGHTING=FALSE;
  ALPHATESTENABLE=FALSE;
  ALPHABLENDENABLE=FALSE;

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_1
   def c1,0.125,0.125,0.125,0.125
   tex t0
   mul r1,c1,t0
   mad_sat r0,c1,c0.a,t0
   lrp r1,c0,r0,r1
   mov_x4_sat r0,r1
   mov_x2_sat r0,r0
  };
 }
}

technique overbright_x4_ps11
{
 pass p0
 {
  ZENABLE=FALSE;
  FOGENABLE=FALSE;
  LIGHTING=FALSE;
  ALPHATESTENABLE=FALSE;
  ALPHABLENDENABLE=FALSE;

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_1
   def c1,0.25,0.25,0.25,0.25
   tex t0
   mul r1,c1,t0
   mad_sat r0,c1,c0.a,t0
   lrp r1,c0,r0,r1
   mov_x4_sat r0,r1
  };
 }
}

technique overbright_x2_ps11
{
 pass p0
 {
  ZENABLE=FALSE;
  FOGENABLE=FALSE;
  LIGHTING=FALSE;
  ALPHATESTENABLE=FALSE;
  ALPHABLENDENABLE=FALSE;

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_1
   def c1,0.5,0.5,0.5,0.5
   tex t0
   mul r1,c1,t0
   mad_sat r0,c1,c0.a,t0
   lrp r1,c0,r0,r1
   mov_x2_sat r0,r1
  };
 }
}

//we need separate ps2_0 version because there is
//ps2.0 hw that has ps1.4 range of -2..2 :(
technique overbright_ps20
{
 pass p0
 {
  ZENABLE=FALSE;
  FOGENABLE=FALSE;
  LIGHTING=FALSE;
  ALPHATESTENABLE=FALSE;
  ALPHABLENDENABLE=FALSE;

  //c0 - shift factor
  //c1 - inverse overbright strength
  //c2 - overbright strength
  PixelShader =
  asm {
   ps_2_0
   dcl t0
   dcl_2d s0
   texld r0,t0,s0
   mov r2,c0.a
   mul r1,r2,c1
   add_sat r0,r0,r1
   mul r1,r0,c2
   lrp_sat r2,c0.a,r1,r0
   mov oC0,r2
  };
 }
}