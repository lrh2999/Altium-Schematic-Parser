--------------------------------------------------------------------------------
-- Copyright (c) 1995-2010 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 12.1
--  \   \         Application : xaw2vhdl
--  /   /         Filename : SdramBoardDeskew.vhd
-- /___/   /\     Timestamp : 07/19/2010 11:13:08
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: xaw2vhdl-st C:\\Users\Public\Documents\Altium\AD 10.0.0.20501\Examples\Soft Designs\FPGA Hardware\Clocking\Xilinx\.\SdramBoardDeskew.xaw C:\\Users\Public\Documents\Altium\AD 10.0.0.20501\Examples\Soft Designs\FPGA Hardware\Clocking\Xilinx\.\SdramBoardDeskew
--Design Name: SdramBoardDeskew
--Device: xc3s1400an-5fgg676
--
-- Module SdramBoardDeskew
-- Generated by Xilinx Architecture Wizard
-- Written for synthesis tool: XST

library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
library UNISIM;
use UNISIM.Vcomponents.ALL;

entity SdramBoardDeskew is
   port ( CLKFB_IN   : in    std_logic; 
          CLKIN_IN   : in    std_logic; 
          RST_IN     : in    std_logic; 
          CLK0_OUT   : out   std_logic; 
          LOCKED_OUT : out   std_logic);
end SdramBoardDeskew;

architecture BEHAVIORAL of SdramBoardDeskew is
   signal CLKFB_IBUFG : std_logic;
   signal CLK0_BUF    : std_logic;
   signal GND_BIT     : std_logic;
begin
   GND_BIT <= '0';
   CLKFB_IBUFG_INST : IBUFG
      port map (I=>CLKFB_IN,
                O=>CLKFB_IBUFG);
   
   CLKOUT_BUFG_INST : BUFG
      port map (I=>CLK0_BUF,
                O=>CLK0_OUT);
   
   DCM_DESKEW_SP_INST : DCM_SP
   generic map( CLK_FEEDBACK => "1X",
            CLKDV_DIVIDE => 2.0,
            CLKFX_DIVIDE => 1,
            CLKFX_MULTIPLY => 4,
            CLKIN_DIVIDE_BY_2 => FALSE,
            CLKIN_PERIOD => 12.500,
            CLKOUT_PHASE_SHIFT => "NONE",
            DESKEW_ADJUST => "SYSTEM_SYNCHRONOUS",
            DFS_FREQUENCY_MODE => "LOW",
            DLL_FREQUENCY_MODE => "LOW",
            DUTY_CYCLE_CORRECTION => TRUE,
            FACTORY_JF => x"C080",
            PHASE_SHIFT => 0,
            STARTUP_WAIT => FALSE)
      port map (CLKFB=>CLKFB_IBUFG,
                CLKIN=>CLKIN_IN,
                DSSEN=>GND_BIT,
                PSCLK=>GND_BIT,
                PSEN=>GND_BIT,
                PSINCDEC=>GND_BIT,
                RST=>RST_IN,
                CLKDV=>open,
                CLKFX=>open,
                CLKFX180=>open,
                CLK0=>CLK0_BUF,
                CLK2X=>open,
                CLK2X180=>open,
                CLK90=>open,
                CLK180=>open,
                CLK270=>open,
                LOCKED=>LOCKED_OUT,
                PSDONE=>open,
                STATUS=>open);
   
end BEHAVIORAL;


