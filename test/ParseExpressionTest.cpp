#include <gtest/gtest.h>
#include <string>
#include "parser.h"

#if 0
/// @@@ Only a test! Change later!!!
TEST(TestNumberExpression, TestInteger) {
	std::string buffer = "1234";
	char *iterator = &buffer[0];

	ASTNode *n = parse_expr(buffer, iterator);

	
	EXPECT_EQ(n
}
#endif
