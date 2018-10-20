#include <string>
#include <unordered_map>

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <gtest/gtest.h>

#include "geo_hash.h"

using util::GeoHash;

namespace util {

class GeoHashTest : public testing::Test {
 protected:
  static void SetUpTestCase();
  static void TearDownTestCase();
  static double round(double src, int precision);
  static int32 geo_hash_prcision;
  static int32 decimal_point_prcision;
  static std::unordered_map<std::string, GeoHash::GeoLocation> test_geohashs;
};

int32 GeoHashTest::geo_hash_prcision = 12;
int32 GeoHashTest::decimal_point_prcision = 6;

std::unordered_map<std::string, GeoHash::GeoLocation>
    GeoHashTest::test_geohashs = {
        {"9q8yt0hwpd6d", {37.70612055, -122.4690807}},
        {"9q9p3tzpunce", {37.869867, -122.268045}},
};

void GeoHashTest::SetUpTestCase() {}

void GeoHashTest::TearDownTestCase() {}

double GeoHashTest::round(double src, int precision = 6) {
  std::stringstream ss;
  ss << std::fixed << std::setprecision(precision) << src;
  ss >> src;
  return src;
}

TEST_F(GeoHashTest, EncodeV1) {
  for (const auto& pair : test_geohashs) {
    std::string geohash_str;
    GeoHash::Encode(pair.second.latitude,
                    pair.second.longitude,
                    geo_hash_prcision,
                    &geohash_str);
    EXPECT_STREQ(geohash_str.c_str(), pair.first.c_str());
  }
}

TEST_F(GeoHashTest, EncodeV2) {
  for (const auto& pair : test_geohashs) {
    std::string geohash_str;
    GeoHash::GeoLocation geo = {pair.second.latitude, pair.second.longitude};
    GeoHash::Encode(geo, geo_hash_prcision, &geohash_str);
    EXPECT_STREQ(geohash_str.c_str(), pair.first.c_str());
  }
}

TEST_F(GeoHashTest, DecodeV1) {
  for (const auto& pair : test_geohashs) {
    GeoHash::GeoLocation geo;
    GeoHash::Decode(pair.first, &geo);
    EXPECT_EQ(round(geo.longitude), round(pair.second.longitude));
    EXPECT_EQ(round(geo.latitude), round(pair.second.latitude));
  }
}

TEST_F(GeoHashTest, DecodeV2) {
  for (const auto& pair : test_geohashs) {
    double latitude;
    double longitude;
    GeoHash::Decode(pair.first, &latitude, &longitude);
    EXPECT_EQ(round(longitude), round(pair.second.longitude));
    EXPECT_EQ(round(latitude), round(pair.second.latitude));
  }
}

}  // namespace util

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
