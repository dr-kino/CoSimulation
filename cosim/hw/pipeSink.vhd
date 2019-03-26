-- PipeSink File
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library std;
use std.textio.all;

--library work;
--use work.math_pkg.all;

entity pipe_sink is
	generic
	(
		DataWidth_g : natural := 8;
		InFile_g : string := "../../../cosim/pipe/sink_sw2hw.txt";
		OutFile_g : string := "../../../cosim/pipe/sink_hw2sw.txt"
	);
	port
	(
		Data_i : in std_logic_vector(DataWidth_g - 1 downto 0);
		Data_o : out std_logic_vector(DataWidth_g - 1 downto 0)
	);
end entity pipe_sink;

architecture behavioral of pipe_sink is
	
	-- Commands
	constant Cmd_Sync_c : string(1 downto 1) := "S";

	-- Acknowledge command
	procedure WriteCommand(OperA:string) is
		file f : text;
		variable l : line;
	begin
		file_open(f, OutFile_g, append_mode);
		write(l, OperA & ";");
		writeline(f, l);
		file_close(f);
	end procedure;

	-- Convert from time to integer in (ps)
	function Time2ps(A:time) return integer is
		variable B : integer;
	begin
		B := A/1ps;
		return B;
	end function;

begin

	process
		variable tlast : time;
		variable tnow : time;
		file f2 : text open write_mode is "STD_OUTPUT";
		variable l2 : line;
		variable test : unsigned(9 downto 0) := (others => '0');

	begin
		-- Store start simulation time
		tnow := now;

		loop
			tlast := tnow;
			tnow := now;
			wait for 100 ns;
			-- Send sample
			--WriteCommand("S", integer'image(Time2ps(tnow - tlast)), integer'image(to_integer(signed(Data_i))));
		end loop;
	end process;

end architecture behavioral;
