package retQSS_indinf

  function infectVolume
    input Integer volumeID;
    input Real eventTime[1];
    input Real detectionTime[1];
    input Real prob;
    input Real t;
    input Real min_evt_time;
    input Real max_evt_time;
    input Real min_detection_time;
    input Real max_detection_time;
    output Boolean status;
    external "C" status=indinf_infectVolume(volumeID,eventTime,detectionTime,prob,t,min_evt_time,max_evt_time,min_detection_time,max_detection_time) annotation(
        Library="indinf",
        Include="#include \"retqss_indinf.h\"");
  end infectVolume;

  function dumpOutput
    input String filename;
    output Boolean status;
    external "C" status=indinf_dumpOutput(filename) annotation(
        Library="indinf",
        Include="#include \"retqss_indinf.h\"");
  end dumpOutput;

  function dumpGraph
    input String filename;
    input Integer vtype;
    output Boolean status;
    external "C" status=indinf_dumpGraph(filename,vtype) annotation(
        Library="indinf",
        Include="#include \"retqss_indinf.h\"");
  end dumpGraph;

  function checkVolumeInfection
    input Integer v;
    output Boolean status;
    external "C" status=indinf_checkVolumeInfection(v) annotation(
        Library="indinf",
        Include="#include \"retqss_indinf.h\"");
  end checkVolumeInfection;

  function updateContacts
    input Integer p;
    input Integer v;
    input Real t;
    output Boolean status;
    external "C" status=indinf_updateContacts(p,v,t) annotation(
        Library="indinf",
        Include="#include \"retqss_indinf.h\"");
  end updateContacts;

  function isolateContacts
    input Integer p;
    input Integer _max;
    output Boolean status;
    external "C" status=indinf_isolateContacts(p,_max) annotation(
        Library="indinf",
        Include="#include \"retqss_indinf.h\"");
  end isolateContacts;

end retQSS_indinf;
