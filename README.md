LiquidReality
=========

**LiquidReality** is a Direct3D 9 demo engine I created after Optical demo (which was OpenGL based, and wasn't really using any engine). This time I went to create real engine, you know, with *tools* :)

The vision was grand, but I ended up using it only for two demos (with one essentially acting as a beta test).

What's int the repo:
- LRCore - the engine main dll
- scene_logic_test - an ugly MFC app I never bothered to rename because it was meant to be temporary (but survived). Essentially it is an editor for all engine objects, and contains code to import Lightwave 7.x file formats.
- ActiveX controls used by the editor (LREnvelopeEditor, LRColorBar, LRSpinner).
- some small WTL/ATL based tools (LRPlayer, LRDesigner).
- prototype for "next gen" editor (LRStudio - Windows Forms/Managed C++ app that mostly abandons modality and goes more towards IDE-like approach).

It was pretty cool stuff (even if overengineered):
- All scene entities were created through a factory that was able to create them by class name. Concrete classes only had to adhere to a small set of abstract interfaces, including serialization/deserialization interface. The idea (never fully realized) was to eventually move towards small engine core with scene entities implemented via dynamically loaded plugins. This idea I got from 3D Studio MAX, I think :)
- Everything was data driven - if something was a numeric parameter it could be edited in the tools, and persisted along with the object. Same applied for envelope data (time-varying floating point values), colors (that could always be animated), and motion data. Any time-varying parameter could be modified by mathematical expression.
- Lightwave 6.x/7.x file formats were supported at import (no support for morphing or skeletal animation, though)
- It had it's own file format for geometry, which encapsulated compressed vertex buffers, so no long time preprocessing of Lightwave LWOs. All the pain was handled at import time in the tools. Even with a lot bigger objects/scenes, load times were just a fraction of what was happening in Optical.
- The engine had real tools. Ok, perhaps I am exaggerating a bit, but there was this crude MFC application that allowed to change pretty much anything at runtime, without digging into text files, and with instant feedback. While UI was bad, the idea if instant feedback (standard thing today) was good, and allowed me and my friends to do a lot of experiments we wouldn't otherwise have time to do.
- Pixel shaders were required, with 1.1 used most of the time. I took my time to optimize some multipass scenatios for 1.4 shaders (I had Radeon 8500 card back then). Shaders were used for surface rendering, and very, very heavy postprocessing.
- Vertex processing was still fixed function. Going for pure programmable pipeline was planned for the future, and never happened :)

What happened next? Life happened. While I was stil tinkering with the engine after hours, I was also a full time programmer, so after spending fair amount of time staring into Visual C++ IDE at work going home to do exactly the same seemed less and less attractive.

I begun to remove the unnecessary bloat: smart pointers, wrapper classes around some Direct3D primitives (like vertex buffers), very complex property/runtime reflection system, I simplified file formats for faster serialization/deserialization etc etc. In the mean time I was also working on LRStudio, which was "next gen" tool based on Windows Forms (implemented in Managed C++, some version of it is present in this repository).

After that, Direct3D 10 happended, and I decided that working on D3D9 engine with tons of fixed function stuff in it doesn't make much sense any more. I started tinkering with OpenGL again, but never really got enthusiastic enough about it to make another engine, and another demoscene production.

