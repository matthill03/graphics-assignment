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
	float4	specularDirection;
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

    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{ 
	float4 vectorBackToLight = -directionalLightVector;

	// calculate the diffuse light and add it to the ambient light
	float diffuseBrightness = saturate(dot(pin.Normal, vectorBackToLight));
	float4 Colour = saturate(ambientLightColour) + (saturate(diffuseBrightness) * saturate(directionalLightColour));

	// Calculate spot lighting using blinn phone model
	float4 lightDir = normalize(pin.WorldPosition - specularDirection);
	float4 viewDir = normalize(pin.WorldPosition - float4(eyePosition, 1.0f));

	float4 halfwayDir = normalize(lightDir - viewDir);
	float halfwayMag = length(halfwayDir);

	float4 halfway = normalize(halfwayDir) / normalize(halfwayMag);

	float specular = pow(saturate(dot(halfway, pin.Normal)), specularPower);

	float4 specCol = specular * specularColour;

	// Add spot lighting to the diffuse lighting
	float4 newColour = Colour + specCol;
	return newColour;


}
