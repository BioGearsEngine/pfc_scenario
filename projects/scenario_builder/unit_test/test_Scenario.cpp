#include <gtest/gtest.h> // googletest header file

#include <memory>
#include "cpp/Scenario.h"
#include "xsd/cpp/MilitaryScenario_1.0.0.hxx"

TEST(Scenario, Constructor)
{
  using namespace ::pfc;
  std::unique_ptr<pfc::Scenario> empty_scenario;

  EXPECT_NO_THROW(empty_scenario = std::make_unique<pfc::Scenario>());
  EXPECT_NO_THROW(empty_scenario = nullptr);
}
//-------------------------------------------------------------------------------
TEST(Scenario, Name)
{
  using namespace ::pfc;
  std::unique_ptr<pfc::Scenario> empty_scenario;

  EXPECT_NO_THROW(empty_scenario = std::make_unique<pfc::Scenario>());

  constexpr auto empty_scenario_name = "Test Scenario";
  empty_scenario->Name(empty_scenario_name);
  EXPECT_STREQ(empty_scenario->Name().toStdString().c_str(), empty_scenario_name);
}
//-------------------------------------------------------------------------------
TEST(Scenario, Type)
{
  using namespace ::pfc;
  std::unique_ptr<pfc::Scenario> empty_scenario;

  EXPECT_NO_THROW(empty_scenario = std::make_unique<pfc::Scenario>());

  constexpr auto empty_scenario_type = "Medical Training";
  empty_scenario->Type(empty_scenario_type);
  EXPECT_STREQ(empty_scenario->Type().toStdString().c_str(), empty_scenario_type);
}
//-------------------------------------------------------------------------------
TEST(Scenario, Version)
{
  using namespace ::pfc;
  std::unique_ptr<pfc::Scenario> empty_scenario;

  EXPECT_NO_THROW(empty_scenario = std::make_unique<pfc::Scenario>());

  constexpr auto empty_scenario_version = "0.0.1";
  empty_scenario->Version(empty_scenario_version);
  EXPECT_STREQ(empty_scenario->Version().toStdString().c_str(), empty_scenario_version);
}
//-------------------------------------------------------------------------------
TEST(Scenario, ModificationDate)
{
  using namespace ::pfc;
  std::unique_ptr<pfc::Scenario> empty_scenario;

  EXPECT_NO_THROW(empty_scenario = std::make_unique<pfc::Scenario>());

  const auto empty_scenario_modDate = QDate(2019,5,9);
  empty_scenario->ModificationDate(empty_scenario_modDate);
  EXPECT_EQ(empty_scenario->ModificationDate(), empty_scenario_modDate);
}
//-------------------------------------------------------------------------------
TEST(Scenario, SecurityClassification)
{
  using namespace ::pfc;
  std::unique_ptr<pfc::Scenario> empty_scenario;

  EXPECT_NO_THROW(empty_scenario = std::make_unique<pfc::Scenario>());

  constexpr auto empty_scenario_security = "Non-Classified";
  empty_scenario->SecurityClassification(empty_scenario_security);
  EXPECT_STREQ(empty_scenario->SecurityClassification().toStdString().c_str(), empty_scenario_security);
}
//-------------------------------------------------------------------------------
TEST(Scenario, ReleaseRestriction)
{
  using namespace ::pfc;
  std::unique_ptr<pfc::Scenario> empty_scenario;

  EXPECT_NO_THROW(empty_scenario = std::make_unique<pfc::Scenario>());

  constexpr auto empty_scenario_releaseRestriction = "No Restriction";
  empty_scenario->ReleaseRestriction(empty_scenario_releaseRestriction);
  EXPECT_STREQ(empty_scenario->ReleaseRestriction().toStdString().c_str(), empty_scenario_releaseRestriction);
}
//------------------------------------------------------------------------------
TEST(Scenario, Purpose)
{
  using namespace ::pfc;
  std::unique_ptr<pfc::Scenario> empty_scenario;

  EXPECT_NO_THROW(empty_scenario = std::make_unique<pfc::Scenario>());

  constexpr auto empty_scenario_purpose = "Purpose: Testing PFC Units";
  empty_scenario->Purpose(empty_scenario_purpose);
  EXPECT_STREQ(empty_scenario->Purpose().toStdString().c_str(), empty_scenario_purpose);
}
//-------------------------------------------------------------------------------
TEST(Scenario, Description)
{
  using namespace ::pfc;
  std::unique_ptr<pfc::Scenario> empty_scenario;

  EXPECT_NO_THROW(empty_scenario = std::make_unique<pfc::Scenario>());

  constexpr auto empty_scenario_description = "Description: A 72 hour medical scenario for unit testing";
  empty_scenario->Description(empty_scenario_description);
  EXPECT_STREQ(empty_scenario->Description().toStdString().c_str(), empty_scenario_description);
}
//-------------------------------------------------------------------------------
TEST(Scenario, UseLimitaiton)
{
  using namespace ::pfc;
  std::unique_ptr<pfc::Scenario> empty_scenario;

  EXPECT_NO_THROW(empty_scenario = std::make_unique<pfc::Scenario>());

  constexpr auto empty_scenario_limitation = "Doesn't do well for TCCC training";
  empty_scenario->UseLimitation(empty_scenario_limitation);
  EXPECT_STREQ(empty_scenario->UseLimitation().toStdString().c_str(), empty_scenario_limitation);
}
//-------------------------------------------------------------------------------