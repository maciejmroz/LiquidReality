
technique negative_shift_ps11
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
   tex t0
   lrp r0,c0,1-t0,t0
  };
 }
}