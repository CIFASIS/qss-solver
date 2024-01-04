model seir

Real S, E, I;
Real Mild, Severe, SevereH, Fatal;
Real R_Mild, R_Severe, R_Fatal;

parameter Real N, I0, R0, Rt;
parameter Real D_incubation, D_infectious;
parameter Real CFR, Time_to_death, P_SEVERE, OMInterventionAmt;
parameter Real InterventionAmt, Intervention_time, duration;

parameter Real a, _gamma;
parameter Real p_mild, p_severe, p_fatal;
parameter Real D_recovery_mild, D_recovery_severe, D_death, D_hospital_lag;

discrete Real beta, beta_time;


initial algorithm 
    N := 7e6;
    I0 := 1.;
    R0 := 2.2;
    D_incubation := 5.2;
    D_infectious := 2.9;
    CFR := 0.02;
    Time_to_death := 32;
    D_recovery_severe := 28.6;
    D_recovery_mild := 11.1;
    P_SEVERE := 0.2;
    D_hospital_lag := 5;
    OMInterventionAmt := 2./3;
    InterventionAmt := 1. - OMInterventionAmt;
    Intervention_time := 100;
    duration := 1e20;

    a := 1./D_incubation;
    _gamma := 1./D_infectious;
    p_severe := P_SEVERE;
    p_fatal := CFR;
    p_mild := 1. - p_severe - p_fatal;
    D_death := Time_to_death - D_infectious;
    Rt := R0*InterventionAmt;

    beta := R0/D_infectious;

    beta_time := Intervention_time;

    S := 1;
    E := 0;
    I := I0/(N-I0);
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
    when time >= beta_time then
        if beta_time == Intervention_time then
            beta := (InterventionAmt)*R0/D_infectious;
            beta_time := Intervention_time+duration;
        elseif beta_time == Intervention_time+duration then
            beta := 0.5*R0/D_infectious;
        end if;
    end when;

annotation(
    experiment(
        MMO_Description="SEIR",
        MMO_Solver=QSS2,
        MMO_SymDiff=false,
        MMO_Period={1e-1},
        MMO_PartitionMethod=Metis,
        MMO_Output={N,S,E,I,Mild,Severe,SevereH,Fatal,R_Mild,R_Severe,R_Fatal,a,beta,_gamma},
        MMO_OutputType=CI_Sampled,
        MMO_Scheduler=ST_Binary,
        Jacobian=Dense,
        StartTime=0,
        StopTime=200,
        Tolerance={1e-10},
        AbsTolerance={1e-13}
    ));

end seir;
