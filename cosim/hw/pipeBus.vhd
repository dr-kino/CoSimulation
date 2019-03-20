-- PipeBus File
library ieee;
use ieee.std_logic_1164.all;

library std;
use std.textio.all;

entity pipe_bus is
	generic
	(
		DataWidth_g : natural := 8;
		InputFile_g : string := "bus_sw2hw.txt"
		OutputFile_g : string := "bus_hw2sw.txt"
	);
	port
	(
		Ready_i : in std_logic;
		DataA_i : in std_logic_vector(DataWidth_g - 1 downto 0);
		DataB_i : in std_logic_vector(DataWidth_g - 1 downto 0);
		Ready_o : out std_logic;
		Data_o : out std_logic_vector(DataWidth_g - 1 downto 0)
	);
end entity pipe_bus;

architecture behavioral of pipe_bus is

begin

end architecture behavioral;
