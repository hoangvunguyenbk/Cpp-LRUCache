#include "gtest/gtest.h"
#include "cache.h"

#define CACHE_SIZE 3
using namespace std;

typedef std::shared_ptr<LRUCache<int, std::string>> SHARED_CACHE;

class LRUCaheTest : public ::testing::Test {

protected:
	SHARED_CACHE cache;

	virtual void SetUp() {
		cache = std::make_shared<LRUCache<int, std::string>> (3);
	}

	virtual void TearDown() {
	}
};


TEST_F(LRUCaheTest, TestGet) {
  
  cache->Put(1,"1"); // 1
  cache->Put(2,"2"); // 2,1
  cache->Put(3,"3"); // 3,2,1
  cache->Put(4,"4"); // 4,3,2

  std::string retrived_value("");
  ASSERT_EQ(cache->Get(1, retrived_value), false); // 4,3,2
  ASSERT_EQ(retrived_value, "");

  ASSERT_EQ(cache->Get(2, retrived_value), true); // 2,4,3
  ASSERT_EQ(retrived_value,"2");

  cache->Put(5,"5"); // 5,2,4
  ASSERT_EQ(cache->Get(5, retrived_value), true); // 5,2,4
  ASSERT_EQ(retrived_value, "5");

  ASSERT_EQ(cache->Get(4, retrived_value), true); // 4, 5, 2
  ASSERT_EQ(retrived_value, "4");


  cache->Put(2,"Two");
  ASSERT_EQ(cache->Get(2, retrived_value), true); // {2 : "II"}, 4, 5
  ASSERT_EQ(retrived_value, "Two");

  // Cache-entries : {2, "Two"}, {4, "4"}, {5, "5"}
  ASSERT_EQ(cache->Get(2, retrived_value), true);
  ASSERT_EQ(cache->Get(4, retrived_value), true);
  ASSERT_EQ(cache->Get(5, retrived_value), true);
}

TEST_F(LRUCaheTest, TestPut) {

  cache->Put(1,"1"); // 1
  cache->Put(2,"2"); // 2,1
  cache->Put(3,"3"); // 3,2,1
  cache->Put(4,"4"); // 4,3,2
  cache->Put(5,"5"); // 5,4,3

  std::string retrived_value("");
  ASSERT_EQ(cache->Get(2, retrived_value), false); // 5,4,3
  ASSERT_EQ(retrived_value, "");

  ASSERT_EQ(cache->Get(4, retrived_value), true); // 4,5,3
  ASSERT_EQ(retrived_value, "4");

  cache->Put(2,"Two");
  ASSERT_EQ(cache->Get(2, retrived_value), true); // II,4,5
  ASSERT_EQ(retrived_value, "Two");

  // Cache-entries : {2, "Two"}, {4, "4"}, {5, "5"}
  ASSERT_EQ(cache->Get(2, retrived_value), true);
  ASSERT_EQ(cache->Get(4, retrived_value), true);
  ASSERT_EQ(cache->Get(5, retrived_value), true);
}

