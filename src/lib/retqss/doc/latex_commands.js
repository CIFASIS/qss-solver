MathJax.Hub.Config(
{
    "TeX": {
        "Macros": {
            "NPart": ["N_p"],
            "NVol": ["N_v"],
            "NVert": ["N_u"],

            "PartPos": ["\\mathbf{x}_{#1}",1],
            "PartVel": ["\\vec{\\mathbf{v}}_{#1}",1],

            "Vol": ["\\textrm{vol}(#1)",1],

            "PartNeigh": ["\\textrm{Neigh}(#1)",1],

            "VecNorm": ["\\left\\Vert #1 \\right\\Vert",1],

            "uMod" : ["\\mu-\\textrm{Modelica}"],
            "dQRel" : ["\\Delta \\textrm{QRel}"],
            "dQMin" : ["\\Delta \\textrm{QMin}"]
        }
    }
}
);
