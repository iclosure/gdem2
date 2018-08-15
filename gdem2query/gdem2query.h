#ifndef GDEM2QUERY_H
#define GDEM2QUERY_H

#ifndef _GDEM2_TYPE_DEF
#define _GDEM2_TYPE_DEF

typedef short gdem2_int16;
typedef unsigned short gdem2_uint16;
typedef int gdem2_int32;
typedef unsigned int gdem2_uint32;

#endif // _GDEM2_TYPE_DEF

//
gdem2_int16 gdem2_query_point(double lat, double lon);

#endif
