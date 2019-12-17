model shallow
constant Integer N = 500;
parameter Real g = 1.0, L = 10.0, h_left = 3.0, h_right = 1.0, hu_left = 0.0, hu_right = 0.0;
Real h[N], hu[N];
Real ubarL[N], ubarR[N], cbarL[N], cbarR[N], a1L[N], a1R[N], a2L[N], a2R[N];
initial algorithm
for i in 1:250 loop
h[i] := 3.0;
end for;
for i in 251:500 loop
h[i] := 1.0;
end for;
equation
ubarL[1] = (hu[1] / sqrt(h[1]) + hu[1] / sqrt(h[1])) / (sqrt(h[1]) + sqrt(h[1]));
cbarL[1] = sqrt(g * (h[1] + h[1]) / 2);
ubarL[N] = (hu[N - 1] / sqrt(h[N - 1]) + hu[N] / sqrt(h[N])) / (sqrt(h[N - 1]) + sqrt(h[N]));
cbarL[N] = sqrt(g * (h[N - 1] + h[N]) / 2);
ubarR[1] = (hu[1] / sqrt(h[1]) + hu[2] / sqrt(h[2])) / (sqrt(h[1]) + sqrt(h[2]));
cbarR[1] = sqrt(g * (h[1] + h[2]) / 2);
ubarR[N] = (hu[N] / sqrt(h[N]) + hu[N] / sqrt(h[N])) / (sqrt(h[N]) + sqrt(h[N]));
cbarR[N] = sqrt(g * (h[N] + h[N]) / 2);
a1L[1] = 0.5 / cbarL[1] * (hu[1] - hu[1] + (ubarL[1] + cbarL[1]) * (h[1] - h[1])); //0
a2L[1] = 0.5 / cbarL[1] * (hu[1] - hu[1] - (ubarL[1] - cbarL[1]) * (h[1] - h[1])); //0
a1L[N] = 0.5 / cbarL[N] * (hu[N - 1] - hu[N] + (ubarL[N] + cbarL[N]) * (h[N] - h[N - 1]));
a2L[N] = 0.5 / cbarL[N] * (hu[N] - hu[N - 1] - (ubarL[N] - cbarL[N]) * (h[N - 1] - h[N]));
a1R[1] = 0.5 / cbarR[1] * (hu[1] - hu[2] + (ubarR[1] + cbarR[1]) * (h[2] - h[1]));
a2R[1] = 0.5 / cbarR[1] * (hu[2] - hu[1] - (ubarR[1] - cbarR[1]) * (h[1] - h[2]));
a1R[N] = 0.5 / cbarR[N] * (hu[N] - hu[N] + (ubarR[N] + cbarR[N]) * (h[N] - h[N])); //0
a2R[N] = 0.5 / cbarR[N] * (hu[N] - hu[N] - (ubarR[N] - cbarR[N]) * (h[N] - h[N])); //0
for j in 2:N - 1 loop
ubarL[j] = (hu[j - 1] / sqrt(h[j - 1]) + hu[j] / sqrt(h[j])) / (sqrt(h[j - 1]) + sqrt(h[j]));
cbarL[j] = sqrt(g * (h[j - 1] + h[j]) / 2);
ubarR[j] = (hu[j] / sqrt(h[j]) + hu[j + 1] / sqrt(h[j + 1])) / (sqrt(h[j]) + sqrt(h[j + 1]));
cbarR[j] = sqrt(g * (h[j] + h[j + 1]) / 2);
a1L[j] = 0.5 / cbarL[j] * (hu[j - 1] - hu[j] + (ubarL[j] + cbarL[j]) * (h[j] - h[j - 1]));
a2L[j] = 0.5 / cbarL[j] * (hu[j] - hu[j - 1] - (ubarL[j] - cbarL[j]) * (h[j] - h[j - 1]));
a1R[j] = 0.5 / cbarR[j] * (hu[j] - hu[j + 1] + (ubarR[j] + cbarR[j]) * (h[j + 1] - h[j]));
a2R[j] = 0.5 / cbarR[j] * (hu[j + 1] - hu[j] - (ubarR[j] - cbarR[j]) * (h[j + 1] - h[j]));
end for;
for j in 1:N loop
der(h[j]) = -((ubarL[j] + cbarL[j]) * a2L[j] + (ubarR[j] - cbarR[j]) * a1R[j]) * N / L;
der(hu[j]) = -((ubarL[j] + cbarL[j]) ^ 2 * a2L[j] + (ubarR[j] - cbarR[j]) ^ 2 * a1R[j]) * N / L;
end for;
	annotation(

	experiment(
		MMO_Description=" Shallow Water wave equation",
		MMO_Solver=LIQSS2,
		MMO_Output={h[249]},
		StartTime= 0.0,
		StopTime= 5.0,
		Tolerance={1e-5},
		AbsTolerance={1e-5}
	));
end shallow;
