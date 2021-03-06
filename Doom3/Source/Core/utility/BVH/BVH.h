#pragma once

#include <memory>
#include <queue>

#include "Physics/Collider/AABB.h"
#include "Physics/Collider/Sphere.h"
#include "Physics/Collider/Ray.h"

#include "BVH_Core.h"
#include "BVH_Node.h"
#include "BVH_Node_View.h"



//BVH is used for rendering, collision detect, raycast, stero audio ......

namespace doom
{
	
	class Ray;
	template <typename ColliderType>
	class BVH;

	//////////////////////////

	//TODO : Two BVH ����
	///The b3DynamicBvhBroadphase implements a broadphase using two dynamic ColliderType bounding volume hierarchies/trees (see b3DynamicBvh).
	///One tree is used for static/non-moving objects, and another tree is used for dynamic objects. Objects can move from one tree to the other.
	///This is a very fast broadphase, especially for very dynamic worlds where many objects are moving. Its insert/add and remove of objects is generally faster than the sweep and prune broadphases b3AxisSweep3 and b332BitAxisSweep3.
	//struct b3DynamicBvhBroadphase
	//https://github.com/bulletphysics/bullet3/blob/master/src/Bullet3Collision/BroadPhaseCollision/b3DynamicBvhBroadphase.h

	//TODO : make Parallel BVH construction 
	//https://meistdan.github.io/publications/phr/paper.pdf

	//https://graphics.cg.uni-saarland.de/courses/cg1-2019/slides/Building_Good_BVHs.pdf
	//http://www.cs.uu.nl/docs/vakken/magr/2016-2017/slides/lecture%2003%20-%20the%20perfect%20BVH.pdf

	/// <summary>
	/// Dynamic(Incremental) BVH
	/// reference : 
	/// https://box2d.org/files/ErinCatto_DynamicBVH_Full.pdf
	/// https://www.researchgate.net/publication/270704721_Incremental_BVH_construction_for_ray_tracing
	/// https://github.com/bulletphysics/bullet3/blob/master/src/BulletCollision/BroadphaseCollision/btDbvt.cpp
	/// </summary>
	/// <typeparam name="ColliderType"></typeparam>
	template <typename ColliderType>
	class BVH
	{
		friend class BVH_TestRoom;
		friend class BVH_Node<ColliderType>;
		friend class BVH_Node_View<ColliderType>;

		using node_type = typename BVH_Node<ColliderType>;
		using node_view_type = typename BVH_Node_View<ColliderType>;

	private:

		/// <summary>
		/// array
		/// Never pop inserted node
		/// Just make it dangling
		/// </summary>
		BVH_Node<ColliderType>* mNodes;
		const int mNodeCapacity;

		int mRootNodeIndex{ NULL_NODE_INDEX };

		/// <summary>
		/// Ever used Node Count
		/// this value contain freed node count
		/// </summary>
		int mCurrentAllocatedNodeCount{ 0 };
		/// <summary>
		/// Current used node count
		/// this value doesn't contain freed node count
		/// </summary>
		int mCurrentActiveNodeCount{ 0 };
		/// <summary>
		/// if you want parallel access, Use concurrentQueue
		/// </summary>
		std::queue<int> freedNodeIndexList{};

		int PickBest(const ColliderType& L);

		int AllocateNewNode();
		int AllocateInternalNode();
		int AllocateLeafNode(const ColliderType& boundingCollider, doom::physics::Collider* collider);
		void FreeNode(int nodeIndex);

		int Balance(int lowerNodeIndex);
		void HillClimingReconstruct(int index);
		//int Balance(int index);
		void ReConstructNodeAABB(int targetNodeIndex);
		float ComputeCost();

		/// <summary>
		/// https://box2d.org/files/ErinCatto_DynamicBVH_Full.pdf see pdf 84 page
		/// </summary>
		/// <param name="newNodeAABB">Inserted ColliderType</param>
		/// <param name="silbingNodeIndexOfNewNode">Node what will be unioned with Inseted ColliderType</param>
		/// <param name="sumOfAreaSize"></param>
		/// <returns></returns>
		float InheritedCost(const ColliderType& L, const ColliderType& candidate);

		int GetSibling(int index);
		bool IsHasChild(int index);
		node_type* GetNode(int nodeIndex);
		/// <summary>
		/// longest path from node to leaf
		/// leaf node will have 0 height
		/// </summary>
		/// <param name="node"></param>
		/// <param name="index"></param>
		/// <param name="currentHeight"></param>
		/// <returns></returns>
		int GetHeight(int index, int& longestHeight = 0, int currentHeight = -1);
		int GetDepth(int index);
		int GetLeafNodeCount();
		/// <summary>
		/// Get Leaf with index
		/// </summary>
		/// <typeparam name="ColliderType"></typeparam>
		int GetLeaf(int index);
		bool IsAncesterOf(int ancesterIndex, int decesterIndex);

	public:

		constexpr BVH(int nodeCapacity) 
			:mNodeCapacity{ nodeCapacity }
		{
			this->mNodes = new BVH_Node<ColliderType>[mNodeCapacity];
		}

		~BVH()
		{
			delete[] this->mNodes;
		}

		bool BVHRayCast(const doom::physics::Ray& ray);

		/// <summary>
		/// Inser New Object(Leaf)
		/// </summary>
		/// <param name="newOjectIndex"></param>
		/// <param name="newObjectBox"></param>
		node_view_type InsertLeaf(const ColliderType& L, doom::physics::Collider* collider);
		void RemoveLeafNode(node_type* targetLeafNode);
		void RemoveLeafNode(int targetLeafNodeIndex);
		
		/// <summary>
		/// Remove paased Node And Re-insert the node
		///
		/// when update leaf node, original leaf node will be destroyed(be dangled)
		/// and return new node
		/// </summary>
		/// <param name="updatedNode"></param>
		/// <param name="force"></param>
		/// <returns></returns>
		node_view_type UpdateLeafNode(int targetLeafNodeIndex, bool force = false);


		void Traverse();

		

		constexpr node_view_type MakeBVH_Node_View(int index)
		{
			D_ASSERT(index < this->mCurrentAllocatedNodeCount);
			return node_view_type(this, index);
		}
	};

	using BVHAABB2D = typename BVH<doom::physics::AABB2D>;
	using BVHAABB3D = typename BVH<doom::physics::AABB3D>;
	using BVHSphere = typename BVH<doom::physics::Sphere>;
	
	extern template class BVH<doom::physics::AABB2D>;
	extern template class BVH<doom::physics::AABB3D>;
	extern template class BVH<doom::physics::Sphere>;

}

template <>
struct std::greater<typename doom::NodeCost>
{
	constexpr bool operator()(const doom::NodeCost& lhs, const doom::NodeCost& rhs) const
	{
		return lhs.second > lhs.second;
	}
};

