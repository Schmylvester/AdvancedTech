cbuffer cbPerObject
{
	float4x4 WVP;
	float4x4 World;
};

struct Light
{
	float3 dir;
	float4 ambient;
	float4 diffuse;
};

cbuffer cbPerFrame
{
	Light light;
};

struct VOut
{
	float4 position : SV_POSITION;
	float4 colour : COLOUR;
	float3 normal : NORMAL;
};

VOut VShader(float4 position : POSITION, float4 colour : COLOUR, float3 normal : NORMAL)
{
	VOut output;

	output.position = mul(position, WVP);
	output.normal = mul(normal, World);
	output.colour = colour;

	return output;
}


float4 PShader(float4 position : SV_POSITION, float4 colour : COLOUR, float3 normal : NORMAL) : SV_TARGET
{
	normal = normalize(normal);
	float4 ret_colour;

	ret_colour = colour * light.ambient;
	ret_colour += saturate(dot(light.dir, normal) * light.diffuse * colour);

	return ret_colour;
}