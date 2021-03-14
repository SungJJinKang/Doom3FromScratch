#pragma once
#include "utility/BVH/BVH.h"

#define VIEWFRUSTUM_BVH_MAX_NODE_COUNT 1000

namespace doom
{
	class Renderer;

	namespace graphics
	{
		/// <summary>
		/// View Frustum Culling Implementation
		/// 
		/// reference : https://cgvr.informatik.uni-bremen.de/teaching/cg_literatur/lighthouse3d_view_frustum_culling/index.html
		/// 
		/// 
		/// 
		/// KDƮ���� ������ ������ ���� ������ ������ ��ü�� ���� �ʴ�.
		/// BVH�� ������ �� ����������, ������ ��ü�� ������ �����ϴ�.
		/// BVH�� KDƮ���� �����Ѵٰ� �� �� �ִ�.
		/// https://blog.hybrid3d.dev/2019-03-22-raytracing-kdtree-bvh
		/// </summary>
		class ViewFrustumCulling
		{
			friend class Graphics_Server;
			friend class ::doom::Renderer;

		private:
			BVH3D mViewFrustumBVH{ VIEWFRUSTUM_BVH_MAX_NODE_COUNT };
			void DebugDraw();
		public:
		};
	}
}

