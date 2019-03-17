-- Full Adder
-- Library
library IEEE;
use IEEE.std_logic_1164.all;

entity full_adder is
port(	
		a 	: in std_logic;
		b 	: in std_logic;
		cin 	: in std_logic;
		so 	: out std_logic;
		cout 	: out std_logic
);
end full_adder;

architecture Behavioral of full_adder is

signal a_plus_b, cx, cy : std_logic;

begin

-- XOR1
a_plus_b <= a xor b;
-- XOR2
so <= cin xor a_plus_b;
-- AND1
cx <= a and b;
-- AND2 
cy <= a_plus_b and cin;
-- OR1
cout <= cx or cy;

end Behavioral;
