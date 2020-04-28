#include "gtest/gtest.h"

extern "C" {
#include "Str.h"
#include "string.h"
}

TEST(StrImpl, value) {
    Str s = Str_value(2);
    ASSERT_EQ(1, s.length); // Vec's length includes null char
    ASSERT_EQ(3, s.capacity); // Requested space + 1 for null char
    char *buffer = (char*) s.buffer;
    ASSERT_EQ('\0', buffer[0]);
    Str_drop(&s);
}

TEST(StrImpl, drop) {
    Str s = Str_value(2);
    Str_drop(&s);
    ASSERT_EQ(0, s.capacity);
    ASSERT_EQ(0, s.length);
    ASSERT_EQ(nullptr, s.buffer);
}

/**
 * Helper fixture to setup a Str whose buffer
 * is prefilled with "abcd". 
 */
Str fixture_abcd()
{
    Str s = Str_value(4);
    char *buffer = (char*) s.buffer;
    buffer[0] = 'a';
    buffer[1] = 'b';
    buffer[2] = 'c';
    buffer[3] = 'd';
    buffer[4] = '\0';
    s.length = 5; // Includes null char
    return s;
}

TEST(StrImpl, length) {
    Str s = Str_value(2);
    ASSERT_EQ(0, Str_length(&s));
    Str_drop(&s);

    s = fixture_abcd();
    ASSERT_EQ(4, Str_length(&s));
    Str_drop(&s);
}

TEST(StrImpl, cstr) {
    Str s = fixture_abcd();
    ASSERT_STREQ("abcd", Str_cstr(&s));
    Str_drop(&s);
}

TEST(StrImpl, ref) {
    Str s = fixture_abcd();
    char *buffer = (char*) s.buffer;
    for (size_t i = 0; i < s.length; ++i) {
        ASSERT_EQ(&buffer[i], Str_ref(&s, i));
    }
    Str_drop(&s);
}

// TODO: Test remaining Str functions

TEST(StrImpl, from_empty) {
	Str s1 = Str_from("");
	Str s2 = Str_value(1);

	ASSERT_TRUE(Vec_equals(&s1, &s2));
	
	Str_drop(&s1);
	Str_drop(&s2);
}

TEST(StrImpl, from_basic) {	
	Str s1 = Str_from("abcd");
	Str s2 = fixture_abcd();

	ASSERT_TRUE(Vec_equals(&s1, &s2));
	
	Str_drop(&s1);
	Str_drop(&s2);
}

TEST(StrImpl, slice_basic)
{
	Str s1 = Str_value(2);
    	char *buffer = (char*) s1.buffer;
    	buffer[0] = 'a';
	buffer[1] = 'x';
    	buffer[2] = '\0';
    	s1.length = 3; // Includes null char
	Str s2 = fixture_abcd();

	Str_splice(&s1, 1, 1, "bcde", 3);

	ASSERT_TRUE(Vec_equals(&s1, &s2));
	
	Str_drop(&s1);
	Str_drop(&s2);
}

TEST(StrImpl, slice_delete_null_terminator)
{
	Str s = fixture_abcd();

	ASSERT_DEATH({
		Str_splice(&s, 3, 2, "abcd", 0);
    	}, ".* - Out of Bounds");
	ASSERT_DEATH({
		Str_splice(&s, 4, 1, "abcd", 0);
    	}, ".* - Out of Bounds");

	Str_drop(&s);
}

TEST(StrImpl, slice_insert_after_null_terminator)
{
	Str s = fixture_abcd();

    	ASSERT_DEATH({
		Str_splice(&s, 5, 0, "abcd", 1);
    	}, ".* - Out of Bounds");

	Str_drop(&s);
}

TEST(StrImpl, append_basic)
{
	Str s1 = Str_value(2);
    	char *buffer = (char*) s1.buffer;
    	buffer[0] = 'a';
	buffer[1] = 'b';
    	buffer[2] = '\0';
    	s1.length = 3; // Includes null char
	Str s2 = fixture_abcd();

	Str_append(&s1, "cd");

	ASSERT_TRUE(Vec_equals(&s1, &s2));
	
	Str_drop(&s1);
	Str_drop(&s2);
}

TEST(StrImpl, append_empty_string)
{
	Str s1 = fixture_abcd(); 
	Str s2 = fixture_abcd();

	Str_append(&s1, "");

	ASSERT_TRUE(Vec_equals(&s1, &s2));
	
	Str_drop(&s1);
	Str_drop(&s2);
}

TEST(StrImpl, append_to_empty_string)
{
	Str s1 = Str_value(0); 
	Str s2 = fixture_abcd();

	Str_append(&s1, "abcd");

	ASSERT_TRUE(Vec_equals(&s1, &s2));
	
	Str_drop(&s1);
	Str_drop(&s2);
}

TEST(StrImpl, get_basic)
{
	Str s = fixture_abcd();

	ASSERT_EQ('a', Str_get(&s, 0));
	ASSERT_EQ('b', Str_get(&s, 1));
	ASSERT_EQ('c', Str_get(&s, 2));
	ASSERT_EQ('d', Str_get(&s, 3));
	
	Str_drop(&s);
}

TEST(StrImpl, get_null_terminator)
{
	Str s1 = Str_value(0);
	Str s2 = fixture_abcd();

    	ASSERT_DEATH({
		Str_get(&s1, 0);
    	}, ".* - Out of Bounds");
    	ASSERT_DEATH({
		Str_get(&s2, 4);
    	}, ".* - Out of Bounds");
	
	Str_drop(&s1);
	Str_drop(&s2);
}

TEST(StrImpl, set_basic)
{
	Str s1 = Str_value(4);
    	char *buffer = (char*) s1.buffer;
    	buffer[0] = 'x';
	buffer[1] = 'x';
	buffer[2] = 'x';
	buffer[3] = 'x';
    	buffer[4] = '\0';
    	s1.length = 5; // Includes null char
	Str s2 = fixture_abcd();

	Str_set(&s1, 0, 'a');
	Str_set(&s1, 1, 'b');
	Str_set(&s1, 2, 'c');
	Str_set(&s1, 3, 'd');

	ASSERT_TRUE(Vec_equals(&s1, &s2));
	
	Str_drop(&s1);
	Str_drop(&s2);
}

TEST(StrImpl, set_to_extend)
{
	Str s1 = Str_value(0);
	Str s2 = fixture_abcd();

	Str_set(&s1, 0, 'a');
	Str_set(&s1, 1, 'b');
	Str_set(&s1, 2, 'c');
	Str_set(&s1, 3, 'd');

	ASSERT_TRUE(Vec_equals(&s1, &s2));
	
	Str_drop(&s1);
	Str_drop(&s2);
}

TEST(StrImpl, set_out_of_bounds)
{
	Str s = fixture_abcd();

    	ASSERT_DEATH({
		Str_set(&s, 5, 'a');
    	}, ".* - Out of Bounds");

	Str_drop(&s);
}
