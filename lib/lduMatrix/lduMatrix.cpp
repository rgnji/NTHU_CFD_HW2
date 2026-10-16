#include "lduMatrix.h"


lduMatrix::lduMatrix(nodeScalarField field)
:
	diag_(field.node(), 0.0)
{}


void lduMatrix::buildLduAddressing
(
	nodeScalarField phi, 
	std::vector<std::pair<int, int>> couplings
)
{
	const auto& list = phi.internalNodeList();

	// i < j for each pair (i, j) = (owner, neighbor)
	for (auto& p : couplings)
		if (p.first > p.second) std::swap(p.first, p.second);
	// sort couplings according to owner index
	std::sort(couplings.begin(), couplings.end());
	// eliminate repeated pairs
	couplings.erase(std::unique(couplings.begin(), couplings.end()),
                    couplings.end());

	// Count how many time a node appears to be a owner
	ownerStart_.assign(phi.node() + 1, 0);
	for (int k=0; k<couplings.size(); k++)
	{
		const auto& [l, u] = couplings[k];

		// Compose addressing array
		lowerAddr_.push_back(l);
		upperAddr_.push_back(u);

		// Find index of l in internalNodeList
		auto it = std::find(list.begin(), list.end(), l);
		int ownerIndex = std::distance(list.begin(), it);

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
		auto it = std::find(list.begin(), list.end(), nbr);
		int nbrIndex = std::distance(list.begin(), it);

		losortStart_[nbrIndex + 1]++;
	}

	// prefix sum
	for (int k=0; k<losortStart_.size()-1; k++)
	{
		losortStart_[k + 1] += losortStart_[k];
	}
}