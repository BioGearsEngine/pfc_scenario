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
//ASSESSMENT TESTS--------------------------------------------------------------
TEST_F(TEST_FIXTURE_NAME, Insert_Assessment)
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
}
TEST_F(TEST_FIXTURE_NAME, Select_Assessment)
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
  EXPECT_EQ(assessment_1, id);
  assessment_3.id = 3;
  EXPECT_EQ(assessment_3, name);
}
TEST_F(TEST_FIXTURE_NAME, Remove_Assessment)
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
//AUTHOR TESTS--------------------------------------------------------------
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

  EXPECT_EQ(0, _db.author_count());
  EXPECT_TRUE(_db.update_author(&author_1));
  EXPECT_EQ(1, _db.author_count());
  EXPECT_TRUE(_db.update_author(&author_2));
  EXPECT_EQ(2, _db.author_count());
  EXPECT_TRUE(_db.update_author(&author_3));
  EXPECT_EQ(3, _db.author_count());
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
  EXPECT_EQ(3, _db.author_count());
  EXPECT_TRUE(_db.remove_author(&author_1));
  EXPECT_TRUE(_db.remove_author(&author_2));
  EXPECT_EQ(1, _db.author_count());
}
//CITATION TESTS--------------------------------------------------------------
TEST_F(TEST_FIXTURE_NAME, Insert_Citation)
{
  using namespace pfc;
  Citation citation_1;
  Citation citation_2;
  Citation citation_3;

  citation_1.key = "skeleton key";
  citation_1.title = "Mr. Bones' Wild Ride";
  citation_1.authors = { 1, 2 };
  citation_1.year = "1000";
  citation_1.publisher = "Boneland";

  citation_2.key = "boss key";
  citation_2.title = "Legend of Zelda";
  citation_2.authors = { 3, 4 };
  citation_2.year = "2000";
  citation_2.publisher = "Nintendo";

  citation_3.key = "rsa key";
  citation_3.title = "Extra Security";
  citation_3.authors = { 5, 6 };
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
TEST_F(TEST_FIXTURE_NAME, Select_Citation)
{
  using namespace pfc;
  Citation citation_1;
  Citation citation_2;
  Citation citation_3;

  citation_1.key = "skeleton key";
  citation_1.title = "Mr. Bones' Wild Ride";
  citation_1.authors = { 1, 2 };
  citation_1.year = "1000";
  citation_1.publisher = "Boneland";

  citation_2.key = "boss key";
  citation_2.title = "Legend of Zelda";
  citation_2.authors = { 3, 4 };
  citation_2.year = "2000";
  citation_2.publisher = "Nintendo";

  citation_3.key = "rsa key";
  citation_3.title = "Extra Security";
  citation_3.authors = { 5, 6 };
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
  EXPECT_EQ(citation_1, id);
  citation_3.id = 3;
  EXPECT_EQ(citation_3, key);
}
TEST_F(TEST_FIXTURE_NAME, Remove_Citation)
{
  using namespace pfc;
  Citation citation_1;
  Citation citation_2;
  Citation citation_3;

  citation_1.key = "skeleton key";
  citation_1.title = "Mr. Bones' Wild Ride";
  citation_1.authors = { 1, 2 };
  citation_1.year = "1000";
  citation_1.publisher = "Boneland";

  citation_2.key = "boss key";
  citation_2.title = "Legend of Zelda";
  citation_2.authors = { 3, 4 };
  citation_2.year = "2000";
  citation_2.publisher = "Nintendo";

  citation_3.key = "rsa key";
  citation_3.title = "Extra Security";
  citation_3.authors = { 5, 6 };
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
//EQUIPMENT TESTS--------------------------------------------------------------
TEST_F(TEST_FIXTURE_NAME, Insert_Equipment)
{
  using namespace pfc;
  Equipment equipment_1;
  Equipment equipment_2;
  Equipment equipment_3;

  equipment_1.name = "Keytar";
  equipment_1.description = "Got a sick keytar solo later";
  equipment_1.image = ("music stand");
  equipment_1.citations.push_back(1);

  equipment_2.name = "piano";
  equipment_2.description = "big instrument with keys";
  equipment_2.image = ("piano bench");
  equipment_2.citations.push_back(2);

  equipment_3.name = "bagpipes";
  equipment_3.description = "please stop playing the bagpipes";
  equipment_3.image = ("a bladder");
  equipment_3.citations.push_back(3);

  EXPECT_EQ(0, _db.equipment_count());
  EXPECT_TRUE(_db.update_equipment(&equipment_1));
  EXPECT_EQ(1, _db.equipment_count());
  EXPECT_TRUE(_db.update_equipment(&equipment_2));
  EXPECT_EQ(2, _db.equipment_count());
  EXPECT_TRUE(_db.update_equipment(&equipment_3));
  EXPECT_EQ(3, _db.equipment_count());
}
TEST_F(TEST_FIXTURE_NAME, Select_Equipment)
{
  using namespace pfc;
  Equipment equipment_1;
  Equipment equipment_2;
  Equipment equipment_3;

  equipment_1.name = "Keytar";
  equipment_1.description = "Got a sick keytar solo later";
  equipment_1.image = ("music stand");
  equipment_1.citations.push_back(1);

  equipment_2.name = "piano";
  equipment_2.description = "big instrument with keys";
  equipment_2.image = ("piano bench");
  equipment_2.citations.push_back(2);

  equipment_3.name = "bagpipes";
  equipment_3.description = "please stop playing the bagpipes";
  equipment_3.image = ("a bladder");
  equipment_3.citations.push_back(3);

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
  EXPECT_EQ(equipment_1, id);
  equipment_3.id = 3;
  EXPECT_EQ(equipment_3, name);
}
TEST_F(TEST_FIXTURE_NAME, Remove_Equipment)
{
  using namespace pfc;
  Equipment equipment_1;
  Equipment equipment_2;
  Equipment equipment_3;

  equipment_1.name = "Keytar";
  equipment_1.description = "Got a sick keytar solo later";
  equipment_1.image = ("music stand");
  equipment_1.citations.push_back(1);

  equipment_2.name = "piano";
  equipment_2.description = "big instrument with keys";
  equipment_2.image = ("piano bench");
  equipment_2.citations.push_back(2);

  equipment_3.name = "bagpipes";
  equipment_3.description = "please stop playing the bagpipes";
  equipment_3.image = ("a bladder");
  equipment_3.citations.push_back(3);

  EXPECT_TRUE(_db.update_equipment(&equipment_1));
  EXPECT_TRUE(_db.update_equipment(&equipment_2));
  EXPECT_TRUE(_db.update_equipment(&equipment_3));
  EXPECT_EQ(3, _db.equipment_count());
  EXPECT_TRUE(_db.remove_equipment(&equipment_1));
  EXPECT_TRUE(_db.remove_equipment(&equipment_3));
  EXPECT_EQ(1, _db.equipment_count());
}
//EVENT TESTS--------------------------------------------------------------
TEST_F(TEST_FIXTURE_NAME, Insert_Event)
{

  //TODO: fill out the Event object and make this test work
  // So this is currently failing, but I don't really know what we're going to do with events
  // As such I'm going to leave this alone until I do know
  using namespace pfc;
  Event event_1;
  Event event_2;
  Event event_3;

  event_1.name = "Barbecue";
  event_1.location = 1;
  event_1.actor = 2;
  event_1.equipment = {"Grill","Spatula"};
  event_1.description = "People cooking and eating meat outdoors";

  event_2.name = "Wedding";
  event_2.location = 3;
  event_2.actor = 4;
  event_2.equipment = {"Tuxedo","Ring"};
  event_2.description = "Two people getting married";

  event_3.name = "Funeral";
  event_3.location = 5;
  event_3.actor = 6;
  event_3.equipment = {"Coffin","Body"};
  event_3.description = "Burying a dead person";

  EXPECT_EQ(0, _db.event_count());
  EXPECT_TRUE(_db.update_event(&event_1));
  EXPECT_EQ(1, _db.event_count());
  EXPECT_TRUE(_db.update_event(&event_2));
  EXPECT_EQ(2, _db.event_count());
  EXPECT_TRUE(_db.update_event(&event_3));
  EXPECT_EQ(3, _db.event_count());
}
TEST_F(TEST_FIXTURE_NAME, Select_Event)
{
  using namespace pfc;
  Event event_1;
  Event event_2;
  Event event_3;

  event_1.name = "Barbecue";
  event_1.location = 1;
  event_1.actor = 2;
  event_1.equipment = { "Grill", "Spatula" };
  event_1.description = "People cooking and eating meat outdoors";

  event_2.name = "Wedding";
  event_2.location = 3;
  event_2.actor = 4;
  event_2.equipment = { "Tuxedo", "Ring" };
  event_2.description = "Two people getting married";

  event_3.name = "Funeral";
  event_3.location = 5;
  event_3.actor = 6;
  event_3.equipment = { "Coffin", "Body" };
  event_3.description = "Burying a dead person";

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
  _db.select_event(&name);

  event_1.id = 1;
  EXPECT_EQ(event_1, id);
  event_3.id = 3;
  EXPECT_EQ(event_3, name);
}
TEST_F(TEST_FIXTURE_NAME, Remove_Event)
{
  using namespace pfc;
  Event event_1;
  Event event_2;
  Event event_3;

  event_1.name = "Barbecue";
  event_1.location = 1;
  event_1.actor = 2;
  event_1.equipment = { "Grill", "Spatula" };
  event_1.description = "People cooking and eating meat outdoors";

  event_2.name = "Wedding";
  event_2.location = 3;
  event_2.actor = 4;
  event_2.equipment = { "Tuxedo", "Ring" };
  event_2.description = "Two people getting married";

  event_3.name = "Funeral";
  event_3.location = 5;
  event_3.actor = 6;
  event_3.equipment = { "Coffin", "Body" };
  event_3.description = "Burying a dead person";

  EXPECT_TRUE(_db.update_event(&event_1));
  EXPECT_TRUE(_db.update_event(&event_2));
  EXPECT_TRUE(_db.update_event(&event_3));
  EXPECT_EQ(3, _db.event_count());
  EXPECT_TRUE(_db.remove_event(&event_1));
  EXPECT_TRUE(_db.remove_event(&event_3));
  EXPECT_EQ(1, _db.event_count());
}
//INJURY TESTS--------------------------------------------------------------
TEST_F(TEST_FIXTURE_NAME, Insert_Injury)
{
  using namespace pfc;
  Injury injury_1;
  Injury injury_2;
  Injury injury_3;

  injury_1.medical_name = "Keyboardus Faceus";
  injury_1.common_name = "Keyboard Face";
  injury_1.description = "Looks like you took a nap on your keyboard";
  injury_1.citations.push_back(1);

  injury_2.medical_name = "Hangnailus";
  injury_2.common_name = "Hangnail";
  injury_2.description = "Ouch, really stings";
  injury_2.citations.push_back(2);

  injury_3.medical_name = "Stubbedus Toeus";
  injury_3.common_name = "Stubbed toe";
  injury_3.description = "ouchie";
  injury_3.citations.push_back(3);

  EXPECT_EQ(0, _db.injury_count());
  EXPECT_TRUE(_db.update_injury(&injury_1));
  EXPECT_EQ(1, _db.injury_count());
  EXPECT_TRUE(_db.update_injury(&injury_2));
  EXPECT_EQ(2, _db.injury_count());
  EXPECT_TRUE(_db.update_injury(&injury_3));
  EXPECT_EQ(3, _db.injury_count());
}
TEST_F(TEST_FIXTURE_NAME, Select_Injury)
{
  using namespace pfc;
  Injury injury_1;
  Injury injury_2;
  Injury injury_3;

  injury_1.medical_name = "Keyboardus Faceus";
  injury_1.common_name = "Keyboard Face";
  injury_1.description = "Looks like you took a nap on your keyboard";
  injury_1.citations.push_back(1);

  injury_2.medical_name = "Hangnailus";
  injury_2.common_name = "Hangnail";
  injury_2.description = "Ouch, really stings";
  injury_2.citations.push_back(2);

  injury_3.medical_name = "Stubbedus Toeus";
  injury_3.common_name = "Stubbed toe";
  injury_3.description = "ouchie";
  injury_3.citations.push_back(3);

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
  EXPECT_EQ(injury_1, id);
  injury_2.id = 2;
  EXPECT_EQ(injury_2, medical_name);
  injury_3.id = 3;
  EXPECT_EQ(injury_3, common_name);
}
TEST_F(TEST_FIXTURE_NAME, Remove_Injury)
{
  using namespace pfc;
  Injury injury_1;
  Injury injury_2;
  Injury injury_3;

  injury_1.medical_name = "Keyboardus Faceus";
  injury_1.common_name = "Keyboard Face";
  injury_1.description = "Looks like you took a nap on your keyboard";
  injury_1.citations.push_back(1);

  injury_2.medical_name = "Hangnailus";
  injury_2.common_name = "Hangnail";
  injury_2.description = "Ouch, really stings";
  injury_2.citations.push_back(2);

  injury_3.medical_name = "Stubbedus Toeus";
  injury_3.common_name = "Stubbed toe";
  injury_3.description = "ouchie";
  injury_3.citations.push_back(3);

  EXPECT_TRUE(_db.update_injury(&injury_1));
  EXPECT_TRUE(_db.update_injury(&injury_2));
  EXPECT_TRUE(_db.update_injury(&injury_3));
  EXPECT_EQ(3, _db.injury_count());
  EXPECT_TRUE(_db.remove_injury(&injury_1));
  EXPECT_TRUE(_db.remove_injury(&injury_3));
  EXPECT_EQ(1, _db.injury_count());
}
//LOCATION TESTS--------------------------------------------------------------
TEST_F(TEST_FIXTURE_NAME, Insert_Location)
{
  using namespace pfc;
  Location location_1;
  Location location_2;
  Location location_3;

  location_1.name = "My House";
  location_1.scene_name = "Getting up for work";
  location_1.time_of_day = "1:30PM";
  location_1.environment = "Bathroom floor";

  location_2.name = "Work Building";
  location_2.scene_name = "Staying awake at work";
  location_2.time_of_day = "2:30PM";
  location_2.environment = "Office Floor";

  location_3.name = "Restaurant";
  location_3.scene_name = "Getting Dinner";
  location_3.time_of_day = "3:00PM";
  location_3.environment = "Booth";

  EXPECT_EQ(0, _db.location_count());
  EXPECT_TRUE(_db.update_location(&location_1));
  EXPECT_EQ(1, _db.location_count());
  EXPECT_TRUE(_db.update_location(&location_2));
  EXPECT_EQ(2, _db.location_count());
  EXPECT_TRUE(_db.update_location(&location_3));
  EXPECT_EQ(3, _db.location_count());
}
TEST_F(TEST_FIXTURE_NAME, Select_Location)
{
  using namespace pfc;
  Location location_1;
  Location location_2;
  Location location_3;

  location_1.name = "My House";
  location_1.scene_name = "Getting up for work";
  location_1.time_of_day = "1:30PM";
  location_1.environment = "Bathroom floor";

  location_2.name = "Work Building";
  location_2.scene_name = "Staying awake at work";
  location_2.time_of_day = "2:30PM";
  location_2.environment = "Office Floor";

  location_3.name = "Restaurant";
  location_3.scene_name = "Getting Dinner";
  location_3.time_of_day = "3:00PM";
  location_3.environment = "Booth";

  EXPECT_EQ(0, _db.location_count());
  EXPECT_TRUE(_db.update_location(&location_1));
  EXPECT_EQ(1, _db.location_count());
  EXPECT_TRUE(_db.update_location(&location_2));
  EXPECT_EQ(2, _db.location_count());
  EXPECT_TRUE(_db.update_location(&location_3));
  EXPECT_EQ(3, _db.location_count());

  Location id;
  Location name;
  Location scene_name;

  id.id = 1;
  name.name = "Work Building";
  scene_name.scene_name = "Getting Dinner";

  _db.select_location(&id);
  _db.select_location(&name);
  _db.select_location(&scene_name);

  location_1.id = 1;
  EXPECT_EQ(location_1, id);
  location_2.id = 2;
  EXPECT_EQ(location_2, name);
  location_3.id = 3;
  EXPECT_EQ(location_3, scene_name);
}
TEST_F(TEST_FIXTURE_NAME, Remove_Location)
{
  using namespace pfc;
  Location location_1;
  Location location_2;
  Location location_3;

  location_1.name = "My House";
  location_1.scene_name = "Getting up for work";
  location_1.time_of_day = "1:30PM";
  location_1.environment = "Bathroom floor";

  location_2.name = "Work Building";
  location_2.scene_name = "Staying awake at work";
  location_2.time_of_day = "2:30PM";
  location_2.environment = "Office Floor";

  location_3.name = "Restaurant";
  location_3.scene_name = "Getting Dinner";
  location_3.time_of_day = "3:00PM";
  location_3.environment = "Booth";

  EXPECT_TRUE(_db.update_location(&location_1));
  EXPECT_TRUE(_db.update_location(&location_2));
  EXPECT_TRUE(_db.update_location(&location_3));
  EXPECT_EQ(3, _db.location_count());
  EXPECT_TRUE(_db.remove_location(&location_1));
  EXPECT_TRUE(_db.remove_location(&location_3));
  EXPECT_EQ(1, _db.location_count());
}
//OBJECTIVE TESTS--------------------------------------------------------------
TEST_F(TEST_FIXTURE_NAME, Insert_Objective)
{
  using namespace pfc;
  Objective objective_1;
  Objective objective_2;
  Objective objective_3;

  objective_1.name = "Kill the Troll";
  objective_1.description = "There is a troll in the forest in a great big whole who has some gold. Kill it";
  objective_1.citations = { 1, 2, 3 };

  objective_2.name = "Steal the Trolls Gold";
  objective_2.description = "Gold is really dense. Its like crazy heavy bring a levetation spell.";
  objective_2.citations = { 4, 5, 6 };

  objective_3.name = "Return the Gold to the Quest Giver. Kill the Quest Giver";
  objective_3.description = "If we want to keep the gold we need to steal from the quest giver, but he will need to die before we can do that.";
  objective_3.citations = { 7, 8, 9 };

  EXPECT_EQ(0, _db.objective_count());
  EXPECT_TRUE(_db.update_objective(&objective_1));
  EXPECT_EQ(1, _db.objective_count());
  EXPECT_TRUE(_db.update_objective(&objective_2));
  EXPECT_EQ(2, _db.objective_count());
  EXPECT_TRUE(_db.update_objective(&objective_3));
  EXPECT_EQ(3, _db.objective_count());
}
TEST_F(TEST_FIXTURE_NAME, Select_Objective)
{
  using namespace pfc;
  Objective objective_1;
  Objective objective_2;
  Objective objective_3;

  objective_1.name = "Kill the Troll";
  objective_1.description = "There is a troll in the forest in a great big whole who has some gold. Kill it";
  objective_1.citations = { 1, 2, 3 };

  objective_2.name = "Steal the Trolls Gold";
  objective_2.description = "Gold is really dense. Its like crazy heavy bring a levetation spell.";
  objective_2.citations = { 4, 5, 6 };

  objective_3.name = "Return the Gold to the Quest Giver. Kill the Quest Giver";
  objective_3.description = "If we want to keep the gold we need to steal from the quest giver, but he will need to die before we can do that.";
  objective_3.citations = { 7, 8, 9 };

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
  EXPECT_EQ(objective_1, id);
  objective_3.id = 3;
  EXPECT_EQ(objective_3, name);
}
TEST_F(TEST_FIXTURE_NAME, Remove_Objective)
{
  using namespace pfc;
  Objective objective_1;
  Objective objective_2;
  Objective objective_3;

  objective_1.name = "Kill the Troll";
  objective_1.description = "There is a troll in the forest in a great big whole who has some gold. Kill it";
  objective_1.citations = { 1, 2, 3 };

  objective_2.name = "Steal the Trolls Gold";
  objective_2.description = "Gold is really dense. Its like crazy heavy bring a levetation spell.";
  objective_2.citations = { 4, 5, 6 };

  objective_3.name = "Return the Gold to the Quest Giver. Kill the Quest Giver";
  objective_3.description = "If we want to keep the gold we need to steal from the quest giver, but he will need to die before we can do that.";
  objective_3.citations = { 7, 8, 9 };

  EXPECT_TRUE(_db.update_objective(&objective_1));
  EXPECT_TRUE(_db.update_objective(&objective_2));
  EXPECT_TRUE(_db.update_objective(&objective_3));
  EXPECT_EQ(3, _db.objective_count());
  EXPECT_TRUE(_db.remove_objective(&objective_1));
  EXPECT_TRUE(_db.remove_objective(&objective_3));
  EXPECT_EQ(1, _db.objective_count());
}
//PROP TESTS--------------------------------------------------------------
TEST_F(TEST_FIXTURE_NAME, Insert_Prop)
{
  using namespace pfc;
  Prop prop_1;
  Prop prop_2;
  Prop prop_3;

  prop_1.equipment = "Rubber duck";

  prop_2.equipment = "Bobblehead";

  prop_3.equipment = "Funko pop";

  EXPECT_EQ(0, _db.prop_count());
  EXPECT_TRUE(_db.update_prop(&prop_1));
  EXPECT_EQ(1, _db.prop_count());
  EXPECT_TRUE(_db.update_prop(&prop_2));
  EXPECT_EQ(2, _db.prop_count());
  EXPECT_TRUE(_db.update_prop(&prop_3));
  EXPECT_EQ(3, _db.prop_count());
}
TEST_F(TEST_FIXTURE_NAME, Select_Prop)
{
  using namespace pfc;
  Prop prop_1;
  Prop prop_2;
  Prop prop_3;

  prop_1.equipment = "Rubber duck";

  prop_2.equipment = "Bobblehead";

  prop_3.equipment = "Funko pop";

  EXPECT_EQ(0, _db.prop_count());
  EXPECT_TRUE(_db.update_prop(&prop_1));
  EXPECT_EQ(1, _db.prop_count());
  EXPECT_TRUE(_db.update_prop(&prop_2));
  EXPECT_EQ(2, _db.prop_count());
  EXPECT_TRUE(_db.update_prop(&prop_3));
  EXPECT_EQ(3, _db.prop_count());

  Prop id;

  id.id = 1;

  _db.select_prop(&id);

  prop_1.id = 1;
  EXPECT_EQ(prop_1, id);
}
TEST_F(TEST_FIXTURE_NAME, Remove_Prop)
{
  using namespace pfc;
  Prop prop_1;
  Prop prop_2;
  Prop prop_3;

  prop_1.equipment = "Rubber duck";

  prop_2.equipment = "Bobblehead";

  prop_3.equipment = "Funko pop";

  EXPECT_TRUE(_db.update_prop(&prop_1));
  EXPECT_TRUE(_db.update_prop(&prop_2));
  EXPECT_TRUE(_db.update_prop(&prop_3));
  EXPECT_EQ(3, _db.prop_count());
  EXPECT_TRUE(_db.remove_prop(&prop_1));
  EXPECT_TRUE(_db.remove_prop(&prop_3));
  EXPECT_EQ(1, _db.prop_count());
}
//PROPERTY TESTS--------------------------------------------------------------
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

  EXPECT_EQ(0, _db.property_count());
  EXPECT_TRUE(_db.update_property(&property_1));
  EXPECT_EQ(1, _db.property_count());
  EXPECT_TRUE(_db.update_property(&property_2));
  EXPECT_EQ(2, _db.property_count());
  EXPECT_TRUE(_db.update_property(&property_3));
  EXPECT_EQ(3, _db.property_count());
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
  EXPECT_EQ(3, _db.property_count());
  EXPECT_TRUE(_db.remove_property(&property_1));
  EXPECT_TRUE(_db.remove_property(&property_3));
  EXPECT_EQ(1, _db.property_count());
}
//RESTRICTION TESTS--------------------------------------------------------------
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

  EXPECT_EQ(0, _db.restriction_count());
  EXPECT_TRUE(_db.update_restriction(&restriction_1));
  EXPECT_EQ(1, _db.restriction_count());
  EXPECT_TRUE(_db.update_restriction(&restriction_2));
  EXPECT_EQ(2, _db.restriction_count());
  EXPECT_TRUE(_db.update_restriction(&restriction_3));
  EXPECT_EQ(3, _db.restriction_count());
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
  EXPECT_EQ(3, _db.restriction_count());
  EXPECT_TRUE(_db.remove_restriction(&restriction_1));
  EXPECT_TRUE(_db.remove_restriction(&restriction_3));
  EXPECT_EQ(1, _db.restriction_count());
}
//ROLE TESTS--------------------------------------------------------------
TEST_F(TEST_FIXTURE_NAME, Insert_Role)
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
TEST_F(TEST_FIXTURE_NAME, Select_Role)
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
  EXPECT_EQ(role_1, id);
  role_3.id = 3;
  EXPECT_EQ(role_3, name);
}
TEST_F(TEST_FIXTURE_NAME, Remove_Role)
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
//TREATMENT TESTS--------------------------------------------------------------
TEST_F(TEST_FIXTURE_NAME, Insert_Treatment)
{
  using namespace pfc;
  Treatment treatment_1;
  Treatment treatment_2;
  Treatment treatment_3;

  treatment_1.medical_name = "Soupus De Gallinus";
  treatment_1.common_name = "Chicken Soup";
  treatment_1.description = "Good for the soul";
  treatment_1.equipment.push_back("Spoon");
  treatment_1.citations.push_back(1);

  treatment_2.medical_name = "Massageus";
  treatment_2.common_name = "Massage";
  treatment_2.description = "Back rub";
  treatment_2.equipment.push_back("Massage Chair");
  treatment_2.citations.push_back(2);

  treatment_3.medical_name = "Bashus Headus againstus Wallus";
  treatment_3.common_name = "Bashing Head Against A Wall";
  treatment_3.description = "Oddly Satisfying";
  treatment_3.equipment.push_back("Wall");
  treatment_3.citations.push_back(3);

  EXPECT_EQ(0, _db.treatment_count());
  EXPECT_TRUE(_db.update_treatment(&treatment_1));
  EXPECT_EQ(1, _db.treatment_count());
  EXPECT_TRUE(_db.update_treatment(&treatment_2));
  EXPECT_EQ(2, _db.treatment_count());
  EXPECT_TRUE(_db.update_treatment(&treatment_3));
  EXPECT_EQ(3, _db.treatment_count());
}
TEST_F(TEST_FIXTURE_NAME, Select_Treatment)
{
  using namespace pfc;
  Treatment treatment_1;
  Treatment treatment_2;
  Treatment treatment_3;

  treatment_1.medical_name = "Soupus De Gallinus";
  treatment_1.common_name = "Chicken Soup";
  treatment_1.description = "Good for the soul";
  treatment_1.equipment.push_back("Spoon");
  treatment_1.citations.push_back(1);

  treatment_2.medical_name = "Massageus";
  treatment_2.common_name = "Massage";
  treatment_2.description = "Back rub";
  treatment_2.equipment.push_back("Massage Chair");
  treatment_2.citations.push_back(2);

  treatment_3.medical_name = "Bashus Headus againstus Wallus";
  treatment_3.common_name = "Bashing Head Against A Wall";
  treatment_3.description = "Oddly Satisfying";
  treatment_3.equipment.push_back("Wall");
  treatment_3.citations.push_back(3);

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
  EXPECT_EQ(treatment_1, id);
  treatment_2.id = 2;
  EXPECT_EQ(treatment_2, medical_name);
  treatment_3.id = 3;
  EXPECT_EQ(treatment_3, common_name);
}
TEST_F(TEST_FIXTURE_NAME, Remove_Treatment)
{
  using namespace pfc;
  Treatment treatment_1;
  Treatment treatment_2;
  Treatment treatment_3;

  treatment_1.medical_name = "Soupus De Gallinus";
  treatment_1.common_name = "Chicken Soup";
  treatment_1.description = "Good for the soul";
  treatment_1.equipment.push_back("Spoon");
  treatment_1.citations.push_back(1);

  treatment_2.medical_name = "Massageus";
  treatment_2.common_name = "Massage";
  treatment_2.description = "Back rub";
  treatment_2.equipment.push_back("Massage Chair");
  treatment_2.citations.push_back(2);

  treatment_3.medical_name = "Bashus Headus againstus Wallus";
  treatment_3.common_name = "Bashing Head Against A Wall";
  treatment_3.description = "Oddly Satisfying";
  treatment_3.equipment.push_back("Wall");
  treatment_3.citations.push_back(3);

  EXPECT_TRUE(_db.update_treatment(&treatment_1));
  EXPECT_TRUE(_db.update_treatment(&treatment_2));
  EXPECT_TRUE(_db.update_treatment(&treatment_3));
  EXPECT_EQ(3, _db.treatment_count());
  EXPECT_TRUE(_db.remove_treatment(&treatment_1));
  EXPECT_TRUE(_db.remove_treatment(&treatment_3));
  EXPECT_EQ(1, _db.treatment_count());
}






