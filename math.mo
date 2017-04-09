package math
function random
		input Real x;
		output Real y;
		external "C" y = mmo_random(x) annotation(
																Library="mmo_math",
																Include="#include <mmo_math.h>");
	end random;
	function rand
		input Real x;
		output Real y;
		external "C" y = mmo_rand(x) annotation(
																Library="mmo_math",
																Include="#include <mmo_math.h>");
	end rand;
function getRandomMarkedValue
		input Real x;
		output Real y;
		external "C" y = mmo_getRandomMarkedValue(x) annotation(
																Library="mmo_math",
																Include="#include <mmo_math.h>");
	end getRandomMarkedValue;
	function getRandomValue
		input Real x;
		output Real y;
		external "C" y = mmo_getRandomValue(x) annotation(
																Library="mmo_math",
																Include="#include <mmo_math.h>");
	end getRandomValue;
	function exponential
		input Real x;
		output Real y;
		external "C" y = mmo_exponential(x) annotation(
																Library="mmo_math",
																Include="#include <mmo_math.h>");
	end exponential;
	function uniform
		input Real a;
		input Real b;
		output Real y;
		external "C" y = mmo_uniform(a,b) annotation(
																Library="mmo_math",
																Include="#include <mmo_math.h>");
	end uniform;
	function normal
		input Real x;
		output Real y;
		external "C" y = mmo_normal(x) annotation(
																Library="mmo_math",
																Include="#include <mmo_math.h>");
	end normal;
	function pow
		input Real a;
		input Real b;
		output Real c;
		external "C" c = pow(a,b);
	end pow; 
	function root
		input Real a;
		input Real b;
		output Real c;
		external "C" c = pow(a,1/b);
	end root;
end math;
