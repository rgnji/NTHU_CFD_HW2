#include "utility/lduMatrix.h"
#include <iostream>


// * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * * //

lduMatrix::lduMatrix(const orthoSquareMesh& field)
:
	nFaces_(field.faces().size()),
	diag_(field.nCells(), 0.0),
	lower_(nFaces_, 0.0),
	upper_(nFaces_, 0.0),
	b_(field.nCells(), 0.0),
	lowerAddr_(field.owner()),
	upperAddr_(field.neighbor()),
	ownerStart_(field.nCells()+1, 0),
	losortAddr_(nFaces_, 0),
	losortStart_(field.nCells()+1, 0)
{
	buildLduAddressing();
}


// * * * * * * * * * * * * * Private Member Functions * * * * * * * * * * * *//

void lduMatrix::buildLduAddressing()
{
	/*
	// sort couplings according to owner index
	std::sort(pairs.begin(), pairs.end());
	// eliminate repeated pairs
	pairs.erase(std::unique(pairs.begin(), pairs.end()),
                    pairs.end());
	*/

	// Collect owners
	for (int k : lowerAddr_)
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
	std::stable_sort(losortAddr_.begin(), losortAddr_.end(), [this](int i1, int i2) {
		return upperAddr_[i1] < upperAddr_[i2];
	});

	// Count how many times a cell appears to be the neighbor
	for (int k : upperAddr_)
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

void lduMatrix::buildBoundaryAddressing
(
	const orthoSquareMesh& phi,
	std::vector<pairType>& boundaryPairs
)
{
	for (int k=0; k<boundaryPairs.size(); k++)
	{
		auto [inter, bound] = boundaryPairs[k].couplings;
		internalAddr_.push_back(inter);
		boundaryAddr_.push_back(bound);
		boundaryDirection_.push_back(boundaryPairs[k].directions);
	}
}


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


void lduMatrix::buildB(int f, double coeff)
{
	b_[f] += coeff;
}


const std::vector<double>& lduMatrix::diag() const
{
	return diag_;
}


const std::vector<double>& lduMatrix::lower() const
{
	return lower_;
}


const std::vector<double>& lduMatrix::upper() const
{
	return upper_;
}

const std::vector<double>& lduMatrix::b() const
{
	return b_;
}


const std::vector<int>& lduMatrix::lowerAddr() const
{
	return lowerAddr_;
}


const std::vector<int>& lduMatrix::upperAddr() const
{
	return upperAddr_;
}


const std::vector<int>& lduMatrix::losortAddr() const
{
	return losortAddr_;
}


const std::vector<int>& lduMatrix::ownerStart() const
{
	return ownerStart_;
}


const std::vector<int>& lduMatrix::losortStart() const
{
	return losortStart_;
}


const std::vector<lduMatrix::direction>& lduMatrix::pairDirection() const
{
	return pairDirection_;
}


const std::vector<int>& lduMatrix::internalAddr() const
{
	return internalAddr_;
}


const std::vector<int>& lduMatrix::boundaryAddr() const
{
	return boundaryAddr_;
}


const std::vector<lduMatrix::direction>& lduMatrix::boundaryDirection() const
{
	return boundaryDirection_;
}


double lduMatrix::residualNorm(const orthoSquareMesh& phi) const
{
    std::vector<double> r(diag_.size());

    // r_P = b_P - a_PP * x_P
    for (int P = 0; P < (int)diag_.size(); P++)
        r[P] = b_[P] - diag_[P] * phi.getValue(phi.globalIndex(P));

    // 非對角項
    for (int f = 0; f < (int)lowerAddr_.size(); f++)
    {
        int l = lowerAddr_[f], u = upperAddr_[f];          // global
        r[phi.internalConvert(l)] -= upper_[f] * phi.getValue(u);
        r[phi.internalConvert(u)] -= lower_[f] * phi.getValue(l);
    }

    double sum = 0.0;
    for (double v : r) sum += v * v;
    return std::sqrt(sum / r.size());
}