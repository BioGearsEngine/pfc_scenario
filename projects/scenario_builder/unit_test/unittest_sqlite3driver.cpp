#include <QList>
#include <boost/filesystem.hpp>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <thread>

#include "sql/SqlLite3Driver.h"

#ifdef DISABLE_BIOGEARS_Path_TEST
#define TEST_FIXTURE_NAME DISABLED_SQLite3Driver
#else
#define TEST_FIXTURE_NAME PathFixture
#endif

// The fixture for testing class Foo.
class TEST_FIXTURE_NAME : public ::testing::Test {
protected:
  TEST_FIXTURE_NAME()
    : _db("unittest_db.sqlite"){

    };
  ~TEST_FIXTURE_NAME()
  {
    _db.close();
    try {
      boost::filesystem::remove("unittest_db.sqlite");
    } catch (boost::system::system_error e) {
      std::cout << e.what() << std::endl;
    }
  }

  virtual void SetUp();
  virtual void TearDown();

public:
  pfc::SQLite3Driver _db;
};

void TEST_FIXTURE_NAME::SetUp()
{
}

void TEST_FIXTURE_NAME::TearDown()
{

}

TEST_F(TEST_FIXTURE_NAME, Constructor)
{
  {
    pfc::SQLite3Driver driver("Scenario.sqlite");
  }
  {
    auto file = std::ofstream("Scenario.sqlite", std::ostream::in);
    EXPECT_TRUE(file.is_open());
    file.close();
  }
  try {
    boost::filesystem::remove("Scenario.sqlite");
  } catch (boost::system::system_error e) {
    std::cout << e.what() << std::endl;
  }
  auto file2 = std::ofstream("Scenario.sqlite", std::ostream::in);
  EXPECT_FALSE(file2.is_open());
}

TEST_F(TEST_FIXTURE_NAME, Insert_Author)
{
  using namespace pfc;
  Author author_1;
  Author author_2;
  Author author_3;

  author_1.first = "Solid";
  author_1.last = "Snake";
  author_1.email = "SolidSnake@MetalGear.com";

  author_2.first = "Rocky";
  author_2.last = "Balboa";
  author_2.email = "ItalianStalion@moives.com";

  author_3.first = "James";
  author_3.last = "Kirk";
  author_3.email = "Captian@Entreprise.uss";

  EXPECT_EQ(0, _db.authors().size());
  EXPECT_TRUE(_db.insert(author_1));
  EXPECT_EQ(1, _db.authors().size());
  EXPECT_TRUE(_db.insert(author_2));
  EXPECT_EQ(2, _db.authors().size());
  EXPECT_TRUE(_db.insert(author_3));
  EXPECT_EQ(3, _db.authors().size());
}
