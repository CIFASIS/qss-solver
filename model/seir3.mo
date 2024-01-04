model seir2

Real S, E, I;
Real Mild, Severe, SevereH, Fatal;
Real R_Mild, R_Severe, R_Fatal;

parameter Real Npop(fixed=false), I0(fixed=false), R0(fixed=false);
parameter Real D_incubation(fixed=false), D_infectious(fixed=false);
parameter Real CFR(fixed=false), Time_to_death(fixed=false),
P_SEVERE(fixed=false), OMInterventionAmt(fixed=false);
parameter Real InterventionAmt(fixed=false),
Intervention_time(fixed=false), duration(fixed=false);
parameter Real a(fixed=false), _gamma(fixed=false);
parameter Real p_mild(fixed=false), p_severe(fixed=false),
p_fatal(fixed=false);
parameter Real D_recovery_mild(fixed=false),
D_recovery_severe(fixed=false), D_death(fixed=false),
D_hospital_lag(fixed=false);
discrete Real beta(fixed=false);

constant Integer Nquar = 5;
discrete Real quarantineDuration[Nquar];
discrete Real postQuarantineDuration[Nquar];
discrete Real supressionFactor[Nquar];
discrete Real mitigationFactor[Nquar];
discrete Real supressionTime[Nquar], mitigationTime[Nquar];


initial algorithm 
    Npop := 45376763.;
    I0 := 1.;
    R0 := 3.34; // R0 Rober
    D_incubation := 5.2;
    D_infectious := 2.9;
    CFR := 0.01; // CFR Rober
    Time_to_death := 32;
    D_recovery_severe := 28.6;
    D_recovery_mild := 11.1;
    P_SEVERE := 0.2;
    D_hospital_lag := 5;
    Intervention_time := 39; // t0_simu = 21/2 y tcuar = 20/3
    duration := 1e20;
    InterventionAmt := 0.27; // Min = 0.15, Max = 0.45, Avg = 0.3

    a := 1./D_incubation;
    _gamma := 1./D_infectious;
    p_severe := P_SEVERE;
    p_fatal := CFR;
    p_mild := 1. - p_severe - p_fatal;
    D_death := Time_to_death - D_infectious;

    beta := R0/D_infectious;

    for i in 1:Nquar loop
        quarantineDuration[i] := 14;
        postQuarantineDuration[i] := 14;

        supressionFactor[i] := 0.1;
        mitigationFactor[i] := 0.5;

        if i == 1 then
            supressionTime[i] := Intervention_time;
        else
            supressionTime[i] := supressionTime[i-1] + (quarantineDuration[i-1]+postQuarantineDuration[i-1]);
        end if;

        mitigationTime[i] := supressionTime[i] + quarantineDuration[i];
    end for;

    S := 1;
    E := 0;
    I := I0/(Npop-I0);
    Mild := 0;
    Severe := 0;
    SevereH := 0;
    Fatal := 0;
    R_Mild := 0;
    R_Severe := 0;
    R_Fatal := 0;

equation
    der(S)         = -beta*I*S;
    der(E)         = beta*I*S - a*E;
    der(I)         = a*E - _gamma*I;
    der(Mild)      = p_mild*_gamma*I - Mild/D_recovery_mild;
    der(Severe)    = p_severe*_gamma*I - Severe/D_hospital_lag;
    der(SevereH)   = Severe/D_hospital_lag - SevereH/D_recovery_severe;
    der(Fatal)     = p_fatal*_gamma*I - Fatal/D_death;
    der(R_Mild)    = Mild/D_recovery_mild;
    der(R_Severe)  = SevereH/D_recovery_severe;
    der(R_Fatal)   = Fatal/D_death;

algorithm   
    for i in 1:Nquar loop
        when time >= supressionTime[i] then
            beta := (R0*supressionFactor[i])/D_infectious;
        end when;

        when time >= mitigationTime[i] then
            beta := (R0*mitigationFactor[i])/D_infectious;
        end when;
    end for;

annotation(
    experiment(
        MMO_Description="SEIR",
        MMO_Solver=QSS2,
        MMO_SymDiff=false,
        MMO_Period={1e-1},
        MMO_PartitionMethod=Metis,
        MMO_Output={Npop,S,E,I,Mild,Severe,SevereH,Fatal,R_Mild,R_Severe,R_Fatal,a,beta,_gamma},
        MMO_OutputType=CI_Sampled,
        MMO_Scheduler=ST_Binary,
        Jacobian=Dense,
        StartTime=0,
        StopTime=200,
        Tolerance={1e-05},
        AbsTolerance={1e-08}
    ));


end seir2;
