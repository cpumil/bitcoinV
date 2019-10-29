BitcoinV Core version 1.1.4 is now available from:

  <https://github.com/bitcoinVBR/bitcoinV/releases>

This is a new minor version release, with various bugfixes
as well as updated translations.

Please report bugs using the issue tracker at GitHub:

  <https://github.com/bitcoinVBR/bitcoinV/issues>


How to Upgrade
==============

If you are running an older version, shut it down. Wait until it has completely
shut down (which might take a few minutes for older versions), then run the
installer (on Windows) or just copy over `/Applications/Bitcoin-Qt` (on Mac)
or `bitcoind`/`bitcoin-qt` (on Linux).


Downgrading warning
-------------------


Compatibility
==============

Bitcoin Core is extensively tested on multiple operating systems using
the Linux kernel, macOS 10.8+, and Windows Vista and later. Windows XP is not supported.

Bitcoin Core should also work on most other Unix-like systems but is not
frequently tested on them.

1.1.4 change log
------------------

### Wallet
- Added command `-setvbrmultiplier=<n>` to the command line options. 
Miners should add this line to bitcoin.conf for VBR multiplier values:
ex: To mine for x4 block rewards
`-setvbrmultiplier=4`
NOTE: Any changes to the GUI pull down multiplier in the 'Miner' tab will override the 
      VBR multiplication, if running bitcoind, this is not a possibility.
### RPC and other APIs
- Ensure VBR multiplier plays nicely with setvbrmultiplier values

### GUI
- Ensure VBR multiplier stays inbounds.


### Build system


### Tests and QA


### Miscellaneous


Credits
=======

Thanks to everyone who directly contributed to this release:

- NullFunctor
- ReelCoders
- GeneralConsultant

