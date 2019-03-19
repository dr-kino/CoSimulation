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
