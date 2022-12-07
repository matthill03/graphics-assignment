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

struct VertexIn
{
	float3 InputPosition : POSITION;
	float3 Normal		 : NORMAL;
};

struct VertexOut
{
	float4 OutputPosition	: SV_POSITION;
	float4 Normal			: TEXCOORD0;
	float4 WorldPosition	: TEXCOORD1;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to homogeneous clip space.
	vout.OutputPosition = mul(worldViewProjection, float4(vin.InputPosition, 1.0f));

	
	float4 adjustedNormal = normalize(mul(worldTransformation, float4(vin.Normal, 0.0f)));

	vout.Normal = adjustedNormal;
	vout.WorldPosition = mul(worldTransformation, float4(vin.InputPosition, 1.0f));

	// calculate the diffuse light and add it to the ambient light
	//float diffuseBrightness = saturate(dot(adjustedNormal, vectorBackToLight));
	//vout.Colour = saturate(ambientLightColour + diffuseBrightness * directionalLightColour);
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{ 
	float4 vectorBackToLight = -directionalLightVector;

	// calculate the diffuse light and add it to the ambient light
	float diffuseBrightness = saturate(dot(pin.Normal, vectorBackToLight));
	float4 Colour = saturate(ambientLightColour + diffuseBrightness * directionalLightColour);

	return Colour;
}


