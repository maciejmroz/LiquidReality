//do full 3x3 (actually 2x3 :) ) transform of texture coordinates
technique transform_m33_ps11
{
 pass p0
 {
  ZENABLE=FALSE;
  FOGENABLE=FALSE;
  LIGHTING=FALSE;
  ALPHATESTENABLE=FALSE;
  ALPHABLENDENABLE=FALSE;

  AddressU[0]=Wrap;
  AddressV[0]=Wrap;

  //c0-c1 texture mtx
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
  };

  PixelShader =
  asm {
   ps_1_1
   tex t0
   mov r0,t0
  };
 }
}