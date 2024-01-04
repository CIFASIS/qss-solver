model seir4

Real S, E, I;
Real Mild, Severe, SevereH, Fatal;
Real R_Mild, R_Severe, R_Fatal;

parameter Real Npop                   = 45376763.;
parameter Real I0                     = 1.;
parameter Real R0                     = 3.34;
parameter Real T_incubation           = 5.2;
parameter Real T_infectious           = 2.9;
parameter Real CFR                    = 0.01;
parameter Real Time_to_death          = 32;
parameter Real P_SEVERE               = 0.2;
parameter Real InterventionAmt        = 0.27;
parameter Real Intervention_time      = 39.;
parameter Real duration               = 1e20;
parameter Real T_recovery_mild        = 11.1;
parameter Real T_recovery_severe      = 28.6;
parameter Real T_hospital_lag         = 5.;

parameter Real quarantineDuration     = 14.;
parameter Real postQuarantineDuration = 14.;
parameter Real supressionFactor       = 0.1;
parameter Real mitigationFactor       = 1.;

parameter Real a = 1./T_incubation;
parameter Real gamma = 1./T_infectious;
parameter Real p_mild = 1. - p_severe - p_fatal;
parameter Real p_severe = P_SEVERE;
parameter Real p_fatal = CFR;
parameter Real T_death = Time_to_death - T_infectious;

discrete Real beta(fixed=false);
discrete Real supressionTime(fixed=false);
discrete Real mitigationTime(fixed=false);


initial algorithm 
    beta := R0/T_infectious;

    supressionTime := Intervention_time;
    mitigationTime := supressionTime + quarantineDuration;

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
    der(I)         = a*E - gamma*I;
    der(Mild)      = p_mild*gamma*I - Mild/T_recovery_mild;
    der(Severe)    = p_severe*gamma*I - Severe/T_hospital_lag;
    der(SevereH)   = Severe/T_hospital_lag - SevereH/T_recovery_severe;
    der(Fatal)     = p_fatal*gamma*I - Fatal/T_death;
    der(R_Mild)    = Mild/T_recovery_mild;
    der(R_Severe)  = SevereH/T_recovery_severe;
    der(R_Fatal)   = Fatal/T_death;

algorithm   
    when time >= supressionTime then
        beta := (R0*supressionFactor)/T_infectious;
        supressionTime := supressionTime + (quarantineDuration+postQuarantineDuration);
    end when;

    when time >= mitigationTime then
        beta := (R0*mitigationFactor)/T_infectious;
        mitigationTime := supressionTime + quarantineDuration;
    end when;

annotation(
    experiment(
        MMO_Description="SEIR",
        MMO_Solver=QSS2,
        MMO_SymDiff=false,
        MMO_Period={1e-1},
        MMO_PartitionMethod=Metis,
        MMO_Output={Npop,S,E,I,Mild,Severe,SevereH,Fatal,R_Mild,R_Severe,R_Fatal,a,beta,gamma},
        MMO_OutputType=CI_Sampled,
        MMO_Scheduler=ST_Binary,
        Jacobian=Dense,
        StartTime=0,
        StopTime=200,
        Tolerance={1e-05},
        AbsTolerance={1e-08}
    ));


end seir4;
