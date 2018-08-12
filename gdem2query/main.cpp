
#include "gdem2query.h"
#include <stdio.h>

int main(int argc, char **argv)
{
	//int height = gdem2_query_point(30.73361111111111, 103.98);  // [1]
	//int height = gdem2_query_point(30.71361111111111, 104.00277777777778); // [2]
	int height = gdem2_query_point(27.988055555555555, 86.92527777777778);	// 珠穆朗玛峰
    //int height = gdem2_query_point(27.9890700296, 86.9254364904);				// 珠峰

	// 中国衡阳市衡东县衡山海拔高度多少米
	// 经度 : 112.887631, 纬度 : 27.21841
	// 海拔 : 94米, 气压 : 100KPa
	//int height = gdem2_query_point(27.21841, 112.887631);

	//int height = gdem2_query_point(29.595555555555556, 101.8788888888889);	// 贡嘎山

	return 0;
}
