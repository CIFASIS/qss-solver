
/*****************************************************************************

    This file is part of Modelica C Compiler.

    Modelica C Compiler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Modelica C Compiler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Modelica C Compiler.  If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/


namespace Modelica
{
  namespace Parser {
    struct QuotedChars : qi::symbols<char,char>
    {
      QuotedChars()
        {
        add
          (" ",' ')
          ("!",'!')
          ("#",'#')
          ("$",'$')
          ("%",'%')
          ("&",'&')
          ("(",'(')
          (")",')')
          ("*",'*')
          ("+",'+')
          (",",',')
          ("-",'-')
          (".",'.')
          ("/",'/')
          (":",':')
          (";",';')
          ("<",'<')
          (">",'>')
          ("=",'=')
          ("?",'?')
          ("@",'@')
          ("[",'[')
          ("]",']')
          ("^",'^')
          ("{",'{')
          ("}",'}')
          ("~",'~')
          ("|",'|')
        ;
        }
    } quoted_chars;

    struct Keywords : qi::symbols<char, std::string>
    {
      Keywords()
      {
        add
           ("algorithm","algorithm")
           ("and","and")
           ("annotation","annotation")
           ("block","block")
           ("break","break")
           ("class","class")
           ("connect","connect")
           ("connector","connector")
           ("constant","constant")
           ("constrainedby","constrainedby")
           ("der","der")
           ("discrete","discrete")
           ("each","each")
           ("else","else")
           ("elseif","elseif")
           ("elsewhen","elsewhen")
           ("encapsulated","encapsulated")
           ("end","end")
           ("enumeration","enumeration")
           ("equation","equation")
           ("expandable","expandable")
           ("extends","extends")
           ("external","external")
           ("false","false")
           ("final","final")
           ("flow","flow")
           ("for","for")
           ("function","function")
           ("if","if")
           ("import","import")
           ("impure","impure")
           ("in","in")
           ("initial","initial")
           ("inner","inner")
           ("input","input")
           ("loop","loop")
           ("model","model")
           ("not","not")
           ("operator","operator")
           ("or","or")
           ("outer","outer")
           ("output","output")
           ("package","package")
           ("parameter","parameter")
           ("partial","partial")
           ("protected","protected")
           ("public","public")
           ("pure","pure")
           ("record","record")
           ("redeclare","redeclare")
           ("replaceable","replaceable")
           ("return","return")
           ("stream","stream")
           ("then","then")
           ("true","true")
           ("type","type")
           ("when","when")
           ("while","while")
           ("within","within")
        ;
      }
    } keywords;
    
    template <typename Iterator>
    IdentRule<Iterator>::IdentRule(Iterator &it) : IdentRule::base_type(ident) {
      using qi::char_;
      using qi::alpha;
      using qi::alnum;
      using qi::lexeme;

      keyword_ident = keywords >> +(alnum | char_('_')) 
                    ;

      ident = lexeme[((char_('_') | alpha) >> *(alnum | char_('_'))) - keywords]
             | lexeme[keyword_ident]
             | lexeme[char_('\'') >> *(alnum | char_('_') | quoted_chars) > char_('\'')] 
             ;


      ident.name("identifier"); 
      keyword_ident.name("identifier"); 
    }
  }
}
