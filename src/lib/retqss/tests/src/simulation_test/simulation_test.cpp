#define private public

#include <math.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

extern "C"
{
#include "common/data.h"
#include "common/simulator.h"
#include "qss/qss_simulator.h"
}

#include "test_common/qss_test_utils.h"

#include "gtest/gtest.h"

using namespace std;


class retQSSSimulationTest : public testing::Test
{
protected:
	virtual void SetUp()
	{
		this->vols.push_back(33);
		this->vols.push_back(49);
		this->vols.push_back(53);
		this->vols.push_back(54);
		this->vols.push_back(58);
		this->vols.push_back(59);
		this->vols.push_back(63);
		this->vols.push_back(47);
		this->vols.push_back(31);
		this->vols.push_back(32);
		this->vols.push_back(16);
		this->vols.push_back(12);
		this->vols.push_back(8);
		this->vols.push_back(0);
	}

public:
	std::vector<int> vols;
};


TEST_F(retQSSSimulationTest, simulation_test)
{
	retQSSTest::run_model(false);

	std::ifstream volumes("volumeID.dat");
	std::ifstream property("prop.dat");

	EXPECT_TRUE(volumes.good());
	EXPECT_TRUE(property.good());

	std::string line_str;
	double time, prop;
	int vol_id;
	size_t i = 0;

	while(std::getline(volumes, line_str))
	{
		std::stringstream line(line_str);
		line >> time >> vol_id;

		EXPECT_EQ(vol_id, this->vols[i]);
		i++;
	}
	EXPECT_EQ(i, this->vols.size());

	i = 0;
	while(std::getline(property, line_str))
	{
		std::stringstream line(line_str);
		line >> time >> prop;

		EXPECT_EQ(prop, 2*this->vols[i]);
		i++;
	}
	EXPECT_EQ(i, this->vols.size());
}
