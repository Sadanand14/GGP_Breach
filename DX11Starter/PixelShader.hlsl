// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
Texture2D res : register(t0);
Texture2D normalMap : register(t1);
SamplerState state  : register(s0);

struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float3 worldPos		: POSITION;
	float2 uv			: TEXCOORD;
	float3 tangent		: TANGENT;
};

struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
	float Shine;
};

struct PointLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Position;
	float Shine;
	float Range;
};

cbuffer PixelBuff : register(b0)
{
	DirectionalLight DLight;
	DirectionalLight DLight2;
	PointLight PLight;
	PointLight PLight2;
	float3 cameraPos;
};

// Range-based attenuation function
float Attenuate(PointLight light, float3 worldPos)
{
	float dist = distance(light.Position, worldPos);

	// Ranged-based attenuation
	float att = saturate(1.0f - (dist * dist / (light.Range * light.Range)));

	// Soft falloff
	return att * att;
}

float4 DirectLightLambert(VertexToPixel input, DirectionalLight lightType) {
	float3 nDir = -normalize(lightType.Direction);

	float NdotL = dot(input.normal, nDir);

	NdotL = saturate(NdotL);

	// Texture surface color
	float4 textureColor = res.Sample(state, input.uv);

	if (textureColor.a < 0.2f)
		discard;

	return (lightType.AmbientColor + (lightType.DiffuseColor * NdotL)) * textureColor;
}

float4 DirectLightPhong(VertexToPixel input, DirectionalLight lightType) {
	float3 dirToCamera = normalize(cameraPos - input.worldPos);

	float3 nDir = -normalize(lightType.Direction);

	float NdotL = dot(input.normal, nDir);

	NdotL = saturate(NdotL);

	// Calculate the reflection based on the direction and the normal
	float3 reflection = reflect(lightType.Direction, input.normal);

	// Calculate the specularity based on the reflection, camera direction and the shine of the light
	float specular = pow(saturate(dot(reflection, dirToCamera)), lightType.Shine);

	// Texture surface color
	float4 textureColor = res.Sample(state, input.uv);

	if (textureColor.a < 0.2f)
		discard;

	return (lightType.AmbientColor + (lightType.DiffuseColor * NdotL) + specular.rrrr) * textureColor;
}

float4 PointLightLambert(VertexToPixel input, PointLight lightType) {
	float3 dirToPointLight = normalize(cameraPos- input.worldPos);

	float3 dirToEye = normalize(input.worldPos - cameraPos);

	float atten = Attenuate(lightType, input.worldPos);

	float NdotL = dot(input.normal, dirToPointLight);

	NdotL = saturate(NdotL);

	// Texture surface color
	float4 textureColor = res.Sample(state, input.uv);

	if (textureColor.a < 0.2f)
		discard;

	return (lightType.AmbientColor + (lightType.DiffuseColor * NdotL)) * textureColor * atten;
}

float4 PointLightPhong(VertexToPixel input, PointLight lightType) {
	float3 dirToPointLight = normalize(cameraPos - input.worldPos);

	float3 dirToEye = normalize(input.worldPos - cameraPos);

	float atten = Attenuate(lightType, input.worldPos);

	float NdotL = dot(input.normal, dirToPointLight);
	float NdotEye = dot(input.normal, dirToEye);

	NdotL = saturate(NdotL);
	NdotEye = saturate(NdotEye);

	float3 reflection = reflect(-dirToPointLight, input.normal);

	float specular = pow(saturate(dot(reflection, dirToPointLight)), lightType.Shine);

	// Texture surface color
	float4 textureColor = res.Sample(state, input.uv);

	if (textureColor.a < 0.2f)
		discard;

	return float4((lightType.AmbientColor * textureColor.rgb) + (lightType.DiffuseColor * NdotL* textureColor.rgb) + specular, textureColor.a) * atten;
}

float4 main(VertexToPixel input) : SV_TARGET
{
	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);

	/// Code section for normal maps
	float3 unpackedNormal = normalMap.Sample(state, input.uv).rgb * 2 - 1;

	float3 N = input.normal;
	float3 T = normalize(input.tangent - N * dot(input.tangent, N));
	float3 B = cross(T, N);
	float3x3 TBN = float3x3(T, B, N);

	input.normal = normalize(mul(unpackedNormal, TBN));
	///

	// Directional light calculations for both Lambert and Phong shading
	//return (DirectLightLambert(input, DLight) + DirectLightLambert(input, DLight2));
	//return (DirectLightPhong(input, DLight) + DirectLightPhong(input, DLight2));

	// Point light calculations for both Lambert and Phong shading
	//return (PointLightLambert(input, PLight) + PointLightLambert(input, PLight2));
	return (PointLightPhong(input, PLight) + PointLightPhong(input, PLight2));
}