-- PipeSink File
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library std;
use std.textio.all;

entity pipe_sink is
	generic
	(
		DataWidth_g : natural := 8;
		OutFile_g : string := "sink.txt";
		Signal_g : boolean := false 
	);
	port
	(
		Data_i : in std_logic_vector(DataWidth_g - 1 downto 0)
	);
end entity pipe_sink;

architecture behavioral of pipe_sink is

begin

end architecture behavioral;
