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
 
	component bitadder
    port(
         A : IN  std_logic_vector(7 downto 0);
         B : IN  std_logic_vector(7 downto 0);
         C_IN : IN  std_logic;
         S_O : OUT  std_logic_vector(7 downto 0);
         C_OUT : OUT  std_logic;
         OVERFLOW : OUT  std_logic
        );
	end component;
    
	component pipe_bus is
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
	end component pipe_bus;

	--component pipe_sink is
	--generic
	--(
	--	DataWidth_g : natural := 8;
	--	InFile_g : string := "sink_sw2hw.txt";
	--	OutFile_g : string := "sink_hw2sw.txt"
	--);
	--port
	--(
	--	Clk_Pipe_i : in std_logic;
	--	Data_i : in std_logic_vector(DataWidth_g - 1 downto 0);
	--	DataSink_o : out std_logic_vector(DataWidth_g - 1 downto 0)
	--);
	--end component pipe_sink;

	-- Pipe Bus
	constant Pipe_DataWidth_g : natural := 8;
	constant Pipe_AddrWidth_g : natural := 2;
	signal Pipe_Ready_i : std_logic := '0';
	signal Pipe_Addr_i : std_logic_vector(Pipe_AddrWidth_g - 1 downto 0) := "00";
	signal Pipe_DataA_i : std_logic_vector(Pipe_DataWidth_g - 1 downto 0) := "00000000";
	signal Pipe_DataB_i : std_logic_vector(Pipe_DataWidth_g - 1 downto 0) := "00000000";
	signal Pipe_Ready_o : std_logic := '0';
	signal Pipe_Data_o : std_logic_vector(Pipe_DataWidth_g - 1 downto 0) := "00000000";
	-- Pipe Sink
	signal PipeSink_DataWord_o : std_logic_vector(7 downto 0) := "00000000";
	signal PipeSink_Data_i : std_logic_vector(7 downto 0) := "00000000";
	signal PipeSink_Data_o : std_logic_vector(7 downto 0) := "00000000";

	--Inputs
	signal SA : std_logic_vector(7 downto 0) := (others => '0');
	signal SB : std_logic_vector(7 downto 0) := (others => '0');
	signal SC_IN : std_logic := '0';

 	--Outputs
	signal SS_O : std_logic_vector(7 downto 0) := (others => '0');
	signal SC_OUT : std_logic := '0';
	signal SOVERFLOW : std_logic := '0';
	-- No clocks detected in port list. Replace <clock> below with 
	-- appropriate port name 
 
	signal clock : std_logic := '0';
	constant clock_period : time := 10 ns;
 
begin
 
	Pipe_inst_bus: pipe_bus
	generic map
	(
		AddrWidth_g => 2,
		DataWidth_g => 8,
		InputFile_g => "../../../cosim/pipe/bus_sw2hw.txt",
		OutputFile_g => "../../../cosim/pipe/bus_hw2sw.txt"
	)
	port map
	(
		Clk_Bus_i => clock,
		Ready_i => Pipe_Ready_i,
		Addr_o => Pipe_Addr_i,
		DataA_o => Pipe_DataA_i,
		DataB_o => Pipe_DataB_i,
		Ready_o => Pipe_Ready_o,
		DataBus_i => Pipe_Data_o
	);

	--Pipe_inst_sink: pipe_sink
	--generic map
	--(
	--	DataWidth_g => PipeSink_DataWord_o'length,
	--	InFile_g => "../../../cosim/pipe/sink_sw2hw.txt",
	--	OutFile_g => "../../../cosim/pipe/sink_hw2sw.txt"
	--)
	--port map
	--(
	--	Clk_Pipe_i => clock,
	--	Data_i => PipeSink_Data_i, 
	--	DataSink_o => PipeSink_Data_o
	--);

	-- Instantiate the Unit Under Test (UUT)
	uut: bitadder PORT MAP (
	       A => Pipe_DataA_i,
	       B => Pipe_DataB_i,
	       C_IN => SC_IN,
	       S_O => Pipe_Data_o,
	       C_OUT => SC_OUT,
	       OVERFLOW => SOVERFLOW
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

		if clock'event and clock = '1' then
			-- insert stimulus here
		end if;
		
	end process;

end;
