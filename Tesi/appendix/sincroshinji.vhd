library ieee;
use ieee.std_logic_1164.all;

entity sincroshinji is
  
  generic (
    par01 : integer;
    par02 : integer;
    par03 : integer;
    par04 : integer;
    par05 : integer;
    par06 : integer;
    par07 : integer;
    par08 : integer;
    par09 : integer);

  port (
    ck               : in  std_logic;
    reset            : in  std_logic;
    datoI            : in  std_logic_vector(par01 downto 1);
    datoQ            : in  std_logic_vector(par01 downto 1);
    inizialeLFSR     : in  std_logic_vector(par09 downto 1);
    sequenzaLFSR1    : in  std_logic_vector(par09 downto 1);
    sequenzaLFSR2    : in  std_logic_vector(par09 downto 1);
    nu1              : in  std_logic;
    nu2              : in  std_logic;
    inizialeCNT1     : in  std_logic_vector(par08 downto 1);
    inizialeCNT2     : in  std_logic_vector(par08 downto 1);
    inizialeCNT3a    : in  std_logic_vector(par08 downto 1);
    inizialeCNT3b    : in  std_logic_vector(par08 downto 1);
    inizialeCNT4     : in  std_logic_vector(par08 downto 1);
    inizialeSuperCNT : in  std_logic_vector(par07 downto 1);
    chipout          : out std_logic;
    fine             : out std_logic);

end entity sincroshinji;


architecture structural of sincroshinji is

  component cuparametrica
    port (
      ck             : in  std_logic;
      reset          : in  std_logic;
      iszero1        : in  std_logic;
      iszero2        : in  std_logic;
      iszero3        : in  std_logic;
      iszero4        : in  std_logic;
      iszero5        : in  std_logic;
      iszero7        : in  std_logic;
      curGold        : in  std_logic;
      zeroSuper      : in  std_logic;
      newmax         : in  std_logic;
      resetGold1     : out std_logic;
      resetGold2     : out std_logic;
      resetGold3     : out std_logic;
      chooseGold     : out std_logic;
      transitorio    : out std_logic;
      enableGold     : out std_logic;
      enableGold1a   : out std_logic;
      enableGold1b   : out std_logic;
      enableGold2a   : out std_logic;
      enableGold2b   : out std_logic;
      enableGold3    : out std_logic;
      chooseEnable   : out std_logic;
      chooseIniziale : out std_logic;
      resetDesp      : out std_logic;
      enableDesp     : out std_logic;
      resetAcc       : out std_logic;
      clearCount1    : out std_logic;
      clearCount2    : out std_logic;
      enableAcc1     : out std_logic;
      enableAcc2     : out std_logic;
      enableSq       : out std_logic;
      enableSUM      : out std_logic;
      enableCMP      : out std_logic;
      loadMAX        : out std_logic;
      loadCNT1       : out std_logic;
      loadCNT2       : out std_logic;
      loadCNT3       : out std_logic;
      selCNT3        : out std_logic;
      loadCNT4a      : out std_logic;
      loadCNT4b      : out std_logic;
      loadCNT5a      : out std_logic;
      loadCNT5b      : out std_logic;
      loadCNT7       : out std_logic;
      loadSuperCNT   : out std_logic;
      enableCNT1     : out std_logic;
      enableCNT2     : out std_logic;
      enableCNT3     : out std_logic;
      enableCNT4     : out std_logic;
      enableCNT5     : out std_logic;
      enableCNT7     : out std_logic;
      enableSuperCNT : out std_logic;
      resetspecial   : out std_logic;
      finito         : out std_logic);
  end component cuparametrica;

  component myor
    port (
      dato1  : in  std_logic;
      dato2  : in  std_logic;
      uscita : out std_logic);
  end component myor;

  component myor3
    port (
      dato1  : in  std_logic;
      dato2  : in  std_logic;
      dato3  : in  std_logic;
      uscita : out std_logic);
  end component myor3;
  
  component abilitaGold
    port (
      orologio     : in std_logic;
      azzera       : in std_logic;
      enableGold1  : in std_logic;
      enableGold2  : in std_logic;
      nuis1        : in std_logic;
      nuis2        : in std_logic;
      abilitazione : out std_logic);
  end component abilitaGold;

  component gold
    generic (
      goldlungh : integer);
    port (
      gck      : in  std_logic;
      greset   : in  std_logic;
      en       : in  std_logic;
      ini1     : in  std_logic_vector(goldlungh downto 1);
      ini2     : in  std_logic_vector(goldlungh downto 1);
      seq1     : in  std_logic_vector(goldlungh downto 1);
      seq2     : in  std_logic_vector(goldlungh downto 1);
      cur1     : out std_logic_vector(goldlungh downto 1);
      cur2     : out std_logic_vector(goldlungh downto 1);
      goldchip : out std_logic);
  end component gold;

  component toggler
    port (
      ck     : in  std_logic;
      reset  : in  std_logic;
      toggle : in  std_logic;
      dato   : out std_logic);
  end component toggler;

  component nflipflop
    port (
      ck    : in  std_logic;
      reset : in  std_logic;
      din   : in  std_logic;
      dout  : out std_logic);
  end component nflipflop;
  
  component mpx
    generic (
      lungh : integer);
    port (
      dato1    : in  std_logic_vector(lungh downto 1);
      dato2    : in  std_logic_vector(lungh downto 1);
      sel      : in  std_logic;
      selected : out std_logic_vector(lungh downto 1));
  end component mpx;

  component despreader
    generic (
      lungh : integer);
    port (
      dato       : in  std_logic_vector(lungh downto 1);
      chip       : in  std_logic;
      ck         : in  std_logic;
      reset      : in  std_logic;
      enable     : in  std_logic;
      despreaded : out std_logic_vector(lungh downto 1));
  end component despreader;

  component accumulatore
    generic (
      lunghin  : integer;
      lunghout : integer);
    port (
      ck      : in  std_logic;
      reset   : in  std_logic;
      clrcnt  : in  std_logic;
      enable  : in  std_logic;
      dato    : in  std_logic_vector(lunghin downto 1);
      sommato : out std_logic_vector(lunghout downto 1));
  end component accumulatore;

  component squaring
    generic (
      lunghin  : integer;                -- lunghout
      lunghout : integer);
    port (
      ck      : in  std_logic;
      reset   : in  std_logic;
      enable  : in  std_logic;
      dato    : in  std_logic_vector(lunghin downto 1);
      squared : out std_logic_vector(lunghout downto 1));
  end component squaring;

  component sommatore
    generic (
      lunghin  : integer;
      lunghout : integer);
    port (
      ck      : in  std_logic;
      reset   : in  std_logic;
      enable  : in  std_logic;
      dato1   : in  std_logic_vector(lunghin downto 1);
      dato2   : in  std_logic_vector(lunghin downto 1);
      sommato : out std_logic_vector(lunghout downto 1));
  end component sommatore;

  component sottratoreconenable
    generic (
      lungh : integer);
    port (
      ck     : in  std_logic;
      reset  : in  std_logic;
      enable : in  std_logic;
      dato1  : in  std_logic_vector(lungh downto 1);
      dato2  : in  std_logic_vector(lungh downto 1);
      diff   : out std_logic_vector(lungh downto 1));
  end component sottratoreconenable;

  component registro
    generic (
      lungh : integer);
    port (
      ck      : in  std_logic;
      reset   : in  std_logic;
      clear   : in  std_logic;
      load    : in  std_logic;
      datoin  : in  std_logic_vector(lungh downto 1);
      datoout : out std_logic_vector(lungh downto 1));
  end component registro;

  component contatore
    generic (
      lungh : integer);
    port (
      ck       : in  std_logic;
      reset    : in  std_logic;
      load     : in  std_logic;
      enable   : in  std_logic;
      iniziale : in  std_logic_vector(lungh downto 1);
      zero     : out std_logic);
  end component contatore;

  
  signal s83, s75 : std_logic;
  signal s00, s01, s02, s03, s04, s05, s06, s07, s08, s09 : std_logic;
  signal s10, s11, s12, s12b, s13, s14, s15, s16: std_logic;
  signal s17, s24, s25, s26, s81, s82, s84, s84b : std_logic;
  signal s15v, s16v, s17v, s24v, s25v, s26v, s81v, s82v : std_logic_vector(1 downto 1);
  
  signal s18, s19, s20, s21, s22, s23 : std_logic_vector(par09 downto 1);

  signal s28, s29 : std_logic_vector(par01 downto 1);
  signal s33, s34 : std_logic_vector(par02 downto 1);
  signal s36, s37 : std_logic_vector(par04 downto 1);
  signal s39 : std_logic_vector(par05 downto 1);
  signal s42, s44, s43 : std_logic_vector(par06 downto 1);

  signal s27, s27b, s30, s31, s32, s35, s38, s40, s41, s45, s46 : std_logic;

  signal s50, s51, s52, s53, s54, s55, s56, s57, s58, s59 : std_logic;
  signal s60, s61, s62, s63, s64, s65, s66, s67, s68, s69 : std_logic;
  signal s71, s72, s73, s78, s79, s80, s86 : std_logic;
  signal s80v, s67v, s68v, s86v, s85v: std_logic_vector(1 downto 1);
  signal s70: std_logic_vector(par08 downto 1);

  signal s90, s91 : std_logic_vector(par09 downto 1);
  
begin  -- architecture structural

  s83<=ck;
  s75<=reset;
  s15v(1)<=s15;
  s16v(1)<=s16;
  s17<=s17v(1);
  s24v(1)<=s24;
  s25v(1)<=s25;
  s26<=s26v(1);
  s81v(1)<=s81;
  s82<=s82v(1);
  s67v(1)<=s67;
  s68v(1)<=s68;
  s80v(1)<=s80;
  s86<=s86v(1);

  z00: cuparametrica
    port map (
      ck             => s83,
      reset          => s75,
      iszero1        => s50,
      iszero2        => s53,
      iszero3        => s56,
      iszero4        => s59,
      iszero5        => s63,
      iszero7        => s78,
      curGold        => s14,
      zeroSuper      => s72,
      newmax         => s43(par06),
      resetGold1     => s00,
      resetGold2     => s09,
      resetGold3     => s12,
      chooseGold     => s13,
      transitorio    => s84,
      enableGold     => s81,
      enableGold1a   => s01,
      enableGold1b   => s02,
      enableGold2a   => s04,
      enableGold2b   => s05,
      enableGold3    => s07,
      chooseEnable   => s10,
      chooseIniziale => s11,
      resetDesp      => s27,
      enableDesp     => s27b,
      resetAcc       => s30,
      clearCount1    => s31,
      clearCount2    => s40,
      enableAcc1     => s32,
      enableAcc2     => s41,
      enableSq       => s35,
      enableSUM      => s38,
      enableCMP      => s46,
      loadMAX        => s45,
      loadCNT1       => s52,
      loadCNT2       => s55,
      loadCNT3       => s58,
      selCNT3        => s69,            --
      loadCNT4a      => s61,
      loadCNT4b      => s62,
      loadCNT5a      => s65,
      loadCNT5b      => s66,
      loadCNT7       => s80,
      loadSuperCNT   => s71,
      enableCNT1     => s51,
      enableCNT2     => s54,
      enableCNT3     => s57,
      enableCNT4     => s60,
      enableCNT5     => s64,
      enableCNT7     => s79,
      enableSuperCNT => s73,
      resetspecial   => s08,
      finito         => fine);

  z01: myor
    port map (
      dato1  => s01,
      dato2  => s02,
      uscita => s03);

  z02: myor
    port map (
      dato1  => s04,
      dato2  => s05,
      uscita => s06);

  z03: abilitaGold
    port map (
      orologio     => s83,
      azzera       => s75,
      enableGold1  => s03,
      enableGold2  => s07,
      nuis1        => nu1,
      nuis2        => nu2,
      abilitazione => s15);

  z04: abilitaGold
    port map (
      orologio     => s83,
      azzera       => s08,
      enableGold1  => s06,
      enableGold2  => s07,
      nuis1        => nu1,
      nuis2        => nu2,
      abilitazione => s16);

  z05: gold
    generic map (
      goldlungh => par09)
    port map (
      gck      => s83,
      greset   => s00,
      en       => s15,
      ini1     => inizialeLFSR,
      ini2     => inizialeLFSR,
      seq1     => sequenzaLFSR1,
      seq2     => sequenzaLFSR2,
      cur1     => s18,
      cur2     => s19,
      goldchip => s24);

  z06: gold
    generic map (
      goldlungh => par09)
    port map (
      gck      => s83,
      greset   => s09,
      en       => s16,
      ini1     => inizialeLFSR,
      ini2     => inizialeLFSR,
      seq1     => sequenzaLFSR1,
      seq2     => sequenzaLFSR2,
      cur1     => s20,
      cur2     => s21,
      goldchip => s25);

  z07: toggler
    port map (
      ck     => s83,
      reset  => s27,
      toggle => s13,
      dato   => s14);

  z08: mpx
    generic map (
      lungh => 1)
    port map (
      dato1    => s24v,
      dato2    => s25v,
      sel      => s14,
      selected => s26v);

  z09: despreader
    generic map (
      lungh => par01)
    port map (
      dato       => datoI,
      chip       => s26,
      ck         => s83,
      reset      => s27,
      enable     => s27b,
      despreaded => s28);

  z10: despreader
    generic map (
      lungh => par01)
    port map (
      dato       => datoQ,
      chip       => s26,
      ck         => s83,
      reset      => s27,
      enable     => s27b,
      despreaded => s29);

  z11: accumulatore
    generic map (
      lunghin  => par01,
      lunghout => par02)
    port map (
      ck      => s83,
      reset   => s30,
      clrcnt  => s31,
      enable  => s32,
      dato    => s28,
      sommato => s33);

  z12: accumulatore
    generic map (
      lunghin  => par01,
      lunghout => par02)
    port map (
      ck      => s83,
      reset   => s30,
      clrcnt  => s31,
      enable  => s32,
      dato    => s29,
      sommato => s34);

  z13: squaring
    generic map (
      lunghin  => par03,
      lunghout => par04)
    port map (
      ck      => s83,
      reset   => s27,
      enable  => s35,
      dato    => s33(par02 downto (par02 -par03 +1)),
      squared => s36);

  z14: squaring
    generic map (
      lunghin  => par03,
      lunghout => par04)
    port map (
      ck      => s83,
      reset   => s27,
      enable  => s35,
      dato    => s34(par02 downto (par02 -par03 +1)),
      squared => s37);

  z15: sommatore
    generic map (
      lunghin  => par04,
      lunghout => par05)
    port map (
      ck      => s83,
      reset   => s27,
      enable  => s38,
      dato1   => s36,
      dato2   => s37,
      sommato => s39);

  z16: accumulatore
    generic map (
      lunghin  => par05,
      lunghout => par06)
    port map (
      ck      => s83,
      reset   => s30,
      clrcnt  => s40,
      enable  => s41,
      dato    => s39,
      sommato => s42);

  z17: registro
    generic map (
      lungh => par06)
    port map (
      ck      => s83,
      reset   => s27,
      clear   => '0',
      load    => s45,
      datoin  => s42,
      datoout => s44);

  z18: sottratoreconenable
    generic map (
      lungh => par06)
    port map (
      ck     => s83,
      reset  => s27,
      enable => s46,
      dato1  => s44,
      dato2  => s42,
      diff   => s43);

  z20: mpx
    generic map (
      lungh => 1)
    port map (
      dato1    => s15v,
      dato2    => s16v,
      sel      => s10,
      selected => s17v);

  z21: mpx
    generic map (
      lungh => par09)
    port map (
      dato1    => s18,
      dato2    => s20,
      sel      => s11,
      selected => s22);

  z22: mpx
    generic map (
      lungh => par09)
    port map (
      dato1    => s19,
      dato2    => s21,
      sel      => s11,
      selected => s23);

  z23: gold
    generic map (
      goldlungh => par09)
    port map (
      gck      => s83,
      greset   => s12b,
      en       => s82,
      ini1     => s22,
      ini2     => s23,
      seq1     => sequenzaLFSR1,
      seq2     => sequenzaLFSR2,
      cur1     => s90,
      cur2     => s91,
      goldchip => chipout);

  z24: nflipflop
    port map (
      ck    => s83,
      reset => s75,
      din   => s12,
      dout  => s12b);

  z25: mpx
    generic map (
      lungh => 1)
    port map (
      dato1    => s81v,
      dato2    => s17v,
      sel      => s84b,
      selected => s82v);

  z26: myor3
    port map (
      dato1  => s84,
      dato2  => nu1,
      dato3  => nu2,
      uscita => s84b);
  
  z30: mpx
    generic map (
      lungh => par08)
    port map (
      dato1    => inizialeCNT3b,
      dato2    => inizialeCNT3a,
      sel      => s69,
      selected => s70);

  z31: myor
    port map (
      dato1  => s61,
      dato2  => s62,
      uscita => s67);

  z32: myor
    port map (
      dato1  => s65,
      dato2  => s66,
      uscita => s68);

  z33: contatore
    generic map (
      lungh    => par07)
    port map (
      ck       => s83,
      reset    => s75,
      load     => s71,
      enable   => s73,
      iniziale => inizialeSuperCNT,
      zero     => s72);

  z34: mpx
    generic map (
      lungh => 1)
    port map (
      dato1    => s67v,
      dato2    => s68v,
      sel      => s10,
      selected => s85v);

  z35: mpx
    generic map (
      lungh => 1)
    port map (
      dato1    => s80v,
      dato2    => s85v,
      sel      => s84,
      selected => s86v);

  z40: contatore
    generic map (
      lungh => par08)
    port map (
      ck       => s83,
      reset    => s75,
      load     => s52,
      enable   => s51,
      iniziale => inizialeCNT1,
      zero     => s50);

  z41: contatore
    generic map (
      lungh => par08)
    port map (
      ck       => s83,
      reset    => s75,
      load     => s55,
      enable   => s54,
      iniziale => inizialeCNT2,
      zero     => s53);

  z42: contatore
    generic map (
      lungh => par08)
    port map (
      ck       => s83,
      reset    => s75,
      load     => s58,
      enable   => s57,
      iniziale => s70,
      zero     => s56);

  z43: contatore
    generic map (
      lungh => par08)
    port map (
      ck       => s83,
      reset    => s75,
      load     => s67,
      enable   => s60,
      iniziale => inizialeCNT4,
      zero     => s59);

  z44: contatore
    generic map (
      lungh => par08)
    port map (
      ck       => s83,
      reset    => s75,
      load     => s68,
      enable   => s64,
      iniziale => inizialeCNT4,
      zero     => s63);

  z45: contatore
    generic map (
      lungh => par08)
    port map (
      ck       => s83,
      reset    => s75,
      load     => s86,
      enable   => s79,
      iniziale => inizialeCNT4,
      zero     => s78);
  
end architecture structural;

