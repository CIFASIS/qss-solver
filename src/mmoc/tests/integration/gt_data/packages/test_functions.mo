package test_functions

import test_functions2;

function set2
	input Real a;
    output Real b;
    output Real c;
algorithm
	(b, c) := set1(a);
end set2;

function set3
	input Real a;
    output Real b;
    output Real c;
algorithm
    (b, c) := set1(a);
end set3;

end test_functions;	