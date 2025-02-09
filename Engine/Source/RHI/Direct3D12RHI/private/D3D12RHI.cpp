#include <array.h>
#include <D3D12Mesh.h>
#include <D3D12RHI.h>
#include <D3D12RHIPrivate.h>
#include <D3D12Material.h>
#include <Math/Vector.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		using namespace Core;

		D3D12RHI::D3D12RHI() :
			m_d3d12Private(std::make_shared<D3D12RHIPrivate>())
		{
			m_d3d12Private->EnableDebugLayer();
		}

		void D3D12RHI::Init()
		{
			m_d3d12Private->Init();
		}

		void D3D12RHI::Update(Mesh::Ptr mesh, Material::Ptr material)
		{
			m_d3d12Private->Update(mesh, material);
		}

		Mesh::Ptr D3D12RHI::CreateBoxMesh()
		{
			array<Vertex, 8> vertices =
			{
				Vertex({ Math::Vector3f(-1.0f, -1.0f, -1.0f), Math::Vector4f((float*)&DirectX::Colors::White) }),
				Vertex({ Math::Vector3f(-1.0f, +1.0f, -1.0f), Math::Vector4f((float*)&DirectX::Colors::Black) }),
				Vertex({ Math::Vector3f(+1.0f, +1.0f, -1.0f), Math::Vector4f((float*)&DirectX::Colors::Red) }),
				Vertex({ Math::Vector3f(+1.0f, -1.0f, -1.0f), Math::Vector4f((float*)&DirectX::Colors::Green) }),
				Vertex({ Math::Vector3f(-1.0f, -1.0f, +1.0f), Math::Vector4f((float*)&DirectX::Colors::Blue) }),
				Vertex({ Math::Vector3f(-1.0f, +1.0f, +1.0f), Math::Vector4f((float*)&DirectX::Colors::Yellow) }),
				Vertex({ Math::Vector3f(+1.0f, +1.0f, +1.0f), Math::Vector4f((float*)&DirectX::Colors::Cyan) }),
				Vertex({ Math::Vector3f(+1.0f, -1.0f, +1.0f), Math::Vector4f((float*)&DirectX::Colors::Magenta) })
			};

			array<uint16_t, 36> indices =
			{
				// front face
				0, 1, 2,
				0, 2, 3,
				// back face
				4, 6, 5,
				4, 7, 6,
				// left face
				4, 5, 1,
				4, 1, 0,
				// right face
				3, 2, 6,
				3, 6, 7,
				// top face
				1, 5, 6,
				1, 6, 2,
				// bottom face
				4, 0, 3,
				4, 3, 7
			};

			return m_d3d12Private->CreateMesh(vertices.begin(), vertices.size(), sizeof(Vertex), indices.begin(), indices.size(), sizeof(uint16_t));
		}

		Mesh::Ptr D3D12RHI::CreateStarMesh()
		{

			auto vertices = std::vector<Vertex>{{
				// forward face
				Vertex({ Math::Vector3f(-0.55f, -0.84f, -0.2f), Math::Vector4f((float*)&DirectX::Colors::Red) }),
				Vertex({ Math::Vector3f(-0.45f, -0.23f, -0.2f), Math::Vector4f((float*)&DirectX::Colors::Red) }),
				Vertex({ Math::Vector3f(-0.90f,  0.21f, -0.2f), Math::Vector4f((float*)&DirectX::Colors::Red) }),
				Vertex({ Math::Vector3f(-0.28f,  0.30f, -0.2f), Math::Vector4f((float*)&DirectX::Colors::Red) }),

				Vertex({ Math::Vector3f( 0.00f,  0.86f, -0.2f), Math::Vector4f((float*)&DirectX::Colors::Red) }),

				Vertex({ Math::Vector3f( 0.28f,  0.30f, -0.2f), Math::Vector4f((float*)&DirectX::Colors::Red) }),
				Vertex({ Math::Vector3f( 0.90f,  0.21f, -0.2f), Math::Vector4f((float*)&DirectX::Colors::Red) }),
				Vertex({ Math::Vector3f( 0.45f, -0.23f, -0.2f), Math::Vector4f((float*)&DirectX::Colors::Red) }),
				Vertex({ Math::Vector3f( 0.55f, -0.84f, -0.2f), Math::Vector4f((float*)&DirectX::Colors::Red) }),

				Vertex({ Math::Vector3f( 0.00f, -0.55f, -0.2f), Math::Vector4f((float*)&DirectX::Colors::Red) }),

				Vertex({ Math::Vector3f( 0.00f,  0.00f, -0.2f), Math::Vector4f((float*)&DirectX::Colors::Yellow) }),
				
				// backward face
				Vertex({ Math::Vector3f(-0.55f, -0.84f, 0.2f), Math::Vector4f((float*)&DirectX::Colors::Red) }),
				Vertex({ Math::Vector3f(-0.45f, -0.23f, 0.2f), Math::Vector4f((float*)&DirectX::Colors::Red) }),
				Vertex({ Math::Vector3f(-0.90f,  0.21f, 0.2f), Math::Vector4f((float*)&DirectX::Colors::Red) }),
				Vertex({ Math::Vector3f(-0.28f,  0.30f, 0.2f), Math::Vector4f((float*)&DirectX::Colors::Red) }),

				Vertex({ Math::Vector3f( 0.00f,  0.86f, 0.2f), Math::Vector4f((float*)&DirectX::Colors::Red) }),

				Vertex({ Math::Vector3f( 0.28f,  0.30f, 0.2f), Math::Vector4f((float*)&DirectX::Colors::Red) }),
				Vertex({ Math::Vector3f( 0.90f,  0.21f, 0.2f), Math::Vector4f((float*)&DirectX::Colors::Red) }),
				Vertex({ Math::Vector3f( 0.45f, -0.23f, 0.2f), Math::Vector4f((float*)&DirectX::Colors::Red) }),
				Vertex({ Math::Vector3f( 0.55f, -0.84f, 0.2f), Math::Vector4f((float*)&DirectX::Colors::Red) }),

				Vertex({ Math::Vector3f( 0.00f, -0.55f, 0.2f), Math::Vector4f((float*)&DirectX::Colors::Red) }),

				Vertex({ Math::Vector3f( 0.00f,  0.00f, 0.2f), Math::Vector4f((float*)&DirectX::Colors::Yellow) }),
			}};

			auto indices = std::vector<uint16_t>{{
				0, 1, 10,
				1, 2, 10,
				2, 3, 10,
				3, 4, 10,
				4, 5, 10,
				5, 6, 10,
				6, 7, 10,
				7, 8, 10,
				8, 9, 10,
				9, 0, 10,

				1 + 11, 0 + 11, 10 + 11,
				2 + 11, 1 + 11, 10 + 11,
				3 + 11, 2 + 11, 10 + 11,
				4 + 11, 3 + 11, 10 + 11,
				5 + 11, 4 + 11, 10 + 11,
				6 + 11, 5 + 11, 10 + 11,
				7 + 11, 6 + 11, 10 + 11,
				8 + 11, 7 + 11, 10 + 11,
				9 + 11, 8 + 11, 10 + 11,
				0 + 11, 9 + 11, 10 + 11,

				 0, 11,  1,
				11, 12,  1,
				13,  1, 12,
				13,  2,  1,

				 0 + 2, 11 + 2,  1 + 2,
				11 + 2, 12 + 2,  1 + 2,
				13 + 2,  1 + 2, 12 + 2,
				13 + 2,  2 + 2,  1 + 2,

				 0 + 4, 11 + 4,  1 + 4,
				11 + 4, 12 + 4,  1 + 4,
				13 + 4,  1 + 4, 12 + 4,
				13 + 4,  2 + 4,  1 + 4,

				 0 + 6, 11 + 6,  1 + 6,
				11 + 6, 12 + 6,  1 + 6,
				13 + 6,  1 + 6, 12 + 6,
				13 + 6,  2 + 6,  1 + 6,

				 0 + 8, 11 + 8,  1 + 8,
				11 + 8, 12 + 8,  1 + 8,
				13 + 8,  1 + 8, 12 + 8,
				13 + 8,  2 + 8,  1 + 8,
			}};


			return m_d3d12Private->CreateMesh(vertices.data(), vertices.size(), sizeof(Vertex), indices.data(), indices.size(), sizeof(uint16_t));
		}

		Material::Ptr D3D12RHI::GetMaterial(const std::string& name)
		{
			return m_d3d12Private->GetMaterial(name);
		}

		void D3D12RHI::ExecuteCommandLists()
		{
			m_d3d12Private->ExecuteCommandLists();
		}

		void D3D12RHI::Flush()
		{
			m_d3d12Private->FlushCommandQueue();
		}

		void D3D12RHI::OnResize()
		{
			m_d3d12Private->OnResize();
		}
	}
}