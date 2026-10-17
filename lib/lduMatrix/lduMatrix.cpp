#include "lduMatrix.h"


lduMatrix::lduMatrix(const nodeScalarField& field)
:
	diag_(field.node(), 0.0)
{}


void lduMatrix::buildLduAddressing
(
	const nodeScalarField& phi, 
	std::vector<pairType> pairs
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

	// Determine sizes of diag_, lower_ and upper_
	diag_.assign(phi.node(), 0.0);
	lower_.assign(lowerAddr_.size(), 0.0);
	upper_.assign(upperAddr_.size(), 0.0);
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


const std::vector<int>& lduMatrix::lowerAddr() const
{
	return lowerAddr_;
}


const std::vector<int>& lduMatrix::upperAddr() const
{
	return upperAddr_;
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