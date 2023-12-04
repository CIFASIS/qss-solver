package retQSS_md

  function dummy
    output Boolean status;
    external "C" status=dummy() annotation(
        Library="md",
        Include="#include \"retqss_md.h\"");
  end dummy;

end retQSS_md;
