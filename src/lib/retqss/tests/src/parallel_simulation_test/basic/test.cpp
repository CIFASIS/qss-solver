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


class retQSSParallelSimulationTest : public testing::Test
{
protected:
	virtual void SetUp()
	{
		this->vols.push_back(157);
		this->vols.push_back(97);
		this->vols.push_back(109);
		this->vols.push_back(169);
		this->vols.push_back(157);
		this->vols.push_back(97);
		this->vols.push_back(109);
		this->vols.push_back(169);
		this->vols.push_back(157);
		this->vols.push_back(97);
		this->vols.push_back(109);
		this->vols.push_back(169);
		this->vols.push_back(157);
		this->vols.push_back(97);
		this->vols.push_back(109);
		this->vols.push_back(169);
		this->vols.push_back(157);
		this->vols.push_back(97);
		this->vols.push_back(109);
		this->vols.push_back(169);
		this->vols.push_back(157);
		this->vols.push_back(97);
		this->vols.push_back(109);
		this->vols.push_back(169);
		this->vols.push_back(157);
		this->vols.push_back(97);
		this->vols.push_back(109);
		this->vols.push_back(169);
		this->vols.push_back(157);
		this->vols.push_back(97);
		this->vols.push_back(109);
		this->vols.push_back(169);
		this->vols.push_back(157);
		this->vols.push_back(158);
		this->vols.push_back(98);
		this->vols.push_back(110);
		this->vols.push_back(170);
		this->vols.push_back(158);
		this->vols.push_back(98);
		this->vols.push_back(110);
		this->vols.push_back(170);
		this->vols.push_back(158);
		this->vols.push_back(98);
		this->vols.push_back(110);
		this->vols.push_back(170);
		this->vols.push_back(158);
		this->vols.push_back(98);
		this->vols.push_back(110);
		this->vols.push_back(170);
		this->vols.push_back(158);
		this->vols.push_back(98);
		this->vols.push_back(110);
		this->vols.push_back(170);
		this->vols.push_back(158);
		this->vols.push_back(98);
		this->vols.push_back(110);
		this->vols.push_back(170);
		this->vols.push_back(158);
		this->vols.push_back(98);
		this->vols.push_back(110);
		this->vols.push_back(170);
		this->vols.push_back(158);
		this->vols.push_back(98);
		this->vols.push_back(110);
		this->vols.push_back(170);
		this->vols.push_back(158);
		this->vols.push_back(98);
		this->vols.push_back(99);
		this->vols.push_back(111);
		this->vols.push_back(171);
		this->vols.push_back(159);
		this->vols.push_back(99);
		this->vols.push_back(111);
		this->vols.push_back(171);
		this->vols.push_back(159);
		this->vols.push_back(99);
		this->vols.push_back(111);
		this->vols.push_back(171);
		this->vols.push_back(159);
		this->vols.push_back(99);
		this->vols.push_back(111);
		this->vols.push_back(171);
		this->vols.push_back(159);
		this->vols.push_back(99);
		this->vols.push_back(111);
		this->vols.push_back(171);
		this->vols.push_back(159);
		this->vols.push_back(99);
		this->vols.push_back(111);
		this->vols.push_back(171);
		this->vols.push_back(159);
		this->vols.push_back(99);
		this->vols.push_back(111);
		this->vols.push_back(171);
		this->vols.push_back(159);
		this->vols.push_back(99);
		this->vols.push_back(111);
		this->vols.push_back(171);
		this->vols.push_back(159);
		this->vols.push_back(99);
		this->vols.push_back(111);
		this->vols.push_back(171);
		this->vols.push_back(159);
		this->vols.push_back(160);
		this->vols.push_back(100);
		this->vols.push_back(112);
		this->vols.push_back(172);
		this->vols.push_back(160);
		this->vols.push_back(100);
		this->vols.push_back(112);
		this->vols.push_back(172);
		this->vols.push_back(160);
		this->vols.push_back(100);
		this->vols.push_back(112);
		this->vols.push_back(172);
		this->vols.push_back(160);
		this->vols.push_back(100);
		this->vols.push_back(112);
		this->vols.push_back(172);
		this->vols.push_back(160);
		this->vols.push_back(100);
		this->vols.push_back(112);
		this->vols.push_back(172);
		this->vols.push_back(160);
		this->vols.push_back(100);
		this->vols.push_back(112);
		this->vols.push_back(172);
		this->vols.push_back(160);
		this->vols.push_back(100);
		this->vols.push_back(112);
		this->vols.push_back(172);
		this->vols.push_back(160);
		this->vols.push_back(100);
		this->vols.push_back(112);
		this->vols.push_back(172);
		this->vols.push_back(160);
		this->vols.push_back(161);
		this->vols.push_back(101);
		this->vols.push_back(113);
		this->vols.push_back(173);
		this->vols.push_back(161);
		this->vols.push_back(101);
		this->vols.push_back(113);
		this->vols.push_back(173);
		this->vols.push_back(161);
		this->vols.push_back(101);
		this->vols.push_back(113);
		this->vols.push_back(173);
		this->vols.push_back(161);
		this->vols.push_back(101);
		this->vols.push_back(113);
		this->vols.push_back(173);
		this->vols.push_back(161);
		this->vols.push_back(101);
	}

public:
	std::vector<int> vols;
};


TEST_F(retQSSParallelSimulationTest, simulation_test)
{
	retQSSTest::run_model(true);
	int N = 4, lps = 3;
	std::string line_str;
	double time, prop;
	int vol_id;
	size_t i;

	for(int j = 1; j <= N; j++)
	{
		i = 0;
		std::stringstream filename;

		filename << "volumeID[" << j << "]-discrete-" << ((j-1)%lps) << ".dat";
		std::ifstream volumes(filename.str());
		EXPECT_TRUE(volumes.good());

		while(std::getline(volumes, line_str))
		{
			std::stringstream line(line_str);
			line >> time >> vol_id;

			EXPECT_EQ(vol_id, this->vols[i]);
			i++;
		}
		EXPECT_EQ(i, this->vols.size());
	}
}
