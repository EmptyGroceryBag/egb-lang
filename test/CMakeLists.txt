if(BUILD_TEST)
	include(FetchContent)
	FetchContent_Declare(
		googletest
		URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
	)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	FetchContent_MakeAvailable(googletest)

	enable_testing()

	add_executable(
		test 
		LexCommentTest.cpp
		LexNumberTest.cpp
		LexStringToDoubleTest.cpp
		LexParenthesisTest.cpp
		ParseExpressionTest.cpp
	)

target_link_libraries(test gtest_main el)
	add_test(NAME hello_test COMMAND hello_test)

	target_include_directories(test PUBLIC ${PROJECT_SOURCE_DIR}/../include)

else()
	add_executable(
		el
		main.cpp
		lexer.cpp
	)
endif()
