#include "geo_hash.h"

#include <gflags/gflags.h>
#include <glog/logging.h>

namespace {
// Normal 32 characer map used for geohashing.
const char base32_codes[] = "0123456789bcdefghjkmnpqrstuvwxyz";
const int base32_indexes[] = {
    0,  1,  2,  3,  4,  5,  6,  7,   // 30-37, '0'..'7'
    8,  9,  -1, -1, -1, -1, -1, -1,  // 38-2F, '8','9'
    -1, -1, 10, 11, 12, 13, 14, 15,  // 40-47, 'B'..'G'
    16, -1, 17, 18, -1, 19, 20, -1,  // 48-4F, 'H','J','K','M','N'
    21, 22, 23, 24, 25, 26, 27, 28,  // 50-57, 'P'..'W'
    29, 30, 31, -1, -1, -1, -1, -1,  // 58-5F, 'X'..'Z'
    -1, -1, 10, 11, 12, 13, 14, 15,  // 60-67, 'b'..'g'
    16, -1, 17, 18, -1, 19, 20, -1,  // 68-6F, 'h','j','k','m','n'
    21, 22, 23, 24, 25, 26, 27, 28,  // 70-77, 'p'..'w'
    29, 30, 31,                      // 78-7A, 'x'..'z'
};
const int bits[] = {16, 8, 4, 2, 1};
}  // namespace

namespace util {

bool GeoHash::Encode(double latitude,
                     double longitude,
                     uint32 precision,
                     std::string* geohash) {
  if (geohash == NULL) {
    return false;
  }
  // Check basic arguments sanity.
  if (precision > 32 || precision == 0) {
    return false;
  }
  // Return an error when trying to index outside the supported
  // constraints.
  if (longitude > 180.0 || longitude < -180.0 || latitude > 90.0 ||
      latitude < -90.0) {
    return false;
  }

  Interval lat_interval = {-90.0, 90.0};
  Interval lon_interval = {-180.0, 180.0};
  bool is_even = true;
  int32 bit = 0;
  int32 ch = 0;

  while ((*geohash).size() < precision) {
    if (is_even) {
      double mid = lon_interval.mid();
      if (longitude > mid) {
        ch |= bits[bit];
        lon_interval.low = mid;
      } else {
        lon_interval.high = mid;
      }
    } else {
      double mid = lat_interval.mid();
      if (latitude > mid) {
        ch |= bits[bit];
        lat_interval.low = mid;
      } else {
        lat_interval.high = mid;
      }
    }
    is_even = !is_even;
    if (bit < 4) {
      ++bit;
    } else {
      (*geohash) += base32_codes[ch];
      ch = 0;
      bit = 0;
    }
  }

  return true;
}

bool GeoHash::Encode(const GeoLocation& geo,
                     uint32 precision,
                     std::string* geohash) {
  return Encode(geo.latitude, geo.longitude, precision, geohash);
}

bool GeoHash::Decode(const std::string& geohash,
                     double* latitude,
                     double* longitude) {
  if (latitude == NULL || longitude == NULL) {
    return false;
  }

  Interval lat_interval = {-90.0, 90.0};
  Interval lon_interval = {-180.0, 180.0};
  double lat_err = 90.0;
  double lon_err = 180.0;
  bool is_even = true;

  for (uint32 i = 0; i < geohash.length(); ++i) {
    char c = geohash[i];
    int32 cd = base32_indexes[c - 48];
    for (int j = 0; j < 5; ++j) {
      if (is_even) {
        lon_err /= 2;
        double mid = lon_interval.mid();
        if (cd & bits[j]) {
          lon_interval.low = mid;
        } else {
          lon_interval.high = mid;
        }
      } else {
        lat_err /= 2;
        double mid = lat_interval.mid();
        if (cd & bits[j]) {
          lat_interval.low = mid;
        } else {
          lat_interval.high = mid;
        }
      }
      is_even = !is_even;
    }
  }

  *latitude = lat_interval.mid();
  *longitude = lon_interval.mid();

  return true;
}

bool GeoHash::Decode(const std::string& geohash, GeoLocation* geo) {
  if (geo == NULL) {
    return false;
  }
  return Decode(geohash, &geo->latitude, &geo->longitude);
}

}  // namespace util
