#include "MathUtility.h"

const Vector2 Vector2::UnitX = Vector2(1.0f, 0.0f);
const Vector2 Vector2::UnitY = Vector2(0.0f, 1.0f);
const Vector2 Vector2::Zero = Vector2(0.0f, 0.0f);

const Vector3 Vector3::UnitX = Vector3(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::UnitY = Vector3(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::UnitZ = Vector3(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::Zero = Vector3(0.0f, 0.0f, 0.0f);

const Vector4 Vector4::Zero = Vector4(0.0f, 0.0f, 0.0f, 0.0f);

const Quaternion Quaternion::Identity = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

const Matrix33 Matrix33::Identity = Matrix33(
	1.0f, 0.0f, 0.0f, 
	0.0f, 1.0f, 0.0f, 
	0.0f, 0.0f, 1.0f);

const Matrix44 Matrix44::Identity = Matrix44(
	1.0f, 0.0f, 0.0f, 0.0f, 
	0.0f, 1.0f, 0.0f, 0.0f, 
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f);

unsigned int Color::HSVAtoRGBA_uint(float h, float s, float v, unsigned int a) {
	Vector4 rgba;

	float H = Math::Fmod(h, 360.0f);
	float V = Math::Clamp(v, 0.0f, 1.0f);
	float S = Math::Clamp(s, 0.0f, 1.0f);;
	if (S <= 0) {
		rgba.xyz(Vector3(v));
	}
	else {

		int Hi = (int)(H / 60.0f);
		float F = H / 60.0f - (float)Hi;
		float M = V * (1.0f - S);
		float N = V * (1.0f - S * F);
		float K = V * (1.0f - S * (1.0f - F));

		switch (Hi)
		{
		case 0:
			rgba.xyz(Vector3(V, K, M));
			break;
		case 1:
			rgba.xyz(Vector3(N, V, M));
			break;
		case 2:
			rgba.xyz(Vector3(M, V, K));
			break;
		case 3:
			rgba.xyz(Vector3(M, N, V));
			break;
		case 4:
			rgba.xyz(Vector3(K, M, V));
			break;
		case 5:
		default:
			rgba.xyz(Vector3(V, M, N));
			break;
		}
	}
	return Color::Create((unsigned int)rgba.x, (unsigned int)rgba.y, (unsigned int)rgba.z, a);
}

Matrix33 Matrix33::CreateFromMatrix44(const Matrix44& mat)
{
	return Matrix33(
		mat[0][0], mat[0][1], mat[0][3],
		mat[1][0], mat[1][1], mat[1][3],
		mat[3][0], mat[3][1], mat[3][3]);
}

Matrix44 Matrix44::CreateFromMatrix33(const Matrix33& mat)
{
	return Matrix44(
		mat[0][0],	mat[0][1],	0.0f,	mat[0][2],
		mat[1][0],	mat[1][1],	0.0f,	mat[1][2],
		0.0f,		0.0f,		1.0f,	0.0f,
		mat[2][0],	mat[2][1],	0.0f,	mat[2][2]);
}
