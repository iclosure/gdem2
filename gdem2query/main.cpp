
#include "gdem2query.h"
#include <stdio.h>

int main(int argc, char **argv)
{
	int height = gdem2_query_point(30.5763307666, 104.0712219292);
	//int height = gdem2_query_point(30.71361111111111, 104.00277777777778);

	return 0;
}