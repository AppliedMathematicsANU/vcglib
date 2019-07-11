"""
Unit tests for vcg.apps.tridecimator.
"""
import unittest as _unittest

have_trimesh = False
try:
    import trimesh as _trimesh
    have_trimesh = True
except BaseException:
    have_trimesh = False

class MeshTest(_unittest.TestCase):
    """
    Test construction of Mesh.
    """
    def test_construct_empty(self):
        """
        """
        from . import Mesh
        
        mesh = Mesh()
        self.assertSequenceEqual([], mesh.vertices.tolist())
        self.assertSequenceEqual([], mesh.faces.tolist())

    def test_construct_one_vertex(self):
        """
        """
        from . import Mesh

        v = [[0.0, 0.0, 0.0],]
        f = []
        mesh = Mesh(vertices=v, faces=f)
        self.assertSequenceEqual(v, mesh.vertices.tolist())
        self.assertSequenceEqual(f, mesh.faces.tolist())

    def test_construct_one_face(self):
        """
        """
        from . import Mesh

        v = [[0.0, 0.0, 0.0], [1.0, 0.0, 0.0], [1.0, 1.0, 0.0]]
        f = [[0, 1, 2],]
        mesh = Mesh(vertices=v, faces=f)
        self.assertSequenceEqual(v, mesh.vertices.tolist())
        self.assertSequenceEqual(f, mesh.faces.tolist())

class DecimateTest(_unittest.TestCase):
    """
    Test construction of Mesh.
    """
    @_unittest.skipUnless(have_trimesh, "Could not import trimesh.")
    def test_decimate(self):
        """
        """
        import numpy as np

        from . import Mesh, decimate
        if hasattr(_trimesh, "io"):
            export_mesh = _trimesh.io.export.export_mesh
        else:
            export_mesh = _trimesh.exchange.export.export_mesh

        t_sphere = _trimesh.primitives.Sphere(radius=1.0, center=(1.0, 2.0, -4.0), subdivisions=4)

        # export_mesh(t_sphere, "t_sphere.ply")

        mesh = Mesh(vertices=t_sphere.vertices, faces=t_sphere.faces)
        # export_mesh(_trimesh.Trimesh(vertices=mesh.vertices, faces=mesh.faces, process=False), "mesh.ply")
        final_num_faces = t_sphere.faces.shape[0] // 4
        decimate(mesh, final_num_faces=final_num_faces)

        # export_mesh(_trimesh.Trimesh(vertices=mesh.vertices, faces=mesh.faces, process=False), "decimated.ply")
        self.assertLessEqual(mesh.num_faces, final_num_faces)
        self.assertLessEqual(mesh.faces.shape[0], final_num_faces)
        self.assertLessEqual(
            np.max(
                np.absolute(
                    _trimesh.proximity.signed_distance(t_sphere, mesh.vertices)
                )
            ),
            0.005
        )


if __name__ == "__main__":
    _unittest.main()
