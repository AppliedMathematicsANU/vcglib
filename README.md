# Fork
This fork of the [Visualization and Computer Graphics Library](https://github.com/cnr-isti-vclab/vcglib) provides
a python API to the *tridecimator* quadric decimation method.

# Install

Requires:

- [boost](https://boost.org) development library, [Boost.Python](https://www.boost.org/doc/libs/release/libs/python/) and [Boost.Numpy](https://www.boost.org/doc/libs/release/libs/python/doc/html/numpy)
- [cmake](https://cmake.org)

```bash
git clone https://github.com/AppliedMathematicsANU/vcglib
SRC_DIR="`pwd`/vcglib"
mkdir vcglib_build
cd vcglib_build
cmake -DCMAKE_INSTALL_PREFIX=$HOME/.local $SRC_DIR
make install
```

# Example Python

```python
import numpy as np
from vcg.apps import tridecimator as td

verts = np.asarray(...)  # (N, 3) shaped array of vertex coordinates
faces = np.asarray(...)  # (M, 3) shaped array of triangular faces (indices in verts).

mesh = td.Mesh(vertices=verts, faces=faces)
td.clean(mesh)  # In-place removal of duplicate vertices and unreferenced vertices
qparams = td.default_q_params()
qparams.PreserveTopology = True
td.decimate(mesh, final_num_faces=faces.shape[0] // 8, qparams=qparams)  # Decimate to quarter number of faces

verts = mesh.vertices  # Decimated vertices
faces = mesh.faces     # Decimated faces

```

# Original README

The **_Visualization and Computer Graphics Library_** (VCGlib for short) is a open source, portable, C++, templated, no dependency, library for manipulation, processing, cleaning, simplifying triangle meshes.

The library, composed by more than 100k lines of code, is released under the GPL license, and it is the base of most of the software tools of the [Visual Computing Lab](http://vcg.isti.cnr.it) of the Italian National Research Council Institute ISTI , like MeshLab, metro and many others.

The VCG library is tailored to mostly manage triangular meshes: The library is fairly large and offers many state of the art functionalities for processing meshes, like:

- high quality quadric-error edge-collapse based simplfication,
- efficient spatial query structures (uniform grids, hashed grids, kdtree, ...) ,
- advanced smoothing and fairing algorithms,
- computation of curvature,
- optimization of texture coordinates,
- Hausdorff distance computation,
- geodesic paths,
- mesh repairing capabilities,
- isosurface extraction and advancing front meshing algorithms,
- Poisson Disk sampling and other tools to sample point distributions over meshes,
- subdivision surfaces

## Notable Applications

A number of applications have been developed using the vcglib:

- MeshLab: the renowed open source mesh processing is based on this library.
- Metro, the tool for measuring differences between meshes
- The first high quality out-of-core mesh simplifier that was used by the Stanford Digital Michelangelo project to process their huge 3D scanned models.

## Contacts

For any info about licensing (portion of) the library please contact us:
Paolo Cignoni (p.cignoni@isti.cnr.it) 
Visual Computing Lab of the Italian National Research Council - ISTI

In case of bugs please report them [here](https://github.com/cnr-isti-vclab/vcglib/issues) .
