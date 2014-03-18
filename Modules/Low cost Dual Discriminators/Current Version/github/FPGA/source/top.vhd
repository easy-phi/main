library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity top is port (
    SPI_MOSI    : in   std_logic;
    SPI_CS      : in   std_logic;
    SPI_CLK     : in   std_logic;
    SPI_MISO    : out  std_logic;
    CLK_10MHz   : in   std_logic;
    EXT_SYNC    : in   std_logic;
    FPGA_INT    : out  std_logic;
    CH2_IN      : in   std_logic;
    CH1_IN      : in   std_logic;
    CH1_ACDC    : out  std_logic;
    CH2_ACDC    : out  std_logic;
    CH2_BIAS    : out  std_logic;
    CH1_BIAS    : out  std_logic;
    FPGA_OUT4   : out  std_logic;
    FPGA_OUT3   : out  std_logic;
    FPGA_OUT2   : out  std_logic;
    FPGA_OUT1   : out  std_logic;
    LED1_1      : out  std_logic;
    LED1_2      : out  std_logic;
    LED1_3      : out  std_logic;
    LED2_1      : out  std_logic;
    LED2_2      : out  std_logic;
    LED2_3      : out  std_logic;
    LED3_1      : out  std_logic;
    LED3_2      : out  std_logic;
    LED3_3      : out  std_logic;
    LED4_1      : out  std_logic;
    LED4_2      : out  std_logic;
    LED4_3      : out  std_logic;
    LED5_1      : out  std_logic;
    LED5_2      : out  std_logic;
    LED5_3      : out  std_logic;
    LED6_1      : out  std_logic;
    LED6_2      : out  std_logic;
    LED6_3      : out  std_logic;
    SYNC2_IN    : in   std_logic;
    SYNC4_IN    : in   std_logic;
    SYNC3_IN    : in   std_logic;
    SYNC2_OUT   : out  std_logic;
    SYNC4_OUT   : out  std_logic;
    SYNC3_OUT   : out  std_logic);
end top;

architecture Behave of top is

  component clk_man is port ( 
    CLKIN_IN        : in    std_logic; 
    CLKFX_OUT       : out   std_logic; 
    CLK0_OUT        : out   std_logic; 
    LOCKED_OUT      : out   std_logic);
  end component;

  component chipscope_icon PORT (
    CONTROL0 : INOUT STD_LOGIC_VECTOR(35 DOWNTO 0));
  end component;

  component chipscope_ila  PORT (
    CONTROL : INOUT STD_LOGIC_VECTOR(35 DOWNTO 0);
    CLK : IN STD_LOGIC;
    TRIG0 : IN STD_LOGIC_VECTOR(79 DOWNTO 0));
  end component;


  signal CONTROL0 : STD_LOGIC_VECTOR(35 DOWNTO 0);
  signal TRIG0    : STD_LOGIC_VECTOR(79 DOWNTO 0);
  
  signal Clock_250 : std_logic;
  signal Clock_10 :  std_logic;
  signal DCM_Locked : std_logic;
  signal Led_test : std_logic;
  
  signal SPI_MOSI_Sync1_s      : std_logic;
  signal SPI_CS_Sync1_s        : std_logic;
  signal SPI_CLK_Sync1_s       : std_logic;
  signal SPI_MOSI_Sync2_s      : std_logic;
  signal SPI_CS_Sync2_s        : std_logic;
  signal SPI_CLK_Sync2_s       : std_logic;

  signal SPI_Data_Out_s       : std_logic_vector(15 downto 0);
  signal SPI_Data_In_We_s     : std_logic;
  signal SPI_Data_In_s        : std_logic_vector(15 downto 0);
  signal SPI_Data_Shift_Out_s : std_logic_vector(15 downto 0);
  signal SPI_Data_Shift_In_s  : std_logic_vector(15 downto 0);
  signal SPI_Address_s  : std_logic_vector(7 downto 0);
  --type   SPI_State_type is (command, write_data, read_data);
  --signal SPI_State  : SPI_State_type := command;
  signal SPI_State  : std_logic;
  
  signal Reset_s        : std_logic;
  signal CLT_INT_s      : std_logic;
  signal FPGA_INT_s     : std_logic;
  signal Channel_1_s             : std_logic;
  signal Channel_2_s             : std_logic;
  signal Channel_1_sync_s        : std_logic;
  signal Channel_2_sync_s        : std_logic;
  signal Channel_1_Cnt_s         : std_logic_vector(25 downto 0);
  signal Channel_2_Cnt_s         : std_logic_vector(25 downto 0);
  signal Channel_1_Cnt_stored_s  : std_logic_vector(25 downto 0);
  signal Channel_2_Cnt_stored_s  : std_logic_vector(25 downto 0);
  
  signal Time_Cnt_s        : std_logic_vector(31 downto 0);
  signal Time_Trig_s       : std_logic;
  signal Time_Trig_Sync_s  : std_logic;
  
  signal Relay_Data_s  : std_logic_vector(15 downto 0);

  signal Sync_Cnt_s  : std_logic_vector(25 downto 0);
  signal Sync_Value_s  : std_logic_vector(2 downto 0);
  
  signal Channel_1_change_s : std_logic;
  signal Channel_2_change_s : std_logic;
  signal LED1_all_s         : std_logic;
  signal LED2_all_s         : std_logic;
  signal LED3_all_s         : std_logic;
  signal LED4_all_s         : std_logic;
  signal LED5_all_s         : std_logic;
  signal LED6_all_s         : std_logic;
  signal LED_trig_sync_s    : std_logic;
  signal LED_Cnt_s          : std_logic_vector(31 downto 0);
  signal LED_Trig_s         : std_logic;

  signal Test_mode_s         : std_logic;

  signal Test_mode_cnt_s  : std_logic_vector(1 downto 0);
  signal LED_1_testm_s   : std_logic;
  signal LED_2_testm_s   : std_logic;
  signal LED_3_testm_s   : std_logic;
  signal CH1_ACDC_testm_s : std_logic;
  signal CH2_ACDC_testm_s : std_logic;
  signal CH1_BIAS_testm_s : std_logic;
  signal CH2_BIAS_testm_s : std_logic;


begin

  LED1_1    <= LED_1_testm_s   when Test_mode_s = '1' else '1';
  LED1_2    <= LED_2_testm_s   when Test_mode_s = '1' else '1';
  LED1_3    <= LED_3_testm_s   when Test_mode_s = '1' else LED1_all_s;
  LED2_1    <= LED_1_testm_s   when Test_mode_s = '1' else '1';
  LED2_2    <= LED_2_testm_s   when Test_mode_s = '1' else LED2_all_s;
  LED2_3    <= LED_3_testm_s   when Test_mode_s = '1' else '1';
  LED3_1    <= LED_1_testm_s   when Test_mode_s = '1' else '1';
  LED3_2    <= LED_2_testm_s   when Test_mode_s = '1' else LED3_all_s;
  LED3_3    <= LED_3_testm_s   when Test_mode_s = '1' else '1';
  LED4_1    <= LED_1_testm_s   when Test_mode_s = '1' else '1';
  LED4_2    <= LED_2_testm_s   when Test_mode_s = '1' else '1';
  LED4_3    <= LED_3_testm_s   when Test_mode_s = '1' else LED4_all_s;
  LED5_1    <= LED_1_testm_s   when Test_mode_s = '1' else '1';
  LED5_2    <= LED_2_testm_s   when Test_mode_s = '1' else LED5_all_s;
  LED5_3    <= LED_3_testm_s   when Test_mode_s = '1' else '1';
  LED6_1    <= LED_1_testm_s   when Test_mode_s = '1' else '1';
  LED6_2    <= LED_2_testm_s   when Test_mode_s = '1' else LED6_all_s;
  LED6_3    <= LED_3_testm_s   when Test_mode_s = '1' else '1';
  CH1_ACDC  <= CH1_ACDC_testm_s when Test_mode_s = '1' else Relay_Data_S(0);
  CH2_ACDC  <= CH2_ACDC_testm_s when Test_mode_s = '1' else Relay_Data_S(1);
  CH1_BIAS  <= CH1_BIAS_testm_s when Test_mode_s = '1' else Relay_Data_S(2);
  CH2_BIAS  <= CH2_BIAS_testm_s when Test_mode_s = '1' else Relay_Data_S(3);


--  SYNC2_OUT <= 'Z';
--  SYNC4_OUT <= 'Z';
--  SYNC3_OUT <= 'Z';
	 SYNC2_OUT <= Sync_Value_s(0);
    SYNC4_OUT <= Sync_Value_s(1);
    SYNC3_OUT <= Sync_Value_s(2);

  FPGA_OUT4 <= CH2_IN;
  FPGA_OUT3 <= CH2_IN;
  FPGA_OUT2 <= CH1_IN;
  FPGA_OUT1 <= CH1_IN;


  process (Clock_10, Reset_s) ---- Trigger counter
  begin
    if Reset_s = '1' then
        Sync_Cnt_s   <= (others => '0');
        Sync_Value_s <= (others => '0');
    elsif Clock_10'event and Clock_10 = '1' then
        Sync_Cnt_s   <= std_logic_vector(unsigned(Sync_Cnt_s) + 1);
		  if Sync_Cnt_s(25 downto 0) = "00000000000000000000000000" then Sync_Value_s(0) <= not Sync_Value_s(0); end if;
		  if Sync_Cnt_s(24 downto 0) = "0000000000000000000000000"  then Sync_Value_s(1) <= not Sync_Value_s(1); end if;
		  if Sync_Cnt_s(23 downto 0) = "000000000000000000000000"   then Sync_Value_s(2) <= not Sync_Value_s(2); end if;
	 
    end if;
  end process;    

 
  process (Clock_250, Reset_s) 
  begin
    if Reset_s = '1' then
      Channel_1_s      <= '0';
      Channel_2_s      <= '0';
      Channel_1_sync_s <= '0';
      Channel_2_sync_s <= '0';
      Channel_1_Cnt_s  <= (others => '0');
      Channel_2_Cnt_s  <= (others => '0');
      Time_Trig_Sync_s <= '0';
      FPGA_INT_s       <= '0';
    elsif Clock_250'event and Clock_250 = '1' then
      Channel_1_s      <= CH1_IN;
      Channel_2_s      <= CH2_IN;
      Channel_1_sync_s <= Channel_1_s;
      Channel_2_sync_s <= Channel_2_s;
      if (Channel_1_sync_s = '0') and (Channel_1_s = '1') then Channel_1_Cnt_s <= std_logic_vector(unsigned(Channel_1_Cnt_s) + 1); end if;
      if (Channel_2_sync_s = '0') and (Channel_2_s = '1') then Channel_2_Cnt_s <= std_logic_vector(unsigned(Channel_2_Cnt_s) + 1); end if;
      
      Time_Trig_Sync_s  <= Time_Trig_s;

      if (Time_Trig_Sync_s = '0') and (Time_Trig_s = '1') then
          Channel_1_Cnt_stored_s  <= Channel_1_Cnt_s;
          Channel_2_Cnt_stored_s  <= Channel_2_Cnt_s;
      end if;
      
      if (CLT_INT_s = '1') then   
          FPGA_INT_s <= '0';
      elsif (Time_Trig_Sync_s = '0') and (Time_Trig_s = '1') then
          FPGA_INT_s <= '1';
      end if;
    end if;
  end process;    

----------------LED CONTROL
  process (Clock_250, Reset_s) 
  begin
    if Reset_s = '1' then
      Channel_1_change_s  <= '0';
      Channel_2_change_s  <= '0';
	  LED1_all_s          <= '1';
	  LED2_all_s          <= '1';
	  LED3_all_s          <= '1';
	  LED4_all_s          <= '1';
	  LED5_all_s          <= '1';
	  LED6_all_s          <= '1';
      LED_trig_sync_s     <= '0';
    elsif Clock_250'event and Clock_250 = '1' then
      LED_trig_sync_s  <= LED_trig_s;
      if (LED_trig_sync_s = '0') and (LED_trig_s = '1') then
        Channel_1_change_s  <= '0';
        Channel_2_change_s  <= '0';
		if Channel_1_change_s = '1' then 
	      LED1_all_s  <= not(LED1_all_s);
		  LED2_all_s  <= not(LED2_all_s);
		  LED3_all_s  <= not(LED3_all_s);
        elsif Channel_1_sync_s = '0' then
	      LED1_all_s  <= '1';
		  LED2_all_s  <= '1';
		  LED3_all_s  <= '1';
        else
	      LED1_all_s  <= '1';
		  LED2_all_s  <= '1';
		  LED3_all_s  <= '1';
        end if;
		if Channel_2_change_s = '1' then 
	      LED4_all_s  <= not(LED4_all_s);
		  LED5_all_s  <= not(LED5_all_s);
		  LED6_all_s  <= not(LED6_all_s);
        elsif Channel_2_sync_s = '0' then
	      LED4_all_s  <= '1';
		  LED5_all_s  <= '1';
		  LED6_all_s  <= '1';
        else
	      LED4_all_s  <= '1';
		  LED5_all_s  <= '1';
		  LED6_all_s  <= '1';
        end if;

      else
		    if (Channel_1_sync_s /= Channel_1_s) then Channel_1_change_s  <= '1';end if;
		    if (Channel_2_sync_s /= Channel_2_s) then Channel_2_change_s  <= '1';end if;
      end if;
    end if;
  end process;    

  
  FPGA_INT <= FPGA_INT_s;

  process (Clock_10, Reset_s) ---- Trigger counter
  begin
    if Reset_s = '1' then
      Time_Cnt_s      <= (others => '0');
      Time_Trig_s     <= '0';
      LED_Cnt_s       <= (others => '0');
      LED_Trig_s      <= '0';
    elsif Clock_10'event and Clock_10 = '1' then
      Time_Trig_s      <= '0';
      Time_Cnt_s <= std_logic_vector(unsigned(Time_Cnt_s) + 1);
      LED_Trig_s      <= '0';
      LED_Cnt_s <= std_logic_vector(unsigned(LED_Cnt_s) + 1);
      if unsigned(Time_Cnt_s) = 2500000 then
          Time_Trig_s      <= '1';
          Time_Cnt_s       <= (others => '0');
      end if;
      if unsigned(LED_Cnt_s) = 500000 then
          LED_Trig_s      <= '1';
          LED_Cnt_s       <= (others => '0');
      end if;
    end if;
  end process;    
  
  process (Clock_10) -----  SPI serializing Process 
  begin
    if Clock_10'event and Clock_10 = '1' then
        SPI_Data_In_We_s <= '0';
        SPI_MOSI_Sync1_s <= SPI_MOSI;
        SPI_CLK_Sync1_s  <= SPI_CLK; 
        SPI_CS_Sync1_s   <= SPI_CS;  
        SPI_MOSI_Sync2_s <= SPI_MOSI_Sync1_s;
        SPI_CLK_Sync2_s  <= SPI_CLK_Sync1_s ;
        SPI_CS_Sync2_s   <= SPI_CS_Sync1_s  ;
        if SPI_CS_Sync1_s = '0' then       
            if (SPI_CLK_Sync2_s = '0') and (SPI_CLK_Sync1_s = '1') then  ---positive edge
                SPI_Data_Shift_Out_s(15 downto 1) <= SPI_Data_Shift_Out_s(14 downto 0);
            end if;     
         
            if (SPI_CLK_Sync2_s = '1') and (SPI_CLK_Sync1_s = '0') then  ---negative edge
                SPI_Data_Shift_In_s(0) <= SPI_MOSI_Sync1_s;
                SPI_Data_Shift_In_s(15 downto 1) <= SPI_Data_Shift_In_s(14 downto 0);
            end if;     
        end if;               
        if (SPI_CS_Sync2_s = '0') and (SPI_CS_Sync1_s = '1') then  ---positive edge read data available
             SPI_Data_In_s <= SPI_Data_Shift_In_s;
             SPI_Data_In_We_s <= '1';
        end if;     

        if (SPI_CS_Sync2_s = '1') and (SPI_CS_Sync1_s = '0') then  ---negative edge load write data
             SPI_Data_Shift_Out_s <= SPI_Data_Out_s;
        end if;     
    end if;
  end process;     

  SPI_MISO <= SPI_Data_Shift_Out_s(15) when SPI_CS_Sync1_s = '0' else 'Z';  
  
  process (Clock_10) -----  SPI decoding Process 
  begin
    if Clock_10'event and Clock_10 = '1' then
        Reset_s   <= '0';
        CLT_INT_s <= '0';   
        if (SPI_Data_In_We_s = '1') then
            case SPI_State is 
                when '0' => -- command =>  
                    if SPI_Data_In_s(15 downto 9) /= "0000000" then -- msb are used to send direct command
                        if SPI_Data_In_s(15) = '1' then
                            Reset_s <= '1';
                        elsif SPI_Data_In_s(14) = '1' then
                            CLT_INT_s <= '1';   
                        end if;    
                    elsif SPI_Data_In_s(8) = '1' then --write enable active --> write command
                        SPI_State     <= '1'; --write_data;
                        SPI_Address_s <= SPI_Data_In_s(7 downto 0);
                    else                             ----write enable inactive --> read command
                        case SPI_Data_In_s(7 downto 0) is  ------------START READ ADDRESS MAPPING---------------
                            when "00000000" => SPI_Data_Out_s <= Channel_1_Cnt_stored_s(15 downto 0);
                            when "00000001" => SPI_Data_Out_s <= "0000000" & Channel_1_Cnt_stored_s(24 downto 16);
                            when "00000010" => SPI_Data_Out_s <= Channel_2_Cnt_stored_s(15 downto 0);
                            when "00000011" => SPI_Data_Out_s <= "0000000" & Channel_2_Cnt_stored_s(24 downto 16);
                            when "00000100" => SPI_Data_Out_s <= Relay_Data_s;
                            when "00000101" => SPI_Data_Out_s <= "000000000000000" & Test_mode_s;
                            when others     => SPI_Data_Out_s <= x"aaaa";
                        end case;                          ------------END READ ADDRESS MAPPING---------------
                    end if;    
                when '1' => --write_data =>  
                    SPI_State <= '0'; --command;
                    if SPI_Data_In_s(15 downto 9) /= "0000000" then -- msb are used to send direct command
                        if SPI_Data_In_s(15) = '1' then
                            Reset_s <= '1';
                        elsif SPI_Data_In_s(14) = '1' then
                            CLT_INT_s <= '1';   
                        end if;    
                    else
                        case SPI_Address_s is                ------------START WRITE ADDRESS MAPPING---------------
                            ---when "00000000" => ----read only
                            ---when "00000001" => ----read only
                            ---when "00000010" => ----read only
                            ---when "00000011" => ----read only
                            when "00000100" => Relay_Data_s <= SPI_Data_In_s;
                            when "00000101" => Test_mode_s <= SPI_Data_In_s(0);
                            when others => null;
                        end case;                            ------------END WRITE ADDRESS MAPPING---------------
                    end if;    
                when others =>      
                    SPI_State <= '0'; --command;
            end case;        
        end if;
    end if;
  end process;     


  clk_man_inst : clk_man port map ( 
    CLKIN_IN        => CLK_10MHz,
    CLKFX_OUT       => Clock_250,
    CLK0_OUT        => Clock_10,
    LOCKED_OUT      => DCM_Locked);

    
    
  process (Clock_10, Reset_s) ---- test mide
  begin
    if Reset_s = '1' then
        Test_mode_cnt_s  <= (others => '0');
        LED_1_testm_s   <= '1';
        LED_2_testm_s   <= '1';
        LED_3_testm_s   <= '1';
        CH1_ACDC_testm_s <= '0';
        CH2_ACDC_testm_s <= '0';
        CH1_BIAS_testm_s <= '0';
        CH2_BIAS_testm_s <= '0';
    
    elsif Clock_10'event and Clock_10 = '1' then
      if Time_Trig_s = '1' then
        Test_mode_cnt_s  <= std_logic_vector(unsigned(Test_mode_cnt_s) + 1);
        case Test_mode_cnt_s is    ------- Test leds
            when "00"   =>
                LED_1_testm_s   <= '0';
                LED_2_testm_s   <= '1';
                LED_3_testm_s   <= '1';
            when "01"   =>
                LED_1_testm_s   <= '1';
                LED_2_testm_s   <= '0';
                LED_3_testm_s   <= '1';
            when "10"   =>
                LED_1_testm_s   <= '1';
                LED_2_testm_s   <= '1';
                LED_3_testm_s   <= '0';
            when others =>
                LED_1_testm_s   <= '1';
                LED_2_testm_s   <= '1';
                LED_3_testm_s   <= '1';
        end case;


        case Test_mode_cnt_s is    ------- Test relays
            when "00"   =>  CH1_ACDC_testm_s <= not(CH1_ACDC_testm_s);
            when "01"   =>  CH2_ACDC_testm_s <= not(CH2_ACDC_testm_s);
            when "10"   =>  CH1_BIAS_testm_s <= not(CH1_BIAS_testm_s);
            when others =>  CH2_BIAS_testm_s <= not(CH2_BIAS_testm_s);
        end case;

      end if;
    end if;
  end process;    
    
    
    
    
  chipscope_icon_inst : chipscope_icon port map (
    CONTROL0 => CONTROL0);

  chipscope_ila_inst : chipscope_ila port map (
    CONTROL => CONTROL0,
    CLK => Clock_10,
    TRIG0 => TRIG0);
 
  TRIG0(0) <= SPI_MOSI_Sync1_s; 
  TRIG0(1) <= SPI_CLK_Sync1_s; 
  TRIG0(2) <= SPI_CS_Sync1_s; 
  TRIG0(18 downto 3) <= SPI_Data_Shift_Out_s; 
  TRIG0(34 downto 19) <= SPI_Data_Shift_In_s; 
  TRIG0(35) <= CLT_INT_s;
  TRIG0(51 downto 36) <= Relay_Data_s; 
  TRIG0(52) <= SPI_Data_In_We_s;
  TRIG0(53) <= FPGA_INT_s;
  TRIG0(54) <= DCM_Locked;
  TRIG0(55) <= Time_Trig_s;
  
  
  TRIG0(57 downto 56) <=    Test_mode_cnt_s ;
  TRIG0(58) <=               LED_1_testm_s   ;
  TRIG0(59) <=               LED_2_testm_s   ;
  TRIG0(60) <=               LED_3_testm_s   ;
  TRIG0(61) <=               CH1_ACDC_testm_s;
  TRIG0(62) <=               CH2_ACDC_testm_s;
  TRIG0(63) <=               CH1_BIAS_testm_s;
  TRIG0(64) <=               CH2_BIAS_testm_s;
  TRIG0(65) <=               Test_mode_s;

  
  TRIG0(79 downto 66) <= (others => '0'); 


  
  
  
end Behave;

