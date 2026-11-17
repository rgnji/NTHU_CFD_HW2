#include "utility/lduMatrix.h"
#include "utility/orthoSquareMesh.h"
#include <iostream>
#include <cmath>
#include <numeric>
#include <algorithm>


// * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * * //

lduMatrix::lduMatrix(const orthoSquareMesh& mesh)
:
	diag_(mesh.nCells(), 0.0),
	lower_(mesh.nInternalFaces(), 0.0),
	upper_(mesh.nInternalFaces(), 0.0),
	owner_(mesh.owner()),
	neighbor_(mesh.neighbor()),
	ownerStart_(mesh.nCells()+1, 0),
	losortAddr_(mesh.nInternalFaces(), 0),
	losortStart_(mesh.nCells()+1, 0)
{
	buildLduAddressing();
}


// * * * * * * * * * * * * * Private Member Functions * * * * * * * * * * * *//

void lduMatrix::buildLduAddressing()
{
	auto lowerAddr = std::span<const int>(owner_.data(), neighbor_.size());
	const auto& upperAddr = neighbor_;

	// Collect owners
	for (int k : lowerAddr)
	{
		ownerStart_[k + 1]++;
	}

	// prefix sum
	for (int k=0; k<ownerStart_.size()-1; k++)
	{
		ownerStart_[k + 1] += ownerStart_[k];
	}

	// The original index of upperAddr_ if couplings is sorted according to .second()
	std::iota(losortAddr_.begin(), losortAddr_.end(), 0);
	std::stable_sort(losortAddr_.begin(), losortAddr_.end(), [upperAddr](int i1, int i2) {
		return upperAddr[i1] < upperAddr[i2];
	});

	// Count how many times a cell appears to be the neighbor
	for (int k : upperAddr)
	{
		losortStart_[k + 1]++;
	}

	// prefix sum
	for (int k=0; k<losortStart_.size()-1; k++)
	{
		losortStart_[k + 1] += losortStart_[k];
	}
}


// * * * * * * * * * * * * * * Public Member Functions * * * * * * * * * * * //

void lduMatrix::buildDiag(int center, double coeff)
{
	diag_[center] += coeff;
}


void lduMatrix::buildLower(int f, double coeff)
{
	lower_[f] += coeff;
}


void lduMatrix::buildUpper(int f, double coeff)
{
	upper_[f] += coeff;
}


std::vector<double> lduMatrix::residual
(
	const std::vector<double>& x,
    const std::vector<double>& b
) const
{
	auto l = lowerAddr();
    const auto& u = upperAddr();
    const int nCells = static_cast<int>(x.size());
    const int nPairs = static_cast<int>(l.size());

    std::vector<double> r(nCells);

	for (int P = 0; P < nCells; ++P)
        r[P] = b[P] - diag_[P] * x[P];

    for (int f = 0; f < nPairs; ++f)
    {
        r[l[f]] -= upper_[f] * x[u[f]];
        r[u[f]] -= lower_[f] * x[l[f]];
    }

	return r;
}


double lduMatrix::residualNorm
(
	const std::vector<double>& x,
    const std::vector<double>& b
) const
{
	auto r = residual(x, b);

	double sum = 0.0;
	for (double v : r) sum += v * v;

	return std::sqrt(sum / static_cast<double>(r.size()));
}