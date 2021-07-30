//--------------------------------------------------------------------
// 文件名:		FixedPointFloat.h
// 内  容:		固定浮点类型，运算不受精度影响
// 说  明:		
// 创建日期:		2020年8月31日
// 创建人:		熊跃华
// 版权所有:		上海灵刃网络科技有限公司
//--------------------------------------------------------------------

#ifndef _ERAENGINE_FIXEDPOINTFLOAT_H
#define _ERAENGINE_FIXEDPOINTFLOAT_H

#include <stdint.h>

class FixedPointFloat
{
#define FRACTION_BITS 16
#define INTEGER_BITS 32

	typedef int64_t StorageType;
	typedef uint64_t CalcType;

private:
	StorageType _num;
	double _show;
	const static CalcType c_one;
	const static StorageType c_intMask;
	const static StorageType c_fracMask;

private:
	typedef unsigned short TableType;
	const static TableType c_sinTableCount;
	const static TableType c_sinTable[];
	const static TableType c_asinTableCount;
	const static TableType c_asinTable[];

public:
	FixedPointFloat();
	FixedPointFloat(int);
	FixedPointFloat(const char* value);
	FixedPointFloat(const FixedPointFloat&);

	void LoadFromClientValue(float value);

private:
	FixedPointFloat(StorageType);

	inline void RefreshShowValue() {_show = (double)_num / (double)c_one; }

public:
	friend FixedPointFloat operator + (const FixedPointFloat&, const FixedPointFloat&);
	FixedPointFloat& operator += (const FixedPointFloat&);

	friend FixedPointFloat operator - (const FixedPointFloat&, const FixedPointFloat&);
	FixedPointFloat& operator -= (const FixedPointFloat&);

	friend FixedPointFloat operator * (const FixedPointFloat&, const FixedPointFloat&);
	FixedPointFloat& operator *= (const FixedPointFloat&);

	friend FixedPointFloat operator / (const FixedPointFloat&, const FixedPointFloat&);
	FixedPointFloat& operator /= (const FixedPointFloat&);

	friend bool operator == (const FixedPointFloat&, const FixedPointFloat&);
	friend bool operator != (const FixedPointFloat&, const FixedPointFloat&);
	friend bool operator > (const FixedPointFloat&, const FixedPointFloat&);
	friend bool operator >= (const FixedPointFloat&, const FixedPointFloat&);
	friend bool operator < (const FixedPointFloat&, const FixedPointFloat&);
	friend bool operator <= (const FixedPointFloat&, const FixedPointFloat&);

	FixedPointFloat& operator ++ ();
	FixedPointFloat& operator -- ();
	FixedPointFloat operator - ();

	operator int() const;
	operator double() const;
	operator float() const;

	FixedPointFloat Sqrt() const;
	FixedPointFloat Abs() const;
	FixedPointFloat Sin() const;
	FixedPointFloat Asin() const;
	FixedPointFloat Atan() const;

	FixedPointFloat& ceil();

private:
	static CalcType GetIntPart(CalcType);
	static CalcType GetFracPart(CalcType);
};
#endif // !_ERAENGINE_FIXEDPOINTFLOAT_H
