package file
	function save 
		input Real x;
		output Real y;
		external "C" y = mmo_save(x) annotation(
																Library="mmo_file",
																Include="#include <mmo_file.h>");
	end save;
end file;