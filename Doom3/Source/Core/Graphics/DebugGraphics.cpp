#include "Core.h"
#include "DebugGraphics.h"

#ifdef DEBUG_MODE

#include "Graphics_Server.h"
#include "../Game/AssetManager/AssetManager.h"
#include "Material.h"

void doom::graphics::DebugGraphics::Init()
{
	this->mDebugMesh.GenMeshBuffer(false);
	this->mDebugMesh.BufferData(MAX_DEBUG_VERTEX_COUNT * 3, NULL, ePrimitiveType::LINES, Mesh::eVertexArrayFlag::VertexVector3);


	auto debug2DShader = doom::assetimporter::AssetManager::GetAsset<asset::eAssetType::SHADER>(DebugGraphics::DEBUG_2D_SHADER);
	this->m2DMaterial = std::make_unique<Material>(debug2DShader);

	auto debug3DShader = doom::assetimporter::AssetManager::GetAsset<asset::eAssetType::SHADER>(DebugGraphics::DEBUG_3D_SHADER);
	this->m3DMaterial = std::make_unique<Material>(debug3DShader);
}

void doom::graphics::DebugGraphics::Reset()
{
	this->mDebugMeshCount = 0;
	for (size_t i = 0; i < this->m2dLine.size(); i++)
	{
		this->m2dLine[i].clear();
	}

	for (size_t i = 0; i < this->m3dLine.size(); i++)
	{
		this->m3dLine[i].clear();
	}

	for (size_t i = 0; i < this->m2dTriangle.size(); i++)
	{
		this->m2dTriangle[i].clear();
	}

	for (size_t i = 0; i < this->m3dTriangle.size(); i++)
	{
		this->m3dTriangle[i].clear();
	}
}



doom::graphics::DebugGraphics::DebugGraphics() : m2DMaterial{}, m3DMaterial{}, m2dLine{}, m3dLine{}, m2dTriangle{}, m3dTriangle{}
{
	for (size_t i = 0; i < this->m2dLine.size(); i++)
	{
		this->m2dLine[i].reserve(10000);
	}

	for (size_t i = 0; i < this->m3dLine.size(); i++)
	{
		this->m3dLine[i].reserve(10000);
	}

	for (size_t i = 0; i < this->m2dTriangle.size(); i++)
	{
		this->m2dTriangle[i].reserve(10000);
	}

	for (size_t i = 0; i < this->m3dTriangle.size(); i++)
	{
		this->m3dTriangle[i].reserve(10000);
	}
}

void doom::graphics::DebugGraphics::DrawDebug()
{
	if (DebugGraphics::mbDrawDebug == false)
		return;

	//
	this->DebugDraw3DLine(math::Vector3::right * -100, math::Vector3::right * 100, eColor::Red);
	this->DebugDraw3DLine(math::Vector3::up * -100, math::Vector3::up * 100, eColor::Green);
	this->DebugDraw3DLine(math::Vector3::forward * -100, math::Vector3::forward * 100, eColor::Blue);
	//

	//TODO : This function codes is too messy. Clean It!!!
	//DRAW 2D FIRST

	/// <summary>
	/// vector3 -> 3, vector4 -> 4
	/// </summary>
	int offsetComponentCount{ 0 };
	unsigned int alreadyDrawedVertexCount{ 0 };

	
	this->m2DMaterial->UseProgram();
	for (size_t i = 0; i < this->m2dLine.size(); i++)
	{
		unsigned int lineCount = static_cast<unsigned int>(this->m2dLine[i].size());
		if (lineCount > 0)
		{
			this->mDebugMesh.BufferSubData(lineCount * 6, this->m2dLine[i].data(), offsetComponentCount * sizeof(float));

			D_ASSERT(MAX_DEBUG_VERTEX_COUNT >= alreadyDrawedVertexCount + lineCount * 2);

			this->m2DMaterial->SetVector4(0, Color::GetColor(static_cast<eColor>(i)));
			this->mDebugMesh.DrawArray(ePrimitiveType::LINES, alreadyDrawedVertexCount, lineCount * 2);

			offsetComponentCount += lineCount * 6;
			alreadyDrawedVertexCount += lineCount * 2;
		}
	}

	for (size_t i = 0; i < this->m2dTriangle.size(); i++)
	{
		unsigned int triangleCount = static_cast<unsigned int>(this->m2dTriangle[i].size());
		if (triangleCount > 0)
		{
			this->mDebugMesh.BufferSubData(triangleCount * 9, this->m2dTriangle[i].data(), offsetComponentCount * sizeof(float));

			

			D_ASSERT(MAX_DEBUG_VERTEX_COUNT >= alreadyDrawedVertexCount + triangleCount * 3);

			this->m2DMaterial->SetVector4(0, Color::GetColor(static_cast<eColor>(i)));
			this->mDebugMesh.DrawArray(ePrimitiveType::TRIANGLES, alreadyDrawedVertexCount, triangleCount * 3);

			offsetComponentCount += triangleCount * 9;
			alreadyDrawedVertexCount += triangleCount * 3;
		}
	}


	this->m3DMaterial->UseProgram();
	for (size_t i = 0; i < this->m3dLine.size(); i++)
	{
		unsigned int lineCount = static_cast<unsigned int>(this->m3dLine[i].size());
		if (lineCount > 0)
		{
			this->mDebugMesh.BufferSubData(lineCount * 6, this->m3dLine[i].data(), offsetComponentCount * sizeof(float));

		

			D_ASSERT(MAX_DEBUG_VERTEX_COUNT >= alreadyDrawedVertexCount + lineCount * 2);

			this->m3DMaterial->SetVector4(0, Color::GetColor(static_cast<eColor>(i)));
			this->mDebugMesh.DrawArray(ePrimitiveType::LINES, alreadyDrawedVertexCount, lineCount * 2);

			offsetComponentCount += lineCount * 6;
			alreadyDrawedVertexCount += lineCount * 2;
		}
	}

	for (size_t i = 0; i < this->m3dTriangle.size(); i++)
	{
		unsigned int triangleCount = static_cast<unsigned int>(this->m3dTriangle[i].size());
		if (triangleCount > 0)
		{
			this->mDebugMesh.BufferSubData(triangleCount * 9, this->m3dTriangle[i].data(), offsetComponentCount * sizeof(float));

			D_ASSERT(MAX_DEBUG_VERTEX_COUNT >= alreadyDrawedVertexCount + triangleCount * 3);

			this->m3DMaterial->SetVector4(0, Color::GetColor(static_cast<eColor>(i)));
			this->mDebugMesh.DrawArray(ePrimitiveType::TRIANGLES, alreadyDrawedVertexCount, triangleCount * 3);

			offsetComponentCount += triangleCount * 9;
			alreadyDrawedVertexCount += triangleCount * 3;
		}
	}

	this->Reset();
}

void doom::graphics::DebugGraphics::DebugDraw3DLine(const math::Vector3& startWorldPos, const math::Vector3& endWorldPos, eColor color, bool drawInstantly /*= false*/)
{
	if (drawInstantly == false)
	{
		this->m3dLine[static_cast<unsigned int>(color)].emplace_back(startWorldPos, endWorldPos);
	}
	else
	{
		this->m3DMaterial->UseProgram();
		this->m3DMaterial->SetVector4(0, Color::GetColor(static_cast<eColor>(color)));
		float data[6]{ startWorldPos.x, startWorldPos.y, startWorldPos.z, endWorldPos.x, endWorldPos.y, endWorldPos.z };
		this->mDebugMesh.BufferSubData(6, data, 0);
		this->mDebugMesh.DrawArray(ePrimitiveType::LINES, 0, 2);
	}

}



/// <summary>
/// Why Use Vector3 not Vector2??
/// Debug mesh's Vertex Array Object have already define that Vector atribute have 3 component.
/// So For reducing complexity, Just use vector3
/// </summary>
/// <param name="startNDCPos"></param>
/// <param name="endNDCPos"></param>
/// <param name="color"></param>
void doom::graphics::DebugGraphics::DebugDraw2DLine(const math::Vector3& startNDCPos, const math::Vector3& endNDCPos, eColor color, bool drawInstantly /*= false*/)
{
	if (drawInstantly == false)
	{
		this->m2dLine[static_cast<unsigned int>(color)].emplace_back(startNDCPos, startNDCPos);
	}
	else
	{
		this->DebugDraw2DLineInstantly(startNDCPos, endNDCPos, color);
	}
}

void doom::graphics::DebugGraphics::DebugDraw2DLineInstantly(const math::Vector3& startNDCPos, const math::Vector3& endNDCPos, eColor color)
{
	if (mDrawInstantlyMaterial != nullptr)
	{
		mDrawInstantlyMaterial->UseProgram();
		mDrawInstantlyMaterial->SetVector4(0, Color::GetColor(static_cast<eColor>(color)));
	}
	else
	{
		DEBUG_MODE("Please set mDrawInstantlyMaterial", eLogType::D_WARNING);
		this->m2DMaterial->UseProgram();
		this->m2DMaterial->SetVector4(0, Color::GetColor(static_cast<eColor>(color)));
	}
	
	float data[6]{ startNDCPos.x, startNDCPos.y, startNDCPos.z, endNDCPos.x, endNDCPos.y, endNDCPos.z };

	this->mDebugMesh.BufferSubData(6, data, 0);
	this->mDebugMesh.DrawArray(ePrimitiveType::LINES, 0, 2);
}



void doom::graphics::DebugGraphics::DebugDraw2DTriangleInstantly(const math::Vector3& pointA, const math::Vector3& pointB, const math::Vector3& pointC, eColor color)
{
	if (mDrawInstantlyMaterial != nullptr)
	{
		DEBUG_MODE("Please set mDrawInstantlyMaterial", eLogType::D_WARNING);
		mDrawInstantlyMaterial->UseProgram();
		mDrawInstantlyMaterial->SetVector4(0, Color::GetColor(static_cast<eColor>(color)));
	}
	else
	{
		this->m2DMaterial->UseProgram();
		this->m2DMaterial->SetVector4(0, Color::GetColor(static_cast<eColor>(color)));
	}

	float data[9]{ pointA.x, pointA.y, pointA.z, pointB.x, pointB.y, pointB.z, pointC.x, pointC.y, pointC.z };
	this->mDebugMesh.BufferSubData(9, data, 0);
	this->mDebugMesh.DrawArray(ePrimitiveType::TRIANGLES, 0, 3);
}

void doom::graphics::DebugGraphics::DebugDraw2DTriangle(const math::Vector3& pointA, const math::Vector3& pointB, const math::Vector3& pointC, eColor color, bool drawInstantly /*= false*/)
{
	if (drawInstantly == false)
	{
		this->m2dTriangle[static_cast<unsigned int>(color)].emplace_back(pointA, pointB, pointC);
	}
	else
	{
		this->DebugDraw2DTriangleInstantly(pointA, pointB, pointC, color);
	}
}

void doom::graphics::DebugGraphics::SetDrawInstantlyMaterial(Material* material)
{
	this->mDrawInstantlyMaterial = material;
}

void doom::graphics::DebugGraphics::DebugDraw3DTriangle(const math::Vector3& pointA, const math::Vector3& pointB, const math::Vector3& pointC, eColor color, bool drawInstantly /*= false*/)
{
	if (drawInstantly == false)
	{
		this->m3dTriangle[static_cast<unsigned int>(color)].emplace_back(pointA, pointB, pointC);

		//For Drawing both face
		this->m3dTriangle[static_cast<unsigned int>(color)].emplace_back(pointC, pointB, pointA);
	}
	else
	{
		this->m3DMaterial->UseProgram();
		this->m3DMaterial->SetVector4(0, Color::GetColor(static_cast<eColor>(color)));
		float data[9]{ pointA.x, pointA.y, pointA.z, pointB.x, pointB.y, pointB.z, pointC.x, pointC.y, pointC.z };
		this->mDebugMesh.BufferSubData(9, data, 0);
		this->mDebugMesh.DrawArray(ePrimitiveType::TRIANGLES, 0, 3);
	}
}


#endif