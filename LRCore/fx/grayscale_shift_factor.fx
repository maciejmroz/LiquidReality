
technique grayscale_shift_factor_ps11
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
   def c1,0.3,0.59,0.11,0.0
   tex t0
   tex t1
   dp3 r1,t1,c1
   mul_sat r1,r1,c0
   dp3_sat t1,t0,c1
   lrp_sat r0,r1,t1,t0
  };
 }
}