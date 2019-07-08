#include <thread>
#include <iostream>
#include <fstream>
#include <gtest/gtest.h>

#include "sql/SqlLite3Driver.h"

#ifdef DISABLE_BIOGEARS_Path_TEST
#define TEST_FIXTURE_NAME DISABLED_SQLite3Driver
#else
#define TEST_FIXTURE_NAME PathFixture
#endif

// The fixture for testing class Foo.
class TEST_FIXTURE_NAME : public ::testing::Test {
protected:

   TEST_FIXTURE_NAME() = default;
  ~TEST_FIXTURE_NAME() = default;

  virtual void SetUp();
virtual void TearDown();

};

void TEST_FIXTURE_NAME::SetUp()
{

}

void TEST_FIXTURE_NAME::TearDown()
{
}

TEST_F( TEST_FIXTURE_NAME, Constructor)
{
  {
    pfc::SQLite3Driver driver("Scenario.sqlite");
  }
  auto file = std::ofstream("Scenario.sqlite", std::ostream::in);
  EXPECT_TRUE(file.is_open());
}
