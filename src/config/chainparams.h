// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CHAINPARAMS_H
#define BITCOIN_CHAINPARAMS_H

#include "params.h"
#include "block/block.h"
#include "p2p/protocol.h"
#include "pubkey.h"

#include <memory>
#include <vector>

struct CDNSSeedData
{
    std::string host;
    bool supportsServiceBitsFiltering;

    CDNSSeedData(const std::string &strHost, bool supportsServiceBitsFilteringIn) : host(strHost),
                                                                                    supportsServiceBitsFiltering(
                                                                                            supportsServiceBitsFilteringIn)
    {
    }
};

struct SeedSpec6
{
    uint8_t addr[16];
    uint16_t port;
};

typedef std::map<int, uint256> MapCheckpoints;

struct CCheckpointData
{
    MapCheckpoints mapCheckpoints;
};

struct ChainTxData
{
    int64_t nTime;
    int64_t nTxCount;
    double dTxRate;
};

/**
 * CChainParams defines various tweakable parameters of a given instance of the
 * Bitcoin system. There are three: the main network on which people trade goods
 * and services, the public test network which gets reset from time to time and
 * a regression test mode which is intended for private networks only. It has
 * minimal difficulty to ensure that blocks can be found instantly.
 */
class CChainParams
{

public:
    /** BIP70 chain name strings (main, test or regtest) */
    static const std::string MAIN;
    static const std::string TESTNET;
    static const std::string REGTEST;


public:
    enum Base58Type
    {
        PUBKEY_ADDRESS,
        SCRIPT_ADDRESS,
        SECRET_KEY,
        EXT_PUBLIC_KEY,
        EXT_SECRET_KEY,

        MAX_BASE58_TYPES
    };

    const std::string &DataDir() const
    {
        return strDataDir;
    }

    const int RPCPort() const
    {
        return nRPCPort;
    }

    const Consensus::Params &GetConsensus() const
    {
        return consensus;
    }

    const CMessageHeader::MessageStartChars &MessageStart() const
    {
        return pchMessageStart;
    }

    int GetDefaultPort() const
    {
        return nDefaultPort;
    }

    const CBlock &GenesisBlock() const
    {
        return genesis;
    }

    /** Make miner wait to have peers to avoid wasting work */
    bool MiningRequiresPeers() const
    {
        return fMiningRequiresPeers;
    }

    /** Default value for -checkmempool and -checkblockindex argument */
    bool DefaultConsistencyChecks() const
    {
        return fDefaultConsistencyChecks;
    }

    /** Policy: Filter transactions that do not match well-defined patterns */
    bool RequireStandard() const
    {
        return fRequireStandard;
    }

    uint64_t PruneAfterHeight() const
    {
        return nPruneAfterHeight;
    }

    /** Make miner stop after a block is found. In RPC, don't return until nGenProcLimit blocks are generated */
    bool MineBlocksOnDemand() const
    {
        return fMineBlocksOnDemand;
    }

    /** Return the BIP70 network string (main, test or regtest) */
    std::string NetworkIDString() const
    {
        return strNetworkID;
    }

    const std::vector<CDNSSeedData> &DNSSeeds() const
    {
        return vSeeds;
    }

    const std::vector<unsigned char> &Base58Prefix(Base58Type type) const
    {
        return base58Prefixes[type];
    }

    const std::vector<SeedSpec6> &FixedSeeds() const
    {
        return vFixedSeeds;
    }

    const CCheckpointData &Checkpoints() const
    {
        return checkpointData;
    }

    bool AddCheckPoint(int const height, const uint256 hash) const;

    const ChainTxData &TxData() const
    {
        return chainTxData;
    }

    void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout);

    const CPubKey &GetCheckPointPKey() const
    {
        return std::move(cCheckPointPubKey);
    }

    CChainParams()
    {
    }

protected:
    int nRPCPort;
    std::string strDataDir;
    Consensus::Params consensus;
    CMessageHeader::MessageStartChars pchMessageStart;
    int nDefaultPort;
    uint64_t nPruneAfterHeight;
    std::vector<CDNSSeedData> vSeeds;
    std::vector<unsigned char> base58Prefixes[MAX_BASE58_TYPES];
    std::string strNetworkID;
    CBlock genesis;
    CPubKey cCheckPointPubKey;
    std::vector<SeedSpec6> vFixedSeeds;
    bool fMiningRequiresPeers;
    bool fDefaultConsistencyChecks;
    bool fRequireStandard;
    bool fMineBlocksOnDemand;
    mutable CCheckpointData checkpointData;
    ChainTxData chainTxData;
};

/**
 * Creates and returns a std::unique_ptr<CChainParams> of the chosen chain.
 * @returns a CChainParams* of the chosen chain.
 * @throws a std::runtime_error if the chain is not supported.
 */
std::unique_ptr<CChainParams> CreateChainParams(const std::string &chain);

std::string ChainNameFromCommandLine();

/**
 * Return the currently selected parameters. This won't change after app
 * startup, except for unit tests.
 */
const CChainParams &Params();

/**
 * Sets the params returned by Params() to those for the given BIP70 chain name.
 * @throws std::runtime_error when the chain is not supported.
 */
void SelectParams(const std::string &chain);

#endif // BITCOIN_CHAINPARAMS_H
