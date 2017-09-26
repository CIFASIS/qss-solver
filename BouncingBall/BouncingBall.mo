model BouncingBall
  Real spring1_flange1_f;
  Real spring1_flange1_s;
  Real spring1_flange1_v;
  Real spring1_flange2_f;
  Real spring1_flange2_s;
  Real spring1_flange2_v;
  Real spring1_dy;
  Real spring1_dv;
  parameter Real spring1_b=10;
  parameter Real spring1_k=10000;
  Real fixed1_flange1_f;
  Real fixed1_flange1_s;
  Real fixed1_flange1_v;
  parameter Real fixed1_s0=0;
  Real ball1_flange1_f;
  Real ball1_flange1_s;
  Real ball1_flange1_v;
  Real ball1_v;
  parameter Real ball1_m=1;
  parameter Real ball1_g=9.8;
  Real ball1_y(start=10);
  discrete Real d0;
  discrete Real d1;
equation
  fixed1_flange1_s = fixed1_s0;
  fixed1_flange1_v = 0;
  der(ball1_y) = ball1_v;
  ball1_flange1_v = ball1_v;
  ball1_flange1_s = ball1_y;
  spring1_flange2_s = fixed1_flange1_s;
  spring1_flange2_v = fixed1_flange1_v;
  spring1_flange1_s = ball1_flange1_s;
  spring1_flange1_v = ball1_flange1_v;
  spring1_dv = spring1_flange1_v-spring1_flange2_v;
  spring1_dy = spring1_flange1_s-spring1_flange2_s;
  spring1_flange1_f = d0*(spring1_b*spring1_dv+spring1_k*spring1_dy)+(1-d0)*(0);
  spring1_flange2_f = (((-spring1_flange1_f)));
  ball1_flange1_f = (-spring1_flange1_f);
  der(ball1_v) = (ball1_flange1_f-ball1_m*ball1_g)*(1/(ball1_m));
  fixed1_flange1_f = (-spring1_flange2_f);
algorithm
    when spring1_dy<0 then 
      d0:=1;
    elsewhen spring1_dy>=0 then 
      d0:=0;
    end when;
    when d0>0 then 
      d1:=1;
    elsewhen d0<1 then 
      d1:=0;
    end when;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=QSS3,
		MMO_PartitionMethod=Metis,
		MMO_Output={ball1_y},
		MMO_OutputType=CI_Sampled,
		Jacobian=Dense,
		StartTime=0.0,
		StopTime=10,
		Tolerance={1e-3},
		AbsTolerance={1e-6}
	));
end BouncingBall;
