#pragma once

using Microsoft::WRL::ComPtr;

template<typename TVertex>
class VertexBuffer {
	std::vector<TVertex> data;
	ComPtr<ID3D11Buffer> buffer;
public:
	uint32_t PushVertex(const TVertex& vtx) {
		data.push_back(vtx);
		return data.size() - 1;
	}

	void Clear() {
		data.clear();
	}

	void Create(DeviceResources* deviceRes) {
		if (data.empty()) return;
		CD3D11_BUFFER_DESC desc(
			sizeof(TVertex) * data.size(),
			D3D11_BIND_VERTEX_BUFFER
		);
		D3D11_SUBRESOURCE_DATA initialData = {};
		initialData.pSysMem = data.data();

		deviceRes->GetD3DDevice()->CreateBuffer(
			&desc,
			&initialData,
			buffer.ReleaseAndGetAddressOf()
		);
	}

	void Apply(DeviceResources* deviceRes, int slot = 0) {
		ID3D11Buffer* vbs[] = { buffer.Get() };
		UINT strides[] = { sizeof(TVertex) };
		UINT offsets[] = { 0 };
		deviceRes->GetD3DDeviceContext()->IASetVertexBuffers(slot, 1, vbs, strides, offsets);
	}
};

class IndexBuffer {
	std::vector<uint32_t> data;
	ComPtr<ID3D11Buffer> buffer;
public:
	void PushTriangle(uint32_t a, uint32_t b, uint32_t c) {
		data.push_back(a);
		data.push_back(b);
		data.push_back(c);
	}

	void Clear() {
		data.clear();
	}

	uint32_t Size() {
		return (uint32_t)data.size();
	}

	void Create(DeviceResources* deviceRes) {
		if (data.empty()) return;
		CD3D11_BUFFER_DESC desc(
			sizeof(uint32_t) * data.size(),
			D3D11_BIND_INDEX_BUFFER
		);
		D3D11_SUBRESOURCE_DATA initialData = {};
		initialData.pSysMem = data.data();

		deviceRes->GetD3DDevice()->CreateBuffer(
			&desc,
			&initialData,
			buffer.ReleaseAndGetAddressOf()
		);
	}

	void Apply(DeviceResources* deviceRes, int slot = 0) {
		deviceRes->GetD3DDeviceContext()->IASetIndexBuffer(buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}
};

template<typename TData>
class ConstantBuffer {
	ComPtr<ID3D11Buffer> buffer;
public:
	TData data;

	void Create(DeviceResources* deviceRes) {
		CD3D11_BUFFER_DESC desc(
			sizeof(TData),
			D3D11_BIND_CONSTANT_BUFFER
		);

		deviceRes->GetD3DDevice()->CreateBuffer(
			&desc,
			NULL,
			buffer.ReleaseAndGetAddressOf()
		);
	}

	void Update(DeviceResources* deviceRes) {
		deviceRes->GetD3DDeviceContext()->UpdateSubresource(
			buffer.Get(), 0, NULL, &data, 0, 0
		);
	}

	void ApplyToVS(DeviceResources* deviceRes, int slot) {
		ID3D11Buffer* cbs[] = { buffer.Get() };
		deviceRes->GetD3DDeviceContext()->VSSetConstantBuffers(slot, 1, cbs);
	}

	void ApplyToPS(DeviceResources* deviceRes, int slot) {
		ID3D11Buffer* cbs[] = { buffer.Get() };
		deviceRes->GetD3DDeviceContext()->PSSetConstantBuffers(slot, 1, cbs);
	}
};