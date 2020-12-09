@echo off
set arg1=%1

if %arg1%==1 .\SurfaceReconstruction.exe heightmaps\Heightmap_Mountain\Heightmap_Mountain_30.0.png reconstructed_heightmaps\Heightmap_Mountain_30.0_reconstructed.png heightmaps\Heightmap_Mountain\Heightmap_Mountain_0.png 10.0 noisemaps\noisemap1.png 2.0
if %arg1%==2 .\SurfaceReconstruction.exe heightmaps\Heightmap_Mountain\Heightmap_Mountain_50.0.png reconstructed_heightmaps\Heightmap_Mountain_50.0_reconstructed.png heightmaps\Heightmap_Mountain\Heightmap_Mountain_0.png 10.0 noisemaps\noisemap1.png 2.0

if %arg1%==3 .\SurfaceReconstruction.exe heightmaps\Heightmap_Plateau\Heightmap_Plateau_30.0.png reconstructed_heightmaps\Heightmap_Plateau_30.0_reconstructed.png heightmaps\Heightmap_Plateau\Heightmap_Plateau_0.png 10.0 noisemaps\noisemap1.png 2.0
if %arg1%==4 .\SurfaceReconstruction.exe heightmaps\Heightmap_Plateau\Heightmap_Plateau_50.0.png reconstructed_heightmaps\Heightmap_Plateau_50.0_reconstructed.png heightmaps\Heightmap_Plateau\Heightmap_Plateau_0.png 10.0 noisemaps\noisemap1.png 2.0

if %arg1%==5 .\SurfaceReconstruction.exe heightmaps\Heightmap_Plateau\Heightmap_Plateau_50.0.png reconstructed_heightmaps\Heightmap_Plateau_50.0_reconstructed.png heightmaps\Heightmap_Plateau\Heightmap_Plateau_0.png 10.0 noisemaps\noisemap1.png 2.0
if %arg1%==6 .\SurfaceReconstruction.exe heightmaps\Heightmap_Plateau\Heightmap_Plateau_50.0.png reconstructed_heightmaps\Heightmap_Plateau_50.0_reconstructed.png heightmaps\Heightmap_Plateau\Heightmap_Plateau_0.png 10.0 noisemaps\noisemap2.png 2.0
if %arg1%==7 .\SurfaceReconstruction.exe heightmaps\Heightmap_Plateau\Heightmap_Plateau_50.0.png reconstructed_heightmaps\Heightmap_Plateau_50.0_reconstructed.png heightmaps\Heightmap_Plateau\Heightmap_Plateau_0.png 10.0 noisemaps\noisemap3.png 2.0

if %arg1%==8 .\SurfaceReconstruction.exe heightmaps\Heightmap_Plateau\Heightmap_Plateau_50.0.png reconstructed_heightmaps\Heightmap_Plateau_50.0_reconstructed.png heightmaps\Heightmap_Plateau\Heightmap_Plateau_0.png 10.0 noisemaps\noisemap1.png 2.0
if %arg1%==9 .\SurfaceReconstruction.exe heightmaps\Heightmap_Plateau\Heightmap_Plateau_50.0.png reconstructed_heightmaps\Heightmap_Plateau_50.0_reconstructed.png heightmaps\Heightmap_Plateau\Heightmap_Plateau_0.png 10.0 noisemaps\noisemap1.png 5.0
if %arg1%==10 .\SurfaceReconstruction.exe heightmaps\Heightmap_Plateau\Heightmap_Plateau_50.0.png reconstructed_heightmaps\Heightmap_Plateau_50.0_reconstructed.png heightmaps\Heightmap_Plateau\Heightmap_Plateau_0.png 10.0 noisemaps\noisemap1.png 8.0





