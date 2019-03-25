--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   23:21:29 03/18/2019
-- Design Name:   
-- Module Name:   /home/rcavalcanti/Projects/tmp/CoSimulation/hw/xil/Adder/FullAdderTester.vhd
-- Project Name:  Adder
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: bitadder
-- 
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends
-- that these types always be used for the top-level I/O of a design in order
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY FullAdderTester IS
END FullAdderTester;
 
ARCHITECTURE behavior OF FullAdderTester IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT bitadder
    PORT(
         A : IN  std_logic_vector(7 downto 0);
         B : IN  std_logic_vector(7 downto 0);
         C_IN : IN  std_logic;
         S_O : OUT  std_logic_vector(7 downto 0);
         C_OUT : OUT  std_logic;
         OVERFLOW : OUT  std_logic
        );
    END COMPONENT;
    
	component pipe_bus is
	generic
	(
		DataWidth_g : natural := 8;
		InputFile_g : string := "bus_sw2hw.txt";
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
	end component pipe_bus;

	component pipe_sink is
	generic
	(
		DataWidth_g : natural := 8;
		InFile_g : string := "sink_sw2hw.txt";
		OutFile_g : string := "sink_hw2sw.txt"
	);
	port
	(
		Data_i : in std_logic_vector(DataWidth_g - 1 downto 0);
		Data_o : out std_logic_vector(DataWidth_g - 1 downto 0)
	);
	end component pipe_sink;

	-- Pipe Bus
	constant Pipe_DataWidth_g : natural := 8;
	signal Pipe_Ready_i : std_logic;
	signal Pipe_DataA_i : std_logic_vector(Pipe_DataWidth_g - 1 downto 0);
	signal Pipe_DataB_i : std_logic_vector(Pipe_DataWidth_g - 1 downto 0);
	signal Pipe_Ready_o : std_logic;
	signal Pipe_Data_o : std_logic_vector(Pipe_DataWidth_g - 1 downto 0);
	-- Pipe Sink
	signal PipeSink_DataWord_o : std_logic_vector(8 downto 0);
	signal PipeSink_Data_i : std_logic_vector(8 downto 0);
	signal PipeSink_Data_o : std_logic_vector(8 downto 0);

   --Inputs
   signal A : std_logic_vector(7 downto 0) := (others => '0');
   signal B : std_logic_vector(7 downto 0) := (others => '0');
   signal C_IN : std_logic := '0';

 	--Outputs
   signal S_O : std_logic_vector(7 downto 0);
   signal C_OUT : std_logic;
   signal OVERFLOW : std_logic;
   -- No clocks detected in port list. Replace <clock> below with 
   -- appropriate port name 
 
	signal clock : std_logic := '0';
   constant clock_period : time := 10 ns;
 
BEGIN
 
	Pipe_inst_bus: pipe_bus
	generic map
	(
		DataWidth_g => 8,
		InputFile_g => "bus_sw2hw.txt",
		OutputFile_g => "bus_hw2sw.txt"
	)
	port map
	(
		Ready_i => Pipe_Ready_i,
		DataA_i => Pipe_DataA_i,
		DataB_i => Pipe_DataB_i,
		Ready_o => Pipe_Ready_o,
		Data_o => Pipe_Data_o
	);

	Pipe_inst_sink: pipe_sink
	generic map
	(
		DataWidth_g => PipeSink_DataWord_o'length,
		InFile_g => "sink_sw2hw.txt",
		OutFile_g => "sink_hw2sw.txt"
	)
	port map
	(
		Data_i => PipeSink_Data_i, 
		Data_o => PipeSink_Data_o
	);

	-- Instantiate the Unit Under Test (UUT)
   uut: bitadder PORT MAP (
          A => A,
          B => B,
          C_IN => C_IN,
          S_O => S_O,
          C_OUT => C_OUT,
          OVERFLOW => OVERFLOW
        );

   -- Clock process definitions
   clock_process :process
   begin
		clock <= '0';
		wait for clock_period/2;
		clock <= '1';
		wait for clock_period/2;
   end process;
 

   -- Stimulus process
   stim_proc: process
   begin		
      -- hold reset state for 100 ns.
      wait for 100 ns;	

      wait for clock_period*10;

      -- insert stimulus here 
		A <= "00001111";
		wait for clock_period*10;
		B <= "10101010";
		wait for clock_period*10;
		A <= "11111111";
		B <= "11111111";
		
      wait;
   end process;

END;
