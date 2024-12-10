#include "catch2/catch.hpp"
#include "hiccup/storage/storage.h"
#include <cmath>
#include <limits>

namespace {
    bool IsDoublesEqual(double a, double b) {
        double epsilon = std::numeric_limits<double>::epsilon();
        return std::abs(a - b) < epsilon;
    }
}

TEST_CASE( "Test Storage Charge" ) {
    Storage s1{1024, ST_BLOCK_STORAGE};
    Storage s2{512, ST_FILE_STORAGE};
    Storage s3{128, ST_OBJECT_STORAGE};

    Lease l1{3,  &s1};
    Lease l2{4,  &s2};
    Lease l3{14, &s3};

    Tenant tenant;

    tenant.numOfLeases = 0;
    tenant.leases[tenant.numOfLeases++] = &l1;
    tenant.leases[tenant.numOfLeases++] = &l2;
    tenant.leases[tenant.numOfLeases++] = &l3;

    double total = 0.0;
    int levels = 0;
    charge(&tenant, &total, &levels);

    REQUIRE(IsDoublesEqual(total, 2185.0));
    REQUIRE(levels == 1);
}
