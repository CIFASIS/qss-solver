// Copyright 2001-2020 iRobot Corporation (Unpublished). All Rights Reserved.

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstring>

#include "util/visitors/algebraics.h"

using ::testing::Return;
using ::testing::StrictMock;

/// @addtogroup QSSSolverUTests
/// @{

using namespace MicroModelica::Util;

/// Unit tests for algebraics variable visitor.
class AlgebraicsTest : public ::testing::Test, public Algebraics {
  public:
  AlgebraicsTest() : Algebraics() {}

  virtual ~AlgebraicsTest() {}
};

TEST_F(AlgebraicsTest, testInitialState) { EXPECT_TRUE(true); }

/// @}
