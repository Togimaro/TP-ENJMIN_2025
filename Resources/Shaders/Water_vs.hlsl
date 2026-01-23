struct Input {
    float3 pos : POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct Output {
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
};

cbuffer ModelData : register(b0) {
    float4x4 Model;
};
cbuffer CameraData : register(b1) {
    float4x4 View;
    float4x4 Projection;
};
cbuffer GlobalData : register(b2) {
    float4 Times;
};


Output main(Input input) {
	Output output = (Output)0;
    
    float4 localPos = float4(input.pos, 1);
    
    float4 globalPos = mul(localPos, Model);
    localPos.y += sin(globalPos.x + Times.x) * 0.1 + cos(globalPos.z + Times.x + 0.2) * 0.05;
    
    output.pos = mul(localPos, Model);
    output.pos = mul(output.pos, View);
    output.pos = mul(output.pos, Projection);
    output.uv = input.uv;
    output.normal = input.normal;
    
	return output;
}