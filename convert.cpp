/*****************************************************************************

 This file is part of QSS Solver.

 QSS Solver is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 QSS Solver is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with QSS Solver.  If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#include <getopt.h>
#ifdef __linux__
#include <libio.h>
#endif
#include <sbml/Model.h>
#include <sbml/SBMLDocument.h>
#include <sbml/SBMLReader.h>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <string>

#include "mmo_convert.h"
#include "mmo_writer.h"

using namespace std;

void
usage ()
{
    cout << "Usage translate-sbml [options] file" << endl;
    cout << "Translate a SBML (Systems Biology Markup Language) model into a MicroModelica model or BioRica model." << endl;
    cout << endl;
    cout << "-h, --help      Display this information and exit" << endl;
    cout << "-o <file>, --output <file>" << endl;
    cout << "                Sets the output to <file>" << endl;
    cout << "-r, --replace-functions  " << endl;
    cout << "                Set SBML function replacement for MicroModelica, each MathML function defined in the model translated into " << endl;
    cout << "                an algebraic variable in MicroModelica. When translating an SBML model into BioRica, this is the only behavior" << endl;
    cout << "                possible given that we can't define functions." << endl;
    cout << "-t <file-type>, --type <file-type>" << endl;
    cout << "                Set the translator output type to <file-type>, the possibilities are:          " << endl;
    cout << "                    mo: Generate MicroModelica file (default)" << endl;
    cout << "                    br: Generate BioRica file                " << endl;
    cout << "-v, --version   Display version information and exit" << endl;
    cout << endl;
    cout << "Report bugs to: joaquin.f.fernandez@gmail.com" << endl;
    cout << "SBML Translator home page: https://sourceforge.net/p/qssengine/" << endl;
}

void
version ()
{
    cout << "SBML Translator 1.0" << endl;
    cout << "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>" << endl;
    cout << "This is free software: you are free to change and redistribute it." << endl;
    cout << "There is NO WARRANTY, to the extent permitted by law." << endl;
}

int
main (int argc, char* argv[])
{
    unsigned int errors = 0;
    extern char *optarg;
    string output;
    string type = "mo";
    WR_Type wType = WR_MMO;
    bool replace = false;
    int opt;
    while (1)
    {
        static struct option long_options[] =
        {
        { "version", no_argument, 0, 'v' },
        { "help", no_argument, 0, 'h' },
        { "type", required_argument, 0, 't' },
        { "replace-functions", no_argument, 0, 'r' },
        { "output", required_argument, 0, 'o' },
        { 0, 0, 0, 0 } };
        int option_index = 0;

        opt = getopt_long (argc, argv, "vhrt:o:", long_options, &option_index);

        /* Detect the end of the options. */
        if (opt == EOF)
            break;
        switch (opt)
        {
            case 'v':
                version ();
                exit (0);
            case 'h':
                usage ();
                exit (0);
            case 'r':
                replace = true;
                break;
            case 't':
                type = optarg;
                if (!type.compare ("br"))
                {
                    replace = true;
                    wType = WR_BR;
                }
                else if (!type.compare ("mo"))
                {
                    replace = false;
                }
                break;
            case 'o':
                output = optarg;
                break;
            case '?':
                usage ();
                exit (-1);
                break;
            default:
                abort ();
        }
    }
    string fileName;
    string prefix;
    if (argv[optind] != NULL)
    {
        fileName = argv[optind];
        if (output.empty ())
        {
            output = fileName;
            unsigned found = output.rfind (".xml");
            if (found != std::string::npos)
            {
                output.erase (output.begin () + found + 1, output.end ());
            }
            output.append (type);
        }
        prefix = output;
        unsigned found = prefix.rfind (".");
        if (found != std::string::npos)
        {
            prefix.erase (prefix.begin () + found, prefix.end ());
        }
        found = prefix.rfind ("/");
        if (found != std::string::npos)
        {
            prefix.erase (prefix.begin (), prefix.begin () + ++found);
        }
        SBMLDocument* document = readSBML (fileName.c_str ());
        Model *model = document->getModel ();
        errors = document->getNumErrors ();
        cout << endl;
        cout << "  filename: " << fileName << endl;
        cout << "  error(s): " << errors << endl;
        cout << "  output: " << output << endl;
        cout << endl;
        if (errors > 0)
        {
            cout << "Errors found: " << endl;
            document->printErrors (cerr);
            return (errors);
        }
        MMOConvert convert = MMOConvert (prefix, replace, wType);
        model->accept (convert);
        convert.model ().write (output);
    }
    return (errors);
}
