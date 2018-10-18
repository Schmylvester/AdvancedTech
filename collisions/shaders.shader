// constant buffer
cbuffer perframe
{
	matrix world;
	matrix viewprojection;
};


struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR)
{
	VOut output;

	output.position = mul(position, world);
	output.position = mul(output.position, viewprojection);
	output.color = color;

	return output;
}


float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
	return float4(color.rgb, 1.0);
}