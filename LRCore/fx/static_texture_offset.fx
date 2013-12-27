//static offset is classic screen space embm fith signed dudv map
//t0 - perturbation texture
//t1 - original rendertarget

technique static_texture_offset_ps11
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
   mov oT1.xy,v1
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT0.xy,r0,c0
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_1
   tex t0              ; Define t0 to get a 2-tuple DuDv
   texbem t1, t0       ; Compute (u',v')
                       ; Sample t1 using (u',v')
   mov r0, t1          ; Output result
  };
 }
}