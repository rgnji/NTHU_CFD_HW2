#include "linearSolver/linearSolver.h"
#include "utility/Dictionary.h"
#include "utility/lduMatrix.h"


linearSolver::linearSolver
(
    double tolerance,
    double relTol,
    int maxIter
)
:
    tolerance_(tolerance),
    relTol_(relTol),
    maxIter_(maxIter)
{}


std::map<std::string, linearSolver::Creator>& table()
{
    static std::map<std::string, linearSolver::Creator> t;
    return t;
}


std::unique_ptr<linearSolver> linearSolver::New(const Dictionary& dict)
{
    const std::string type = dict.get<std::string>("solver");
    auto it = table().find(type);
    if (it == table().end())
        throw std::runtime_error("Unknown linear solver: " + type);
    return it->second(dict);
}
