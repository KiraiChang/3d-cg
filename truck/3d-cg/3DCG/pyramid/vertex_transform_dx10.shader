//�]�w���I����Ʈ榡
struct VS_INPUT
{
	float4 Position : POSITION;
};

//�]�wvertex shader��X�榡
struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
};

//�ഫ�x�}
uniform row_major float4x4 viewproj_matrix;

//vertex shader
VS_OUTPUT VS(VS_INPUT In)
{
	VS_OUTPUT Out;
	//�y���ഫ
	Out.Position = mul(In.Position, viewproj_matrix);
	return Out;
};

//pixel shader
float4 PS(VS_OUTPUT In) : SV_Target
{
	//�Ǧ^�զ�
	return float4(1, 1, 1, 1);
};