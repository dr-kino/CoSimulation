-- PipeBus File
library ieee;
use ieee.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.numeric_std.all;

library std;
use std.textio.all;

library work;
use work.math_pkg.all;

entity pipe_bus is
	generic
	(
		AddrWidth_g : natural := 2;
		DataWidth_g : natural := 8;
		InputFile_g : string := "bus_sw2hw.txt";
		OutputFile_g : string := "bus_hw2sw.txt"
	);
	port
	(
		Clk_Bus_i : in std_logic;
		Ready_i : in std_logic;
		Addr_o : out std_logic_vector(AddrWidth_g - 1 downto 0);
		DataA_o : out std_logic_vector(DataWidth_g - 1 downto 0);
		DataB_o : out std_logic_vector(DataWidth_g - 1 downto 0);
		Ready_o : out std_logic;
		DataBus_i : in std_logic_vector(DataWidth_g - 1 downto 0)
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
		variable result : std_logic_vector(7 downto 0);
	begin
		result := (others => '0');
		for i in 0 to A'length - 1 loop
			result(4*(i + 1) - 1 downto 4*i) := ToHex(A(i + 1));
		end loop;
		return result(Size - 1 downto 0);
	end function;

	function ToHex(A:std_logic_vector; Size:positive) return string is
		variable result : string(Size downto 1);
		variable a_v : std_logic_vector(max(Size * 4, 4 *cdiv(A'length, 4)) - 1 downto 0);
	begin
		a_v := (others => '0');
		a_v(A'length - 1 downto 0) := A;
		for i in 0 to Size - 1 loop
			result(i + 1) := ToHex(A_v(4 * (i + 1) - 1 downto 4 * i));
		end loop;
		return result(Size downto 1);
	end function;

	function InsertChar(A:string; B:character) return string is
		variable C : string(A'range);
	begin
		for i in A'low + 1 to A'high loop
			C(i) := A(i - 1);
		end loop;
		C(A'low) := B;
		return C;
	end function;
 
	function To_01(A:std_logic_vector) return std_logic_vector is
		variable result : std_logic_vector(A'length -1 downto 0);
	begin
		result := To_X01(A);
		for i in result'range loop
			if Is_X(A(i)) then
				result(i) := '0';
			end if;
		end loop;
		return result;
	end function;

	procedure ReadCommand(file f: TEXT; OperA : out string; OperB : out string; OperC : out string) is
		variable l : line;
		variable char : character;
		variable good : boolean;
		variable exec : boolean;
		variable OperA_v : string(OperA'range);
		variable OperB_v : string(OperB'range);
		variable OperC_v : string(OperC'range);
		variable field : natural;
	begin
		-- Start filling field 0
		field := 0;
		-- There is no command to exec
		exec := false;
		-- Fill fields with spaces
		OperA_v := (others => ' ');
		OperB_v := (others => ' ');
		OperC_v := (others => ' ');

		-- Is there any command?
		if not endfile(f) then
			-- Read line and extract fields
			readline(f, l);
			read(l, char, good);
			while good and not exec loop
				case char is
					when ' ' | ',' =>
						-- Next field
						field := field + 1;
					when ';' =>
						-- Execute command
						exec := true;
					when others =>
						-- Extract field
						case field is
							when 0 =>
								OperA_v := InsertChar(OperA_v, char);
							when 1 =>
								OperB_v := InsertChar(OperB_v, char);
							when 2 =>
								OperC_v := InsertChar(OperC_v, char);
							when others =>
								null;
						end case;
				end case;

				-- Read next character
				read(l, char, good);
			end loop;
		end if;

		-- Execute command
		if exec then
			OperA := OperA_v;
			OperB := OperB_v;
			OperC := OperC_v;
		end if;
	end procedure;

	procedure WriteCommand(OperA:string) is
		file f : text;
		variable l : line;
	begin
		file_open(f, OutputFile_g, append_mode);
		write(l, OperA & ";");
		writeline(f, l);
		file_close(f);
	end procedure;

	procedure WriteCommand(OperA:string;OperB:string) is
		file f : text;
		variable l : line;
	begin
		file_open(f, OutputFile_g, append_mode);
		write(l, OperA & ",");
		write(l, OperB & ";");
		writeline(f, l);
		file_close(f);
	end procedure;

begin

	process
		file f : text;
		variable f_st : file_open_status;
		variable Addr : string((Addr_o'length - 1) downto 1);
		variable Data : string((DataBus_i'length - 1)/4 + 1 downto 1);
		variable Cmd : string(1 downto 1);
		file f2 : text open write_mode is "STD_OUTPUT";
		variable l2 : line;
		variable odata_v : string((DataBus_i'length -1)/4 + 1 downto 1);
	begin
		write(l2, "Opening " & InputFile_g);
		writeline(f2, l2);
		f_st := status_error;
		while f_st /= open_ok loop
			file_open(f_st, f, InputFile_g, read_mode);
		end loop;
		write(l2, InputFile_g & " was successfuly opened");
		writeline(f2, l2);

		loop
			wait for 100 ns;
		
			write(l2, "Waiting command...");
			writeline(f2, l2);

			Cmd := (others => ' ');
			Addr := (others => ' ');
			Data := (others => ' ');
			-- Blocked waiting command
			WriteCommand("W");
			ReadCommand(f, Cmd, Data, Addr);
			write(l2, "Command: ");
			case Cmd is
				when "W" | "w" =>
					write(l2, "Write ");
					write(l2, Data & " to address ");
					write(l2, Addr);
					writeline(f2, l2);

					Addr_o <= ToHex(Addr, Addr_o'length);

					if ToHex(Addr, Addr_o'length) = "01" then
						DataA_o <= ToHex(Data, DataA_o'length);
					else
						DataB_o <= ToHex(Data, DataB_o'length);
					end if;
					
					-- Acknowledge command was accepted
					WriteCommand("A");

					-- Wait until command is executed
					loop
						wait until rising_edge(Clk_Bus_i);
						if Ready_i = '0' then
							-- Done executing
							WriteCommand("D");
							exit;
						else
							-- Still busy
							WriteCommand("B");
						end if;
						
					end loop;

				when "R" | "r" =>
					write(l2, "Read from address ");
					writeline(f2, l2);

					-- Acknowledge command was accepted
					WriteCommand("A");

					-- Wait until command is executed
					loop
						wait until rising_edge(Clk_Bus_i);
						if Ready_i = '0' then
							-- Done executing
							WriteCommand("D");
							exit;
						else
							-- Still busy
							WriteCommand("B");
						end if;
						
					end loop;

					-- Wait until data may be fecthed
					wait until rising_edge(Clk_Bus_i);
					-- Fetch data
					odata_v := ToHex(To_01(DataBus_i), odata_v'length);
					WriteCommand("F", odata_v);
					-- Done executing
					WriteCommand("D");

				when "N" | "n" =>
					write(l2, "No Operation");
					writeline(f2, l2);

					-- Acknowledge command
					WriteCommand("A");

					-- Wait one clock cycle
					wait until rising_edge(Clk_Bus_i);

					-- Done executing
					WriteCommand("D");

				when "Q" | "q" =>
					write(l2, "Quit co-simulation by free running vhdl simulation...");
					writeline(f2, l2);

					-- Acknowledge command
					WriteCommand("A");
					-- Other simulation side regard as done 
					WriteCommand("D");

					-- Wait forever, so vhdl simulation may run freely.
					wait;

				when "S" | "s" =>
					write(l2, "Sync ");
					writeline(f2, l2);

					-- Simulation sync acknowledge
					WriteCommand("S");

				when others =>
					write(l2, "Invalid");
					writeline(f2, l2);
					Addr_o <= (others => '0');

					-- Acknowledge invalid command
					WriteCommand("I");
			end case;
		end loop;
	end process;
						
end architecture behavioral;
