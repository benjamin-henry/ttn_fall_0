#include "gps_coords.h"
#include <stdlib.h>
#include <stdint.h>

gps_coords gps_coords_init() {
    gps_coords coords = (gps_coords)malloc(sizeof(gps_coords_t));
    coords->bytes = (unsigned char*)malloc(sizeof(unsigned char)*6);
    coords->latitude=0.;
    coords->longitude=0.;
    coords_to_bytes(coords);
    return coords;
}


void set_gps_coords(gps_coords coords, float latitude, float longitude) {
    coords->latitude=latitude;
    coords->longitude=longitude;
    coords_to_bytes(coords);
}

void coords_to_bytes(gps_coords coords) {
    float latf = (coords->latitude+180.) * 10000.;
    float longf = (coords->longitude+180.) * 10000.;
    uint32_t lat_i = (uint32_t)latf;
    uint32_t long_i = (uint32_t)longf;
    coords->bytes[0] = 0xFF & (lat_i>>16);
    coords->bytes[1] = 0xFF & (lat_i>>8);
    coords->bytes[2] = 0xFF & lat_i;
    coords->bytes[3] = 0xFF & (long_i>>16);
    coords->bytes[4] = 0xFF & (long_i>>8);
    coords->bytes[5] = 0xFF & long_i;
}
