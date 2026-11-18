#include <iostream>
#include <fstream>
#include <filesystem>

#include "utility/Dictionary.h"
#include "utility/orthoSquareMesh.h"
#include "utilHeader/fvMatrix.h"
#include "field/volField.h"
#include "field/surfaceField.h"
#include "diffusionScheme/secondCentralDiff.h"
#include "convectionScheme/convectionScheme.h"
#include "utility/Vector.h"
#include "linearSolver/linearSolver.h"
#include <memory>


int main()
{
    // Read dictionary
    // PROJECT_DIR is a cmake variable in the root CMakeLists.txt
    const std::filesystem::path casePath 
        = std::filesystem::path(PROJECT_DIR) / "inputDict.json";
    const Dictionary dict = Dictionary::read(casePath);
    const Dictionary initialDict = dict.subDict("0");

    // Create mesh
    const orthoSquareMesh mesh(dict.subDict("mesh"));

    // Create initial field
    volField<double> T("T", mesh, initialDict.subDict("T"));

    // Calculate face flux
    surfaceField<Vector> velocity(mesh, Vector{1.0, 1.0});
    surfaceField<Vector> Sf(mesh, mesh.Sf());
    surfaceField<double> phi = velocity & Sf;
    const double Gamma = 1.0 / dict.get<double>("PecletNumber");
    surfaceField<double> diffusion(mesh, mesh.magSf());   // Gamma_f * |S_f|
    for (int f = 0; f < mesh.nFaces(); f++)
        diffusion.set(f, Gamma * diffusion.get(f));

    // Calculate coefficients of diffusion scheme
    fvMatrix matrix = 
        diffusionScheme::secondCentralDiff<double>().interpolate(diffusion, T);

    // Add coefficients of convection scheme (implicit part, built once)
    auto conv = convectionScheme<double>::New(dict);
    conv->add(matrix, phi);

    // Linear solver (inner iterations)
    const std::unique_ptr<linearSolver> solver =
        linearSolver::New(dict.subDict("linearSolver"));

    // Outer iterations for the deferred correction
    const Dictionary outerDict = dict.subDict("outerIterations");
    const int    maxOuter = outerDict.get<int>("maxIter");
    const double outerTol = outerDict.get<double>("tolerance");

    linearSolver::SolverPerformance convergence;
    double r0 = 0.0;
    int nOuter = 0;

    for (int outer = 0; outer < maxOuter; outer++)
    {
        matrix.clearSource();
        conv->correct(matrix, phi);

        convergence = matrix.solve(*solver);
        nOuter = outer + 1;

        if (outer == 0) r0 = convergence.initialResidual;

        if
        (
            r0 == 0.0
         || convergence.initialResidual < outerTol * r0
         || convergence.nIterations == 0
        )
        {
            break;
        }
    }

    // Output flow field and convergence result
    T.write("T.csv");
    std::ofstream file("converge.txt");
    file << "initial residual: " << convergence.initialResidual << "\n"
         << "final residual  : " << convergence.finalResidual << "\n"
         << "nIterations     : " << convergence.nIterations << "\n"
         << "converged       : " << static_cast<bool>(convergence.converged) << "\n"
         << "outer iterations: " << nOuter << std::endl;
    file.close();
}