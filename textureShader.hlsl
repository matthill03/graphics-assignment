cbuffer ConstantBuffer
{
	matrix	worldViewProjection;
	matrix  worldTransformation;
	float4	ambientLightColour;
	float4  directionalLightColour;
	float4  directionalLightVector;
	float3	eyePosition;
	float	specularPower;
	float4	specularColour;
	float3	padding;

};

Texture2D Texture;
SamplerState ss;

struct VertexIn
{
	float3 InputPosition : POSITION;
	float3 Normal		 : NORMAL;
	float2 TexCoord		 : TEXTURE;
};

struct VertexOut
{
	float4 OutputPosition	: SV_POSITION;
	float4 Normal			: TEXCOORD0;
	float4 WorldPosition	: TEXCOORD1;
	float2 TexCoord			: TEXTURE;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	// Transform to homogeneous clip space.
	vout.OutputPosition = mul(worldViewProjection, float4(vin.InputPosition, 1.0f));

	// calculate the diffuse light and add it to the ambient light
	float4 adjustedNormal = normalize(mul(worldTransformation, float4(vin.Normal, 0.0f)));

	vout.Normal = adjustedNormal;
	vout.WorldPosition = mul(worldTransformation, float4(vin.InputPosition, 1.0f));
	vout.TexCoord = vin.TexCoord;
	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	float4 vectorBackToLight = -directionalLightVector;

	float4 lightDir = normalize(pin.OutputPosition - directionalLightVector);
	float4 viewDir = normalize(pin.OutputPosition - float4(eyePosition, 1.0f));
	float4 halfwayDir = normalize(lightDir + viewDir);
	float halfwayMag = length(halfwayDir);

	float4 halfway = normalize(halfwayDir) / normalize(halfwayMag);

	float specular = pow(saturate(dot(pin.Normal, halfway)), specularPower);
	float4 specCol = specular * specularColour;


	// calculate the diffuse light and add it to the ambient light
	float diffuseBrightness = saturate(dot(pin.Normal, vectorBackToLight));
	float4 Colour = saturate(ambientLightColour) + (saturate(diffuseBrightness) * saturate(directionalLightColour) * saturate(specCol));

	return Colour * Texture.Sample(ss, pin.TexCoord);
}


