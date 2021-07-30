﻿#include "FixedPointFloat.h"
#include "../protable/portable.h"
#include "../module_log/log_controller.h"
#include <gtest/gtest.h>

DEFINE_LOG(FixedPointFloat)

#define USE_NEW_FIXPOINT_MULT 1

const static FixedPointFloat PI("3.141592653");
const static FixedPointFloat HalfPI("1.570796326");
const static FixedPointFloat TwoPI("6.283185307");
const static uint64_t MASK = 4294967295L;

inline FixedPointFloat::CalcType FixedPointFloat::GetIntPart(FixedPointFloat::CalcType x)
{
	return (x >> FRACTION_BITS) & c_intMask;
}

inline FixedPointFloat::CalcType FixedPointFloat::GetFracPart(FixedPointFloat::CalcType x)
{
	return x & c_fracMask;
}

bool operator == (const FixedPointFloat& a, const FixedPointFloat& b) { return a._num == b._num; }
bool operator != (const FixedPointFloat& a, const FixedPointFloat& b) { return a._num != b._num; }
bool operator > (const FixedPointFloat& a, const FixedPointFloat& b) { return a._num > b._num; }
bool operator >= (const FixedPointFloat& a, const FixedPointFloat& b) { return a._num >= b._num; }
bool operator < (const FixedPointFloat& a, const FixedPointFloat& b) { return a._num < b._num; }
bool operator <= (const FixedPointFloat& a, const FixedPointFloat& b) { return a._num <= b._num; }

FixedPointFloat& FixedPointFloat::operator ++() { _num += c_one; return (*this); }
FixedPointFloat& FixedPointFloat::operator --() { _num -= c_one; return (*this); }
FixedPointFloat FixedPointFloat::operator -() { FixedPointFloat ret(*this); ret *= FixedPointFloat(-1); return ret; }

const FixedPointFloat::CalcType FixedPointFloat::c_one = (FixedPointFloat::CalcType)1 << FRACTION_BITS;
const FixedPointFloat::StorageType FixedPointFloat::c_intMask = ((FixedPointFloat::StorageType)1 << INTEGER_BITS) - 1;
const FixedPointFloat::StorageType FixedPointFloat::c_fracMask = ((FixedPointFloat::StorageType)1 << FRACTION_BITS) - 1;

const FixedPointFloat::TableType FixedPointFloat::c_sinTableCount = 803;
const FixedPointFloat::TableType FixedPointFloat::c_sinTable[FixedPointFloat::c_sinTableCount] = {
	0,128,256,384,512,640,768,896,1024,1152,1280,1408,1536,1664,1792,1920,2048,2176,2304,2431,2559,2687,2815,2943,3071,3199,3327,3454,3582,3710,3838,3966,4093,4221,4349,4477,4604,4732,4860,4987,5115,5242,5370,5498,5625,5753,5880,6008,6135,6262,6390,6517,6645,6772,6899,7026,7154,7281,7408,7535,7662,7790,7917,8044,8171,8298,8425,8552,8678,8805,8932,9059,9186,9312,9439,9566,9692,9819,9945,10072,10198,10325,10451,10578,10704,10830,10956,11082,11209,11335,11461,11587,11713,11839,11965,12090,12216,12342,12468,12593,12719,12844,12970,13095,13221,13346,13471,13597,13722,13847,13972,14097,14222,14347,14472,14597,14721,14846,14971,15095,15220,15344,15469,15593,15717,15842,15966,16090,16214,16338,16462,16586,16709,16833,16957,17080,17204,17327,17451,17574,17698,17821,17944,18067,18190,18313,18436,18559,18681,18804,18927,19049,19171,19294,19416,19538,19660,19783,19905,20026,20148,20270,20392,20513,20635,20756,20878,20999,21120,21241,21362,21483,21604,21725,21846,21966,22087,22207,22328,22448,22568,22688,22808,22928,23048,23168,23288,23407,23527,23646,23766,23885,24004,24123,24242,24361,24480,24598,24717,24836,24954,25072,25190,25309,25427,25544,25662,25780,25898,26015,26133,26250,26367,26484,26601,26718,26835,26952,27068,27185,27301,27418,27534,27650,27766,27882,27998,28113,28229,28344,28460,28575,28690,28805,28920,29035,29150,29264,29379,29493,29607,29721,29835,29949,30063,30177,30290,30404,30517,30630,30744,30856,30969,31082,31195,31307,31420,31532,31644,31756,31868,31980,32091,32203,32314,32426,32537,32648,32759,32870,32980,33091,33201,33312,33422,33532,33642,33751,33861,33971,34080,34189,34298,34407,34516,34625,34734,34842,34951,35059,35167,35275,35383,35490,35598,35705,35812,35920,36027,36133,36240,36347,36453,36559,36666,36772,36878,36983,37089,37194,37300,37405,37510,37615,37719,37824,37929,38033,38137,38241,38345,38449,38552,38656,38759,38862,38965,39068,39171,39273,39376,39478,39580,39682,39784,39885,39987,40088,40189,40290,40391,40492,40593,40693,40793,40893,40993,41093,41193,41292,41391,41491,41590,41688,41787,41886,41984,42082,42180,42278,42376,42473,42571,42668,42765,42862,42959,43055,43152,43248,43344,43440,43536,43631,43727,43822,43917,44012,44107,44201,44296,44390,44484,44578,44672,44765,44859,44952,45045,45138,45231,45323,45416,45508,45600,45692,45783,45875,45966,46057,46148,46239,46330,46420,46510,46601,46690,46780,46870,46959,47048,47137,47226,47315,47403,47492,47580,47668,47755,47843,47930,48018,48105,48191,48278,48365,48451,48537,48623,48709,48794,48879,48965,49050,49134,49219,49303,49388,49472,49556,49639,49723,49806,49889,49972,50055,50137,50220,50302,50384,50465,50547,50628,50710,50791,50871,50952,51032,51113,51193,51272,51352,51431,51511,51590,51669,51747,51826,51904,51982,52060,52137,52215,52292,52369,52446,52523,52599,52675,52751,52827,52903,52978,53054,53129,53204,53278,53353,53427,53501,53575,53648,53722,53795,53868,53941,54013,54086,54158,54230,54302,54373,54445,54516,54587,54657,54728,54798,54868,54938,55008,55077,55147,55216,55285,55353,55422,55490,55558,55626,55693,55761,55828,55895,55961,56028,56094,56160,56226,56292,56357,56422,56487,56552,56617,56681,56745,56809,56873,56936,57000,57063,57126,57188,57251,57313,57375,57437,57498,57559,57620,57681,57742,57802,57863,57923,57982,58042,58101,58160,58219,58278,58336,58395,58453,58510,58568,58625,58682,58739,58796,58852,58908,58964,59020,59076,59131,59186,59241,59296,59350,59404,59458,59512,59565,59619,59672,59724,59777,59829,59881,59933,59985,60036,60088,60139,60189,60240,60290,60340,60390,60440,60489,60538,60587,60636,60684,60733,60781,60828,60876,60923,60970,61017,61064,61110,61156,61202,61248,61293,61338,61383,61428,61472,61517,61561,61604,61648,61691,61734,61777,61820,61862,61904,61946,61988,62029,62071,62111,62152,62193,62233,62273,62313,62352,62391,62431,62469,62508,62546,62584,62622,62660,62697,62734,62771,62808,62844,62881,62917,62952,62988,63023,63058,63093,63127,63161,63195,63229,63263,63296,63329,63362,63395,63427,63459,63491,63522,63554,63585,63616,63646,63677,63707,63737,63767,63796,63825,63854,63883,63911,63939,63967,63995,64023,64050,64077,64103,64130,64156,64182,64208,64234,64259,64284,64309,64333,64357,64381,64405,64429,64452,64475,64498,64521,64543,64565,64587,64608,64630,64651,64672,64692,64713,64733,64753,64772,64792,64811,64829,64848,64866,64885,64902,64920,64938,64955,64972,64988,65005,65021,65037,65052,65068,65083,65098,65112,65127,65141,65155,65168,65182,65195,65208,65221,65233,65245,65257,65269,65280,65291,65302,65313,65323,65334,65344,65353,65363,65372,65381,65389,65398,65406,65414,65422,65429,65436,65443,65450,65456,65463,65468,65474,65480,65485,65490,65494,65499,65503,65507,65511,65514,65517,65520,65523,65525,65527,65529,65531,65533,65534,65535,65535
};

const FixedPointFloat::TableType FixedPointFloat::c_asinTableCount = 1024;
const FixedPointFloat::TableType FixedPointFloat::c_asinTable[FixedPointFloat::c_asinTableCount] = {
	0,32,64,96,128,160,192,224,256,288,320,352,384,416,448,480,512,544,576,608,640,672,704,736,768,800,832,864,896,928,960,992,1024,1056,1088,1120,1152,1184,1216,1248,1280,1312,1344,1376,1408,1440,1472,1505,1537,1569,1601,1633,1665,1697,1729,1761,1793,1825,1857,1889,1921,1953,1985,2017,2049,2081,2113,2146,2178,2210,2242,2274,2306,2338,2370,2402,2434,2466,2498,2531,2563,2595,2627,2659,2691,2723,2755,2787,2819,2852,2884,2916,2948,2980,3012,3044,3077,3109,3141,3173,3205,3237,3269,3302,3334,3366,3398,3430,3462,3495,3527,3559,3591,3623,3656,3688,3720,3752,3784,3817,3849,3881,3913,3946,3978,4010,4042,4074,4107,4139,4171,4204,4236,4268,4300,4333,4365,4397,4429,4462,4494,4526,4559,4591,4623,4656,4688,4720,4753,4785,4817,4850,4882,4914,4947,4979,5012,5044,5076,5109,5141,5173,5206,5238,5271,5303,5336,5368,5400,5433,5465,5498,5530,5563,5595,5628,5660,5693,5725,5758,5790,5823,5855,5888,5920,5953,5985,6018,6050,6083,6115,6148,6181,6213,6246,6278,6311,6344,6376,6409,6441,6474,6507,6539,6572,6605,6637,6670,6703,6735,6768,6801,6833,6866,6899,6932,6964,6997,7030,7063,7095,7128,7161,7194,7226,7259,7292,7325,7358,7390,7423,7456,7489,7522,7555,7588,7621,7653,7686,7719,7752,7785,7818,7851,7884,7917,7950,7983,8016,8049,8082,8115,8148,8181,8214,8247,8280,8313,8346,8379,8412,8445,8478,8511,8545,8578,8611,8644,8677,8710,8743,8777,8810,8843,8876,8909,8943,8976,9009,9042,9076,9109,9142,9175,9209,9242,9275,9309,9342,9375,9409,9442,9476,9509,9542,9576,9609,9643,9676,9709,9743,9776,9810,9843,9877,9910,9944,9977,10011,10045,10078,10112,10145,10179,10213,10246,10280,10313,10347,10381,10414,10448,10482,10516,10549,10583,10617,10651,10684,10718,10752,10786,10820,10853,10887,10921,10955,10989,11023,11057,11091,11124,11158,11192,11226,11260,11294,11328,11362,11396,11430,11464,11499,11533,11567,11601,11635,11669,11703,11737,11772,11806,11840,11874,11908,11943,11977,12011,12045,12080,12114,12148,12183,12217,12251,12286,12320,12355,12389,12424,12458,12492,12527,12561,12596,12630,12665,12700,12734,12769,12803,12838,12873,12907,12942,12977,13011,13046,13081,13115,13150,13185,13220,13254,13289,13324,13359,13394,13429,13464,13499,13533,13568,13603,13638,13673,13708,13743,13778,13814,13849,13884,13919,13954,13989,14024,14059,14095,14130,14165,14200,14236,14271,14306,14341,14377,14412,14448,14483,14518,14554,14589,14625,14660,14696,14731,14767,14802,14838,14873,14909,14945,14980,15016,15052,15087,15123,15159,15195,15230,15266,15302,15338,15374,15410,15446,15482,15518,15553,15589,15625,15662,15698,15734,15770,15806,15842,15878,15914,15950,15987,16023,16059,16095,16132,16168,16204,16241,16277,16314,16350,16386,16423,16459,16496,16533,16569,16606,16642,16679,16716,16752,16789,16826,16862,16899,16936,16973,17010,17047,17083,17120,17157,17194,17231,17268,17305,17342,17379,17417,17454,17491,17528,17565,17602,17640,17677,17714,17752,17789,17826,17864,17901,17939,17976,18014,18051,18089,18126,18164,18202,18239,18277,18315,18353,18390,18428,18466,18504,18542,18580,18618,18656,18694,18732,18770,18808,18846,18884,18922,18960,18999,19037,19075,19114,19152,19190,19229,19267,19306,19344,19383,19421,19460,19498,19537,19576,19615,19653,19692,19731,19770,19809,19848,19886,19925,19964,20003,20043,20082,20121,20160,20199,20238,20278,20317,20356,20396,20435,20474,20514,20553,20593,20633,20672,20712,20752,20791,20831,20871,20911,20950,20990,21030,21070,21110,21150,21190,21230,21271,21311,21351,21391,21432,21472,21512,21553,21593,21634,21674,21715,21755,21796,21837,21877,21918,21959,22000,22041,22082,22123,22164,22205,22246,22287,22328,22369,22411,22452,22493,22535,22576,22618,22659,22701,22742,22784,22826,22867,22909,22951,22993,23035,23077,23119,23161,23203,23245,23287,23330,23372,23414,23457,23499,23541,23584,23627,23669,23712,23755,23797,23840,23883,23926,23969,24012,24055,24098,24141,24185,24228,24271,24315,24358,24402,24445,24489,24532,24576,24620,24664,24708,24751,24795,24839,24884,24928,24972,25016,25061,25105,25149,25194,25238,25283,25328,25372,25417,25462,25507,25552,25597,25642,25687,25732,25778,25823,25868,25914,25959,26005,26051,26096,26142,26188,26234,26280,26326,26372,26418,26465,26511,26557,26604,26650,26697,26744,26790,26837,26884,26931,26978,27025,27072,27120,27167,27214,27262,27309,27357,27405,27453,27500,27548,27596,27645,27693,27741,27789,27838,27886,27935,27983,28032,28081,28130,28179,28228,28277,28326,28376,28425,28475,28524,28574,28624,28674,28724,28774,28824,28874,28925,28975,29026,29076,29127,29178,29229,29280,29331,29382,29433,29485,29536,29588,29640,29691,29743,29795,29847,29900,29952,30005,30057,30110,30163,30216,30269,30322,30375,30428,30482,30535,30589,30643,30697,30751,30805,30860,30914,30969,31023,31078,31133,31188,31243,31299,31354,31410,31466,31521,31577,31634,31690,31746,31803,31860,31917,31974,32031,32088,32145,32203,32261,32319,32377,32435,32493,32552,32611,32670,32729,32788,32847,32907,32966,33026,33086,33147,33207,33268,33328,33389,33450,33512,33573,33635,33697,33759,33821,33884,33946,34009,34072,34136,34199,34263,34327,34391,34455,34520,34585,34650,34715,34780,34846,34912,34978,35045,35112,35179,35246,35313,35381,35449,35517,35586,35655,35724,35793,35863,35933,36003,36073,36144,36215,36287,36358,36430,36503,36576,36649,36722,36796,36870,36944,37019,37094,37170,37246,37322,37398,37475,37553,37631,37709,37788,37867,37946,38026,38107,38188,38269,38351,38433,38516,38599,38683,38768,38853,38938,39024,39111,39198,39286,39374,39463,39553,39643,39734,39825,39918,40011,40104,40199,40294,40390,40487,40585,40683,40783,40883,40984,41087,41190,41294,41399,41505,41613,41721,41831,41942,42054,42168,42283,42399,42517,42636,42757,42880,43004,43130,43258,43388,43520,43655,43791,43930,44072,44216,44363,44514,44667,44824,44985,45150,45319,45493,45672,45856,46047,46245,46450,46665,46889,47124,47373,47638,47923,48232,48575,48963,49424,50024
};

FixedPointFloat::FixedPointFloat() : _num(0) { RefreshShowValue(); }

FixedPointFloat::FixedPointFloat(int i) : _num((StorageType)i << FRACTION_BITS) { RefreshShowValue(); }

FixedPointFloat::FixedPointFloat(const char* value)
{
	char buf[128];
	int size = (int)strlen(value) + 1;

	if (size > 128)
	{
		memcpy(buf, value, 127);
		buf[127] = 0;
	}
	else
	{
		memcpy(buf, value, size);
	}

	char* splitter = strchr(buf, '.');

	if (NULL == splitter)
	{
		_num = ((StorageType)atoi(buf) << FRACTION_BITS);
		RefreshShowValue();
		return;
	}

	*splitter = 0;

	int beforeDot = atoi(buf);
	char* minus = strchr(buf, '-');

	int minusProcValue = 0;
	//当整数部分为0的负数时先将整数部分置为-1,后续再去除
	if (NULL != minus && 0 == beforeDot)
	{
		minusProcValue = 1;
		beforeDot -= minusProcValue;
	}
	_num = ((StorageType)beforeDot << FRACTION_BITS);

	char* afterDotStart = splitter + 1;
	int afterDot = 0;

	size = (int)strlen(afterDotStart);
	size = (size > 4 ? 4 : size);
	int rate = 1;

	for (int i = 0; i < size; ++i)
	{
		char value = (afterDotStart[i] - '0');

		if (value < 0 || value > 9)
		{
			break;
		}

		rate *= 10;
		afterDot *= 10;
		afterDot += value;
	}

	FixedPointFloat a = FixedPointFloat(afterDot) / FixedPointFloat(rate);

	if (beforeDot >= 0)
	{
		_num += a._num;
	}
	else
	{
		_num -= a._num;
		if (minusProcValue != 0)
		{
			_num += FixedPointFloat(minusProcValue)._num;
		}
	}

	RefreshShowValue();
}

FixedPointFloat::FixedPointFloat(FixedPointFloat::StorageType n) : _num(n) { RefreshShowValue(); }

FixedPointFloat::FixedPointFloat(const FixedPointFloat& fpf) : _num(fpf._num) { RefreshShowValue(); }

void FixedPointFloat::LoadFromClientValue(float value)
{
	_num = ((StorageType)(value * c_one + (value >= 0 ? 0.5 : -0.5)));
}

FixedPointFloat& FixedPointFloat::operator += (const FixedPointFloat& x)
{
	_num += x._num;
	RefreshShowValue();
	return *this;
}

FixedPointFloat operator + (const FixedPointFloat& a, const FixedPointFloat& b)
{
	return FixedPointFloat(a._num + b._num);
}

FixedPointFloat& FixedPointFloat::operator -= (const FixedPointFloat& x)
{
	_num -= x._num;
	RefreshShowValue();
	return *this;
}

FixedPointFloat operator - (const FixedPointFloat& a, const FixedPointFloat& b)
{
	return FixedPointFloat(a._num - b._num);
}

FixedPointFloat operator * (const FixedPointFloat& a, const FixedPointFloat& b)
{
#ifdef USE_NEW_FIXPOINT_MULT
	FixedPointFloat::StorageType ret = (a._num * b._num) >> FRACTION_BITS;
	if (ret > FixedPointFloat::c_intMask)
	{
		ret = (FixedPointFloat::GetIntPart(ret) << FRACTION_BITS) +
		FixedPointFloat::GetFracPart(ret);
	}
	return ret;
#else
	bool isLhsNegative = a._num < 0;
	bool isRhsNegative = b._num < 0;
	FixedPointFloat::CalcType lhs = (FixedPointFloat::CalcType)(isLhsNegative ? (-a._num) : a._num);
	FixedPointFloat::CalcType rhs = (FixedPointFloat::CalcType)(isRhsNegative ? (-b._num) : b._num);
	FixedPointFloat::CalcType lhs_int = FixedPointFloat::GetIntPart(lhs);
	FixedPointFloat::CalcType lhs_frac = FixedPointFloat::GetFracPart(lhs);
	FixedPointFloat::CalcType rhs_int = FixedPointFloat::GetIntPart(rhs);
	FixedPointFloat::CalcType rhs_frac = FixedPointFloat::GetFracPart(rhs);

	FixedPointFloat::CalcType integer = (lhs_int * rhs_int) & FixedPointFloat::c_intMask;
	FixedPointFloat::CalcType fraction =
		lhs_int * rhs_frac +
		lhs_frac * rhs_int +
		((lhs_frac * rhs_frac) >> FRACTION_BITS);

	FixedPointFloat::CalcType result = (integer << FRACTION_BITS);
	result = result + fraction;

	FixedPointFloat::StorageType ret = (FixedPointFloat::StorageType)result;
	if ((isLhsNegative && !isRhsNegative) || (!isLhsNegative && isRhsNegative))
	{
		ret = -ret;
	}
	return FixedPointFloat(ret);
#endif
}

FixedPointFloat& FixedPointFloat::operator *= (const FixedPointFloat& x)
{
#ifdef USE_NEW_FIXPOINT_MULT
	_num = (_num * x._num) >> FRACTION_BITS;
	if (_num > FixedPointFloat::c_intMask)
	{
		_num = (FixedPointFloat::GetIntPart(_num) << FRACTION_BITS) +
			FixedPointFloat::GetFracPart(_num);
	}
	RefreshShowValue();
	return *this;
#else
	FixedPointFloat result = (*this) * x;
	_num = result._num;
	RefreshShowValue();
	return (*this);
#endif
}

FixedPointFloat operator / (const FixedPointFloat& a, const FixedPointFloat& b)
{
	return FixedPointFloat(((a._num) << FRACTION_BITS) / (b._num));
}

FixedPointFloat& FixedPointFloat::operator /= (const FixedPointFloat& x)
{
	_num = (_num << FRACTION_BITS) / x._num;
	RefreshShowValue();
	return (*this);
}

FixedPointFloat::operator int() const
{
	return (int)(_num >> FRACTION_BITS);
}

FixedPointFloat::operator double() const
{
	return (double)_num / (double)c_one;
}

FixedPointFloat::operator float() const
{
	return (float)((double)_num / (double)c_one);
}

FixedPointFloat FixedPointFloat::Sqrt() const
{
	CalcType root, remHi, remLo, testDiv, count;
	root = 0; /* Clear root */
	remHi = 0; /* Clear high part of partial remainder */
	remLo = (CalcType)(_num); /* Get argument into low part of partial remainder */
	count = 31 + (FRACTION_BITS >> 1); /* Load loop counter */
	do
	{
		remHi = (remHi << 2) | (remLo >> 62); remLo <<= 2; /* get 2 bits of arg */
		root <<= 1; /* Get ready for the next bit in the root */
		testDiv = (root << 1) + 1; /* Test radical */
		if (remHi >= testDiv)
		{
			remHi -= testDiv;
			root += 1;
		}
	} while (count-- != 0);

	return FixedPointFloat((StorageType)root);
}

FixedPointFloat FixedPointFloat::Abs() const
{
	return FixedPointFloat(_num < 0 ? -_num : _num);
}

FixedPointFloat FixedPointFloat::Sin() const
{
	bool isNegative = _num < 0;
	CalcType ang = (CalcType)(isNegative ? (-_num) : _num);
	ang = ang % TwoPI._num;
	if (ang > (CalcType)(PI._num))
	{
		ang = TwoPI._num - ang;
		isNegative = !isNegative;
	}
	if (ang > (CalcType)(HalfPI._num))
		ang = PI._num - ang;
	unsigned int idx = (unsigned int)(ang >> 7);
	if (ang % 128 >= 64)
		idx++;
	StorageType ret;
	if (idx < c_sinTableCount)
		ret = (StorageType)(c_sinTable[idx]);
	else
		ret = (StorageType)c_one;
	if (isNegative)
		ret = -ret;
	return FixedPointFloat(ret);
}

FixedPointFloat FixedPointFloat::Asin() const
{
	bool isNegative = _num < 0;
	CalcType sin_val = (CalcType)(isNegative ? (-_num) : _num);
	/*
	if (sin_val > c_one)
	{
		return 0;
	}
	*/
	unsigned int idx = (unsigned int)(sin_val >> 6);
	if (sin_val % 64 >= 32)
		idx++;
	StorageType ret;
	if (idx < c_asinTableCount)
		ret = (StorageType)(c_asinTable[idx]) << 1;
	else
		ret = HalfPI._num;
	if (isNegative)
		ret = -ret;
	return FixedPointFloat(ret);
}

FixedPointFloat FixedPointFloat::Atan() const
{
	FixedPointFloat tmp = (*this)*(*this);
	tmp._num += c_one;
	return ((*this) / tmp.Sqrt()).Asin();
}

FixedPointFloat& FixedPointFloat::ceil()
{
	int64_t fractionPart = _num & c_fracMask;

	// 判断小数部分
	if (fractionPart > 0)
	{
		_num += c_one;
	}

	// 对_num取整
	_num = _num >> FRACTION_BITS << FRACTION_BITS;

	return (*this);
}

FixedPointFloat f1("23.124");
FixedPointFloat f2("-3.4392");

TEST(FixedPointFloat, testAdd)
{
	FixedPointFloat f3("1.204");
	auto s = Port_GetTimeOfDay();
	for (int i = 0; i < 20000000; ++i)
	{
		f3 /= f1*f2;
	}
	auto cost = Port_GetTimeOfDay() - s;

	DEBUG_LOG << "cost:" << cost << " f3:" << double(f3);
}


TEST(FixedPointFloat, testMinus)
{

	FixedPointFloat f3;
	for (int i = 0; i < 20000000; ++i)
	{
		f3 = f1 - f2;
	}
}

//TEST(FixedPointFloat, testMulti)
//{
//	VFactor f1(23124L, 1000L);
//	VFactor f2(34392L, 10000L);
//
//	VFactor f3(0, 0);
//	for (int i = 0; i < 20000000; ++i)
//	{
//		f3 = f1 * f2;
//	}
//}
//
//TEST(FixedPointFloat, testDivid)
//{
//	VFactor f1(23124L, 1000L);
//	VFactor f2(34392L, 10000L);
//
//	VFactor f3(0, 0);
//	for (int i = 0; i < 20000000; ++i)
//	{
//		f3 = f1 / f2;
//	}
//}
