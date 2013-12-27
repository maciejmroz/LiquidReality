//calculate offset map from dynamic heightfield
//then encode it to 0..1 range as required by
//texbem instruction
technique directional_light_ps11
{
 pass p0
 {
  ZENABLE=FALSE;
  FOGENABLE=FALSE;
  LIGHTING=FALSE;
  ALPHATESTENABLE=FALSE;
  ALPHABLENDENABLE=FALSE;

  //c0 - shift factors
  //c1 - light direction
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
   mov oT3.xy,c1
  };

  //t0 - center sample
  //t1 - top sample
  //t2 - left sample
  //difference is in -1..1 range
  //remap and pack to r0.rg
  //c0 - light direction in rg
  //c1 - light color
  PixelShader =
  asm {
   ps_1_1
   def c5,1.0,0.0,0.0,0.0
   def c6,0.0,1.0,0.0,0.0
   def c7,0.3,0.59,0.11,0.0
   tex t0
   tex t1
   tex t2
   texcoord t3
   sub r0,t0,t2
   dp3 r0,r0,c7
   mul r0,r0,c5
   sub r1,t0,t1
   dp3 r1,r1,c7
   mad r0,r1,c6,r0
   dp3_x2_sat r0,r0,t3
   mad_sat r0.rgb,r0,c1,t0
   +mov r0.a,t0.a
  };
 }
}
