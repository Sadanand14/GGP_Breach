struct VertexToPixel 
{
	float3 Position : POSITION;
	float3 Normal	: NORMAL;
	float2 UV		: UV;
};

float4 main(VertexToPixel input) : SV_TARGET
{
	return float4(0,0,1,1);
}