#include "gtest/gtest.h"

extern "C" {
#include "stdint.h"
#include "Vec.h"
}

/**
 * The purpose of these tests is to prove correctness of the
 * abstraction barrier from the user's point-of-view. You should
 * not access any member of the Vec directly.
 */

TEST(VecSpec, values_init_empty) {
    Vec v = Vec_value(10, sizeof(double));
    ASSERT_EQ(0, Vec_length(&v));
    Vec_drop(&v);
}

TEST(VecSpec, get_set_contract) {
    Vec v = Vec_value(1, sizeof(int16_t));
    int16_t x = 211;
    int16_t y = 311;
    ASSERT_EQ(0, Vec_length(&v));
    Vec_set(&v, 0, &x);
    ASSERT_EQ(1, Vec_length(&v));
    Vec_set(&v, 1, &y);
    ASSERT_EQ(2, Vec_length(&v));

    int16_t x_out = 0;
    int16_t y_out = 0;
    Vec_get(&v, 0, &x_out);
    ASSERT_EQ(x, x_out);
    Vec_get(&v, 1, &y_out);
    ASSERT_EQ(y, y_out);
}

TEST(VecSpec, set_based_equal) {
	Vec v1 = Vec_value(1, sizeof(int64_t));
	Vec v2 = Vec_value(1, sizeof(int64_t));

	int64_t a = 1;
	int64_t b = 32;
	int64_t c = 44;

	Vec_set(&v1, 0, &a);
	ASSERT_EQ(1, Vec_length(&v1));
	Vec_set(&v1, 1, &b);
	ASSERT_EQ(2, Vec_length(&v1));
	Vec_set(&v1, 2, &c);
	ASSERT_EQ(3, Vec_length(&v1));
	
	Vec_set(&v2, 0, &c);
	ASSERT_EQ(1, Vec_length(&v2));
	Vec_set(&v2, 1, &c);
	ASSERT_EQ(2, Vec_length(&v2));
	Vec_set(&v2, 2, &c);
	ASSERT_EQ(3, Vec_length(&v2));
	Vec_set(&v2, 1, &b);
	ASSERT_EQ(3, Vec_length(&v2));
	Vec_set(&v2, 0, &a);
	ASSERT_EQ(3, Vec_length(&v2));
	
	int64_t a1_out = 0;
	int64_t a2_out = 0;
	int64_t b1_out = 0;
	int64_t b2_out = 0;
	int64_t c1_out = 0;
	int64_t c2_out = 0;
	Vec_get(&v1, 0, &a1_out);
	Vec_get(&v1, 1, &b1_out);
	Vec_get(&v1, 2, &c1_out);
	Vec_get(&v2, 0, &a2_out);
	Vec_get(&v2, 1, &b2_out);
	Vec_get(&v2, 2, &c2_out);
	ASSERT_EQ(a1_out, a2_out);
	ASSERT_EQ(b1_out, b2_out);
	ASSERT_EQ(c1_out, c2_out);

	ASSERT_TRUE(Vec_equals(&v1, &v2));
	
	Vec_drop(&v1);
	Vec_drop(&v2);
}

TEST(VecSpec, set_based_not_equal) {
	Vec v1 = Vec_value(1, sizeof(int));
	Vec v2 = Vec_value(1, sizeof(int));

	int a = 10;
	int b = 20;
	int c = 30;
	Vec_set(&v1, 0, &a);
	Vec_set(&v1, 1, &b);
	Vec_set(&v2, 0, &a);
	Vec_set(&v2, 1, &c);
	
	ASSERT_FALSE(Vec_equals(&v1, &v2));
	
	Vec_drop(&v1);
	Vec_drop(&v2);
}

TEST(VecSpec, splice_based_equal) {
	Vec v1 = Vec_value(1, sizeof(int));
	Vec v2 = Vec_value(1, sizeof(int));

	int a = 10;
	Vec_set(&v1, 0, &a);
	
	int items[] = {100, 200, 300, 400};
	Vec_splice(&v1, 0, 1, items, 4);
	Vec_splice(&v2, 0, 0, items, 4);

	ASSERT_TRUE(Vec_equals(&v1, &v2));
	
	Vec_drop(&v1);
	Vec_drop(&v2);
}

TEST(VecSpec, set_splice_equal) {
	Vec v1 = Vec_value(1, sizeof(int));
	Vec v2 = Vec_value(1, sizeof(int));

	int a = 100;
	int b = 200;
	int c = 300;
	int d = 400;
	Vec_set(&v1, 0, &a);
	Vec_set(&v1, 1, &b);
	Vec_set(&v1, 2, &c);
	Vec_set(&v1, 3, &d);
	
	int items[] = {100, 200, 300, 400};
	Vec_splice(&v2, 0, 0, items, 4);

	ASSERT_TRUE(Vec_equals(&v1, &v2));
	
	Vec_drop(&v1);
	Vec_drop(&v2);
}

TEST(VecSpec, splice_empty) {
	Vec v1 = Vec_value(1, sizeof(int));
	Vec v2 = Vec_value(1, sizeof(int));

	int a = 100;
	int b = 200;
	int c = 300;
	int d = 400;
	Vec_set(&v1, 0, &a);
	Vec_set(&v1, 1, &b);
	Vec_set(&v1, 2, &c);
	Vec_set(&v1, 3, &d);
	
	int items[] = {100, 200, 300, 400};
	Vec_splice(&v1, 0, 4, items, 0);

	ASSERT_TRUE(Vec_equals(&v1, &v2));
	
	Vec_drop(&v1);
	Vec_drop(&v2);
}
