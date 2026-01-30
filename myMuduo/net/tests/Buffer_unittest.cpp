#include"myMuduo/net/Buffer.h"

// #define BOOST_TEST_MODULE BufferTest
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include<boost/test/unit_test.hpp>

using myMuduo::string;
using myMuduo::net::Buffer;

BOOST_AUTO_TEST_CASE(testBufferAppendRetrieve) {
	Buffer buf;
	BOOST_CHECK_EQUAL(buf.readableBytes(), 0);
	BOOST_CHECK_EQUAL(buf.writableBytes(), Buffer::kInitialSize);
	BOOST_CHECK_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend);

	const string str(200, 'x');
	buf.append(str);
	BOOST_CHECK_EQUAL(buf.readableBytes(), str.size());
	BOOST_CHECK_EQUAL(buf.writableBytes(), Buffer::kInitialSize - str.size());
	BOOST_CHECK_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend);

	const string str2 = buf.retrieveAsString(50);
	BOOST_CHECK_EQUAL(str2.size(), 50);
	BOOST_CHECK_EQUAL(buf.readableBytes(), str.size() - str2.size());
	BOOST_CHECK_EQUAL(buf.writableBytes(), Buffer::kInitialSize - str.size());
	BOOST_CHECK_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend + str2.size());
	BOOST_CHECK_EQUAL(str2, string(50, 'x'));

	buf.append(str);
	//BOOST_CHECK_EQUAL()
}