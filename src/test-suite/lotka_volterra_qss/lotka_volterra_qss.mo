model lotka_volterra_qss
  Real qss_multiplier_0_u[2];
  Real qss_multiplier_0_y[1];
  parameter Real QSSIntegrator_1_p[4](each fixed= false );
  parameter Real QSSIntegrator_1_x0=0.5;
  Real QSSIntegrator_1_u[1];
  Real QSSIntegrator_1_y[1](each start=QSSIntegrator_1_x0);
  parameter Real QSSIntegrator_2_p[4];
  parameter Real QSSIntegrator_2_x0=0.5;
  Real QSSIntegrator_2_u[1];
  Real QSSIntegrator_2_y[1](each start=QSSIntegrator_2_x0);
  parameter Real WSum_3_p[9];
  constant Integer WSum_3_n=2;
  parameter Real WSum_3_w[WSum_3_n];
  Real WSum_3_u[WSum_3_n];
  Real WSum_3_y[1];
  parameter Real WSum_4_p[9];
  constant Integer WSum_4_n=2;
  parameter Real WSum_4_w[WSum_4_n];
  Real WSum_4_u[WSum_4_n];
  Real WSum_4_y[1];
initial algorithm
    QSSIntegrator_1_p[1]:=0;
    QSSIntegrator_1_p[2]:=1e-06;
    QSSIntegrator_1_p[3]:=0.001;
    QSSIntegrator_1_p[4]:=0.5;
    QSSIntegrator_2_p[1]:=0;
    QSSIntegrator_2_p[2]:=1e-06;
    QSSIntegrator_2_p[3]:=0.001;
    QSSIntegrator_2_p[4]:=0.5;
    WSum_3_p[1]:=0.1;
    WSum_3_p[2]:=(-0.1);
    WSum_3_p[3]:=0;
    WSum_3_p[4]:=0;
    WSum_3_p[5]:=0;
    WSum_3_p[6]:=0;
    WSum_3_p[7]:=0;
    WSum_3_p[8]:=0;
    WSum_3_p[9]:=2;
    for i in 1:WSum_3_n loop
      WSum_3_w[i]:=WSum_3_p[i];
    end for;
    WSum_4_p[1]:=0.1;
    WSum_4_p[2]:=(-0.1);
    WSum_4_p[3]:=0;
    WSum_4_p[4]:=0;
    WSum_4_p[5]:=0;
    WSum_4_p[6]:=0;
    WSum_4_p[7]:=0;
    WSum_4_p[8]:=0;
    WSum_4_p[9]:=2;
    for i in 1:WSum_4_n loop
      WSum_4_w[i]:=WSum_4_p[i];
    end for;
equation
  qss_multiplier_0_u[1] = QSSIntegrator_1_y[1];
  qss_multiplier_0_u[2] = QSSIntegrator_2_y[1];
  WSum_3_u[1] = QSSIntegrator_1_y[1];
  WSum_4_u[2] = QSSIntegrator_2_y[1];
  qss_multiplier_0_y[1] = qss_multiplier_0_u[2]*qss_multiplier_0_u[1];
  WSum_3_u[2] = qss_multiplier_0_y[1];
  WSum_4_u[1] = qss_multiplier_0_y[1];
  WSum_4_y[1] = WSum_4_u*WSum_4_w;
  WSum_3_y[1] = WSum_3_u*WSum_3_w;
  QSSIntegrator_2_u[1] = WSum_4_y[1];
  QSSIntegrator_1_u[1] = WSum_3_y[1];
  der(QSSIntegrator_2_y[1]) = QSSIntegrator_2_u[1];
  der(QSSIntegrator_1_y[1]) = QSSIntegrator_1_u[1];
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=QSS3,
		MMO_SymDiff=false,
		MMO_Output={QSSIntegrator_1_y[1],QSSIntegrator_2_y[1]},
		StartTime=0.0,
		StopTime=500,
		Tolerance={1e-3},
		AbsTolerance={1e-3}
	));
end lotka_volterra_qss;
