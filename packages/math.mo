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

  function rand_seed
    input Real x;
    input Real seed;
    output Real y;
    external "C" y = mmo_rand_seed(x, seed) annotation(
                                Library="mmo_math",
                                Include="#include <mmo_math.h>");
  end rand_seed;

  function rand_int
    input Real x;
    input Real diff;
    output Real y;
    external "C" y = mmo_rand_int(x, diff) annotation(
                                Library="mmo_math",
                                Include="#include <mmo_math.h>");
  end rand_int;

  function integer
    input Real x;
    output Real y;
    external "C" y = mmo_integer(x) annotation(
                                Library="mmo_math",
                                Include="#include <mmo_math.h>");
  end integer;

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

function fabs
    input Real a;
    output Real c;
    external "C" c = fabs(a);
end root;

end math;
