-- File 8bitadder.vhd
-- Library
library IEEE;
use IEEE.std_logic_1164.all;

-- Definitions
entity 8bitadder is
	port(
		A		: in  std_logic_vector(7 downto 0); 
		B		: in  std_logic_vector(7 downto 0);
		C_IN	: in  std_logic;
		S		: out std_logic_vector(7 downto 0);
		C_OUT	: out std_logic;
		OUTPUT	: out std_logic
	);
end 8bitadder;

architecture behavior of 8bitadder is

-- Component
component full_adder
	port(
		a		: in  std_logic;
		b		: in  std_logic;
		cin		: in  std_logic;
		s		: out std_logic;
		cout	: out std_logic
	);
end component;

-- Signals
signal CARRY_OUT : std_logic_vector(8 downto 0);

begin
CARRY_OUT(0) <= C_IN;

adderGen : for i in 0 to 7 generate
	UFullAdder: full_adder port map(
		a => A(i) , b => B(i), cin => C_IN(i), s => S(i), cout => CARRY_OUT(i+1)
	);	   

	C_OUT <= CARRY_OUT(8);
	V <= CARRY_OUT(7) xor CARRY_OUT(8);

end behavior;
