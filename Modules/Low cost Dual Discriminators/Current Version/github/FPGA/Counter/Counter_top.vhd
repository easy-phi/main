library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity counter_top is
    Port (
        clk_10MHz : in std_logic;        -- board clock input 10MHz
		  --int_5Hz: out std_logic;           -- system interrupt at 5Hz
		  led: out std_logic_vector(7 downto 0);
		  input1: in std_logic
        --- SPI interface ---           
--        spi_ssel_o : out std_logic;                     -- spi port SSEL
--        spi_sck_o : out std_logic;                      -- spi port SCK
--        spi_mosi_o : out std_logic;                     -- spi port MOSI
--        spi_miso_o : out std_logic                     -- spi port MISO
        --- counter ---            
    );                      
end counter_top;

architecture rtl of counter_top is

signal clk_125MHz : std_logic; -- main system clock
signal clkn_125MHz : std_logic;
signal int_5Hz : std_logic;
signal toogle : std_logic;
signal toogle2 : std_logic;
signal div : STD_LOGIC_VECTOR(24 DOWNTO 0);
signal edge_found : std_logic;
--signal debug : std_logic;

begin

	DCM_clock_generator: entity work.DCM
		port map(
			CLKIN_IN => clk_10MHz,
			RST_IN => '0',
         CLKFX_OUT => clk_125MHz,
			CLKFX180_OUT => clkn_125MHz,
         CLKIN_IBUFG_OUT => open,
         CLK0_OUT => open, 
         LOCKED_OUT => open
			 );
	
	clock_divisor: entity work.c_counter_binary_v11_0
		port map(
			clk => clk_125MHz,
			thresh0 => int_5Hz,
			q => div
			);

	inst_edge_detect: entity work.edge_detect
		port map(
			clk => clk_125MHz,
			clkn => clkn_125MHz,
			test_signal => input1,
			rising_edge_found => edge_found
			);
			  
--    -- spi slave port: data and control signals driven by the slave fsm
--    Inst_spi_slave_port: entity work.spi_slave(rtl) 
--        generic map (N => N, CPOL => CPOL, CPHA => CPHA, PREFETCH => PREFETCH)
--        port map( 
--            clk_i => pclk_i,
--            spi_ssel_i => spi_ssel,             -- driven by the spi master
--            spi_sck_i => spi_sck,               -- driven by the spi master
--            spi_mosi_i => spi_mosi,             -- driven by the spi master
--            spi_miso_o => spi_miso,
--            di_req_o => spi_di_req_s,
--            di_i => spi_di_reg_s,
--            wren_i => spi_wren_reg_s,
--            wr_ack_o => spi_wr_ack_s,
--            do_valid_o => spi_do_valid_s,
--            do_o => spi_do_s,
--            ------------ debug pins ------------
--            state_dbg_o => spi_state_s          -- debug: internal state register
--        );                      

--		System_sampling: process (clk_125MHz) is
--         variable sampling_clk_counter : integer range 25000000-1 downto 0 := 0;
-- 	   begin
--			if rising_edge(clk_125MHz) then
--				if sampling_clk_counter = 25000000-1 then
--					int_5Hz <= '1';
--					sampling_clk_counter := 0;
--				else
--					int_5Hz <= '0';
--					sampling_clk_counter := sampling_clk_counter + 1;
--				end if;
--			end if;
--		end process System_sampling;
		
		LedUpdate: process (clk_125MHz) is
		begin
			if rising_edge(clk_125MHz) then
				if int_5Hz = '1' then
					toogle <= not toogle;
				end if;
			end if;
		end process LedUpdate;
		
		System_sampling: process (clk_125MHz) is
		begin
			if rising_edge(clk_125MHz) then
				if edge_found = '1' then
					toogle2 <= not toogle2;
				end if;
			end if;
		end process System_sampling;

		led(0) <= toogle;
		Led(1) <= toogle2;
		Led(2) <= input1;
		Led(3) <= '0';
		Led(4) <= '0';
		Led(5) <= '0';
		Led(6) <= '0';
		Led(7) <= '0';	

    --=============================================================================================
    --  CLOCK GENERATION
    --=============================================================================================
    -- All registers are clocked directly from the 10MHz system clock.
    -- The clock generation block derives 2 clock enable signals, divided down from the 100MHz input 
    -- clock. 
    --      input sample clock enable, 
    --      fsm clock enable,
    -----------------------------------------------------------------------------------------------
    -- generate the sampling clock enable from the 100MHz board input clock 
--    samp_ce_gen_proc: process (pclk_i) is
--        variable clk_cnt : integer range SAMP_CE_DIV-1 downto 0 := 0;
--    begin
--        if pclk_i'event and pclk_i = '1' then
--            if clk_cnt = SAMP_CE_DIV-1 then
--                samp_ce <= '1';                 -- generate a single pulse every SAMP_CE_DIV clocks
--                clk_cnt := 0;
--            else
--                samp_ce <= '0';
--                clk_cnt := clk_cnt + 1;
--            end if;
--        end if;
--    end process samp_ce_gen_proc;
--    -- generate the fsm clock enable from the 100MHz board input clock 
--    fsm_ce_gen_proc: process (pclk_i) is
--        variable clk_cnt : integer range FSM_CE_DIV-1 downto 0 := 0;
--    begin
--        if pclk_i'event and pclk_i = '1' then
--            if clk_cnt = FSM_CE_DIV-1 then
--                fsm_ce <= '1';                  -- generate a single pulse every FSM_CE_DIV clocks
--                clk_cnt := 0;
--            else
--                fsm_ce <= '0';
--                clk_cnt := clk_cnt + 1;
--            end if;
--        end if;
--    end process fsm_ce_gen_proc;


end rtl;

