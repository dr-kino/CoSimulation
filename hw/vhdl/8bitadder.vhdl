-- File bitadder.vhd
-- Library
library IEEE;
use IEEE.std_logic_1164.all;

-- Definitions
entity bitadder is
	port(
		A		: in  std_logic_vector(7 downto 0); 
		B		: in  std_logic_vector(7 downto 0);
		C_IN	: in  std_logic;
		S_O		: out std_logic_vector(7 downto 0);
		C_OUT	: out std_logic;
		OVERFLOW: out std_logic
	);
end bitadder;

architecture behavior of bitadder is

-- Component
component full_adder
	port(
		a		: in  std_logic;
		b		: in  std_logic;
		cin		: in  std_logic;
		so		: out std_logic;
		cout	: out std_logic
	);
end component;

-- Signals
signal CARRY_OUT : std_logic_vector(8 downto 0);

begin
CARRY_OUT(0) <= C_IN;

uGen1 : for i in 0 to 7 generate
	UFA0: full_adder port map(
		a => A(i), 
		b => B(i), 
		cin => CARRY_OUT(i), 
		so => S_O(i), 
		cout => CARRY_OUT(i+1)
	);	   
end generate uGen1;

C_OUT <= CARRY_OUT(8);
OVERFLOW <= CARRY_OUT(7) xor CARRY_OUT(8);

end behavior;
