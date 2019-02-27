// Copyright (c) 2018 The BitcoinV Core developers
// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <consensus/merkle.h>
#include "pow.h"
#include <tinyformat.h>
#include <util.h>
#include <utilstrencodings.h>

#include <assert.h>

#include <chainparamsseeds.h>


static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
// $ date --date='@1544800000'
// Fri Dec 14 09:06:40 CST 2018

static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{  
    const char* pszTimestamp = "Keep the little guys mining for the JACKPOT, here is bitcoinV - 14/Dec/2018";
    const CScript genesisOutputScript = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
   return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 210000; // #blocks before halving occurs. every 4 years
        consensus.BIP16Exception = uint256S("0x00000000000002dc756eebf4f49723ed8d30cc28a5f108eb94b1ba88ac4f9c22");
        consensus.BIP34Height = 22;
        consensus.BIP34Hash = uint256S("0x000000000000024b89b42a942fe0d9fea3bb44ab7bd1b19115dd6a759c0808b8");
        consensus.BIP65Height = 38; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        consensus.BIP66Height = 36; // 00000000000000000379eaa19dce8c9b722d46ae6a57c2f1a988119488b50931
        consensus.powLimit = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 8 * 60 * 60;  // 8 hours, difficulty update time in seconds
        consensus.nPowTargetSpacing = 10 * 60; // block time in minutes
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.nRuleChangeActivationThreshold = (uint32_t)(0.95*consensus.nMinerConfirmationWindow); // segwit activation after this # blocks

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1462060800; // May 1st, 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1493596800; // May 1st, 2017

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // nMinimumChainWork is a value that is updated at every release. It is retrieved from the Debug command
        //            getblockchaininfo 
        // RPC of a node that is up at the time of release. nMinimumChainWork is updated at the same time as assumevalid. 
        // It is calculated by summing the work done in each block which is calculated by doing 2^256/(target+1)

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0000000000000000000000000000000000000000000000000001c051c051c050");
                                                
        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("00000000000a402a29b8e86414552bf6d7f0c5026d59fc0147fdcc48e5fbcf8c");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xf5;
        pchMessageStart[1] = 0xb4;
        pchMessageStart[2] = 0xf3;
        pchMessageStart[3] = 0xa2;
        nDefaultPort = 9333;
        nPruneAfterHeight = 100000;

        uint32_t nTime=1544800001;
        uint32_t nNonce=771851678;
       
        // @"consensus.hashGenesisBlock: 000000004c2738ff52ee6dc039d4fde2f3292fed9afa9d712f895d7094f8d350\r\n"
        // @"genesis.hashMerkleRoot: c36c4216baf256beb34d939e7aa158a54b7488be996e8bdab8d83ff9c73f1f4d\r\n"
        // @"genesis.nNonce: 771851678\r\n"
        // Difficulty bits:
        // Using following formula target can be obtained from any block. For example if a target packed in a block appears as 0x1b0404cb its hexadecimal version will look as following:
        // 0x0404cb * 2**(8*(0x1b - 3)) = 0x00000000000404CB000000000000000000000000000000000000000000000000
        bool proof_of_work_valid = false;
        for (nTime=1544800001; ;nTime++)
        {
            genesis = CreateGenesisBlock(nTime, nNonce, 0x1d00ffff, 1, 50 * COIN);

            for (genesis.nNonce = 771851678; genesis.nNonce <= 0xfffffff0;genesis.nNonce++)
            {
                proof_of_work_valid = CheckProofOfWork(genesis.GetHash(), genesis.nBits, consensus);
                if (proof_of_work_valid) {
                    break;
                }
            }
            if (proof_of_work_valid) {
                break;
            }

            std::cout << "time: " << nTime << std::endl;
        }

        
        consensus.hashGenesisBlock = genesis.GetHash();
        std::cout << "consensus.hashGenesisBlock: " << consensus.hashGenesisBlock.ToString() << std::endl;
        std::cout << "genesis.hashMerkleRoot: " << genesis.hashMerkleRoot.ToString() << std::endl;   
        std::cout << "genesis.nNonce: " << genesis.nNonce << std::endl;

                                                         
        assert(consensus.hashGenesisBlock == uint256S("000000004c2738ff52ee6dc039d4fde2f3292fed9afa9d712f895d7094f8d350"));
        assert(genesis.hashMerkleRoot == uint256S("c36c4216baf256beb34d939e7aa158a54b7488be996e8bdab8d83ff9c73f1f4d"));


        // The domains listed in chainparams.cpp are for DNS seeders. DNS seeders are not nodes themselves, 
        // but rather are DNS servers that serve the IP addresses of nodes that are available to be connected 
        // to. These can be connected to for both a normal connection and just one to retrieve more IP addresses 
        // of nodes. These are only used for first time boot up.
        vSeeds.emplace_back("seed1.bitcoinv.org");
        vSeeds.emplace_back("seed2.bitcoinv.org");
        vSeeds.emplace_back("seed3.bitcoinv.org");
        vSeeds.emplace_back("seed4.bitcoinv.org");
        vSeeds.emplace_back("seed5.bitcoinv.org");
        vSeeds.emplace_back("seed6.bitcoinv.org");
        vSeeds.emplace_back("seed7.bitcoinv.org");
        vSeeds.emplace_back("seed8.bitcoinv.org");

        vSeeds.emplace_back("seed1.bitcoinv.io");
        vSeeds.emplace_back("seed2.bitcoinv.io");
        vSeeds.emplace_back("seed3.bitcoinv.io");
        vSeeds.emplace_back("seed4.bitcoinv.io");

        vSeeds.emplace_back("seed1.bitcoinvbr.org");
        vSeeds.emplace_back("seed2.bitcoinvbr.org");
        vSeeds.emplace_back("seed3.bitcoinvbr.org");
        vSeeds.emplace_back("seed4.bitcoinvbr.org");

        vSeeds.emplace_back("seed1.bitcoinvbr.com");
        vSeeds.emplace_back("seed2.bitcoinvbr.com");
        vSeeds.emplace_back("seed3.bitcoinvbr.com");
        vSeeds.emplace_back("seed4.bitcoinvbr.com");


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,0);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        bech32_hrp = "bc";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = {
            {
                {0,     uint256S("000000004c2738ff52ee6dc039d4fde2f3292fed9afa9d712f895d7094f8d350")},
                {100,   uint256S("00000000015488a23508da3eec027eaa670122e9532251091c706f00e97374f6")},
                {200,   uint256S("0000000004768f1061608e1c060ed5cb106cc40e8f83513293059b0432267fef")},
                {300,   uint256S("0000000000ca1d3601bce417ef0024f29f68b8e81a6affd1368227345ee24a90")},
                {395,   uint256S("00000000000a402a29b8e86414552bf6d7f0c5026d59fc0147fdcc48e5fbcf8c")},

            }
        };

        chainTxData = ChainTxData{
            // type     getchaintxstats    in debug console
            /* nTime    */ 1544800001,
            /* nTxCount */ 1,
            /* dTxRate  */ 0.9900000
        };

        /* disable fallback fee on mainnet */
        m_fallback_fee_enabled = false;
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 210000; // #blocks before halving occurs. every 4 years
        consensus.BIP16Exception = uint256S("0x00000000000002dc756eebf4f49723ed8d30cc28a5f108eb94b1ba88ac4f9c22");
        consensus.BIP34Height = 227931;
        consensus.BIP34Hash = uint256S("0x000000000000024b89b42a942fe0d9fea3bb44ab7bd1b19115dd6a759c0808b8");
        consensus.BIP65Height = 388381; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        consensus.BIP66Height = 363725; // 00000000000000000379eaa19dce8c9b722d46ae6a57c2f1a988119488b50931
        consensus.powLimit = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 8 * 60 * 60; // 8 hours // difficulty update time in minutes
        consensus.nPowTargetSpacing = 10 * 60; // block time in seconds
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.nRuleChangeActivationThreshold = (uint32_t)(0.95*consensus.nMinerConfirmationWindow);

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1462060800; // May 1st, 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1493596800; // May 1st, 2017

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1479168000; // November 15th, 2016.
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1510704000; // November 15th, 2017.

        // nMinimumChainWork is a value that is updated at every release. It is retrieved from the Debug command
        //            getblockchaininfo 
        // RPC of a node that is up at the time of release. nMinimumChainWork is updated at the same time as assumevalid. 
        // It is calculated by summing the work done in each block which is calculated by doing 2^256/(target+1)

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0000000000000000000000000000000000000000000000000000000000010001");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("00009a004b86c066b21aaffe2325a2bf5cb80ccf572c137cb24086cc83ca0542");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xf5;
        pchMessageStart[1] = 0xb4;
        pchMessageStart[2] = 0xf3;
        pchMessageStart[3] = 0xa2;
        nDefaultPort = 19333;
        nPruneAfterHeight = 100000;

        uint32_t nTime=1544800001;
        uint32_t nNonce=771851678;
       
        // @"consensus.hashGenesisBlock: 000000004c2738ff52ee6dc039d4fde2f3292fed9afa9d712f895d7094f8d350\r\n"
        // @"genesis.hashMerkleRoot: c36c4216baf256beb34d939e7aa158a54b7488be996e8bdab8d83ff9c73f1f4d\r\n"
        // @"genesis.nNonce: 771851678\r\n"
        // Difficulty bits:
        // Using following formula target can be obtained from any block. For example if a target packed in a block appears as 0x1b0404cb its hexadecimal version will look as following:
        // 0x0404cb * 2**(8*(0x1b - 3)) = 0x00000000000404CB000000000000000000000000000000000000000000000000
        bool proof_of_work_valid = false;
        for (nTime=1544800001; ;nTime++)
        {
            genesis = CreateGenesisBlock(nTime, nNonce, 0x1d00ffff, 1, 50 * COIN);

            for (genesis.nNonce = 771851678; genesis.nNonce <= 0xfffffff0;genesis.nNonce++)
            {
                proof_of_work_valid = CheckProofOfWork(genesis.GetHash(), genesis.nBits, consensus);
                if (proof_of_work_valid) {
                    break;
                }
            }
            if (proof_of_work_valid) {
                break;
            }

            std::cout << "time: " << nTime << std::endl;
        }

        
        consensus.hashGenesisBlock = genesis.GetHash();
        std::cout << "consensus.hashGenesisBlock: " << consensus.hashGenesisBlock.ToString() << std::endl;
        std::cout << "genesis.hashMerkleRoot: " << genesis.hashMerkleRoot.ToString() << std::endl;   
        std::cout << "genesis.nNonce: " << genesis.nNonce << std::endl;

                                                         
        assert(consensus.hashGenesisBlock == uint256S("000000004c2738ff52ee6dc039d4fde2f3292fed9afa9d712f895d7094f8d350"));
        assert(genesis.hashMerkleRoot == uint256S("c36c4216baf256beb34d939e7aa158a54b7488be996e8bdab8d83ff9c73f1f4d"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("testnet-seed.bitcoin.jonasschnelli.ch");
        vSeeds.emplace_back("seed.tbtc.petertodd.org");
        vSeeds.emplace_back("seed.testnet.bitcoin.sprovoost.nl");
        vSeeds.emplace_back("testnet-seed.bluematt.me"); // Just a static list of stable node(s), only supports x9

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "tb";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;


        checkpointData = {
            {
                {1,   uint256S("0000000009d5004afaff8c02e03e5d2a50bfb8ea84d58815d7caf618da33c11b")},
                {100, uint256S("000000000296f49e82cee9167801e00782c6deef9925e140d5e89f934646560c")},
                {200, uint256S("00000000000277e52ab19e9b96d0af69e2786e35bf3ff467e55049477efe126c")},
                {300, uint256S("00000000004da6e4818b5fc51a0a6ddaada08dd653f9495bfab9a4eadc6b8a06")},
                {400, uint256S("00000000000619c2c233de1cbf758fa38cb9ac6721533056761ae9092ceb22a3")},
                {500, uint256S("0000000000006fd1f1c6b63ef25e7363b1705c9ea624d94c8a7539aaa82a5858")},
            }
        };

        chainTxData = ChainTxData{
            // Data from rpc: getchaintxstats 4096 0000000000000037a8cd3e06cd5edbfe9dd1dbcc5dacab279376ef7cfc2b4c75
            /* nTime    */ 1531929919,
            /* nTxCount */ 19438708,
            /* dTxRate  */ 0.626
        };

        /* enable fallback fee on testnet */
        m_fallback_fee_enabled = true;
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP16Exception = uint256();
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 18444;
        nPruneAfterHeight = 1000;

        uint32_t nTime=1544800000;
        uint32_t nNonce=485334016;
        
        // Difficulty bits:
        // Using following formula target can be obtained from any block. For example if a target packed in a block appears as 0x1b0404cb its hexadecimal version will look as following:
        // 0x0404cb * 2**(8*(0x1b - 3)) = 0x00000000000404CB000000000000000000000000000000000000000000000000

        genesis = CreateGenesisBlock(nTime, nNonce, 0x1f00ffff, 1, 50 * COIN);

        // while(!CheckProofOfWork(genesis.GetHash(), genesis.nBits, consensus)){ 
        //     ++genesis.nNonce;          
        // }


        consensus.hashGenesisBlock = genesis.GetHash();
        std::cout << "consensus.hashGenesisBlock: " << consensus.hashGenesisBlock.ToString() << std::endl;
        std::cout << "genesis.hashMerkleRoot: " << genesis.hashMerkleRoot.ToString() << std::endl;   
        std::cout << "genesis.nNonce: " << genesis.nNonce << std::endl;
        // consensus.hashGenesisBlock: c12a6d0e08a807bbdcfef151cdcb6e2f7c5d6ac66f6e1de22e1c950e25c6a183
        // genesis.hashMerkleRoot:     c41041da878b479cd4e0537bf00525f1738c42e36e30b9214a7bfa7358fe89d0
                                                         
        assert(consensus.hashGenesisBlock == uint256S("0x00009a004b86c066b21aaffe2325a2bf5cb80ccf572c137cb24086cc83ca0542"));
        assert(genesis.hashMerkleRoot == uint256S("0xc41041da878b479cd4e0537bf00525f1738c42e36e30b9214a7bfa7358fe89d0"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = {
            {
                {0, uint256S("0x00009a004b86c066b21aaffe2325a2bf5cb80ccf572c137cb24086cc83ca0542")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "bcrt";

        /* enable fallback fee on regtest */
        m_fallback_fee_enabled = true;
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
