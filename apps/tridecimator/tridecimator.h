#ifndef VCGLIB_APPS_TRIDECIMATOR_H
#define VCGLIB_APPS_TRIDECIMATOR_H

// stuff to define the mesh
#include <vcg/complex/complex.h>

// local optimization
#include <vcg/complex/algorithms/clean.h>
#include <vcg/complex/algorithms/local_optimization.h>
#include <vcg/complex/algorithms/local_optimization/tri_edge_collapse_quadric.h>
#include <utility>
#include <vector>

/**********************************************************
Mesh Classes for Quadric Edge collapse based simplification

For edge collpases we need verteses with:
- V->F adjacency
- per vertex incremental mark
- per vertex Normal


Moreover for using a quadric based collapse the vertex class
must have also a Quadric member Q();
Otherwise the user have to provide an helper function object
to recover the quadric.

******************************************************/
// The class prototypes.
class MyVertex;
class MyEdge;
class MyFace;

struct MyUsedTypes: public vcg::UsedTypes<vcg::Use<MyVertex>::AsVertexType,vcg::Use<MyEdge>::AsEdgeType,vcg::Use<MyFace>::AsFaceType>{};

class MyVertex  : public vcg::Vertex< MyUsedTypes,
    vcg::vertex::VFAdj,
    vcg::vertex::Coord3f,
    vcg::vertex::Normal3f,
    vcg::vertex::Mark,
    vcg::vertex::Qualityf,
    vcg::vertex::BitFlags  >{
public:
  vcg::math::Quadric<double> &Qd() {return q;}
private:
  vcg::math::Quadric<double> q;
  };

class MyEdge : public vcg::Edge< MyUsedTypes> {};

typedef vcg::tri::BasicVertexPair<MyVertex> VertexPair;

class MyFace    : public vcg::Face< MyUsedTypes,
  vcg::face::VFAdj,
  vcg::face::VertexRef,
  vcg::face::BitFlags > {};

// the main mesh class
class MyMesh    : public vcg::tri::TriMesh<std::vector<MyVertex>, std::vector<MyFace> > {};


class MyTriEdgeCollapse: public vcg::tri::TriEdgeCollapseQuadric< MyMesh, VertexPair, MyTriEdgeCollapse, vcg::tri::QInfoStandard<MyVertex>  > {
            public:
            typedef  vcg::tri::TriEdgeCollapseQuadric< MyMesh,  VertexPair, MyTriEdgeCollapse, vcg::tri::QInfoStandard<MyVertex>  > TECQ;
            typedef  MyMesh::VertexType::EdgeType EdgeType;
            inline MyTriEdgeCollapse(  const VertexPair &p, int i, vcg::BaseParameterClass *pp) :TECQ(p,i,pp){}
};

std::pair<std::size_t, std::size_t>
cleanMesh(
    MyMesh & mesh,
    const bool removeDuplicate=true,
    const bool removeUnreferences=true
);

double
decimateMesh(
    MyMesh & mesh,
    const std::size_t finalNumFaces,
    vcg::tri::TriEdgeCollapseQuadricParameter & qparms,
    const float targetError=std::numeric_limits<float>::max()
);

#endif /* VCGLIB_APPS_TRIDECIMATOR_H */
