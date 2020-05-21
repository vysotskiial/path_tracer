#include <gtest/gtest.h>
#include "object.h"
#include <cmath>

TEST(test, triangle_has_intersection)
{
	Triangle t({-1, 1, 0}, {1, 1, 0}, {0, -1, 0});
	Ray r = {{3, 3, 3}, {-1, -1, -1}};

	double time;
	auto intersection = t.intersect(r, time);
	EXPECT_DOUBLE_EQ(time, 3.);
}

TEST(test, sphere_has_intersection)
{
	Sphere s(vec3(), 1);
	Ray r = {{3, 3, 3}, {-1, -1, -1}};

	double time;
	auto intersection = s.intersect(r, time);
	EXPECT_TRUE(time);
	EXPECT_DOUBLE_EQ(intersection.start.x, std::sqrt(1. / 3));
	EXPECT_DOUBLE_EQ(intersection.start.y, std::sqrt(1. / 3));
	EXPECT_DOUBLE_EQ(intersection.start.z, std::sqrt(1. / 3));
}

TEST(test, sphere_no_intersection)
{
	Sphere s(vec3(), 1);
	Ray r = {{3, 3, 3}, {1, 1, 1}};

	double time;
	[[maybe_unused]] auto intersection = s.intersect(r, time);
	EXPECT_FALSE(time);
}

TEST(test, sphere_inside)
{
	Sphere s(vec3(), 1);
	Ray r = {{0, 0, 0}, {-1, 1, -1}};

	double time;
	auto intersection = s.intersect(r, time);
	EXPECT_TRUE(time);
	EXPECT_DOUBLE_EQ(intersection.start.x, -std::sqrt(1. / 3));
	EXPECT_DOUBLE_EQ(intersection.start.y, std::sqrt(1. / 3));
	EXPECT_DOUBLE_EQ(intersection.start.z, -std::sqrt(1. / 3));
}

TEST(test, vector_prod)
{
	vec3 v1{1., 2., 3.};
	vec3 v2{4., 5., 6.};

	vec3 prod = vector_prod(v1, v2);

	EXPECT_DOUBLE_EQ(prod * v1, 0.);
	EXPECT_DOUBLE_EQ(prod * v2, 0.);
}

TEST(test, determinant)
{
	vec3 col1{1., 2., 3.};
	vec3 col2{4., 5., 6.};
	vec3 col3{7., 8., 9.};

	EXPECT_DOUBLE_EQ(det(col1, col2, col3), 0.);
	col3.y = 33;
	EXPECT_DOUBLE_EQ(det(col1, col2, col3), 150.);
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
