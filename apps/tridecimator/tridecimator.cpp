
#include "tridecimator.h"
#include <boost/format.hpp>

std::string
toString(vcg::tri::TriEdgeCollapseQuadricParameter & qparams)
{
    return
        (
            boost::format(
                "TriEdgeCollapseQuadricParameter(\n"
                "   double    BoundaryWeight        = %f;\n"
                "   double    CosineThr             = %f;\n"
                "   bool      FastPreserveBoundary  = %s;\n"
                "   bool      NormalCheck           = %s;\n"
                "   double    NormalThrRad          = %f;\n"
                "   bool      OptimalPlacement      = %s;\n"
                "   bool      PreserveTopology      = %s;\n"
                "   bool      PreserveBoundary      = %s;\n"
                "   double    QuadricEpsilon        = %f;\n"
                "   bool      QualityCheck          = %s;\n"
                "   bool      QualityQuadric        = %s;\n"
                "   double    QualityThr            = %f;\n"
                "   bool      QualityWeight         = %s;\n"
                "   double    QualityWeightFactor   = %f;\n"
                "   double    ScaleFactor           = %f;\n"
                "   bool      ScaleIndependent      = %s;\n"
                "   bool      UseArea               = %s;\n"
                "   bool      UseVertexWeight       = %s;\n"
                ")"
            )
            %
            qparams.BoundaryWeight
            %
            qparams.CosineThr
            %
            (qparams.FastPreserveBoundary ? "true" : "false")
            %
            (qparams.NormalCheck ? "true" : "false")
            %
            qparams.NormalThrRad
            %
            (qparams.OptimalPlacement ? "true" : "false")
            %
            (qparams.PreserveTopology ? "true" : "false")
            %
            (qparams.PreserveBoundary ? "true" : "false")
            %
            qparams.QuadricEpsilon
            %
            (qparams.QualityCheck ? "true" : "false")
            %
            (qparams.QualityQuadric ? "true" : "false")
            %
            qparams.QualityThr
            %
            (qparams.QualityWeight ? "true" : "false")
            %
            qparams.QualityWeightFactor
            %
            qparams.ScaleFactor
            %
            (qparams.ScaleIndependent ? "true" : "false")
            %
            (qparams.UseArea ? "true" : "false")
            %
            (qparams.UseVertexWeight ? "true" : "false")
        ).str();
}

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
    const double targetError
)
{
    vcg::tri::UpdateBounding<MyMesh>::Box(mesh);

    // decimator initialization
    vcg::LocalOptimization<MyMesh> DeciSession(mesh,&qparams);

    DeciSession.Init<MyTriEdgeCollapse>();

    DeciSession.SetTargetSimplices(finalNumFaces);
    DeciSession.SetTimeBudget(0.5f);
    if(targetError< std::numeric_limits<double>::max() ) DeciSession.SetTargetMetric(targetError);

    while(DeciSession.DoOptimization() && (mesh.fn > finalNumFaces) && (DeciSession.currMetric < targetError))
    {
        /*
         * printf("Current Mesh size %7i heap sz %9i err %9g \r",mesh.fn, int(DeciSession.h.size()),DeciSession.currMetric);
         */
    }


    return DeciSession.currMetric;
}
