#pragma once
#ifndef MATHUTILITY_H_
#define MATHUTILITY_H_

#include <cmath>
#include <cassert>

struct Vector2;
struct Vector3;
struct Vector4;
struct Quaternion;
struct Matrix33;
struct Matrix44;

/// <summary>
/// 数学
/// </summary>
namespace Math
{
	constexpr float Pi = 3.14159265359f;
	constexpr float TwoPi = 2.0f * Pi;
	constexpr float PiDiv2 = Pi / 2.0f;

	/// <summary>
	/// 弧度法変換
	/// </summary>
	/// <param name="degrees">度数法</param>
	/// <returns>弧度法</returns>
	constexpr inline float ToRadians(float degrees) {
		return (degrees * Pi / 180.0f);
	}
	/// <summary>
	/// 度数法変換
	/// </summary>
	/// <param name="radians">弧度法</param>
	/// <returns>度数法</returns>
	constexpr inline float ToDegrees(float radians) {
		return (radians * 180.0f / Pi);
	}
	/// <summary>
	/// ほぼゼロ
	/// </summary>
	/// <param name="value">値</param>
	/// <param name="epsilon">誤差</param>
	/// <returns>true : false</returns>
	inline bool NearZero(float value, float epsilon = 0.01f) {
		return (fabs(value) <= epsilon);
	}
	/// <summary>
	/// 符号を判断
	/// </summary>
	/// <typeparam name="TYPE">型</typeparam>
	/// <param name="v">値</param>
	/// <returns> 0は0 </returns>
	template <typename TYPE>
	int GetSign(TYPE v) {
		return (v > 0) - (v < 0);
	}
	/// <summary>
	/// 大きいほうを求める
	/// </summary>
	/// <typeparam name="TYPE">型</typeparam>
	/// <param name="v1">値１</param>
	/// <param name="v2">値２</param>
	/// <returns>値１　or　値２</returns>
	template <typename TYPE>
	TYPE Max(const TYPE& v1, const TYPE& v2) {
		return (v1 < v2 ? v2 : v1);
	}
	/// <summary>
	/// 小さいほうを求める
	/// </summary>
	/// <typeparam name="TYPE">型</typeparam>
	/// <param name="v1">値１</param>
	/// <param name="v2">値２</param>
	/// <returns>値１　or　値２</returns>
	template <typename TYPE>
	TYPE Min(const TYPE& v1, const TYPE& v2) {
		return (v1 < v2 ? v1 : v2);
	}
	// 値を入れ替える
	template <typename TYPE>
	void Swap(TYPE& v1, TYPE& v2) {
		TYPE tmp = v1;
		v1 = v2;
		v2 = tmp;
	}
	/// <summary>
	/// 値を収める
	/// </summary>
	/// <typeparam name="TYPE">型</typeparam>
	/// <param name="value">値</param>
	/// <param name="lower">下限</param>
	/// <param name="upper">上限</param>
	/// <returns></returns>
	template <typename TYPE>
	TYPE Clamp(const TYPE& value, const TYPE& lower, const TYPE& upper) {
		return Min(upper, Max(lower, value));
	}
	/// <summary>
	/// 値をループさせる
	/// </summary>
	/// <param name="value">値</param>
	/// <param name="range">範囲</param>
	/// <param name="min">最小</param>
	/// <returns></returns>
	inline float Loop(float value, float range, float min = 0.0f) {
		if (min < value && value < min + range) return value;
		float x = fmodf(value - min, range);
		if (x < 0) {
			x += range;
		}
		return x + min;
	}
	/// <summary>
	/// 絶対値
	/// </summary>
	/// <param name="value">値</param>
	/// <returns></returns>
	inline float Abs(float value) {
		return fabsf(value);
	}
	/// <summary>
	///	サイン
	/// </summary>
	/// <param name="angle">ラジアン</param>
	/// <returns></returns>
	inline float Sin(float angle) {
		return sinf(angle);
	}
	/// <summary>
	/// コサイン
	/// </summary>
	/// <param name="angle">ラジアン</param>
	/// <returns></returns>
	inline float Cos(float angle) {
		return cosf(angle);
	}
	/// <summary>
	/// タンジェント
	/// </summary>
	/// <param name="angle">ラジアン</param>
	/// <returns></returns>
	inline float Tan(float angle) {
		return tanf(angle);
	}
	/// <summary>
	/// アークサイン
	/// </summary>
	/// <param name="angle">ラジアン</param>
	/// <returns></returns>
	inline float Asin(float angle) {
		return asinf(angle);
	}
	/// <summary>
	/// アークコサイン
	/// </summary>
	/// <param name="angle">ラジアン</param>
	/// <returns></returns>
	inline float Acos(float angle) {
		return acosf(angle);
	}
	/// <summary>
	/// アークタンジェント
	/// </summary>
	/// <param name="y">y成分</param>
	/// <param name="x">x成分</param>
	/// <returns>ラジアン</returns>
	inline float Atan2(float y, float x) {
		return atan2f(y, x);
	}
	/// <summary>
	/// 線形補間
	/// </summary>
	/// <param name="t">変化量</param>
	/// <param name="start">スタート</param>
	/// <param name="end">エンド</param>
	/// <returns></returns>
	inline float Lerp(float t, float start, float end) {
		return (start + t * (end - start));
	}
	/// <summary>
	/// 現在範囲から別の範囲に変換
	/// </summary>
	/// <param name="value">値</param>
	/// <param name="start1">現在範囲の最小</param>
	/// <param name="end1">現在範囲の最大</param>
	/// <param name="start2">変換範囲の最小</param>
	/// <param name="end2">変換範囲の最大</param>
	/// <returns></returns>
	inline float Map(float value, float start1, float end1, float start2, float end2) {
		return (start2 + (end1 - start2) * ((value - start1) / (end2 - start1)));
	}
	/// <summary>
	/// 平方根
	/// </summary>
	/// <param name="value">値</param>
	/// <returns></returns>
	inline float Sqrt(float value) {
		return sqrtf(value);
	}
	/// <summary>
	/// float剰余演算
	/// </summary>
	/// <param name="number">割られる数</param>
	/// <param name="denom">割る数</param>
	/// <returns>余り</returns>
	inline float Fmod(float number, float denom) {
		return fmodf(number, denom);
	}
	/// <summary>
	/// 冪乗
	/// </summary>
	/// <param name="base">底</param>
	/// <param name="exponent">指数</param>
	/// <returns></returns>
	inline float Pow(float base, float exponent) {
		return powf(base, exponent);
	}

};
/// <summary>
/// 二次元ベクトル
/// </summary>
struct Vector2 {
	static const Vector2 UnitX;
	static const Vector2 UnitY;
	static const Vector2 Zero;


	float x; // x成分
	float y; // y成分


	inline Vector2() : x(0.0f), y(0.0f) {}
	inline explicit Vector2(float x, float y) : x(x), y(y) {}
	inline explicit Vector2(float xy) : x(xy), y(xy) {}
	
#pragma region 演算子のオーバーロード
	inline float& operator[](size_t i) {
		return *((&x) + i);
	}

	inline Vector2 operator+() const {
		return Vector2(x, y);
	}
	inline Vector2 operator-() const {
		return Vector2(-x, -y);
	}
	friend inline Vector2 operator+(const Vector2& v1, const Vector2& v2) {
		return Vector2(v1.x + v2.x, v1.y + v2.y);
	}
	friend inline Vector2 operator-(const Vector2& v1, const Vector2& v2) {
		return Vector2(v1.x - v2.x, v1.y - v2.y);
	}
	friend inline Vector2 operator*(const Vector2& v1, const Vector2& v2) {
		return Vector2(v1.x * v2.x, v1.y * v2.y);
	}
	friend inline Vector2 operator*(const Vector2& v, float s) {
		return Vector2(v.x * s, v.y * s);
	}
	friend inline Vector2 operator*(float s, const Vector2& v) {
		return Vector2(s * v.x, s * v.y);
	}
	friend inline Vector2 operator/(const Vector2& v, float s) {
		return Vector2(v.x / s, v.y / s);
	}
	
	inline Vector2& operator+=(const Vector2& v) {
		x += v.x;
		y += v.y;
		return *this;
	}
	inline Vector2& operator-=(const Vector2& v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}
	inline Vector2& operator*=(const Vector2& v) {
		x *= v.x;
		y *= v.y;
		return *this;
	}
	inline Vector2& operator*=(float s) {
		x *= s;
		y *= s;
		return *this;
	}
	inline Vector2& operator/=(const Vector2& v) {
		x /= v.x;
		y /= v.y;
		return *this;
	}
	friend inline bool operator==(const Vector2& v1, const Vector2& v2) {
		return v1.x == v2.x && v1.y == v2.y;
	}
	friend inline bool operator!=(const Vector2& v1, const Vector2& v2) {
		return v1.x != v2.x || v1.y != v2.y;
	}
#pragma endregion

	/// <summary>
	/// 長さの二乗
	/// </summary>
	/// <returns></returns>
	inline float LengthSquare() const {
		return (x * x + y * y);
	}
	/// <summary>
	/// 長さ
	/// </summary>
	/// <returns></returns>
	inline float Length() const {
		return (Math::Sqrt(LengthSquare()));
	}
	/// <summary>
	/// 正規化
	/// </summary>
	/// <returns></returns>
	inline Vector2 Normalized() const {
		return (*this / Length());
	}
	/// <summary>
	/// 回転したベクトル
	/// </summary>
	/// <param name="angle">回転させる角度</param>
	/// <returns></returns>
	inline Vector2 Rotated(float angle) const {
		float s = Math::Sin(angle);
		float c = Math::Cos(angle);
		return Vector2(x * c - y * s, x * s + y * c );
	}
	/// <summary>
	/// ゼロベクトルか
	/// </summary>
	/// <returns></returns>
	inline bool IsZero() const {
		return x == 0.0f && y == 0.0f;
	}

// 静的関数

	/// <summary>
	/// 内積 
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	friend inline float Dot(const Vector2& v1, const Vector2& v2) {
		return (v1.x * v2.x + v1.y * v2.y);
	}
	/// <summary>
	/// 外積
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	friend inline float Cross(const Vector2& v1, const Vector2& v2) {
		return (v1.x * v2.y - v1.y * v2.x);
	}
	/// <summary>
	/// 線形補間
	/// </summary>
	/// <param name="t">0 ~ 1</param>
	/// <param name="start">0の時</param>
	/// <param name="end">1の時</param>
	/// <returns></returns>
	friend inline Vector2 Lerp(float t, const Vector2& start, const Vector2& end) {
		return (start + t * (end - start));
	}
	/// <summary>
	/// 他ベクトルとの距離
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	friend inline float Distance(const Vector2& v1, const Vector2& v2) {
		return (v2 - v1).Length();
	}
	/// <summary>
	/// 中点
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	friend inline Vector2 Mid(const Vector2& v1, const Vector2& v2) {
		return ((v1 + v2) * 0.5f);
	}
	/// <summary>
	/// 反射したベクトル
	/// </summary>
	/// <param name="v">ベクトル</param>
	/// <param name="n">正規化された壁の法線</param>
	/// <returns></returns>
	friend inline Vector2 Reflected(const Vector2& v, const Vector2& n) {
		return (Dot(n, -v) * 2.0f * n + v);
	}
};
/// <summary>
/// 三次元ベクトル
/// </summary>
struct Vector3 {
	static const Vector3 UnitX;
	static const Vector3 UnitY;
	static const Vector3 UnitZ;
	static const Vector3 Zero;


	float x; // x成分
	float y; // y成分
	float z; // z成分


	inline Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
	inline explicit Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
	inline explicit Vector3(float xyz) : x(xyz), y(xyz), z(xyz) {}
	inline explicit Vector3(const Vector2& xy, float z) : x(xy.x), y(xy.y), z(z) {}

	inline void xy(const Vector2& xy) {
		x = xy.x;
		y = xy.y;
	}
	inline Vector2 xy() const { 
		return Vector2(x, y); 
	}
	inline void xz(const Vector2& xz) {
		x = xz.x;
		z = xz.y;
	}
	inline Vector2 xz() const { 
		return Vector2(x, z); 
	}
	inline void yz(const Vector2& yz) {
		y = yz.x;
		z = yz.y;
	}
	inline Vector2 yz() const { 
		return Vector2(y, z); 
	}

#pragma region 演算子のオーバーロード
	inline float& operator[](size_t i) {
		return *((&x) + i);
	}
	inline Vector3 operator+() const {
		return Vector3(x, y, z);
	}
	inline Vector3 operator-() const {
		return Vector3(-x, -y, -z);
	}
	friend inline Vector3 operator+(const Vector3& v1, const Vector3& v2) {
		return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
	}
	friend inline Vector3 operator-(const Vector3& v1, const Vector3& v2) {
		return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z + v2.z);
	}
	friend inline Vector3 operator*(const Vector3& v1, const Vector3& v2) {
		return Vector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
	}
	friend inline Vector3 operator*(const Vector3& v, float s) {
		return Vector3(v.x * s, v.y * s, v.z * s);
	}
	friend inline Vector3 operator*(float s, const Vector3& v) {
		return Vector3(s * v.x, s * v.y, s * v.z);
	}
	friend inline Vector3 operator/(const Vector3& v, float s) {
		return Vector3(v.x / s, v.y / s, v.z / s);
	}

	inline Vector3& operator+=(const Vector3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	inline Vector3& operator-=(const Vector3& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	inline Vector3& operator*=(const Vector3& v) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}
	inline Vector3& operator*=(float s) {
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}
	inline Vector3& operator/=(const Vector3& v) {
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}
	friend inline bool operator==(const Vector3& v1, const Vector3& v2) {
		return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
	}
	friend inline bool operator!=(const Vector3& v1, const Vector3& v2) {
		return v1.x != v2.x || v1.y != v2.y || v1.z != v2.z;
	}
#pragma endregion

	/// <summary>
	/// 長さの二乗
	/// </summary>
	/// <returns></returns>
	inline float LengthSquare() const {
		return (x * x + y * y + z * z);
	}
	/// <summary>
	/// 長さ
	/// </summary>
	/// <returns></returns>
	inline float Length() const {
		return (Math::Sqrt(LengthSquare()));
	}
	/// <summary>
	/// 正規化
	/// </summary>
	/// <returns></returns>
	inline Vector3 Normalized() const {
		return (*this / Length());
	}
	/// <summary>
	/// ゼロベクトルか
	/// </summary>
	/// <returns></returns>
	inline bool IsZero() const {
		return x == 0.0f && y == 0.0f && z == 0.0f;
	}

	// 静的関数

	/// <summary>
	/// 内積 
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	friend inline float Dot(const Vector3& v1, const Vector3& v2) {
		return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
	}
	/// <summary>
	/// 外積
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	friend inline Vector3 Cross(const Vector3& v1, const Vector3& v2) {
		return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x - v2.z, v1.x * v2.y - v1.y * v2.x);
	}
	/// <summary>
	/// 線形補間
	/// </summary>
	/// <param name="t">0 ~ 1</param>
	/// <param name="start">0の時</param>
	/// <param name="end">1の時</param>
	/// <returns></returns>
	friend inline Vector3 Lerp(float t, const Vector3& start, const Vector3& end) {
		return (start + t * (end - start));
	}
	/// <summary>
	/// 他ベクトルとの距離
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	friend inline float Distance(const Vector3& v1, const Vector3& v2) {
		return (v2 - v1).Length();
	}
	/// <summary>
	/// 中点
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	friend inline Vector3 Mid(const Vector3& v1, const Vector3& v2) {
		return ((v1 + v2) * 0.5f);
	}
	/// <summary>
	/// 反射したベクトル
	/// </summary>
	/// <param name="v">ベクトル</param>
	/// <param name="n">正規化された壁の法線</param>
	/// <returns></returns>
	friend inline Vector3 Reflected(const Vector3& v, const Vector3& n) {
		return (Dot(n, -v) * 2.0f * n + v);
	}
};
/// <summary>
/// 四次元ベクトル
/// </summary>
struct Vector4 {
	static const Vector4 Zero;


	float x;
	float y;
	float z;
	float w;


	inline Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	inline explicit Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	inline explicit Vector4(float xyzw) : x(xyzw), y(xyzw), z(xyzw), w(xyzw) {}
	inline explicit Vector4(const Vector3& xyz, float w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}

	inline void xyz(const Vector3& xyz) {
		x = xyz.x;
		y = xyz.y;
		z = xyz.z;
	}
	inline Vector3 xyz() const {
		return Vector3(x, y, z);
	}

#pragma region 演算子のオーバーロード
	inline float& operator[](size_t i) {
		return *((&x) + i);
	}
	inline Vector4 operator+() const {
		return Vector4(x, y, z, w);
	}
	inline Vector4 operator-() const {
		return Vector4(-x, -y, -z, -w);
	}
	friend inline Vector4 operator+(const Vector4& v1, const Vector4& v2) {
		return Vector4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
	}
	friend inline Vector4 operator-(const Vector4& v1, const Vector4& v2) {
		return Vector4(v1.x - v2.x, v1.y - v2.y, v1.z + v2.z, v1.w - v2.w);
	}
	friend inline Vector4 operator*(const Vector4& v, float s) {
		return Vector4(v.x * s, v.y * s, v.z * s, v.w * s);
	}
	friend inline Vector4 operator*(float s, const Vector4& v) {
		return Vector4(s * v.x, s * v.y, s * v.z, s * v.w);
	}
	friend inline Vector4 operator/(const Vector4& v, float s) {
		return Vector4(v.x / s, v.y / s, v.z / s, v.w / s);
	}

	inline Vector4& operator+=(const Vector4& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}
	inline Vector4& operator-=(const Vector4& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}
	inline Vector4& operator*=(const Vector4& v) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
		return *this;
	}
	inline Vector4& operator*=(float s) {
		x *= s;
		y *= s;
		z *= s;
		w *= s;
		return *this;
	}
	inline Vector4& operator/=(const Vector4& v) {
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;
		return *this;
	}
	friend inline bool operator==(const Vector4& v1, const Vector4& v2) {
		return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w;
	}
	friend inline bool operator!=(const Vector4& v1, const Vector4& v2) {
		return v1.x != v2.x || v1.y != v2.y || v1.z != v2.z && v1.w != v2.w;
	}
#pragma endregion

	/// <summary>
	/// 長さの二乗
	/// </summary>
	/// <returns></returns>
	inline float LengthSquare() const {
		return (x * x + y * y + z * z + w * w);
	}
	/// <summary>
	/// 長さ
	/// </summary>
	/// <returns></returns>
	inline float Length() const {
		return (Math::Sqrt(LengthSquare()));
	}
	/// <summary>
	/// 正規化
	/// </summary>
	/// <returns></returns>
	inline Vector4 Normalized() const {
		return (*this / Length());
	}
	/// <summary>
	/// ゼロベクトルか
	/// </summary>
	/// <returns></returns>
	inline bool IsZero() const {
		return x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f;
	}

	// 静的関数

	/// <summary>
	/// 内積 
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	friend inline float Dot(const Vector4& v1, const Vector4& v2) {
		return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w + v2.w);
	}
	/// <summary>
	/// 線形補間
	/// </summary>
	/// <param name="t">0 ~ 1</param>
	/// <param name="start">0の時</param>
	/// <param name="end">1の時</param>
	/// <returns></returns>
	friend inline Vector4 Lerp(float t, const Vector4& start, const Vector4& end) {
		return (start + t * (end - start));
	}
};
/// <summary>
/// クォータニオン
/// </summary>
struct Quaternion {
	static const Quaternion Identity;
	

	float x;
	float y;
	float z;
	float w;


	inline Quaternion(){
		*this = Identity;
	}
	inline explicit Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}


	inline void xyz(const Vector3& xyz) {
		x = xyz.x;
		y = xyz.y;
		z = xyz.z;
	}
	inline Vector3 xyz() const {
		return Vector3(x, y, z);
	}
	/// <summary>
	/// 軸周りの角度を取得
	/// </summary>
	/// <returns></returns>
	inline float GetAngle() const {
		return Math::Acos(w) * 2.0f;
	}
	/// <summary>
	/// 回転軸を取得
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetAxis() const {
		return xyz() / Math::Sin(Math::Acos(w));
	}
	/// <summary>
	/// オイラー角を取得
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetEuler() const {
		Vector3 result;
		result.x = asinf(2.0f * y * z + 2.0f * x * w);
		if (result.x != 0.0f) {
			result.y = atanf(-((2.0f * x * z + -2.0f * y * w) / (2.0f * w * w + 2.0f * z * z - 1)));
			result.z = atanf(-((2.0f * x * y + -2.0f * z * w) / (2.0f * w * w + 2.0f * y * y - 1)));
			return result;
		}
		result.y = 0.0f;
		result.z = atanf((2.0f * x * y + 2.0f * z * w) / (2.0f * w * w + 2.0f * x * x - 1));
		return result;
	}

#pragma region 演算子のオーバーロード
	friend inline Quaternion operator+(const Quaternion& q1, const Quaternion& q2) {
		return Quaternion(q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w);
	}
	friend inline Quaternion operator*(float s, const Quaternion& q) {
		return Quaternion(s * q.x, s * q.y, s * q.z, s * q.w);
	}
	friend inline Quaternion operator*(const Quaternion& q, float s) {
		return Quaternion(q.x * s, q.y * s, q.z * s, q.w * s);
	}
	friend inline Quaternion operator/(const Quaternion& q, float s) {
		return Quaternion(q.x / s, q.y / s, q.z / s, q.w / s);
	}
	friend inline Quaternion operator*(const Quaternion& q1, const Quaternion& q2) {
		Vector3 qv = q1.xyz();
		Vector3 pv = q2.xyz();
		Vector3 v = q1.w * pv + q2.w * qv + Cross(qv, pv);
		float w = q1.w * q2.w - Dot(qv, pv);
		return Quaternion(v.x, v.y, v.z, w);
	}
	friend inline Vector3 operator*(const Quaternion& q, const Vector3& v) {
		Vector3 qv = q.xyz();
		return (v + 2.0f * Cross(qv, Cross(qv, v) + q.w * v));
	}

#pragma endregion
	/// <summary>
	/// 長さの二乗
	/// </summary>
	/// <returns></returns>
	inline float LengthSquare() const {
		return (x * x + y * y + z * z + w * w);
	}
	/// <summary>
	/// 長さ
	/// </summary>
	/// <returns></returns>
	inline float Length() const {
		return sqrtf(LengthSquare());
	}
	/// <summary>
	/// 正規化
	/// </summary>
	inline Quaternion Normalized() const {
		return *this / Length();
	}
	/// <summary>
	/// 共役
	/// </summary>
	/// <returns></returns>
	inline Quaternion Conjugate() const {
		return Quaternion(-x, -y, -z, w);
	}
	/// <summary>
	/// 逆
	/// </summary>
	/// <returns></returns>
	inline Quaternion Inverse() const {
		return Conjugate() / LengthSquare();
	}
	/// <summary>
	/// 内積
	/// </summary>
	friend inline float Dot(const Quaternion& q1, const Quaternion& p2) {
		return (q1.x * p2.x + q1.y * p2.y + q1.z * p2.z + q1.w * p2.w);
	}
	/// <summary>
	/// 球面線形補間
	/// </summary>
	/// <param name="t">0 ~ 1</param>
	/// <param name="start">0の時</param>
	/// <param name="end">1の時</param>
	/// <returns></returns>
	friend inline Quaternion Slerp(float t, const Quaternion& start, const Quaternion& end) {
		Quaternion s = start;
		float dot_val = Dot(start, end);
		// q1, q2が反対向きの場合
		if (dot_val < 0) {
			s.w = -s.w;
			s.x = -s.x;
			s.y = -s.y;
			s.z = -s.z;
			dot_val = -dot_val;
		}
		// 球面線形補間の計算
		float theta = Math::Acos(dot_val);
		return (Math::Sin((1 - t) * theta) * s + Math::Sin(t * theta) * end) / Math::Sin(theta);
	}

	/// <summary>
	/// 回転角と回転軸からクォータニオンを作成
	/// </summary>
	/// <param name="angle"></param>
	/// <param name="axis"></param>
	/// <returns></returns>
	static inline Quaternion CreateFromAngleAxis(float angle, const Vector3& axis) {
		float sin2 = Math::Sin(angle * 0.5f);
		Quaternion result;
		result.x = axis.x * sin2;
		result.y = axis.y * sin2;
		result.z = axis.z * sin2;
		result.w = Math::Cos(angle * 0.5f);
		return result;
	}
	/// <summary>
	/// オイラー角からクォータニオンを作成
	/// </summary>
	/// /// <param name="euler"></param>
	/// <returns></returns>
	static inline Quaternion CreateFromEuler(const Vector3& euler) {
		// ピッチ ヨー ロールの順
		Vector3 s = Vector3(Math::Sin(euler.x / 2.0f), Math::Sin(euler.y / 2.0f), Math::Sin(euler.z / 2.0f));
		Vector3 c = Vector3(Math::Cos(euler.x / 2.0f), Math::Cos(euler.y / 2.0f), Math::Cos(euler.z / 2.0f));
		Quaternion result;
		result.x = -c.x * s.y * s.z + s.x * c.y * c.z;
		result.y = c.x * s.y * c.z + s.x * c.y * s.z;
		result.z = s.x * s.y * c.z + c.x * c.y * s.z;
		result.w = -s.x * s.y * s.z + c.x * c.y * c.z;
		return result;
	}
	/// <summary>
	/// ピッチ角からクォータニオンを作成
	/// </summary>
	/// <param name="angle"></param>
	/// <returns></returns>
	static inline Quaternion CreateFromPitch(float angle) {
		return Quaternion(Math::Sin(angle / 2.0f), 0.0f, 0.0f, Math::Cos(angle / 2.0f));
	}
	/// <summary>
	/// ヨー角からクォータニオンを作成
	/// </summary>
	/// <param name="angle"></param>
	/// <returns></returns>
	static inline Quaternion CreateFromYaw(float angle) {
		return Quaternion(0.0f, Math::Sin(angle / 2.0f), 0.0f, Math::Cos(angle / 2.0f));
	}
	/// <summary>
	/// ロール角からクォータニオンを作成
	/// </summary>
	/// <param name="angle"></param>
	/// <returns></returns>
	static inline Quaternion CreateFromRoll(float angle) {
		return Quaternion(0.0f, 0.0f, Math::Sin(angle / 2.0f), Math::Cos(angle / 2.0f));
	}
	/// <summary>
	/// 二つのベクトルからクォータニオンを作成
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	static inline Quaternion CreateFromTwoVector(const Vector3& v1, const Vector3& v2) {
		Vector3 axis = Cross(v1, v2).Normalized();
		float angle = Math::Acos(Dot(v1, v2));
		return CreateFromAngleAxis(angle, axis);
	}
};
/// <summary>
/// 3×3行列
/// </summary>
struct Matrix33 {
	static const Matrix33 Identity;

	
	float m[3][3];


	inline Matrix33() {
		*this = Identity;
	}
	inline explicit Matrix33(
		float _00, float _01, float _02,
		float _10, float _11, float _12,
		float _20, float _21, float _22) {
		m[0][0] = _00, m[0][1] = _01, m[0][2] = _02;
		m[1][0] = _10, m[1][1] = _11, m[1][2] = _12;
		m[2][0] = _20, m[2][1] = _21, m[2][2] = _22;
	}
	/// <summary>
	/// x軸
	/// </summary>
	/// <returns></returns>
	inline Vector2 GetXAxis() const { return Vector2(m[0][0], m[0][1]); }
	/// <summary>
	/// y軸
	/// </summary>
	/// <returns></returns>
	inline Vector2 GetYAxis() const { return Vector2(m[1][0], m[1][1]); }
	/// <summary>
	/// 平行移動成分
	/// </summary>
	/// <returns></returns>
	inline Vector2 GetTranslation() const { return Vector2(m[2][0], m[2][1]); }

#pragma region 演算子のオーバーロード
	inline float* operator[](size_t i) {
		return m[i];
	}
	inline const float* operator[](size_t i) const {
		return m[i];
	}
	friend inline Matrix33 operator*(const Matrix33& m1, const Matrix33& m2) {
		return Matrix33(
			m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0],
			m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1],
			m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2],

			m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0],
			m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1],
			m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2],

			m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0],
			m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1],
			m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2]);
	}
	inline Matrix33& operator*=(const Matrix33& m) {
		*this = *this * m;
		return *this;
	}
	friend inline Vector2 operator*(const Vector2& v, const Matrix33& m) {
		return Vector2(
			v.x * m.m[0][0] + v.y * m.m[1][0] + 1.0f * m.m[2][0],
			v.x * m.m[0][1] + v.y * m.m[1][1] + 1.0f * m.m[2][1]);
	}
	friend inline Matrix33 operator*(float s, const Matrix33& m) {
		return Matrix33(
			s * m.m[0][0], s * m.m[0][1], s * m.m[0][2],
			s * m.m[1][0], s * m.m[1][1], s * m.m[1][2],
			s * m.m[2][0], s * m.m[2][1], s * m.m[2][2]);
	}
	friend inline Matrix33 operator*(const Matrix33& m, float s) {
		return Matrix33(
			m.m[0][0] * s, m.m[0][1] * s, m.m[0][2] * s,
			m.m[1][0] * s, m.m[1][1] * s, m.m[1][2] * s,
			m.m[2][0] * s, m.m[2][1] * s, m.m[2][2] * s);
	}
#pragma endregion
	/// <summary>
	/// 行列式
	/// </summary>
	/// <returns></returns>
	inline float Determinant() const {
		return m[0][0] * m[1][1] * m[2][2] + m[0][1] * m[1][2] * m[2][0] + m[0][2] * m[1][0] * m[2][1]
			- m[0][2] * m[1][1] * m[2][0] - m[0][1] * m[1][0] * m[2][2] - m[0][0] * m[1][2] * m[2][1];
	}
	/// <summary>
	/// 余因子行列
	/// </summary>
	/// <returns></returns>
	inline Matrix33 Adjugate() const {
		return Matrix33(
			  m[1][1] * m[2][2] - m[1][2] * m[2][1],
			-(m[0][1] * m[2][2] - m[0][2] * m[2][1]),
			  m[0][1] * m[1][2] - m[0][2] * m[1][1],

			-(m[1][0] * m[2][2] - m[1][2] * m[2][0]),
			  m[0][0] * m[2][2] - m[0][2] * m[2][0],
			-(m[0][0] * m[1][2] - m[0][2] * m[1][0]),

			  m[1][0] * m[2][1] - m[1][1] * m[2][0],
			-(m[0][0] * m[2][1] - m[0][1] * m[2][0]),
			  m[0][0] * m[1][1] - m[0][1] * m[1][0]);
	}
	/// <summary>
	/// 逆行列
	/// </summary>
	/// <returns></returns>
	inline Matrix33 Inverse() const {
		float det = Determinant();
		assert(det != 0.0f); // 0の場合逆行列がない
		return 1.0f / det * Adjugate();
	}
	/// <summary>
	/// 転置行列
	/// </summary>
	/// <returns></returns>
	inline Matrix33 Transpose() const {
		return Matrix33(
			m[0][0], m[1][0], m[2][0],
			m[0][1], m[1][1], m[2][1],
			m[0][2], m[1][2], m[2][2]);
	}

	/// <summary>
	/// 拡大縮小行列を作成
	/// </summary>
	/// <param name="scale"></param>
	/// <returns></returns>
	static inline Matrix33 CreateScaling(const Vector2& scale) {
		return Matrix33(
			scale.x,	0.0f,		0.0f,
			0.0f,		scale.y,	0.0f,
			0.0f,		0.0f,		1.0f);
	}
	/// <summary>
	/// 回転行列を作成
	/// </summary>
	/// <param name="rotate"></param>
	/// <returns></returns>
	static inline Matrix33 CreateRotation(float rotate) {
		float s = Math::Sin(rotate);
		float c = Math::Cos(rotate);
		return Matrix33(
			c,		s,		0.0f,
			-s,		c,		0.0f,
			0.0f,	0.0f,	1.0f);
	}
	/// <summary>
	/// 平行移動行列を作成
	/// </summary>
	/// <param name="vector"></param>
	/// <returns></returns>
	static inline Matrix33 CreateTranslation(const Vector2& vector) {
		return Matrix33(
			1.0f,		0.0f,		0.0f,
			0.0f,		1.0f,		0.0f,
			vector.x,	vector.y,	1.0f);
	}
	/// <summary>
	/// アフィン変換行列を作成する(SRT)
	/// </summary>
	/// <param name="scale"></param>
	/// <param name="rotate"></param>
	/// <param name="vector"></param>
	/// <returns></returns>
	static inline Matrix33 CreateAffineTransform(const Vector2& scale, float rotate, const Vector2& vector) {
		Matrix33 result;
		result *= CreateScaling(scale);
		result *= CreateRotation(rotate);
		result *= CreateTranslation(vector);
		return result;
	}
	/// <summary>
	/// 平行投影行列を作成する
	/// </summary>
	/// <param name="left"></param>
	/// <param name="top"></param>
	/// <param name="right"></param>
	/// <param name="bottom"></param>
	/// <returns></returns>
	static inline Matrix33 CreateOrthoProjection(float left, float top, float right, float bottom) {
		return Matrix33(
			2.0f / (right - left) ,					0.0f,								0.0f,
			0.0f,									2.0f / (top - bottom),				0.0f,
			(left + right) / (left - right),		(top + bottom) / (bottom - top),	1.0f );

	}
	/// <summary>
	/// ビューポート行列を作成する
	/// </summary>
	/// <param name="left"></param>
	/// <param name="top"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <returns></returns>
	static inline Matrix33 CreateViewport(float left, float top, float width, float height) {
		float halfW = width / 2.0f;
		float halfh = height / 2.0f;
		return Matrix33(
			halfW,			0.0f,			0.0f,
			0.0f,			-halfh,			0.0f,
			left + halfW,	top + halfh,	1.0f);
	}
	/// <summary>
	/// 44行列から33行列を作成
	/// </summary>
	/// <param name="mat"></param>
	/// <returns></returns>
	static Matrix33 CreateFromMatrix44(const Matrix44& mat);
};
/// <summary>
/// 4×4行列
/// </summary>
struct Matrix44 {
	static const Matrix44 Identity;


	float m[4][4];


	inline Matrix44() {
		*this = Identity;
	}
	inline explicit Matrix44(
		float _00, float _01, float _02, float _03,
		float _10, float _11, float _12, float _13,
		float _20, float _21, float _22, float _23,
		float _30, float _31, float _32, float _33) {
		m[0][0] = _00, m[0][1] = _01, m[0][2] = _02, m[0][3] = _03;
		m[1][0] = _10, m[1][1] = _11, m[1][2] = _12, m[1][3] = _13;
		m[2][0] = _20, m[2][1] = _21, m[2][2] = _22, m[2][3] = _23;
		m[3][0] = _30, m[3][1] = _31, m[3][2] = _32, m[3][3] = _33;
	}

	/// <summary>
	/// x軸
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetXAxis() const { return Vector3(m[0][0], m[0][1], m[0][2]); }
	/// <summary>
	/// y軸
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetYAxis() const { return Vector3(m[1][0], m[1][1], m[1][2]); }
	/// <summary>
	/// z軸
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetZAxis() const { return Vector3(m[2][0], m[2][1], m[2][2]); }
	/// <summary>
	/// 平行移動成分
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetTranslation() const { return Vector3(m[3][0], m[3][1], m[3][2]); }

#pragma region 演算子のオーバーロード
	inline float* operator[](size_t i) {
		return m[i];
	}
	inline const float* operator[](size_t i) const {
		return m[i];
	}
	friend inline Matrix44 operator*(const Matrix44& m1, const Matrix44& m2) {
		return Matrix44(
			m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0],
			m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1],
			m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2],
			m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3],

			m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0],
			m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1],
			m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2],
			m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3],

			m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0],
			m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1],
			m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2],
			m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3],

			m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0],
			m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1],
			m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2],
			m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3]);
	}
	inline Matrix44& operator*=(const Matrix44& m) {
		*this = *this * m;
		return *this;
	}
	friend inline Matrix44 operator*(float s, const Matrix44& m) {
		return Matrix44(
			s * m.m[0][0], s * m.m[0][1], s * m.m[0][2], s * m.m[0][3],
			s * m.m[1][0], s * m.m[1][1], s * m.m[1][2], s * m.m[1][3],
			s * m.m[2][0], s * m.m[2][1], s * m.m[2][2], s * m.m[2][3],
			s * m.m[3][0], s * m.m[3][1], s * m.m[3][2], s * m.m[3][3]);
	}
	friend inline Matrix44 operator*(const Matrix44& m, float s) {
		return Matrix44(
			m.m[0][0] * s, m.m[0][1] * s, m.m[0][2] * s, m.m[0][3] * s,
			m.m[1][0] * s, m.m[1][1] * s, m.m[1][2] * s, m.m[1][3] * s,
			m.m[2][0] * s, m.m[2][1] * s, m.m[2][2] * s, m.m[2][3] * s,
			m.m[3][0] * s, m.m[3][1] * s, m.m[3][2] * s, m.m[3][3] * s);
	}
	friend inline Vector3 operator*(const Vector3& v, const Matrix44& m) {
		return Vector3(
			v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0],
			v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1],
			v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]);
	}
	friend inline Vector4 operator*(const Vector4& v, const Matrix44& m) {
		return Vector4(
			v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + v.w * m.m[3][0],
			v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + v.w * m.m[3][1],
			v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + v.w * m.m[3][2],
			v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + v.w * m.m[3][3]);
	}
#pragma endregion
	/// <summary>
	/// 行列式
	/// </summary>
	/// <returns></returns>
	inline float Determinant() const {
		float result = 0.0f;

		result += m[0][0] * m[1][1] * m[2][2] * m[3][3]; // +11,22,33,44
		result += m[0][0] * m[1][2] * m[2][3] * m[3][1]; // +11,23,34,42
		result += m[0][0] * m[1][3] * m[2][1] * m[3][2]; // +11,24,32,43

		result -= m[0][0] * m[1][3] * m[2][2] * m[3][1]; // -11,24,33,42
		result -= m[0][0] * m[1][2] * m[2][1] * m[3][3]; // -11,23,32,44
		result -= m[0][0] * m[1][1] * m[2][3] * m[3][2]; // -11,22,34,43

		result -= m[0][1] * m[1][0] * m[2][2] * m[3][3]; // -12,21,33,44
		result -= m[0][2] * m[1][0] * m[2][3] * m[3][1]; // -13,21,34,42
		result -= m[0][3] * m[1][0] * m[2][1] * m[3][2]; // -14,21,32,43

		result += m[0][3] * m[1][0] * m[2][2] * m[3][1]; // +14,21,33,42
		result += m[0][2] * m[1][0] * m[2][1] * m[3][3]; // +13,21,32,44
		result += m[0][1] * m[1][0] * m[2][3] * m[3][2]; // +12,21,34,43

		result += m[0][1] * m[1][2] * m[2][0] * m[3][3]; // +12,23,31,44
		result += m[0][2] * m[1][3] * m[2][0] * m[3][1]; // +13,24,31,42
		result += m[0][3] * m[1][1] * m[2][0] * m[3][2]; // +14,22,31,43

		result -= m[0][3] * m[1][2] * m[2][0] * m[3][1]; // -14,23,31,42
		result -= m[0][2] * m[1][1] * m[2][0] * m[3][3]; // -13,22,31,44
		result -= m[0][1] * m[1][3] * m[2][0] * m[3][2]; // -12,24,31,43

		result -= m[0][1] * m[1][2] * m[2][3] * m[3][0]; // -12,23,34,41
		result -= m[0][2] * m[1][3] * m[2][1] * m[3][0]; // -13,24,32,41
		result -= m[0][3] * m[1][1] * m[2][2] * m[3][0]; // -14,22,33,41

		result += m[0][3] * m[1][2] * m[2][1] * m[3][0]; // +14,23,32,41
		result += m[0][2] * m[1][1] * m[2][3] * m[3][0]; // +13,22,34,41
		result += m[0][1] * m[1][3] * m[2][2] * m[3][0]; // +12,24,33,41

		return result;
	}
	/// <summary>
	/// 余因子行列
	/// </summary>
	/// <returns></returns>
	inline Matrix44 Adjugate() const {
		Matrix44 result;
		// 1行目
		result.m[0][0] = 0.0f;							// 11
		result.m[0][0] += m[1][1] * m[2][2] * m[3][3];	// +22,33,44
		result.m[0][0] += m[1][2] * m[2][3] * m[3][1];	// +23,34,42
		result.m[0][0] += m[1][3] * m[2][1] * m[3][2];	// +24,32,43
		result.m[0][0] -= m[1][3] * m[2][2] * m[3][1];	// -24,33,42
		result.m[0][0] -= m[1][2] * m[2][1] * m[3][3];	// -23,32,44
		result.m[0][0] -= m[1][1] * m[2][3] * m[3][2];	// -22,34,43

		result.m[0][1] = 0.0f;							// 12
		result.m[0][1] -= m[0][1] * m[2][2] * m[3][3];	// -12,33,44
		result.m[0][1] -= m[0][2] * m[2][3] * m[3][1];	// -13,34,42
		result.m[0][1] -= m[0][3] * m[2][1] * m[3][2];	// -14,32,43
		result.m[0][1] += m[0][3] * m[2][2] * m[3][1];	// +14,33,42
		result.m[0][1] += m[0][2] * m[2][1] * m[3][3];	// +13,32,44
		result.m[0][1] += m[0][1] * m[2][3] * m[3][2];	// +12,34,43

		result.m[0][2] = 0.0f;							// 13
		result.m[0][2] += m[0][1] * m[1][2] * m[3][3];	// +12,23,44
		result.m[0][2] += m[0][2] * m[1][3] * m[3][1];	// +13,24,42
		result.m[0][2] += m[0][3] * m[1][1] * m[3][2];	// +14,22,43
		result.m[0][2] -= m[0][3] * m[1][2] * m[3][1];	// -14,23,42
		result.m[0][2] -= m[0][2] * m[1][1] * m[3][3];	// -13,22,44
		result.m[0][2] -= m[0][1] * m[1][3] * m[3][2];	// -12,24,43

		result.m[0][3] = 0.0f;							// 14
		result.m[0][3] -= m[0][1] * m[1][2] * m[2][3];	// -12,23,34
		result.m[0][3] -= m[0][2] * m[1][3] * m[2][1];	// -13,24,32
		result.m[0][3] -= m[0][3] * m[1][1] * m[2][2];	// -14,22,33
		result.m[0][3] += m[0][3] * m[1][2] * m[2][1];	// +14,23,32
		result.m[0][3] += m[0][2] * m[1][1] * m[2][3];	// +13,22,34
		result.m[0][3] += m[0][1] * m[1][3] * m[2][2];	// +12,24,33


		// 2行目
		result.m[1][0] = 0.0f;							// 21
		result.m[1][0] -= m[1][0] * m[2][2] * m[3][3];	// -21,33,44
		result.m[1][0] -= m[1][2] * m[2][3] * m[3][0];	// -23,34,41
		result.m[1][0] -= m[1][3] * m[2][0] * m[3][2];	// -24,31,43
		result.m[1][0] += m[1][3] * m[2][2] * m[3][0];	// +24,33,41
		result.m[1][0] += m[1][2] * m[2][0] * m[3][3];	// +23,31,44
		result.m[1][0] += m[1][0] * m[2][3] * m[3][2];	// +21,34,43

		result.m[1][1] = 0.0f;							// 22
		result.m[1][1] += m[0][0] * m[2][2] * m[3][3];	// +11,33,44
		result.m[1][1] += m[0][2] * m[2][3] * m[3][0];	// +13,34,41
		result.m[1][1] += m[0][3] * m[2][0] * m[3][2];	// +14,31,43
		result.m[1][1] -= m[0][3] * m[2][2] * m[3][0];	// -14,33,41
		result.m[1][1] -= m[0][2] * m[2][0] * m[3][3];	// -13,31,44
		result.m[1][1] -= m[0][0] * m[2][3] * m[3][2];	// -11,34,43

		result.m[1][2] = 0.0f;							// 23
		result.m[1][2] -= m[0][0] * m[1][2] * m[3][3];	// -11,23,44
		result.m[1][2] -= m[0][2] * m[1][3] * m[3][0];	// -13,24,41
		result.m[1][2] -= m[0][3] * m[1][0] * m[3][2];	// -14,21,43
		result.m[1][2] += m[0][3] * m[1][2] * m[3][0];	// +14,23,41
		result.m[1][2] += m[0][2] * m[1][0] * m[3][3];	// +13,21,44
		result.m[1][2] += m[0][0] * m[1][3] * m[3][2];	// +11,24,43

		result.m[1][3] = 0.0f;							// 24
		result.m[1][3] += m[0][0] * m[1][2] * m[2][3];	// +11,23,34
		result.m[1][3] += m[0][2] * m[1][3] * m[2][0];	// +13,24,31
		result.m[1][3] += m[0][3] * m[1][0] * m[2][2];	// +14,21,33
		result.m[1][3] -= m[0][3] * m[1][2] * m[2][0];	// -14,23,31
		result.m[1][3] -= m[0][2] * m[1][0] * m[2][3];	// -13,21,34
		result.m[1][3] -= m[0][0] * m[1][3] * m[2][2];	// -11,24,33


		// 3行目
		result.m[2][0] = 0.0f;							// 31
		result.m[2][0] += m[1][0] * m[2][1] * m[3][3];	// +21,32,44
		result.m[2][0] += m[1][1] * m[2][3] * m[3][0];	// +22,34,41
		result.m[2][0] += m[1][3] * m[2][0] * m[3][1];	// +24,31,42
		result.m[2][0] -= m[1][3] * m[2][1] * m[3][0];	// -24,32,41
		result.m[2][0] -= m[1][1] * m[2][0] * m[3][3];	// -22,31,44
		result.m[2][0] -= m[1][0] * m[2][3] * m[3][1];	// -21,34,42

		result.m[2][1] = 0.0f;							// 32
		result.m[2][1] -= m[0][0] * m[2][1] * m[3][3];	// -11,32,44
		result.m[2][1] -= m[0][1] * m[2][3] * m[3][0];	// -12,34,41
		result.m[2][1] -= m[0][3] * m[2][0] * m[3][1];	// -14,31,42
		result.m[2][1] += m[0][3] * m[2][1] * m[3][0];	// +14,32,41
		result.m[2][1] += m[0][1] * m[2][0] * m[3][3];	// +12,31,44
		result.m[2][1] += m[0][0] * m[2][3] * m[3][1];	// +11,34,42

		result.m[2][2] = 0.0f;							// 33
		result.m[2][2] += m[0][0] * m[1][1] * m[3][3];	// +11,22,44
		result.m[2][2] += m[0][1] * m[1][3] * m[3][0];	// +12,24,41
		result.m[2][2] += m[0][3] * m[1][0] * m[3][1];	// +14,21,42
		result.m[2][2] -= m[0][3] * m[1][1] * m[3][0];	// -14,22,41
		result.m[2][2] -= m[0][1] * m[1][0] * m[3][3];	// -12,21,44
		result.m[2][2] -= m[0][0] * m[1][3] * m[3][1];	// -11,24,42

		result.m[2][3] = 0.0f;							// 34
		result.m[2][3] -= m[0][0] * m[1][1] * m[2][3];	// -11,22,34
		result.m[2][3] -= m[0][1] * m[1][3] * m[2][0];	// -12,24,31
		result.m[2][3] -= m[0][3] * m[1][0] * m[2][1];	// -14,21,32
		result.m[2][3] += m[0][3] * m[1][1] * m[2][0];	// +14,22,31
		result.m[2][3] += m[0][1] * m[1][0] * m[2][3];	// +12,21,34
		result.m[2][3] += m[0][0] * m[1][3] * m[2][1];	// +11,24,32


		// 4行目
		result.m[3][0] = 0.0f;							// 41
		result.m[3][0] -= m[1][0] * m[2][1] * m[3][2];	// -21,32,43
		result.m[3][0] -= m[1][1] * m[2][2] * m[3][0];	// -22,33,41
		result.m[3][0] -= m[1][2] * m[2][0] * m[3][1];	// -23,31,42
		result.m[3][0] += m[1][2] * m[2][1] * m[3][0];	// +23,32,41
		result.m[3][0] += m[1][1] * m[2][0] * m[3][2];	// +22,31,43
		result.m[3][0] += m[1][0] * m[2][2] * m[3][1];	// +21,33,42

		result.m[3][1] = 0.0f;							// 42
		result.m[3][1] += m[0][0] * m[2][1] * m[3][2];	// +11,32,43
		result.m[3][1] += m[0][1] * m[2][2] * m[3][0];	// +12,33,41
		result.m[3][1] += m[0][2] * m[2][0] * m[3][1];	// +13,31,42
		result.m[3][1] -= m[0][2] * m[2][1] * m[3][0];	// -13,32,41
		result.m[3][1] -= m[0][1] * m[2][0] * m[3][2];	// -12,31,43
		result.m[3][1] -= m[0][0] * m[2][2] * m[3][1];	// -11,33,42

		result.m[3][2] = 0.0f;							// 43
		result.m[3][2] -= m[0][0] * m[1][1] * m[3][2];	// -11,22,43
		result.m[3][2] -= m[0][1] * m[1][2] * m[3][0];	// -12,23,41
		result.m[3][2] -= m[0][2] * m[1][0] * m[3][1];	// -13,21,42
		result.m[3][2] += m[0][2] * m[1][1] * m[3][0];	// +13,22,41
		result.m[3][2] += m[0][1] * m[1][0] * m[3][2];	// +12,21,43
		result.m[3][2] += m[0][0] * m[1][2] * m[3][1];	// +11,23,42

		result.m[3][3] = 0.0f;							// 44
		result.m[3][3] += m[0][0] * m[1][1] * m[2][2];	// +11,22,33
		result.m[3][3] += m[0][1] * m[1][2] * m[2][0];	// +12,23,31
		result.m[3][3] += m[0][2] * m[1][0] * m[2][1];	// +13,21,32
		result.m[3][3] -= m[0][2] * m[1][1] * m[2][0];	// -13,22,31
		result.m[3][3] -= m[0][1] * m[1][0] * m[2][2];	// -12,21,33
		result.m[3][3] -= m[0][0] * m[1][2] * m[2][1];	// -11,23,32

		return result;
	}
	/// <summary>
	/// 逆行列(処理多)
	/// </summary>
	/// <returns></returns>
	inline Matrix44 Inverse() const {
		float det = Determinant();
		assert(det != 0.0f); // 0の場合逆行列がない
		return (1.0f / det * Adjugate());
	}
	/// <summary>
	/// 転置行列
	/// </summary>
	/// <returns></returns>
	inline Matrix44 Transpose() const {
		return Matrix44(
			m[0][0], m[1][0], m[2][0], m[3][0],
			m[0][1], m[1][1], m[2][1], m[3][1],
			m[0][2], m[1][2], m[2][2], m[3][2],
			m[0][3], m[1][3], m[2][3], m[3][3]);
	}

	/// <summary>
	/// 拡大縮小行列を作成
	/// </summary>
	/// <param name="scale"></param>
	/// <returns></returns>
	static inline Matrix44 CreateScaling(const Vector3& scale) {
		return Matrix44(
			scale.x,	0.0f,		0.0f,		0.0f,
			0.0f,		scale.y,	0.0f,		0.0f,
			0.0f,		0.0f,		scale.z,	0.0f,
			0.0f,		0.0f,		0.0f,		1.0f);
	}
	/// <summary>
	/// x軸回りの回転行列を作成
	/// </summary>
	/// <param name="theta"></param>
	/// <returns></returns>
	static inline Matrix44 CreateRotationX(float theta) {
		float c = Math::Cos(theta);
		float s = Math::Sin(theta);
		return Matrix44(
			1.0f,	0.0f,	0.0f,	0.0f,
			0.0f,	c,		s,		0.0f,
			0.0f,	-s,		c,		0.0f,
			0.0f,	0.0f,	0.0f,	1.0f);
	}
	/// <summary>
	/// y軸回りの回転行列を作成
	/// </summary>
	/// <param name="theta"></param>
	/// <returns></returns>
	static inline Matrix44 CreateRotationY(float theta) {
		float c = Math::Cos(theta);
		float s = Math::Sin(theta);
		return Matrix44(
			c,		0.0f,	-s,		0.0f,
			0.0f,	1.0f,	0.0f,	0.0f,
			s,		0.0f,	c,		0.0f,
			0.0f,	0.0f,	0.0f,	1.0f);
	}
	/// <summary>
	/// z軸回りの回転行列を作成
	/// </summary>
	/// /// <param name="theta"></param>
	/// <returns></returns>
	static inline Matrix44 CreateRotationZ(float theta) {
		float c = Math::Cos(theta);
		float s = Math::Sin(theta);
		return Matrix44(
			c,		s,		0.0f,	0.0f,
			-s,		c,		0.0f,	0.0f,
			0.0f,	0.0f,	1.0f,	0.0f,
			0.0f,	0.0f,	0.0f,	1.0f);
	}
	/// <summary>
	/// オイラー角から回転行列を作成
	/// </summary>
	/// <param name="euler"></param>
	/// <returns></returns>
	static inline Matrix44 CreateRotationFromEuler(const Vector3& euler) {
		Vector3 s = Vector3(Math::Sin(euler.x), Math::Sin(euler.y), Math::Sin(euler.z));
		Vector3 c = Vector3(Math::Cos(euler.x), Math::Cos(euler.y), Math::Cos(euler.z));

		return Matrix44(
			-s.x * s.y * s.z + c.y * c.z,	s.x * s.y * c.z + c.y * s.z,	-c.x * s.y,		0.0f,
			-c.x * s.z,						c.x * c.z,						s.x,			0.0f,
			s.x * c.y * s.z + s.y * c.z,	-s.x * c.y * c.z + s.y * s.z,	c.x * c.y,		0.0f,
			0.0f,							0.0f,							0.0f,			1.0f);;
	}
	/// <summary>
	/// クォータニオン回転行列を作成
	/// </summary>
	/// <param name="q"></param>
	/// <returns></returns>
	static Matrix44 CreateRotationFromQuaternion(const Quaternion& q) {
		float w2 = q.w * q.w, x2 = q.x * q.x, y2 = q.y * q.y, z2 = q.z * q.z;
		float wx = q.w * q.x, wy = q.w * q.y, wz = q.w * q.z;
		float xy = q.x * q.y, xz = q.x * q.z, yz = q.y * q.z;

		return Matrix44(
			w2 + x2 - y2 - z2,	2.0f * (wz + xy),	2.0f * (xz - wy),	0.0f,
			2.0f * (xy - wz),	w2 - x2 + y2 - z2,	2.0f * (yz + wx),	0.0f,
			2.0f * (wy + xz),	2.0f * (-wx + yz),	w2 - x2 - y2 + z2,	0.0f,
			0.0f,				0.0f,				0.0f,				1.0f);
	}
	/// <summary>
	/// 平行移動行列を作成
	/// </summary>
	/// <param name="vector"></param>
	/// <returns></returns>
	static inline Matrix44 CreateTranslation(const Vector3& vector) {
		return Matrix44(
			1.0f,		0.0f,		0.0f,		0.0f,
			0.0f,		1.0f,		0.0f,		0.0f,
			0.0f,		0.0f,		1.0f,		0.0f,
			vector.x,	vector.y,	vector.z,	1.0f);
	}
	/// <summary>
	/// ルックアット行列を作成
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="target">向ける座標</param>
	/// <param name="up">上ベクトル</param>
	/// <returns></returns>
	static inline Matrix44 CreateLookAt(const Vector3& pos, const Vector3& target, const Vector3& up) {
		Vector3 z = (target - pos).Normalized();
		Vector3 x = Cross(up, z).Normalized();
		Vector3 y = Cross(z, x);
		return Matrix44(
			x.x,	x.y,	x.z,	0.0f,
			y.x,	y.y,	y.z,	0.0f,
			z.x,	z.y,	z.z,	0.0f,
			0.0f,	0.0f,	0.0f,	1.0f);
	}
	/// <summary>
	/// 全方向ビルボード行列を作成
	/// </summary>
	/// <param name="camera_eye">カメラの座標</param>
	/// <param name="camera_target">注視点座標</param>
	/// <param name="camera_up">上ベクトル</param>
	/// <returns></returns>
	static inline Matrix44 CreateBillBoard(const Vector3& camera_eye, const Vector3& camera_target, const Vector3& camera_up) {
		return CreateLookAt(camera_eye, camera_target, camera_up);
	}
	/// <summary>
	/// y軸ビルボード行列を作成
	/// </summary>
	/// <param name="camera_eye">カメラの座標</param>
	/// <param name="camera_target">注視点座標</param>
	/// <param name="camera_up">上ベクトル</param>
	/// <returns></returns>
	static inline Matrix44 CreateBillBoardYAxis(const Vector3& camera_eye, const Vector3& camera_target, const Vector3& camera_up) {
		Vector3 z = (camera_target - camera_eye).Normalized();
		Vector3 x = Cross(camera_up, z).Normalized();
		Vector3 y = camera_up.Normalized();
		z = Cross(x, y);	// zを再計算
		return Matrix44(
			x.x,	x.y,	x.z,	0.0f,
			y.z,	y.y,	y.z,	0.0f,
			z.x,	z.y,	z.z,	0.0f,
			0.0f,	0.0f,	0.0f,	1.0f);
	}
	/// <summary>
	/// ビュー行列を作成
	/// </summary>
	/// <param name="eye">カメラの座標</param>
	/// <param name="target">注視点座標</param>
	/// <param name="up">上ベクトル</param>
	/// <returns></returns>
	static inline Matrix44 CreateView(const Vector3& eye, const Vector3& target, const Vector3& up) {
		Vector3 z = (target - eye).Normalized();
		Vector3 x = Cross(up, z).Normalized();
		Vector3 y = Cross(z, x);
		Vector3 w = Vector3(-Dot(x, eye), -Dot(y, eye), -Dot(z, eye));
		return Matrix44(
			x.x, y.x, z.x, 0.0f,
			x.y, y.y, z.y, 0.0f,
			x.z, y.z, z.z, 0.0f,
			w.x, w.y, w.z, 1.0f);
	}
	/// <summary>
	/// 透視投影行列を作成
	/// </summary>
	/// <param name="fovY">垂直画角</param>
	/// <param name="aspect">アスペクト比 高 / 幅</param>
	/// <param name="nearZ">クリッピング距離(近)</param>
	/// <param name="farZ">クリッピング距離(遠)</param>
	/// <returns></returns>
	static inline Matrix44 CreatePerspectiveProjection(float fovY, float aspect, float nearZ, float farZ) {
		float s = 1.0f / Math::Tan(fovY / 2.0f);
		float a = farZ / (farZ - nearZ);
		return Matrix44(
			 s / aspect,	0.0f,		0.0f,			0.0f,
			 0.0f,			s,			0.0f,			0.0f,
			 0.0f,			0.0f,		a,				1.0f,
			 0.0f,			0.0f,		a * -nearZ,		0.0f);
	}
	/// <summary>
	/// 平行投影行列を作成
	/// </summary>
	/// <param name="left">左</param>
	/// <param name="top">上</param>
	/// <param name="right">右</param>
	/// <param name="bottom">下</param>
	/// <param name="nearZ">クリッピング距離(近)</param>
	/// <param name="farZ">クリッピング距離(遠)</param>
	/// <returns></returns>
	static inline Matrix44 CreateOrthographicProjection(float left, float top, float right, float bottom, float nearZ = 0.0f, float farZ = 1.0f) {
		return Matrix44(
			2.0f / (right - left),				0.0f,								0.0f,						0.0f,
			0.0f,								2.0f / (top - bottom),				0.0f,						0.0f,
			0.0f,								0.0f,								1.0f / (farZ - nearZ),		0.0f,
			(left + right) / (left - right),	(top + bottom) / (bottom - top),	nearZ / (nearZ - farZ),		1.0f);
	}
	/// <summary>
	/// ビューポート行列を作成
	/// </summary>
	/// <param name="left">左</param>
	/// <param name="top">上</param>
	/// <param name="width">幅</param>
	/// <param name="height">高</param>
	/// <param name="nearZ">クリッピング距離(近)</param>
	/// <param name="farZ">クリッピング距離(遠)</param>
	/// <returns></returns>
	static inline Matrix44 CreateViewport(float left, float top, float width, float height, float nearZ = 0.0f, float farZ = 1.0f) {
		float halfW = width / 2.0f;
		float halfh = height / 2.0f;
		return Matrix44(
			halfW,			0.0f,			0.0f,			0.0f,
			0.0f,			-halfh,			0.0f,			0.0f,
			0.0f,			0.0f,			farZ - nearZ,	0.0f,
			left + halfW,	top + halfh,	nearZ,			1.0f);
	}
	/// <summary>
	/// 33行列から44行列を作成
	/// </summary>
	/// <param name="mat"></param>
	/// <returns></returns>
	static Matrix44 CreateFromMatrix33(const Matrix33& mat);
};
/// <summary>
/// 色
/// </summary>
namespace Color {
	static const unsigned char kByteMax = 0xFF;

	static const Vector4 Black = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	static const Vector4 White = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	static const Vector4 Red = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	static const Vector4 Green = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	static const Vector4 Blue = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

	inline unsigned int Create(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
		return r << 24 | g << 16 | b << 8 | a;
	}
	/// <summary>
	/// unsigned int から Vector4に変換
	/// </summary>
	/// <param name="color"></param>
	/// <returns></returns>
	inline Vector4 ToVector4(unsigned int color) {
		Vector4 result = Vector4(
			static_cast<float>((color >> 24) & 0xFF) / 255.0f,
			static_cast<float>((color >> 16) & 0xFF) / 255.0f,
			static_cast<float>((color >> 8) & 0xFF) / 255.0f,
			static_cast<float>((color >> 0) & 0xFF) / 255.0f
		);
		static auto toLinear = [](float color) {
			if (0 <= color && color <= 0.04045f) {
				return color / 12.92f;
			}
			return Math::Pow((color + 0.055f) / 1.055f, 2.4f);
		};

		result.x = toLinear(result.x);
		result.y = toLinear(result.y);
		result.z = toLinear(result.z);
		return result;
	}
	/// <summary>
	/// HSVからRGBに変換
	/// </summary>
	/// <param name="h">色相 : 0 ~ 360</param>
	/// <param name="s">彩度 : 0 ~ 1</param>
	/// <param name="v">明度 : 0 ~ 1</param>
	/// <param name="a">透明度 0 ~ 255 符号なし整数型</param>
	/// <returns></returns>
	unsigned int HSVAtoRGBA_uint(float h, float s, float v, unsigned int a);
	/// <summary>
	/// HSVAからRGBAに変換
	/// </summary>
	/// <param name="H">色相</param>
	/// <param name="S">彩度</param>
	/// <param name="V">明度</param>
	/// <param name="A">透明度</param>
	/// <returns></returns>
	inline Vector4 HSVAtoRGBA(float H, float S, float V, float A) {
		if (S <= 0.0f) {
			return Vector4(Vector3(V), A);
		}
		int i = static_cast<int>(floor(H * 6.0f));
		float f = H * 6.0f - i;
		float p = V * (1.0f - S);
		float q = V * (1.0f - f * S);
		float t = V * (1.0f - (1.0f - f) * S);

		Vector4 color;
		switch (i % 6) {
		case 0: {
			color.x = V; 
			color.y = t; 
			color.z = p;
			break;
			}
		case 1: {
			color.x = q; 
			color.y = V; 
			color.z = p;
			break;
			}
		case 2: {
			color.x = p; 
			color.y = V; 
			color.z = t;
			break;
			}
		case 3: {
			color.x = p;
			color.y = q; 
			color.z = V;
			break;
			}
		case 4: {
			color.x = t; 
			color.y = p; 
			color.z = V;
			break;
			}
		case 5: {
			color.x = V; 
			color.y = p; 
			color.z = q;
			break;
			}
		}
		color.w = A;
		return color;
	}
};

#endif
