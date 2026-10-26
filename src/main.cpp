#include <iostream>
#include <fstream>
#include <cmath>
#include <numbers>

#include "nodeScalarField/nodeScalarField.h"
#include "lduMatrix/lduMatrix.h"
#include "centralDiff/centralDiff.h"
#include "Jacobi/Jacobi.h"


int main()
{
    int nNodes = 81;
    double length = 1;
    double h = length / (nNodes - 1);

    nodeScalarField X(nNodes, nNodes);
    nodeScalarField Y(nNodes, nNodes);
    nodeScalarField T(nNodes, nNodes);

    // Grid
    for (int i=0; i<nNodes; i++)
    {
        for (int j=0; j<nNodes; j++)
        {
            int index = i + j * nNodes;
            
            X.setValue(index, i * h);
            Y.setValue(index, j * h);
        }
    }

    // Initial flow field
    for (int i = 0; i < nNodes; i++)
    {
        int index = i;

        T.setValue(index, 10);
    }
    for (int j = 0; j < nNodes; j++)
    {
        int indexLeft = j * nNodes;
        int indexRight = (nNodes - 1) + j * nNodes;

        T.setValue(indexLeft, 10);
        T.setValue(indexRight, 10);
    }
    for (int i = 0; i < nNodes; i++)
    {
        int index = i + (nNodes - 1) * nNodes;

        T.setValue(index, std::sin(std::numbers::pi * i * h));
    }

    // Solve with Jacobi
    diffusionScheme::centralDiff scheme(true);
    Jacobi linearSolver(T);

    lduMatrix A = scheme.secondScheme(T, X, Y);
    auto [converge, iteration] = linearSolver.iterate(A, T);

    std::cout << std::boolalpha;
    std::cout << "Converge:  " << converge << std::endl
              << "Iteration: " << iteration << std::endl;
    std::cout << std::noboolalpha;

    // Write residual
    std::ofstream residual("residual_jacobi.txt");

    if (!residual.is_open()) 
    {
        std::cerr << "Can't open residual_jacobi.txt" << std::endl;
        return 1;
    }

    for (int k = 0; k < linearSolver.rHistory().size(); k++)
    {
        residual << linearSolver.rHistory()[k] << std::endl;
    }

    residual.close();

    // Write final flow field
    std::ofstream flowfield("flowfield.csv");

    if(!flowfield.is_open())
    {
        std::cerr << "Can't open flowfield.csv" << std::endl;
        return 1;
    }

    //flowfield << nNodes << "," << nNodes << std::endl;

    for (int j = 0; j < nNodes; j++)
    {
        for (int i = 0; i < nNodes; i++)
        {
            int index = i + j * nNodes;

            if (i == nNodes - 1)
            {
                flowfield << T.getValue(index) << std::endl;
            }
            else
            {
                flowfield << T.getValue(index) << ",";
            }
        }
    }

    flowfield.close();
}