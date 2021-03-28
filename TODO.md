# TODO List

## Core

### Graphics
Full 3D pipeline (lighting, shadows):
- Implement shadows by shadow mapping
- Implement shadows by ray tracing
- Implement shadows by volumes (maybe?)
- Define and implement a complete material interface
- Implement cheap lighting
- Implement PBR lighting
- Antialiasing
- Ambient occlusion
- Screen space reflections
- Ray traced reflections
- Vulkan backend

### Misc
- Data IO api
- Make a configuration editor
- Find a solution for versioning resources and enable hash cache

## Launcher

### Game Selection Screen
A 3-wide grid of cells. Each cell corresponds to a game, with a 16:9 image (screenshot or art) and label. Selecting the game brings up a screen with more details and a button to load it. There should also be a way to access games quickly.

### Misc
At the moment, the code for all games will be included in the executable for the collection. At some point, the games should be separated into libraries that are dynamically loaded by the launcher.

## Agorafold
- Prototype first area
- Basic story introduction
- Level design
- Splash screen
- Aesthetic: garage (or basement?)
- To model:
- Light bulb
- Workbench
- Some tools (which ones?)
- Fold machine
- Outline story and progression
