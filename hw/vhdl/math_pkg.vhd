--
--	Package File Template
--
--	Purpose: This package defines supplemental types, subtypes, 
--		 constants, and functions 
--
--   To use any of the example code shown below, uncomment the lines and modify as necessary
--

library IEEE;
use IEEE.STD_LOGIC_1164.all;
use ieee.numeric_std.all;
use ieee.math_real.all;

package math_pkg is

	function saturate(ARG : unsigned; SIZE : natural) return unsigned;
	function saturate(ARG : signed; SIZE : natural) return signed;
	function saturate(ARG : unsigned; SIZE : natural) return signed;
	function saturate(ARG : signed; SIZE : natural) return unsigned;

	function truncate(ARG : unsigned; SIZE : natural) return signed;
	function truncate(ARG : signed; SIZE : natural) return unsigned;

	function ext(ARG: bit_vector; SIZE: natural) return bit_vector;

	function min(L, R : integer) return integer;
	function max(L, R : integer) return integer;

	-- Ceil log2
	function clog2(ARG : natural) return natural;
	-- Floor log2
	function flog2(ARG : natural) return natural;
	
	-- Ceil divide
	function cdiv(L, R : integer) return integer;
	-- Floor divide
	function fdiv(L, R : integer) return integer;

end math_pkg;

package body math_pkg is

	function min(L, R : integer) return integer is
		variable result : integer;
	begin
		if L > R then
			result := R;
		else
			result := L;
		end if;
		return result;
	end;

	function max(L, R : integer) return integer is
		variable result : integer;
	begin
		if L > R then
			result := L;
		else
			result := R;
		end if;
		return result;
	end;

	
	function truncate(ARG : unsigned; SIZE : natural) return signed is
		variable result_v : unsigned(SIZE - 1 downto 0);
	begin
		result_v := resize(ARG, SIZE);
		result_v(SIZE - 1) := '0';
		return signed(result_v);
	end function;
 
	function truncate(ARG : signed; SIZE : natural) return unsigned is
		variable result_v : unsigned(SIZE - 1 downto 0);
	begin
		if (ARG < 0) then
			result_v := to_unsigned(0, SIZE);
		else
			result_v := resize(unsigned(ARG), SIZE);
		end if;
		return result_v;
	end function;
 
	function saturate(ARG : unsigned; SIZE : natural) return unsigned is
		variable result_v : unsigned(SIZE - 1 downto 0);
	begin
		if ARG > 2**SIZE - 1 then
			result_v := to_unsigned(2**SIZE - 1, SIZE);
		else
			result_v := resize(ARG, SIZE);
		end if;
		return result_v;
	end function;

	function saturate(ARG : signed; SIZE : natural) return signed is
		variable result_v : signed(SIZE - 1 downto 0);
	begin
		if ARG > 2**(SIZE - 1) - 1 then
			result_v := to_signed(2**(SIZE - 1) - 1, SIZE);
		elsif ARG < -2**(SIZE - 1) then
			result_v := to_signed(-2**(SIZE - 1), SIZE);
		else
			result_v := resize(ARG, SIZE);
		end if;
		return result_v;
	end function;

	function saturate(ARG : unsigned; SIZE : natural) return signed is
		variable result_v : signed(SIZE - 1 downto 0);
	begin
		if result_v > 2**(SIZE - 1) - 1 then
			result_v := to_signed(2**(SIZE - 1) - 1, SIZE);
		else
			result_v := resize(signed(ARG), SIZE);
		end if;
		return result_v;
	end function;

	function saturate(ARG : signed; SIZE : natural) return unsigned is
		variable result_v : unsigned(SIZE - 1 downto 0);
	begin
		if ARG > 2**SIZE - 1 then
			result_v := to_unsigned(2**SIZE - 1, SIZE);
		elsif ARG < 0 then
			result_v := to_unsigned(0, SIZE);
		else
			result_v := resize(unsigned(ARG), SIZE);
		end if;
		return result_v;
	end function;
	
	-- Ceil log2
	function clog2(ARG : natural) return natural is
		variable tmp_v : real;
	begin
		tmp_v := ceil(log2(real(ARG)));
		return natural(tmp_v);
	end function;

	-- Floor log2
	function flog2(ARG : natural) return natural is 
		variable tmp_v : real;
	begin
		tmp_v := floor(log2(real(ARG)));
		return natural(tmp_v);
	end function;

	function ext(ARG: bit_vector; SIZE: natural) return bit_vector is
		variable rslt_v : bit_vector(SIZE - 1 downto 0);
	begin
		if ARG'length >= SIZE then
			rslt_v := ARG(SIZE - 1 downto 0);
		else
			rslt_v(ARG'length - 1 downto 0) := ARG;
			rslt_v(SIZE - 1 downto ARG'length) := (others => '0');
		end if;
		return rslt_v;
	end function;

	function cdiv(L, R : integer) return integer is
		variable tmp_v : real;
        begin
		tmp_v := real(L) / real(R);
		return integer(ceil(tmp_v));
	end function cdiv;

	function fdiv(L, R : integer) return integer is
		variable tmp_v : real;
        begin
		tmp_v := real(L) / real(R);
		return integer(floor(tmp_v));
	end function fdiv;

end math_pkg;
