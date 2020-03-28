#include <iostream>
#include <sstream>
#include <memory>
#include "gtest/gtest.h"

#include "stringcommon/stringcommon.hpp"
TEST(stringcommon, Trim)
{
	std::string str = "hello";
	ASSERT_STREQ(stringcommon::Trim(str).c_str(), "hello");
	
	str = " hello";
	ASSERT_STREQ(stringcommon::Trim(str).c_str(), "hello");

	str = "   hello  ";
	ASSERT_STREQ(stringcommon::Trim(str).c_str(), "hello");

	str = "";
	ASSERT_STREQ(stringcommon::Trim(str).c_str(), "");

	str = "hello world   ";
	ASSERT_STREQ(stringcommon::Trim(str).c_str(), "hello world");
}

TEST(stringcommon, Format)
{
	std::string s = stringcommon::Format("hello, x=%d, y=%g", 42, 3.14);
	ASSERT_STREQ(s.c_str(), "hello, x=42, y=3.14");
}

TEST(stringcommon, Split)
{
	std::vector<std::string> splits = stringcommon::Split("hello:world:123:3.14", ":");
	ASSERT_EQ(static_cast<int>(splits.size()), 4);
	ASSERT_STREQ(splits[0].c_str(), "hello");
	ASSERT_STREQ(splits[2].c_str(), "123");

	splits = stringcommon::Split("1:2:3:", ":");
	ASSERT_EQ(splits.size(), 3u);

	splits = stringcommon::Split("::123:", ":");
	ASSERT_EQ(splits.size(), 3u);
}

#include "configstruct/attributepairconfig.hpp"
TEST(Test_AttributePairConfig, test1)
{
	{
		std::string config_str = "gongji=1000, fangyu=2000";
		std::istringstream iss(config_str);

		AttributePairConfig config;
		ASSERT_TRUE(iss >> config);

		const auto& pair_list = config.GetPairList();
		ASSERT_EQ(pair_list.size(), 2u);

		ASSERT_TRUE(pair_list[0].attr_str == "gongji" && pair_list[0].attr_value == 1000);
		ASSERT_TRUE(pair_list[1].attr_str == "fangyu" && pair_list[1].attr_value == 2000);
	}
	
	{
		std::string config_str = "   gongji =  1000";
		std::istringstream iss(config_str);

		AttributePairConfig config;
		ASSERT_TRUE(iss >> config);

		const auto& pair_list = config.GetPairList();
		ASSERT_EQ(pair_list.size(), 1u);

		ASSERT_TRUE(pair_list[0].attr_str == "gongji" && pair_list[0].attr_value == 1000);
	}

	{
		std::string config_str = "gongji= 1000,";
		std::istringstream iss(config_str);

		AttributePairConfig config;
		ASSERT_TRUE(iss >> config);
	}

	{
		std::string config_str = "gongji=1000|";
		std::istringstream iss(config_str);

		AttributePairConfig config;
		ASSERT_FALSE(iss >> config);
	}
}

#include "configstruct/serveropendayconfig.hpp"
TEST(Test_ServerOpenDayConfig, test1)
{
	{
		std::string config_str = "1,7";
		std::istringstream iss(config_str);

		ServerOpenDayConfig config;
		ASSERT_TRUE(iss >> config);
		ASSERT_TRUE(config.IsInServerOpenDay(1));
		ASSERT_TRUE(config.IsInServerOpenDay(7));
		ASSERT_TRUE(config.IsInServerOpenDay(5));
		ASSERT_TRUE(!config.IsInServerOpenDay(10));
	}

	{
		std::string config_str = "1|7";
		std::istringstream iss(config_str);

		ServerOpenDayConfig config;
		ASSERT_FALSE(iss >> config);
	}
}

#include "fixlencontainer/bitset.hpp"
TEST(Test_BitSet, test1)
{
	Bitset<8> b8;
	ASSERT_TRUE(b8.Test(0) == false);
	b8.Set(0);
	ASSERT_TRUE(b8.Test(0));
	b8.UnSet(0);
	ASSERT_TRUE(b8.Test(0) == false);
}

#include "fixlencontainer/fixlenarray.hpp"
TEST(Test_FixLenArray, test)
{
	FixLenArray<int, 4> a4;
	ASSERT_EQ(sizeof(a4), sizeof(int) * 4);

	// 值初始化
	ASSERT_EQ(a4[3], 0);

	a4[3] = 42;
	ASSERT_EQ(a4[3], 42);

	ASSERT_THROW(a4[4], std::out_of_range);
	ASSERT_THROW(a4[-1], std::out_of_range);
}

#include "fixlencontainer/fixlenhashtable.hpp"
TEST(Test_FixLenHashTable, test)
{
	FixLenHashTable<int, int, 5> hash_table;

	ASSERT_EQ(hash_table.Size(), 0);

	hash_table.Put(1, 10);
	ASSERT_EQ(hash_table.Size(), 1);
	ASSERT_EQ(*hash_table.Get(1), 10);

	ASSERT_FALSE(hash_table.Put(1, 20));	// 不能重复key
	ASSERT_EQ(hash_table.Size(), 1);
	ASSERT_EQ(*hash_table.Get(1), 10);

	for (int i = 3; i <= 6; i++)
	{
		ASSERT_TRUE(hash_table.Put(i, i * i));
	}
	ASSERT_FALSE(hash_table.Put(7, 100));	// full

	ASSERT_EQ(*hash_table.Get(6), 36);
}

#include "fixlencontainer/fixlenqueue.hpp"
TEST(Test_FixLenQueue, test)
{
	FixLenQueue<int, 5> fq;

	for (int i = 1; i <= 5; i++)
	{
		fq.Push(i);
	}

	ASSERT_TRUE(fq.IsFull());

	ASSERT_EQ(fq[0], 5);
	ASSERT_EQ(fq[4], 1);
	ASSERT_THROW(fq[5], std::out_of_range);

	fq.Push(100);
	ASSERT_EQ(fq[0], 100);
	ASSERT_EQ(fq[1], 5);
}

#include "fixlencontainer/fixlenranklist.hpp"
TEST(Test_FixLenRankList, test1)
{
	FixLenRankList<int, 10> ranklist;

    ranklist.Push(10);
    ranklist.Push(20);
    ranklist.Push(5);
    ranklist.Push(20);

    std::vector<int> top = ranklist.GetTopList(1);
    EXPECT_EQ(20, top[0]);

    for (int i = 0; i < 50; i++)
    {
        ranklist.Push(i);
    }
    EXPECT_EQ(49, ranklist.GetTop());
}

TEST(Test_FixLenRankList, test2)
{
	class Foo
	{
	public:
		Foo() :
			id(0),
			value(0)
		{
		}
		Foo(int _id, int _value) : 
			id(_id),
			value(_value)
		{
		}
		bool operator==(const Foo &rhs) const
		{
			return id == rhs.id;
		}

		bool operator<(const Foo &rhs) const
		{
			return value < rhs.value;
		}
		int id;
		int value;
	};

    FixLenRankList<Foo, 5> ranklist;

    ranklist.Push(Foo(0, 30));
    ranklist.Push(Foo(1, 10));  // pass
    ranklist.Push(Foo(2, 11));
    ranklist.Push(Foo(3, 22));
    ranklist.Push(Foo(4, 11));
    ranklist.Push(Foo(4, 9));   // pass
    ranklist.Push(Foo(4, 100));

    const auto &top = ranklist.GetTop();
    EXPECT_EQ(top.id, 4);
}

#include "fixlencontainer/fixlenstr.hpp"
TEST(Test_FixLenStr, test)
{
	FixLenStr<32> str = "hello";
    ASSERT_STREQ(str.Data(), "hello");

    char raw[32];
    str.Copy(raw);
    ASSERT_STREQ(raw, "hello");

    const char *raw_str = str;
    ASSERT_STREQ(raw_str, "hello");

    std::string s = str;
    ASSERT_STREQ(s.c_str(), "hello");

    FixLenStr<5> str2 = s;
    ASSERT_STRNE(str2, "hello");
	ASSERT_STREQ(str2, "hell");

    str = str;
    ASSERT_STREQ(str, "hello");

    str = str.Data();
    ASSERT_STREQ(str, "hello");

    ASSERT_EQ(str.Len(), 5);
}

#include "tools/misc/misc.hpp"
TEST(Misc, StringToType)
{
	std::string s = "42";
	int i;

	ASSERT_TRUE(StringToType(s, i));
	ASSERT_EQ(i, 42);

	s = "3.14";
	double d;
	ASSERT_TRUE(StringToType(s, d));
	ASSERT_DOUBLE_EQ(d, 3.14);

	s = "3.14a";
	ASSERT_FALSE(StringToType(s, d));

	s = "b42";
	ASSERT_FALSE(StringToType(s, i));

	std::string s_v;
	ASSERT_TRUE(StringToType(s, s_v));
	ASSERT_STREQ(s.c_str(), s_v.c_str());
}

TEST(Misc, TypeToString)
{
	std::string s;
	int i = 42;

	ASSERT_TRUE(TypeToString(i, s));
	ASSERT_STREQ(s.c_str(), "42");

	double d = 3.14;
	ASSERT_TRUE(TypeToString(d, s));
	ASSERT_STREQ(s.c_str(), "3.14");
}

TEST(Misc, IsNumber)
{
	double d;
	ASSERT_TRUE(IsNumber("42", &d));
	ASSERT_DOUBLE_EQ(d, 42);

	ASSERT_TRUE(IsNumber("3.14", &d));
	ASSERT_DOUBLE_EQ(d, 3.14);

	ASSERT_FALSE(IsNumber("  3.45  "));
	ASSERT_FALSE(IsNumber("abc"));
	ASSERT_FALSE(IsNumber("12a3"));
}

#include "tools/serializer/serializer.hpp"
TEST(Tools, Serializer)
{
	Serializer ser;
	char buffer[32] = "hello";
	ser.Write(42, 3.14, buffer);	 // 4 + 8 + 32 = 44

	ASSERT_EQ(ser.Size(), 44u);

	DeSerializer de(ser.Ptr(), ser.Size());
	int a;
	double d;
	de.Read(a, d, buffer);

	ASSERT_EQ(a, 42);
	ASSERT_DOUBLE_EQ(d, 3.14);
	ASSERT_STREQ(buffer, "hello");

	de.Read(a);
	ASSERT_TRUE(de.Eof());

	char userbuffer[8] {};
	ser.SetUserBuffer(userbuffer, sizeof(userbuffer));
	ser.Write(100, 1000);
	ASSERT_THROW(ser.Write(999), std::runtime_error);

	ASSERT_EQ(ser.Size(), 8u);

	DeSerializer de2(ser.Ptr(), ser.Size());
	de2.FetchData((char*)&a, sizeof(a));
	ASSERT_EQ(a, 100);

	int b;
	de2 >> a >> b;
	ASSERT_EQ(a, 100);
	ASSERT_EQ(b, 1000);
}

TEST(Tools, Serializer_operator)
{
	Serializer ser;
	char buffer[32] = "hello";
	ser << 42 << 3.14 << buffer;	// 4 + 8 + (4+5) = 21

	ASSERT_EQ(ser.Size(), 21u);

	DeSerializer de(ser.Ptr(), ser.Size());
	int a;
	double d;
	memset(buffer, 0, sizeof(buffer));
	de >> a >> d >> buffer;

	ASSERT_EQ(a, 42);
	ASSERT_DOUBLE_EQ(d, 3.14);
	ASSERT_STREQ(buffer, "hello");
}

TEST(Tools, Serializer_string)
{
	Serializer ser;
	std::string s = "hello";	// 4 + 5 = 9
	ser << s;

	ASSERT_EQ(ser.Size(), 9u);

	DeSerializer de(ser.Ptr(), ser.Size());
	s = "";
	de >> s;

	ASSERT_STREQ(s.c_str(), "hello");
}

TEST(Tools, Serializer_vec_list)
{
	{
		Serializer ser;
		std::vector<int> vi {1, 2, 3, 4}; // 4 + 4 * 4 = 20
		ser << vi;

		ASSERT_EQ(ser.Size(), 20u);

		DeSerializer de(ser.Ptr(), ser.Size());
		vi = {};
		ASSERT_EQ(vi.size(), 0u);

		de >> vi;

		ASSERT_EQ(vi.size(), 4u);
		ASSERT_TRUE(vi[0] == 1 && vi[1] == 2 && vi[2] == 3 && vi[3] == 4);
	}

	{
		Serializer ser;
		std::list<int> vi {1, 2, 3, 4}; // 4 + 4 * 4 = 20
		ser << vi;

		ASSERT_EQ(ser.Size(), 20u);

		DeSerializer de(ser.Ptr(), ser.Size());
		vi = {};
		ASSERT_EQ(vi.size(), 0u);

		de >> vi;

		ASSERT_EQ(vi.size(), 4u);
		ASSERT_TRUE(vi.front() == 1);
	}
}

TEST(Tools, Serializer_map)
{
	Serializer ser;
	std::map<int, std::string> cache =
	{
		{1, "one"},
		{2, "two"},
		{3, "three"},
	};
	ser << cache; // 4 + (4+4+3) + (4+4+3) + (4+4+5) = 39

	ASSERT_EQ(ser.Size(), 39u);

	DeSerializer de(ser.Ptr(), ser.Size());
	cache.clear();
	de >> cache;

	ASSERT_EQ(cache.size(), 3u);
	ASSERT_STREQ(cache[1].c_str(), "one");
	ASSERT_STREQ(cache[2].c_str(), "two");
	ASSERT_STREQ(cache[3].c_str(), "three");
}

TEST(Tools, Serializer_set)
{
	Serializer ser;
	std::set<int> iset {1, 2, 3, 4};

	ser << iset;

	iset.clear();

	DeSerializer de(ser.Ptr(), ser.Size());
	de >> iset;

	ASSERT_EQ(iset.size(), 4u);
}

struct SerFoo
{
	int a;
	double d;
};

#include "tools/buffer.hpp"
Serializer& operator<<(Serializer &s, const SerFoo &v)
{
	unsigned int len = sizeof(v);
	s << len;
	s.Write(v);
	return s;
}
DeSerializer& operator>>(DeSerializer& d, SerFoo& v)
{
	unsigned int len;
	d >> len;
	Buffer buffer(len);
	d.ReadData(buffer.Data(), len);
	if (sizeof(v) < len)
	{
		d.SetFailState();
		return d;
	}
	memcpy(&v, buffer.Data(), len);
	return d;
}

TEST(Tools, Serializer_Custom)
{
	Serializer ser;
	std::vector<SerFoo> vec = {
		{1, 1.1},
		{2, 2.2},
		{3, 3.3},
	};

	ser << vec;

	DeSerializer de(ser.Ptr(), ser.Size());
	vec.clear();
	de >> vec;

	ASSERT_EQ(vec.size(), 3u);
	ASSERT_EQ(vec[0].a, 1);
	ASSERT_DOUBLE_EQ(vec[0].d, 1.1);
}

#include "tools/buffer.hpp"
TEST(Tools, Buffer)
{
	std::string hello = "hello";
	Buffer buffer(hello.size());
	strncpy(buffer.Data(), hello.c_str(), hello.size());

	ASSERT_STREQ(buffer.Data(), "hello");
}

/*
#include "3rd/str_bkdr.hpp"
TEST(ThirdPart, STR_BKDR)
{
	std::string s = "hello";
	ASSERT_EQ(TypicalBKDR(s.c_str()), "hello"_bkdr);

	switch (TypicalBKDR(s.c_str()))
	{
	case "hello"_bkdr: break;	// hit
	default: { assert(0); } break;
	}
}
*/

#include "misc/upgradefunc.hpp"
TEST(Misc, UpgradeFunc)
{
	long long cur_exp = 0;
	int cur_level = 1;

	const std::vector<long long> upgrade_exp_list = {
		0, 100, 200, 500, 0
	};//0, 1    2    3    4

	int inc_exp = 350;
	Upgrade(cur_exp, inc_exp, cur_level, upgrade_exp_list);

	ASSERT_EQ(cur_level, 3);
	ASSERT_EQ(cur_exp, 50);

	Upgrade(cur_exp, 450, cur_level, upgrade_exp_list);

	ASSERT_EQ(cur_level, 4);
	ASSERT_EQ(cur_exp, 0);

	bool ret = Upgrade(cur_exp, 1, cur_level, upgrade_exp_list);
	ASSERT_FALSE(ret);
}

#include "tools/eventhandler.hpp"
static void HelloEventFoo(void *pthis, DeSerializer &ds);

struct EventFoo
{
	EventFoo()
	{
		EventHandler::Instance().RegisterEvent("EventFoo::Hello", { this, HelloEventFoo });
	}

	~EventFoo()
	{
		EventHandler::Instance().UnRegisterEvent("EventFoo::Hello", this);
	}

	void Hello(const std::string &msg)
	{
		std::cout << "Foo::Hello: " << msg << std::endl;
	}
};

static void HelloEventFoo(void *pthis, DeSerializer &ds)
{
	std::string msg;
	if (ds >> msg)
		static_cast<EventFoo*>(pthis)->Hello(msg);
};

TEST(Tools, EventHandler)
{
	EventHandler::EventItem event_item = { nullptr, [](void *none, DeSerializer &ds){ 
		int a = 0;
		double d = 3.14;
		if (ds >> a >> d)
			std::cout << "This is a test from EventHandler " << a << " " << d << "\n"; 
	} };
	EventHandler::Instance().RegisterEvent("TestEvent", event_item);

	EventHandler::Instance().Dispatch("TestEvent", 42, 3.14);

	{
		EventFoo event_foo;
		EventHandler::Instance().Dispatch("EventFoo::Hello", "hi event");
	}
	EventHandler::Instance().Dispatch("EventFoo::Hello", "hi event impossible");
}

#include "tools/Logger.hpp"
int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);

    // only call once
    // return 0 if all succ
    // otherwise, not
    return RUN_ALL_TESTS();
}
