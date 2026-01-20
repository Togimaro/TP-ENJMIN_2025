#pragma once

using namespace DirectX::SimpleMath;

struct VertexLayout_Position {
	// Constructor for ease of use
	VertexLayout_Position() = default;
	VertexLayout_Position(Vector3 const& pos) noexcept : position(pos) {}

	// The actual data inside the struct
	Vector3 position;

	// Input Layout Descriptor
	static inline const std::vector<D3D11_INPUT_ELEMENT_DESC> InputElementDescs = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
};

struct VertexLayout_PositionColor {
	// Constructor for ease of use
	VertexLayout_PositionColor() = default;
	VertexLayout_PositionColor(Vector3 const& pos, Vector4 const& col) noexcept : position(pos), color(col) {}

	// The actual data inside the struct
	Vector3 position;
	Vector4 color;

	// Input Layout Descriptor
	static inline const std::vector<D3D11_INPUT_ELEMENT_DESC> InputElementDescs = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
};

struct VertexLayout_PositionColorUV {
	// Constructor for ease of use
	VertexLayout_PositionColorUV() = default;
	VertexLayout_PositionColorUV(Vector3 const& pos, Vector4 const& col, Vector2 const& uv) noexcept : position(pos), color(col), uv(uv) {}

	// The actual data inside the struct
	Vector3 position;
	Vector4 color;
	Vector2 uv;

	// Input Layout Descriptor
	static inline const std::vector<D3D11_INPUT_ELEMENT_DESC> InputElementDescs = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
};

struct VertexLayout_PositionUV {
	// Constructor for ease of use
	VertexLayout_PositionUV() = default;
	VertexLayout_PositionUV(Vector3 const& pos, Vector2 const& uv) noexcept : position(pos), uv(uv) {}

	// The actual data inside the struct
	Vector3 position;
	Vector2 uv;

	// Input Layout Descriptor
	static inline const std::vector<D3D11_INPUT_ELEMENT_DESC> InputElementDescs = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
};

struct VertexLayout_PositionNormalUV {
	// Constructor for ease of use
	VertexLayout_PositionNormalUV() = default;
	VertexLayout_PositionNormalUV(Vector3 const& pos, Vector3 const& normal, Vector2 const& uv) noexcept : position(pos), normal(normal), uv(uv) {}

	// The actual data inside the struct
	Vector3 position;
	Vector3 normal;
	Vector2 uv;

	// Input Layout Descriptor
	static inline const std::vector<D3D11_INPUT_ELEMENT_DESC> InputElementDescs = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
};
