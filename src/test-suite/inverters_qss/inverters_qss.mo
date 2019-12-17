model inverters_qss
  parameter Real IndexShift_1_p[2];
  constant Integer IndexShift_1_Shift=1;
  constant Integer IndexShift_1_N=100;
  Real IndexShift_1_u_1[IndexShift_1_N];
  Real IndexShift_1_y_1[IndexShift_1_N];
  parameter Real VecInt_2_p[5];
  constant Integer VecInt_2_N=100;
  parameter Real VecInt_2_x0=5;
  Real VecInt_2_u_1[100];
  Real VecInt_2_y_1[100];
  parameter Real VectorSum_3_p[10];
  constant Integer VectorSum_3_N=100;
  constant Integer VectorSum_3_nin=4;
  parameter Real VectorSum_3_w[4];
  Real VectorSum_3_u_1[100];
  Real VectorSum_3_u_2[100];
  Real VectorSum_3_u_3[100];
  Real VectorSum_3_u_4[100];
  Real VectorSum_3_y_1[100];
  parameter Real vector_sat_4_p[3];
  constant Integer vector_sat_4_N=100;
  parameter Real vector_sat_4_xl(fixed= true )=0;
  constant Real vector_sat_4_xu(fixed= true )=1e+06;
  Real vector_sat_4_u_1[vector_sat_4_N];
  Real vector_sat_4_y_1[vector_sat_4_N];
  discrete Real vector_sat_4_under[vector_sat_4_N];
  discrete Real vector_sat_4_above[vector_sat_4_N];
  parameter Real Scalar2Vector_5_p[2];
  constant Integer Scalar2Vector_5_N=100;
  constant Integer Scalar2Vector_5_Index=0;
  Real Scalar2Vector_5_u[1];
  Real Scalar2Vector_5_y_1[Scalar2Vector_5_N];
  parameter Real Vec2Scalar_6_p[2];
  parameter Integer Vec2Scalar_6_Index=1;
  constant Integer Vec2Scalar_6_N=100;
  Real Vec2Scalar_6_u_1[100];
  Real Vec2Scalar_6_y[1];
  parameter Real vector_pow2_7_p[1];
  constant Integer vector_pow2_7_N=100;
  Real vector_pow2_7_u_1[vector_pow2_7_N];
  Real vector_pow2_7_y_1[vector_pow2_7_N];
  parameter Real vector_pow2_8_p[1];
  constant Integer vector_pow2_8_N=100;
  Real vector_pow2_8_u_1[vector_pow2_8_N];
  Real vector_pow2_8_y_1[vector_pow2_8_N];
  parameter Real vector_sat_9_p[3];
  constant Integer vector_sat_9_N=100;
  parameter Real vector_sat_9_xl(fixed= true )=0;
  constant Real vector_sat_9_xu(fixed= true )=1e+06;
  Real vector_sat_9_u_1[vector_sat_9_N];
  Real vector_sat_9_y_1[vector_sat_9_N];
  discrete Real vector_sat_9_under[vector_sat_9_N];
  discrete Real vector_sat_9_above[vector_sat_9_N];
  parameter Real VectorSum_10_p[10];
  constant Integer VectorSum_10_N=100;
  constant Integer VectorSum_10_nin=3;
  parameter Real VectorSum_10_w[3];
  Real VectorSum_10_u_1[100];
  Real VectorSum_10_u_2[100];
  Real VectorSum_10_u_3[100];
  Real VectorSum_10_y_1[100];
  parameter Real VectorSum_11_p[10];
  constant Integer VectorSum_11_N=100;
  constant Integer VectorSum_11_nin=2;
  parameter Real VectorSum_11_w[2];
  Real VectorSum_11_u_1[100];
  Real VectorSum_11_u_2[100];
  Real VectorSum_11_y_1[100];
  parameter Real Scalar2Vector_12_p[2];
  constant Integer Scalar2Vector_12_N=100;
  constant Integer Scalar2Vector_12_Index=(((-1)));
  Real Scalar2Vector_12_u[1];
  Real Scalar2Vector_12_y_1[Scalar2Vector_12_N];
  parameter Real ramp_sci_13_p[5];
  parameter Real ramp_sci_13_t0=5;
  parameter Real ramp_sci_13_tr=5;
  parameter Real ramp_sci_13_v=5;
  discrete Real ramp_sci_13_s;
  discrete Real ramp_sci_13_e;
  Real ramp_sci_13_y[1];
  parameter Real ramp_sci_14_p[5];
  parameter Real ramp_sci_14_t0=15;
  parameter Real ramp_sci_14_tr=2;
  parameter Real ramp_sci_14_v=5;
  discrete Real ramp_sci_14_s;
  discrete Real ramp_sci_14_e;
  Real ramp_sci_14_y[1];
  parameter Real Constant_15_p[1];
  parameter Real Constant_15_k=1;
  Real Constant_15_y[1];
  parameter Real Constant_16_p[1];
  parameter Real Constant_16_k=5;
  Real Constant_16_y[1];
  parameter Real Scalar2Vector_17_p[2];
  constant Integer Scalar2Vector_17_N=100;
  constant Integer Scalar2Vector_17_Index=(((-1)));
  Real Scalar2Vector_17_u[1];
  Real Scalar2Vector_17_y_1[Scalar2Vector_17_N];
  parameter Real Vec2Scalar_18_p[2];
  parameter Integer Vec2Scalar_18_Index=100;
  constant Integer Vec2Scalar_18_N=100;
  Real Vec2Scalar_18_u_1[100];
  Real Vec2Scalar_18_y[1];
  parameter Real WSum_20_p[9];
  constant Integer WSum_20_n=2;
  parameter Real WSum_20_w[WSum_20_n];
  Real WSum_20_u[WSum_20_n];
  Real WSum_20_y[1];
initial algorithm
    for VecInt_2_i in 1:100 loop
      VecInt_2_y_1[VecInt_2_i]:=5;
    end for;
    for vector_sat_4_i in 1:vector_sat_4_N loop
          vector_sat_4_under[vector_sat_4_i]:=0;
          vector_sat_4_above[vector_sat_4_i]:=0;
    end for;
    for vector_sat_9_i in 1:vector_sat_9_N loop
          vector_sat_9_under[vector_sat_9_i]:=1;
          vector_sat_9_above[vector_sat_9_i]:=0;
    end for;
    ramp_sci_13_e:=0;
    ramp_sci_13_s:=0;
    ramp_sci_14_e:=0;
    ramp_sci_14_s:=0;
    IndexShift_1_p[1]:=1;
    IndexShift_1_p[2]:=100;
    VecInt_2_p[1]:=0;
    VecInt_2_p[2]:=0.0001;
    VecInt_2_p[3]:=0.01;
    VecInt_2_p[4]:=5;
    VecInt_2_p[5]:=100;
    VectorSum_3_p[1]:=1;
    VectorSum_3_p[2]:=(((-100)));
    VectorSum_3_p[3]:=100;
    VectorSum_3_p[4]:=(((-1)));
    VectorSum_3_p[5]:=0;
    VectorSum_3_p[6]:=0;
    VectorSum_3_p[7]:=0;
    VectorSum_3_p[8]:=0;
    VectorSum_3_p[9]:=4;
    VectorSum_3_p[10]:=100;
    for i in 1:4 loop
      VectorSum_3_w[i]:=VectorSum_3_p[i];
    end for;
    vector_sat_4_p[1]:=0;
    vector_sat_4_p[2]:=1e+06;
    vector_sat_4_p[3]:=100;
    Scalar2Vector_5_p[1]:=0;
    Scalar2Vector_5_p[2]:=100;
    Vec2Scalar_6_p[1]:=0;
    Vec2Scalar_6_p[2]:=100;
    vector_pow2_7_p[1]:=100;
    vector_pow2_8_p[1]:=100;
    vector_sat_9_p[1]:=0;
    vector_sat_9_p[2]:=1e+06;
    vector_sat_9_p[3]:=100;
    VectorSum_10_p[1]:=1;
    VectorSum_10_p[2]:=(((-1)));
    VectorSum_10_p[3]:=1;
    VectorSum_10_p[4]:=0;
    VectorSum_10_p[5]:=0;
    VectorSum_10_p[6]:=0;
    VectorSum_10_p[7]:=0;
    VectorSum_10_p[8]:=0;
    VectorSum_10_p[9]:=3;
    VectorSum_10_p[10]:=100;
    for i in 1:3 loop
      VectorSum_10_w[i]:=VectorSum_10_p[i];
    end for;
    VectorSum_11_p[1]:=1;
    VectorSum_11_p[2]:=(((-1)));
    VectorSum_11_p[3]:=0;
    VectorSum_11_p[4]:=0;
    VectorSum_11_p[5]:=0;
    VectorSum_11_p[6]:=0;
    VectorSum_11_p[7]:=0;
    VectorSum_11_p[8]:=0;
    VectorSum_11_p[9]:=2;
    VectorSum_11_p[10]:=100;
    for i in 1:2 loop
      VectorSum_11_w[i]:=VectorSum_11_p[i];
    end for;
    Scalar2Vector_12_p[1]:=(((-1)));
    Scalar2Vector_12_p[2]:=100;
    ramp_sci_13_p[1]:=5;
    ramp_sci_13_p[2]:=5;
    ramp_sci_13_p[3]:=5;
    ramp_sci_13_p[4]:=0;
    ramp_sci_13_p[5]:=0.01;
    ramp_sci_14_p[1]:=15;
    ramp_sci_14_p[2]:=2;
    ramp_sci_14_p[3]:=5;
    ramp_sci_14_p[4]:=0;
    ramp_sci_14_p[5]:=0.01;
    Constant_15_p[1]:=1;
    Constant_16_p[1]:=5;
    Scalar2Vector_17_p[1]:=(((-1)));
    Scalar2Vector_17_p[2]:=100;
    Vec2Scalar_18_p[1]:=99;
    Vec2Scalar_18_p[2]:=100;
    WSum_20_p[1]:=1;
    WSum_20_p[2]:=(((-1)));
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
equation
  for IndexShift_1_i in 1:IndexShift_1_Shift loop
    IndexShift_1_y_1[IndexShift_1_i] = 0;
  end for;
  for Scalar2Vector_5_i in Scalar2Vector_5_Index+2:Scalar2Vector_5_N loop
    Scalar2Vector_5_y_1[Scalar2Vector_5_i] = 0;
  end for;
  ramp_sci_13_y[1] = ((-ramp_sci_13_e*time*ramp_sci_13_s))+5*ramp_sci_13_e+5*ramp_sci_13_e*ramp_sci_13_s+time*ramp_sci_13_s-5*ramp_sci_13_s;
  ramp_sci_14_y[1] = ((-75/2*ramp_sci_14_s))+5*ramp_sci_14_e+75/2*ramp_sci_14_s*ramp_sci_14_e-5/2*ramp_sci_14_s*ramp_sci_14_e*time+5/2*ramp_sci_14_s*time;
  Constant_15_y[1] = 1;
  Constant_16_y[1] = 5;
  for i in 1:100 loop
    IndexShift_1_u_1[i] = VecInt_2_y_1[i];
  end for;
  for i in 1:100 loop
    VectorSum_3_u_4[i] = VecInt_2_y_1[i];
  end for;
  for i in 1:100 loop
    Vec2Scalar_6_u_1[i] = VecInt_2_y_1[i];
  end for;
  for i in 1:100 loop
    VectorSum_11_u_2[i] = VecInt_2_y_1[i];
  end for;
  Scalar2Vector_12_u[1] = Constant_15_y[1];
  Scalar2Vector_17_u[1] = Constant_16_y[1];
  for i in 1:100 loop
    Vec2Scalar_18_u_1[i] = VecInt_2_y_1[i];
  end for;
  WSum_20_u[1] = ramp_sci_13_y[1];
  WSum_20_u[2] = ramp_sci_14_y[1];
  for Scalar2Vector_12_i in 1:Scalar2Vector_12_N loop
    Scalar2Vector_12_y_1[Scalar2Vector_12_i] = Scalar2Vector_12_u[1];
  end for;
  for Scalar2Vector_17_i in 1:Scalar2Vector_17_N loop
    Scalar2Vector_17_y_1[Scalar2Vector_17_i] = Scalar2Vector_17_u[1];
  end for;
  for i in 1:Scalar2Vector_12_N loop
    VectorSum_10_u_2[i] = Scalar2Vector_12_y_1[i];
  end for;
  for i in 1:Scalar2Vector_17_N loop
    VectorSum_3_u_1[i] = Scalar2Vector_17_y_1[i];
  end for;
  WSum_20_y[1] = WSum_20_w*WSum_20_u;
  Scalar2Vector_5_u[1] = WSum_20_y[1];
  Scalar2Vector_5_y_1[1] = Scalar2Vector_5_u[1];
  for IndexShift_1_i in 1:IndexShift_1_N-IndexShift_1_Shift loop
    IndexShift_1_y_1[IndexShift_1_i+IndexShift_1_Shift] = IndexShift_1_u_1[IndexShift_1_i];
  end for;
  for i in 1:IndexShift_1_N loop
    VectorSum_10_u_3[i] = IndexShift_1_y_1[i];
  end for;
  for i in 1:Scalar2Vector_5_N loop
    VectorSum_10_u_1[i] = Scalar2Vector_5_y_1[i];
  end for;
  for VectorSum_10_i in 1:100 loop
    VectorSum_10_y_1[VectorSum_10_i] = VectorSum_10_u_1[VectorSum_10_i]*VectorSum_10_w[1]+VectorSum_10_u_2[VectorSum_10_i]*VectorSum_10_w[2]+VectorSum_10_u_3[VectorSum_10_i]*VectorSum_10_w[3];
  end for;
  for i in 1:100 loop
    VectorSum_11_u_1[i] = VectorSum_10_y_1[i];
  end for;
  for i in 1:100 loop
    vector_sat_4_u_1[i] = VectorSum_10_y_1[i];
  end for;
  for VectorSum_11_i in 1:100 loop
    VectorSum_11_y_1[VectorSum_11_i] = VectorSum_11_u_1[VectorSum_11_i]*VectorSum_11_w[1]+VectorSum_11_u_2[VectorSum_11_i]*VectorSum_11_w[2];
  end for;
  for i in 1:100 loop
    vector_sat_9_u_1[i] = VectorSum_11_y_1[i];
  end for;
  for vector_sat_4_i in 1:vector_sat_4_N loop
    vector_sat_4_y_1[vector_sat_4_i] = pre(vector_sat_4_under[vector_sat_4_i])*vector_sat_4_xl+(1-pre(vector_sat_4_under[vector_sat_4_i]))*(pre(vector_sat_4_above[vector_sat_4_i])*vector_sat_4_xu+(1-pre(vector_sat_4_above[vector_sat_4_i]))*vector_sat_4_u_1[vector_sat_4_i]);
  end for;
  for vector_sat_9_i in 1:vector_sat_9_N loop
    vector_sat_9_y_1[vector_sat_9_i] = pre(vector_sat_9_under[vector_sat_9_i])*vector_sat_9_xl+(1-pre(vector_sat_9_under[vector_sat_9_i]))*(pre(vector_sat_9_above[vector_sat_9_i])*vector_sat_9_xu+(1-pre(vector_sat_9_above[vector_sat_9_i]))*vector_sat_9_u_1[vector_sat_9_i]);
  end for;
  for i in 1:vector_sat_9_N loop
    vector_pow2_8_u_1[i] = vector_sat_9_y_1[i];
  end for;
  for i in 1:vector_sat_4_N loop
    vector_pow2_7_u_1[i] = vector_sat_4_y_1[i];
  end for;
  for vector_pow2_8_i in 1:vector_pow2_8_N loop
    vector_pow2_8_y_1[vector_pow2_8_i] = vector_pow2_8_u_1[vector_pow2_8_i]*vector_pow2_8_u_1[vector_pow2_8_i];
  end for;
  for vector_pow2_7_i in 1:vector_pow2_7_N loop
    vector_pow2_7_y_1[vector_pow2_7_i] = vector_pow2_7_u_1[vector_pow2_7_i]*vector_pow2_7_u_1[vector_pow2_7_i];
  end for;
  for i in 1:vector_pow2_8_N loop
    VectorSum_3_u_3[i] = vector_pow2_8_y_1[i];
  end for;
  for i in 1:vector_pow2_7_N loop
    VectorSum_3_u_2[i] = vector_pow2_7_y_1[i];
  end for;
  for VectorSum_3_i in 1:100 loop
    VectorSum_3_y_1[VectorSum_3_i] = VectorSum_3_u_1[VectorSum_3_i]*VectorSum_3_w[1]+VectorSum_3_u_2[VectorSum_3_i]*VectorSum_3_w[2]+VectorSum_3_u_3[VectorSum_3_i]*VectorSum_3_w[3]+VectorSum_3_u_4[VectorSum_3_i]*VectorSum_3_w[4];
  end for;
  for i in 1:100 loop
    VecInt_2_u_1[i] = VectorSum_3_y_1[i];
  end for;
  Vec2Scalar_18_y[1] = Vec2Scalar_18_u_1[100];
  Vec2Scalar_6_y[1] = Vec2Scalar_6_u_1[1];
  for VecInt_2_i in 1:100 loop
    der(VecInt_2_y_1[VecInt_2_i]) = VecInt_2_u_1[VecInt_2_i];
  end for;
algorithm
    for vector_sat_4_i in 1:vector_sat_4_N loop
      when vector_sat_4_u_1[vector_sat_4_i]<vector_sat_4_xl then 
        vector_sat_4_under[vector_sat_4_i]:=1;
      end when;
      when vector_sat_4_u_1[vector_sat_4_i]>=vector_sat_4_xl then 
        vector_sat_4_under[vector_sat_4_i]:=0;
      end when;
      when vector_sat_4_u_1[vector_sat_4_i]>vector_sat_4_xu then 
        vector_sat_4_above[vector_sat_4_i]:=1;
      end when;
      when vector_sat_4_u_1[vector_sat_4_i]<=vector_sat_4_xu then 
        vector_sat_4_above[vector_sat_4_i]:=0;
      end when;
    end for;
    for vector_sat_9_i in 1:vector_sat_9_N loop
      when vector_sat_9_u_1[vector_sat_9_i]<vector_sat_9_xl then 
        vector_sat_9_under[vector_sat_9_i]:=1;
      end when;
      when vector_sat_9_u_1[vector_sat_9_i]>=vector_sat_9_xl then 
        vector_sat_9_under[vector_sat_9_i]:=0;
      end when;
      when vector_sat_9_u_1[vector_sat_9_i]>vector_sat_9_xu then 
        vector_sat_9_above[vector_sat_9_i]:=1;
      end when;
      when vector_sat_9_u_1[vector_sat_9_i]<=vector_sat_9_xu then 
        vector_sat_9_above[vector_sat_9_i]:=0;
      end when;
    end for;
    when time>5 then 
      ramp_sci_13_s:=1;
    end when;
    when time>10 then 
      ramp_sci_13_e:=1;
    end when;
    when time>15 then 
      ramp_sci_14_s:=1;
    end when;
    when time>17 then 
      ramp_sci_14_e:=1;
    end when;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=LIQSS2,
		MMO_SymDiff=false,
		MMO_PartitionMethod=Metis,
		MMO_Output={VecInt_2_y_1[1],VecInt_2_y_1[100]},
		Jacobian=Dense,
		StartTime=0.0,
		StopTime=100,
		Tolerance={1e-3},
		AbsTolerance={1e-3}
	));
end inverters_qss;
