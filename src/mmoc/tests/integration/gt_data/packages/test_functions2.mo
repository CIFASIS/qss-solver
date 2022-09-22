package test_functions2

function set1
	input Real a;
    output Real b;
    output Real c;
algorithm
    b := a + 1.0;
    c := a + 2.0;
end set1;

/*
function noOutputFunction
	input Real a;
protected
	Real b;
algorithm
	b := a;
	b := b + 1;
end noOutputFunction;
*/

function setArray
	input Real v;
	input Real arr[2];
    output Real res;
    constant Integer ord = 2;
algorithm
	(arr[1], arr[2+ord]) := set1(v);
end setArray;

function setArrayTest
	input Real arr[2];
	input Real v1;
    output Real res;
algorithm
	arr[1] := v1;
arr[2] := v1;
end setArrayTest;

function setArrayTest1
	output Real arr[2];
	input Real v1;
 algorithm
	arr[1] := v1;
arr[2] := v1;
end setArrayTest1;

function setArrayTest2
	output Real arr;
	input Real v1;
 algorithm
	arr := v1;
end setArrayTest2;



function setNoParams1
    output Integer r;
algorithm
    r := 1;
end setNoParams1;

function setNoParams2
    output Real b;
    output Real c;
algorithm
    b := 2.0;
    c := 3.0;
end setNoParams;

function maxi
	input Real x;
	input Real y;
	output Real res1;
algorithm
	res1 := x;
	if y > x then
		res1 := y;
		return;
		end if;
	return;
end maxi;

end test_functions2;