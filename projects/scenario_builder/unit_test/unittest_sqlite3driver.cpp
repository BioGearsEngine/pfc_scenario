#include <QDebug>
#include <QList>

#include <boost/filesystem.hpp>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <thread>

#include "sql/SqlLite3Driver.h"
#include "sql/SqlLite3_Statments.h"
#include "xml/SchemaUtils.h"
#include "xml/Serializer.h"
#include <pfc/schema/pfc_scenario.hxx>

#define DISABLE_ASSESSMENT_TEST
#define DISABLE_Author_TEST
#define DISABLE_Citation_TEST
#define DISABLE_Equipment_TEST
#define DISABLE_Event_TEST
#define DISABLE_Injury_TEST
#define DISABLE_InjurySet_TEST
#define DISABLE_Location_TEST
#define DISABLE_Objectives_TEST
#define DISABLE_Property_TEST
#define DISABLE_Restrictions_TEST
#define DISABLE_Role_TEST
#undef DISABLE_Scene_TEST
#define DISABLE_Treatment_TEST
#define DISABLE_RoleMap_TEST
#define DISABLE_EventMap_TEST
#define DISABLE_SceneMap_TEST
#define DISABLE_EquipmentMap_TEST
#define DISABLE_CitationMap_TEST
#define DISABLE_LocationMap_TEST
#undef DISABLE_Backdoor_TEST
#undef DISABLE_REMOVAL_TEST
#define DISABLE_Serialization_TEST

// The fixture for testing class Foo.
class SQLiteDriver : public ::testing::Test {
protected:
  SQLiteDriver()
    : _db("unittest_db.sqlite"){

    };
  ~SQLiteDriver()
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

class SerializationTest : public ::testing::Test {
protected:
  SerializationTest()
    : _db("unittest_db.sqlite")
  {
    _db.close();
    try {
      boost::filesystem::remove("unittest_db.sqlite");
    } catch (boost::system::system_error e) {
      std::cout << e.what() << std::endl;
    }
    _db.open("unittest_db.sqlite");
  };
  ~SerializationTest()
  {
    _db.close();
    try {
      boost::filesystem::remove("unittest_db.sqlite");
    } catch (boost::system::system_error e) {
      std::cout << e.what() << std::endl;
    }
  };

  virtual void SetUp();
  virtual void TearDown();

public:
  pfc::SQLite3Driver _db;
  std::filebuf file_buf;
  std::unique_ptr<pfc::schema::ScenarioSchema> scenario_schema;
  bool load_result = false;
};

void SQLiteDriver::SetUp()
{
}

void SQLiteDriver::TearDown()
{
}

void SerializationTest::SetUp()
{
  file_buf.open("Scenario.pfc.xml", std::ios::in);
  std::istream i_stream(&file_buf);
  try { // If the parsing fails this prints out every error
    scenario_schema = pfc::schema::Scenario(i_stream);
  } catch (const xml_schema::parsing& e) {
    std::cout << e << '\n';
    FAIL();
  }
}

void SerializationTest::TearDown()
{
  scenario_schema.release();
  file_buf.close();
}

//TEST_F(SQLiteDriver, DISABLED_saving)
//{
//  pfc::SQLite3Driver driver("pfc_sb_working.sqlite", "", nullptr);
//  pfc::Serializer serializer(nullptr);
//  serializer.save(&driver);
//}

TEST_F(SQLiteDriver, Constructor)
{
  {
    pfc::SQLite3Driver driver("UnitScenario.sqlite");
  }
  {
    auto file = std::ofstream("UnitScenario.sqlite", std::ostream::in);
    EXPECT_TRUE(file.is_open());
    file.close();
  }
  try {
    boost::filesystem::remove("UnitScenario.sqlite");
  } catch (boost::system::system_error e) {
    std::cout << e.what() << std::endl;
  }
  auto file2 = std::ofstream("UnitScenario.sqlite", std::ostream::in);
  EXPECT_FALSE(file2.is_open());
}

//ASSESSMENT TESTS----------------------un---------------------------------------
#ifndef DISABLE_ASSESSMENT_TEST
#define ASSESSMENT_TEST(X) X##_Assessment
#else
#define ASSESSMENT_TEST(X) DISABLED_##X##_Assessment
#endif
TEST_F(SQLiteDriver, ASSESSMENT_TEST(Insert))
{
  using namespace pfc;
  Assessment assessment_1; //   Assessment* assessment_1 = new Assessment(NULLPTR);
  Assessment assessment_2;
  Assessment assessment_3;

  assessment_1.name = "Math Test";
  assessment_1.description = "Gotta do some math";
  assessment_1.type = "Don't fail";
  assessment_1.available_points = 100;
  assessment_1.criteria = "what's 2+2";

  assessment_2.name = "English Test";
  assessment_2.description = "Write some English";
  assessment_2.type = "Moby Dick I guess";
  assessment_2.available_points = 200;
  assessment_2.criteria = "What's Moby Dick?";

  assessment_3.name = "History Test";
  assessment_3.description = "Who's George Washington";
  assessment_3.type = "Write who he is";
  assessment_3.available_points = 300;
  assessment_3.criteria = "yeah write it";

  EXPECT_EQ(0, _db.assessment_count());
  EXPECT_TRUE(_db.update_assessment(&assessment_1));
  EXPECT_EQ(1, _db.assessment_count());
  EXPECT_TRUE(_db.update_assessment(&assessment_2));
  EXPECT_EQ(2, _db.assessment_count());
  EXPECT_TRUE(_db.update_assessment(&assessment_3));
  EXPECT_EQ(3, _db.assessment_count());
}
TEST_F(SQLiteDriver, ASSESSMENT_TEST(Select))
{
  using namespace pfc;
  Assessment assessment_1;
  Assessment assessment_2;
  Assessment assessment_3;

  assessment_1.name = "Math Test";
  assessment_1.description = "Gotta do some math";
  assessment_1.type = "Don't fail";
  assessment_1.available_points = 100;
  assessment_1.criteria = "what's 2+2";

  assessment_2.name = "English Test";
  assessment_2.description = "Write some English";
  assessment_2.type = "Moby Dick I guess";
  assessment_2.available_points = 200;
  assessment_2.criteria = "What's Moby Dick?";

  assessment_3.name = "History Test";
  assessment_3.description = "Who's George Washington";
  assessment_3.type = "Write who he is";
  assessment_3.available_points = 300;
  assessment_3.criteria = "yeah write it";

  EXPECT_EQ(0, _db.assessment_count());
  EXPECT_TRUE(_db.update_assessment(&assessment_1));
  EXPECT_EQ(1, _db.assessment_count());
  EXPECT_TRUE(_db.update_assessment(&assessment_2));
  EXPECT_EQ(2, _db.assessment_count());
  EXPECT_TRUE(_db.update_assessment(&assessment_3));
  EXPECT_EQ(3, _db.assessment_count());

  Assessment id;
  Assessment name;

  id.id = 1;
  name.name = "History Test";

  _db.select_assessment(&id);
  _db.select_assessment(&name);

  assessment_1.id = 1;
  EXPECT_TRUE(assessment_1 == id);
  assessment_3.id = 3;
  EXPECT_TRUE(assessment_3 == name);
}
TEST_F(SQLiteDriver, ASSESSMENT_TEST(Remove))
{
  using namespace pfc;
  Assessment assessment_1;
  Assessment assessment_2;
  Assessment assessment_3;

  assessment_1.name = "Math Test";
  assessment_1.description = "Gotta do some math";
  assessment_1.type = "Don't fail";
  assessment_1.available_points = 100;
  assessment_1.criteria = "what's 2+2";

  assessment_2.name = "English Test";
  assessment_2.description = "Write some English";
  assessment_2.type = "Moby Dick I guess";
  assessment_2.available_points = 200;
  assessment_2.criteria = "What's Moby Dick?";

  assessment_3.name = "History Test";
  assessment_3.description = "Who's George Washington";
  assessment_3.type = "Write who he is";
  assessment_3.available_points = 300;
  assessment_3.criteria = "yeah write it";

  EXPECT_TRUE(_db.update_assessment(&assessment_1));
  EXPECT_TRUE(_db.update_assessment(&assessment_2));
  EXPECT_TRUE(_db.update_assessment(&assessment_3));
  EXPECT_EQ(3, _db.assessment_count());
  EXPECT_TRUE(_db.remove_assessment(&assessment_1));
  EXPECT_TRUE(_db.remove_assessment(&assessment_3));
  EXPECT_EQ(1, _db.assessment_count());
}
TEST_F(SQLiteDriver, ASSESSMENT_TEST(Equality))
{
  using namespace pfc;
  Assessment assessment_1;
  Assessment assessment_2;

  assessment_1.name = "Math Test";
  assessment_1.description = "Gotta do some math";
  assessment_1.type = "Don't fail";
  assessment_1.available_points = 100;
  assessment_1.criteria = "what's 2+2";

  assessment_2.name = "Math Test";

  EXPECT_TRUE(_db.update_assessment(&assessment_1));
  EXPECT_TRUE(_db.select_assessment(&assessment_2));
  EXPECT_TRUE(assessment_1 == assessment_2);
}
//AUTHOR TESTS--------------------------------------------------------------
#ifndef DISABLE_Author_TEST
#define AUTHOR_TEST(X) X##_Author
#else
#define AUTHOR_TEST(X) DISABLED_##X##_Author
#endif
TEST_F(SQLiteDriver, AUTHOR_TEST(Insert))
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

  EXPECT_EQ(0, _db.author_count());
  EXPECT_TRUE(_db.update_author(&author_1));
  EXPECT_EQ(1, _db.author_count());
  EXPECT_TRUE(_db.update_author(&author_2));
  EXPECT_EQ(2, _db.author_count());
  EXPECT_TRUE(_db.update_author(&author_3));
  EXPECT_EQ(3, _db.author_count());
}
TEST_F(SQLiteDriver, AUTHOR_TEST(Insert_First))
{
  using namespace pfc;
  Author author_1;
  Author author_2;
  Author author_3;

  author_1.first = "Solid";
  author_1.last = "Snake";
  author_1.email = "SolidSnake@MetalGear.com";
  author_1.zip = "55555";
  author_1.state = "NC";
  author_1.country = "USA";
  author_1.phone = "(555)555-5555";
  author_1.organization = "Foxhound";

  author_2.first = "Rocky";
  author_2.last = "Balboa";
  author_2.email = "ItalianStalion@moives.com";
  author_2.zip = "01010";
  author_2.state = "NC";
  author_2.country = "USA";
  author_2.phone = "(555)555-5555";
  author_2.organization = "Foxhound";

  author_3.email = "SolidSnake@MetalGear.com";

  EXPECT_EQ(0, _db.author_count());
  EXPECT_TRUE(_db.update_first_author(&author_1));
  author_1.id = 1;
  EXPECT_EQ(1, _db.author_count());
  _db.select_author(&author_3);
  EXPECT_TRUE(author_1 == author_3);

  author_3.first = "Rocky";
  author_3.last = "Balboa";
  author_3.email = "ItalianStalion@moives.com";

  EXPECT_TRUE(_db.update_first_author(&author_2));
  author_2.id = 1;
  EXPECT_EQ(1, _db.author_count());
  _db.select_author(&author_3);
  EXPECT_TRUE(author_2 == author_3);
}
TEST_F(SQLiteDriver, AUTHOR_TEST(Select))
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
  EXPECT_TRUE(author_1 == id);
  author_2.id = 2;
  EXPECT_TRUE(author_2 == email);
}
TEST_F(SQLiteDriver, AUTHOR_TEST(Remove))
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
  EXPECT_EQ(3, _db.author_count());
  EXPECT_TRUE(_db.remove_author(&author_1));
  EXPECT_TRUE(_db.remove_author(&author_2));
  EXPECT_EQ(1, _db.author_count());
}
TEST_F(SQLiteDriver, AUTHOR_TEST(Equality))
{
  using namespace pfc;
  Author author_1;
  Author author_2;

  author_1.first = "Solid";
  author_1.last = "Snake";
  author_1.email = "SolidSnake@MetalGear.com";

  author_2.email = "SolidSnake@MetalGear.com";

  EXPECT_TRUE(_db.update_author(&author_1));
  EXPECT_TRUE(_db.select_author(&author_2));
  //EXPECT_TRUE( author_1 == author_2 );
  EXPECT_EQ(author_1.id, author_2.id);
  EXPECT_EQ(author_1.first, author_2.first);
  EXPECT_EQ(author_1.middle, author_2.middle);
  EXPECT_EQ(author_1.last, author_2.last);
  EXPECT_EQ(author_1.email, author_2.email);
  EXPECT_EQ(author_1.zip, author_2.zip);
  EXPECT_EQ(author_1.plus_4, author_2.plus_4);
  EXPECT_EQ(author_1.state, author_2.state);
  EXPECT_EQ(author_1.country, author_2.country);
  EXPECT_EQ(author_1.phone, author_2.phone);
  EXPECT_EQ(author_1.organization, author_2.organization);
}
//CITATION TESTS--------------------------------------------------------------
#ifndef DISABLE_Citation_TEST
#define Citation_TEST(X) X##_Citation
#else
#define Citation_TEST(X) DISABLED_##X##_Citation
#endif
TEST_F(SQLiteDriver, Citation_TEST(Insert))
{
  using namespace pfc;
  Citation citation_1;
  Citation citation_2;
  Citation citation_3;

  citation_1.key = "skeleton key";
  citation_1.title = "Mr. Bones' Wild Ride";
  citation_1.authors = "Nathan;Angel";
  citation_1.year = "1000";
  citation_1.publisher = "Boneland";

  citation_2.key = "boss key";
  citation_2.title = "Legend of Zelda";
  citation_2.authors = "Austin;Matthew";
  citation_2.year = "2000";
  citation_2.publisher = "Nintendo";

  citation_3.key = "rsa key";
  citation_3.title = "Extra Security";
  citation_3.authors = "Steven;Lucas";
  citation_3.year = "2019";
  citation_3.publisher = "ARA";

  EXPECT_EQ(0, _db.citation_count());
  EXPECT_TRUE(_db.update_citation(&citation_1));
  EXPECT_EQ(1, _db.citation_count());
  EXPECT_TRUE(_db.update_citation(&citation_2));
  EXPECT_EQ(2, _db.citation_count());
  EXPECT_TRUE(_db.update_citation(&citation_3));
  EXPECT_EQ(3, _db.citation_count());
}
TEST_F(SQLiteDriver, Citation_TEST(Select))
{
  using namespace pfc;
  Citation citation_1;
  Citation citation_2;
  Citation citation_3;

  citation_1.key = "skeleton key";
  citation_1.title = "Mr. Bones' Wild Ride";
  citation_1.authors = "Nathan;Angel";
  citation_1.year = "1000";
  citation_1.publisher = "Boneland";

  citation_2.key = "boss key";
  citation_2.title = "Legend of Zelda";
  citation_2.authors = "Austin;Matthew";
  citation_2.year = "2000";
  citation_2.publisher = "Nintendo";

  citation_3.key = "rsa key";
  citation_3.title = "Extra Security";
  citation_3.authors = "Steven;Lucas";
  citation_3.year = "2019";
  citation_3.publisher = "ARA";

  EXPECT_EQ(0, _db.citation_count());
  EXPECT_TRUE(_db.update_citation(&citation_1));
  EXPECT_EQ(1, _db.citation_count());
  EXPECT_TRUE(_db.update_citation(&citation_2));
  EXPECT_EQ(2, _db.citation_count());
  EXPECT_TRUE(_db.update_citation(&citation_3));
  EXPECT_EQ(3, _db.citation_count());

  Citation id;
  Citation key;

  id.id = 1;
  key.key = "rsa key";

  _db.select_citation(&id);
  _db.select_citation(&key);

  citation_1.id = 1;
  EXPECT_TRUE(citation_1 == id);
  citation_3.id = 3;
  EXPECT_TRUE(citation_3 == key);
}
TEST_F(SQLiteDriver, Citation_TEST(Remove))
{
  using namespace pfc;
  Citation citation_1;
  Citation citation_2;
  Citation citation_3;

  citation_1.key = "skeleton key";
  citation_1.title = "Mr. Bones' Wild Ride";
  citation_1.authors = "Nathan;Angel";
  citation_1.year = "1000";
  citation_1.publisher = "Boneland";

  citation_2.key = "boss key";
  citation_2.title = "Legend of Zelda";
  citation_2.authors = "Austin;Matthew";
  citation_2.year = "2000";
  citation_2.publisher = "Nintendo";

  citation_3.key = "rsa key";
  citation_3.title = "Extra Security";
  citation_3.authors = "Steven;Lucas";
  citation_3.year = "2019";
  citation_3.publisher = "ARA";

  EXPECT_TRUE(_db.update_citation(&citation_1));
  EXPECT_TRUE(_db.update_citation(&citation_2));
  EXPECT_TRUE(_db.update_citation(&citation_3));
  EXPECT_EQ(3, _db.citation_count());
  EXPECT_TRUE(_db.remove_citation(&citation_1));
  EXPECT_TRUE(_db.remove_citation(&citation_3));
  EXPECT_EQ(1, _db.citation_count());
}
TEST_F(SQLiteDriver, Citation_TEST(Equality))
{
  using namespace pfc;
  Citation citation_1;
  Citation citation_2;

  citation_1.key = "skeleton key";
  citation_1.title = "Mr. Bones' Wild Ride";
  citation_1.authors = "White;Marin";
  citation_1.year = "1000";
  citation_1.publisher = "Boneland";

  citation_2.key = "skeleton key";

  EXPECT_TRUE(_db.update_citation(&citation_1));
  EXPECT_TRUE(_db.select_citation(&citation_2));
  EXPECT_TRUE(citation_1 == citation_2);
}
//EQUIPMENT TESTS--------------------------------------------------------------
#ifndef DISABLE_Equipment_TEST
#define Equipment_TEST(X) X##_Equipment
#else
#define Equipment_TEST(X) DISABLED_##X##_Equipment
#endif
TEST_F(SQLiteDriver, Equipment_TEST(Insert))
{
  using namespace pfc;
  Equipment equipment_1;
  Equipment equipment_2;
  Equipment equipment_3;

  equipment_1.name = "Keytar";
  equipment_1.type = 1;
  equipment_1.description = "Got a sick keytar solo later";
  equipment_1.image = ("music stand");
  equipment_1.citations = { 1 };

  equipment_2.name = "piano";
  equipment_2.type = 2;
  equipment_2.description = "big instrument with keys";
  equipment_2.image = ("piano bench");
  equipment_2.citations = { 2 };

  equipment_3.name = "bagpipes";
  equipment_3.type = 3;
  equipment_3.description = "please stop playing the bagpipes";
  equipment_3.image = ("a bladder");
  equipment_3.citations = { 3 };

  EXPECT_EQ(0, _db.equipment_count());
  EXPECT_TRUE(_db.update_equipment(&equipment_1));
  EXPECT_EQ(1, _db.equipment_count());
  EXPECT_TRUE(_db.update_equipment(&equipment_2));
  EXPECT_EQ(2, _db.equipment_count());
  EXPECT_TRUE(_db.update_equipment(&equipment_3));
  EXPECT_EQ(3, _db.equipment_count());
}
TEST_F(SQLiteDriver, Equipment_TEST(Select))
{
  using namespace pfc;
  Equipment equipment_1;
  Equipment equipment_2;
  Equipment equipment_3;

  equipment_1.name = "Keytar";
  equipment_1.type = 1;
  equipment_1.description = "Got a sick keytar solo later";
  equipment_1.image = ("music stand");
  equipment_1.citations = { 1 };

  equipment_2.name = "piano";
  equipment_2.type = 2;
  equipment_2.description = "big instrument with keys";
  equipment_2.image = ("piano bench");
  equipment_2.citations = { 2 };

  equipment_3.name = "bagpipes";
  equipment_3.type = 3;
  equipment_3.description = "please stop playing the bagpipes";
  equipment_3.image = ("a bladder");
  equipment_3.citations = { 3 };

  EXPECT_EQ(0, _db.equipment_count());
  EXPECT_TRUE(_db.update_equipment(&equipment_1));
  EXPECT_EQ(1, _db.equipment_count());
  EXPECT_TRUE(_db.update_equipment(&equipment_2));
  EXPECT_EQ(2, _db.equipment_count());
  EXPECT_TRUE(_db.update_equipment(&equipment_3));
  EXPECT_EQ(3, _db.equipment_count());

  Equipment id;
  Equipment name;

  id.id = 1;
  name.name = "bagpipes";

  _db.select_equipment(&id);
  _db.select_equipment(&name);

  equipment_1.id = 1;
  EXPECT_TRUE(equipment_1 == id);
  equipment_3.id = 3;
  EXPECT_TRUE(equipment_3 == name);
}
TEST_F(SQLiteDriver, Equipment_TEST(Remove))
{
  using namespace pfc;
  Equipment equipment_1;
  Equipment equipment_2;
  Equipment equipment_3;

  equipment_1.name = "Keytar";
  equipment_1.type = 1;
  equipment_1.description = "Got a sick keytar solo later";
  equipment_1.image = ("music stand");
  equipment_1.citations = { 1 };

  equipment_2.name = "piano";
  equipment_2.type = 2;
  equipment_2.description = "big instrument with keys";
  equipment_2.image = ("piano bench");
  equipment_2.citations = { 2 };

  equipment_3.name = "bagpipes";
  equipment_3.type = 3;
  equipment_3.description = "please stop playing the bagpipes";
  equipment_3.image = ("a bladder");
  equipment_3.citations = { 3 };

  EXPECT_TRUE(_db.update_equipment(&equipment_1));
  EXPECT_TRUE(_db.update_equipment(&equipment_2));
  EXPECT_TRUE(_db.update_equipment(&equipment_3));
  EXPECT_EQ(3, _db.equipment_count());
  EXPECT_TRUE(_db.remove_equipment(&equipment_1));
  EXPECT_TRUE(_db.remove_equipment(&equipment_3));
  EXPECT_EQ(1, _db.equipment_count());
}
TEST_F(SQLiteDriver, Equipment_TEST(Equality))
{
  using namespace pfc;
  Equipment equipment_1;
  Equipment equipment_2;

  equipment_1.name = "Keytar";
  equipment_1.type = 1;
  equipment_1.description = "Got a sick keytar solo later";
  equipment_1.image = ("music stand");
  equipment_1.citations = { 1 };

  equipment_2.name = "Keytar";

  EXPECT_TRUE(_db.update_equipment(&equipment_1));
  EXPECT_TRUE(_db.select_equipment(&equipment_2));
  EXPECT_TRUE(equipment_1 == equipment_2);
}
//EVENT TESTS--------------------------------------------------------------
#ifndef DISABLE_Event_TEST
#define Event_TEST(X) X##_Event
#else
#define Event_TEST(X) DISABLED_##X##_Event
#endif
TEST_F(SQLiteDriver, Event_TEST(Insert))
{

  //TODO: fill out the Event object and make this test work
  // So this is currently failing, but I don't really know what we're going to do with events
  // As such I'm going to leave this alone until I do know
  using namespace pfc;
  Event event_1;
  Event event_2;
  Event event_3;

  event_1.name = "Barbecue";
  event_1.description = "Barbecue Barbecue";
  event_1.category = "DIALOG";
  event_1.fidelity = "ACTION";
  event_1.fk_actor_1 = "2";
  event_1.fk_actor_2 = "3";
  event_1.equipment = "5;6;7";
  event_1.details = "People cooking and eating meat outdoors";

  event_2.name = "Wedding";
  event_2.description = "Wedding Barbecue";
  event_2.category = "DIALOG";
  event_2.fidelity = "ACTION";
  event_2.fk_actor_1 = "4";
  event_2.fk_actor_2 = "5";
  event_2.equipment = "3;4;5";
  event_2.details = "Two people getting married";

  event_3.name = "Funeral";
  event_3.description = "Funeral Barbecue";
  event_3.category = "DIALOG";
  event_3.fidelity = "ACTION";
  event_3.fk_actor_1 = "6";
  event_3.fk_actor_2 = "7";
  event_3.equipment = "1;2;3";
  event_3.details = "Burying a dead person";

  EXPECT_EQ(0, _db.event_count());
  EXPECT_TRUE(_db.update_event(&event_1));
  EXPECT_EQ(1, _db.event_count());
  EXPECT_TRUE(_db.update_event(&event_2));
  EXPECT_EQ(2, _db.event_count());
  EXPECT_TRUE(_db.update_event(&event_3));
  EXPECT_EQ(3, _db.event_count());
}
TEST_F(SQLiteDriver, Event_TEST(Select))
{
  using namespace pfc;
  Event event_1;
  Event event_2;
  Event event_3;

  event_1.name = "Barbecue";
  event_1.description = "Barbecue Barbecue";
  event_1.category = "DIALOG";
  event_1.fidelity = "ACTION";
  event_1.fk_actor_1 = "2";
  event_1.fk_actor_2 = "3";
  event_1.equipment = "5;6;7";
  event_1.details = "People cooking and eating meat outdoors";

  event_2.name = "Wedding";
  event_2.description = "Wedding Barbecue";
  event_2.category = "DIALOG";
  event_2.fidelity = "ACTION";
  event_2.fk_actor_1 = "4";
  event_2.fk_actor_2 = "5";
  event_2.equipment = "3;4;5";
  event_2.details = "Two people getting married";

  event_3.name = "Funeral";
  event_3.description = "Funeral Barbecue";
  event_3.category = "DIALOG";
  event_3.fidelity = "ACTION";
  event_3.fk_actor_1 = "6";
  event_3.fk_actor_2 = "7";
  event_3.equipment = "1;2;3";
  event_3.details = "Burying a dead person";

  EXPECT_EQ(0, _db.event_count());
  EXPECT_TRUE(_db.update_event(&event_1));
  EXPECT_EQ(1, _db.event_count());
  EXPECT_TRUE(_db.update_event(&event_2));
  EXPECT_EQ(2, _db.event_count());
  EXPECT_TRUE(_db.update_event(&event_3));
  EXPECT_EQ(3, _db.event_count());

  Event id;
  Event name;

  id.id = 1;
  name.name = "Funeral";

  _db.select_event(&id);
  _db.select_event(&name);
  event_1.id = 1;
  event_3.id = 3;
  EXPECT_TRUE(event_1 == id);
  EXPECT_TRUE(event_3 == name);
}
TEST_F(SQLiteDriver, Event_TEST(Remove))
{
  using namespace pfc;
  Event event_1;
  Event event_2;
  Event event_3;

  event_1.name = "Barbecue";
  event_1.description = "Barbecue Barbecue";
  event_1.category = "DIALOG";
  event_1.fidelity = "ACTION";
  event_1.fk_actor_1 = "2";
  event_1.fk_actor_2 = "3";
  event_1.equipment = "5;6;7";
  event_1.details = "People cooking and eating meat outdoors";

  event_2.name = "Wedding";
  event_2.description = "Wedding Barbecue";
  event_2.category = "DIALOG";
  event_2.fidelity = "ACTION";
  event_2.fk_actor_1 = "4";
  event_2.fk_actor_2 = "5";
  event_2.equipment = "3;4;5";
  event_2.details = "Two people getting married";

  event_3.name = "Funeral";
  event_3.description = "Funeral Barbecue";
  event_3.category = "DIALOG";
  event_3.fidelity = "ACTION";
  event_3.fk_actor_1 = "6";
  event_3.fk_actor_2 = "7";
  event_3.equipment = "1;2;3";
  event_3.details = "Burying a dead person";

  EXPECT_TRUE(_db.update_event(&event_1));
  EXPECT_TRUE(_db.update_event(&event_2));
  EXPECT_TRUE(_db.update_event(&event_3));
  EXPECT_EQ(3, _db.event_count());
  EXPECT_TRUE(_db.remove_event(&event_1));
  EXPECT_TRUE(_db.remove_event(&event_3));
  EXPECT_EQ(1, _db.event_count());
}
TEST_F(SQLiteDriver, Event_TEST(Equality))
{
  using namespace pfc;
  Event event_1;
  Event event_2;

  event_1.name = "Barbecue";
  event_1.description = "Barbecue Barbecue";
  event_1.category = "DIALOG";
  event_1.fidelity = "ACTION";
  event_1.fk_actor_1 = "2";
  event_1.fk_actor_2 = "3";
  event_1.equipment = "5;6;7";
  event_1.details = "People cooking and eating meat outdoors";

  event_2.name = "Barbecue";

  EXPECT_TRUE(_db.update_event(&event_1));
  EXPECT_TRUE(_db.select_event(&event_2));
  EXPECT_TRUE(event_1 == event_2);
}
//INJURY TESTS--------------------------------------------------------------
#ifndef DISABLE_Injury_TEST
#define Injury_TEST(X) X##_Injury
#else
#define Injury_TEST(X) DISABLED_##X##_Injury
#endif
TEST_F(SQLiteDriver, Injury_TEST(Insert))
{
  using namespace pfc;
  Injury injury_1;
  Injury injury_2;
  Injury injury_3;

  injury_1.medical_name = "Keyboardus Faceus";
  injury_1.common_name = "Keyboard Face";
  injury_1.description = "Looks like you took a nap on your keyboard";
  injury_1.citations = { 1 };

  injury_2.medical_name = "Hangnailus";
  injury_2.common_name = "Hangnail";
  injury_2.description = "Ouch, really stings";
  injury_2.citations = { 2 };

  injury_3.medical_name = "Stubbedus Toeus";
  injury_3.common_name = "Stubbed toe";
  injury_3.description = "ouchie";
  injury_3.citations = { 3 };

  EXPECT_EQ(0, _db.injury_count());
  EXPECT_TRUE(_db.update_injury(&injury_1));
  EXPECT_EQ(1, _db.injury_count());
  EXPECT_TRUE(_db.update_injury(&injury_2));
  EXPECT_EQ(2, _db.injury_count());
  EXPECT_TRUE(_db.update_injury(&injury_3));
  EXPECT_EQ(3, _db.injury_count());
}
TEST_F(SQLiteDriver, Injury_TEST(Select))
{
  using namespace pfc;
  Injury injury_1;
  Injury injury_2;
  Injury injury_3;

  injury_1.medical_name = "Keyboardus Faceus";
  injury_1.common_name = "Keyboard Face";
  injury_1.description = "Looks like you took a nap on your keyboard";
  injury_1.citations = { 1 };

  injury_2.medical_name = "Hangnailus";
  injury_2.common_name = "Hangnail";
  injury_2.description = "Ouch, really stings";
  injury_2.citations = { 2 };

  injury_3.medical_name = "Stubbedus Toeus";
  injury_3.common_name = "Stubbed toe";
  injury_3.description = "ouchie";
  injury_3.citations = { 3 };

  EXPECT_EQ(0, _db.injury_count());
  EXPECT_TRUE(_db.update_injury(&injury_1));
  EXPECT_EQ(1, _db.injury_count());
  EXPECT_TRUE(_db.update_injury(&injury_2));
  EXPECT_EQ(2, _db.injury_count());
  EXPECT_TRUE(_db.update_injury(&injury_3));
  EXPECT_EQ(3, _db.injury_count());

  Injury id;
  Injury medical_name;
  Injury common_name;

  id.id = 1;
  medical_name.medical_name = "Hangnailus";
  common_name.common_name = "Stubbed toe";

  _db.select_injury(&id);
  _db.select_injury(&medical_name);
  _db.select_injury(&common_name);

  injury_1.id = 1;
  EXPECT_TRUE(injury_1 == id);
  injury_2.id = 2;
  EXPECT_TRUE(injury_2 == medical_name);
  injury_3.id = 3;
  EXPECT_TRUE(injury_3 == common_name);
}
TEST_F(SQLiteDriver, Injury_TEST(Remove))
{
  using namespace pfc;
  Injury injury_1;
  Injury injury_2;
  Injury injury_3;

  injury_1.medical_name = "Keyboardus Faceus";
  injury_1.common_name = "Keyboard Face";
  injury_1.description = "Looks like you took a nap on your keyboard";
  injury_1.citations = { 1 };

  injury_2.medical_name = "Hangnailus";
  injury_2.common_name = "Hangnail";
  injury_2.description = "Ouch, really stings";
  injury_2.citations = { 2 };

  injury_3.medical_name = "Stubbedus Toeus";
  injury_3.common_name = "Stubbed toe";
  injury_3.description = "ouchie";
  injury_3.citations = { 3 };

  EXPECT_TRUE(_db.update_injury(&injury_1));
  EXPECT_TRUE(_db.update_injury(&injury_2));
  EXPECT_TRUE(_db.update_injury(&injury_3));
  EXPECT_EQ(3, _db.injury_count());
  EXPECT_TRUE(_db.remove_injury(&injury_1));
  EXPECT_TRUE(_db.remove_injury(&injury_3));
  EXPECT_EQ(1, _db.injury_count());
}
TEST_F(SQLiteDriver, Injury_TEST(Equality))
{
  using namespace pfc;
  Injury injury_1;
  Injury injury_2;

  injury_1.medical_name = "Keyboardus Faceus";
  injury_1.common_name = "Keyboard Face";
  injury_1.description = "Looks like you took a nap on your keyboard";
  injury_1.citations = { 1 };

  injury_2.medical_name = "Keyboardus Faceus";

  EXPECT_TRUE(_db.update_injury(&injury_1));
  EXPECT_TRUE(_db.select_injury(&injury_2));
  EXPECT_TRUE(injury_1 == injury_2);
}
//INJURY SET TESTS--------------------------------------------------------------
#ifndef DISABLE_InjurySet_TEST
#define InjurySet_TEST(X) X##_InjurySet
#else
#define InjurySet_TEST(X) DISABLED_##X##_InjurySet
#endif
TEST_F(SQLiteDriver, InjurySet_TEST(Insert))
{
  using namespace pfc;
  InjurySet injury_set_1;
  InjurySet injury_set_2;
  InjurySet injury_set_3;

  injury_set_1.name = "Keyboardus Faceus";
  injury_set_1.description = "Looks like you took a nap on your keyboard";
  injury_set_1.injuries = { 1 };
  injury_set_1.locations = { 1 };
  injury_set_1.severities = { 1 };

  injury_set_2.name = "Hangnailus";
  injury_set_2.description = "Ouch, really stings";
  injury_set_2.injuries = { 2 };
  injury_set_2.locations = { 2 };
  injury_set_2.severities = { 2 };

  injury_set_3.name = "Stubbedus Toeus";
  injury_set_3.description = "ouchie";
  injury_set_3.injuries = { 3 };
  injury_set_3.locations = { 3 };
  injury_set_3.severities = { 3 };

  EXPECT_EQ(0, _db.injury_set_count());
  EXPECT_TRUE(_db.update_injury_set(&injury_set_1));
  EXPECT_EQ(1, _db.injury_set_count());
  EXPECT_TRUE(_db.update_injury_set(&injury_set_2));
  EXPECT_EQ(2, _db.injury_set_count());
  EXPECT_TRUE(_db.update_injury_set(&injury_set_3));
  EXPECT_EQ(3, _db.injury_set_count());
}
TEST_F(SQLiteDriver, InjurySet_TEST(Select))
{
  using namespace pfc;
  InjurySet injury_set_1;
  InjurySet injury_set_2;
  InjurySet injury_set_3;

  injury_set_1.name = "Keyboardus Faceus";
  injury_set_1.description = "Looks like you took a nap on your keyboard";
  injury_set_1.injuries = { 1 };
  injury_set_1.locations = { 1 };
  injury_set_1.severities = { 1 };

  injury_set_2.name = "Hangnailus";
  injury_set_2.description = "Ouch, really stings";
  injury_set_2.injuries = { 2 };
  injury_set_2.locations = { 2 };
  injury_set_2.severities = { 2 };

  injury_set_3.name = "Stubbedus Toeus";
  injury_set_3.description = "ouchie";
  injury_set_3.injuries = { 3 };
  injury_set_3.locations = { 3 };
  injury_set_3.severities = { 3 };

  EXPECT_EQ(0, _db.injury_set_count());
  EXPECT_TRUE(_db.update_injury_set(&injury_set_1));
  EXPECT_EQ(1, _db.injury_set_count());
  EXPECT_TRUE(_db.update_injury_set(&injury_set_2));
  EXPECT_EQ(2, _db.injury_set_count());
  EXPECT_TRUE(_db.update_injury_set(&injury_set_3));
  EXPECT_EQ(3, _db.injury_set_count());

  InjurySet id;
  InjurySet name;
  InjurySet description;

  id.id = 1;
  name.name = "Hangnailus";

  _db.select_injury_set(&id);
  _db.select_injury_set(&name);

  injury_set_1.id = 1;
  EXPECT_TRUE(injury_set_1 == id);
  injury_set_2.id = 2;
  EXPECT_TRUE(injury_set_2 == name);
}
TEST_F(SQLiteDriver, InjurySet_TEST(Remove))
{
  using namespace pfc;
  InjurySet injury_set_1;
  InjurySet injury_set_2;
  InjurySet injury_set_3;

  injury_set_1.name = "Keyboardus Faceus";
  injury_set_1.description = "Looks like you took a nap on your keyboard";
  injury_set_1.injuries = { 1 };
  injury_set_1.locations = { 1 };
  injury_set_1.severities = { 1 };

  injury_set_2.name = "Hangnailus";
  injury_set_2.description = "Ouch, really stings";
  injury_set_2.injuries = { 2 };
  injury_set_2.locations = { 2 };
  injury_set_2.severities = { 2 };

  injury_set_3.name = "Stubbedus Toeus";
  injury_set_3.description = "ouchie";
  injury_set_3.injuries = { 3 };
  injury_set_3.locations = { 3 };
  injury_set_3.severities = { 3 };

  EXPECT_TRUE(_db.update_injury_set(&injury_set_1));
  EXPECT_TRUE(_db.update_injury_set(&injury_set_2));
  EXPECT_TRUE(_db.update_injury_set(&injury_set_3));
  EXPECT_EQ(3, _db.injury_set_count());
  EXPECT_TRUE(_db.remove_injury_set(&injury_set_1));
  EXPECT_TRUE(_db.remove_injury_set(&injury_set_3));
  EXPECT_EQ(1, _db.injury_set_count());
}
TEST_F(SQLiteDriver, InjurySet_TEST(Equality))
{
  using namespace pfc;
  InjurySet injury_set_1;
  InjurySet injury_set_2;

  injury_set_1.name = "Keyboardus Faceus";
  injury_set_1.description = "Looks like you took a nap on your keyboard";
  injury_set_1.injuries = { 1 };
  injury_set_1.severities = { 2 };
  injury_set_1.locations = { 3 };

  injury_set_2.name = "Keyboardus Faceus";

  EXPECT_TRUE(_db.update_injury_set(&injury_set_1));
  EXPECT_TRUE(_db.select_injury_set(&injury_set_2));
  EXPECT_TRUE(injury_set_1 == injury_set_2);
}
//LOCATION TESTS--------------------------------------------------------------
#ifndef DISABLE_Location_TEST
#define Location_TEST(X) X##_Location
#else
#define Location_TEST(X) DISABLED_##X##_Location
#endif
TEST_F(SQLiteDriver, Location_TEST(Insert))
{
  using namespace pfc;
  Location location_1;
  Location location_2;
  Location location_3;

  location_1.name = "My House";
  location_1.environment = "Bathroom floor";

  location_2.name = "Work Building";
  location_2.environment = "Office Floor";

  location_3.name = "Restaurant";
  location_3.environment = "Booth";

  EXPECT_EQ(0, _db.location_count());
  EXPECT_TRUE(_db.update_location(&location_1));
  EXPECT_EQ(1, _db.location_count());
  EXPECT_TRUE(_db.update_location(&location_2));
  EXPECT_EQ(2, _db.location_count());
  EXPECT_TRUE(_db.update_location(&location_3));
  EXPECT_EQ(3, _db.location_count());
}
TEST_F(SQLiteDriver, Location_TEST(Select))
{
  using namespace pfc;
  Location location_1;
  Location location_2;
  Location location_3;

  location_1.name = "My House";
  location_1.environment = "Bathroom floor";

  location_2.name = "Work Building";
  location_2.environment = "Office Floor";

  EXPECT_EQ(0, _db.location_count());
  EXPECT_TRUE(_db.update_location(&location_1));
  EXPECT_EQ(1, _db.location_count());
  EXPECT_TRUE(_db.update_location(&location_2));
  EXPECT_EQ(2, _db.location_count());

  Location id;
  Location name;
  Location scene_name;

  id.id = 1;
  name.name = "Work Building";

  _db.select_location(&id);
  _db.select_location(&name);

  location_1.id = 1;
  EXPECT_TRUE(location_1 == id);
  location_2.id = 2;
  EXPECT_TRUE(location_2 == name);
}
TEST_F(SQLiteDriver, Location_TEST(Remove))
{
  using namespace pfc;
  Location location_1;
  Location location_2;
  Location location_3;

  location_1.name = "My House";
  location_1.environment = "Bathroom floor";

  location_2.name = "Work Building";
  location_2.environment = "Office Floor";

  location_3.name = "Restaurant";
  location_3.environment = "Booth";

  EXPECT_TRUE(_db.update_location(&location_1));
  EXPECT_TRUE(_db.update_location(&location_2));
  EXPECT_TRUE(_db.update_location(&location_3));
  EXPECT_EQ(3, _db.location_count());
  EXPECT_TRUE(_db.remove_location(&location_1));
  EXPECT_TRUE(_db.remove_location(&location_3));
  EXPECT_EQ(1, _db.location_count());
}
TEST_F(SQLiteDriver, Location_TEST(Equality))
{
  using namespace pfc;
  Location location_1;
  Location location_2;

  location_1.name = "My House";
  location_1.environment = "Bathroom floor";

  location_2.name = "My House";

  EXPECT_TRUE(_db.update_location(&location_1));
  EXPECT_TRUE(_db.select_location(&location_2));
  EXPECT_TRUE(location_1 == location_2);
}

//!
//!  Test for Relationship Map Tables
//!
//

//OBJECTIVE TESTS--------------------------------------------------------------
#ifndef DISABLE_Objectives_TEST
#define Objectives_TEST(X) X##_Objectives
#else
#define Objectives_TEST(X) DISABLED_##X##_Objectives
#endif
TEST_F(SQLiteDriver, Objectives_TEST(Insert))
{
  using namespace pfc;
  Objective objective_1;
  Objective objective_2;
  Objective objective_3;

  objective_1.name = "Kill the Troll";
  objective_1.description = "There is a troll in the forest in a great big whole who has some gold. Kill it";
  objective_1.citations = "1;2;3";

  objective_2.name = "Steal the Trolls Gold";
  objective_2.description = "Gold is really dense. Its like crazy heavy bring a levetation spell.";
  objective_2.citations = "4;5;6";

  objective_3.name = "Return the Gold to the Quest Giver. Kill the Quest Giver";
  objective_3.description = "If we want to keep the gold we need to steal from the quest giver, but he will need to die before we can do that.";
  objective_3.citations = "7;8;9";

  EXPECT_EQ(0, _db.objective_count());
  EXPECT_TRUE(_db.update_objective(&objective_1));
  EXPECT_EQ(1, _db.objective_count());
  EXPECT_TRUE(_db.update_objective(&objective_2));
  EXPECT_EQ(2, _db.objective_count());
  EXPECT_TRUE(_db.update_objective(&objective_3));
  EXPECT_EQ(3, _db.objective_count());
}
TEST_F(SQLiteDriver, Objectives_TEST(Select))
{
  using namespace pfc;
  Objective objective_1;
  Objective objective_2;
  Objective objective_3;

  objective_1.name = "Kill the Troll";
  objective_1.description = "There is a troll in the forest in a great big whole who has some gold. Kill it";
  objective_1.citations = "1;2;3";

  objective_2.name = "Steal the Trolls Gold";
  objective_2.description = "Gold is really dense. Its like crazy heavy bring a levetation spell.";
  objective_2.citations = "4;5;6";

  objective_3.name = "Return the Gold to the Quest Giver. Kill the Quest Giver";
  objective_3.description = "If we want to keep the gold we need to steal from the quest giver, but he will need to die before we can do that.";
  objective_3.citations = "7;8;9";

  EXPECT_EQ(0, _db.objective_count());
  EXPECT_TRUE(_db.update_objective(&objective_1));
  EXPECT_EQ(1, _db.objective_count());
  EXPECT_TRUE(_db.update_objective(&objective_2));
  EXPECT_EQ(2, _db.objective_count());
  EXPECT_TRUE(_db.update_objective(&objective_3));
  EXPECT_EQ(3, _db.objective_count());

  Objective id;
  Objective name;

  id.id = 1;
  name.name = "Return the Gold to the Quest Giver. Kill the Quest Giver";

  _db.select_objective(&id);
  _db.select_objective(&name);

  objective_1.id = 1;
  EXPECT_TRUE(objective_1 == id);
  objective_3.id = 3;
  EXPECT_TRUE(objective_3 == name);
}
TEST_F(SQLiteDriver, Objectives_TEST(Remove))
{
  using namespace pfc;
  Objective objective_1;
  Objective objective_2;
  Objective objective_3;

  objective_1.name = "Kill the Troll";
  objective_1.description = "There is a troll in the forest in a great big whole who has some gold. Kill it";
  objective_1.citations = "1;2;3";

  objective_2.name = "Steal the Trolls Gold";
  objective_2.description = "Gold is really dense. Its like crazy heavy bring a levetation spell.";
  objective_2.citations = "4;5;6";

  objective_3.name = "Return the Gold to the Quest Giver. Kill the Quest Giver";
  objective_3.description = "If we want to keep the gold we need to steal from the quest giver, but he will need to die before we can do that.";
  objective_3.citations = "7;8;9";

  EXPECT_TRUE(_db.update_objective(&objective_1));
  EXPECT_TRUE(_db.update_objective(&objective_2));
  EXPECT_TRUE(_db.update_objective(&objective_3));
  EXPECT_EQ(3, _db.objective_count());
  EXPECT_TRUE(_db.remove_objective(&objective_1));
  EXPECT_TRUE(_db.remove_objective(&objective_3));
  EXPECT_EQ(1, _db.objective_count());
}
TEST_F(SQLiteDriver, Objectives_TEST(Equality))
{
  using namespace pfc;
  Objective objective_1;
  Objective objective_2;

  objective_1.name = "Kill the Troll";
  objective_1.description = "There is a troll in the forest in a great big whole who has some gold. Kill it";
  objective_1.citations = "1;2;3";

  objective_2.name = "Kill the Troll";

  EXPECT_TRUE(_db.update_objective(&objective_1));
  EXPECT_TRUE(_db.select_objective(&objective_2));
  EXPECT_TRUE(objective_1 == objective_2);
}
//PROPERTY TESTS--------------------------------------------------------------
#ifndef DISABLE_Property_TEST
#define Property_TEST(X) X##_Property
#else
#define Property_TEST(X) DISABLED_##X##_Property
#endif
TEST_F(SQLiteDriver, Property_TEST(Insert))
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

  EXPECT_EQ(0, _db.property_count());
  EXPECT_TRUE(_db.update_property(&property_1));
  EXPECT_EQ(1, _db.property_count());
  EXPECT_TRUE(_db.update_property(&property_2));
  EXPECT_EQ(2, _db.property_count());
  EXPECT_TRUE(_db.update_property(&property_3));
  EXPECT_EQ(3, _db.property_count());
}
TEST_F(SQLiteDriver, Property_TEST(Select))
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
  EXPECT_TRUE(property_1 == id);
  property_2.id = 2;
  EXPECT_TRUE(property_2 == name);
}
TEST_F(SQLiteDriver, Property_TEST(Remove))
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
  EXPECT_EQ(3, _db.property_count());
  EXPECT_TRUE(_db.remove_property(&property_1));
  EXPECT_TRUE(_db.remove_property(&property_3));
  EXPECT_EQ(1, _db.property_count());
}
TEST_F(SQLiteDriver, Property_TEST(Equality))
{
  using namespace pfc;
  Property property_1;
  Property property_2;

  property_1.name = "Solid";
  property_1.value = "Snake";

  property_2.name = "Solid";

  EXPECT_TRUE(_db.update_property(&property_1));
  EXPECT_TRUE(_db.select_property(&property_2));
  EXPECT_TRUE(property_1 == property_2);
}
//ROLE TESTS------------------------------------------------------------------
#ifndef DISABLE_Role_TEST
#define Role_TEST(X) X##_Role
#else
#define Role_TEST(X) DISABLED_##X##_Role
#endif
TEST_F(SQLiteDriver, Role_TEST(Insert))
{
  using namespace pfc;
  Role role_1;
  Role role_2;
  Role role_3;

  role_1.name = "Software Developer";
  role_1.description = "Bash your head against a wall for 8 hours a day";

  role_2.name = "Exterminator";
  role_2.description = "Look for bugs 8 hours a day";

  role_3.name = "Tailor";
  role_3.description = "Make quick patches 8 hours a day";

  EXPECT_EQ(0, _db.role_count());
  EXPECT_TRUE(_db.update_role(&role_1));
  EXPECT_EQ(1, _db.role_count());
  EXPECT_TRUE(_db.update_role(&role_2));
  EXPECT_EQ(2, _db.role_count());
  EXPECT_TRUE(_db.update_role(&role_3));
  EXPECT_EQ(3, _db.role_count());
}
TEST_F(SQLiteDriver, Role_TEST(Select))
{
  using namespace pfc;
  Role role_1;
  Role role_2;
  Role role_3;

  role_1.name = "Software Developer";
  role_1.description = "Bash your head against a wall for 8 hours a day";

  role_2.name = "Exterminator";
  role_2.description = "Look for bugs 8 hours a day";

  role_3.name = "Tailor";
  role_3.description = "Make quick patches 8 hours a day";

  EXPECT_EQ(0, _db.role_count());
  EXPECT_TRUE(_db.update_role(&role_1));
  EXPECT_EQ(1, _db.role_count());
  EXPECT_TRUE(_db.update_role(&role_2));
  EXPECT_EQ(2, _db.role_count());
  EXPECT_TRUE(_db.update_role(&role_3));
  EXPECT_EQ(3, _db.role_count());

  Role id;
  Role name;

  id.id = 1;
  name.name = "Tailor";

  _db.select_role(&id);
  _db.select_role(&name);

  role_1.id = 1;
  EXPECT_TRUE(role_1 == id);
  role_3.id = 3;
  EXPECT_TRUE(role_3 == name);
}
TEST_F(SQLiteDriver, Role_TEST(Remove))
{
  using namespace pfc;
  Role role_1;
  Role role_2;
  Role role_3;

  role_1.name = "Software Developer";
  role_1.description = "Bash your head against a wall for 8 hours a day";

  role_2.name = "Exterminator";
  role_2.description = "Look for bugs 8 hours a day";

  role_3.name = "Tailor";
  role_3.description = "Make quick patches 8 hours a day";

  EXPECT_TRUE(_db.update_role(&role_1));
  EXPECT_TRUE(_db.update_role(&role_2));
  EXPECT_TRUE(_db.update_role(&role_3));
  EXPECT_EQ(3, _db.role_count());
  EXPECT_TRUE(_db.remove_role(&role_1));
  EXPECT_TRUE(_db.remove_role(&role_3));
  EXPECT_EQ(1, _db.role_count());
}
TEST_F(SQLiteDriver, Role_TEST(Equality))
{
  using namespace pfc;
  Role role_1;
  Role role_2;

  role_1.name = "Software Developer";
  role_1.description = "Bash your head against a wall for 8 hours a day";

  role_2.name = "Software Developer";

  EXPECT_TRUE(_db.update_role(&role_1));
  EXPECT_TRUE(_db.select_role(&role_2));
  EXPECT_TRUE(role_1 == role_2);
}
//SCENE TESTS----------------------------------------------------------------
#ifndef DISABLE_Scene_TEST
#define Scene_TEST(X) X##_Scene
#else
#define Scene_TEST(X) DISABLED_##X##_Scene
#endif
TEST_F(SQLiteDriver, Scene_TEST(Insert))
{
  using namespace pfc;
  Scene scene_1;
  Scene scene_2;
  Scene scene_3;

  scene_1.name = "Opening";

  scene_2.name = "Middle";

  scene_3.name = "Ending";

  EXPECT_EQ(0, _db.scene_count());
  EXPECT_TRUE(_db.update_scene(&scene_1));
  EXPECT_EQ(1, _db.scene_count());
  EXPECT_TRUE(_db.update_scene(&scene_2));
  EXPECT_EQ(2, _db.scene_count());
  EXPECT_TRUE(_db.update_scene(&scene_3));
  EXPECT_EQ(3, _db.scene_count());
}
TEST_F(SQLiteDriver, Scene_TEST(Select))
{
  using namespace pfc;
  Scene scene_1;
  Scene scene_2;
  Scene scene_3;

  scene_1.name = "Opening";

  scene_2.name = "Middle";

  scene_3.name = "Ending";

  EXPECT_EQ(0, _db.scene_count());
  EXPECT_TRUE(_db.update_scene(&scene_1));
  EXPECT_EQ(1, _db.scene_count());
  EXPECT_TRUE(_db.update_scene(&scene_2));
  EXPECT_EQ(2, _db.scene_count());
  EXPECT_TRUE(_db.update_scene(&scene_3));
  EXPECT_EQ(3, _db.scene_count());

  Scene id;
  Scene name;

  id.id = 1;
  name.name = "Middle";

  _db.select_scene(&id);
  _db.select_scene(&name);

  scene_1.id = 1;
  EXPECT_TRUE(scene_1 == id);
  scene_2.id = 2;
  EXPECT_TRUE(scene_2 == name);
}
TEST_F(SQLiteDriver, Scene_TEST(Remove))
{
  using namespace pfc;
  Scene scene_1;
  Scene scene_2;
  Scene scene_3;

  scene_1.name = "Opening";

  scene_2.name = "Middle";

  scene_3.name = "Ending";

  EXPECT_TRUE(_db.update_scene(&scene_1));
  EXPECT_TRUE(_db.update_scene(&scene_2));
  EXPECT_TRUE(_db.update_scene(&scene_3));
  EXPECT_EQ(3, _db.scene_count());
  EXPECT_TRUE(_db.remove_scene(&scene_1));
  EXPECT_TRUE(_db.remove_scene(&scene_3));
  EXPECT_EQ(1, _db.scene_count());
}
TEST_F(SQLiteDriver, Scene_TEST(Equality))
{
  using namespace pfc;
  Scene scene_1;
  Scene scene_2;
  Scene scene_3;

  scene_1.name = "Opening";
  scene_1.description = "Opening";
  scene_1.time_of_day = "Opening";
  scene_1.time_in_simulation = 60;
  scene_1.items = "Opening";
  scene_1.details = "Opening";

  scene_2.name = "Opening";

  EXPECT_TRUE(_db.update_scene(&scene_1));
  EXPECT_TRUE(_db.select_scene(&scene_2));
  EXPECT_TRUE(scene_1 == scene_2);
}
//TREATMENT TESTS--------------------------------------------------------------
#ifndef DISABLE_Treatment_TEST
#define Treatment_TEST(X) X##_Treatment
#else
#define Treatment_TEST(X) DISABLED_##X##_Treatment
#endif
TEST_F(SQLiteDriver, Treatment_TEST(Insert))
{
  using namespace pfc;
  Treatment treatment_1;
  Treatment treatment_2;
  Treatment treatment_3;

  treatment_1.medical_name = "Soupus De Gallinus";
  treatment_1.common_name = "Chicken Soup";
  treatment_1.description = "Good for the soul";
  treatment_1.equipment = { 2 };
  treatment_1.citations = { 1 };

  treatment_2.medical_name = "Massageus";
  treatment_2.common_name = "Massage";
  treatment_2.description = "Back rub";
  treatment_2.equipment = { 3 };
  treatment_2.citations = { 2 };

  treatment_3.medical_name = "Bashus Headus againstus Wallus";
  treatment_3.common_name = "Bashing Head Against A Wall";
  treatment_3.description = "Oddly Satisfying";
  treatment_3.equipment = { 4 };
  treatment_3.citations = { 3 };

  EXPECT_EQ(0, _db.treatment_count());
  EXPECT_TRUE(_db.update_treatment(&treatment_1));
  EXPECT_EQ(1, _db.treatment_count());
  EXPECT_TRUE(_db.update_treatment(&treatment_2));
  EXPECT_EQ(2, _db.treatment_count());
  EXPECT_TRUE(_db.update_treatment(&treatment_3));
  EXPECT_EQ(3, _db.treatment_count());
}
TEST_F(SQLiteDriver, Treatment_TEST(Select))
{
  using namespace pfc;
  Treatment treatment_1;
  Treatment treatment_2;
  Treatment treatment_3;

  treatment_1.medical_name = "Soupus De Gallinus";
  treatment_1.common_name = "Chicken Soup";
  treatment_1.description = "Good for the soul";
  treatment_1.equipment = { 2 };
  treatment_1.citations = { 1 };

  treatment_2.medical_name = "Massageus";
  treatment_2.common_name = "Massage";
  treatment_2.description = "Back rub";
  treatment_2.equipment = { 3 };
  treatment_2.citations = { 2 };

  treatment_3.medical_name = "Bashus Headus againstus Wallus";
  treatment_3.common_name = "Bashing Head Against A Wall";
  treatment_3.description = "Oddly Satisfying";
  treatment_3.equipment = { 4 };
  treatment_3.citations = { 3 };

  EXPECT_EQ(0, _db.treatment_count());
  EXPECT_TRUE(_db.update_treatment(&treatment_1));
  EXPECT_EQ(1, _db.treatment_count());
  EXPECT_TRUE(_db.update_treatment(&treatment_2));
  EXPECT_EQ(2, _db.treatment_count());
  EXPECT_TRUE(_db.update_treatment(&treatment_3));
  EXPECT_EQ(3, _db.treatment_count());

  Treatment id;
  Treatment medical_name;
  Treatment common_name;

  id.id = 1;
  medical_name.medical_name = "Massageus";
  common_name.common_name = "Bashing Head Against A Wall";

  _db.select_treatment(&id);
  _db.select_treatment(&medical_name);
  _db.select_treatment(&common_name);

  treatment_1.id = 1;
  EXPECT_TRUE(treatment_1 == id);
  treatment_2.id = 2;
  EXPECT_TRUE(treatment_2 == medical_name);
  treatment_3.id = 3;
  EXPECT_TRUE(treatment_3 == common_name);
}
TEST_F(SQLiteDriver, Treatment_TEST(Remove))
{
  using namespace pfc;
  Treatment treatment_1;
  Treatment treatment_2;
  Treatment treatment_3;

  treatment_1.medical_name = "Soupus De Gallinus";
  treatment_1.common_name = "Chicken Soup";
  treatment_1.description = "Good for the soul";
  treatment_1.equipment = { 2 };
  treatment_1.citations = { 1 };

  treatment_2.medical_name = "Massageus";
  treatment_2.common_name = "Massage";
  treatment_2.description = "Back rub";
  treatment_2.equipment = { 3 };
  treatment_2.citations = { 2 };

  treatment_3.medical_name = "Bashus Headus againstus Wallus";
  treatment_3.common_name = "Bashing Head Against A Wall";
  treatment_3.description = "Oddly Satisfying";
  treatment_3.equipment = { 4 };
  treatment_3.citations = { 3 };

  EXPECT_TRUE(_db.update_treatment(&treatment_1));
  EXPECT_TRUE(_db.update_treatment(&treatment_2));
  EXPECT_TRUE(_db.update_treatment(&treatment_3));
  EXPECT_EQ(3, _db.treatment_count());
  EXPECT_TRUE(_db.remove_treatment(&treatment_1));
  EXPECT_TRUE(_db.remove_treatment(&treatment_3));
  EXPECT_EQ(1, _db.treatment_count());
}
TEST_F(SQLiteDriver, Treatment_TEST(Equality))
{
  using namespace pfc;
  Treatment treatment_1;
  Treatment treatment_2;

  treatment_1.medical_name = "Soupus De Gallinus";
  treatment_1.common_name = "Chicken Soup";
  treatment_1.description = "Good for the soul";
  treatment_1.equipment = { 2 };
  treatment_1.citations = { 1 };

  treatment_2.medical_name = "Soupus De Gallinus";

  EXPECT_TRUE(_db.update_treatment(&treatment_1));
  EXPECT_TRUE(_db.select_treatment(&treatment_2));
  EXPECT_TRUE(treatment_1 == treatment_2);
}
//SCENE_MAP_ROLE TESTS--------------------------------------------------------
#ifndef DISABLE_RoleMap_TEST
#define RoleMap_TEST(X) RoleMap_Test_##X
#else
#define RoleMap_TEST(X) DISABLED_RoleMap_Test_##X
#endif

TEST_F(SQLiteDriver, RoleMap_TEST(Insert))
{
  using namespace pfc;
  RoleMap map_1;
  RoleMap map_2;
  RoleMap map_3;

  map_1.fk_scene = 1;
  map_1.fk_role = 2;

  map_2.fk_scene = 3;
  map_2.fk_role = 4;

  map_3.fk_scene = 5;
  map_3.fk_role = 6;

  EXPECT_EQ(0, _db.role_map_count());
  EXPECT_TRUE(_db.update_role_map(&map_1));
  EXPECT_EQ(1, _db.role_map_count());
  EXPECT_TRUE(_db.update_role_map(&map_2));
  EXPECT_EQ(2, _db.role_map_count());
  EXPECT_TRUE(_db.update_role_map(&map_3));
  EXPECT_EQ(3, _db.role_map_count());
}
TEST_F(SQLiteDriver, RoleMap_TEST(Select))
{
  using namespace pfc;
  RoleMap map_1;
  RoleMap map_2;

  map_1.fk_scene = 1;
  map_1.fk_role = 2;

  map_2.fk_scene = 3;
  map_2.fk_role = 4;

  EXPECT_EQ(0, _db.role_map_count());
  EXPECT_TRUE(_db.update_role_map(&map_1));
  EXPECT_EQ(1, _db.role_map_count());
  EXPECT_TRUE(_db.update_role_map(&map_2));
  EXPECT_EQ(2, _db.role_map_count());

  RoleMap id;
  RoleMap fk;

  id.id = 1;
  fk.fk_scene = 3;
  fk.fk_role = 4;

  _db.select_role_map(&id);
  _db.select_role_map(&fk);

  map_1.id = 1;
  EXPECT_TRUE(map_1 == id);
  map_2.id = 2;
  EXPECT_TRUE(map_2 == fk);
}
TEST_F(SQLiteDriver, RoleMap_TEST(Remove))
{
  using namespace pfc;
  RoleMap map_1;
  RoleMap map_2;
  RoleMap map_3;

  map_1.fk_scene = 1;
  map_1.fk_role = 2;

  map_2.fk_scene = 3;
  map_2.fk_role = 4;

  map_3.fk_scene = 5;
  map_3.fk_role = 6;

  EXPECT_TRUE(_db.update_role_map(&map_1));
  EXPECT_TRUE(_db.update_role_map(&map_2));
  EXPECT_TRUE(_db.update_role_map(&map_3));
  EXPECT_EQ(3, _db.role_map_count());
  EXPECT_TRUE(_db.remove_role_map(&map_1));
  EXPECT_TRUE(_db.remove_role_map(&map_3));
  EXPECT_EQ(1, _db.role_map_count());
}
TEST_F(SQLiteDriver, RoleMap_TEST(Equality))
{
  using namespace pfc;
  RoleMap map_1;
  RoleMap map_2;

  map_1.fk_scene = 1;
  map_1.fk_role = 2;

  map_2.id = 1;

  EXPECT_TRUE(_db.update_role_map(&map_1));
  EXPECT_TRUE(_db.select_role_map(&map_2));
  EXPECT_TRUE(map_1 == map_2);
}
TEST_F(SQLiteDriver, RoleMap_TEST(Role_Insertion))
{
  using namespace pfc;
  Scene scene_1;
  Scene scene_2;
  Scene scene_3;
  RoleMap map_1;
  RoleMap map_2;
  RoleMap map_3;
  RoleMap map_4;
  RoleMap map_5;
  RoleMap map_6;
  RoleMap map_7;
  RoleMap map_8;
  RoleMap map_9;
  Role role_1;
  Role role_2;
  Role role_3;
  scene_1.name = "Opening";
  scene_2.name = "Middle";
  scene_3.name = "Ending";

  _db.update_scene(&scene_1);
  _db.update_scene(&scene_2);
  _db.update_scene(&scene_3);

  _db.select_scene(&scene_1);
  _db.select_scene(&scene_2);
  _db.select_scene(&scene_3);

  role_1.name = "Software Developer";
  role_1.description = "Bash your head against a wall for 8 hours a day";
  role_2.name = "Exterminator";
  role_2.description = "Look for bugs 8 hours a day";
  role_3.name = "Tailor";
  role_3.description = "Make quick patches 8 hours a day";

  _db.update_role(&role_1);
  _db.update_role(&role_2);
  _db.update_role(&role_3);

  _db.select_role(&role_1);
  _db.select_role(&role_2);
  _db.select_role(&role_3);

  EXPECT_EQ(0, _db.role_map_count());
  _db.update_role_in_scene(&scene_1, &role_1);
  EXPECT_EQ(1, _db.role_map_count());
  _db.update_role_in_scene(&scene_1, &role_2);
  EXPECT_EQ(2, _db.role_map_count());
  _db.update_role_in_scene(&scene_1, &role_3);
  EXPECT_EQ(3, _db.role_map_count());
  _db.update_role_in_scene(&scene_2, &role_1);
  EXPECT_EQ(4, _db.role_map_count());
  _db.update_role_in_scene(&scene_2, &role_2);
  EXPECT_EQ(5, _db.role_map_count());
  _db.update_role_in_scene(&scene_2, &role_3);
  EXPECT_EQ(6, _db.role_map_count());
  _db.update_role_in_scene(&scene_3, &role_1);
  EXPECT_EQ(7, _db.role_map_count());
  _db.update_role_in_scene(&scene_3, &role_2);
  EXPECT_EQ(8, _db.role_map_count());
  _db.update_role_in_scene(&scene_3, &role_3);
  EXPECT_EQ(9, _db.role_map_count());
}
TEST_F(SQLiteDriver, RoleMap_TEST(Role_Removal))
{
  using namespace pfc;
  Scene scene_1;
  Scene scene_2;
  Scene scene_3;
  RoleMap map_1;
  RoleMap map_2;
  RoleMap map_3;
  RoleMap map_4;
  RoleMap map_5;
  RoleMap map_6;
  RoleMap map_7;
  RoleMap map_8;
  RoleMap map_9;
  Role role_1;
  Role role_2;
  Role role_3;
  scene_1.name = "Opening";
  scene_2.name = "Middle";
  scene_3.name = "Ending";

  _db.update_scene(&scene_1);
  _db.update_scene(&scene_2);
  _db.update_scene(&scene_3);

  _db.select_scene(&scene_1);
  _db.select_scene(&scene_2);
  _db.select_scene(&scene_3);

  role_1.name = "Software Developer";
  role_1.description = "Bash your head against a wall for 8 hours a day";
  role_2.name = "Exterminator";
  role_2.description = "Look for bugs 8 hours a day";
  role_3.name = "Tailor";
  role_3.description = "Make quick patches 8 hours a day";

  _db.update_role(&role_1);
  _db.update_role(&role_2);
  _db.update_role(&role_3);

  _db.select_role(&role_1);
  _db.select_role(&role_2);
  _db.select_role(&role_3);

  _db.update_role_in_scene(&scene_1, &role_1);
  _db.update_role_in_scene(&scene_1, &role_2);
  _db.update_role_in_scene(&scene_1, &role_3);
  _db.update_role_in_scene(&scene_2, &role_1);
  _db.update_role_in_scene(&scene_2, &role_2);
  _db.update_role_in_scene(&scene_2, &role_3);
  _db.update_role_in_scene(&scene_3, &role_1);
  _db.update_role_in_scene(&scene_3, &role_2);
  _db.update_role_in_scene(&scene_3, &role_3);

  EXPECT_EQ(9, _db.role_map_count());
  _db.remove_role_from_scene(&role_1, &scene_1);
  EXPECT_EQ(8, _db.role_map_count());
  _db.remove_role_from_scene(&role_1, &scene_2);
  EXPECT_EQ(7, _db.role_map_count());
  _db.remove_role_from_scene(&role_1, &scene_3);
  EXPECT_EQ(6, _db.role_map_count());
  _db.remove_role_from_scene(&role_2, &scene_1);
  EXPECT_EQ(5, _db.role_map_count());
  _db.remove_role_from_scene(&role_2, &scene_2);
  EXPECT_EQ(4, _db.role_map_count());
  _db.remove_role_from_scene(&role_2, &scene_3);
  EXPECT_EQ(3, _db.role_map_count());
  _db.remove_role_from_scene(&role_3, &scene_1);
  EXPECT_EQ(2, _db.role_map_count());
  _db.remove_role_from_scene(&role_3, &scene_2);
  EXPECT_EQ(1, _db.role_map_count());
  _db.remove_role_from_scene(&role_3, &scene_3);
  EXPECT_EQ(0, _db.role_map_count());
  EXPECT_EQ(3, _db.role_count());
}
TEST_F(SQLiteDriver, RoleMap_TEST(Role_Deletion))
{
  using namespace pfc;
  Scene scene_1;
  Scene scene_2;
  Scene scene_3;
  RoleMap map_1;
  RoleMap map_2;
  RoleMap map_3;
  RoleMap map_4;
  RoleMap map_5;
  RoleMap map_6;
  RoleMap map_7;
  RoleMap map_8;
  RoleMap map_9;
  Role role_1;
  Role role_2;
  Role role_3;
  scene_1.name = "Opening";
  scene_2.name = "Middle";
  scene_3.name = "Ending";

  _db.update_scene(&scene_1);
  _db.update_scene(&scene_2);
  _db.update_scene(&scene_3);

  _db.select_scene(&scene_1);
  _db.select_scene(&scene_2);
  _db.select_scene(&scene_3);

  role_1.name = "Software Developer";
  role_1.description = "Bash your head against a wall for 8 hours a day";
  role_2.name = "Exterminator";
  role_2.description = "Look for bugs 8 hours a day";
  role_3.name = "Tailor";
  role_3.description = "Make quick patches 8 hours a day";

  _db.update_role(&role_1);
  _db.update_role(&role_2);
  _db.update_role(&role_3);

  _db.select_role(&role_1);
  _db.select_role(&role_2);
  _db.select_role(&role_3);

  _db.update_role_in_scene(&scene_1, &role_1);
  _db.update_role_in_scene(&scene_1, &role_2);
  _db.update_role_in_scene(&scene_1, &role_3);
  _db.update_role_in_scene(&scene_2, &role_1);
  _db.update_role_in_scene(&scene_2, &role_2);
  _db.update_role_in_scene(&scene_2, &role_3);
  _db.update_role_in_scene(&scene_3, &role_1);
  _db.update_role_in_scene(&scene_3, &role_2);
  _db.update_role_in_scene(&scene_3, &role_3);

  EXPECT_EQ(9, _db.role_map_count());
  EXPECT_EQ(3, _db.role_count(&scene_1));
  EXPECT_EQ(3, _db.role_count(&scene_2));
  EXPECT_EQ(3, _db.role_count(&scene_3));
  _db.remove_role(&role_1);
  EXPECT_EQ(6, _db.role_map_count());
  EXPECT_EQ(2, _db.role_count(&scene_1));
  EXPECT_EQ(2, _db.role_count(&scene_2));
  EXPECT_EQ(2, _db.role_count(&scene_3));
  _db.remove_role(&role_2);
  EXPECT_EQ(3, _db.role_map_count());
  EXPECT_EQ(1, _db.role_count(&scene_1));
  EXPECT_EQ(1, _db.role_count(&scene_2));
  EXPECT_EQ(1, _db.role_count(&scene_3));
  _db.remove_role(&role_3);
  EXPECT_EQ(0, _db.role_map_count());
  EXPECT_EQ(0, _db.role_count(&scene_1));
  EXPECT_EQ(0, _db.role_count(&scene_2));
  EXPECT_EQ(0, _db.role_count(&scene_3));
}
TEST_F(SQLiteDriver, RoleMap_TEST(Scene_Deletion))
{
  using namespace pfc;
  Scene scene_1;
  Scene scene_2;
  Scene scene_3;
  RoleMap map_1;
  RoleMap map_2;
  RoleMap map_3;
  RoleMap map_4;
  RoleMap map_5;
  RoleMap map_6;
  RoleMap map_7;
  RoleMap map_8;
  RoleMap map_9;
  Role role_1;
  Role role_2;
  Role role_3;
  scene_1.name = "Opening";
  scene_2.name = "Middle";
  scene_3.name = "Ending";

  _db.update_scene(&scene_1);
  _db.update_scene(&scene_2);
  _db.update_scene(&scene_3);

  _db.select_scene(&scene_1);
  _db.select_scene(&scene_2);
  _db.select_scene(&scene_3);

  role_1.name = "Software Developer";
  role_1.description = "Bash your head against a wall for 8 hours a day";
  role_2.name = "Exterminator";
  role_2.description = "Look for bugs 8 hours a day";
  role_3.name = "Tailor";
  role_3.description = "Make quick patches 8 hours a day";

  _db.update_role(&role_1);
  _db.update_role(&role_2);
  _db.update_role(&role_3);

  _db.select_role(&role_1);
  _db.select_role(&role_2);
  _db.select_role(&role_3);

  _db.update_role_in_scene(&scene_1, &role_1);
  _db.update_role_in_scene(&scene_1, &role_2);
  _db.update_role_in_scene(&scene_1, &role_3);
  _db.update_role_in_scene(&scene_2, &role_1);
  _db.update_role_in_scene(&scene_2, &role_2);
  _db.update_role_in_scene(&scene_2, &role_3);
  _db.update_role_in_scene(&scene_3, &role_1);
  _db.update_role_in_scene(&scene_3, &role_2);
  _db.update_role_in_scene(&scene_3, &role_3);

  EXPECT_EQ(9, _db.role_map_count());
  _db.remove_scene(&scene_1);
  EXPECT_EQ(6, _db.role_map_count());
  _db.remove_scene(&scene_2);
  EXPECT_EQ(3, _db.role_map_count());
  _db.remove_scene(&scene_3);
  EXPECT_EQ(0, _db.role_map_count());
}
//SCENE_MAP_EVENT TESTS------------------------------------------------------
#ifndef DISABLE_EventMap_TEST
#define EventMap_TEST(X) EventMap_Test_##X
#else
#define EventMap_TEST(X) DISABLED_EventMap_Test_##X
#endif
TEST_F(SQLiteDriver, EventMap_TEST(Insert))
{
  using namespace pfc;
  EventMap event_map_1;
  EventMap event_map_2;
  EventMap event_map_3;

  event_map_1.fk_scene = 1;
  event_map_1.fk_event = 2;

  event_map_2.fk_scene = 3;
  event_map_2.fk_event = 4;

  event_map_3.fk_scene = 5;
  event_map_3.fk_event = 6;

  EXPECT_EQ(0, _db.event_map_count());
  EXPECT_TRUE(_db.update_event_map(&event_map_1));
  EXPECT_EQ(1, _db.event_map_count());
  EXPECT_TRUE(_db.update_event_map(&event_map_2));
  EXPECT_EQ(2, _db.event_map_count());
  EXPECT_TRUE(_db.update_event_map(&event_map_3));
  EXPECT_EQ(3, _db.event_map_count());
}
TEST_F(SQLiteDriver, EventMap_TEST(Select))
{
  using namespace pfc;
  EventMap event_map_1;
  EventMap event_map_2;

  event_map_1.fk_scene = 1;
  event_map_1.fk_event = 2;

  event_map_2.fk_scene = 3;
  event_map_2.fk_event = 4;

  EXPECT_EQ(0, _db.event_map_count());
  EXPECT_TRUE(_db.update_event_map(&event_map_1));
  EXPECT_EQ(1, _db.event_map_count());
  EXPECT_TRUE(_db.update_event_map(&event_map_2));
  EXPECT_EQ(2, _db.event_map_count());

  EventMap id;
  EventMap fk;

  id.id = 1;
  fk.fk_scene = 3;
  fk.fk_event = 4;

  _db.select_event_map(&id);
  _db.select_event_map(&fk);

  event_map_1.id = 1;
  EXPECT_TRUE(event_map_1 == id);
  event_map_2.id = 2;
  EXPECT_TRUE(event_map_2 == fk);
}
TEST_F(SQLiteDriver, EventMap_TEST(Remove))
{
  using namespace pfc;
  EventMap event_map_1;
  EventMap event_map_2;
  EventMap event_map_3;

  event_map_1.fk_scene = 1;
  event_map_1.fk_event = 2;

  event_map_2.fk_scene = 3;
  event_map_2.fk_event = 4;

  event_map_3.fk_scene = 5;
  event_map_3.fk_event = 6;

  EXPECT_TRUE(_db.update_event_map(&event_map_1));
  EXPECT_TRUE(_db.update_event_map(&event_map_2));
  EXPECT_TRUE(_db.update_event_map(&event_map_3));
  EXPECT_EQ(3, _db.event_map_count());
  EXPECT_TRUE(_db.remove_event_map(&event_map_1));
  EXPECT_TRUE(_db.remove_event_map(&event_map_3));
  EXPECT_EQ(1, _db.event_map_count());
}
TEST_F(SQLiteDriver, EventMap_TEST(Equality))
{
  using namespace pfc;
  EventMap event_map_1;
  EventMap event_map_2;

  event_map_1.fk_scene = 1;
  event_map_1.fk_event = 2;

  event_map_2.id = 1;

  EXPECT_TRUE(_db.update_event_map(&event_map_1));
  EXPECT_TRUE(_db.select_event_map(&event_map_2));
  EXPECT_TRUE(event_map_1 == event_map_2);
}
TEST_F(SQLiteDriver, EventMap_TEST(Event_Insertion))
{
  using namespace pfc;
  Scene scene_1;
  Scene scene_2;
  Scene scene_3;
  EventMap map_1;
  EventMap map_2;
  EventMap map_3;
  EventMap map_4;
  EventMap map_5;
  EventMap map_6;
  EventMap map_7;
  EventMap map_8;
  EventMap map_9;
  Event event_1;
  Event event_2;
  Event event_3;
  scene_1.name = "Opening";
  scene_2.name = "Middle";
  scene_3.name = "Ending";

  _db.update_scene(&scene_1);
  _db.update_scene(&scene_2);
  _db.update_scene(&scene_3);

  _db.select_scene(&scene_1);
  _db.select_scene(&scene_2);
  _db.select_scene(&scene_3);

  event_1.name = "Software Developer";
  event_1.description = "Bash your head against a wall for 8 hours a day";
  event_2.name = "Exterminator";
  event_2.description = "Look for bugs 8 hours a day";
  event_3.name = "Tailor";
  event_3.description = "Make quick patches 8 hours a day";

  _db.update_event(&event_1);
  _db.update_event(&event_2);
  _db.update_event(&event_3);

  _db.select_event(&event_1);
  _db.select_event(&event_2);
  _db.select_event(&event_3);

  EXPECT_EQ(0, _db.event_map_count());
  _db.update_event_in_scene(&scene_1, &event_1);
  EXPECT_EQ(1, _db.event_map_count());
  _db.update_event_in_scene(&scene_1, &event_2);
  EXPECT_EQ(2, _db.event_map_count());
  _db.update_event_in_scene(&scene_1, &event_3);
  EXPECT_EQ(3, _db.event_map_count());
  _db.update_event_in_scene(&scene_2, &event_1);
  EXPECT_EQ(4, _db.event_map_count());
  _db.update_event_in_scene(&scene_2, &event_2);
  EXPECT_EQ(5, _db.event_map_count());
  _db.update_event_in_scene(&scene_2, &event_3);
  EXPECT_EQ(6, _db.event_map_count());
  _db.update_event_in_scene(&scene_3, &event_1);
  EXPECT_EQ(7, _db.event_map_count());
  _db.update_event_in_scene(&scene_3, &event_2);
  EXPECT_EQ(8, _db.event_map_count());
  _db.update_event_in_scene(&scene_3, &event_3);
  EXPECT_EQ(9, _db.event_map_count());
}
TEST_F(SQLiteDriver, EventMap_TEST(Event_Removal))
{
  using namespace pfc;
  Scene scene_1;
  Scene scene_2;
  Scene scene_3;
  EventMap map_1;
  EventMap map_2;
  EventMap map_3;
  EventMap map_4;
  EventMap map_5;
  EventMap map_6;
  EventMap map_7;
  EventMap map_8;
  EventMap map_9;
  Event event_1;
  Event event_2;
  Event event_3;
  scene_1.name = "Opening";
  scene_2.name = "Middle";
  scene_3.name = "Ending";

  _db.update_scene(&scene_1);
  _db.update_scene(&scene_2);
  _db.update_scene(&scene_3);

  _db.select_scene(&scene_1);
  _db.select_scene(&scene_2);
  _db.select_scene(&scene_3);

  event_1.name = "Software Developer";
  event_1.description = "Bash your head against a wall for 8 hours a day";
  event_2.name = "Exterminator";
  event_2.description = "Look for bugs 8 hours a day";
  event_3.name = "Tailor";
  event_3.description = "Make quick patches 8 hours a day";

  _db.update_event(&event_1);
  _db.update_event(&event_2);
  _db.update_event(&event_3);

  _db.select_event(&event_1);
  _db.select_event(&event_2);
  _db.select_event(&event_3);

  _db.update_event_in_scene(&scene_1, &event_1);
  _db.update_event_in_scene(&scene_1, &event_2);
  _db.update_event_in_scene(&scene_1, &event_3);
  _db.update_event_in_scene(&scene_2, &event_1);
  _db.update_event_in_scene(&scene_2, &event_2);
  _db.update_event_in_scene(&scene_2, &event_3);
  _db.update_event_in_scene(&scene_3, &event_1);
  _db.update_event_in_scene(&scene_3, &event_2);
  _db.update_event_in_scene(&scene_3, &event_3);

  EXPECT_EQ(9, _db.event_map_count());
  _db.remove_event_from_scene(&event_1, &scene_1);
  EXPECT_EQ(8, _db.event_map_count());
  _db.remove_event_from_scene(&event_1, &scene_2);
  EXPECT_EQ(7, _db.event_map_count());
  _db.remove_event_from_scene(&event_1, &scene_3);
  EXPECT_EQ(6, _db.event_map_count());
  _db.remove_event_from_scene(&event_2, &scene_1);
  EXPECT_EQ(5, _db.event_map_count());
  _db.remove_event_from_scene(&event_2, &scene_2);
  EXPECT_EQ(4, _db.event_map_count());
  _db.remove_event_from_scene(&event_2, &scene_3);
  EXPECT_EQ(3, _db.event_map_count());
  _db.remove_event_from_scene(&event_3, &scene_1);
  EXPECT_EQ(2, _db.event_map_count());
  _db.remove_event_from_scene(&event_3, &scene_2);
  EXPECT_EQ(1, _db.event_map_count());
  _db.remove_event_from_scene(&event_3, &scene_3);
  EXPECT_EQ(0, _db.event_map_count());
  EXPECT_EQ(3, _db.event_count());
}
TEST_F(SQLiteDriver, EventMap_TEST(Event_Deletion))
{
  using namespace pfc;
  Scene scene_1;
  Scene scene_2;
  Scene scene_3;
  EventMap map_1;
  EventMap map_2;
  EventMap map_3;
  EventMap map_4;
  EventMap map_5;
  EventMap map_6;
  EventMap map_7;
  EventMap map_8;
  EventMap map_9;
  Event event_1;
  Event event_2;
  Event event_3;
  scene_1.name = "Opening";
  scene_2.name = "Middle";
  scene_3.name = "Ending";

  _db.update_scene(&scene_1);
  _db.update_scene(&scene_2);
  _db.update_scene(&scene_3);

  _db.select_scene(&scene_1);
  _db.select_scene(&scene_2);
  _db.select_scene(&scene_3);

  event_1.name = "Software Developer";
  event_1.description = "Bash your head against a wall for 8 hours a day";
  event_2.name = "Exterminator";
  event_2.description = "Look for bugs 8 hours a day";
  event_3.name = "Tailor";
  event_3.description = "Make quick patches 8 hours a day";

  _db.update_event(&event_1);
  _db.update_event(&event_2);
  _db.update_event(&event_3);

  _db.select_event(&event_1);
  _db.select_event(&event_2);
  _db.select_event(&event_3);

  _db.update_event_in_scene(&scene_1, &event_1);
  _db.update_event_in_scene(&scene_1, &event_2);
  _db.update_event_in_scene(&scene_1, &event_3);
  _db.update_event_in_scene(&scene_2, &event_1);
  _db.update_event_in_scene(&scene_2, &event_2);
  _db.update_event_in_scene(&scene_2, &event_3);
  _db.update_event_in_scene(&scene_3, &event_1);
  _db.update_event_in_scene(&scene_3, &event_2);
  _db.update_event_in_scene(&scene_3, &event_3);

  EXPECT_EQ(9, _db.event_map_count());
  EXPECT_EQ(3, _db.event_count(&scene_1));
  EXPECT_EQ(3, _db.event_count(&scene_2));
  EXPECT_EQ(3, _db.event_count(&scene_3));
  _db.remove_event(&event_1);
  EXPECT_EQ(6, _db.event_map_count());
  EXPECT_EQ(2, _db.event_count(&scene_1));
  EXPECT_EQ(2, _db.event_count(&scene_2));
  EXPECT_EQ(2, _db.event_count(&scene_3));
  _db.remove_event(&event_2);
  EXPECT_EQ(3, _db.event_map_count());
  EXPECT_EQ(1, _db.event_count(&scene_1));
  EXPECT_EQ(1, _db.event_count(&scene_2));
  EXPECT_EQ(1, _db.event_count(&scene_3));
  _db.remove_event(&event_3);
  EXPECT_EQ(0, _db.event_map_count());
  EXPECT_EQ(0, _db.event_count(&scene_1));
  EXPECT_EQ(0, _db.event_count(&scene_2));
  EXPECT_EQ(0, _db.event_count(&scene_3));
}
TEST_F(SQLiteDriver, EventMap_TEST(Scene_Deletion))
{
  using namespace pfc;
  Scene scene_1;
  Scene scene_2;
  Scene scene_3;
  EventMap map_1;
  EventMap map_2;
  EventMap map_3;
  EventMap map_4;
  EventMap map_5;
  EventMap map_6;
  EventMap map_7;
  EventMap map_8;
  EventMap map_9;
  Event event_1;
  Event event_2;
  Event event_3;
  scene_1.name = "Opening";
  scene_2.name = "Middle";
  scene_3.name = "Ending";

  _db.update_scene(&scene_1);
  _db.update_scene(&scene_2);
  _db.update_scene(&scene_3);

  _db.select_scene(&scene_1);
  _db.select_scene(&scene_2);
  _db.select_scene(&scene_3);

  event_1.name = "Software Developer";
  event_1.description = "Bash your head against a wall for 8 hours a day";
  event_2.name = "Exterminator";
  event_2.description = "Look for bugs 8 hours a day";
  event_3.name = "Tailor";
  event_3.description = "Make quick patches 8 hours a day";

  _db.update_event(&event_1);
  _db.update_event(&event_2);
  _db.update_event(&event_3);

  _db.select_event(&event_1);
  _db.select_event(&event_2);
  _db.select_event(&event_3);

  _db.update_event_in_scene(&scene_1, &event_1);
  _db.update_event_in_scene(&scene_1, &event_2);
  _db.update_event_in_scene(&scene_1, &event_3);
  _db.update_event_in_scene(&scene_2, &event_1);
  _db.update_event_in_scene(&scene_2, &event_2);
  _db.update_event_in_scene(&scene_2, &event_3);
  _db.update_event_in_scene(&scene_3, &event_1);
  _db.update_event_in_scene(&scene_3, &event_2);
  _db.update_event_in_scene(&scene_3, &event_3);

  EXPECT_EQ(9, _db.event_map_count());
  _db.remove_scene(&scene_1);
  EXPECT_EQ(6, _db.event_map_count());
  _db.remove_scene(&scene_2);
  EXPECT_EQ(3, _db.event_map_count());
  _db.remove_scene(&scene_3);
  EXPECT_EQ(0, _db.event_map_count());
}
//SCENE_MAP_EQUIPMENT TESTS-----------------------------------------------
#ifndef DISABLE_EquipmentMap_TEST
#define EquipmentMap_TEST(X) EquipmentMap_Test_##X
#else
#define EquipmentMap_TEST(X) DISABLED_EquipmentMap_Test_##X
#endif
TEST_F(SQLiteDriver, EquipmentMap_TEST(EquipmentMap_Insertion))
{
  using namespace pfc;
  Scene scene_1;
  Scene scene_2;
  Scene scene_3;

  Equipment Equipment_1;
  Equipment Equipment_2;
  Equipment Equipment_3;
  scene_1.name = "Opening";
  scene_2.name = "Middle";
  scene_3.name = "Ending";

  _db.update_scene(&scene_1);
  _db.update_scene(&scene_2);
  _db.update_scene(&scene_3);

  _db.select_scene(&scene_1);
  _db.select_scene(&scene_2);
  _db.select_scene(&scene_3);

  Equipment_1.name = "Keytar";
  Equipment_1.type = 1;
  Equipment_1.description = "Got a sick keytar solo later";
  Equipment_1.image = ("music stand");
  Equipment_1.citations = { 1 };

  Equipment_2.name = "piano";
  Equipment_2.type = 2;
  Equipment_2.description = "big instrument with keys";
  Equipment_2.image = ("piano bench");
  Equipment_2.citations = { 2 };

  Equipment_3.name = "bagpipes";
  Equipment_3.type = 3;
  Equipment_3.description = "please stop playing the bagpipes";
  Equipment_3.image = ("a bladder");
  Equipment_3.citations = { 3 };

  _db.update_equipment(&Equipment_1);
  _db.update_equipment(&Equipment_2);
  _db.update_equipment(&Equipment_3);

  _db.select_equipment(&Equipment_1);
  _db.select_equipment(&Equipment_2);
  _db.select_equipment(&Equipment_3);

  EquipmentMap map;

  EXPECT_EQ(0, _db.equipment_map_count());

  map.clear();
  map.scene->assign(scene_1);
  map.equipment->assign(Equipment_1);
  _db.update_equipment_in_scene(&map);
  EXPECT_EQ(1, _db.equipment_map_count());

  map.clear();
  map.scene->assign(scene_1);
  map.equipment->assign(Equipment_2);
  _db.update_equipment_in_scene(&map);
  EXPECT_EQ(2, _db.equipment_map_count());

  map.clear();
  map.scene->assign(scene_1);
  map.equipment->assign(Equipment_3);
  _db.update_equipment_in_scene(&map);
  EXPECT_EQ(3, _db.equipment_map_count());

  map.clear();
  map.scene->assign(scene_2);
  map.equipment->assign(Equipment_1);
  _db.update_equipment_in_scene(&map);
  EXPECT_EQ(4, _db.equipment_map_count());

  map.clear();
  map.scene->assign(scene_2);
  map.equipment->assign(Equipment_2);
  _db.update_equipment_in_scene(&map);
  EXPECT_EQ(5, _db.equipment_map_count());

  map.clear();
  map.scene->assign(scene_2);
  map.equipment->assign(Equipment_3);
  _db.update_equipment_in_scene(&map);
  EXPECT_EQ(6, _db.equipment_map_count());

  map.clear();
  map.scene->assign(scene_3);
  map.equipment->assign(Equipment_1);
  _db.update_equipment_in_scene(&map);
  EXPECT_EQ(7, _db.equipment_map_count());

  map.clear();
  map.scene->assign(scene_3);
  map.equipment->assign(Equipment_2);
  _db.update_equipment_in_scene(&map);
  EXPECT_EQ(8, _db.equipment_map_count());

  map.clear();
  map.scene->assign(scene_3);
  map.equipment->assign(Equipment_3);
  _db.update_equipment_in_scene(&map);
  EXPECT_EQ(9, _db.equipment_map_count());
}
TEST_F(SQLiteDriver, EquipmentMap_TEST(EquipmentMap_Removal))
{
  using namespace pfc;
  Scene scene_1;
  Scene scene_2;
  Scene scene_3;

  Equipment equipment_1;
  Equipment equipment_2;
  Equipment equipment_3;
  scene_1.name = "Opening";
  scene_2.name = "Middle";
  scene_3.name = "Ending";

  _db.update_scene(&scene_1);
  _db.update_scene(&scene_2);
  _db.update_scene(&scene_3);

  _db.select_scene(&scene_1);
  _db.select_scene(&scene_2);
  _db.select_scene(&scene_3);

  equipment_1.name = "Keytar";
  equipment_1.type = 1;
  equipment_1.description = "Got a sick keytar solo later";
  equipment_1.image = ("music stand");
  equipment_1.citations = { 1 };

  equipment_2.name = "piano";
  equipment_2.type = 2;
  equipment_2.description = "big instrument with keys";
  equipment_2.image = ("piano bench");
  equipment_2.citations = { 2 };

  equipment_3.name = "bagpipes";
  equipment_3.type = 3;
  equipment_3.description = "please stop playing the bagpipes";
  equipment_3.image = ("a bladder");
  equipment_3.citations = { 3 };

  _db.update_equipment(&equipment_1);
  _db.update_equipment(&equipment_2);
  _db.update_equipment(&equipment_3);

  _db.select_equipment(&equipment_1);
  _db.select_equipment(&equipment_2);
  _db.select_equipment(&equipment_3);

  EquipmentMap map_1;
  map_1.clear();
  map_1.scene->assign(scene_1);
  map_1.equipment->assign(equipment_1);

  EquipmentMap map_2;
  map_2.clear();
  map_2.scene->assign(scene_1);
  map_2.equipment->assign(equipment_2);

  EquipmentMap map_3;
  map_3.clear();
  map_3.scene->assign(scene_1);
  map_3.equipment->assign(equipment_3);

  EquipmentMap map_4;
  map_4.clear();
  map_4.scene->assign(scene_2);
  map_4.equipment->assign(equipment_1);

  EquipmentMap map_5;
  map_5.clear();
  map_5.scene->assign(scene_2);
  map_5.equipment->assign(equipment_2);

  EquipmentMap map_6;
  map_6.clear();
  map_6.scene->assign(scene_2);
  map_6.equipment->assign(equipment_3);

  EquipmentMap map_7;
  map_7.clear();
  map_7.scene->assign(scene_3);
  map_7.equipment->assign(equipment_1);

  EquipmentMap map_8;
  map_8.clear();
  map_8.scene->assign(scene_3);
  map_8.equipment->assign(equipment_2);

  EquipmentMap map_9;
  map_9.clear();
  map_9.scene->assign(scene_3);
  map_9.equipment->assign(equipment_3);

  _db.update_equipment_in_scene(&map_1);
  _db.update_equipment_in_scene(&map_2);
  _db.update_equipment_in_scene(&map_3);
  _db.update_equipment_in_scene(&map_4);
  _db.update_equipment_in_scene(&map_5);
  _db.update_equipment_in_scene(&map_6);
  _db.update_equipment_in_scene(&map_7);
  _db.update_equipment_in_scene(&map_8);
  _db.update_equipment_in_scene(&map_9);

  EXPECT_EQ(9, _db.equipment_map_count());
  _db.remove_equipment_from_scene(&equipment_1, &scene_1);
  EXPECT_EQ(8, _db.equipment_map_count());
  _db.remove_equipment_from_scene(&equipment_1, &scene_2);
  EXPECT_EQ(7, _db.equipment_map_count());
  _db.remove_equipment_from_scene(&equipment_1, &scene_3);
  EXPECT_EQ(6, _db.equipment_map_count());
  _db.remove_equipment_from_scene(&equipment_2, &scene_1);
  EXPECT_EQ(5, _db.equipment_map_count());
  _db.remove_equipment_from_scene(&equipment_2, &scene_2);
  EXPECT_EQ(4, _db.equipment_map_count());
  _db.remove_equipment_from_scene(&equipment_2, &scene_3);
  EXPECT_EQ(3, _db.equipment_map_count());
  _db.remove_equipment_from_scene(&equipment_3, &scene_1);
  EXPECT_EQ(2, _db.equipment_map_count());
  _db.remove_equipment_from_scene(&equipment_3, &scene_2);
  EXPECT_EQ(1, _db.equipment_map_count());
  _db.remove_equipment_from_scene(&equipment_3, &scene_3);
  EXPECT_EQ(0, _db.equipment_map_count());
  EXPECT_EQ(3, _db.equipment_count());
}
TEST_F(SQLiteDriver, EquipmentMap_TEST(EquipmentMap_Deletion))
{
  using namespace pfc;
  Scene scene_1;
  Scene scene_2;
  Scene scene_3;

  Equipment equipment_1;
  Equipment equipment_2;
  Equipment equipment_3;
  scene_1.name = "Opening";
  scene_2.name = "Middle";
  scene_3.name = "Ending";

  _db.update_scene(&scene_1);
  _db.update_scene(&scene_2);
  _db.update_scene(&scene_3);

  _db.select_scene(&scene_1);
  _db.select_scene(&scene_2);
  _db.select_scene(&scene_3);

  equipment_1.name = "Keytar";
  equipment_1.type = 1;
  equipment_1.description = "Got a sick keytar solo later";
  equipment_1.image = ("music stand");
  equipment_1.citations = { 1 };

  equipment_2.name = "piano";
  equipment_2.type = 2;
  equipment_2.description = "big instrument with keys";
  equipment_2.image = ("piano bench");
  equipment_2.citations = { 2 };

  equipment_3.name = "bagpipes";
  equipment_3.type = 3;
  equipment_3.description = "please stop playing the bagpipes";
  equipment_3.image = ("a bladder");
  equipment_3.citations = { 3 };

  _db.update_equipment(&equipment_1);
  _db.update_equipment(&equipment_2);
  _db.update_equipment(&equipment_3);

  _db.select_equipment(&equipment_1);
  _db.select_equipment(&equipment_2);
  _db.select_equipment(&equipment_3);

  EquipmentMap map;
  map.clear();
  map.scene->assign(scene_1);
  map.equipment->assign(equipment_1);
  _db.update_equipment_in_scene(&map);

  map.clear();
  map.scene->assign(scene_1);
  map.equipment->assign(equipment_2);
  _db.update_equipment_in_scene(&map);

  map.clear();
  map.scene->assign(scene_1);
  map.equipment->assign(equipment_3);
  _db.update_equipment_in_scene(&map);

  map.clear();
  map.scene->assign(scene_2);
  map.equipment->assign(equipment_1);
  _db.update_equipment_in_scene(&map);

  map.clear();
  map.scene->assign(scene_2);
  map.equipment->assign(equipment_2);
  _db.update_equipment_in_scene(&map);

  map.clear();
  map.scene->assign(scene_2);
  map.equipment->assign(equipment_3);
  _db.update_equipment_in_scene(&map);

  map.clear();
  map.scene->assign(scene_3);
  map.equipment->assign(equipment_1);
  _db.update_equipment_in_scene(&map);

  map.clear();
  map.scene->assign(scene_3);
  map.equipment->assign(equipment_2);
  _db.update_equipment_in_scene(&map);

  map.clear();
  map.scene->assign(scene_3);
  map.equipment->assign(equipment_3);
  _db.update_equipment_in_scene(&map);

  EXPECT_EQ(9, _db.equipment_map_count());
  EXPECT_EQ(3, _db.equipment_count(&scene_1));
  EXPECT_EQ(3, _db.equipment_count(&scene_2));
  EXPECT_EQ(3, _db.equipment_count(&scene_3));
  _db.remove_equipment(&equipment_1);
  EXPECT_EQ(6, _db.equipment_map_count());
  EXPECT_EQ(2, _db.equipment_count(&scene_1));
  EXPECT_EQ(2, _db.equipment_count(&scene_2));
  EXPECT_EQ(2, _db.equipment_count(&scene_3));
  _db.remove_equipment(&equipment_2);
  EXPECT_EQ(3, _db.equipment_map_count());
  EXPECT_EQ(1, _db.equipment_count(&scene_1));
  EXPECT_EQ(1, _db.equipment_count(&scene_2));
  EXPECT_EQ(1, _db.equipment_count(&scene_3));
  _db.remove_equipment(&equipment_3);
  EXPECT_EQ(0, _db.equipment_map_count());
  EXPECT_EQ(0, _db.equipment_count(&scene_1));
  EXPECT_EQ(0, _db.equipment_count(&scene_2));
  EXPECT_EQ(0, _db.equipment_count(&scene_3));
}
TEST_F(SQLiteDriver, EquipmentMap_TEST(Scene_Deletion))
{
  using namespace pfc;
  Scene scene_1;
  Scene scene_2;
  Scene scene_3;

  Equipment equipment_1;
  Equipment equipment_2;
  Equipment equipment_3;
  scene_1.name = "Opening";
  scene_2.name = "Middle";
  scene_3.name = "Ending";

  _db.update_scene(&scene_1);
  _db.update_scene(&scene_2);
  _db.update_scene(&scene_3);

  _db.select_scene(&scene_1);
  _db.select_scene(&scene_2);
  _db.select_scene(&scene_3);

  equipment_1.name = "Keytar";
  equipment_1.type = 1;
  equipment_1.description = "Got a sick keytar solo later";
  equipment_1.image = ("music stand");
  equipment_1.citations = { 1 };

  equipment_2.name = "piano";
  equipment_2.type = 2;
  equipment_2.description = "big instrument with keys";
  equipment_2.image = ("piano bench");
  equipment_2.citations = { 2 };

  equipment_3.name = "bagpipes";
  equipment_3.type = 3;
  equipment_3.description = "please stop playing the bagpipes";
  equipment_3.image = ("a bladder");
  equipment_3.citations = { 3 };

  _db.update_equipment(&equipment_1);
  _db.update_equipment(&equipment_2);
  _db.update_equipment(&equipment_3);

  _db.select_equipment(&equipment_1);
  _db.select_equipment(&equipment_2);
  _db.select_equipment(&equipment_3);

  EquipmentMap map;

  map.clear();
  map.scene->assign(scene_1);
  map.equipment->assign(equipment_1);
  _db.update_equipment_in_scene(&map);

  map.clear();
  map.scene->assign(scene_1);
  map.equipment->assign(equipment_2);
  _db.update_equipment_in_scene(&map);

  map.clear();
  map.scene->assign(scene_1);
  map.equipment->assign(equipment_3);
  _db.update_equipment_in_scene(&map);

  map.clear();
  map.scene->assign(scene_2);
  map.equipment->assign(equipment_1);
  _db.update_equipment_in_scene(&map);

  map.clear();
  map.scene->assign(scene_2);
  map.equipment->assign(equipment_2);
  _db.update_equipment_in_scene(&map);

  map.clear();
  map.scene->assign(scene_2);
  map.equipment->assign(equipment_3);
  _db.update_equipment_in_scene(&map);

  map.clear();
  map.scene->assign(scene_3);
  map.equipment->assign(equipment_1);
  _db.update_equipment_in_scene(&map);

  map.clear();
  map.scene->assign(scene_3);
  map.equipment->assign(equipment_2);
  _db.update_equipment_in_scene(&map);

  map.clear();
  map.scene->assign(scene_3);
  map.equipment->assign(equipment_3);
  _db.update_equipment_in_scene(&map);

  EXPECT_EQ(9, _db.equipment_map_count());
  _db.remove_scene(&scene_1);
  EXPECT_EQ(6, _db.equipment_map_count());
  _db.remove_scene(&scene_2);
  EXPECT_EQ(3, _db.equipment_map_count());
  _db.remove_scene(&scene_3);
  EXPECT_EQ(0, _db.equipment_map_count());
}
//SCENE_MAP_CITATION TESTS------------------------------------------------
#ifndef DISABLE_CitationMap_TEST
#define CitationMap_TEST(X) CitationMap_Test_##X
#else
#define CitationMap_TEST(X) DISABLED_CitationMap_##X
#endif
TEST_F(SQLiteDriver, CitationMap_TEST(Citation_Insertion))
{
  using namespace pfc;
  Scene scene_1;
  Scene scene_2;
  Scene scene_3;

  Citation citation_1;
  Citation citation_2;
  Citation citation_3;
  scene_1.name = "Opening";
  scene_2.name = "Middle";
  scene_3.name = "Ending";

  _db.update_scene(&scene_1);
  _db.update_scene(&scene_2);
  _db.update_scene(&scene_3);

  _db.select_scene(&scene_1);
  _db.select_scene(&scene_2);
  _db.select_scene(&scene_3);

  citation_1.key = "skeleton key";
  citation_1.title = "Mr. Bones' Wild Ride";
  citation_1.authors = "Nathan;Angel";
  citation_1.year = "1000";
  citation_1.publisher = "Boneland";

  citation_2.key = "boss key";
  citation_2.title = "Legend of Zelda";
  citation_2.authors = "Austin;Matthew";
  citation_2.year = "2000";
  citation_2.publisher = "Nintendo";

  citation_3.key = "rsa key";
  citation_3.title = "Extra Security";
  citation_3.authors = "Steven;Lucas";
  citation_3.year = "2019";
  citation_3.publisher = "ARA";

  _db.update_citation(&citation_1);
  _db.update_citation(&citation_2);
  _db.update_citation(&citation_3);

  _db.select_citation(&citation_1);
  _db.select_citation(&citation_2);
  _db.select_citation(&citation_3);

  EXPECT_EQ(0, _db.citation_map_count());
  _db.update_citation_in_scene(&scene_1, &citation_1);
  EXPECT_EQ(1, _db.citation_map_count());
  _db.update_citation_in_scene(&scene_1, &citation_2);
  EXPECT_EQ(2, _db.citation_map_count());
  _db.update_citation_in_scene(&scene_1, &citation_3);
  EXPECT_EQ(3, _db.citation_map_count());
  _db.update_citation_in_scene(&scene_2, &citation_1);
  EXPECT_EQ(4, _db.citation_map_count());
  _db.update_citation_in_scene(&scene_2, &citation_2);
  EXPECT_EQ(5, _db.citation_map_count());
  _db.update_citation_in_scene(&scene_2, &citation_3);
  EXPECT_EQ(6, _db.citation_map_count());
  _db.update_citation_in_scene(&scene_3, &citation_1);
  EXPECT_EQ(7, _db.citation_map_count());
  _db.update_citation_in_scene(&scene_3, &citation_2);
  EXPECT_EQ(8, _db.citation_map_count());
  _db.update_citation_in_scene(&scene_3, &citation_3);
  EXPECT_EQ(9, _db.citation_map_count());
}
TEST_F(SQLiteDriver, CitationMap_TEST(Citation_Removal))
{
  using namespace pfc;
  Scene scene_1;
  Scene scene_2;
  Scene scene_3;

  Citation citation_1;
  Citation citation_2;
  Citation citation_3;
  scene_1.name = "Opening";
  scene_2.name = "Middle";
  scene_3.name = "Ending";

  _db.update_scene(&scene_1);
  _db.update_scene(&scene_2);
  _db.update_scene(&scene_3);

  _db.select_scene(&scene_1);
  _db.select_scene(&scene_2);
  _db.select_scene(&scene_3);

  citation_1.key = "skeleton key";
  citation_1.title = "Mr. Bones' Wild Ride";
  citation_1.authors = "Nathan;Angel";
  citation_1.year = "1000";
  citation_1.publisher = "Boneland";

  citation_2.key = "boss key";
  citation_2.title = "Legend of Zelda";
  citation_2.authors = "Austin;Matthew";
  citation_2.year = "2000";
  citation_2.publisher = "Nintendo";

  citation_3.key = "rsa key";
  citation_3.title = "Extra Security";
  citation_3.authors = "Steven;Lucas";
  citation_3.year = "2019";
  citation_3.publisher = "ARA";

  _db.update_citation(&citation_1);
  _db.update_citation(&citation_2);
  _db.update_citation(&citation_3);

  _db.select_citation(&citation_1);
  _db.select_citation(&citation_2);
  _db.select_citation(&citation_3);

  _db.update_citation_in_scene(&scene_1, &citation_1);
  _db.update_citation_in_scene(&scene_1, &citation_2);
  _db.update_citation_in_scene(&scene_1, &citation_3);
  _db.update_citation_in_scene(&scene_2, &citation_1);
  _db.update_citation_in_scene(&scene_2, &citation_2);
  _db.update_citation_in_scene(&scene_2, &citation_3);
  _db.update_citation_in_scene(&scene_3, &citation_1);
  _db.update_citation_in_scene(&scene_3, &citation_2);
  _db.update_citation_in_scene(&scene_3, &citation_3);

  EXPECT_EQ(9, _db.citation_map_count());
  _db.remove_citation_from_scene(&citation_1, &scene_1);
  EXPECT_EQ(8, _db.citation_map_count());
  _db.remove_citation_from_scene(&citation_1, &scene_2);
  EXPECT_EQ(7, _db.citation_map_count());
  _db.remove_citation_from_scene(&citation_1, &scene_3);
  EXPECT_EQ(6, _db.citation_map_count());
  _db.remove_citation_from_scene(&citation_2, &scene_1);
  EXPECT_EQ(5, _db.citation_map_count());
  _db.remove_citation_from_scene(&citation_2, &scene_2);
  EXPECT_EQ(4, _db.citation_map_count());
  _db.remove_citation_from_scene(&citation_2, &scene_3);
  EXPECT_EQ(3, _db.citation_map_count());
  _db.remove_citation_from_scene(&citation_3, &scene_1);
  EXPECT_EQ(2, _db.citation_map_count());
  _db.remove_citation_from_scene(&citation_3, &scene_2);
  EXPECT_EQ(1, _db.citation_map_count());
  _db.remove_citation_from_scene(&citation_3, &scene_3);
  EXPECT_EQ(0, _db.citation_map_count());
  EXPECT_EQ(3, _db.citation_count());
}
TEST_F(SQLiteDriver, CitationMap_TEST(Citation_Deletion))
{
  using namespace pfc;
  Scene scene_1;
  Scene scene_2;
  Scene scene_3;

  Citation citation_1;
  Citation citation_2;
  Citation citation_3;
  scene_1.name = "Opening";
  scene_2.name = "Middle";
  scene_3.name = "Ending";

  _db.update_scene(&scene_1);
  _db.update_scene(&scene_2);
  _db.update_scene(&scene_3);

  _db.select_scene(&scene_1);
  _db.select_scene(&scene_2);
  _db.select_scene(&scene_3);

  citation_1.key = "skeleton key";
  citation_1.title = "Mr. Bones' Wild Ride";
  citation_1.authors = "Nathan;Angel";
  citation_1.year = "1000";
  citation_1.publisher = "Boneland";

  citation_2.key = "boss key";
  citation_2.title = "Legend of Zelda";
  citation_2.authors = "Austin;Matthew";
  citation_2.year = "2000";
  citation_2.publisher = "Nintendo";

  citation_3.key = "rsa key";
  citation_3.title = "Extra Security";
  citation_3.authors = "Steven;Lucas";
  citation_3.year = "2019";
  citation_3.publisher = "ARA";

  _db.update_citation(&citation_1);
  _db.update_citation(&citation_2);
  _db.update_citation(&citation_3);

  _db.select_citation(&citation_1);
  _db.select_citation(&citation_2);
  _db.select_citation(&citation_3);

  _db.update_citation_in_scene(&scene_1, &citation_1);
  _db.update_citation_in_scene(&scene_1, &citation_2);
  _db.update_citation_in_scene(&scene_1, &citation_3);
  _db.update_citation_in_scene(&scene_2, &citation_1);
  _db.update_citation_in_scene(&scene_2, &citation_2);
  _db.update_citation_in_scene(&scene_2, &citation_3);
  _db.update_citation_in_scene(&scene_3, &citation_1);
  _db.update_citation_in_scene(&scene_3, &citation_2);
  _db.update_citation_in_scene(&scene_3, &citation_3);

  EXPECT_EQ(9, _db.citation_map_count());
  EXPECT_EQ(3, _db.citation_count(&scene_1));
  EXPECT_EQ(3, _db.citation_count(&scene_2));
  EXPECT_EQ(3, _db.citation_count(&scene_3));
  _db.remove_citation(&citation_1);
  EXPECT_EQ(6, _db.citation_map_count());
  EXPECT_EQ(2, _db.citation_count(&scene_1));
  EXPECT_EQ(2, _db.citation_count(&scene_2));
  EXPECT_EQ(2, _db.citation_count(&scene_3));
  _db.remove_citation(&citation_2);
  EXPECT_EQ(3, _db.citation_map_count());
  EXPECT_EQ(1, _db.citation_count(&scene_1));
  EXPECT_EQ(1, _db.citation_count(&scene_2));
  EXPECT_EQ(1, _db.citation_count(&scene_3));
  _db.remove_citation(&citation_3);
  EXPECT_EQ(0, _db.citation_map_count());
  EXPECT_EQ(0, _db.citation_count(&scene_1));
  EXPECT_EQ(0, _db.citation_count(&scene_2));
  EXPECT_EQ(0, _db.citation_count(&scene_3));
}
TEST_F(SQLiteDriver, CitationMap_TEST(Scene_Deletion))
{
  using namespace pfc;
  Scene scene_1;
  Scene scene_2;
  Scene scene_3;

  Citation citation_1;
  Citation citation_2;
  Citation citation_3;
  scene_1.name = "Opening";
  scene_2.name = "Middle";
  scene_3.name = "Ending";

  _db.update_scene(&scene_1);
  _db.update_scene(&scene_2);
  _db.update_scene(&scene_3);

  _db.select_scene(&scene_1);
  _db.select_scene(&scene_2);
  _db.select_scene(&scene_3);

  citation_1.key = "skeleton key";
  citation_1.title = "Mr. Bones' Wild Ride";
  citation_1.authors = "Nathan;Angel";
  citation_1.year = "1000";
  citation_1.publisher = "Boneland";

  citation_2.key = "boss key";
  citation_2.title = "Legend of Zelda";
  citation_2.authors = "Austin;Matthew";
  citation_2.year = "2000";
  citation_2.publisher = "Nintendo";

  citation_3.key = "rsa key";
  citation_3.title = "Extra Security";
  citation_3.authors = "Steven;Lucas";
  citation_3.year = "2019";
  citation_3.publisher = "ARA";

  _db.update_citation(&citation_1);
  _db.update_citation(&citation_2);
  _db.update_citation(&citation_3);

  _db.select_citation(&citation_1);
  _db.select_citation(&citation_2);
  _db.select_citation(&citation_3);

  _db.update_citation_in_scene(&scene_1, &citation_1);
  _db.update_citation_in_scene(&scene_1, &citation_2);
  _db.update_citation_in_scene(&scene_1, &citation_3);
  _db.update_citation_in_scene(&scene_2, &citation_1);
  _db.update_citation_in_scene(&scene_2, &citation_2);
  _db.update_citation_in_scene(&scene_2, &citation_3);
  _db.update_citation_in_scene(&scene_3, &citation_1);
  _db.update_citation_in_scene(&scene_3, &citation_2);
  _db.update_citation_in_scene(&scene_3, &citation_3);

  EXPECT_EQ(9, _db.citation_map_count());
  _db.remove_scene(&scene_1);
  EXPECT_EQ(6, _db.citation_map_count());
  _db.remove_scene(&scene_2);
  EXPECT_EQ(3, _db.citation_map_count());
  _db.remove_scene(&scene_3);
  EXPECT_EQ(0, _db.citation_map_count());
}
//-------------------------------------------------------------------------------
#ifndef DISABLE_LocationMap_TEST
#define LocationMap_TEST(X) X##_LocationMap_Test
#else
#define LocationMap_TEST(X) DISABLED_##X##_LocationMap_Test
#endif
TEST_F(SQLiteDriver, LocationMap_TEST(Location_Insertion))
{
  using namespace pfc;
  Scene scene_1;
  Scene scene_2;
  Scene scene_3;
  LocationMap map_1;
  LocationMap map_2;
  LocationMap map_3;
  LocationMap map_4;
  LocationMap map_5;
  LocationMap map_6;
  LocationMap map_7;
  LocationMap map_8;
  LocationMap map_9;
  Location location_1;
  Location location_2;
  Location location_3;
  scene_1.name = "Opening";
  scene_2.name = "Middle";
  scene_3.name = "Ending";
  EXPECT_EQ(0, _db.location_map_count());
  _db.update_scene(&scene_1);
  EXPECT_EQ(1, _db.location_map_count());
  _db.update_scene(&scene_2);
  EXPECT_EQ(2, _db.location_map_count());
  _db.update_scene(&scene_3);
}
TEST_F(SQLiteDriver, LocationMap_TEST(Location_Removal))
{
  using namespace pfc;
  Scene scene_1;
  Scene scene_2;
  Scene scene_3;
  LocationMap map_1;
  LocationMap map_2;
  LocationMap map_3;
  scene_1.name = "Opening";
  scene_2.name = "Middle";
  scene_3.name = "Ending";

  _db.update_scene(&scene_1);
  _db.update_scene(&scene_2);
  _db.update_scene(&scene_3);

  _db.select_scene(&scene_1);
  _db.select_scene(&scene_2);
  _db.select_scene(&scene_3);

  EXPECT_EQ(3, _db.location_map_count());
  _db.remove_scene(&scene_1);
  EXPECT_EQ(2, _db.location_map_count());
  _db.remove_scene(&scene_2);
  EXPECT_EQ(1, _db.location_map_count());
  _db.remove_scene(&scene_3);
  EXPECT_EQ(0, _db.location_map_count());
  EXPECT_EQ(0, _db.location_count());
}
TEST_F(SQLiteDriver, DISABLED_Location_Deletion)
{ // We're no longer using this logic as part of the DB, locations<->scenes are 1-to-1, and users can only delete locations
  // Given that this relationship could be changed again down the line I'd like to leave these tests in, but there's no reason to be running them for now
  using namespace pfc;
  Scene scene_1;
  Scene scene_2;
  Scene scene_3;
  LocationMap map_1;
  LocationMap map_2;
  LocationMap map_3;
  LocationMap map_4;
  LocationMap map_5;
  LocationMap map_6;
  LocationMap map_7;
  LocationMap map_8;
  LocationMap map_9;
  Location location_1;
  Location location_2;
  Location location_3;
  scene_1.name = "Opening";
  scene_2.name = "Middle";
  scene_3.name = "Ending";

  _db.update_scene(&scene_1);
  _db.update_scene(&scene_2);
  _db.update_scene(&scene_3);

  _db.select_scene(&scene_1);
  _db.select_scene(&scene_2);
  _db.select_scene(&scene_3);

  location_1.name = "My House";
  location_1.environment = "Bathroom floor";

  location_2.name = "Work Building";
  location_2.environment = "Office Floor";

  location_3.name = "Restaurant";
  location_3.environment = "Booth";

  _db.update_location(&location_1);
  _db.update_location(&location_2);
  _db.update_location(&location_3);

  _db.select_location(&location_1);
  _db.select_location(&location_2);
  _db.select_location(&location_3);

  _db.update_location_in_scene(&scene_1, &location_1);
  _db.update_location_in_scene(&scene_1, &location_2);
  _db.update_location_in_scene(&scene_1, &location_3);
  _db.update_location_in_scene(&scene_2, &location_1);
  _db.update_location_in_scene(&scene_2, &location_2);
  _db.update_location_in_scene(&scene_2, &location_3);
  _db.update_location_in_scene(&scene_3, &location_1);
  _db.update_location_in_scene(&scene_3, &location_2);
  _db.update_location_in_scene(&scene_3, &location_3);

  EXPECT_EQ(3, _db.location_map_count());
  EXPECT_EQ(1, _db.location_count(&scene_1));
  EXPECT_EQ(1, _db.location_count(&scene_2));
  EXPECT_EQ(1, _db.location_count(&scene_3));
  _db.remove_location(&location_1);
  EXPECT_EQ(3, _db.location_map_count());
  EXPECT_EQ(1, _db.location_count(&scene_1));
  EXPECT_EQ(1, _db.location_count(&scene_2));
  EXPECT_EQ(1, _db.location_count(&scene_3));
  _db.remove_location(&location_2);
  EXPECT_EQ(3, _db.location_map_count());
  EXPECT_EQ(1, _db.location_count(&scene_1));
  EXPECT_EQ(1, _db.location_count(&scene_2));
  EXPECT_EQ(1, _db.location_count(&scene_3));
  _db.remove_location(&location_3);
  EXPECT_EQ(0, _db.location_map_count());
  EXPECT_EQ(0, _db.location_count(&scene_1));
  EXPECT_EQ(0, _db.location_count(&scene_2));
  EXPECT_EQ(0, _db.location_count(&scene_3));
}
TEST_F(SQLiteDriver, LocationMap_TEST(Test_Scene_Deletion))
{
  using namespace pfc;
  Scene scene_1;
  Scene scene_2;
  Scene scene_3;
  LocationMap map_1;
  LocationMap map_2;
  LocationMap map_3;
  LocationMap map_4;
  LocationMap map_5;
  LocationMap map_6;
  LocationMap map_7;
  LocationMap map_8;
  LocationMap map_9;
  Location location_1;
  Location location_2;
  Location location_3;
  scene_1.name = "Opening";
  scene_2.name = "Middle";
  scene_3.name = "Ending";

  _db.update_scene(&scene_1);
  _db.update_scene(&scene_2);
  _db.update_scene(&scene_3);

  _db.select_scene(&scene_1);
  _db.select_scene(&scene_2);
  _db.select_scene(&scene_3);

  location_1.name = "My House";
  location_1.environment = "Bathroom floor";

  location_2.name = "Work Building";
  location_2.environment = "Office Floor";

  location_3.name = "Restaurant";
  location_3.environment = "Booth";

  _db.update_location(&location_1);
  _db.update_location(&location_2);
  _db.update_location(&location_3);

  _db.select_location(&location_1);
  _db.select_location(&location_2);
  _db.select_location(&location_3);

  _db.update_location_in_scene(&scene_1, &location_1);
  _db.update_location_in_scene(&scene_1, &location_2);
  _db.update_location_in_scene(&scene_1, &location_3);
  _db.update_location_in_scene(&scene_2, &location_1);
  _db.update_location_in_scene(&scene_2, &location_2);
  _db.update_location_in_scene(&scene_2, &location_3);
  _db.update_location_in_scene(&scene_3, &location_1);
  _db.update_location_in_scene(&scene_3, &location_2);
  _db.update_location_in_scene(&scene_3, &location_3);

  EXPECT_EQ(3, _db.location_map_count());
  _db.remove_scene(&scene_1);
  EXPECT_EQ(2, _db.location_map_count());
  _db.remove_scene(&scene_2);
  EXPECT_EQ(1, _db.location_map_count());
  _db.remove_scene(&scene_3);
  EXPECT_EQ(0, _db.location_map_count());
}
//-------------------------------------------------------------------------------
#ifndef DISABLE_Backdoor_TEST
#define Backdoor_TEST(X) X##_Backdoor
#else
#define Backdoor_TEST(X) DISABLED_##X##_Backdoor
#endif
TEST_F(SQLiteDriver, Backdoor_TEST(get_authors))
{
  using namespace pfc;
  Author author_1;

  author_1.first = "Solid";
  author_1.last = "Snake";
  author_1.email = "SolidSnake@MetalGear.com";

  EXPECT_EQ(0, _db.author_count());
  EXPECT_TRUE(_db.update_author(&author_1));

  auto list = _db.get_authors();

  EXPECT_TRUE(list[0]->first.compare(author_1.first) == 0);
  EXPECT_TRUE(list[0]->last.compare(author_1.last) == 0);
  EXPECT_TRUE(list[0]->email.compare(author_1.email) == 0);
}
TEST_F(SQLiteDriver, Backdoor_TEST(get_assessments))
{
  using namespace pfc;
  Assessment assessment_1; //   Assessment* assessment_1 = new Assessment(NULLPTR);

  assessment_1.name = "Math Test";
  assessment_1.description = "Gotta do some math";
  assessment_1.type = "Don't fail";
  assessment_1.available_points = 100;
  assessment_1.criteria = "what's 2+2";

  EXPECT_EQ(0, _db.assessment_count());
  EXPECT_TRUE(_db.update_assessment(&assessment_1));

  auto list = _db.get_assessments();

  EXPECT_TRUE(list[0]->name.compare(assessment_1.name) == 0);
  EXPECT_TRUE(list[0]->description.compare(assessment_1.description) == 0);
  EXPECT_TRUE(list[0]->type.compare(assessment_1.type) == 0);
  EXPECT_TRUE(list[0]->available_points == assessment_1.available_points);
  EXPECT_TRUE(list[0]->criteria.compare(assessment_1.criteria) == 0);
}
TEST_F(SQLiteDriver, Backdoor_TEST(get_citations))
{
  using namespace pfc;
  Citation citation_1;

  citation_1.key = "skeleton key";
  citation_1.title = "Mr. Bones' Wild Ride";
  citation_1.authors = "Nathan;Angel";
  citation_1.year = "1000";
  citation_1.publisher = "Boneland";

  EXPECT_EQ(0, _db.citation_count());
  EXPECT_TRUE(_db.update_citation(&citation_1));

  auto list = _db.get_citations();

  EXPECT_TRUE(list[0]->key.compare(citation_1.key) == 0);
  EXPECT_TRUE(list[0]->title.compare(citation_1.title) == 0);
  EXPECT_TRUE(list[0]->authors.compare(citation_1.authors) == 0);
  EXPECT_TRUE(list[0]->year.compare(citation_1.year) == 0);
  EXPECT_TRUE(list[0]->publisher.compare(citation_1.publisher) == 0);
}
TEST_F(SQLiteDriver, Backdoor_TEST(get_events))
{
  using namespace pfc;
  Event event_1;

  event_1.name = "Barbecue";
  event_1.description = "Barbecue Barbecue";
  event_1.category = "DIALOG";
  event_1.fidelity = "ACTION";
  event_1.fk_actor_1 = "2";
  event_1.fk_actor_2 = "3";
  event_1.equipment = "5;6;7";
  event_1.details = "People cooking and eating meat outdoors";
  EXPECT_EQ(0, _db.event_count());
  EXPECT_TRUE(_db.update_event(&event_1));
  auto list = _db.get_events();

  EXPECT_TRUE(list[0]->name.compare(event_1.name) == 0);
  EXPECT_TRUE(list[0]->description.compare(event_1.description) == 0);
  EXPECT_TRUE(list[0]->equipment.compare(event_1.equipment) == 0);
  EXPECT_TRUE(list[0]->category.compare(event_1.category) == 0);
  EXPECT_TRUE(list[0]->fidelity.compare(event_1.fidelity) == 0);
  EXPECT_TRUE(list[0]->fk_actor_1.compare(event_1.fk_actor_1) == 0);
  EXPECT_TRUE(list[0]->fk_actor_2.compare(event_1.fk_actor_2) == 0);
  EXPECT_TRUE(list[0]->details.compare(event_1.details) == 0);
}

TEST_F(SQLiteDriver, Backdoor_TEST(get_equipments))
{
  using namespace pfc;
  Equipment equipment_1;

  equipment_1.name = "Keytar";
  equipment_1.type = 1;
  equipment_1.description = "Got a sick keytar solo later";
  equipment_1.image = "music stand";
  equipment_1.citations = { 1 };

  EXPECT_EQ(0, _db.equipment_count());
  EXPECT_TRUE(_db.update_equipment(&equipment_1));

  auto list = _db.get_equipments();

  EXPECT_TRUE(list[0]->name.compare(equipment_1.name) == 0);
  EXPECT_TRUE(list[0]->description.compare(equipment_1.description) == 0);
  EXPECT_TRUE(list[0]->type == equipment_1.type);
  EXPECT_TRUE(list[0]->image.compare(equipment_1.image) == 0);
  EXPECT_TRUE(list[0]->citations.compare(equipment_1.citations) == 0);
}
TEST_F(SQLiteDriver, Backdoor_TEST(get_injuries))
{
  using namespace pfc;
  Injury injury_1;

  injury_1.medical_name = "Keyboardus Faceus";
  injury_1.common_name = "Keyboard Face";
  injury_1.description = "Looks like you took a nap on your keyboard";
  injury_1.citations = { 1 };

  EXPECT_EQ(0, _db.injury_count());
  EXPECT_TRUE(_db.update_injury(&injury_1));

  auto list = _db.get_injuries();

  EXPECT_TRUE(list[0]->medical_name.compare(injury_1.medical_name) == 0);
  EXPECT_TRUE(list[0]->description.compare(injury_1.description) == 0);
  EXPECT_TRUE(list[0]->common_name.compare(injury_1.common_name) == 0);
  EXPECT_TRUE(list[0]->citations.compare(injury_1.citations) == 0);
}
TEST_F(SQLiteDriver, Backdoor_TEST(get_injury_sets))
{
  using namespace pfc;
  InjurySet injury_set_1;

  injury_set_1.name = "Keyboardus Faceus";
  injury_set_1.description = "Looks like you took a nap on your keyboard";
  injury_set_1.injuries = { 1 };
  injury_set_1.locations = { 1 };
  injury_set_1.severities = { 1 };

  EXPECT_EQ(0, _db.injury_set_count());
  EXPECT_TRUE(_db.update_injury_set(&injury_set_1));

  auto list = _db.get_injury_sets();

  EXPECT_TRUE(list[0]->name.compare(injury_set_1.name) == 0);
  EXPECT_TRUE(list[0]->description.compare(injury_set_1.description) == 0);
  EXPECT_TRUE(list[0]->injuries.compare(injury_set_1.injuries) == 0);
  EXPECT_TRUE(list[0]->locations.compare(injury_set_1.locations) == 0);
  EXPECT_TRUE(list[0]->severities.compare(injury_set_1.severities) == 0);
}
TEST_F(SQLiteDriver, Backdoor_TEST(get_role_maps))
{
  using namespace pfc;
  Scene scene_1;
  RoleMap map_1;
  Role role_1;
  scene_1.name = "Opening";

  _db.update_scene(&scene_1);

  _db.select_scene(&scene_1);

  role_1.name = "Software Developer";
  role_1.description = "Bash your head against a wall for 8 hours a day";

  _db.update_role(&role_1);
  _db.select_role(&role_1);
  _db.update_role_in_scene(&scene_1, &role_1);

  auto list = _db.get_role_maps();

  EXPECT_EQ(1, list[0]->fk_scene);
  EXPECT_EQ(1, list[0]->fk_role);
}
TEST_F(SQLiteDriver, Backdoor_TEST(get_event_maps))
{
  using namespace pfc;
  Scene scene_1;
  EventMap map_1;
  Event event_1;
  scene_1.name = "Opening";

  _db.update_scene(&scene_1);

  _db.select_scene(&scene_1);

  event_1.name = "Software Developer";
  event_1.description = "Bash your head against a wall for 8 hours a day";

  _db.update_event(&event_1);
  _db.select_event(&event_1);
  _db.update_event_in_scene(&scene_1, &event_1);

  auto list = _db.get_event_maps();

  EXPECT_EQ(1, list[0]->fk_scene);
  EXPECT_EQ(1, list[0]->fk_event);
}
//-------------------------------------------------------------------------------
TEST_F(SQLiteDriver, Backdoor_TEST(get_citation_maps))
{
  using namespace pfc;
  Scene scene_1;
  CitationMap map_1;
  Citation citation_1;
  scene_1.name = "Opening";

  _db.update_scene(&scene_1);

  _db.select_scene(&scene_1);

  citation_1.key = "skeleton key";
  citation_1.title = "Mr. Bones' Wild Ride";
  citation_1.authors = "Nathan;Angel";
  citation_1.year = "1000";
  citation_1.publisher = "Boneland";

  _db.update_citation(&citation_1);
  _db.select_citation(&citation_1);
  _db.update_citation_in_scene(&scene_1, &citation_1);

  auto list = _db.get_citation_maps();

  EXPECT_EQ(1, list[0]->fk_scene);
  EXPECT_EQ(1, list[0]->fk_citation);
}
TEST_F(SQLiteDriver, Backdoor_TEST(get_equipment_maps))
{
  using namespace pfc;
  Scene scene_1;
  Equipment equipment_1;
  EquipmentMap map_1;
  scene_1.name = "Opening";

  ASSERT_TRUE(_db.update_scene(&scene_1));
  ASSERT_TRUE(_db.select_scene(&scene_1));

  equipment_1.name = "Keytar";
  equipment_1.type = 1;
  equipment_1.description = "Got a sick keytar solo later";
  equipment_1.image = "music stand";
  equipment_1.citations = { 1 };

  ASSERT_TRUE(_db.update_equipment(&equipment_1));
  ASSERT_TRUE(_db.select_equipment(&equipment_1));

  map_1.scene->assign(scene_1);
  map_1.equipment->assign(equipment_1);
  _db.update_equipment_in_scene(&map_1);

  auto list = _db.get_equipment_maps();
  EXPECT_EQ(1, list.size());
  EXPECT_EQ(1, list[0]->scene->id);
  EXPECT_EQ(1, list[0]->equipment->id);
}
TEST_F(SQLiteDriver, Backdoor_TEST(get_objectives))
{
  using namespace pfc;
  Objective objective_1;

  objective_1.name = "Kill the Troll";
  objective_1.description = "There is a troll in the forest in a great big whole who has some gold. Kill it";
  objective_1.citations = "1;2;3";

  EXPECT_EQ(0, _db.objective_count());
  EXPECT_TRUE(_db.update_objective(&objective_1));

  auto list = _db.get_objectives();

  EXPECT_TRUE(list[0]->name.compare(objective_1.name) == 0);
  EXPECT_TRUE(list[0]->description.compare(objective_1.description) == 0);
  EXPECT_TRUE(list[0]->citations.compare(objective_1.citations) == 0);
}
TEST_F(SQLiteDriver, Backdoor_TEST(get_properties))
{
  using namespace pfc;
  Property property_1;

  property_1.name = "Solid";
  property_1.value = "Snake";

  EXPECT_EQ(0, _db.property_count());
  EXPECT_TRUE(_db.update_property(&property_1));

  auto list = _db.get_properties();

  EXPECT_TRUE(list[0]->name.compare(property_1.name) == 0);
  EXPECT_TRUE(list[0]->value.compare(property_1.value) == 0);
}
//-------------------------------------------------------------------------------
TEST_F(SQLiteDriver, Backdoor_TEST(get_roles))
{
  using namespace pfc;
  Role role_1;

  role_1.name = "Software Developer";
  role_1.description = "Bash your head against a wall for 8 hours a day";

  EXPECT_EQ(0, _db.role_count());
  EXPECT_TRUE(_db.update_role(&role_1));

  auto list = _db.get_roles();

  EXPECT_TRUE(list[0]->name.compare(role_1.name) == 0);
  EXPECT_TRUE(list[0]->description.compare(role_1.description) == 0);
}
TEST_F(SQLiteDriver, Backdoor_TEST(get_treatments))
{
  using namespace pfc;
  Treatment treatment_1;

  treatment_1.medical_name = "Soupus De Gallinus";
  treatment_1.common_name = "Chicken Soup";
  treatment_1.description = "Good for the soul";
  treatment_1.equipment = { 2 };
  treatment_1.citations = { 1 };

  EXPECT_EQ(0, _db.treatment_count());
  EXPECT_TRUE(_db.update_treatment(&treatment_1));

  auto list = _db.get_treatments();

  EXPECT_TRUE(list[0]->medical_name.compare(treatment_1.medical_name) == 0);
  EXPECT_TRUE(list[0]->description.compare(treatment_1.description) == 0);
  EXPECT_TRUE(list[0]->common_name.compare(treatment_1.common_name) == 0);
  EXPECT_TRUE(list[0]->equipment.compare(treatment_1.equipment) == 0);
  EXPECT_TRUE(list[0]->citations.compare(treatment_1.citations) == 0);
}
TEST_F(SQLiteDriver, Backdoor_TEST(get_scenes))
{
  using namespace pfc;
  Scene scene_1;

  scene_1.name = "Opening";
  scene_1.description = "Desc";
  scene_1.time_of_day = "00:00:00";
  scene_1.time_in_simulation = 0;
  scene_1.weather = "Overcast";
  scene_1.events = "1";
  scene_1.items = "2";
  scene_1.roles = "3";
  scene_1.details = "Dets";

  EXPECT_EQ(0, _db.scene_count());
  EXPECT_TRUE(_db.update_scene(&scene_1));

  auto list = _db.get_scenes();

  EXPECT_TRUE(list[0]->name.compare(scene_1.name) == 0);
  EXPECT_TRUE(list[0]->description.compare(scene_1.description) == 0);
  EXPECT_TRUE(list[0]->time_of_day.compare(scene_1.time_of_day) == 0);
  EXPECT_TRUE(list[0]->time_in_simulation == scene_1.time_in_simulation);
  EXPECT_TRUE(list[0]->weather.compare(scene_1.weather) == 0);
  EXPECT_TRUE(list[0]->events.compare(scene_1.events) == 0);
  EXPECT_TRUE(list[0]->items.compare(scene_1.items) == 0);
  EXPECT_TRUE(list[0]->roles.compare(scene_1.roles) == 0);
  EXPECT_TRUE(list[0]->details.compare(scene_1.details) == 0);
}
//-------------------------------------------------------------------------------
#ifndef DISABLE_REMOVAL_TEST
#define REMOVAL_TEST(X) Remove_##X
#else
#define REMOVAL_TEST(X) DISABLED_Remove_##X
#endif
TEST_F(SQLiteDriver, REMOVAL_TEST(equipment_from_treatments))
{
  using namespace pfc;
  Treatment treatment_1;
  Treatment treatment_2;
  Treatment treatment_3;
  Treatment treatment_4;
  Treatment treatment_5;
  Treatment treatment_6;

  treatment_1.medical_name = "Soupus De Gallinus";
  treatment_1.common_name = "Chicken Soup";
  treatment_1.description = "Good for the soul";
  treatment_1.equipment = "1;2;3;4";
  treatment_1.citations = { 1 };

  treatment_6.medical_name = "Soupus De Gallinus2";
  treatment_6.common_name = "Chicken Soup2";
  treatment_6.description = "Good for the soul";
  treatment_6.equipment = "1;2;3;4";
  treatment_6.citations = { 1 };

  treatment_2.equipment = "1;3;4";
  treatment_3.equipment = "1;3";
  treatment_4.equipment = "3";
  treatment_5.equipment = "";

  _db.update_treatment(&treatment_1);
  _db.update_treatment(&treatment_6);
  _db.select_treatment(&treatment_1);
  _db.select_treatment(&treatment_6);

  EXPECT_TRUE(_db.remove_equipment_from_treatments(2));
  _db.select_treatment(&treatment_1);
  _db.select_treatment(&treatment_6);
  EXPECT_EQ(treatment_2.equipment.toStdString(), treatment_1.equipment.toStdString());
  EXPECT_EQ(treatment_2.equipment.toStdString(), treatment_6.equipment.toStdString());
  EXPECT_TRUE(_db.remove_equipment_from_treatments(4));
  _db.select_treatment(&treatment_1);
  _db.select_treatment(&treatment_6);
  EXPECT_EQ(treatment_3.equipment.toStdString(), treatment_1.equipment.toStdString());
  EXPECT_EQ(treatment_3.equipment.toStdString(), treatment_6.equipment.toStdString());
  EXPECT_TRUE(_db.remove_equipment_from_treatments(1));
  _db.select_treatment(&treatment_1);
  _db.select_treatment(&treatment_6);
  EXPECT_EQ(treatment_4.equipment.toStdString(), treatment_1.equipment.toStdString());
  EXPECT_EQ(treatment_4.equipment.toStdString(), treatment_6.equipment.toStdString());
  EXPECT_TRUE(_db.remove_equipment_from_treatments(3));
  _db.select_treatment(&treatment_1);
  _db.select_treatment(&treatment_6);
  EXPECT_EQ(treatment_5.equipment.toStdString(), treatment_1.equipment.toStdString());
  EXPECT_EQ(treatment_5.equipment.toStdString(), treatment_6.equipment.toStdString());
}
TEST_F(SQLiteDriver, REMOVAL_TEST(citation_from_treatments))
{
  using namespace pfc;
  Treatment treatment_1;
  Treatment treatment_2;
  Treatment treatment_3;
  Treatment treatment_4;
  Treatment treatment_5;
  Treatment treatment_6;

  treatment_1.medical_name = "Soupus De Gallinus";
  treatment_1.common_name = "Chicken Soup";
  treatment_1.description = "Good for the soul";
  treatment_1.citations = "1;2;3;4";
  treatment_1.equipment = { 1 };

  treatment_6.medical_name = "Soupus De Gallinus2";
  treatment_6.common_name = "Chicken Soup2";
  treatment_6.description = "Good for the soul";
  treatment_6.citations = "1;2;3;4";
  treatment_6.equipment = { 1 };

  treatment_2.citations = "1;3;4";
  treatment_3.citations = "1;3";
  treatment_4.citations = "3";
  treatment_5.citations = "";

  _db.update_treatment(&treatment_1);
  _db.update_treatment(&treatment_6);
  _db.select_treatment(&treatment_1);
  _db.select_treatment(&treatment_6);

  EXPECT_TRUE(_db.remove_citation_from_treatments(2));
  _db.select_treatment(&treatment_1);
  _db.select_treatment(&treatment_6);
  EXPECT_EQ(treatment_2.citations.toStdString(), treatment_1.citations.toStdString());
  EXPECT_EQ(treatment_2.citations.toStdString(), treatment_6.citations.toStdString());
  EXPECT_TRUE(_db.remove_citation_from_treatments(4));
  _db.select_treatment(&treatment_1);
  _db.select_treatment(&treatment_6);
  EXPECT_EQ(treatment_3.citations.toStdString(), treatment_1.citations.toStdString());
  EXPECT_EQ(treatment_3.citations.toStdString(), treatment_6.citations.toStdString());
  EXPECT_TRUE(_db.remove_citation_from_treatments(1));
  _db.select_treatment(&treatment_1);
  _db.select_treatment(&treatment_6);
  EXPECT_EQ(treatment_4.citations.toStdString(), treatment_1.citations.toStdString());
  EXPECT_EQ(treatment_4.citations.toStdString(), treatment_6.citations.toStdString());
  EXPECT_TRUE(_db.remove_citation_from_treatments(3));
  _db.select_treatment(&treatment_1);
  _db.select_treatment(&treatment_6);
  EXPECT_EQ(treatment_5.citations.toStdString(), treatment_1.citations.toStdString());
  EXPECT_EQ(treatment_5.citations.toStdString(), treatment_6.citations.toStdString());
}
TEST_F(SQLiteDriver, REMOVAL_TEST(citation_from_injuries))
{
  using namespace pfc;
  Injury injury_1;
  Injury injury_2;
  Injury injury_3;
  Injury injury_4;
  Injury injury_5;
  Injury injury_6;

  injury_1.medical_name = "Keyboardus Faceus";
  injury_1.common_name = "Keyboard Face";
  injury_1.description = "Looks like you took a nap on your keyboard";
  injury_1.citations = "1;2;3;4";

  injury_6.medical_name = "Keyboardus Faceus2";
  injury_6.common_name = "Keyboard Face2";
  injury_6.description = "Looks like you took a nap on your keyboard";
  injury_6.citations = "1;2;3;4";

  injury_2.citations = "1;3;4";
  injury_3.citations = "1;3";
  injury_4.citations = "3";
  injury_5.citations = "";

  _db.update_injury(&injury_1);
  _db.update_injury(&injury_6);
  _db.select_injury(&injury_1);
  _db.select_injury(&injury_6);

  EXPECT_TRUE(_db.remove_citation_from_injuries(2));
  _db.select_injury(&injury_1);
  _db.select_injury(&injury_6);
  EXPECT_EQ(injury_2.citations.toStdString(), injury_1.citations.toStdString());
  EXPECT_EQ(injury_2.citations.toStdString(), injury_6.citations.toStdString());
  EXPECT_TRUE(_db.remove_citation_from_injuries(4));
  _db.select_injury(&injury_1);
  _db.select_injury(&injury_6);
  EXPECT_EQ(injury_3.citations.toStdString(), injury_1.citations.toStdString());
  EXPECT_EQ(injury_3.citations.toStdString(), injury_6.citations.toStdString());
  EXPECT_TRUE(_db.remove_citation_from_injuries(1));
  _db.select_injury(&injury_1);
  _db.select_injury(&injury_6);
  EXPECT_EQ(injury_4.citations.toStdString(), injury_1.citations.toStdString());
  EXPECT_EQ(injury_4.citations.toStdString(), injury_6.citations.toStdString());
  EXPECT_TRUE(_db.remove_citation_from_injuries(3));
  _db.select_injury(&injury_1);
  _db.select_injury(&injury_6);
  EXPECT_EQ(injury_5.citations.toStdString(), injury_1.citations.toStdString());
  EXPECT_EQ(injury_5.citations.toStdString(), injury_6.citations.toStdString());
}
TEST_F(SQLiteDriver, REMOVAL_TEST(citation_from_equipment))
{
  using namespace pfc;
  Equipment equipment_1;
  Equipment equipment_2;
  Equipment equipment_3;
  Equipment equipment_4;
  Equipment equipment_5;
  Equipment equipment_6;

  equipment_1.name = "Keytar";
  equipment_1.type = 1;
  equipment_1.description = "Got a sick keytar solo later";
  equipment_1.image = ("music stand");
  equipment_1.citations = "1;2;3;4";

  equipment_6.name = "Keytar2";
  equipment_6.type = 1;
  equipment_6.description = "Got a sick keytar solo later";
  equipment_6.image = ("music stand");
  equipment_6.citations = "1;2;3;4";

  equipment_2.citations = "1;3;4";
  equipment_3.citations = "1;3";
  equipment_4.citations = "3";
  equipment_5.citations = "";

  _db.update_equipment(&equipment_1);
  _db.update_equipment(&equipment_6);
  _db.select_equipment(&equipment_1);
  _db.select_equipment(&equipment_6);

  EXPECT_TRUE(_db.remove_citation_from_equipment(2));
  _db.select_equipment(&equipment_1);
  _db.select_equipment(&equipment_6);
  EXPECT_EQ(equipment_2.citations.toStdString(), equipment_1.citations.toStdString());
  EXPECT_EQ(equipment_2.citations.toStdString(), equipment_6.citations.toStdString());
  EXPECT_TRUE(_db.remove_citation_from_equipment(4));
  _db.select_equipment(&equipment_1);
  _db.select_equipment(&equipment_6);
  EXPECT_EQ(equipment_3.citations.toStdString(), equipment_1.citations.toStdString());
  EXPECT_EQ(equipment_3.citations.toStdString(), equipment_6.citations.toStdString());
  EXPECT_TRUE(_db.remove_citation_from_equipment(1));
  _db.select_equipment(&equipment_1);
  _db.select_equipment(&equipment_6);
  EXPECT_EQ(equipment_4.citations.toStdString(), equipment_1.citations.toStdString());
  EXPECT_EQ(equipment_4.citations.toStdString(), equipment_6.citations.toStdString());
  EXPECT_TRUE(_db.remove_citation_from_equipment(3));
  _db.select_equipment(&equipment_1);
  _db.select_equipment(&equipment_6);
  EXPECT_EQ(equipment_5.citations.toStdString(), equipment_1.citations.toStdString());
  EXPECT_EQ(equipment_5.citations.toStdString(), equipment_6.citations.toStdString());
}
TEST_F(SQLiteDriver, REMOVAL_TEST(citation_from_objectives))
{
  using namespace pfc;
  Objective objective_1;
  Objective objective_2;
  Objective objective_3;
  Objective objective_4;
  Objective objective_5;
  Objective objective_6;

  objective_1.name = "Kill the Troll";
  objective_1.description = "There is a troll in the forest in a great big whole who has some gold. Kill it";
  objective_1.citations = "1;2;3;4";

  objective_6.name = "Kill the Troll2";
  objective_6.description = "There is a troll in the forest in a great big whole who has some gold. Kill it";
  objective_6.citations = "1;2;3;4";

  objective_2.citations = "1;3;4";
  objective_3.citations = "1;3";
  objective_4.citations = "3";
  objective_5.citations = "";

  _db.update_objective(&objective_1);
  _db.update_objective(&objective_6);
  _db.select_objective(&objective_1);
  _db.select_objective(&objective_6);

  EXPECT_TRUE(_db.remove_citation_from_objectives(2));
  _db.select_objective(&objective_1);
  _db.select_objective(&objective_6);
  EXPECT_EQ(objective_2.citations.toStdString(), objective_1.citations.toStdString());
  EXPECT_EQ(objective_2.citations.toStdString(), objective_6.citations.toStdString());
  EXPECT_TRUE(_db.remove_citation_from_objectives(4));
  _db.select_objective(&objective_1);
  _db.select_objective(&objective_6);
  EXPECT_EQ(objective_3.citations.toStdString(), objective_1.citations.toStdString());
  EXPECT_EQ(objective_3.citations.toStdString(), objective_6.citations.toStdString());
  EXPECT_TRUE(_db.remove_citation_from_objectives(1));
  _db.select_objective(&objective_1);
  _db.select_objective(&objective_6);
  EXPECT_EQ(objective_4.citations.toStdString(), objective_1.citations.toStdString());
  EXPECT_EQ(objective_4.citations.toStdString(), objective_6.citations.toStdString());
  EXPECT_TRUE(_db.remove_citation_from_objectives(3));
  _db.select_objective(&objective_1);
  _db.select_objective(&objective_6);
  EXPECT_EQ(objective_5.citations.toStdString(), objective_1.citations.toStdString());
  EXPECT_EQ(objective_5.citations.toStdString(), objective_6.citations.toStdString());
}
TEST_F(SQLiteDriver, REMOVAL_TEST(injury_from_injury_sets))
{
  using namespace pfc;
  InjurySet injury_set_1;
  InjurySet injury_set_2;
  InjurySet injury_set_3;
  InjurySet injury_set_4;
  InjurySet injury_set_5;
  InjurySet injury_set_6;

  injury_set_1.name = "Keyboardus Faceus";
  injury_set_1.description = "Punch;Kick;Stomp;Tear";
  injury_set_1.injuries = "1;2;3;4";
  injury_set_1.locations = "Head;Shoulders;Knees;Toes";
  injury_set_1.severities = "Good;Better;Bad;Worse";

  injury_set_6.name = "Keyboardus Faceus";
  injury_set_6.description = "Punch;Kick;Stomp;Tear";
  injury_set_6.injuries = "1;2;3;4";
  injury_set_6.locations = "Head;Shoulders;Knees;Toes";
  injury_set_6.severities = "Good;Better;Bad;Worse";

  injury_set_2.injuries = "1;3;4";
  injury_set_2.locations = "Head;Knees;Toes";
  injury_set_2.description = "Punch;Stomp;Tear";
  injury_set_2.severities = "Good;Bad;Worse";
  injury_set_3.injuries = "1;3";
  injury_set_3.locations = "Head;Knees";
  injury_set_3.description = "Punch;Stomp";
  injury_set_3.severities = "Good;Bad";
  injury_set_4.injuries = "3";
  injury_set_4.locations = "Knees";
  injury_set_4.description = "Stomp";
  injury_set_4.severities = "Bad";
  injury_set_5.injuries = "";
  injury_set_5.locations = "";
  injury_set_5.description = "";
  injury_set_5.severities = "";

  _db.update_injury_set(&injury_set_1);
  _db.update_injury_set(&injury_set_6);
  _db.select_injury_set(&injury_set_1);
  _db.select_injury_set(&injury_set_6);

  EXPECT_EQ(_db.list_find(std::string("1;2;3;4"), std::string("1")), 0);
  EXPECT_EQ(_db.list_find(std::string("1;2;3;4"), std::string("2")), 1);
  EXPECT_EQ(_db.list_find(std::string("1;2;3;4"), std::string("3")), 2);
  EXPECT_EQ(_db.list_find(std::string("1;2;3;4"), std::string("4")), 3);

  EXPECT_TRUE(_db.remove_injury_from_injury_sets(100));
  _db.select_injury_set(&injury_set_1);
  _db.select_injury_set(&injury_set_6);
  EXPECT_EQ(injury_set_6.injuries.toStdString(), injury_set_1.injuries.toStdString());
  EXPECT_EQ(injury_set_6.description.toStdString(), injury_set_1.description.toStdString());
  EXPECT_EQ(injury_set_6.locations.toStdString(), injury_set_1.locations.toStdString());
  EXPECT_EQ(injury_set_6.severities.toStdString(), injury_set_1.severities.toStdString());

  EXPECT_TRUE(_db.remove_injury_from_injury_sets(2));
  _db.select_injury_set(&injury_set_1);
  _db.select_injury_set(&injury_set_6);
  EXPECT_EQ(injury_set_2.injuries.toStdString(), injury_set_1.injuries.toStdString());
  EXPECT_EQ(injury_set_2.description.toStdString(), injury_set_1.description.toStdString());
  EXPECT_EQ(injury_set_2.locations.toStdString(), injury_set_1.locations.toStdString());
  EXPECT_EQ(injury_set_2.severities.toStdString(), injury_set_1.severities.toStdString());

  EXPECT_EQ(injury_set_2.injuries.toStdString(), injury_set_6.injuries.toStdString());
  EXPECT_EQ(injury_set_2.description.toStdString(), injury_set_6.description.toStdString());
  EXPECT_EQ(injury_set_2.locations.toStdString(), injury_set_6.locations.toStdString());
  EXPECT_EQ(injury_set_2.severities.toStdString(), injury_set_6.severities.toStdString());

  EXPECT_TRUE(_db.remove_injury_from_injury_sets(4));
  _db.select_injury_set(&injury_set_1);
  _db.select_injury_set(&injury_set_6);
  EXPECT_EQ(injury_set_3.injuries.toStdString(), injury_set_1.injuries.toStdString());
  EXPECT_EQ(injury_set_3.description.toStdString(), injury_set_1.description.toStdString());
  EXPECT_EQ(injury_set_3.locations.toStdString(), injury_set_1.locations.toStdString());
  EXPECT_EQ(injury_set_3.severities.toStdString(), injury_set_1.severities.toStdString());

  EXPECT_EQ(injury_set_3.injuries.toStdString(), injury_set_6.injuries.toStdString());
  EXPECT_EQ(injury_set_3.description.toStdString(), injury_set_6.description.toStdString());
  EXPECT_EQ(injury_set_3.locations.toStdString(), injury_set_6.locations.toStdString());
  EXPECT_EQ(injury_set_3.severities.toStdString(), injury_set_6.severities.toStdString());

  EXPECT_TRUE(_db.remove_injury_from_injury_sets(1));
  _db.select_injury_set(&injury_set_1);
  _db.select_injury_set(&injury_set_6);
  EXPECT_EQ(injury_set_4.injuries.toStdString(), injury_set_1.injuries.toStdString());
  EXPECT_EQ(injury_set_4.description.toStdString(), injury_set_1.description.toStdString());
  EXPECT_EQ(injury_set_4.locations.toStdString(), injury_set_1.locations.toStdString());
  EXPECT_EQ(injury_set_4.severities.toStdString(), injury_set_1.severities.toStdString());

  EXPECT_EQ(injury_set_4.injuries.toStdString(), injury_set_6.injuries.toStdString());
  EXPECT_EQ(injury_set_4.description.toStdString(), injury_set_6.description.toStdString());
  EXPECT_EQ(injury_set_4.locations.toStdString(), injury_set_6.locations.toStdString());
  EXPECT_EQ(injury_set_4.severities.toStdString(), injury_set_6.severities.toStdString());

  EXPECT_TRUE(_db.remove_injury_from_injury_sets(3));
  _db.select_injury_set(&injury_set_1);
  _db.select_injury_set(&injury_set_6);
  EXPECT_EQ(injury_set_5.injuries.toStdString(), injury_set_1.injuries.toStdString());
  EXPECT_EQ(injury_set_5.description.toStdString(), injury_set_1.description.toStdString());
  EXPECT_EQ(injury_set_5.locations.toStdString(), injury_set_1.locations.toStdString());
  EXPECT_EQ(injury_set_5.severities.toStdString(), injury_set_1.severities.toStdString());

  EXPECT_EQ(injury_set_5.injuries.toStdString(), injury_set_6.injuries.toStdString());
  EXPECT_EQ(injury_set_5.description.toStdString(), injury_set_6.description.toStdString());
  EXPECT_EQ(injury_set_5.locations.toStdString(), injury_set_6.locations.toStdString());
  EXPECT_EQ(injury_set_5.severities.toStdString(), injury_set_6.severities.toStdString());
}
//-------------------------------------------------------------------------------
#ifndef DISABLE_Serialization_TEST
#define Serialization_TEST(X) Serialization_load_##X
#else
#define Serialization_TEST(X) DISABLED_Serialization_load_##X
#endif
TEST_F(SerializationTest, Serialization_TEST(Authors))
{
  pfc::schema::PFC::load_authors(std::move(scenario_schema), _db, load_result);
  EXPECT_EQ(1, _db.author_count());
  auto temp = _db.get_authors();
  EXPECT_EQ(0, temp[0]->email.compare("Test@Email.com"));
  EXPECT_EQ(0, temp[0]->first.compare("Test_First_Name"));
  EXPECT_EQ(0, temp[0]->last.compare("Test_Last_Name"));
  EXPECT_EQ(0, temp[0]->phone.compare("Test_Phone#"));
  EXPECT_EQ(0, temp[0]->zip.compare("Test_Zip"));
  EXPECT_EQ(0, temp[0]->state.compare("Test_State"));
  EXPECT_EQ(0, temp[0]->country.compare("Test_Country"));
}
TEST_F(SerializationTest, Serialization_TEST(Assessment))
{
  pfc::schema::PFC::load_assessments(std::move(scenario_schema), _db, load_result);
  EXPECT_EQ(1, _db.assessment_count());
  auto temp = _db.get_assessments();
  EXPECT_EQ(0, temp[0]->name.compare("Assessment_Name"));
  EXPECT_EQ(0, temp[0]->description.compare("Assessment_Description"));
  EXPECT_EQ(0, temp[0]->type.compare("binary"));
  EXPECT_EQ(1, temp[0]->available_points);
  EXPECT_EQ(0, temp[0]->criteria.compare("Assessment_Criteria"));
}
TEST_F(SerializationTest, Serialization_TEST(Citation))
{
  pfc::schema::PFC::load_citations(std::move(scenario_schema), _db, load_result);
  EXPECT_EQ(1, _db.citation_count());
  auto temp = _db.get_citations();
  EXPECT_EQ(0, temp[0]->key.compare("Citation_Key"));
  EXPECT_EQ(0, temp[0]->title.compare("Citation_Title"));
  EXPECT_EQ(0, temp[0]->authors.compare("Citation_Authors"));
  EXPECT_EQ(0, temp[0]->year.compare("Citation_Year"));
}
TEST_F(SerializationTest, Serialization_TEST(Equipment))
{
  pfc::schema::PFC::load_equipment(std::move(scenario_schema), _db, load_result);
  EXPECT_EQ(1, _db.equipment_count());
  auto temp = _db.get_equipments();
  EXPECT_EQ(0, temp[0]->name.compare("Equipment_Name"));
  EXPECT_EQ(0, temp[0]->description.compare("Equipment_Description"));
  EXPECT_EQ(0, temp[0]->type);
  EXPECT_EQ(0, temp[0]->image.compare("Equipment_Image"));
}
TEST_F(SerializationTest, Serialization_TEST(Event))
{
  pfc::schema::PFC::load_events(std::move(scenario_schema), _db, load_result);
  EXPECT_EQ(1, _db.event_count());
  auto temp = _db.get_events();
  EXPECT_EQ(0, temp[0]->name.compare("Event_Name"));
  EXPECT_EQ(0, temp[0]->description.compare("Event_Description"));
  EXPECT_EQ(0, temp[0]->category.compare("ACTION"));
  EXPECT_EQ(0, temp[0]->fidelity.compare("LOW"));
  EXPECT_EQ(0, temp[0]->details.compare("Event_Details"));
  EXPECT_EQ(0, temp[0]->fk_actor_1.compare("Actor_1"));
  EXPECT_EQ(0, temp[0]->fk_actor_2.compare("Actor_2"));
}
TEST_F(SerializationTest, Serialization_TEST(Trauma))
{
  pfc::schema::PFC::load_injuries(std::move(scenario_schema), _db, load_result);
  EXPECT_EQ(1, _db.injury_count());
  auto temp = _db.get_injuries();
  EXPECT_EQ(0, temp[0]->medical_name.compare("Injury_Medical_Name"));
  EXPECT_EQ(0, temp[0]->common_name.compare("Injury_Common_Name"));
  EXPECT_EQ(0, temp[0]->description.compare("Injury_Description"));
  EXPECT_EQ(1, temp[0]->lower_bound);
  EXPECT_EQ(2, temp[0]->upper_bound);
  EXPECT_EQ(0, temp[0]->citations.compare("0;1"));
}
TEST_F(SerializationTest, Serialization_TEST(InjurySet))
{
  pfc::schema::PFC::load_injury_sets(std::move(scenario_schema), _db, load_result);
  EXPECT_EQ(1, _db.injury_set_count());
  auto temp = _db.get_injury_sets();
  EXPECT_EQ(0, temp[0]->name.compare("Injury_Set_Name"));
  EXPECT_EQ(0, temp[0]->description.compare("Description_1;Description_2"));
  EXPECT_EQ(0, temp[0]->injuries.compare("0;1"));
  EXPECT_EQ(0, temp[0]->locations.compare("Location_1;Location_2"));
  EXPECT_EQ(0, temp[0]->severities.compare("0;1"));
}
TEST_F(SerializationTest, Serialization_TEST(Locations))
{
  pfc::schema::PFC::load_locations(std::move(scenario_schema), _db, load_result);
  EXPECT_EQ(1, _db.location_count());
  auto temp = _db.get_locations();
  EXPECT_EQ(0, temp[0]->name.compare("Scene_Name Location"));
}
TEST_F(SerializationTest, Serialization_TEST(Objective))
{
  pfc::schema::PFC::load_objectives(std::move(scenario_schema), _db, load_result);
  EXPECT_EQ(1, _db.objective_count());
  auto temp = _db.get_objectives();
  EXPECT_EQ(0, temp[0]->name.compare("Objective_Name"));
  EXPECT_EQ(0, temp[0]->description.compare("Objective_Description"));
  EXPECT_EQ(0, temp[0]->citations.compare("0;1"));
}
TEST_F(SerializationTest, Serialization_TEST(Role))
{
  pfc::schema::PFC::load_roles(std::move(scenario_schema), _db, load_result);
  EXPECT_EQ(1, _db.role_count());
  auto temp = _db.get_roles();
  EXPECT_EQ(0, temp[0]->name.compare("Role_Name"));
  EXPECT_EQ(0, temp[0]->description.compare("Role_Description"));
}
TEST_F(SerializationTest, Serialization_TEST(Scene))
{
  pfc::schema::PFC::load_scenes(std::move(scenario_schema), _db, load_result);
  EXPECT_EQ(1, _db.scene_count());
  auto temp = _db.get_scenes();
  EXPECT_EQ(0, temp[0]->name.compare("Scene_Name"));
  EXPECT_EQ(0, temp[0]->description.compare("Scene_Description"));
  EXPECT_EQ(0, temp[0]->time_of_day.compare("00:00:00"));
  EXPECT_EQ(0, temp[0]->time_in_simulation);
}
TEST_F(SerializationTest, Serialization_TEST(Treatment))
{
  pfc::schema::PFC::load_treatments(std::move(scenario_schema), _db, load_result);
  EXPECT_EQ(1, _db.treatment_count());
  auto temp = _db.get_treatments();
  EXPECT_EQ(0, temp[0]->medical_name.compare("Treatment_Medical_Name"));
  EXPECT_EQ(0, temp[0]->common_name.compare("Treatment_Common_Name"));
  EXPECT_EQ(0, temp[0]->description.compare("Treatment_Description"));
  EXPECT_EQ(0, temp[0]->citations.compare("0;1"));
  EXPECT_EQ(0, temp[0]->equipment.compare("0;1"));
}