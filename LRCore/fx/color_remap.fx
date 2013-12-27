//ps1.2+ version - single pass
technique color_remap_ps12
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
   def c0,0.3,0.59,0.11,0.0
   dcl_position v0
   dcl_texcoord v1
   mov oPos,v0
   mov oT0.xy,v1
   mov oT1,c0
  };


  //t0 - original texture
  //t1 - gradient map; t1 coords are constant (0.3,0.59,0.11,0.0)
  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_2
   tex t0
   texdp3tex t1,t0
   lrp r0,c0,t1,t0
  };
 }
}
