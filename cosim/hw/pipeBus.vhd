-- PipeBus File
library ieee;
use ieee.std_logic_1164.all;

library std;
use std.textio.all;

library work;
use work.math_pkg.all;

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

	function ToHex(A : character) return std_logic_vector is
		variable R : std_logic_vector(3 downto 0);
	begin
		case A is
			when '0' =>
				R := x"0";
			when '1' =>
				R := x"1";
			when '2' =>
				R := x"2";
			when '3' =>
				R := x"3";
			when '4' =>
				R := x"4";
			when '5' =>
				R := x"5";
			when '6' =>
				R := x"6";
			when '7' =>
				R := x"7";
			when '8' =>
				R := x"8";
			when '9' =>
				R := x"9";
			when 'A' | 'a' =>
				R := x"A";
			when 'B' | 'b' =>
				R := x"B";
			when 'C' | 'c' =>
				R := x"C";
			when 'D' | 'd' =>
				R := x"D";
			when 'E' | 'e' =>
				R := x"E";
			when 'F' | 'f' =>
				R := x"F";
			when ' ' =>
				R := x"0";
			when others =>
				R := (others => 'X');
		end case;
		return R;
	end function;

	function ToHex(A : std_logic_vector(3 downto 0)) return character is
		variable R : character;
	begin
		case A is
			when x"0" =>
				R := '0';
			when x"1" =>
				R := '1';
			when x"2" =>
				R := '2';
			when x"3" =>
				R := '3';
			when x"4" =>
				R := '4';
			when x"5" =>
				R := '5';
			when x"6" =>
				R := '6';
			when x"7" =>
				R := '7';
			when x"8" =>
				R := '8';
			when x"9" =>
				R := '9';
			when x"A" =>
				R := 'A';
			when x"B" =>
				R := 'B';
			when x"C" =>
				R := 'C';
			when x"D" =>
				R := 'D';
			when x"E" =>
				R := 'E';
			when x"F" =>
				R := 'F';
			when others =>
				R := 'X';
		end case;
		return R;
	end function;

	function ToHex(A:string; Size:positive) return std_logic_vector is
		variable result : std_logic_vector(max(A'length * 4, Size) - 1 downto 0);
	begin
		result := (others => '0');
		for i in 0 to A'length - 1 loop
			result(4*(i + 1) - 1 downto 4*i) := ToHex(A(i + 1));
		end loop;
		return result(Size - 1 downto 0);
	end function;

	function ToHex(A:std_logic_vector; Size:positive) return string is
		variable result : string(Size downto 1);
		variable a_v : std_logic_vector(max(Size * 4, 4 * cdiv(A'length, 4)) - 1 downto 0);
	begin
		a_v := (others => '0');
		a_v(A'length - 1 downto 0) := A;
		for i in 0 to Size - 1 loop
			result(i + 1) := ToHex(A_v(4 * (i + 1) - 1 downto 4 * i));
		end loop;
		return result(Size downto 1);
	end function;

begin

end architecture behavioral;
