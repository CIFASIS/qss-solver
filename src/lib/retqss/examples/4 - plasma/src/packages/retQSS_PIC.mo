package retQSS_PIC

  function solvePotentialInNodes
    input Integer ny;
    input Real q_eps;
    input Real n0;
    input Real phi0;
    input Real Te;
    output Integer status;
    external "C" status=PIC_solvePotentialInNodes(ny,q_eps,n0,phi0,Te) annotation(
        Library="pic",
        Include="#include \"retqss_pic.h\"");
  end solvePotentialInNodes;

end retQSS_PIC;
