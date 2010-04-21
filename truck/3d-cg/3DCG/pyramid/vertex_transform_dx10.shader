//設定頂點的資料格式
struct VS_INPUT
{
	float4 Position : POSITION;
};

//設定vertex shader輸出格式
struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
};

//轉換矩陣
uniform row_major float4x4 viewproj_matrix;

//vertex shader
VS_OUTPUT VS(VS_INPUT In)
{
	VS_OUTPUT Out;
	//座標轉換
	Out.Position = mul(In.Position, viewproj_matrix);
	return Out;
};

//pixel shader
float4 PS(VS_OUTPUT In) : SV_Target
{
	//傳回白色
	return float4(1, 1, 1, 1);
};