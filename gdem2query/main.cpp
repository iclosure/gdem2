
#include "gdem2query.h"
#include <stdio.h>

int main(int argc, char **argv)
{
	//gdem2_int16 height = gdem2_query_point(30.73361111111111, 103.98);  // [1]
	//gdem2_int16 height = gdem2_query_point(30.71361111111111, 104.00277777777778); // [2]
	//gdem2_int16 height = gdem2_query_point(27.988055555555555, 86.92527777777778);	// 珠穆朗玛峰
    //gdem2_int16 height = gdem2_query_point(27.9890700296, 86.9254364904);				// 珠峰

	// 中国衡阳市衡东县衡山海拔高度多少米
	// 经度 : 112.887631, 纬度 : 27.21841
	// 海拔 : 94米, 气压 : 100KPa
	//gdem2_int16 height = gdem2_query_point(27.21841, 112.887631);

	//gdem2_int16 height = gdem2_query_point(27.9996173242, 88.0008217720);	// 5740
	//gdem2_int16 height = gdem2_query_point(27.9999997039, 88.0000007543);	// 5470.042
	gdem2_int16 height = gdem2_query_point(27.2011427135, 88.4628411742);	// 

	return 0;
}
