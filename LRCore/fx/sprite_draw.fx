
technique sprite_blend_ps11
{
 pass p0
 {
  ZENABLE=FALSE;
  FOGENABLE=FALSE;
  LIGHTING=FALSE;
  ALPHATESTENABLE=FALSE;
  ALPHABLENDENABLE=TRUE;
  SRCBLEND=SRCALPHA;
  DESTBLEND=INVSRCALPHA;
  BLENDOP=ADD;

  //c0 - intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   mov r0.rgb,t0
   +mul r0.a,t0,c0
  };
 }
}

technique sprite_add_ps11
{
 pass p0
 {
  ZENABLE=FALSE;
  FOGENABLE=FALSE;
  LIGHTING=FALSE;
  ALPHATESTENABLE=FALSE;
  ALPHABLENDENABLE=TRUE;
  SRCBLEND=SRCALPHA;
  DESTBLEND=ONE;
  BLENDOP=ADD;

  //c0 - intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   mov r0.rgb,t0
   +mul r0.a,t0,c0
  };
 }
}

technique sprite_constant_blend_ps11
{
 pass p0
 {
  ZENABLE=FALSE;
  FOGENABLE=FALSE;
  LIGHTING=FALSE;
  ALPHATESTENABLE=FALSE;
  ALPHABLENDENABLE=TRUE;
  SRCBLEND=SRCALPHA;
  DESTBLEND=INVSRCALPHA;
  BLENDOP=ADD;

  //c0 - intensity
  PixelShader =
  asm {
   ps_1_1
   tex t0
   mov r0.rgb,t0
   +mov r0.a,c0
  };
 }
}
