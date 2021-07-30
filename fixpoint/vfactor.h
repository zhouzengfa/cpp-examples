#pragma once

class VFactor
{
	typedef long IntPart;
	typedef long FactPart;
	typedef long long int64_t;

private:
	int64_t nom;
	int64_t den;

	static const int64_t mask_ = 9223372036854775807L;
	static const int64_t upper_ = 16777215L;

public:
	//int roundInt()
	//{
	//	return (int)IntMath.Divide(this.nom, this.den);
	//}

	int integer()
	{
		return (int)(nom / den);
	}

	float single()
	{
		double num = (double)nom / (double)den;
		return (float)num;
	}

	bool IsPositive()
	{
		if (nom == 0L)
		{
			return false;
		}
		bool flag = nom > 0L;
		bool flag2 = den > 0L;
		return !(flag ^ flag2);
	}

	bool IsNegative()
	{
		if (nom == 0L)
		{
			return false;
		}
		bool flag = nom > 0L;
		bool flag2 = den > 0L;
		return flag ^ flag2;
	}

	bool IsZero()
	{
		return nom == 0L;
	}

	VFactor Inverse()
	{
		return VFactor(den, nom);
	}

	VFactor(int64_t n, int64_t d)
	{
		nom = n;
		den = d;
	}

	void strip()
	{
		while ((nom & VFactor::mask_) > VFactor::upper_ && (den & VFactor::mask_) > VFactor::upper_)
		{
			nom >>= 1;
			den >>= 1;
		}
	}

	friend bool operator <(const VFactor& a, const VFactor& b)
	{
		int64_t num = a.nom * b.den;
		int64_t num2 = b.nom * a.den;
		bool flag = (b.den > 0L) ^ (a.den > 0L);
		return (!flag) ? (num < num2) : (num > num2);
	}

	friend bool operator >(const VFactor& a, const VFactor& b)
	{
		int64_t num = a.nom * b.den;
		int64_t num2 = b.nom * a.den;
		bool flag = (b.den > 0L) ^ (a.den > 0L);
		return (!flag) ? (num > num2) : (num < num2);
	}

	friend bool operator <=(const VFactor& a, const VFactor& b)
	{
		int64_t num = a.nom * b.den;
		int64_t num2 = b.nom * a.den;
		bool flag = (b.den > 0L) ^ (a.den > 0L);
		return (!flag) ? (num <= num2) : (num >= num2);
	}

	friend bool operator >=(const VFactor& a, const VFactor& b)
	{
		int64_t num = a.nom * b.den;
		int64_t num2 = b.nom * a.den;
		bool flag = (b.den > 0L) ^ (a.den > 0L);
		return (!flag) ? (num >= num2) : (num <= num2);
	}

	friend bool operator ==(const VFactor& a, const VFactor& b)
	{
		return a.nom * b.den == b.nom * a.den;
	}

	friend bool operator !=(const VFactor& a, const VFactor& b)
	{
		return a.nom * b.den != b.nom * a.den;
	}

	friend bool operator <(VFactor a, int64_t b)
	{
		int64_t num = a.nom;
		int64_t num2 = b * a.den;
		return (a.den <= 0L) ? (num > num2) : (num < num2);
	}

	friend bool operator >(VFactor a, int64_t b)
	{
		int64_t num = a.nom;
		int64_t num2 = b * a.den;
		return (a.den <= 0L) ? (num < num2) : (num > num2);
	}

	friend bool operator <=(VFactor a, int64_t b)
	{
		int64_t num = a.nom;
		int64_t num2 = b * a.den;
		return (a.den <= 0L) ? (num >= num2) : (num <= num2);
	}

	friend bool operator >=(VFactor a, int64_t b)
	{
		int64_t num = a.nom;
		int64_t num2 = b * a.den;
		return (a.den <= 0L) ? (num <= num2) : (num >= num2);
	}

	friend bool operator ==(VFactor a, int64_t b)
	{
		return a.nom == b * a.den;
	}

	friend bool operator !=(VFactor a, int64_t b)
	{
		return a.nom != b * a.den;
	}

	friend VFactor operator +(const VFactor& a, const VFactor& b)
	{
		return VFactor(a.nom * b.den + b.nom * a.den, a.den * b.den);
	}

	VFactor& operator+=(const VFactor& a)
	{
		nom = nom * a.den + den * a.nom;
		den *= a.den;
		strip();
		return *this;
	}

	friend VFactor& operator +(VFactor& a, int64_t b)
	{
		a.nom += b * a.den;
		return a;
	}

	friend VFactor operator -(const VFactor& a, const VFactor& b)
	{
		return VFactor(a.nom * b.den - b.nom * a.den, a.den * b.den);
	}

	friend VFactor operator *(const VFactor& a, const VFactor& b)
	{
		VFactor tmp(a.nom * b.nom, a.den * b.den);
		tmp.strip();
		return tmp;
	}

	VFactor& operator *=(const VFactor& a)
	{
		nom *= a.nom;
		den *= a.den;
		strip();
		return *this;
	}

	friend VFactor& operator -(VFactor& a, int64_t b)
	{
		a.nom -= b * a.den;
		return a;
	}

	friend VFactor& operator *(VFactor& a, int64_t b)
	{
		a.nom *= b;
		return a;
	}

	friend VFactor& operator /(VFactor& a, int64_t b)
	{
		a.den *= b;
		return a;
	}

	friend VFactor operator /(const VFactor& a, const VFactor& b)
	{
		return VFactor(a.nom * b.den, a.den * b.nom);
	}

	// static VInt3 operator *(VInt3 v, VFactor f)
	//{
	//	return IntMath.Divide(v, f.nom, f.den);
	//}
	//public static VInt2 operator *(VInt2 v, VFactor f)
	//{
	//	return IntMath.Divide(v, f.nom, f.den);
	//}
	//public static VInt3 operator /(VInt3 v, VFactor f)
	//{
	//	return IntMath.Divide(v, f.den, f.nom);
	//}
	//public static VInt2 operator /(VInt2 v, VFactor f)
	//{
	//	return IntMath.Divide(v, f.den, f.nom);
	//}

	friend VFactor& operator -(VFactor& a)
	{
		a.nom = -a.nom;
		return a;
	}
};

