#include <QList>
#include <boost/filesystem.hpp>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <thread>

#include "sql/SqlLite3Driver.h"
#include "sql/SqlLite3_Statments.h"

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
  EXPECT_TRUE(_db.update_author(&author_1));
  EXPECT_EQ(1, _db.authors().size());
  EXPECT_TRUE(_db.update_author(&author_2));
  EXPECT_EQ(2, _db.authors().size());
  EXPECT_TRUE(_db.update_author(&author_3));
  EXPECT_EQ(3, _db.authors().size());
}

TEST_F(TEST_FIXTURE_NAME, Insert_Property)
{
  using namespace pfc;
  Property property_1;
  Property property_2;
  Property property_3;

  property_1.name = "Solid";
  property_1.value = "Snake";

  property_2.name = "Rocky";
  property_2.value = "Balboa";

  property_3.name = "James";
  property_3.value = "Kirk";

  EXPECT_EQ(0, _db.properties().size());
  EXPECT_TRUE(_db.update_property(&property_1));
  EXPECT_EQ(1, _db.properties().size());
  EXPECT_TRUE(_db.update_property(&property_2));
  EXPECT_EQ(2, _db.properties().size());
  EXPECT_TRUE(_db.update_property(&property_3));
  EXPECT_EQ(3, _db.properties().size());
}

TEST_F(TEST_FIXTURE_NAME, Insert_Restriction)
{
  using namespace pfc;
  Restriction restriction_1;
  Restriction restriction_2;
  Restriction restriction_3;

  restriction_1.name = "Solid";
  restriction_1.value = "Snake";
  
  restriction_2.name = "Rocky";
  restriction_2.value = "Balboa";
  
  restriction_3.name = "James";
  restriction_3.value = "Kirk";

  EXPECT_EQ(0, _db.restrictions().size());
  EXPECT_TRUE(_db.update_restriction(&restriction_1));
  EXPECT_EQ(1, _db.restrictions().size());
  EXPECT_TRUE(_db.update_restriction(&restriction_2));
  EXPECT_EQ(2, _db.restrictions().size());
  EXPECT_TRUE(_db.update_restriction(&restriction_3));
  EXPECT_EQ(3, _db.restrictions().size());
}

TEST_F(TEST_FIXTURE_NAME, Select_Restriction)
{
  using namespace pfc;
  Restriction restriction_1;
  Restriction restriction_2;

  restriction_1.name = "Solid";
  restriction_1.value = "Snake";
  restriction_2.name = "Rocky";
  restriction_2.value = "Balboa";

  EXPECT_TRUE(_db.update_restriction(&restriction_1));
  EXPECT_TRUE(_db.update_restriction(&restriction_2));

  Restriction id;
  Restriction name;

  id.id = 1;
  name.name = "Rocky";

  _db.select_restriction(&id);
  _db.select_restriction(&name);

  restriction_1.id = 1;
  EXPECT_EQ(restriction_1, id);
  restriction_2.id = 2;
  EXPECT_EQ(restriction_2, name);
}


TEST_F(TEST_FIXTURE_NAME, Select_Property)
{
  using namespace pfc;
  Property property_1;
  Property property_2;

  property_1.name = "Solid";
  property_1.value = "Snake";
  property_2.name = "Rocky";
  property_2.value = "Balboa";

  EXPECT_TRUE(_db.update_property(&property_1));
  EXPECT_TRUE(_db.update_property(&property_2));

  Property id;
  Property name;

  id.id = 1;
  name.name = "Rocky";

  _db.select_property(&id);
  _db.select_property(&name);

  property_1.id = 1;
  EXPECT_EQ(property_1, id);
  property_2.id = 2;
  EXPECT_EQ(property_2, name);
}

TEST_F(TEST_FIXTURE_NAME, Select_Author)
{
  using namespace pfc;
  Author author_1;
  Author author_2;

  author_1.first = "Solid";
  author_1.last = "Snake";
  author_1.email = "solid.name@email.email";
  author_1.zip = "71111";
  author_1.plus_4 = "2222";
  author_1.state = "la";
  author_1.country = "USA";
  author_1.organization = "BioGears";
  author_1.phone = "555-5555";

  author_2.first = "Rocky";
  author_2.last = "Balboa";
  author_2.email = "Rocky.Balboa@muscle.man";
  author_2.zip = "33333";
  author_2.plus_4 = "4444";
  author_2.state = "Nb";
  author_2.country = "Canada";
  author_2.organization = "PFC";
  author_2.phone = "222-555-1111";


  EXPECT_TRUE(_db.update_author(&author_1));
  EXPECT_TRUE(_db.update_author(&author_2));

  Author id;
  Author email;

  id.id = 1;
  email.email = "Rocky.Balboa@muscle.man";

  _db.select_author(&id);
  _db.select_author(&email);

  author_1.id = 1;
  EXPECT_EQ(author_1, id);
  author_2.id = 2;
  EXPECT_EQ(author_2, email);
}

TEST_F(TEST_FIXTURE_NAME, Remove_Author)
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

  EXPECT_TRUE(_db.update_author(&author_1));
  EXPECT_TRUE(_db.update_author(&author_2));
  EXPECT_TRUE(_db.update_author(&author_3));
  EXPECT_EQ(3, _db.authors().size());
  EXPECT_TRUE(_db.remove_author(&author_1));
  EXPECT_TRUE(_db.remove_author(&author_2));
  EXPECT_EQ(1, _db.authors().size());
}

TEST_F(TEST_FIXTURE_NAME, Remove_Property)
{
  using namespace pfc;
  Property property_1;
  Property property_2;
  Property property_3;

  property_1.name = "Solid";
  property_1.value = "Snake";

  property_2.name = "Rocky";
  property_2.value = "Balboa";

  property_3.name = "James";
  property_3.value = "Kirk";

  EXPECT_TRUE(_db.update_property(&property_1));
  EXPECT_TRUE(_db.update_property(&property_2));
  EXPECT_TRUE(_db.update_property(&property_3));
  EXPECT_EQ(3, _db.properties().size());
  EXPECT_TRUE(_db.remove_property(&property_1));
  EXPECT_TRUE(_db.remove_property(&property_3));
  EXPECT_EQ(1, _db.properties().size());
}

TEST_F(TEST_FIXTURE_NAME, Remove_Restriction)
{
  using namespace pfc;
  Restriction restriction_1;
  Restriction restriction_2;
  Restriction restriction_3;

  restriction_1.name = "Solid";
  restriction_1.value = "Snake";

  restriction_2.name = "Rocky";
  restriction_2.value = "Balboa";

  restriction_3.name = "James";
  restriction_3.value = "Kirk";

  EXPECT_TRUE(_db.update_restriction(&restriction_1));
  EXPECT_TRUE(_db.update_restriction(&restriction_2));
  EXPECT_TRUE(_db.update_restriction(&restriction_3));
  EXPECT_EQ(3, _db.restrictions().size());
  EXPECT_TRUE(_db.remove_restriction(&restriction_1));
  EXPECT_TRUE(_db.remove_restriction(&restriction_3));
  EXPECT_EQ(1, _db.restrictions().size());
}