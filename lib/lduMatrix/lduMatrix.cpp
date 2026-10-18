#include "lduMatrix.h"
#include <iostream>


lduMatrix::lduMatrix(const nodeScalarField& field)
:
	diag_(field.node(), 0.0),
	b_(field.node())
{}


void lduMatrix::buildLduAddressing
(
	const nodeScalarField& phi, 
	std::vector<pairType>& pairs
)
{
	// i < j for each pair (i, j) = (owner, neighbor)
	for (auto& p : pairs)
		if (p.couplings.first > p.couplings.second) 
		{
			std::swap(p.couplings.first, p.couplings.second);

			switch(p.directions)
			{				
				case direction::W:
					p.directions = direction::E;
					break;

				case direction::S:
					p.directions = direction::N;
					break;

				case direction::WW:
					p.directions = direction::EE;
					break;

				case direction::SS:
					p.directions = direction::NN;
					break;

				default:
					std::cout << "Wrong direction in internal node pairs!" << std::endl;
					break;
			}
		}
			
	// sort couplings according to owner index
	std::sort(pairs.begin(), pairs.end());
	// eliminate repeated pairs
	pairs.erase(std::unique(pairs.begin(), pairs.end()),
                    pairs.end());

	// Count how many time a node appears to be a owner
	ownerStart_.assign(phi.node() + 1, 0);
	for (int k=0; k<pairs.size(); k++)
	{
		const auto& [l, u] = pairs[k].couplings;

		// Compose addressing array
		lowerAddr_.push_back(l);
		upperAddr_.push_back(u);
		pairDirection_.push_back(pairs[k].directions);

		// Find index of l in internalNodeList
		int ownerIndex = phi.internalConvert(l);

		// Count
		ownerStart_[ownerIndex + 1]++;
	}

	// prefix sum
	for (int k=0; k<ownerStart_.size()-1; k++)
	{
		ownerStart_[k + 1] += ownerStart_[k];
	}

	// The original index of upperAddr_ if couplings is sorted according to .second()
	losortAddr_.assign(upperAddr_.size(), 0);
	std::iota(losortAddr_.begin(), losortAddr_.end(), 0);
	std::stable_sort(losortAddr_.begin(), losortAddr_.end(), [this](int i1, int i2) {
		return upperAddr_[i1] < upperAddr_[i2];
	});

	// Count how many times a node appears to be the neighbor
	losortStart_.assign(phi.node() + 1, 0);
	for (int k=0; k<upperAddr_.size(); k++)
	{
		const int nbr = upperAddr_[k];

		// Find index nbr in internalNodeList
		int nbrIndex = phi.internalConvert(nbr);

		losortStart_[nbrIndex + 1]++;
	}

	// prefix sum
	for (int k=0; k<losortStart_.size()-1; k++)
	{
		losortStart_[k + 1] += losortStart_[k];
	}

	// Determine sizes of lower_ and upper_
	lower_.assign(lowerAddr_.size(), 0.0);
	upper_.assign(upperAddr_.size(), 0.0);
}


void lduMatrix::buildBoundaryAddressing
(
	const nodeScalarField& phi,
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


double lduMatrix::residualNorm(const nodeScalarField& phi) const
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