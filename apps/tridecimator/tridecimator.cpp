
#include "tridecimator.h"

std::pair<std::size_t, std::size_t>
cleanMesh(MyMesh & mesh, const bool removeDuplicate, const bool removeUnreferenced)
{
    std::size_t dup = 0;
    std::size_t unref = 0;

    if (removeDuplicate)
    {
        dup = vcg::tri::Clean<MyMesh>::RemoveDuplicateVertex(mesh);
    }
    if (removeUnreferenced)
    {
        unref =  vcg::tri::Clean<MyMesh>::RemoveUnreferencedVertex(mesh);
    }
    return std::make_pair(dup, unref);
}

double
decimateMesh(
    MyMesh & mesh,
    std::size_t finalNumFaces,
    vcg::tri::TriEdgeCollapseQuadricParameter & qparams,
    const float targetError
)
{
    vcg::tri::UpdateBounding<MyMesh>::Box(mesh);

    // decimator initialization
    vcg::LocalOptimization<MyMesh> DeciSession(mesh,&qparams);

    DeciSession.Init<MyTriEdgeCollapse>();

    DeciSession.SetTargetSimplices(finalNumFaces);
    DeciSession.SetTimeBudget(0.5f);
    if(targetError< std::numeric_limits<float>::max() ) DeciSession.SetTargetMetric(targetError);

    while(DeciSession.DoOptimization() && (mesh.fn > finalNumFaces) && (DeciSession.currMetric < targetError))
    {
        /*
         * printf("Current Mesh size %7i heap sz %9i err %9g \r",mesh.fn, int(DeciSession.h.size()),DeciSession.currMetric);
         */
    }


    return DeciSession.currMetric;
}
