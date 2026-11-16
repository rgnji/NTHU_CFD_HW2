#ifndef linearSolver_H
#define linearSolver_H

#include <vector>
#include <map>
#include <memory>
#include <string>


class lduMatrix;

class Dictionary;


class linearSolver
{
public:

    struct SolverPerformance
    {
        double initialResidual = 0.0;
        double finalResidual   = 0.0;
        int    nIterations     = 0;
        bool   converged       = false;
    };


protected:

    double tolerance_;
    double relTol_;
    int maxIter_;

    bool converged(const SolverPerformance& perf) const
    {
        return perf.finalResidual < tolerance_
            || perf.finalResidual < relTol_ * perf.initialResidual;
    }


public:

    // Constructor

        linearSolver
        (
            double tolerance,
            double relTol,
            int maxIter
        );


    // Destructor
    virtual ~linearSolver() = default;


    // Runtime selection
    using Creator = std::unique_ptr<linearSolver> (*)(const Dictionary&);

    static std::map<std::string, Creator>& table();
    static std::unique_ptr<linearSolver> New(const Dictionary& dict);

    template<class Derived>
    struct Register
    {
        explicit Register(const std::string& type)
        {
            table()[type] = [](const Dictionary& dict) -> std::unique_ptr<linearSolver>
            {
                return std::make_unique<Derived>(dict);
            };
        }
    };


    // Member Functions

    virtual SolverPerformance solve
    (
        const lduMatrix& A,
        std::vector<double>& x,
        const std::vector<double>& b
    ) const = 0;
};


#endif