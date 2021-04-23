# mesh-viewer

OPenGL applications for viewing meshes

## How to build

*Windows*

Open git bash to the directory containing this repository.

```
mesh-viewer $ mkdir build
mesh-viewer $ cd build
mesh-viewer/build $ cmake -G "Visual Studio 16 2019" ..
mesh-viewer/build $ start CS312-MeshViewer.sln
```

Your solution file should contain multiple projects, such as `mesh-viewer`.
To run from the git bash command shell, 

```
mesh-viewer/build $ ../bin/Debug/mesh-viewer
```

*macOS*

Open terminal to the directory containing this repository.

```
mesh-viewer $ mkdir build
mesh-viewer $ cd build
mesh-viewer/build $ cmake ..
mesh-viewer/build $ make
```

To run each program from build, you would type

```
mesh-viewer/build $ ../bin/mesh-viewer
```

**A Simple Mesh Viewer**

Dragging with the left mouse button down to rotate the camera around the origin.

SHIFT+Drag with the left mouse button down should move the camera towards and away from the origin.

<img src="images/viewer1.png" alt="unique" width="500"/>

<img src="images/viewer2.png" alt="unique" width="500"/>

<img src="images/viewer3.png" alt="unique" width="500"/>

<img src="images/viewer4.png" alt="unique" width="500"/>

<img src="images/viewer5.png" alt="unique" width="500"/>

**Phong Shader**

<img src="images/mug_phong.png" alt="unique" width="500"/>

<img src="images/dolphin_phong.png" alt="unique" width="500"/>

**Toon Shader**

<img src="images/mug_toon.png" alt="unique" width="500"/>

<img src="images/dolphin_toon.png" alt="unique" width="500"/>