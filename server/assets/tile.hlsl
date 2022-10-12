//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2019 Ryo Suzuki
//	Copyright (c) 2016-2019 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

Texture2D		g_texture0 : register(t0);
SamplerState	g_sampler0 : register(s0);

cbuffer PSConstants2D : register(b0)
{
	float4 g_colorAdd;
	float4 g_sdfParam;
	float4 g_internal;
}

struct PSInput
{
	float4 position	: SV_POSITION;
	float4 color	: COLOR0;
	float2 uv		: TEXCOORD0;
};

float4 PS(PSInput input) : SV_TARGET
{
	float t = 8.0;
	float x = min(int(g_texture0.Sample(g_sampler0, input.uv).x * t) / t, 1.0 - 1.0 / t);
	float y = min(int(g_texture0.Sample(g_sampler0, input.uv).y * t) / t, 1.0 - 1.0 / t);
	float z = min(int(g_texture0.Sample(g_sampler0, input.uv).z * t) / t, 1.0 - 1.0 / t);
	float w = min(int(g_texture0.Sample(g_sampler0, input.uv).w * t) / t, 1.0 - 1.0 / t);

	return float4(x, y, z, w);
}
