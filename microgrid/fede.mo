package fede
	function get_nextpeople_qss
		input Real x;
		output Real y;
		external "C" y = get_nextpeople_qss(x) annotation(
																Include="#include \"people.c\""); 
	end get_nextpeople_qss;

	function get_npeople_qss
		input Real x;
		output Real y;
		external "C" y = get_npeople_qss(x) annotation(
																Include="#include \"people.c\""); 
	end get_npeople_qss;
end fede;
