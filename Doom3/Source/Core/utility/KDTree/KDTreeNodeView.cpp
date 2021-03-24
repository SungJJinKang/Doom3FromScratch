#include "KDTreeNodeView.h"

#include "KDTree.h"
#include "KDTreeNode.h"

template<int Dimension>
typename doom::KDTreeNodeView<Dimension>::node_type* doom::KDTreeNodeView<Dimension>::operator->()
{
	D_ASSERT(this->IsValid() == true);
	return this->mKDTreePointer->mKDTreeNodes + this->mNodeIndex;
}

template class doom::KDTreeNodeView<2>;
template class doom::KDTreeNodeView<3>;