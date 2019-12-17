model lc_line
	constant Integer N = 200;
	Real x[N];
	discrete Real d (start= 1);

	equation


	for i in 2:N-1 loop
		der(x[i]) = x[i-1] - x[i+1];
	end for;
	der(x[1]) = d - x[2];
	der(x[N]) = x[N-1];
	algorithm
	when time > 1 then
		d := 0;
	elsewhen time <= 1 then
		d := 1;
	end when;

	annotation(
	experiment(
		MMO_Description="LC transmission line",
		MMO_Solver=QSS3,
		MMO_Output={x[1],x[N],x[101],x[100]},
		StartTime=0,
		StopTime=200,
		Tolerance={ 1e-3},
		AbsTolerance={ 1e-6}
	));
end lc_line;
