struct VertexShaderInput
{
	float3 Position : POSITION;
	float3 Normal	: NORMAL;
	float2 UV		: UV;
};

struct VertexToPixel
{
	float3 Position : POSITION;
	float3 Normal	: NORMAL;
	float2 UV		: UV;
};

VertexToPixel main(VertexShaderInput input)
{
	VertexToPixel Output;
	return Output;
}