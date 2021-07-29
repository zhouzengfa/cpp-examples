#pragma once

class VFactor
{
	typedef long IntPart;
	typedef long FactPart;
private:
	long nom;
	long den;

	static const long mask_ = 9223372036854775807L;
	static const long upper_ = 16777215L;

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

	VFactor(long n, long d)
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
		long num = a.nom * b.den;
		long num2 = b.nom * a.den;
		bool flag = b.den > 0L ^ a.den > 0L;
		return (!flag) ? (num < num2) : (num > num2);
	}

	friend bool operator >(const VFactor& a, const VFactor& b)
	{
		long num = a.nom * b.den;
		long num2 = b.nom * a.den;
		bool flag = b.den > 0L ^ a.den > 0L;
		return (!flag) ? (num > num2) : (num < num2);
	}

	friend bool operator <=(const VFactor& a, const VFactor& b)
	{
		long num = a.nom * b.den;
		long num2 = b.nom * a.den;
		bool flag = b.den > 0L ^ a.den > 0L;
		return (!flag) ? (num <= num2) : (num >= num2);
	}

	friend bool operator >=(const VFactor& a, const VFactor& b)
	{
		long num = a.nom * b.den;
		long num2 = b.nom * a.den;
		bool flag = b.den > 0L ^ a.den > 0L;
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

	friend bool operator <(VFactor a, long b)
	{
		long num = a.nom;
		long num2 = b * a.den;
		return (a.den <= 0L) ? (num > num2) : (num < num2);
	}

	friend bool operator >(VFactor a, long b)
	{
		long num = a.nom;
		long num2 = b * a.den;
		return (a.den <= 0L) ? (num < num2) : (num > num2);
	}

	friend bool operator <=(VFactor a, long b)
	{
		long num = a.nom;
		long num2 = b * a.den;
		return (a.den <= 0L) ? (num >= num2) : (num <= num2);
	}

	friend bool operator >=(VFactor a, long b)
	{
		long num = a.nom;
		long num2 = b * a.den;
		return (a.den <= 0L) ? (num <= num2) : (num >= num2);
	}

	friend bool operator ==(VFactor a, long b)
	{
		return a.nom == b * a.den;
	}

	friend bool operator !=(VFactor a, long b)
	{
		return a.nom != b * a.den;
	}

	friend VFactor operator +(const VFactor& a, const VFactor& b)
	{
		return VFactor(a.nom * b.den + b.nom * a.den, a.den * b.den);
	}

	friend VFactor& operator +(VFactor& a, long b)
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
		return VFactor(a.nom * b.nom, a.den * b.den);
	}

	friend VFactor& operator -(VFactor& a, long b)
	{
		a.nom -= b * a.den;
		return a;
	}

	friend VFactor& operator *(VFactor& a, long b)
	{
		a.nom *= b;
		return a;
	}

	friend VFactor& operator /(VFactor& a, long b)
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

