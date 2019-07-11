
#include <boost/version.hpp>

#if BOOST_VERSION < 106500
#include <boost/python/numeric.hpp>
#else  // #if BOOST_VERSION < 106500
#include <boost/python/numpy.hpp>
#endif

#include <boost/python.hpp>
#include "tridecimator.h"
#include <vcg/complex/allocate.h>

namespace vcg
{
namespace python
{

#if BOOST_VERSION < 106500

namespace bpnp = ::boost::python::numeric;
using BpNumpyArray = typename ::boost::python::numeric::array;

inline void set_module_and_type_wrapper()
{
    BpNumpyArray::set_module_and_type("numpy", "ndarray");
}

#else

namespace bpnp = ::boost::python::numpy;
using BpNumpyArray = typename boost::python::numpy::ndarray;

inline void set_module_and_type_wrapper() {}

#endif

class MyMeshPy: public MyMesh
{
public:
    typedef double CoordScalarType;
    typedef BpNumpyArray ndarray;
    typedef SimpleTempData<MyMeshPy::VertContainer,int> SimpTempData;

    MyMeshPy() : MyMesh()
    {
    }

    MyMeshPy(boost::python::object vertices, boost::python::object faces)
      : MyMesh()
    {
        this->initialise(vertices, faces);
    }

    void initialise(boost::python::object verticesObj, boost::python::object facesObj)
    {
        namespace p = boost::python;
        namespace np = bpnp;

        ndarray vertices = bpnp::array(verticesObj);
        ndarray faces = bpnp::array(facesObj);
        const std::int64_t numVertices = vertices.shape(0);
        const std::int64_t numFaces = faces.shape(0);

        this->Clear();

        {
            VertexIterator vi=vcg::tri::Allocator<MyMeshPy>::AddVertices(*this, numVertices);
            for (std::size_t i = 0; i < numVertices; ++i)
            {
                for (std::size_t j = 0; j < 3; ++j)
                {
                    (*vi).P()[j] = p::extract<CoordScalarType>(vertices[p::make_tuple(i, j)])();
                }
                ++vi;
            }
        }

        {
            FaceIterator fi=vcg::tri::Allocator<MyMeshPy>::AddFaces(*this, numFaces);

            for (std::size_t i = 0; i < numFaces; ++i)
            {
                for (std::size_t j = 0; j < 3; ++j)
                {
                    (*fi).V(j) = &(this->vert[p::extract<int64_t>(faces[p::make_tuple(i, j)])()]);
                }
                ++fi;
            }
        }
    }

    ndarray getVertices()
    {
        namespace p = boost::python;
        namespace np = bpnp;

        p::tuple shape = p::make_tuple(this->vn, 3);
        np::dtype dtype = np::dtype::get_builtin<CoordScalarType>();
        np::ndarray vertices = np::empty(shape, dtype);
        {
            std::size_t i = 0;
            VertexIterator vi;
            for (vi = this->vert.begin(); vi != this->vert.end(); ++vi)
            {
                if (!vi->IsD())
                {
                    for (std::size_t j = 0; j < 3; ++j)
                    {
                        vertices[p::make_tuple(i, j)] = vi->P()[j];
                    }
                    ++i;
                }
            }
        }
        return vertices;
    }

    SimpTempData getSimpleTempData()
    {
        SimpTempData indices(this->vert);
        {
            std::size_t i = 0;
            VertexIterator vi;
            for (vi = this->vert.begin(); vi != this->vert.end(); ++vi)
            {
                indices[vi] = i;
                if (!vi->IsD())
                {
                    ++i;
                }
            }
        }
        return indices;
    }

    ndarray getFaces()
    {
        namespace p = boost::python;
        namespace np = bpnp;

        p::tuple shape = p::make_tuple(this->fn, 3);
        np::dtype dtype = np::dtype::get_builtin<std::int64_t>();
        np::ndarray faces = np::empty(shape, dtype);
        {
            SimpTempData indices = getSimpleTempData();
            std::size_t i = 0;
            FaceIterator fi;
            for (fi = this->face.begin(); fi != this->face.end(); ++fi)
            {
                if (!fi->IsD())
                {
                    for (std::size_t j = 0; j < 3; ++j)
                    {
                        faces[p::make_tuple(i, j)] = indices[fi->V(j)];
                    }
                    ++i;
                }
            }
        }
        return faces;
    }
};

boost::python::object
cleanMeshPy(
    MyMeshPy & mesh,
    const bool removeDuplicate,
    const bool removeUnreferenced
)
{
    auto p = cleanMesh(mesh, removeDuplicate, removeUnreferenced);
    return boost::python::make_tuple(p.first, p.second);
}

vcg::tri::TriEdgeCollapseQuadricParameter
defaultQParams()
{
    vcg::tri::TriEdgeCollapseQuadricParameter qparams;


    qparams.QualityCheck     = true;
    qparams.NormalCheck      = false;
    qparams.OptimalPlacement = true;
    qparams.ScaleIndependent = true;
    qparams.PreserveBoundary = false;
    qparams.PreserveTopology = false;
    qparams.QualityQuadric   = false;
    qparams.QualityWeight    = false;
    qparams.QualityWeightFactor = 10.0;
    qparams.QualityThr       = 0.3;
    qparams.NormalThrRad = math::ToRad(90.0);
    qparams.BoundaryWeight   = 0.5;

/*
          "     -e# QuadricError threshold  (range [0,inf) default inf)\n"
          "     -b# Boundary Weight (default .5)\n"
          "     -q# Quality threshold (range [0.0, 0.866],  default .3 )\n"
          "     -n# Normal threshold  (degree range [0,180] default 90)\n"
          "     -w# Quality weight factor  (10)\n"
          "     -E# Minimal admitted quadric value (default 1e-15, must be >0)\n"
          "     -Q[y|n]  Use or not Face Quality Threshold (default yes)\n"
          "     -P[y|n]  Add or not QualityQuadric (default no)\n"
          "     -N[y|n]  Use or not Face Normal Threshold (default no)\n"
          "     -A[y|n]  Use or not Area Weighted Quadrics (default yes)\n"
          "     -O[y|n]  Use or not vertex optimal placement (default yes)\n"
          "     -S[y|n]  Use or not Scale Independent quadric measure(default yes) \n"
          "     -B[y|n]  Preserve or not mesh boundary (default no)\n"
          "     -T[y|n]  Preserve or not Topology (default no)\n"
          "     -W[y|n]  Use or not per vertex Quality to weight the quadric error (default no)\n"
          "     -C       Before simplification, remove duplicate & unreferenced vertices\n"

 */
    return qparams;
}

double
decimateMeshPy(
    MyMeshPy & mesh,
    const std::size_t finalNumFaces,
    boost::python::object qparmsObj,
    const double targetError=std::numeric_limits<double>::max()
)
{
    vcg::tri::TriEdgeCollapseQuadricParameter qparms = defaultQParams();
    if (!qparmsObj.is_none())
    {
        qparms = boost::python::extract<vcg::tri::TriEdgeCollapseQuadricParameter>(qparmsObj)();
    }
    std::cout << std::endl << "qparms:\n" << toString(qparms) << std::endl;

    return decimateMesh(mesh, finalNumFaces, qparms, targetError);
}

namespace
{

void exportMesh()
{
    using namespace boost::python;
    class_<MyMeshPy, boost::noncopyable>(
        "Mesh",
        "Triangular mesh.\n\n"
    )
    .def(
        init<>("Default constructor, empty mesh.")
    )
    .def(
        init<object, object>(
            "Construct with specified vertices and faces.\n\n",
            (arg("vertices"), arg("faces"))
        )
    )
    .def_readonly(
        "num_vertices",
        &MyMeshPy::vn,
        "Number of vertices."
    )
    .def_readonly(
        "num_faces",
        &MyMeshPy::fn,
        "Number of faces."
    )
    .add_property(
        "vertices",
        &MyMeshPy::getVertices,
        "A (N,3) shaped numpy.ndarray of vertex coordinates."
    )
    .add_property(
        "faces",
        &MyMeshPy::getFaces,
        "A (M,3) shaped numpy.ndarray of triangular faces."
    )

    ;
}

void exportParams()
{
    using namespace boost::python;
    class_<vcg::tri::TriEdgeCollapseQuadricParameter>(
        "TriEdgeCollapseQuadricParameter",
        "Parameters for quadric decimation.\n\n"
    )
    .def_readwrite(
        "BoundaryWeight",
        &vcg::tri::TriEdgeCollapseQuadricParameter::BoundaryWeight,
        "Boundary weight."
    )
    .def_readwrite(
        "CosineThr",
        &vcg::tri::TriEdgeCollapseQuadricParameter::CosineThr,
        "Cosine threshold."
    )
    .def_readwrite(
        "FastPreserveBoundary",
        &vcg::tri::TriEdgeCollapseQuadricParameter::FastPreserveBoundary,
        "Fast preserve boundary."
    )
    .def_readwrite(
        "NormalCheck",
        &vcg::tri::TriEdgeCollapseQuadricParameter::NormalCheck,
        "Normal check."
    )
    .def_readwrite(
        "NormalThrRad",
        &vcg::tri::TriEdgeCollapseQuadricParameter::NormalThrRad,
        "Normal threshold radians."
    )
    .def_readwrite(
        "OptimalPlacement",
        &vcg::tri::TriEdgeCollapseQuadricParameter::OptimalPlacement,
        "Optimal placement."
    )
    .def_readwrite(
        "PreserveTopology",
        &vcg::tri::TriEdgeCollapseQuadricParameter::PreserveTopology,
        "Preserve topology."
    )
    .def_readwrite(
        "PreserveBoundary",
        &vcg::tri::TriEdgeCollapseQuadricParameter::PreserveBoundary,
        "Preserve boundary."
    )
    .def_readwrite(
        "QuadricEpsilon",
        &vcg::tri::TriEdgeCollapseQuadricParameter::QuadricEpsilon,
        "Quadric epsilon."
    )
    .def_readwrite(
        "QualityCheck",
        &vcg::tri::TriEdgeCollapseQuadricParameter::QualityCheck,
        "Quality check."
    )
    .def_readwrite(
        "QualityQuadric",
        &vcg::tri::TriEdgeCollapseQuadricParameter::QualityQuadric,
        "During the initialization manage all the edges as border edges adding"
        "a set of additional quadrics that are useful mostly for keeping face "
        "aspect ratio good."
    )
    .def_readwrite(
        "QualityThr",
        &vcg::tri::TriEdgeCollapseQuadricParameter::QualityThr,
        "Collapsed that generate faces with quality LOWER than this value"
        "are penalized."
    )
    .def_readwrite(
        "QualityWeight",
        &vcg::tri::TriEdgeCollapseQuadricParameter::QualityWeight,
        "Quality weight."
    )
    .def_readwrite(
        "QualityWeightFactor",
        &vcg::tri::TriEdgeCollapseQuadricParameter::QualityWeightFactor,
        "Quality weight factor."
    )
    .def_readwrite(
        "ScaleFactor",
        &vcg::tri::TriEdgeCollapseQuadricParameter::ScaleFactor,
        "Scale factor."
    )
    .def_readwrite(
        "ScaleIndependent",
        &vcg::tri::TriEdgeCollapseQuadricParameter::ScaleIndependent,
        "Scale independent."
    )
    .def_readwrite(
        "UseArea",
        &vcg::tri::TriEdgeCollapseQuadricParameter::UseArea,
        "Use area."
    )
    .def_readwrite(
        "UseVertexWeight",
        &vcg::tri::TriEdgeCollapseQuadricParameter::UseVertexWeight,
        "Use vertex weight."
    )
    .def("__str__", &toString)
    ;

}

void exportDecimate()
{
    using namespace boost::python;

    def(
        "default_q_params",
        &defaultQParams,
        "Returns default TriEdgeCollapseQuadricParameter parameters."
    );

    def(
        "clean",
        &cleanMeshPy,
        "Cleans mesh in-place, removes duplicate and unreferenced vertices.",
        (
            arg("mesh"),
            arg("remove_duplicate")=true,
            arg("remove_unreferenced")=true
        )
    );
    def(
        "decimate",
        &decimateMeshPy,
        "In-place quadric decimation of mesh.",
        (
            arg("mesh"),
            arg("final_num_faces"),
            arg("qparms")=boost::python::object(),
            arg("target_error")=std::numeric_limits<double>::max()
        )
    );

}

}

}
}

BOOST_PYTHON_MODULE(_tridecimator)
{
    vcg::python::bpnp::initialize();
    vcg::python::exportParams();
    vcg::python::exportMesh();
    vcg::python::exportDecimate();
}
