model lcline_qss
  parameter Real VecInt_1_p[5];
  constant Integer VecInt_1_N = 50;
  parameter Real VecInt_1_x0 = 0;
  Real VecInt_1_u_1[50];
  Real VecInt_1_y_1[50];
  parameter Real VecInt_2_p[5];
  constant Integer VecInt_2_N = 50;
  parameter Real VecInt_2_x0 = 0;
  Real VecInt_2_u_1[50];
  Real VecInt_2_y_1[50];
  parameter Real VectorSum_3_p[10];
  constant Integer VectorSum_3_N = 50;
  constant Integer VectorSum_3_nin = 2;
  parameter Real VectorSum_3_w[2];
  Real VectorSum_3_u_1[50];
  Real VectorSum_3_u_2[50];
  Real VectorSum_3_y_1[50];
  parameter Real IndexShift_4_p[2];
  constant Integer IndexShift_4_Shift = 1;
  constant Integer IndexShift_4_N = 50;
  Real IndexShift_4_u_1[IndexShift_4_N];
  Real IndexShift_4_y_1[IndexShift_4_N];
  parameter Real VectorSum_5_p[10];
  constant Integer VectorSum_5_N = 50;
  constant Integer VectorSum_5_nin = 3;
  parameter Real VectorSum_5_w[3];
  Real VectorSum_5_u_1[50];
  Real VectorSum_5_u_2[50];
  Real VectorSum_5_u_3[50];
  Real VectorSum_5_y_1[50];
  parameter Real IndexShift_6_p[2];
  constant Integer IndexShift_6_Shift = -1;
  constant Integer IndexShift_6_N = 50;
  Real IndexShift_6_u_1[IndexShift_6_N];
  Real IndexShift_6_y_1[IndexShift_6_N];
  parameter Real Scalar2Vector_7_p[2];
  constant Integer Scalar2Vector_7_N = 50;
  constant Integer Scalar2Vector_7_Index = 0;
  Real Scalar2Vector_7_u[1];
  Real Scalar2Vector_7_y_1[Scalar2Vector_7_N];
  parameter Real pulse_sci_8_p[4];
  parameter Real pulse_sci_8_low = 0;
  parameter Real pulse_sci_8_amplitude = 1;
  parameter Real pulse_sci_8_ti = 0;
  parameter Real pulse_sci_8_tf = 1;
  discrete Real pulse_sci_8_d(start = 0);
  Real pulse_sci_8_y[1];
  parameter Real Vec2Scalar_10_p[2];
  parameter Integer Vec2Scalar_10_Index = 1;
  constant Integer Vec2Scalar_10_N = 50;
  Real Vec2Scalar_10_u_1[50];
  Real Vec2Scalar_10_y[1];
  parameter Real Vec2Scalar_11_p[2];
  parameter Integer Vec2Scalar_11_Index = 50;
  constant Integer Vec2Scalar_11_N = 50;
  Real Vec2Scalar_11_u_1[50];
  Real Vec2Scalar_11_y[1];
initial algorithm
  for VecInt_1_i in 1:50 loop
    VecInt_1_y_1[VecInt_1_i] := 0;
  end for;
  for VecInt_2_i in 1:50 loop
    VecInt_2_y_1[VecInt_2_i] := 0;
  end for;
  VecInt_1_p[1] := 0;
  VecInt_1_p[2] := 1e-06;
  VecInt_1_p[3] := 0.001;
  VecInt_1_p[4] := 0;
  VecInt_1_p[5] := 50;
  VecInt_2_p[1] := 0;
  VecInt_2_p[2] := 1e-06;
  VecInt_2_p[3] := 0.001;
  VecInt_2_p[4] := 0;
  VecInt_2_p[5] := 50;
  VectorSum_3_p[1] := 1;
  VectorSum_3_p[2] := -1;
  VectorSum_3_p[3] := 0;
  VectorSum_3_p[4] := 0;
  VectorSum_3_p[5] := 0;
  VectorSum_3_p[6] := 0;
  VectorSum_3_p[7] := 0;
  VectorSum_3_p[8] := 0;
  VectorSum_3_p[9] := 2;
  VectorSum_3_p[10] := 50;
  for i in 1:2 loop
    VectorSum_3_w[i] := VectorSum_3_p[i];
  end for;
  IndexShift_4_p[1] := 1;
  IndexShift_4_p[2] := 50;
  VectorSum_5_p[1] := 1;
  VectorSum_5_p[2] := 1;
  VectorSum_5_p[3] := -1;
  VectorSum_5_p[4] := 0;
  VectorSum_5_p[5] := 0;
  VectorSum_5_p[6] := 0;
  VectorSum_5_p[7] := 0;
  VectorSum_5_p[8] := 0;
  VectorSum_5_p[9] := 3;
  VectorSum_5_p[10] := 50;
  for i in 1:3 loop
    VectorSum_5_w[i] := VectorSum_5_p[i];
  end for;
  IndexShift_6_p[1] := -1;
  IndexShift_6_p[2] := 50;
  Scalar2Vector_7_p[1] := 0;
  Scalar2Vector_7_p[2] := 50;
  pulse_sci_8_p[1] := 0;
  pulse_sci_8_p[2] := 1;
  pulse_sci_8_p[3] := 0;
  pulse_sci_8_p[4] := 1;
  Vec2Scalar_10_p[1] := 0;
  Vec2Scalar_10_p[2] := 50;
  Vec2Scalar_11_p[1] := 49;
  Vec2Scalar_11_p[2] := 50;
  pulse_sci_8_d := 1;
equation
  for IndexShift_4_i in 1:IndexShift_4_Shift loop
    IndexShift_4_y_1[IndexShift_4_i] = 0;
  end for;
  for IndexShift_6_i in IndexShift_6_N + IndexShift_6_Shift + 1:IndexShift_6_N loop
    IndexShift_6_y_1[IndexShift_6_i] = 0;
  end for;
  for Scalar2Vector_7_i in Scalar2Vector_7_Index + 2:Scalar2Vector_7_N loop
    Scalar2Vector_7_y_1[Scalar2Vector_7_i] = 0;
  end for;
  pulse_sci_8_y[1] = pulse_sci_8_d;
  for i in 1:50 loop
    VectorSum_3_u_1[i] = VecInt_2_y_1[i];
  end for;
  for i in 1:50 loop
    IndexShift_4_u_1[i] = VecInt_2_y_1[i];
  end for;
  for i in 1:50 loop
    VectorSum_5_u_3[i] = VecInt_1_y_1[i];
  end for;
  for i in 1:50 loop
    IndexShift_6_u_1[i] = VecInt_1_y_1[i];
  end for;
  Scalar2Vector_7_u[1] = pulse_sci_8_y[1];
  for i in 1:50 loop
    Vec2Scalar_10_u_1[i] = VecInt_1_y_1[i];
  end for;
  for i in 1:50 loop
    Vec2Scalar_11_u_1[i] = VecInt_1_y_1[i];
  end for;
  Scalar2Vector_7_y_1[1] = Scalar2Vector_7_u[1];
  for IndexShift_6_i in 1:IndexShift_6_N + IndexShift_6_Shift loop
    IndexShift_6_y_1[IndexShift_6_i] = IndexShift_6_u_1[IndexShift_6_i - IndexShift_6_Shift];
  end for;
  for i in 1:IndexShift_6_N loop
    VectorSum_5_u_2[i] = IndexShift_6_y_1[i];
  end for;
  for i in 1:Scalar2Vector_7_N loop
    VectorSum_5_u_1[i] = Scalar2Vector_7_y_1[i];
  end for;
  for IndexShift_4_i in 1:IndexShift_4_N - IndexShift_4_Shift loop
    IndexShift_4_y_1[IndexShift_4_i + IndexShift_4_Shift] = IndexShift_4_u_1[IndexShift_4_i];
  end for;
  for i in 1:IndexShift_4_N loop
    VectorSum_3_u_2[i] = IndexShift_4_y_1[i];
  end for;
  for VectorSum_5_i in 1:50 loop
    VectorSum_5_y_1[VectorSum_5_i] = VectorSum_5_u_1[VectorSum_5_i] * VectorSum_5_w[1] + VectorSum_5_u_2[VectorSum_5_i] * VectorSum_5_w[2] + VectorSum_5_u_3[VectorSum_5_i] * VectorSum_5_w[3];
  end for;
  for VectorSum_3_i in 1:50 loop
    VectorSum_3_y_1[VectorSum_3_i] = VectorSum_3_u_1[VectorSum_3_i] * VectorSum_3_w[1] + VectorSum_3_u_2[VectorSum_3_i] * VectorSum_3_w[2];
  end for;
  for i in 1:50 loop
    VecInt_2_u_1[i] = VectorSum_5_y_1[i];
  end for;

  for i in 1:50 loop
    VecInt_1_u_1[i] = VectorSum_3_y_1[i];
  end for;
  Vec2Scalar_11_y[1] = Vec2Scalar_11_u_1[50];
  Vec2Scalar_10_y[1] = Vec2Scalar_10_u_1[1];
  for VecInt_2_i in 1:50 loop
    der(VecInt_2_y_1[VecInt_2_i]) = VecInt_2_u_1[VecInt_2_i];
  end for;
  for VecInt_1_i in 1:50 loop
    der(VecInt_1_y_1[VecInt_1_i]) = VecInt_1_u_1[VecInt_1_i];
  end for;
algorithm
  when time > 0 then
    pulse_sci_8_d := 1;
  end when;
  when time > 1 then
    pulse_sci_8_d := 0;
  end when;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=QSS3,
		MMO_SymDiff=false,
		MMO_Output={VecInt_2_y_1[1]},
		MMO_OutputType=CI_Sampled,
		StartTime=0.0,
		StopTime=100,
		Tolerance={1e-3},
		AbsTolerance={1e-6}
	));
end lcline_qss;
