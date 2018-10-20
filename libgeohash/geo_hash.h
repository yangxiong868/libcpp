#ifndef GEO_HASH_H_
#define GEO_HASH_H_

#include <string>

typedef int32_t int32;
typedef uint32_t uint32;

namespace util {
class GeoHash {
 public:
  struct GeoLocation {
    double latitude;
    double longitude;
  };
  static bool Encode(double latitude,
                     double longitude,
                     uint32 precision,
                     std::string* geohash);

  static bool Encode(const GeoLocation& geo,
                     uint32 precision,
                     std::string* geohash);

  static bool Decode(const std::string& geohash,
                     double* latitude,
                     double* longitude);

  static bool Decode(const std::string& geohash, GeoLocation* geo);

 private:
  typedef struct IntervalStruct {
    double low;
    double high;

    double mid() { return (low + high) / 2; }
  } Interval;
};
}  // namespace util

#endif
