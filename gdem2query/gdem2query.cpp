#include "gdem2query.h"
#include <io.h>
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <math.h>

#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

#define _HEADER_SIZE 8
#define _MAX_FILENAME 12
#define _MAX_PATH 260
#define _GDEM2_PATH_PREFIX "C:\\Users\\iclosure\\Documents\\workspace\\GDEM2"
#define __max(a,b) (((a) > (b)) ? (a) : (b))
#define __min(a,b) (((a) < (b)) ? (a) : (b))
#define _PRECISION (100)
#define _PRECISION_SCALE (0.01)
const gdem2_uint32 _MATRIX_SIZE = ((gdem2_uint32)((3600UL / (_PRECISION * _PRECISION_SCALE) + 1UL)));

int gdem2_gen_filename(char *name, size_t name_size, double lat, double lon)
{
	char buffer[10], *result = 0;
	int length = 0;
	gdem2_uint32 u_lon = 0, u_lat = 0;

	if (0 == name || _MAX_FILENAME != name_size) {
		return -1;
	}

	// - longitude

	name[0] = (lat > 0 ? 'N' : 'S');

	u_lat = (gdem2_uint32)floor(fabs(lat));

	memset(buffer, 0, sizeof(buffer));
	result = ultoa(u_lat, buffer, 10);
	if (result == 0) {
		return -1;
	}

	length = strlen(result);
	if (length > 2) {
		length = 2;
	}
	memset(name + 1, '0', 2 - length);
	memcpy(name + 1 + (2 - length), result, length);

	// - latitude

	name[3] = (lon > 0 ? 'E' : 'W');

	u_lon = (gdem2_uint32)floor(fabs(lon));

	memset(buffer, 0, sizeof(buffer));
	result = ultoa(u_lon, buffer, 10);
	if (result == 0) {
		return -1;
	}

	length = strlen(result);
	if (length > 3) {
		length = 3;
	}

	memset(name + 4, '0', 3 - length);
	memcpy(name + 4 + (3 - length), result, length);

	// suffix
	memcpy(name + 7, ".dat", 4);

	return 0;
}

int gdem2_gen_filepath(char *filepath, size_t filepath_size, double lat, double lon)
{
	char filename[_MAX_FILENAME];
	int length = 0;

	if (0 == filepath || _MAX_PATH > filepath_size) {
		return -1;
	}

	length = strlen(_GDEM2_PATH_PREFIX);
	if (length > _MAX_PATH - _MAX_FILENAME - 1) { // -1 mean suffix '\\'
		return -1;
	}

	memset(filename, 0, sizeof(filename));

	if (0 != gdem2_gen_filename(filename, sizeof(filename), lat, lon)) {
		return -1;
	}

	memcpy(filepath, _GDEM2_PATH_PREFIX, length);

	if (filepath[length - 1] != '\\') {
		filepath[length] = '\\';
		memcpy(filepath + length + 1, filename, sizeof(filename));
	}
	else {
		memcpy(filepath + length, filename, sizeof(filename));
	}

	return 0;
}

int gdem2_get_pos(double lat, double lon, double *dot_lat, double *dot_lon)
{
	double fabs_lat = 0.0, fabs_lon = 0.0;

	if (0 == dot_lat || 0 == dot_lon) {
		return -1;
	}

	fabs_lat = fabs(lat);
	fabs_lon = fabs(lon);
	*dot_lat = fabs_lat - ((gdem2_uint32)floor(fabs_lat));
	*dot_lon = fabs_lon - ((gdem2_uint32)floor(fabs_lon));

	return 0;
}

gdem2_int16 gdem2_query_point(double lat, double lon)
{
	char filepath[_MAX_PATH];
	double dot_lat = 0.0, dot_lon = 0.0;
	gdem2_uint32 offset = 0, height = 0, x = 0, y = 0, line_count = 0;
	char buffer[2];
	size_t fsize = 0;

	memset(filepath, 0, sizeof(filepath));

	if (-1 == gdem2_gen_filepath(filepath, sizeof(filepath), lat, lon)) {
		return 0;
	}

	if (-1 == gdem2_get_pos(lat, lon, &dot_lat, &dot_lon)) {
		return 0;
	}

	FILE *fp = fopen(filepath, "rb");
	if (0 == fp) {
		return 0;
	}

	x = (gdem2_uint32)(dot_lon * (_MATRIX_SIZE - 1));
	if (x >= _MATRIX_SIZE) {
		x = _MATRIX_SIZE - 1;
	}

	y = (gdem2_uint32)((1 - dot_lat) * (_MATRIX_SIZE - 1));
	if (y >= _MATRIX_SIZE) {
		y = _MATRIX_SIZE - 1;
	}

	offset = _HEADER_SIZE + (x + y * _MATRIX_SIZE) * 2;

	if (-1 == fseek(fp, offset, SEEK_SET)) {
		fclose(fp);
		return 0;
	}

	fsize = fread(buffer, sizeof(char), 2, fp);
	if (2 != fsize) {
		fclose(fp);
		return 0;
	}
	
	height = *(gdem2_int16*)buffer;

	fclose(fp);

	return height;
}
