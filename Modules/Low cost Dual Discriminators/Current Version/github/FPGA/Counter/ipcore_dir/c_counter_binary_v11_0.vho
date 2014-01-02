--------------------------------------------------------------------------------
--    This file is owned and controlled by Xilinx and must be used solely     --
--    for design, simulation, implementation and creation of design files     --
--    limited to Xilinx devices or technologies. Use with non-Xilinx          --
--    devices or technologies is expressly prohibited and immediately         --
--    terminates your license.                                                --
--                                                                            --
--    XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" SOLELY    --
--    FOR USE IN DEVELOPING PROGRAMS AND SOLUTIONS FOR XILINX DEVICES.  BY    --
--    PROVIDING THIS DESIGN, CODE, OR INFORMATION AS ONE POSSIBLE             --
--    IMPLEMENTATION OF THIS FEATURE, APPLICATION OR STANDARD, XILINX IS      --
--    MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION IS FREE FROM ANY      --
--    CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE FOR OBTAINING ANY       --
--    RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY       --
--    DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE   --
--    IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR          --
--    REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF         --
--    INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A   --
--    PARTICULAR PURPOSE.                                                     --
--                                                                            --
--    Xilinx products are not intended for use in life support appliances,    --
--    devices, or systems.  Use in such applications are expressly            --
--    prohibited.                                                             --
--                                                                            --
--    (c) Copyright 1995-2013 Xilinx, Inc.                                    --
--    All rights reserved.                                                    --
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
--    Generated from core with identifier:                                    --
--    xilinx.com:ip:c_counter_binary:11.0                                     --
--                                                                            --
--    The Xilinx LogiCORE Binary Counter creates up counters, down            --
--    counters, and up/down counters with output widths ranging up to 256     --
--    bits.  The upper count limit is user programmable, and the counter's    --
--    increment value can either be user-defined, or specified via an         --
--    external input port.                                                    --
--------------------------------------------------------------------------------

-- Interfaces:
--    sset_intf
--    clk_intf
--    sclr_intf
--    ce_intf
--    sinit_intf
--    up_intf
--    load_intf
--    l_intf
--    thresh0_intf
--    q_intf

-- The following code must appear in the VHDL architecture header:

------------- Begin Cut here for COMPONENT Declaration ------ COMP_TAG
COMPONENT c_counter_binary_v11_0
  PORT (
    clk : IN STD_LOGIC;
    thresh0 : OUT STD_LOGIC;
    q : OUT STD_LOGIC_VECTOR(24 DOWNTO 0)
  );
END COMPONENT;
-- COMP_TAG_END ------ End COMPONENT Declaration ------------

-- The following code must appear in the VHDL architecture
-- body. Substitute your own instance name and net names.

------------- Begin Cut here for INSTANTIATION Template ----- INST_TAG
your_instance_name : c_counter_binary_v11_0
  PORT MAP (
    clk => clk,
    thresh0 => thresh0,
    q => q
  );
-- INST_TAG_END ------ End INSTANTIATION Template ------------

-- You must compile the wrapper file c_counter_binary_v11_0.vhd when simulating
-- the core, c_counter_binary_v11_0. When compiling the wrapper file, be sure to
-- reference the XilinxCoreLib VHDL simulation library. For detailed
-- instructions, please refer to the "CORE Generator Help".

