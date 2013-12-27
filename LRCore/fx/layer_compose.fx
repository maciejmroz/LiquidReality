//some basic Photoshop-like layer composition modes
//there is no official info on that available so
//all below are just (though _probably_ accurate) assumptions
//t0 -original
//t1 -overlay
//t1 layer alpha modulates constant factor (c0)


technique normal_a_add_ps11
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
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT0.xy,r0,c0
   m3x2 oT1.xy,r0,c2
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mul_sat r1.a,t1.a,c0
   lrp r0.rgb,r1.a,t1,t0
   +add r0.a,t0.a,r1.a
  };
 }
}

technique normal_a_keep_ps11
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
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT0.xy,r0,c0
   m3x2 oT1.xy,r0,c2
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mul_sat r1.a,t1.a,c0
   lrp r0.rgb,r1.a,t1,t0
   +mov r0.a,t0.a
  };
 }
}

technique normal_a_replace_ps11
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
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT0.xy,r0,c0
   m3x2 oT1.xy,r0,c2
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mul_sat r1.a,t1.a,c0
   lrp r0.rgb,r1.a,t1,t0
   +mov r0.a,r1.a
  };
 }
}

technique darken_a_add_ps11
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
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT0.xy,r0,c0
   m3x2 oT1.xy,r0,c2
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   sub_sat r1.rgb,t1,t0
   +mul_sat r1.a,t1.a,c0
   sub_sat r0.rgb,t1,r1
   lrp r0.rgb,r1.a,r0,t0
   +add r0.a,t0.a,r1.a
  };
 }
}

technique darken_a_keep_ps11
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
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT0.xy,r0,c0
   m3x2 oT1.xy,r0,c2
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   sub_sat r1.rgb,t1,t0
   +mul_sat r1.a,t1.a,c0
   sub_sat r0.rgb,t1,r1
   lrp r0.rgb,r1.a,r0,t0
   +mov r0.a,t0.a
  };
 }
}

technique darken_a_replace_ps11
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
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT0.xy,r0,c0
   m3x2 oT1.xy,r0,c2
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   sub_sat r1.rgb,t1,t0
   +mul_sat r1.a,t1.a,c0
   sub_sat r0.rgb,t1,r1
   lrp r0.rgb,r1.a,r0,t0
   +mov r0.a,r1.a
  };
 }
}

//beware of premultiplied alpha here
technique multiply_a_add_ps11
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
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT0.xy,r0,c0
   m3x2 oT1.xy,r0,c2
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mul r0.rgb,t0,t1
   +mul_sat r0.a,t1.a,c0
   lrp r0.rgb,r0.a,r0,t0
   +add r0.a,t0.a,r0.a
  };
 }
}

technique multiply_a_keep_ps11
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
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT0.xy,r0,c0
   m3x2 oT1.xy,r0,c2
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mul r0.rgb,t0,t1
   +mul_sat r0.a,t1.a,c0
   lrp r0.rgb,r0.a,r0,t0
   +mov r0.a,t0.a
  };
 }
}

technique multiply_a_replace_ps11
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
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT0.xy,r0,c0
   m3x2 oT1.xy,r0,c2
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mul r0.rgb,t0,t1
   +mul_sat r0.a,t1.a,c0
   lrp r0.rgb,r0.a,r0,t0
  };
 }
}

technique lighten_a_add_ps11
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
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT0.xy,r0,c0
   m3x2 oT1.xy,r0,c2
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   sub_sat r1.rgb,t1,t0
   +mul_sat r1.a,t1.a,c0
   add r0.rgb,t0,r1
   lrp r0.rgb,r1.a,r0,t0
   +add r0.a,t0.a,r1.a
  };
 }
}

technique lighten_a_keep_ps11
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
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT0.xy,r0,c0
   m3x2 oT1.xy,r0,c2
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   sub_sat r1.rgb,t1,t0
   +mul_sat r1.a,t1.a,c0
   add r0.rgb,t0,r1
   lrp r0.rgb,r1.a,r0,t0
   +mov r0.a,t0.a
  };
 }
}

technique lighten_a_replace_ps11
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
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT0.xy,r0,c0
   m3x2 oT1.xy,r0,c2
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   sub_sat r1.rgb,t1,t0
   +mul_sat r1.a,t1.a,c0
   add r0.rgb,t0,r1
   lrp r0.rgb,r1.a,r0,t0
   +mov r0.a,r1.a
  };
 }
}

//beware of premultiplied alpha here
technique screen_a_add_ps11
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
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT0.xy,r0,c0
   m3x2 oT1.xy,r0,c2
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mul_sat r0.rgb,1-t0,1-t1
   +mul_sat r0.a,t1.a,c0
   lrp r0.rgb,r0.a,1-r0,t0
   +add r0.a,t0.a,r0.a
  };
 }
}

technique screen_a_keep_ps11
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
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT0.xy,r0,c0
   m3x2 oT1.xy,r0,c2
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mul_sat r0.rgb,1-t0,1-t1
   +mul_sat r0.a,t1.a,c0
   lrp r0.rgb,r0.a,1-r0,t0
   +mov r0.a,t0.a
  };
 }
}

technique screen_a_replace_ps11
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
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT0.xy,r0,c0
   m3x2 oT1.xy,r0,c2
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   mul_sat r0.rgb,1-t0,1-t1
   +mul_sat r0.a,t1.a,c0
   lrp r0.rgb,r0.a,1-r0,t0
   +mov r0.a,r0.a
  };
 }
}

technique difference_a_add_ps11
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
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT0.xy,r0,c0
   m3x2 oT1.xy,r0,c2
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   sub_sat r0.rgb,t0,t1
   sub_sat r1.rgb,t1,t0
   add_sat r1.rgb,r0,r1
   +mul_sat r1.a,t1.a,c0
   lrp r0.rgb,r1.a,r1,t0
   +add r0.a,t0.a,r1.a
  };
 }
}

technique difference_a_keep_ps11
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
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT0.xy,r0,c0
   m3x2 oT1.xy,r0,c2
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   sub_sat r0.rgb,t0,t1
   sub_sat r1.rgb,t1,t0
   add_sat r1.rgb,r0,r1
   +mul_sat r1.a,t1.a,c0
   lrp r0.rgb,r1.a,r1,t0
   +mov r0.a,t0.a
  };
 }
}

technique difference_a_replace_ps11
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
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT0.xy,r0,c0
   m3x2 oT1.xy,r0,c2
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_1
   tex t0
   tex t1
   sub_sat r0.rgb,t0,t1
   sub_sat r1.rgb,t1,t0
   add_sat r1.rgb,r0,r1
   +mul_sat r1.a,t1.a,c0
   lrp r0.rgb,r1.a,r1,t0
   +mov r0.a,r1.a
  };
 }
}

//ps1.4 optimized techniques
technique overlay_a_add_ps14
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
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT0.xy,r0,c0
   m3x2 oT1.xy,r0,c2
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_4
   texld r0,t0
   texld r1,t1
   mul_x2 r2,r0,r1
   mul_x2 r3,1-r0,1-r1
   cnd r2.rgb,r0,1-r3,r2
   +mul_sat r2.a,r1.a,c0
   lrp r0.rgb,r2.a,r2,r0
   +add r0.a,r0.a,r2.a
  };
 }
}

technique overlay_a_keep_ps14
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
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT0.xy,r0,c0
   m3x2 oT1.xy,r0,c2
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_4
   texld r0,t0
   texld r1,t1
   mul_x2 r2,r0,r1
   mul_x2 r3,1-r0,1-r1
   cnd r2.rgb,r0,1-r3,r2
   +mul_sat r2.a,r1.a,c0
   lrp r0.rgb,r2.a,r2,r0
  };
 }
}

technique overlay_a_replace_ps14
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
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT0.xy,r0,c0
   m3x2 oT1.xy,r0,c2
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_4
   texld r0,t0
   texld r1,t1
   mul_x2 r2,r0,r1
   mul_x2 r3,1-r0,1-r1
   cnd r2.rgb,r0,1-r3,r2
   +mul_sat r2.a,r1.a,c0
   lrp r0.rgb,r2.a,r2,r0
   +mov r0.a,r2.a
  };
 }
}

technique hard_light_a_add_ps14
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
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT0.xy,r0,c0
   m3x2 oT1.xy,r0,c2
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_4
   texld r0,t0
   texld r1,t1
   mul_x2 r2,r0,r1
   mul_x2 r3,1-r0,1-r1
   cnd r2.rgb,r1,1-r3,r2
   +mul_sat r2.a,r1.a,c0
   lrp r0.rgb,r2.a,r2,r0
   +add r0.a,r0.a,r2.a
  };
 }
}

technique hard_light_a_keep_ps14
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
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT0.xy,r0,c0
   m3x2 oT1.xy,r0,c2
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_4
   texld r0,t0
   texld r1,t1
   mul_x2 r2,r0,r1
   mul_x2 r3,1-r0,1-r1
   cnd r2.rgb,r1,1-r3,r2
   +mul_sat r2.a,r1.a,c0
   lrp r0.rgb,r2.a,r2,r0
  };
 }
}

technique hard_light_a_replace_ps14
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
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT0.xy,r0,c0
   m3x2 oT1.xy,r0,c2
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_4
   texld r0,t0
   texld r1,t1
   mul_x2 r2,r0,r1
   mul_x2 r3,1-r0,1-r1
   cnd r2.rgb,r1,1-r3,r2
   +mul_sat r2.a,r1.a,c0
   lrp r0.rgb,r2.a,r2,r0
   +mov r0.a,r2.a
  };
 }
}

technique exclusion_a_add_ps14
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
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT0.xy,r0,c0
   m3x2 oT1.xy,r0,c2
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_4
   texld r0,t0
   texld r1,t1
   add r2.rgb,r0,r1
   mul_x2 r1.rgb,r0,r1
   sub_sat r1.rgb,r2,r1
   +mul_sat r1.a,r1.a,c0
   lrp r0.rgb,r1.a,r1,r0
   +add r0.a,r0.a,r1.a
  };
 }
}

technique exclusion_a_keep_ps14
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
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT0.xy,r0,c0
   m3x2 oT1.xy,r0,c2
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_4
   texld r0,t0
   texld r1,t1
   add r2.rgb,r0,r1
   mul_x2 r1.rgb,r0,r1
   sub_sat r1.rgb,r2,r1
   +mul_sat r1.a,r1.a,c0
   lrp r0.rgb,r1.a,r1,r0
  };
 }
}

technique exclusion_a_replace_ps14
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
   mov r0.xy,v1
   mov r0.z,c8
   m3x2 oT0.xy,r0,c0
   m3x2 oT1.xy,r0,c2
  };

  //c0 - shift factor
  PixelShader =
  asm {
   ps_1_4
   texld r0,t0
   texld r1,t1
   add r2.rgb,r0,r1
   mul_x2 r1.rgb,r0,r1
   sub_sat r1.rgb,r2,r1
   +mul_sat r1.a,r1.a,c0
   lrp r0.rgb,r1.a,r1,r0
   +mov r0.a,r1.a
  };
 }
}
