model bball_downstairs
	Real x(start=0.575),vx(start=0.5),y(start=10.5),vy;
	discrete Real stair(start=10),contact;
	equation
		der(y) = vy;
		der(vy) = -9.8 - 0.1 * vy - contact * ((y - stair) *1e6+ vy * 30);
		der(x) = vx;
		der(vx) = -0.1 * vx;
	algorithm
	when y < stair then
		contact := 1;
	elsewhen y > stair then
		contact := 0;
	end when;
	when x - 11 +stair > 0 then
		stair := stair - 1;
	end when;
	annotation(

	experiment(
		MMO_Description="Ball bouncing downstairs.",
		MMO_Solver=QSS3,
		MMO_PartitionMethod=Metis,
		MMO_Output={y},
		Jacobian=Dense,
		StartTime=0,
		StopTime=30,
		Tolerance={ 1e-6},
		AbsTolerance={ 1e-9}
	));
end bball_downstairs;
