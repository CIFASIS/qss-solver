model func_test

import test_functions;
import test_functions2;

//constant Real PI = 3.1415926;

constant Integer N = 2;
	
Real x[N], y[N];
Real ux, uy;

Real arr1[2], arr2[2];

Real array[2];

discrete Real times1[2];
discrete Real times2[2];
discrete Real ax, ay, _, aux;

initial algorithm
	ax := setNoParams1();
	(ax, ay) := setNoParams2();
	(ax, ay) := set1(1.0);
	//ax := PI;
    (ux, 	uy) := set2(3.0);
//(times1) := setArrayTest1(0.1);

    for i in 1:N loop
        (x[i], y[i]) := set3(5.0);
        times1[i] := 0.1;
        times2[i] := 1;
        array[i] := 0.0;
    end for;
    //noOutputFunction(1.0);
   _ := setArray(0.0, arr1);
    _ := setArray(2.0, arr2);

 

equation
    for i in 1:N loop
        der(x[i]) = 0.0;
        der(y[i]) = 0.0;
		der(arr1[i]) = 0;
		der(arr2[i]) = 0;
		der(array[i]) = 0;
    end for;
	der(ux) = 0.0;
	der(uy) = 0.0;
	
algorithm
	for i in 1:2 loop
		when time > times1[i] then
			reinit(array[i], 4.0);
			aux := time + 0.2;
			times2[i] := setArrayTest2(aux);
for j in 1:N loop
		times1[j] := 10;
end for;			
//times2[1] := aux;
			//times2[i] := aux;
		end when;
		when time > times2[i] then
			reinit(array[i], 2.0);
		end when;
	end for;
	
	annotation(

	experiment(
		MMO_Description="Test",
		MMO_Solver=QSS2,
		MMO_SymDiff=false,
		MMO_Period={0.2},
		MMO_PartitionMethod=Metis,
		MMO_Output={arr1},
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		StartTime=0.0,
		StopTime=0.4,
		Tolerance={1e-5},
		AbsTolerance={1e-8}
	));
end func_test;
