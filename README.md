# City Rendering Engine

#### This is an engine being developed for a research project at Newcastle University.

It should be able to generate and efficiently render complex virtual environment, being mainly focused on cities and its amenities.


Ideas for the engine:

ShaderParameters: class to set shader parameters before each shader use call. Used to send the shader variables like time, light parameters, etc
Find a way to optimize the draw calls, maybe by grouping meshes with the same shader and texture and in the same area to use only one draw call
Better profiling tools: profile all opengl calls
Group opengl calls in one single OpenglHelper class to make profiling and other modifications to opengl easier
Use NBT file format to save generated city structure
Generate city structure grouping it in "chunks", like Minecraft
Read the first lines of the shaders to  find out the glsl version required, so the engine can detect if the current card/driver supports it
Make Renderer class and keep track of the current VAO, texture and shader being used on opengl, and make every opengl call pass through it
Make a class to help calculate those values that I need an average over the last 60 frames or so, like FPS
Find a method to keep all measures scale-independent, so the game will work fine on all resolutions, and make this transparent to outside the engine
Resource loader what will load resources asynchronously and call a callback function when finished

Small things TODO:
Speed up the Matrix4 multiplication operator