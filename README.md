# SMXTOOL
PlayStation RSD and Scarlet SMX model viewer and material editor. Intended to be part of Project Scarlet (not to be confused with the codename of the next Xbox) but due to development delays, this tool has been forked into its own thing for release to make up for the delays and that it can be used without the Scarlet engine as it can edit RSD model data.

This tool is a bit early in development but should be useable for coloring and texturing RSD models. Suggestions for more features or quality of life improvements are welcome.

## Features
* Uses GLSL shaders to emulate PSX style texture brightening (make sure to enable it in settings).
* Supports blended polygons with all blend modes correctly represented.
* Gouraud shaded polygons with unique colors on each vertex.
* Double-sided (no culling) flag can be set on primitives.
* Primitives can be sorted by hand.
* Texture view can be zoomed (no more eyestrain to those who've used RSDTOOL!).
* Can select and manipulate multiple primitives at once.
* Can load and save RSD model files.

## RSD Format Support
SMXTOOL supports Sony's RSD model format used by licensed PlayStation developers for creating 3D models to be rendered on the console using Sony's libraries after converting to the TMD file format. Not all RSD format features are supported but what is supported should be enough for most cases.

The following features of the RSD format are not supported:
* Line and sprite primitives.
* Textured primitives with repeating textures.
* GRP,MSH,PVT,COD and MOT files.

## Tips
* You can double click a primitive to snap the axis point of the camera to the center of the primitive. Most useable in orbital camera view.

## Compiling
You will need the following libraries:
* tinyxml2
* FLTK
* GLEW

On Windows, it is recommended to compile using MinGW. MSVC compatibility is not guaranteed.

Compile with 'mingw32-make CONF=Release' or open the project using Netbeans and compile through that.

## To-do
* Line primitives.
* Supporting STP mask bit on textures (not sure if possible with shaders).
* Vertex paint tool.
* Lighting..?