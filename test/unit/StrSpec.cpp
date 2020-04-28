#include "gtest/gtest.h"

extern "C" {
#include "stdint.h"
#include "Str.h"
}

/**
 * The purpose of these tests is to prove correctness of the Str
 * abstraction barrier from the user's point-of-view.
 */

TEST(StrSpec, values_init_empty) {
    Str s = Str_value(10);
    ASSERT_EQ(0, Str_length(&s));
    Str_drop(&s);
}

TEST(StrSpec, values_init_cstr) {
    Str s = Str_value(10);
    ASSERT_STREQ("", Str_cstr(&s));
    Str_drop(&s);
}

TEST(StrSpec, get_set_contract) {
	Str s = Str_value(1);

	Str_set(&s, 0, 'a');
	Str_set(&s, 1, 'b');
	Str_set(&s, 2, 'c');

	ASSERT_EQ(3, Str_length(&s));
	ASSERT_EQ('a', Str_get(&s, 0));
	ASSERT_EQ('b', Str_get(&s, 1));
	ASSERT_EQ('c', Str_get(&s, 2));

	Str_drop(&s);
}

TEST(StrSpec, cstr_set_contract) {
	Str s = Str_value(1);

	Str_set(&s, 0, 'a');
	Str_set(&s, 1, 'p');
	Str_set(&s, 2, 'p');
	Str_set(&s, 3, 'l');
	Str_set(&s, 4, 'e');

	ASSERT_EQ(5, Str_length(&s));
	ASSERT_STREQ("apple", Str_cstr(&s));

	Str_drop(&s);
}

TEST(StrSpec, from_get_contract) {
	Str s = Str_from("hello");

	ASSERT_EQ(5, Str_length(&s));
	ASSERT_EQ('h', Str_get(&s, 0));
	ASSERT_EQ('e', Str_get(&s, 1));
	ASSERT_EQ('l', Str_get(&s, 2));
	ASSERT_EQ('l', Str_get(&s, 3));
	ASSERT_EQ('o', Str_get(&s, 4));

	Str_drop(&s);
}

TEST(StrSpec, splice_shorten) {
	Str s = Str_from("racecar");
	ASSERT_EQ(7, Str_length(&s));

	Str_splice(&s, 1, 4, "o", 1);
	ASSERT_EQ(4, Str_length(&s));
	ASSERT_STREQ("roar", Str_cstr(&s));

	Str_drop(&s);
}

TEST(StrSpec, splice_lengthen) {
	Str s = Str_from("car");
	ASSERT_EQ(3, Str_length(&s));

	Str_splice(&s, 0, 0, "race", 4);
	ASSERT_EQ(7, Str_length(&s));
	ASSERT_STREQ("racecar", Str_cstr(&s));

	Str_drop(&s);
}

TEST(StrSpec, append_equality) {
	Str s = Str_value(0);
	ASSERT_EQ(0, Str_length(&s));

	Str_set(&s, 0, 'a');
	ASSERT_EQ(1, Str_length(&s));

	Str_append(&s, "pple");
	ASSERT_EQ(5, Str_length(&s));
	ASSERT_STREQ("apple", Str_cstr(&s));

	Str_drop(&s);
}


