#include "gtest/gtest.h"

extern "C" {
#include "stdint.h"
#include "Vec.h"
}

/*
 * Unit tests established during the implementation of Vec.c
 * These tests are permitted to reach through the abstraction
 * and access members of a Vec struct directly.
 */

TEST(VecImpl, value) {
    Vec v = Vec_value(2, sizeof(int16_t));
    ASSERT_EQ(0, v.length);
    ASSERT_EQ(2, v.capacity);
    ASSERT_STREQ((char*)v.buffer, "");
    Vec_drop(&v);
}

TEST(VecImpl, drop) {
    Vec v = Vec_value(2, sizeof(int16_t));
    Vec_drop(&v);
    ASSERT_EQ(0, v.length);
    ASSERT_EQ(0, v.capacity);
    ASSERT_EQ(nullptr, v.buffer);
}

TEST(VecImpl, ref) {
    Vec v = Vec_value(2, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 1;
    buffer[1] = 2;
    v.length = 2;
    ASSERT_EQ(&buffer[0], Vec_ref(&v, 0));
    ASSERT_EQ(&buffer[1], Vec_ref(&v, 1));
    Vec_drop(&v);
}

TEST(VecImpl, ref_out_of_bounds) {
    Vec v = Vec_value(4, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    buffer[2] = 300;
    v.length = 3;
    ASSERT_DEATH({
       Vec_ref(&v, 3);
    }, ".* - Out of Bounds");
    Vec_drop(&v);
}

TEST(VecImpl, get_basic) {
    	Vec v = Vec_value(1, sizeof(int16_t));
    	int16_t *buffer = (int16_t*)v.buffer;
    	buffer[0] = 1;
    	v.length = 1;

    	int16_t value = 0;
    	Vec_get(&v, 0, &value);
    	ASSERT_EQ(1, value);

    	Vec_drop(&v);
}

TEST(VecImpl, get_3_items) {
	Vec v = Vec_value(3, sizeof(int16_t));
	int16_t *buffer = (int16_t*)v.buffer;
	buffer[0] = 1;
	buffer[1] = 25;
	buffer[2] = 33;
	v.length = 3;

	int16_t value = 0;
	Vec_get(&v, 0, &value);
	ASSERT_EQ(1, value);
	Vec_get(&v, 1, &value);
	ASSERT_EQ(25, value);
	Vec_get(&v, 2, &value);
	ASSERT_EQ(33, value);

	Vec_drop(&v);
}

TEST(VecImpl, get_out_of_bounds) {
	Vec v = Vec_value(3, sizeof(int16_t));
	int16_t *buffer = (int16_t*)v.buffer;
	buffer[0] = 1;
	buffer[1] = 25;
	buffer[2] = 33;
	v.length = 3;

	int16_t value = 0;
    	ASSERT_DEATH({
       		Vec_get(&v, 3, &value);
    	}, ".* - Out of Bounds");
	ASSERT_DEATH({
		Vec_get(&v, -1, &value);
	}, ".* - Out of Bounds");

    	Vec_drop(&v);
}

TEST(VecImpl, get_from_empty) {
    	Vec v = Vec_value(1, sizeof(int16_t));

    	int16_t value = 0;
    	ASSERT_DEATH({
       		Vec_get(&v, 0, &value);
    	}, ".* - Out of Bounds");

    	Vec_drop(&v);
}

TEST(VecImpl, set_basic) {
	Vec v = Vec_value(1, sizeof(int16_t));
    	ASSERT_EQ(0, v.length);

    	int16_t x = 1;
    	Vec_set(&v, 0, &x);
    	ASSERT_EQ(1, v.length);

    	int16_t *buffer = (int16_t*)v.buffer;
    	ASSERT_EQ(x, buffer[0]);

    	Vec_drop(&v);
}

TEST(VecImpl, set_multiple_and_expand_capacity) {
    	Vec v = Vec_value(1, sizeof(int16_t));
    	ASSERT_EQ(0, v.length);

    	int16_t x = 1;
    	Vec_set(&v, 0, &x);
    	ASSERT_EQ(1, v.length);

    	int16_t y = 2;
    	Vec_set(&v, 1, &y);
    	ASSERT_EQ(2, v.length);
    
    	int16_t *buffer = (int16_t*)v.buffer;
    	ASSERT_EQ(x, buffer[0]);
    	ASSERT_EQ(y, buffer[1]);

    	Vec_drop(&v);
}

TEST(VecImpl, set_change_item) {
	Vec v = Vec_value(3, sizeof(int16_t));
	int16_t *buffer = (int16_t*)v.buffer;
	buffer[0] = 1;
	buffer[1] = 25;
	buffer[2] = 33;
	v.length = 3;

    	ASSERT_EQ(3, v.length);	
    	ASSERT_EQ(1, buffer[0]);
    	ASSERT_EQ(25, buffer[1]);
    	ASSERT_EQ(33, buffer[2]);
    
    	int16_t x = 5;
    	Vec_set(&v, 1, &x);

    	ASSERT_EQ(3, v.length);	
    	ASSERT_EQ(1, buffer[0]);
    	ASSERT_EQ(5, buffer[1]);
    	ASSERT_EQ(33, buffer[2]);

    	Vec_drop(&v);
}

TEST(VecImpl, set_out_of_bounds) {
	Vec v = Vec_value(3, sizeof(int16_t));
	int16_t *buffer = (int16_t*)v.buffer;
	buffer[0] = 1;
	buffer[1] = 25;
	buffer[2] = 33;
	v.length = 3;
    
	int16_t value = 0;
    	ASSERT_DEATH({
       		Vec_set(&v, 4, &value);
    	}, ".* - Out of Bounds");
	ASSERT_DEATH({
		Vec_set(&v, -1, &value);
	}, ".* - Out of Bounds");

    	Vec_drop(&v);
}

TEST(VecImpl, equals_empty) {
	Vec v1 = Vec_value(1, sizeof(int16_t));
	Vec v2 = Vec_value(1, sizeof(int16_t));
	
	ASSERT_TRUE(Vec_equals(&v1, &v2));

	Vec_drop(&v1);
	Vec_drop(&v2);
}

TEST(VecImpl, equals_3_items) {
	Vec v1 = Vec_value(3, sizeof(int16_t));
	int16_t *buffer1 = (int16_t*)v1.buffer;
	buffer1[0] = 1;
	buffer1[1] = 25;
	buffer1[2] = 33;
	v1.length = 3;

	Vec v2 = Vec_value(3, sizeof(int16_t));
	int16_t *buffer2 = (int16_t*)v2.buffer;
	buffer2[0] = 1;
	buffer2[1] = 25;
	buffer2[2] = 33;
	v2.length = 3;

	ASSERT_TRUE(Vec_equals(&v1, &v2));

	Vec_drop(&v1);
	Vec_drop(&v2);
}

TEST(VecImpl, equals_different_capacities) {
	Vec v1 = Vec_value(3, sizeof(int16_t));
	int16_t *buffer1 = (int16_t*)v1.buffer;
	buffer1[0] = 1;
	buffer1[1] = 25;
	buffer1[2] = 33;
	v1.length = 3;

	Vec v2 = Vec_value(4, sizeof(int16_t));
	int16_t *buffer2 = (int16_t*)v2.buffer;
	buffer2[0] = 1;
	buffer2[1] = 25;
	buffer2[2] = 33;
	v2.length = 3;

	ASSERT_TRUE(Vec_equals(&v1, &v2));

	Vec_drop(&v1);
	Vec_drop(&v2);
}

TEST(VecImpl, equals_not_equal_items) {
	Vec v1 = Vec_value(2, sizeof(int16_t));
	int16_t *buffer1 = (int16_t*)v1.buffer;
	buffer1[0] = 1;
	buffer1[1] = 25;
	v1.length = 2;

	Vec v2 = Vec_value(2, sizeof(int16_t));
	int16_t *buffer2 = (int16_t*)v2.buffer;
	buffer2[0] = 1;
	buffer2[1] = -3;
	v2.length = 2;

	ASSERT_FALSE(Vec_equals(&v1, &v2));

	Vec_drop(&v1);
	Vec_drop(&v2);
}

TEST(VecImpl, equals_different_lengths) {
	Vec v1 = Vec_value(3, sizeof(int16_t));
	int16_t *buffer1 = (int16_t*)v1.buffer;
	buffer1[0] = 1;
	buffer1[1] = 25;
	v1.length = 2;

	Vec v2 = Vec_value(3, sizeof(int16_t));
	int16_t *buffer2 = (int16_t*)v2.buffer;
	buffer2[0] = 1;
	buffer2[1] = 25;
	buffer2[2] = 33;
	v2.length = 3;

	ASSERT_FALSE(Vec_equals(&v1, &v2));

	Vec_drop(&v1);
	Vec_drop(&v2);
}

TEST(VecImpl, equals_different_item_sizes) {
	Vec v1 = Vec_value(3, sizeof(int8_t));
	int8_t *buffer1 = (int8_t*)v1.buffer;
	buffer1[0] = 1;
	buffer1[1] = 25;
	buffer1[2] = 33;
	v1.length = 3;

	Vec v2 = Vec_value(3, sizeof(int16_t));
	int16_t *buffer2 = (int16_t*)v2.buffer;
	buffer2[0] = 1;
	buffer2[1] = 25;
	buffer2[2] = 33;
	v2.length = 3;

	ASSERT_FALSE(Vec_equals(&v1, &v2));

	Vec_drop(&v1);
	Vec_drop(&v2);
}

TEST(VecImpl, splice_no_change) {
	Vec v = Vec_value(4, sizeof(int16_t));
	int16_t *buffer = (int16_t*)v.buffer;
	buffer[0] = 100;
	buffer[1] = 200;
	buffer[2] = 300;
	buffer[3] = 400;
	v.length = 4;

	int16_t a[] = {800, 900};

	Vec_splice(&v, 0, 0, a, 0);

    	ASSERT_EQ(4, v.length);	
    	ASSERT_EQ(100, buffer[0]);
    	ASSERT_EQ(200, buffer[1]);
    	ASSERT_EQ(300, buffer[2]);
	ASSERT_EQ(400, buffer[3]);

	Vec_drop(&v);
}

TEST(VecImpl, splice_negative_counts) {
	Vec v = Vec_value(4, sizeof(int16_t));
	int16_t *buffer = (int16_t*)v.buffer;
	buffer[0] = 100;
	buffer[1] = 200;
	buffer[2] = 300;
	buffer[3] = 400;
	v.length = 4;

	int16_t a[] = {800, 900};

    	ASSERT_DEATH({
		Vec_splice(&v, 0, -1, a, 0);
    	}, ".* - Out of Bounds");
	ASSERT_DEATH({
		Vec_splice(&v, 0, 0, a, -1);
	}, ".* - Out of Bounds");

	Vec_drop(&v);
}

TEST(VecImpl, splice_delete_insert_beyond_length) {
	Vec v = Vec_value(4, sizeof(int16_t));
	int16_t *buffer = (int16_t*)v.buffer;
	buffer[0] = 100;
	buffer[1] = 200;
	buffer[2] = 300;
	buffer[3] = 400;
	v.length = 4;

	int16_t a[] = {800, 900};

    	ASSERT_DEATH({
		Vec_splice(&v, 1, 4, a, 0);
    	}, ".* - Out of Bounds");
	ASSERT_DEATH({
		Vec_splice(&v, 5, 0, a, 1);
	}, ".* - Out of Bounds");

	Vec_drop(&v);
}

TEST(VecImpl, splice_delete_only) {
	Vec v = Vec_value(4, sizeof(int16_t));
	int16_t *buffer = (int16_t*)v.buffer;
	buffer[0] = 100;
	buffer[1] = 200;
	buffer[2] = 300;
	buffer[3] = 400;
	v.length = 4;

	int16_t a[] = {800, 900};

	Vec_splice(&v, 1, 2, a, 0);

    	ASSERT_EQ(2, v.length);	
    	ASSERT_EQ(100, buffer[0]);
    	ASSERT_EQ(400, buffer[1]);

	Vec_drop(&v);
}

TEST(VecImpl, splice_insert_only) {
	Vec v = Vec_value(4, sizeof(int16_t));
	int16_t *buffer = (int16_t*)v.buffer;
	buffer[0] = 100;
	buffer[1] = 200;
	buffer[2] = 300;
	buffer[3] = 400;
	v.length = 4;

	int16_t a[] = {800, 900};

	Vec_splice(&v, 2, 0, a, 2);

    	ASSERT_EQ(6, v.length);	
    	ASSERT_EQ(100, buffer[0]);
    	ASSERT_EQ(200, buffer[1]);
    	ASSERT_EQ(800, buffer[2]);
	ASSERT_EQ(900, buffer[3]);
    	ASSERT_EQ(300, buffer[4]);
	ASSERT_EQ(400, buffer[5]);

	Vec_drop(&v);
}

TEST(VecImpl, splice_insert_at_beginning) {
	Vec v = Vec_value(4, sizeof(int16_t));
	int16_t *buffer = (int16_t*)v.buffer;
	buffer[0] = 100;
	buffer[1] = 200;
	buffer[2] = 300;
	buffer[3] = 400;
	v.length = 4;

	int16_t a[] = {800, 900};

	Vec_splice(&v, 0, 0, a, 1);

    	ASSERT_EQ(5, v.length);	
    	ASSERT_EQ(800, buffer[0]);
    	ASSERT_EQ(100, buffer[1]);
    	ASSERT_EQ(200, buffer[2]);
	ASSERT_EQ(300, buffer[3]);
    	ASSERT_EQ(400, buffer[4]);

	Vec_drop(&v);
}

TEST(VecImpl, splice_insert_at_end) {
	Vec v = Vec_value(4, sizeof(int16_t));
	int16_t *buffer = (int16_t*)v.buffer;
	buffer[0] = 100;
	buffer[1] = 200;
	buffer[2] = 300;
	buffer[3] = 400;
	v.length = 4;

	int16_t a[] = {800, 900};

	Vec_splice(&v, 4, 0, a, 2);

    	ASSERT_EQ(6, v.length);	
    	ASSERT_EQ(100, buffer[0]);
    	ASSERT_EQ(200, buffer[1]);
    	ASSERT_EQ(300, buffer[2]);
	ASSERT_EQ(400, buffer[3]);
    	ASSERT_EQ(800, buffer[4]);
	ASSERT_EQ(900, buffer[5]);

	Vec_drop(&v);
}

TEST(VecImpl, splice_insert_some_delete_more) {
	Vec v = Vec_value(4, sizeof(int16_t));
	int16_t *buffer = (int16_t*)v.buffer;
	buffer[0] = 100;
	buffer[1] = 200;
	buffer[2] = 300;
	buffer[3] = 400;
	v.length = 4;

	int16_t a[] = {800, 900};

	Vec_splice(&v, 0, 3, a, 1);

    	ASSERT_EQ(2, v.length);	
    	ASSERT_EQ(800, buffer[0]);
    	ASSERT_EQ(400, buffer[1]);

	Vec_drop(&v);
}

TEST(VecImpl, splice_delete_some_insert_more) {
	Vec v = Vec_value(4, sizeof(int16_t));
	int16_t *buffer = (int16_t*)v.buffer;
	buffer[0] = 100;
	buffer[1] = 200;
	buffer[2] = 300;
	buffer[3] = 400;
	v.length = 4;

	int16_t a[] = {800, 900};

	Vec_splice(&v, 2, 1, a, 2);

    	ASSERT_EQ(5, v.length);	
    	ASSERT_EQ(100, buffer[0]);
    	ASSERT_EQ(200, buffer[1]);
    	ASSERT_EQ(800, buffer[2]);
	ASSERT_EQ(900, buffer[3]);
    	ASSERT_EQ(400, buffer[4]);

	Vec_drop(&v);
}

TEST(VecImpl, splice_balanced_insert_and_delete) {
	Vec v = Vec_value(4, sizeof(int16_t));
	int16_t *buffer = (int16_t*)v.buffer;
	buffer[0] = 100;
	buffer[1] = 200;
	buffer[2] = 300;
	buffer[3] = 400;
	v.length = 4;

	int16_t a[] = {800, 900};

	Vec_splice(&v, 2, 2, a, 2);

    	ASSERT_EQ(4, v.length);	
    	ASSERT_EQ(100, buffer[0]);
    	ASSERT_EQ(200, buffer[1]);
    	ASSERT_EQ(800, buffer[2]);
	ASSERT_EQ(900, buffer[3]);

	Vec_drop(&v);
}

TEST(VecImpl, splice_delete_all_then_insert) {
	Vec v = Vec_value(4, sizeof(int16_t));
	int16_t *buffer = (int16_t*)v.buffer;
	buffer[0] = 100;
	buffer[1] = 200;
	buffer[2] = 300;
	buffer[3] = 400;
	v.length = 4;

	int16_t a[] = {800, 900};

	Vec_splice(&v, 0, 4, a, 2);

    	ASSERT_EQ(2, v.length);	
    	ASSERT_EQ(800, buffer[0]);
    	ASSERT_EQ(900, buffer[1]);

	Vec_drop(&v);
}
