#pragma once

#include <array>
#include <utility>

#include <StaticContainer/StaticContainer.h>
#include <Entity.h>
#include "../Core/Scene/Layer.h"





namespace doom
{
	namespace graphics
	{
		class SceneGraphics;
	}
	class Renderer;

	
	template <>
	class StaticContainer<Renderer>
	{
		friend class graphics::SceneGraphics;

		using this_type = typename StaticContainer<Renderer>;

	private:

		Renderer* mRenderer_ptr;

		static inline std::array<std::vector<Renderer*>, MAX_LAYER_COUNT> mComponentsInLayer{};
		
	protected:

		void AddRendererToStaticContainer();
		void RemoveRendererToStaticContainer();

		/// <summary>
		/// Component constructor should be called before StaticIterator constructor
		/// </summary>
		StaticContainer();
		virtual ~StaticContainer();

		virtual void OnEntityLayerChanged(Entity& entity);

	public:

		[[nodiscard]] static std::pair<Renderer**, size_t> GetAllComponentsWithLayerIndex(unsigned int layerIndex);

	};

	using RendererComponentStaticIterator = StaticContainer<Renderer>;
}

