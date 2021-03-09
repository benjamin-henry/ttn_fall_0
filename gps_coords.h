#ifndef GPS_COORDS_H
#define GPS_COORDS_H

#include "stdlib.h"

#ifndef __cplusplus
extern "C" {
#endif


typedef struct gps_coords_t {
  float latitude;
  float longitude;
  unsigned char * bytes;
};
typedef gps_coords_t* gps_coords;

gps_coords gps_coords_init();
void set_gps_coords(gps_coords coords, float latitude, float longitude);
void coords_to_bytes(gps_coords coords);

#ifndef __cplusplus
}
#endif

#endif
