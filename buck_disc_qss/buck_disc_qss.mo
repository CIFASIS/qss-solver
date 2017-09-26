model buck_disc_qss
  parameter Real QSSIntegrator_1_p[4](each fixed= false );
  parameter Real QSSIntegrator_1_x0=0;
  Real QSSIntegrator_1_u[1];
  Real QSSIntegrator_1_y[1]( start = 0);
  parameter Real QSSIntegrator_2_p[4](each fixed= false );
  parameter Real QSSIntegrator_2_x0=0;
  Real QSSIntegrator_2_u[1];
  Real QSSIntegrator_2_y[1](start=0);
  parameter Real WSum_3_p[9](each fixed= false );
  constant Integer WSum_3_n=2;
  parameter Real WSum_3_w[WSum_3_n](each fixed= false );
  Real WSum_3_u[WSum_3_n];
  Real WSum_3_y[1];
  Real inverse_function_4_u[1];
  Real inverse_function_4_y[1];
  Real qss_multiplier_5_u[2];
  Real qss_multiplier_5_y[1];
  Real qss_multiplier_6_u[2];
  Real qss_multiplier_6_y[1];
  Real qss_multiplier_7_u[2];
  Real qss_multiplier_7_y[1];
  Real inverse_function_8_u[1];
  Real inverse_function_8_y[1];
  Real qss_multiplier_9_u[2];
  Real qss_multiplier_9_y[1];
  Real qss_multiplier_10_u[2];
  Real qss_multiplier_10_y[1];
  parameter Real WSum_11_p[9](each fixed= false );
  constant Integer WSum_11_n=2;
  parameter Real WSum_11_w[WSum_11_n](each fixed= false );
  Real WSum_11_u[WSum_11_n];
  Real WSum_11_y[1];
  parameter Real qss_switch_12_p[1](each fixed= false );
  parameter Real qss_switch_12_level=0.5;
  Real qss_switch_12_u[3];
  Real qss_switch_12_y[1];
  discrete Real qss_switch_12_d;
  parameter Real qss_switch_13_p[1](each fixed= false );
  parameter Real qss_switch_13_level=0;
  Real qss_switch_13_u[3];
  Real qss_switch_13_y[1];
  discrete Real qss_switch_13_d;
  parameter Real Constant_14_p[1](each fixed= false );
  parameter Real Constant_14_k=1e-05;
  Real Constant_14_y[1];
  parameter Real Constant_15_p[1](each fixed= false );
  parameter Real Constant_15_k=100000;
  Real Constant_15_y[1];
  parameter Real square_sci_16_p[3](each fixed= false );
  parameter Real square_sci_16_amplitude=1;
  parameter Real square_sci_16_freq=10000;
  parameter Real square_sci_16_DC=50/100;
  discrete Real square_sci_16_lev(start=1);
  discrete Real square_sci_16_next(start=0);
  Real square_sci_16_y[1];
  parameter Real hysteretic_18_p[4](each fixed= false );
  parameter Real hysteretic_18_xl=((((-1e-06))));
  parameter Real hysteretic_18_xu=1e-06;
  parameter Real hysteretic_18_yl=((((-1))));
  parameter Real hysteretic_18_yu=1;
  Real hysteretic_18_u[1];
  Real hysteretic_18_y[1];
  discrete Real hysteretic_18_state;
  parameter Real qss_switch_19_p[1](each fixed= false );
  parameter Real qss_switch_19_level=0;
  Real qss_switch_19_u[3];
  Real qss_switch_19_y[1];
  discrete Real qss_switch_19_d;
  parameter Real WSum_20_p[9](each fixed= false );
  constant Integer WSum_20_n=2;
  parameter Real WSum_20_w[WSum_20_n](each fixed= false );
  Real WSum_20_u[WSum_20_n];
  Real WSum_20_y[1];
  parameter Real WSum_21_p[9](each fixed= false );
  constant Integer WSum_21_n=2;
  parameter Real WSum_21_w[WSum_21_n](each fixed= false );
  Real WSum_21_u[WSum_21_n];
  Real WSum_21_y[1];
initial algorithm
    if qss_switch_12_u[2]>qss_switch_12_level then 
      qss_switch_12_d:=1;
    elseif qss_switch_12_u[2]<qss_switch_12_level then 
      qss_switch_12_d:=0;
    end if;
    if qss_switch_13_u[2]>0 then 
      qss_switch_13_d:=1;
    elseif qss_switch_13_u[2]<0 then 
      qss_switch_13_d:=0;
    end if;
    square_sci_16_next:=square_sci_16_DC/10000;
    if hysteretic_18_u[1]>hysteretic_18_xu then 
      hysteretic_18_state:=1;
    end if;
    if hysteretic_18_u[1]<hysteretic_18_xl then 
      hysteretic_18_state:=hysteretic_18_yl;
    end if;
    if qss_switch_19_u[2]>0 then 
      qss_switch_19_d:=1;
    elseif qss_switch_19_u[2]<0 then 
      qss_switch_19_d:=0;
    end if;
    QSSIntegrator_1_p[1]:=0;
    QSSIntegrator_1_p[2]:=0.1;
    QSSIntegrator_1_p[3]:=0.001;
    QSSIntegrator_1_p[4]:=0;
    QSSIntegrator_2_p[1]:=0;
    QSSIntegrator_2_p[2]:=0.001;
    QSSIntegrator_2_p[3]:=0.001;
    QSSIntegrator_2_p[4]:=0;
    WSum_3_p[1]:=1;
    WSum_3_p[2]:=1;
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
    WSum_11_p[1]:=24;
    WSum_11_p[2]:=((((-1))));
    WSum_11_p[3]:=0;
    WSum_11_p[4]:=0;
    WSum_11_p[5]:=0;
    WSum_11_p[6]:=0;
    WSum_11_p[7]:=0;
    WSum_11_p[8]:=0;
    WSum_11_p[9]:=2;
    for i in 1:WSum_11_n loop
      WSum_11_w[i]:=WSum_11_p[i];
    end for;
    qss_switch_12_p[1]:=0.5;
    qss_switch_13_p[1]:=0;
    Constant_14_p[1]:=1e-05;
    Constant_15_p[1]:=100000;
    square_sci_16_p[1]:=1;
    square_sci_16_p[2]:=10000;
    square_sci_16_p[3]:=50;
    hysteretic_18_p[1]:=((((-1e-06))));
    hysteretic_18_p[2]:=1e-06;
    hysteretic_18_p[3]:=((((-1))));
    hysteretic_18_p[4]:=1;
    qss_switch_19_p[1]:=0;
    WSum_20_p[1]:=((((-1000))));
    WSum_20_p[2]:=10000;
    WSum_20_p[3]:=0;
    WSum_20_p[4]:=0;
    WSum_20_p[5]:=0;
    WSum_20_p[6]:=0;
    WSum_20_p[7]:=0;
    WSum_20_p[8]:=0;
    WSum_20_p[9]:=2;
    for i in 1:WSum_20_n loop
      WSum_20_w[i]:=WSum_20_p[i];
    end for;
    WSum_21_p[1]:=10000;
    WSum_21_p[2]:=((((-10000))));
    WSum_21_p[3]:=0;
    WSum_21_p[4]:=0;
    WSum_21_p[5]:=0;
    WSum_21_p[6]:=0;
    WSum_21_p[7]:=0;
    WSum_21_p[8]:=0;
    WSum_21_p[9]:=2;
    for i in 1:WSum_21_n loop
      WSum_21_w[i]:=WSum_21_p[i];
    end for;
qss_switch_19_d := 1;
hysteretic_18_state := 1;
square_sci_16_lev := 1;
qss_switch_13_d := 1;
qss_switch_12_d :=1;

equation
  Constant_14_y[1] = Constant_14_k;
  Constant_15_y[1] = 100000;
  square_sci_16_y[1] = pre(square_sci_16_lev)*1;
  hysteretic_18_y[1] = pre(hysteretic_18_state);
  qss_switch_12_u[2] = square_sci_16_y[1];
  WSum_20_u[1] = QSSIntegrator_2_y[1];
  WSum_21_u[2] = QSSIntegrator_2_y[1];
  qss_switch_19_u[2] = hysteretic_18_y[1];
  qss_switch_13_u[2] = hysteretic_18_y[1];
  qss_multiplier_7_u[2] = QSSIntegrator_1_y[1];
  WSum_20_u[2] = QSSIntegrator_1_y[1];
  qss_switch_13_u[3] = Constant_14_y[1];
  qss_switch_12_u[1] = Constant_14_y[1];
  qss_switch_13_u[1] = Constant_15_y[1];
  qss_switch_12_u[3] = Constant_15_y[1];
  qss_switch_12_y[1] = qss_switch_12_u[1]*pre(qss_switch_12_d)+qss_switch_12_u[3]*(1-pre(qss_switch_12_d));
  qss_switch_13_y[1] = qss_switch_13_u[1]*pre(qss_switch_13_d)+qss_switch_13_u[3]*(1-pre(qss_switch_13_d));
  qss_multiplier_5_u[2] = qss_switch_12_y[1];
  qss_multiplier_6_u[2] = qss_switch_13_y[1];
  inverse_function_8_u[1] = qss_switch_13_y[1];
  qss_multiplier_10_u[1] = qss_switch_13_y[1];
  inverse_function_8_y[1] = 1/inverse_function_8_u[1];
  qss_multiplier_9_u[1] = inverse_function_8_y[1];
  WSum_3_u[2] = qss_switch_12_y[1];
  WSum_3_u[1] = qss_switch_13_y[1];
  WSum_3_y[1] = WSum_3_u[1]*WSum_3_w[1]+WSum_3_u[2]*WSum_3_w[2];
  WSum_20_y[1] = WSum_20_u[1]*WSum_20_w[1]+WSum_20_u[2]*WSum_20_w[2];
  QSSIntegrator_2_u[1] = WSum_20_y[1];
  inverse_function_4_u[1] = WSum_3_y[1];
  inverse_function_4_y[1] = 1/inverse_function_4_u[1];
  qss_multiplier_5_u[1] = inverse_function_4_y[1];
  qss_multiplier_10_u[2] = inverse_function_4_y[1];
  qss_multiplier_5_y[1] = qss_multiplier_5_u[1]*qss_multiplier_5_u[2];
  qss_multiplier_6_u[1] = qss_multiplier_5_y[1];
  qss_multiplier_6_y[1] = qss_multiplier_6_u[1]*qss_multiplier_6_u[2];
  qss_multiplier_7_u[1] = qss_multiplier_6_y[1];
  qss_multiplier_7_y[1] = qss_multiplier_7_u[1]*qss_multiplier_7_u[2];
  qss_multiplier_10_y[1] = qss_multiplier_10_u[1]*qss_multiplier_10_u[2];
  WSum_11_u[2] = qss_multiplier_7_y[1];
  WSum_11_u[1] = qss_multiplier_10_y[1];
  WSum_11_y[1] = WSum_11_u[1]*WSum_11_w[1]+WSum_11_u[2]*WSum_11_w[2];
  WSum_21_u[1] = WSum_11_y[1];
  qss_multiplier_9_u[2] = WSum_11_y[1];
  qss_multiplier_9_y[1] = qss_multiplier_9_u[1]*qss_multiplier_9_u[2];
  qss_switch_19_u[3] = qss_multiplier_9_y[1];
  qss_switch_19_u[1] = WSum_11_y[1];
  WSum_21_y[1] = WSum_21_u[1]*WSum_21_w[1]+WSum_21_u[2]*WSum_21_w[2];
  QSSIntegrator_1_u[1] = WSum_21_y[1];
  qss_switch_19_y[1] = qss_switch_19_u[1]*pre(qss_switch_19_d)+qss_switch_19_u[3]*(1-pre(qss_switch_19_d));
  hysteretic_18_u[1] = qss_switch_19_y[1];
  der(QSSIntegrator_2_y[1]) = QSSIntegrator_2_u[1];
  der(QSSIntegrator_1_y[1]) = QSSIntegrator_1_u[1];
algorithm
    when qss_switch_12_u[2]>qss_switch_12_level then 
      qss_switch_12_d:=1;
    elsewhen qss_switch_12_u[2]<qss_switch_12_level then 
      qss_switch_12_d:=0;
    end when;
    when qss_switch_13_u[2]>0 then 
      qss_switch_13_d:=1;
    elsewhen qss_switch_13_u[2]<0 then 
      qss_switch_13_d:=0;
    end when;
    when time>square_sci_16_next then 
      square_sci_16_lev:=1-square_sci_16_lev;
      square_sci_16_next:=time+square_sci_16_lev*square_sci_16_DC/10000+(1-square_sci_16_lev)*(1-square_sci_16_DC)/10000;
    end when;
    when hysteretic_18_u[1]>hysteretic_18_xu then 
      hysteretic_18_state:=1;
    end when;
    when hysteretic_18_u[1]<hysteretic_18_xl then 
      hysteretic_18_state:=hysteretic_18_yl;
    end when;
    when qss_switch_19_u[2]>0 then 
      qss_switch_19_d:=1;
    elsewhen qss_switch_19_u[2]<0 then 
      qss_switch_19_d:=0;
	end when;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=LIQSS3,
		MMO_PartitionMethod=Metis,
		MMO_Output={QSSIntegrator_1_y[1],QSSIntegrator_2_y[1]},
		Jacobian=Dense,
		StartTime=0.0,
		StopTime=0.01,
		Tolerance={1e-3},
		AbsTolerance={1e-3}
	));
end buck_disc_qss;
