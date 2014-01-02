----------------------------------------------------------------------------------
-- Company: 
-- Engineer: Mike Field <hamster@snap.net.nz>
-- 
-- Description: Uses a DDR input to reliably detect a rising edge at something like
--              1/2 clk frequency. Also syncronises the signal to the CLK domain
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VComponents.all;

entity edge_detect is
    Port ( clk : in  STD_LOGIC;
           clkn : in  STD_LOGIC;
           test_signal : in  STD_LOGIC;
           rising_edge_found : out  STD_LOGIC);
end edge_detect;

architecture Behavioral of edge_detect is
   signal raw : std_logic_vector(2 downto 0);
   signal cooked : std_logic_vector(2 downto 0);
begin

   IDDR2_inst : IDDR2
   generic map(
      DDR_ALIGNMENT => "NONE", INIT_Q0 => '0', INIT_Q1 => '0', SRTYPE => "SYNC")
   port map (
      Q0 => raw(0), Q1 => raw(1), 
      C0 => clkn,    C1 => clk, 
      CE => '1',    D => test_signal,  
      R => '0',     S => '0'     
   );

process(clk)
   begin
      if rising_edge(clk) then
         if cooked(2 downto 1) = "01" or cooked(1 downto 0) = "01" then
            rising_edge_found <= '1';
         else
            rising_edge_found <= '0';
         end if;
         cooked <= raw;
         raw(2) <= raw(0);
      end if;
   end process;

end Behavioral;