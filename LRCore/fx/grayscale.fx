
technique grayscale_shift_ps11
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
   def c1,0.3,0.59,0.11,0.0
   tex t0
   dp3_sat r0,t0,c1
   lrp r0,c0,r0,t0
  };
 }
}