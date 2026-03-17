// Minimal GoogleTest stub for bare-metal cross-compilation.
// Provides no-op macros so test TUs compile (exercising static_assert)
// without requiring POSIX or the hosted C++ standard library.
#ifndef GTEST_STUB_H
#define GTEST_STUB_H

#define TEST(suite, name) void suite##_##name()
#define TEST_F(fixture, name) void fixture##_##name()

#define ASSERT_TRUE(x)      ((void)(x))
#define ASSERT_FALSE(x)     ((void)(x))
#define ASSERT_EQ(a, b)     ((void)(a), (void)(b))
#define ASSERT_NE(a, b)     ((void)(a), (void)(b))
#define ASSERT_NEAR(a, b, t) ((void)(a), (void)(b), (void)(t))

#define EXPECT_TRUE(x)      ((void)(x))
#define EXPECT_FALSE(x)     ((void)(x))
#define EXPECT_EQ(a, b)     ((void)(a), (void)(b))
#define EXPECT_NE(a, b)     ((void)(a), (void)(b))
#define EXPECT_NEAR(a, b, t) ((void)(a), (void)(b), (void)(t))

#endif // GTEST_STUB_H
